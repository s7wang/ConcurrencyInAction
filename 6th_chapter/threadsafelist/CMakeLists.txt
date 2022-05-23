cmake_minimum_required(VERSION 2.8)

project(threadsafelist)

set(CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS}")

include_directories(${CMAKE_SOURCE_DIR})
add_definitions("-Wall -g") #"-g" means allow using gdb debug

add_executable(exec main.cpp)

target_link_libraries(exec pthread)