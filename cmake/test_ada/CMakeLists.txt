# Top-level CMakeLists.txt for a simple Ada-language project.  See the README
# file in this directory for the motivations behind this project.
cmake_minimum_required(VERSION 3.0.2 FATAL_ERROR)

# Ideally use "project(simple_ada_test Ada)", but that doesn't work
# (at least for case where Ada support files are in user-specified
# location below).  The only combination that appears to work
# correctly is to specify C for the base language in the project
# command (NONE does not work) and use "enable_language(Ada)" below in
# ada.cmake.  This is obviously a workaround for some bug in our Ada language
# support, but you would have to understand the inner workings of
# CMake language support much better than I do to find and fix the
# bug.

project(simple_ada_test C)

# First location where simple_ada_test looks for CMake Ada language
# support files.
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake/Modules)

option(BUILD_SHARED_LIBS "Build shared libraries" ON)

# Set up Ada variables specific to this project.
include(ada.cmake)

# Build Ada library
add_subdirectory(src_lib)
# Build Ada executable that links to that Ada library
add_subdirectory(src_executable)
