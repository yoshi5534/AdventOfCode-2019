cmake_minimum_required(VERSION 3.0)

project(AdventOfCode)
if ( CMAKE_COMPILER_IS_GNUCC )
    set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -std=c++17 -Wall -Wextra")
endif()

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

find_package(doctest REQUIRED)
include(CTest)
include(doctest)

add_subdirectory(spaceship)
add_subdirectory(puzzles)

add_subdirectory(unit-tests)