add_library (wires wires.h wires.cpp)
target_include_directories(wires PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
  $<INSTALL_INTERFACE:include/wires>  # <prefix>/include/wires
)
