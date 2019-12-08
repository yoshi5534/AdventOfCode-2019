add_library (spaceship 
  include/fuel.h
  include/computer.h src/computer.cpp
  include/password.h src/password.cpp
  include/wires.h src/wires.cpp
  )

target_include_directories(spaceship PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>/include
)