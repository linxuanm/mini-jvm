include(lib_core.cmake)

file(GLOB_RECURSE LIB_VM_SRC ${ROOT}/src/vm/*.cpp)
add_library(vm STATIC ${LIB_VM_SRC})
target_include_directories(vm PRIVATE ${ROOT}/inc/vm)
