#include "jwaim.hpp"
#include <iostream>

jwaim::jwaim() : remote_process_csgo{csgo_linux_process_name} 
{

}
jwaim::~jwaim()
{

}
int jwaim::run()
{
    bool quit = false;
    while(!quit)
    {
        auto x = remote_process_csgo.find_pattern_in_module_with_name("client_client.so", "\xE8\x00\x00\x00\x00\x48\x8b\x3d\x00\x00\x00\x00\xBE\x01\x00\x00\x00\xC7", "x????xxx????xxxxxx");
        std::cout<<std::hex<<x<<std::endl;
    }
}