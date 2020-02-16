#!/bin/bash

SOURCE_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"

conan remote add conan-cpp https://api.bintray.com/conan/squawkcpp/conan-cpp 

conan install "$SOURCE_DIR" --install-folder build --profile ./profiles/clang
conan build "$SOURCE_DIR" --build-folder build