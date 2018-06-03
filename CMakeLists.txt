cmake_minimum_required(VERSION 3.2.0)
project(jwaim)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

find_package(PkgConfig REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Threads REQUIRED)
find_package(X11 REQUIRED)

pkg_check_modules(LIBCONFIG REQUIRED libconfig++)

add_executable(jwaim
    hack.cpp
    helper.cpp
    main.cpp
    remote.cpp
    window.cpp
)

target_link_libraries(jwaim
    Qt5::Widgets
    ${LIBCONFIG_LIBRARIES}
    ${CMAKE_THREAD_LIBS_INIT}
    ${X11_LIBRARIES}
)

configure_file(settings_example.cfg settings.cfg COPYONLY)
