#include "remote_process_interface.hpp"

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
remote_process::remote_process(std::string proc_name) : proc_id{remote_process::get_remote_process_id_by_name(proc_name)}
{
    if (!proc_id)
    {
        std::cerr << "proc_name: " << proc_name << "\n";
        throw std::invalid_argument("proc_id == 0 in remote_process::remote_process(std::string)");
    }
    this->regions = this->parse_maps();
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
            region_result.push_back(region);
        }
    }
    return region_result;
}
uint32_t remote_process::get_remote_process_id_by_name(std::string proc_name)
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
                std::string exe_name = remote_process::get_symlink_target(path_to_exe);
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
std::string remote_process::get_symlink_target(std::string target)
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
bool remote_process::read_single(uintptr_t address, size_t size, char *buf_out) const
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
bool remote_process::write_single(uintptr_t address, size_t size, char *buf_in) const
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
uintptr_t remote_process::find_pattern_in_module_with_name(std::string module_name, std::string data, std::string pattern) const
{
    memory_region const &module = [&]() -> memory_region const & {
        for (auto &region : this->regions)
        {
            if (region.file_name == module_name)
            {
                return region;
            }
        }
        std::cerr << "module_name: " << module_name << "\n";
        throw std::runtime_error("memory region not found in remote_process::find_pattern_in_module_with_name");
    }();
    std::cout<<std::hex<<"start: "<<module.start<<" end: "<<module.end<<std::endl;
    uintptr_t found_address{this->find_pattern_in_module(module, data, pattern)};
    return found_address;
}
//converts "e5??d9??ff??"" to {0xe5,-1,0xd9,-1,0xff,-1}
//?? and -1 represent wildcard
std::vector<short> convert_ida_style_pattern(std::string pattern)
{
    size_t length {pattern.length()};
    if(length%2!=0)
    {
        std::cerr<<"pattern: " <<pattern<<"\n";
        throw std::invalid_argument("invalid pattern (odd length of characters) in convert_ida_style_pattern");
    }
    std::vector<short> pattern_bytes;
    pattern_bytes.reserve(length/2);
    if (pattern.find_first_of(" ") == std::string::npos)
    {
        for (int i = 0; i < length / 2; i++)
        {
            std::string const &byte_str{pattern.substr(i, 2)};
            short byte_or_wildcard{byte_str == "??" ? -1 : std::stoul(byte_str)};
            pattern_bytes.push_back(byte_or_wildcard);
        }
    }
    else
    {
        while(size_t pattern.find_first_not_of()
    }
}
uintptr_t remote_process::find_pattern_in_module(memory_region const &module, std::string data, std::string pattern, size_t offset) const
{
    auto pat = convert_ida_style_pattern("ff??ff??ff??e5");
    char buffer[0x1000];
    size_t pattern_len{data.length()};
    size_t chunk_size{sizeof(buffer)};
    size_t total_size{module.end - module.start};
    size_t chunk_it{0};
    while (total_size)
    {
        size_t readsize = (total_size < chunk_size) ? total_size : chunk_size;
        uintptr_t readaddr = module.start + offset + (chunk_size * chunk_it);

        memset(buffer, 0, chunk_size);

        if (this->read_single(readaddr, readsize, buffer))
        {
            for (uintptr_t b = 0; b < readsize; b++)
            {
                size_t matches = 0;

                while (buffer[b + matches] == data[matches] || data[matches] != 'x')
                {
                    matches++;

                    if (matches == pattern_len)
                    {
                        return (readaddr + b);
                    }
                }
            }
        }

        total_size -= readsize;
        chunk_it++;
    }
    //if we didn't find pattern (hence not returned above), try again with offset to catch patterns in the middle of pages
    if(offset==0)
    {
        std::cout<<"got here"<<std::endl;
        return remote_process::find_pattern_in_module(module, data, pattern, chunk_size/2);
    }
    //if we tried search with offset
    else
    {
        throw std::runtime_error("did not find pattern in remote_process::find_pattern_in_module");
    }
    std::cout<<"got here"<<std::endl;
}