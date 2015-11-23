# Top-level CMakeLists.txt for a simple Ada-language project.  See the README
# file in this directory for the motivations behind this project.
cmake_minimum_required(VERSION 3.0.2 FATAL_ERROR)

# Before the project command must specify the location where
# the test_ada project looks for CMake Ada language support files.
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/Modules)

project(test_ada Ada)

option(BUILD_SHARED_LIBS "Build shared libraries" ON)

# Set up Ada variables specific to this project.
include(ada.cmake)

# Build Ada library
add_subdirectory(src_lib)
# Build Ada executable that links to that Ada library
add_subdirectory(src_executable)
