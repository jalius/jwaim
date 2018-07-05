#ifndef JWAIM_HPP
#define JWAIM_HPP
#include "remote_process_interface.hpp"

class jwaim
{
public:
  jwaim();
  ~jwaim();
  int run();

private:
  remote_process remote_process_csgo;
};
#endif