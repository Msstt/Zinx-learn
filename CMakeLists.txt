cmake_minimum_required(VERSION 3.13)

project(Zinx_learn)

set(CMAKE_CXX_STANDARD 17)

# 传递参数
set(MAIN_PATH ${CMAKE_SOURCE_DIR})
configure_file(${CMAKE_SOURCE_DIR}/utils/include/utils/param_deliver.h.in ${CMAKE_SOURCE_DIR}/utils/include/utils/param_deliver.h)

link_libraries(gtest pthread glog)

# glog 配置
# add_definitions("-DNDEBUG")

add_subdirectory(utils)

add_subdirectory(znet)

add_subdirectory(core)
