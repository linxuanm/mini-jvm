cmake_minimum_required(VERSION 3.22)
project(mini_jvm)

set(CMAKE_CXX_STANDARD 20)

set(VM_DIR ${CMAKE_CURRENT_LIST_DIR})

file(GLOB_RECURSE VM_LIB_SRCS
    ${VM_DIR}/src/*.c
    ${VM_DIR}/src/*.cpp
    ${VM_DIR}/api/*.c
    ${VM_DIR}/api/*.cpp)

add_library(vm ${VM_LIB_SRCS})
target_include_directories(vm PRIVATE ${VM_DIR}/inc)

add_executable(mini_jvm main.cpp
)
target_link_libraries(mini_jvm vm)
