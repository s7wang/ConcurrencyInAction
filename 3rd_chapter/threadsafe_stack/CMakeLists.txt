cmake_minimum_required(VERSION 2.8)

project(threadsafe_stack)
include_directories("${CMAKE_SOURCE_DIR}") 
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")



add_executable(exec main_test.cpp)

target_link_libraries(exec pthread)