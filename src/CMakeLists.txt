set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -g -O0")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

# Check for necessary build packages here
find_package(Threads)

add_executable(jwaim
    aimbot.cpp
    glowhack.cpp
    jwaim.cpp
    main.cpp
    remote_process.cpp
    pattern_scanner.cpp
)

# Link build packages here
target_link_libraries(jwaim
    ${CMAKE_THREAD_LIBS_INIT}
)
