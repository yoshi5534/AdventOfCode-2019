cmake_minimum_required (VERSION 3.0)

project (AdventOfCode)
add_definitions("-std=c++17")

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

find_package(Catch2 REQUIRED)
include(CTest)
include(Catch)

add_subdirectory (1)
add_subdirectory (2)
add_subdirectory (3)

add_subdirectory (unit-tests)
