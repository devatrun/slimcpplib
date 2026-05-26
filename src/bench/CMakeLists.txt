# project's target name
set (TargetName "slimcpplib_bench")

# project's header files
set (HeaderFiles
    bench_helpers.h
    type_sets.h
)

# project's source files
set (SourceFiles
    arithmetic_bench.cpp
)

# include benchmark library
include(FetchContent)

# DOWNLOAD_EXTRACT_TIMESTAMP requires CMake 3.24+
if(CMAKE_VERSION VERSION_GREATER_EQUAL 3.24)
  FetchContent_Declare(
    googlebenchmark
    URL https://github.com/google/benchmark/archive/refs/tags/v1.9.4.zip
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
  )
else()
  FetchContent_Declare(
    googlebenchmark
    URL https://github.com/google/benchmark/archive/refs/tags/v1.9.4.zip
  )
endif()

# for Windows: prevent overriding the parent project's compiler/linker settings
set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "" FORCE)
set(BENCHMARK_ENABLE_GTEST_TESTS OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googlebenchmark)

# project's targets
add_executable (${TargetName} ${HeaderFiles} ${SourceFiles})

target_link_libraries (${TargetName}
    slimcpplib
    benchmark::benchmark
    benchmark::benchmark_main
)

# include common properties
include("${CMAKE_SOURCE_DIR}/config/cmake/base.cmake")
