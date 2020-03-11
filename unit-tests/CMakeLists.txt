add_executable(
  AOCtests
  main.cpp
  amplifier.cpp
  asteroids.cpp
  computer.cpp
  fft.cpp
  fuel.cpp
  maze.cpp
  motion.cpp
  orbit.cpp
  password.cpp
  spaceimage.cpp
  vault.cpp
  wires.cpp)

target_link_libraries(AOCtests spaceship doctest::doctest)
target_compile_options(AOCtests PRIVATE -std=c++17)

doctest_discover_tests(AOCtests)