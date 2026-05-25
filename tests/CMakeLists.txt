# cmakelist.txt : top-level cmake project file, do global configuration
cmake_minimum_required (VERSION 3.14)

project ("slimcpplibtest")

enable_testing()

# include sub-projects
add_subdirectory ("src")