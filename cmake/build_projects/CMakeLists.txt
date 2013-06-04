# Top-level CMakeLists.txt file for a project that simply builds
# and installs external projects using epa.cmake include fragments
# representing the build and test instructions for several different projects.

# Copyright (C) 2013 Alan W. Irwin

# This file is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License, or (at your option) any later version.

# This file is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.

# You should have received a copy of the GNU Lesser General Public
# License along with this file; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

project(build_projects NONE)

message(STATUS "CMake version = ${CMAKE_VERSION}")
message(STATUS "CMAKE_SYSTEM = ${CMAKE_SYSTEM}")
message(STATUS "CMAKE_INSTALL_PREFIX = ${CMAKE_INSTALL_PREFIX}")

cmake_minimum_required(VERSION 2.8.10.2 FATAL_ERROR)

# Use modified version of ExternalProject where cmake -E touch has
# been replaced with the touch command (to work around cmake bug 14020)
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR})
include(ExternalProject)

set(EP_BASE ${CMAKE_BINARY_DIR}/build_projects)
message(STATUS "EP_BASE = ${EP_BASE}")
set_directory_properties(PROPERTIES EP_BASE ${EP_BASE})

# Set build_projects variables to control builds in general

if(NOT ${CMAKE_GENERATOR} STREQUAL "Unix Makefiles" AND NOT ${CMAKE_GENERATOR} STREQUAL "MSYS Makefiles")
  message(FATAL_ERROR "This project only works with either the \"Unix Makefiles\" or \"MSYS Makefiles\" generators")
endif(NOT ${CMAKE_GENERATOR} STREQUAL "Unix Makefiles" AND NOT ${CMAKE_GENERATOR} STREQUAL "MSYS Makefiles")
  
# Propagate the overall CMake generator and install prefix to all CMake-based
# software project builds.
set(BP_CMAKE_COMMAND cmake "-G${CMAKE_GENERATOR}" -DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_INSTALL_PREFIX})
 
# The non-parallel versions are for build systems that
# apparently have race conditions for parallel builds.
set(BP_PARALLEL_BUILD_COMMAND make -j4)
set(BP_NON_PARALLEL_MAKE_COMMAND make)
set(BP_PARALLEL_CTEST_COMMAND ctest -j4)
set(BP_NON_PARALLEL_CTEST_COMMAND ctest)
set(BP_ORIGINAL_NATIVE_PATH "$ENV{PATH}")
set(BP_PATH "${BP_ORIGINAL_NATIVE_PATH}")

if(MSYS)
  # Useful function to convert Windows list of semicolon-delimited
  # paths to the equivalent list of MSYS paths (exactly like the
  # colon-delimited Unix list of paths except the driver letters are
  # specified as the initial one-character component of each of the
  # paths).  to MSYS path (like Unix path but with the drive letters
  # as single character initial component of the path).  For example,
  # this function will will transform the Windows list of paths,
  # "z:\whatever_z;c:\whatever_c" to /z/whatever_z:/c/whatever_c.
  function(determine_msys_path MSYS_PATH NATIVE_PATH)
    #message(STATUS "NATIVE_PATH = ${NATIVE_PATH}")
    string(REGEX REPLACE "^\([a-zA-z]\):" "/\\1" PATH  "${NATIVE_PATH}")
    string(REGEX REPLACE ";\([a-zA-z]\):" ";/\\1" PATH  "${PATH}")
    string(REGEX REPLACE ";" ":" PATH  "${PATH}")
    file(TO_CMAKE_PATH "${PATH}" PATH)
    #message(STATUS "MSYS_PATH = ${PATH}")
    set(${MSYS_PATH} ${PATH} PARENT_SCOPE)
  endfunction(determine_msys_path)
endif(MSYS)

include(shapelib/ep.cmake)
include(ndiff/ep.cmake)
include(libqhull/ep.cmake)
include(libharu/ep.cmake)
