cmake_minimum_required(VERSION 2.8)
project(table)

set(CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS}")

include_directories(${CMAKE_SOURCE_DIR})

add_executable(exec main.cpp)

target_link_libraries(exec pthread boost_thread)