add_executable(
  AOCtests
  main.cpp
  amplifier.cpp
  asteroids.cpp
  computer.cpp
  fuel.cpp
  motion.cpp
  orbit.cpp
  password.cpp
  spaceimage.cpp
  wires.cpp)

target_link_libraries(AOCtests spaceship ${CONAN_LIBS})

catch_discover_tests(AOCtests)