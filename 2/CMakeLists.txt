add_library (computer computer.h computer.cpp)
target_include_directories(computer PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
  $<INSTALL_INTERFACE:include/computer>  # <prefix>/include/computer
)

add_executable (AOC_2 main.cpp)
target_link_libraries (AOC_2 computer)