cmake_minimum_required(VERSION 2.8)

project(08_01)
include_directories("${CMAKE_SOURCE_DIR}") 
# 添加对gdb的支持
SET(CMAKE_BUILD_TYPE "Debug")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g2 -ggdb")
SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")


add_executable(exec 08_01.cpp)

target_link_libraries(exec pthread)