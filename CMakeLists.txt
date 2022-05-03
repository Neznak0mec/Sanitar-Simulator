cmake_minimum_required(VERSION 3.21)
project(DurkaCPP)

set(CMAKE_CXX_STANDARD 14)


file(GLOB source_files CONFIGURE_DEPENDS
        "head/*"
        )


find_package(SFML 2.5.1 COMPONENTS graphics audio REQUIRED)

add_executable(DurkaCPP main.cpp ${source_files})
target_link_libraries(DurkaCPP sfml-graphics sfml-audio)
