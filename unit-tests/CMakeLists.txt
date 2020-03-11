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

doctest_discover_tests(AOCtests)