cmake_minimum_required(VERSION 3.22.1)
set(CMAKE_CXX_STANDARD 23)

add_library(rzt_containers INTERFACE)

target_link_directories(rzt_containers INTERFACE
        .)