cmake_minimum_required(VERSION 3.0)

project(AdventOfCode)

if(MSVC)
  add_compile_options(/std:c++latest /W4 /WX)
else()
  add_compile_options(-std=c++17 -Wall -Wextra -pedantic)
endif()

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

find_package(doctest REQUIRED)
include(CTest)
include(doctest)

add_subdirectory(spaceship)
add_subdirectory(puzzles)

add_subdirectory(unit-tests)