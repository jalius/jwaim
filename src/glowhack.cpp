#include "glowhack.hpp"
#include "remote_process_interface.hpp"
#include <iostream>

glowhack::glowhack(remote_process const &csgo_process) : csgo_process{csgo_process}
{

}
glowhack::~glowhack()
{

}
int glowhack::run()
{
    /*std::cout<<std::hex<<"sizeof(GlowObjectDefinition_t) "<<sizeof(GlowObjectDefinition_t)<<std::endl;
    std::cout<<"sizeof(CGlowObjectManager) "<<sizeof(CGlowObjectManager)<<std::endl;
    std::cout<<"sizeof(CUtlVector) "<<sizeof(CUtlVector<int>)<<std::endl;*/
    uintptr_t glow_call {csgo_process.find_pattern_in_module_with_name("client_client.so","E8????????488b3d????????BE01??????C7")};
    uintptr_t glow_call_operand {csgo_process.get_relative_call_address(glow_call)};
    uintptr_t glow_manager {csgo_process.get_relative_call_address(glow_call_operand + 0xf)};
    for(bool end {false};!end;)
    {
    }
}