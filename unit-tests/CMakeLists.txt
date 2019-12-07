add_executable (AOC2test aoc2.cpp)
target_link_libraries (AOC2test computer ${CONAN_LIBS})

catch_discover_tests(AOC2test)

add_executable (AOC3test aoc3.cpp)
target_link_libraries (AOC3test wires ${CONAN_LIBS})

catch_discover_tests(AOC3test)

add_executable (AOC4test aoc4.cpp)
target_link_libraries (AOC4test password ${CONAN_LIBS})

catch_discover_tests(AOC4test)