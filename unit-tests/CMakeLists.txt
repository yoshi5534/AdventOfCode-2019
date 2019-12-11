add_executable(
  AOCtests
  main.cpp
  aoc2.cpp
  aoc3.cpp
  aoc4.cpp
  aoc5.cpp
  aoc6.cpp
  aoc7.cpp
  aoc8.cpp)
target_link_libraries(AOCtests spaceship ${CONAN_LIBS})

catch_discover_tests(AOCtests)
