#include "jwaim.hpp"
#include <thread>
#include "glowhack.hpp"
#include <iostream>
constexpr auto csgo_linux_process_name = "csgo_linux64";

jwaim::jwaim() : remote_process_csgo{csgo_linux_process_name} //init csgo process object
{

}
jwaim::~jwaim()
{

}
int jwaim::run()
{
    glowhack g{this->remote_process_csgo};
    std::thread t(&glowhack::run, &g);
    t.join();
}