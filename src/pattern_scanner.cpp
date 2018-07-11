#include "pattern_scanner.hpp"
#include <iostream>
#include <array>
#include <algorithm>
#include <cstring>

pattern_scanner::pattern_scanner(remote_process const &proc) : proc{proc}
{

}
static std::vector<pattern_byte> convert_ida_style_pattern(std::string pattern);

uintptr_t pattern_scanner::find_pattern_in_module_with_name(std::string module_name, std::string data) const
{
    memory_region const &module = this->proc.get_memory_region_by_name(module_name);
    std::cout<<std::hex<<"start: "<<module.start<<" end: "<<module.end<<std::endl;
    uintptr_t found_address{this->find_pattern_in_module(module, data)};
    return found_address;
}
///converts "e5??d9??ff??" or "e5 ?? d9 ?? ff ??" or "e 5 ? ? d 9 ? ? f f ? ?" (spaces are removed) to {0xe5,-1,0xd9,-1,0xff,-1}
///?? and -1 represent wildcard
static std::vector<pattern_byte> convert_ida_style_pattern(std::string pattern)
{
    std::array<char, 23> constexpr byte_or_wildcard_chars {'0','1','2','3','4','5','6','7','8','9','A','a','B','b','C','c','D','d','E','e','F','f','?'};
    pattern.erase( //remove chars not part of hex or wildcard (spaces, etc)
        std::remove_if(pattern.begin(), pattern.end(), [&byte_or_wildcard_chars](char x)
            {   
                return std::find(byte_or_wildcard_chars.begin(), byte_or_wildcard_chars.end(), x) == byte_or_wildcard_chars.end(); // if char x not part of hex or wildcard (?), then remove it
            }
        ), 
        pattern.end()
    );
    size_t length { pattern.length() };
    if(length%2!=0)
    {
        std::cerr<<"pattern: " <<pattern<<"\n";
        throw std::invalid_argument("invalid pattern (odd length of characters) in convert_ida_style_pattern");
    }
    std::vector<pattern_byte> pattern_bytes;
    pattern_bytes.reserve(length/2);
    for (int i = 0; i < length; i += 2)
    {
        std::string const &byte_str{pattern.substr(i, 2)};
        pattern_byte byte_or_wildcard{ byte_str == "??" ? pattern_byte(pattern_byte_action::wildcard) : 
            pattern_byte(pattern_byte_action::matching, static_cast<char>(std::stoul(byte_str, nullptr, 16)))
            };
        pattern_bytes.push_back(byte_or_wildcard);
    }
    return pattern_bytes;
}
uintptr_t pattern_scanner::find_pattern_in_module(memory_region const &module, std::string data, size_t offset) const
{
    std::vector<pattern_byte> pat = convert_ida_style_pattern(data);
    constexpr int page_size {0x1000};
    char read_buffer[page_size] {};
    size_t pattern_length {pat.size()};
    size_t chunk_size {sizeof(read_buffer)};
    size_t total_size {module.end - module.start};
    for (size_t chunk_it{0}, read_size; total_size > 0; total_size -= read_size, chunk_it++)
    {
        read_size = (total_size < chunk_size) ? total_size : chunk_size;
        uintptr_t readaddr = module.start + offset + (chunk_size * chunk_it);
        memset(read_buffer, 0, chunk_size);
        if (this->proc.read_single(readaddr, read_size, read_buffer))
        {
            for (uintptr_t b = 0; b < read_size; b++)
            {
                size_t matches = 0;

                while (pat[matches].action == pattern_byte_action::wildcard || read_buffer[b + matches] == pat[matches].byte)
                {
                    matches++;

                    if (matches == pattern_length)
                    {
                        return (readaddr + b);
                    }
                }
            }
        }
    }
    //if we didn't find pattern (hence not returned above), try again with offset to catch patterns in the middle of pages
    if(offset==0)
    {
        return this->find_pattern_in_module(module, data, chunk_size/2);
    }
    //if we tried search with offset
    else
    {
        std::cerr<<"pattern: " <<data<<"\n";
        throw std::runtime_error("did not find pattern in remote_process::find_pattern_in_module");
    }
}
uintptr_t pattern_scanner::get_relative_call_address(uintptr_t call_begin) const
{
    uintptr_t code {0};
    call_begin += 1;//skip the call op, go to read the relative address
    if (this->proc.read_single(call_begin, 4, &code))
    {
        return code + call_begin + 4;
    }

    return 0;
}