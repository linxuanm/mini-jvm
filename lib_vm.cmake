file(GLOB_RECURSE LIB_VM_SRC ${ROOT}/src/vm/*.cpp)
add_library(vm STATIC ${LIB_VM_SRC})
target_include_directories(vm PRIVATE ${ROOT}/inc ${ROOT}/inc/vm)

include(lib_core.cmake)
target_link_libraries(vm PUBLIC core)
