cmake_minimum_required(VERSION 3.22.1)

set(CMAKE_CXX_STANDARD 17)

project(libtest)

add_executable(libtest libtest.cpp)

target_link_libraries(libtest gtest_main)
