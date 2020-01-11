#!/bin/bash

SOURCE_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"

mkdir "$SOURCE_DIR/build"
cd "$SOURCE_DIR/build"
conan remote add conan-cpp https://api.bintray.com/conan/squawkcpp/conan-cpp 
conan profile new aoc2019 --detect
conan install "$SOURCE_DIR"

cmake "$SOURCE_DIR"
cmake --build "$SOURCE_DIR/build" --config Release --target all