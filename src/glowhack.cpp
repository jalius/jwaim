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
    /*std::cout<<std::hex<<"sizeof(GlowObjectDefinition_t) "<<sizeof(GlowObjectDefinition_t)<<std::endl;
    std::cout<<"sizeof(CGlowObjectManager) "<<sizeof(CGlowObjectManager)<<std::endl;
    std::cout<<"sizeof(CUtlVector) "<<sizeof(CUtlVector<int>)<<std::endl;*/
    uintptr_t glow_call {csgo_pattern_scanner.find_pattern_in_module_with_name("client_client.so","E8????????488b3d????????BE01??????C7")};
    uintptr_t glow_call_operand {csgo_pattern_scanner.get_relative_call_address(glow_call)};
    uintptr_t glow_manager {csgo_pattern_scanner.get_relative_call_address(glow_call_operand + 0xf)};
    for(bool end {false};!end;)
    {
    }
}