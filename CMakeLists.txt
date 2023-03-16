cmake_minimum_required(VERSION 3.22.1)

project(stl_extended LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)

enable_testing()

add_subdirectory(lib)
add_subdirectory(test)