#ifndef PATTERN_SCANNER_CPP
#define PATTERN_SCANNER_CPP

#include "remote_process.hpp"

class pattern_scanner
{
public:
    pattern_scanner(remote_process const &proc);
    uintptr_t find_pattern_in_module_with_name(std::string module_name, std::string data) const;
    uintptr_t get_relative_call_address(uintptr_t call_begin) const;
private:
    remote_process const &proc;
    uintptr_t find_pattern_in_module(memory_region const &module, std::string data, size_t offset = 0) const;
};
#endif