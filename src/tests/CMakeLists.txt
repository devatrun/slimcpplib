# project's target name
set (TargetName "slimcpplib_test")

# project's header files
set (HeaderFiles
    type_sets.h
)

# project's source files
set (SourceFiles
    base_tests.cpp
    overflow_boundary_tests.cpp
    compound_shift_tests.cpp
    arithmetic_tests.cpp
    signed_tests.cpp
    io_tests.cpp
    fixdiv_tests.cpp
    literal_mixed_tests.cpp
    stl_properties_tests.cpp
)

# include gtest library
include(FetchContent)

# DOWNLOAD_EXTRACT_TIMESTAMP requires CMake 3.24+
if(CMAKE_VERSION VERSION_GREATER_EQUAL 3.24)
  FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
  )
else()
  FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
  )
endif()

# for Windows: prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# project's targets
add_executable (${TargetName} ${HeaderFiles} ${SourceFiles})

target_link_libraries (${TargetName}
    slimcpplib
    gtest_main
)

# include common properties
include("${CMAKE_SOURCE_DIR}/config/cmake/base.cmake")

# enable tests
include(GoogleTest)
gtest_discover_tests(${TargetName})
