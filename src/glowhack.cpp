#include "glowhack.hpp"
#include "remote_process.hpp"
#include "pattern_scanner.hpp"
#include <iostream>

glowhack::glowhack(remote_process const &csgo_process) : csgo_process{csgo_process}, csgo_pattern_scanner{pattern_scanner(this->csgo_process)}    
{
    auto x = pattern_scanner(this->csgo_process);
}
glowhack::~glowhack()
{

}
int glowhack::run()
{
    uintptr_t get_glow_mgr_pointer_function_call {csgo_pattern_scanner.find_pattern_in_module_with_name("client_client.so","48 89 f3 e8 ?? ?? ?? ?? 41 89 c4 e8") + 0x9};
    uintptr_t get_glow_mgr_pointer_function {csgo_pattern_scanner.get_relative_call_address(get_glow_mgr_pointer_function_call)};
    uintptr_t glow_manager {0};
    bool success { csgo_process.read_single(get_glow_mgr_pointer_function + 0xc, sizeof(glow_manager), &glow_manager) };

}