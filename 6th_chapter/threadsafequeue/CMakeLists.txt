cmake_minimum_required(VERSION 2.8)

project(ts_queue)

set(CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS}")
# Add compilation options
# add_definitions("-Wall -g") #"-g" means allow using gdb debug
include_directories("${CMAKE_SOURCE_DIR}")

add_executable(exec main.cpp)

target_link_libraries(exec pthread)