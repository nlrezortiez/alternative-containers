cmake_minimum_required(VERSION 3.22.1)

set(CMAKE_CXX_STANDARD 23)

project(libtest)

find_package(GTest REQUIRED)

add_executable(libtest libtest.cpp)
target_link_libraries(libtest rzt_containers ${GTEST_LIBRARIES})
gtest_discover_tests(libtest)
