# Top-level CMakeLists.txt for a simple Ada-language project.  See the README
# file in this directory for the motivations behind this project.

# Version 2.4.5 or above of cmake is required!
CMAKE_MINIMUM_REQUIRED(VERSION 2.4.5 FATAL_ERROR)
# set CMake 2.6.x policy if the cmake_policy command exists, (i.e., if
# the user is using CMake 2.6.0 or newer).
if(COMMAND  cmake_policy)
  # empty elements are not ignored in lists.
  cmake_policy(SET CMP0007 NEW)
endif(COMMAND  cmake_policy)

# Ideally use "project(simple_ada_test Ada)", but that doesn't work
# (at least for case where Ada support files are in user-specified location
# below).  Therefore, specify project with C even though C is unused.

project(simple_ada_test C)

# First location where simple_ada_test looks for CMake Ada language
# support files.
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake_working/Modules)

option(BUILD_SHARED_LIBS "Build shared libraries" ON)

# Set up Ada variables specific to this project.
include(ada.cmake)

# Build Ada library
add_subdirectory(src_lib)
# Build Ada executable that links to that Ada library
add_subdirectory(src_executable)
