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
message(STATUS "CMAKE_GENERATOR = ${CMAKE_GENERATOR}")
message(STATUS "CMAKE_INSTALL_PREFIX = ${CMAKE_INSTALL_PREFIX}")

cmake_minimum_required(VERSION 2.8.10.2 FATAL_ERROR)

set(EP_BASE ${CMAKE_BINARY_DIR}/build_projects)
message(STATUS "EP_BASE = ${EP_BASE}")
set_directory_properties(PROPERTIES EP_BASE ${EP_BASE})

# Set build_projects variables to control builds in general

# Test whether there is access to the MSYS platform on the PATH.  Note
# this is a different question than whether you are using the "MSYS
# Makefiles" generator since some other generators (e.g., "MinGW
# Makefiles") can have MSYS on the path so long as sh.exe is renamed
# or removed from that PATH.  Note, we search for the automatic
# installer app, mingw-get.exe and the bash.exe shell to check
# (approximately) whether we have access to the MSYS platform, and
# then we test that platform (very approximately) for completeness by
# making sure unzip (necessary for some build configurations to unpack
# their packages) is installed (using mingw-get).

find_program(MINGW_GET_EXECUTABLE mingw-get)
find_program(BASH_EXECUTABLE bash)
if(MINGW_GET_EXECUTABLE AND BASH_EXECUTABLE)
  set(MSYS_PLATFORM ON)
else(MINGW_GET_EXECUTABLE AND BASH_EXECUTABLE)
  set(MSYS_PLATFORM OFF)
endif(MINGW_GET_EXECUTABLE AND BASH_EXECUTABLE)
message(STATUS "WIN32 = ${WIN32}")
message(STATUS "MINGW = ${MINGW}")
message(STATUS "MSYS = ${MSYS}")
message(STATUS "MSYS_PLATFORM = ${MSYS_PLATFORM}")
message(STATUS "CYGWIN = ${CYGWIN}")

if(CYGWIN)
  message(FATAL_ERROR "build_projects does not work correctly on the Cygwin platform.  Use the MinGW compiler and MSYS platform instead.")
endif(CYGWIN)

if(WIN32 AND NOT MSYS_PLATFORM)
  message(FATAL_ERROR "build_projects does not work correctly on Windows without the MSYS platform.  Install the MSYS platform (perhaps without sh.exe depending on CMake generator) on Windows.")
endif(WIN32 AND NOT MSYS_PLATFORM)

# List of executables required by build_projects CMake logic.
set(executables_LIST
  # ExternalProject
  touch
  # Unpacking
  unzip
  # Updating
  mkdir
  cp
  patch
  # General
  env
  # Not specifically required by build_projects (yet), but some
  # individual project configurations will not work unless this is
  # available so might as well check it here.
  bash
  make
  )

foreach(executable ${executables_LIST})
  string(TOUPPER ${executable} EXECUTABLE)
  find_program(${EXECUTABLE}_EXECUTABLE ${executable})
  if(NOT ${EXECUTABLE}_EXECUTABLE)
    if(MSYS_PLATFORM)
      message(STATUS "${executable} missing from your MSYS platform.  Use mingw-get to install it.")
    endif(MSYS_PLATFORM)
    message(FATAL_ERROR "${executable} must be on your PATH in order for build_projects to work correctly")  
  endif(NOT ${EXECUTABLE}_EXECUTABLE)
endforeach(executable ${executables_LIST})

# Use modified version of ExternalProject where cmake -E touch has
# been replaced with the TOUCH_EXECUTABLE command (to work around
# cmake bug 14020)
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR})
include(ExternalProject)

if(MSYS_PLATFORM)
  # Useful function to convert Windows list of semicolon-delimited
  # PATHs to the equivalent list of MSYS PATHs (exactly like the
  # colon-delimited Unix list of PATHs except the driver letters are
  # specified as the initial one-character component of each of the
  # PATHs).  For example, this function will transform the Windows
  # list of PATHs, "z:\path1;c:\path2" to "/z/path1:/c/path2".
  function(determine_msys_path MSYS_PATH NATIVE_PATH)
    #message(STATUS "NATIVE_PATH = ${NATIVE_PATH}")
    string(REGEX REPLACE "^\([a-zA-z]\):" "/\\1" PATH  "${NATIVE_PATH}")
    string(REGEX REPLACE ";\([a-zA-z]\):" ";/\\1" PATH  "${PATH}")
    string(REGEX REPLACE ";" ":" PATH  "${PATH}")
    file(TO_CMAKE_PATH "${PATH}" PATH)
    #message(STATUS "MSYS_PATH = ${PATH}")
    set(${MSYS_PATH} ${PATH} PARENT_SCOPE)
  endfunction(determine_msys_path)
