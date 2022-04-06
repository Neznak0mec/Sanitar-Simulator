cmake_minimum_required(VERSION 3.20)
project(DurkaCPP)

set(CMAKE_CXX_STANDARD 14)


## If you want to link SFML statically
set(SFML_STATIC_LIBRARIES TRUE)

## In most cases better set in the CMake cache
set(SFML_DIR "C:/SFML-2.5.1/lib/cmake/SFML")

find_package(SFML 2.5.1 COMPONENTS graphics audio REQUIRED)
include_directories(${SFML_INCLUDE_DIR})
add_executable(DurkaCPP main.cpp Object.h Object.cpp UI.cpp UI.h Player.cpp Player.h)
target_link_libraries(DurkaCPP sfml-graphics sfml-audio)