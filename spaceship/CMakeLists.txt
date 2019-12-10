find_package (range-v3)
add_library (spaceship 
  include/fuel.h
  include/amplifier.h src/amplifier.cpp
  include/computer.h src/computer.cpp
  include/orbit.h src/orbit.cpp
  include/password.h src/password.cpp
  include/wires.h src/wires.cpp
  )
target_link_libraries (spaceship ${CONAN_LIBS})

target_include_directories(spaceship PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>/include
)