endif(MSYS_PLATFORM)

# This gives the full pathname of the associated build tool for at
# least the "Unix Makefiles", "Ninja", "MSYS Makefiles", "MinGW
# Makefiles", and "NMake Makefiles JOM" CMake generators.
set(BP_BUILD_COMMAND "${CMAKE_MAKE_PROGRAM}")

# Actual make programme used for autotools builds.
set(BP_MAKE_COMMAND ${MAKE_EXECUTABLE})

set(BP_CTEST_COMMAND "${CMAKE_CTEST_COMMAND}")

if(MSYS_PLATFORM)
  # On the MSYS platform, the env command (used to set relevant
  # environment variables for the commands below in the */bp.cmake
  # scripts) requires all full paths be in the MSYS platform form.
  determine_msys_path(BP_CMAKE_COMMAND "${BP_CMAKE_COMMAND}")
  determine_msys_path(BP_BUILD_COMMAND "${BP_BUILD_COMMAND}")
  determine_msys_path(BP_MAKE_COMMAND "${BP_MAKE_COMMAND}")
  determine_msys_path(BP_CTEST_COMMAND "${BP_CTEST_COMMAND}")
  determine_msys_path(MSYS_CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")
  determine_msys_path(MSYS_CMAKE_COMMAND "${CMAKE_COMMAND}")
  # Propagate the overall CMake generator and install prefix to all CMake-based
  # software project builds.
  set(BP_CMAKE_COMMAND ${MSYS_CMAKE_COMMAND} "-G${CMAKE_GENERATOR}" -DCMAKE_INSTALL_PREFIX:PATH=${MSYS_CMAKE_INSTALL_PREFIX})
  # Propagate the install prefix to autotools-based builds
  set(BP_CONFIGURE_COMMAND configure --prefix=${MSYS_CMAKE_INSTALL_PREFIX})
else(MSYS_PLATFORM)
  # Propagate the overall CMake generator and install prefix to all CMake-based
  # software project builds.
  set(BP_CMAKE_COMMAND ${CMAKE_COMMAND} "-G${CMAKE_GENERATOR}" -DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_INSTALL_PREFIX})
  # Propagate the install prefix to autotools-based builds
  set(BP_CONFIGURE_COMMAND configure --prefix=${CMAKE_INSTALL_PREFIX})
endif(MSYS_PLATFORM)

# The parallel versions are for software packages that
# do not have race conditions for parallel builds or tests.
set(BP_PARALLEL_BUILD_COMMAND "${BP_BUILD_COMMAND}" -j4)
set(BP_PARALLEL_CTEST_COMMAND "${BP_CTEST_COMMAND}" -j4)
# For autotools based builds.
set(BP_PARALLEL_MAKE_COMMAND "${BP_MAKE_COMMAND}" -j4)

message(STATUS "BP_CMAKE_COMMAND = ${BP_CMAKE_COMMAND}")
message(STATUS "BP_CONFIGURE_COMMAND = ${BP_CONFIGURE_COMMAND}")
message(STATUS "BP_BUILD_COMMAND = ${BP_BUILD_COMMAND}")
message(STATUS "BP_PARALLEL_BUILD_COMMAND = ${BP_PARALLEL_BUILD_COMMAND}")
message(STATUS "BP_MAKE_COMMAND = ${BP_MAKE_COMMAND}")
message(STATUS "BP_PARALLEL_MAKE_COMMAND = ${BP_PARALLEL_MAKE_COMMAND}")
message(STATUS "BP_CTEST_COMMAND = ${BP_CTEST_COMMAND}")
message(STATUS "BP_PARALLEL_CTEST_COMMAND = ${BP_PARALLEL_CTEST_COMMAND}")

set(BP_ORIGINAL_NATIVE_PATH "$ENV{PATH}")
set(BP_PATH "${BP_ORIGINAL_NATIVE_PATH}")

# Now include the build configurations for certain targets and their
# dependencies.

# Each (directly or indirectly) included build configuration
# that is judged to be working should append its target to build_target_LIST
# so that in the logic below we can creat a "build_all" target that depends
# on those individual build targets
set(build_target_LIST)

# Configure a build of ndiff.  The ndiff application is not a a PLplot
# dependency, but still this fuzzy diff is quite useful for PLplot
# developers.
include(ndiff/bp.cmake)

# Configure a build of wxwidgets, a soft (optional) PLplot dependency
include(wxwidgets/bp.cmake)

# Configure a build of PLplot and its dependencies.
include(plplot/bp.cmake)

add_custom_target(build_all)
add_dependencies(build_all ${build_target_LIST})
