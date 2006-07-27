# User overrides of CMake default compiler and linking rules.
if(APPLE)
  set(CMAKE_SHARED_LINKER_FLAGS "-single_module"
  CACHE STRING "Flags used to link a shared library.")
endif(APPLE)
