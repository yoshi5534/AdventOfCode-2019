add_library (password password.h password.cpp)
target_include_directories(password PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
  $<INSTALL_INTERFACE:include/password>  # <prefix>/include/password
)
