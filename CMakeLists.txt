cmake_minimum_required(VERSION 3.22)
set(CMAKE_CXX_STANDARD 20)
project(mini_jvm)

option(ENABLE_TRACE "Enables runtime tracing options" ON)

set(ROOT ${CMAKE_SOURCE_DIR})

include(FetchContent)
FetchContent_Declare(
    cxxopts
    GIT_REPOSITORY https://github.com/jarro2783/cxxopts
    GIT_TAG        3bf268481da8208d171d8908e6491459de3651d7)
FetchContent_MakeAvailable(cxxopts)

add_executable(mini_jvm main.cpp)
target_include_directories(mini_jvm PRIVATE ${ROOT}/inc)

include(lib_vm.cmake)
target_link_libraries(mini_jvm vm cxxopts)
