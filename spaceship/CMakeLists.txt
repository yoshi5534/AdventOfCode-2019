find_package (range-v3)
add_library (spaceship 
  include/amplifier.h src/amplifier.cpp
  include/arcade.h src/arcade.cpp
  include/ascii.h src/ascii.cpp
  include/asteroids.h src/asteroids.cpp
  include/computer.h src/computer.cpp
  include/droid.h src/droid.cpp
  include/fuel.h src/fuel.cpp
  include/fft.h src/fft.cpp
  include/motion.h src/motion.cpp
  include/orbit.h src/orbit.cpp
  include/password.h src/password.cpp
  include/robot.h src/robot.cpp
  include/spaceimage.h src/spaceimage.cpp
  include/wires.h src/wires.cpp
  )
target_link_libraries (spaceship ${CONAN_LIBS})

target_include_directories(spaceship PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>/include
)