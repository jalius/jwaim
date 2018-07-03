#include <cstdint>
#include <stdexcept>
#include <iostream>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <vector>
#include <string>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h> 
#include <sys/uio.h>
#include <vector>
#include <sstream>
#include <cstring>

class memory_region
{
private:
		// Memory
		uintptr_t start;
		uintptr_t end;

		// Permissions
		bool readable;
		bool writable;
		bool executable;
		bool shared;

		// File data
		ptrdiff_t offset;
		unsigned char device_major;
		unsigned char device_minor;
		unsigned long inode_file_num;
		std::string path_name;
		std::string file_name;
friend class remote_process;
};
class remote_process
{
public:
    remote_process();
    remote_process(uint32_t proc_id);
    remote_process(std::string proc_name);
    ~remote_process();
    bool read_multi(std::vector<iovec> const &local_iov, std::vector<iovec> const &remote_iov) const;
    bool write_multi(std::vector<iovec> const &local_iov, std::vector<iovec> const &remote_iov) const;
    bool read_single(uintptr_t address, size_t size, char *buf_out) const;
    bool write_single(uintptr_t address, size_t size, char *buf_in) const;
    uintptr_t find_pattern_in_module_with_name(std::string module_name, std::string data, std::string pattern) const;
private:
    std::vector<memory_region> parse_maps();
    const uint32_t proc_id {0};
    std::vector<memory_region> regions;
    static uint32_t get_remote_process_id_by_name(std::string proc_name);
    static std::string get_symlink_target(std::string target);
    uintptr_t find_pattern_in_module(memory_region const &module, std::string data, std::string pattern, size_t offset = 0) const;
};
