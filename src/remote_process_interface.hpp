#ifndef REMOTE_PROCESS_INTERFACE_HPP
#define REMOTE_PROCESS_INTERFACE_HPP
#include <string>
#include <vector>
#include <sys/uio.h>


enum class pattern_byte_action : int
{
    matching = 0,
    wildcard = 1
};
struct pattern_byte
{
    pattern_byte(char byte) : 
    byte{byte}, action{pattern_byte_action::matching} 
    {}
    pattern_byte(pattern_byte_action action, char byte = 0) : 
    action{action}, byte{byte} 
    {}
    pattern_byte_action action;
    char byte;
};
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
//TODO: Too much going on here. Leave the reading and writing to this class, and let's port some of the higher level functions to another class.
public:
    remote_process();
    remote_process(uint32_t proc_id);
    remote_process(std::string proc_name);
    ~remote_process();
    bool read_multi(std::vector<iovec> const &local_iov, std::vector<iovec> const &remote_iov) const;
    bool write_multi(std::vector<iovec> const &local_iov, std::vector<iovec> const &remote_iov) const;
    bool read_single(uintptr_t address, size_t size, void *buf_out) const;
    bool write_single(uintptr_t address, size_t size, void *buf_in) const;
    uintptr_t find_pattern_in_module_with_name(std::string module_name, std::string data) const;
    uintptr_t get_relative_call_address(uintptr_t call_begin) const;
private:
    std::vector<memory_region> parse_maps();
    const uint32_t proc_id {0};
    std::vector<memory_region> regions;
    static uint32_t get_remote_process_id_by_name(std::string proc_name);
    static std::string get_symlink_target(std::string target);
    static std::vector<pattern_byte> convert_ida_style_pattern(std::string pattern);
    uintptr_t find_pattern_in_module(memory_region const &module, std::string data, size_t offset = 0) const;
    bool parse_maps_line(std::string line, memory_region &memory_region_out);
};
#endif
