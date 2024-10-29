file(GLOB_RECURSE LIB_TRACE_SRC ${ROOT}/src/trace/*.cpp)
add_library(trace STATIC ${LIB_TRACE_SRC})
target_include_directories(trace PRIVATE ${ROOT}/inc)

include(FetchContent)
FetchContent_Declare(
    fmt
    GIT_REPOSITORY https://github.com/fmtlib/fmt
    GIT_TAG        e69e5f977d458f2650bb346dadf2ad30c5320281) # 10.2.1
FetchContent_MakeAvailable(fmt)

target_link_libraries(trace PRIVATE fmt::fmt)
