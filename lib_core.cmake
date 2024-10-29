file(GLOB_RECURSE LIB_CORE_SRC ${ROOT}/src/core/*.cpp)
add_library(core STATIC ${LIB_CORE_SRC})
target_include_directories(core PRIVATE ${ROOT}/inc)

include(FetchContent)
FetchContent_Declare(
    fmt
    GIT_REPOSITORY https://github.com/fmtlib/fmt
    GIT_TAG        e69e5f977d458f2650bb346dadf2ad30c5320281) # 10.2.1
FetchContent_MakeAvailable(fmt)

target_link_libraries(core PUBLIC fmt::fmt)

if(ENABLE_TRACE)
  target_compile_definitions(core PUBLIC ENABLE_TRACE=1)
endif()
