cmake_minimum_required(VERSION 3.13)

project(Zinx_learn)

set(CMAKE_CXX_STANDARD 17)

link_libraries(gtest pthread glog)

# glog 配置
add_definitions("-DNDEBUG")

add_subdirectory(tools)

add_subdirectory(znet)
