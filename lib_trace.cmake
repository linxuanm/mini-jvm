file(GLOB_RECURSE LIB_TRACE_SRC ${ROOT}/src/trace/*.cpp)
add_library(trace STATIC ${LIB_TRACE_SRC})
target_include_directories(trace PRIVATE ${ROOT}/inc/trace)
