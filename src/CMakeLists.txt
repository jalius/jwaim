set (CMAKE_CXX_FLAGS "-Wall -g -lpthread -pthread -std=c++14")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

add_executable(jwaim main.cpp jwaim.cpp remote_process_interface.cpp glowhack.cpp aimbot.cpp process_utils.cpp)