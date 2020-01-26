cmake_minimum_required(VERSION 3.0)

project(AdventOfCode)
add_definitions("-std=c++17")

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

find_package(doctest REQUIRED)
include(CTest)
include(doctest)

add_subdirectory(spaceship)
add_subdirectory(puzzles)

add_subdirectory(unit-tests)