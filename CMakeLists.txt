cmake_minimum_required(VERSION 3.22)
set(CMAKE_CXX_STANDARD 20)
project(mini_jvm)

set(ROOT ${CMAKE_SOURCE_DIR})

include(lib_vm.cmake)

add_executable(mini_jvm main.cpp)
target_link_libraries(mini_jvm core vm)
