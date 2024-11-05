cmake_minimum_required(VERSION 3.13)

project(Zinx_learn)

set(CMAKE_CXX_STANDARD 17)

link_libraries(gtest pthread)

add_subdirectory(znet)
