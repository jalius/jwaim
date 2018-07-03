#include "remote_process_interface.hpp"

constexpr auto csgo_linux_process_name = "csgo_linux64";

class jwaim
{
  public:
    jwaim();
    ~jwaim();
    int run();

  private:
    remote_process remote_process_csgo;
};