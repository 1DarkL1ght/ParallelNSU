cmake_minimum_required(VERSION 3.10)
project(task1)

set(CMAKE_CXX_STANDARD 17)

option(USE_DOUBLE ON)

if(USE_DOUBLE)
    add_compile_definitions(USE_DOUBLE)
endif()

add_executable(task1 main.cpp)