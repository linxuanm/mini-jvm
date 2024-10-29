option(ENABLE_TRACE "Enables runtime tracing options" ON)

file(GLOB_RECURSE LIB_CORE_SRC ${ROOT}/src/core/*.cpp)
add_library(core STATIC ${LIB_CORE_SRC})
target_include_directories(core PRIVATE ${ROOT}/inc)

if(ENABLE_TRACE)
  include(lib_trace.cmake)
  target_link_libraries(core PUBLIC trace)
  target_compile_definitions(core PRIVATE ENABLE_TRACE=1)
endif()
