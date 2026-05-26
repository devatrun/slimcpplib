# cmakelist.txt : top-level cmake project file, do global configuration
cmake_minimum_required (VERSION 3.14)

set (ProjectName "slimcpplib")
project (${ProjectName})

enable_testing()

add_library (${ProjectName} INTERFACE)
target_include_directories (${ProjectName} INTERFACE
    ${CMAKE_SOURCE_DIR}/include
)

# include sub-projects
add_subdirectory ("${CMAKE_CURRENT_SOURCE_DIR}/src/tests" "${CMAKE_CURRENT_BINARY_DIR}/tests")
add_subdirectory ("${CMAKE_CURRENT_SOURCE_DIR}/src/bench" "${CMAKE_CURRENT_BINARY_DIR}/bench")
