add_executable (AOC2test aoc2.cpp)
target_link_libraries (AOC2test computer ${CONAN_LIBS})

catch_discover_tests(AOC2test)