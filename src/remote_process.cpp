#include "remote_process.hpp"
#include <algorithm>
#include <array>
#include <sstream>
#include <cstring>
#include <unistd.h> 
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <stdexcept>
#include <iostream>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <cstdint>
static std::string get_symlink_target(std::string target);
static bool parse_maps_line(std::string line, memory_region &memory_region_out);
static int32_t get_remote_process_id_by_name(std::string proc_name);

remote_process::remote_process()
{
    this->regions = this->parse_maps();
}
remote_process::~remote_process()
{
}
remote_process::remote_process(uint32_t proc_id) : proc_id{proc_id}
{
    if (!proc_id)
    {
        throw std::invalid_argument("proc_id == 0 in remote_process::remote_process(uint32_t)");
    }
    this->regions = this->parse_maps();
}
remote_process::remote_process(std::string proc_name) : proc_id{::get_remote_process_id_by_name(proc_name)}
{
    if (!proc_id)
    {
        std::cerr << "proc_name: " << proc_name << "\n";
        throw std::invalid_argument("proc_id == 0 in remote_process::remote_process(std::string)");
    }
    this->regions = this->parse_maps();
}
static bool parse_maps_line(std::string line, memory_region &memory_region_out)
{
    bool result = false;
    std::istringstream map_line{line};
    /* memory map line:
        columns (space delimited): memory_space permission offset device inode path_name
        7f12c1720000-7f12c19af000 rw-p 017d5000 08:06 14946590                   /home/jalius/.steam/steamapps/common/Counter-Strike Global Offensive/csgo/bin/linux64/client_client.so
        */
    std::string memory_space, permissions, offset, device, inode;
    if (map_line >> memory_space >> permissions >> offset >> device >> inode)
    {
        std::string path_name;

        for (size_t ls{0}, i{0}; i < line.length(); i++)
        {
            if (line.substr(i, 1) == " ")
            {
                ls++;

                if (ls == 5)
                {
                    size_t begin{line.substr(i, line.size()).find_first_not_of(' ')};

                    if (begin != -1)
                    {
                        path_name = line.substr(begin + i, line.size());
                    }
                    else
                    {
                        path_name.clear();
                    }
                }
            }
        }

        memory_region region;

        size_t memory_split{memory_space.find_first_of('-')};
        size_t device_split{device.find_first_of(':')};

        std::stringstream ss;

        if (memory_split != -1)
        {
            ss << std::hex << memory_space.substr(0, memory_split);
            ss >> region.start;
            ss.clear();
            ss << std::hex << memory_space.substr(memory_split + 1, memory_space.size());
            ss >> region.end;
            ss.clear();
        }

        if (device_split != -1)
        {
            ss << std::hex << device.substr(0, device_split);
            ss >> region.device_major;
            ss.clear();
            ss << std::hex << device.substr(device_split + 1, device.size());
            ss >> region.device_minor;
            ss.clear();
        }

        ss << std::hex << offset;
        ss >> region.offset;
        ss.clear();
        ss << inode;
        ss >> region.inode_file_num;

        region.readable = (permissions[0] == 'r');
        region.writable = (permissions[1] == 'w');
        region.executable = (permissions[2] == 'x');
        region.shared = (permissions[3] != '-');

        if (!path_name.empty())
        {
            region.path_name = path_name;

            size_t file_name_split{path_name.find_last_of('/')};

            if (file_name_split != -1)
            {
                region.file_name = path_name.substr(file_name_split + 1, path_name.size());
            }
        }
        memory_region_out = region;
        result = true;
    }
    return result;
}
static int32_t get_remote_process_id_by_name(std::string proc_name)
{
    if (proc_name.empty())
    {
        throw std::runtime_error("empty process name in remote_process::get_remote_process_id_by_name");
    }
    uint32_t pid{0};
    DIR *dirp{opendir("/proc")};
    if (dirp != NULL)
    {
        //get the next directory in /proc
        struct dirent *dir_ent;
        while (pid == 0 && (dir_ent = readdir(dirp)))
        {
            int id{atoi(dir_ent->d_name)};
            if (id > 0)
            {
                std::string path_to_exe = std::string("/proc/") + dir_ent->d_name + "/exe";
                // /proc/{pid}/exe is a symlink to the exe file of the process {pid}. this has the name of the exe file which we need to check.
                std::string exe_name = ::get_symlink_target(path_to_exe);
                if (!exe_name.empty())
                {
                    size_t pos{exe_name.rfind('/')};
                    if (pos != std::string::npos)
                    {
                        exe_name = exe_name.substr(pos + 1);
                        if (exe_name == proc_name)
                        {
                            pid = id;
                        }
                    }
                }
            }
        }
    }
    if (!pid)
    {
        throw std::runtime_error("process id not found in remote_process::get_remote_process_id_by_name");
    }
    closedir(dirp);
    return pid;
}
static std::string get_symlink_target(std::string target)
{
    char buf[PATH_MAX];

    ssize_t len{::readlink(target.c_str(), buf, sizeof(buf) - 1)};

    if (len != -1)
    {
        buf[len] = '\0';

        return std::string(buf);
    }

    return std::string();
}
std::vector<memory_region> remote_process::parse_maps()
{
    if (!this->proc_id)
    {
        throw std::runtime_error("proc_id == 0 in remote_process::parse_maps()");
    }

    std::ifstream maps{"/proc/" + std::to_string(this->proc_id) + "/maps"};

    std::vector<memory_region> region_result;
    std::string line;
    while (std::getline(maps, line))
    {
        memory_region region;
        if(::parse_maps_line(line, region))
        {
            std::cout<<"parsed maps line: " << region.file_name<<std::endl;
            region_result.push_back(region);
        }
    }
    return region_result;
}
bool remote_process::read_single(uintptr_t address, size_t size, void *buf_out) const
{
    iovec liov;
    liov.iov_base = buf_out;
    liov.iov_len = size;
    iovec riov;
    riov.iov_base = reinterpret_cast<void *>(address);
    riov.iov_len = size;
    ssize_t bytes_written{process_vm_readv(this->proc_id, &liov, 1, &riov, 1, 0)};
    if (bytes_written == -1)
    {
        std::cerr << "err: " << strerror(errno) << "address: " << address << " size: " << size << "\n";
        throw std::runtime_error("process_vm_readv() error in remote_process::read_single(uintptr_t, size_t, char *)");
    }
    return bytes_written == size;
}
bool remote_process::write_single(uintptr_t address, size_t size, void *buf_in) const
{
    iovec liov;
    liov.iov_base = buf_in;
    liov.iov_len = size;
    iovec riov;
    riov.iov_base = reinterpret_cast<void *>(address);
    riov.iov_len = size;
    ssize_t bytes_written{process_vm_writev(this->proc_id, &liov, 1, &riov, 1, 0)};
    if (bytes_written == -1)
    {
        std::cerr << "err: " << strerror(errno) << "address: " << address << " size: " << size << "\n";
        throw std::runtime_error("process_vm_writev() error in remote_process::write_single(uintptr_t, size_t, char *)");
    }
    return bytes_written == size;
}
bool remote_process::read_multi(std::vector<iovec> const &local_iov, std::vector<iovec> const &remote_iov) const
{
    size_t local_bytes{0};
    for (auto &local_iov_element : local_iov)
    {
        local_bytes += local_iov_element.iov_len;
    }
    size_t remote_bytes{0};
    for (auto &remote_iov_element : remote_iov)
    {
        remote_bytes += remote_iov_element.iov_len;
    }
    if (local_bytes != remote_bytes)
    {
        std::cerr << "local_bytes: " << local_bytes << " remote_bytes: " << remote_bytes << "\n";
        throw std::invalid_argument("size of local buffer should be the same size as remote buffer we are reading");
    }
    ssize_t bytes_read{process_vm_readv(this->proc_id, &local_iov[0], local_iov.size(), &remote_iov[0], remote_iov.size(), 0)};
    return bytes_read == remote_bytes;
}
bool remote_process::write_multi(std::vector<iovec> const &local_iov, std::vector<iovec> const &remote_iov) const
{
    size_t local_bytes{0};
    for (auto &local_iov_element : local_iov)
    {
        local_bytes += local_iov_element.iov_len;
    }
    size_t remote_bytes{0};
    for (auto &remote_iov_element : remote_iov)
    {
        remote_bytes += remote_iov_element.iov_len;
    }
    if (local_bytes != remote_bytes)
    {
        std::cerr << "local_bytes: " << local_bytes << " remote_bytes: " << remote_bytes << "\n";
        throw std::invalid_argument("size of local buffer should be the same size as remote buffer we are writing");
    }
    ssize_t bytes_written{process_vm_writev(this->proc_id, &local_iov[0], local_iov.size(), &remote_iov[0], remote_iov.size(), 0)};
    return bytes_written == remote_bytes;
}
memory_region const remote_process::get_memory_region_by_name(std::string module_name) const
{
    for (auto region : this->regions)
    {
        if (region.file_name == module_name)
        {
            return region;
        }
    }
    std::cerr << "module_name: " << module_name << "\n";
    throw std::runtime_error("memory region not found in remote_process::get_memory_region_by_name");
}