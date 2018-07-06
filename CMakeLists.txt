cmake_minimum_required(VERSION 3.5.0)

# Set the C++ standard and force a requirement to C++14
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)

project(jwaim)

add_subdirectory(src)

# Warn the user if the build type hasn't been set with -DCMAKE_BUILD_TYPE={TYPE}
# Example: mkdir build && cd build && cmake -DCMAKE_BUILD_TYPE=Debug ..
# This would enable -g for developers

if(NOT CMAKE_BUILD_TYPE)
    message("CMAKE_BUILD_TYPE not set, defaulting to Release!")
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING
      "Choose the type of build, options are: Debug, Release, RelWithDebInfo, MinSizeRel." FORCE)
endif(NOT CMAKE_BUILD_TYPE)
