add_executable (AOCtests main.cpp aoc2.cpp aoc3.cpp aoc4.cpp)
target_link_libraries (AOCtests spaceship ${CONAN_LIBS})

catch_discover_tests(AOCtests)