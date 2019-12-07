cmake_minimum_required (VERSION 3.0)

project (AdventOfCode)
add_definitions("-std=c++11")

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

add_subdirectory (1)
add_subdirectory (2)

add_subdirectory (unit-tests)