cmake_minimum_required(VERSION 3.10)
project(multithread LANGUAGES CXX)

find_package(Threads REQUIRED)
set(CMAKE_CXX_STANDARD 20)
add_executable(${PROJECT_NAME} main.cpp)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)
target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic -Werror)
target_link_libraries(${PROJECT_NAME} PRIVATE Threads::Threads)