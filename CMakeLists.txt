# Top-level CMakeLists.txt for PLplot
###
### Process this file with cmake to produce Makefile
###
# Copyright (C) 2006-2012 Alan W. Irwin
#
# This file is part of PLplot.
#
# PLplot is free software; you can redistribute it and/or modify
# it under the terms of the GNU Library General Public License as published
# by the Free Software Foundation; version 2 of the License.
#
# PLplot is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Library General Public License for more details.
#
# You should have received a copy of the GNU Library General Public License
# along with PLplot; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA

# Recommended way to do user overrides of C-related variables, is given at
# http://public.kitware.com/pipermail/cmake/2006-July/010334.html.
# See also, CMakeCInformation.cmake.  There are also C++, and Fortran-specific
# overrides that can be done.  However, I prefer putting overrides in just
# one file so I use the overall variable CMAKE_USER_MAKE_RULES_OVERRIDE
# instead.
set(
  CMAKE_USER_MAKE_RULES_OVERRIDE
  ${CMAKE_SOURCE_DIR}/cmake/UserOverride.cmake
  )

cmake_minimum_required(VERSION 2.8.9 FATAL_ERROR)

# cmake-2.8.12.1 (2.8.12 screwed up the implementation) introduced a
# corrected implementation of keyword signatures for
# target_link_libraries which are preferred now over the traditional
# plain signatures.  There are a lot of implications concerning these
# preferred keyword signatures which I don't understand at the moment,
# but it is clear some modifications to our build system will be
# required to start using keyword signatures appropriately.  But it is
# also clear we can do nothing about this until we can guarantee that
# all our users are running 2.8.12.1 or higher.  Therefore, until we
# bump the actual minimum version to 2.8.12.1 in the above
# cmake_minimum_required call and deal with all the errors in our
# CMake code that will generate, quiet the warnings about this issue
# by explicitly setting policies CMP0022 and CMP0023 (if they exist
# for whatever CMake version the user is running) to OLD.
if(POLICY CMP0022)
  message(STATUS "Explicitly setting policy CMP0022 to OLD")
  cmake_policy(SET CMP0022 OLD)
endif(POLICY CMP0022)
if(POLICY CMP0023)
  message(STATUS "Explicitly setting policy CMP0023 to OLD")
  cmake_policy(SET CMP0023 OLD)
endif(POLICY CMP0023)
if(POLICY CMP0026)
  message(STATUS "Explicitly setting policy CMP0026 to OLD")
  cmake_policy(SET CMP0026 OLD)
endif(POLICY CMP0026)

# It is a fatal error if no working C compiler is available to build
# the PLplot core C library and core C examples.  All other compilers
# required by our bindings are optional in that if no working compiler
# of the kind needed is available, the associated bindings and
# examples are disabled.
project(plplot C)

message(STATUS "CMake version = ${CMAKE_VERSION}")
message(STATUS "CMAKE_SYSTEM_NAME = ${CMAKE_SYSTEM_NAME}")

# Set unambiguous names for these variables as a reminder.
set(WIN32_AND_NOT_CYGWIN ${WIN32})
if(WIN32 OR CYGWIN)
  set(WIN32_OR_CYGWIN ON)
else(WIN32 OR CYGWIN)
  set(WIN32_OR_CYGWIN OFF)
endif(WIN32 OR CYGWIN)

#foreach(policy RANGE 0 9)
#  cmake_policy(GET CMP000${policy} policy_result)
#  message(STATUS "Policy CMP000${policy} is ${policy_result}")
#endforeach(policy RANGE 0 9)

set(PACKAGE plplot)

# Locations where PLplot cmake build system first looks for cmake modules.
set(CMAKE_MODULE_PATH
  ${PROJECT_SOURCE_DIR}/cmake/modules
  ${PROJECT_SOURCE_DIR}/cmake/modules/language_support/cmake
  ${PROJECT_SOURCE_DIR}/cmake/modules/language_support/cmake-2.8
  )

# We need the path to the MinGW/Borland compiler in order to find
# the import libraries for system libraries.
if(MINGW)
  get_filename_component(MINGWBINPATH ${CMAKE_C_COMPILER} PATH)
  set(MINGWLIBPATH ${MINGWBINPATH}/../lib
    CACHE FILEPATH
    DOCSTRING "Path to MinGW import libraries")
endif(MINGW)
if(BORLAND)
  get_filename_component(BORLANDBINPATH ${CMAKE_C_COMPILER} PATH)
  set(BORLANDLIBPATH ${BORLANDBINPATH}/../Lib/PSDK
    CACHE FILEPATH
    DOCSTRING "Path to Borland import libraries")
endif(BORLAND)

# Version data that need modification for each release.
include(plplot_version)
# parameters to control overall cmake behaviour.

# Configure PLplot component variables....

include(plplot)

# Use configured variables to process configurable top-level files.

# This CMake-configured template file is processed further by sed for
# abi-compliance-checker.
configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/abi-compliance-checker.xml.template.in
  ${CMAKE_CURRENT_BINARY_DIR}/abi-compliance-checker.xml.template
  @ONLY
  )

configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/plplot_config.h.in
  ${CMAKE_CURRENT_BINARY_DIR}/plplot_config.h
  )
# Allow access to the generated plplot_config.h for this build.
add_definitions("-DPLPLOT_HAVE_CONFIG_H")
# Install top-level files

# Enable testing framework for examples
if(BUILD_TEST)
  include(CTest)
endif(BUILD_TEST)

# Disable warnings about deprecated functions (Visual C++ 2005)
if(MSVC_VERSION GREATER 1399)
  add_definitions("/D_CRT_SECURE_NO_DEPRECATE")
endif(MSVC_VERSION GREATER 1399)

# Borland Compiler must compile in ANSII mode
if(BORLAND)
  add_definitions(-A)
endif(BORLAND)

set(top_level_DOCFILES
  ABOUT
  AUTHORS
  COPYING.LIB
  ChangeLog.release
  Copyright
  FAQ
  NEWS
  PROBLEMS
  README
  README.release
  README.testing
  )
install(FILES ${top_level_DOCFILES} DESTINATION ${DOC_DIR})

if(BUILD_SHARED_LIBS AND WIN32_OR_CYGWIN)
  # For platforms (currently WIN32 or Cygwin, although the Cygwin version
  # of CMake may support this in future since -rpath apparently does work
  # on that platform) where CMake does not use -rpath, use a workaround
  # where all dll's are collected in the dll subdirectory of the build tree.
  set(USE_DLL_SUBDIRECTORY ON)
else(BUILD_SHARED_LIBS AND WIN32_OR_CYGWIN)
  set(USE_DLL_SUBDIRECTORY OFF)
endif(BUILD_SHARED_LIBS AND WIN32_OR_CYGWIN)

# in windows all created dlls are gathered in the dll directory
# if you add this directory to your PATH all shared libraries are available
if(USE_DLL_SUBDIRECTORY)
  set(LIBRARY_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR}/dll)
endif(USE_DLL_SUBDIRECTORY)

# Process other directories including using configured variables to
# process configurable files in those directories.
# N.B. Order is important here at the cmake stage because targets must
# be processed by cmake in a specific order e.g., due to get_target_property
# invocations.  Note, this order has nothing to do with the order which make
# processes these subdirectories at build time.  That build-time order
# is determined by the dependencies between targets and also by file
# dependencies that are established by the cmake configuration files.

add_subdirectory(fonts)
add_subdirectory(lib)
add_subdirectory(include)
add_subdirectory(src)
add_subdirectory(data)
add_subdirectory(bindings)
add_subdirectory(drivers)
add_subdirectory(utils)
add_subdirectory(examples)
add_subdirectory(plplot_test)
add_subdirectory(scripts)
add_subdirectory(doc)
add_subdirectory(www)
add_subdirectory(pkgcfg)
summary()

if(PREBUILD_DIST)
  # Pre-build everything required for a distribution tarball and copy it to
  # the source tree (if build tree is different from source tree).

  # List of targets that must be (pre-)built.
  set(DIST_TARGETS)

  if(BUILD_PRINT)
    list(APPEND DIST_TARGETS print)
  endif(BUILD_PRINT)

  if(BUILD_INFO)
    list(APPEND DIST_TARGETS info)
  endif(BUILD_INFO)

  if(BUILD_MAN)
    list(APPEND DIST_TARGETS man)
  endif(BUILD_MAN)

  if(BUILD_HTML)
    list(APPEND DIST_TARGETS html)
  endif(BUILD_HTML)

  if(BUILD_DOX_DOC)
    list(APPEND DIST_TARGETS build_doxygen)
  endif(BUILD_DOX_DOC)

  if(CMAKE_BINARY_DIR STREQUAL "${CMAKE_SOURCE_DIR}")
    add_custom_target(prebuild_dist)
  else(CMAKE_BINARY_DIR STREQUAL "${CMAKE_SOURCE_DIR}")
    # copy prebuilds back to source tree.
    # N.B. copy_directory quietly creates an empty directory with no
    # error condition if the source directory does not exist.
    add_custom_target(
      prebuild_dist
      COMMAND ${CMAKE_COMMAND} -E remove_directory
      ${CMAKE_SOURCE_DIR}/doc/doxygen
      COMMAND ${CMAKE_COMMAND} -E copy_directory
      ${CMAKE_BINARY_DIR}/doc/doxygen
      ${CMAKE_SOURCE_DIR}/doc/doxygen
      COMMAND cp
      `cat static_built_files ${INFO_MANIFEST} ${MAN_MANIFEST} ${HTML_MANIFEST}`
      ${CMAKE_SOURCE_DIR}/doc/docbook/src
      WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/doc/docbook/src
      )
  endif(CMAKE_BINARY_DIR STREQUAL "${CMAKE_SOURCE_DIR}")
  add_dependencies(prebuild_dist ${DIST_TARGETS})
endif(PREBUILD_DIST)

#
# Packing stuff
#
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "The Scientific Plotting Library PLplot")
set(CPACK_PACKAGE_VENDOR "PLplot development team")
set(CPACK_PACKAGE_DESCRIPTION_FILE ${CMAKE_CURRENT_SOURCE_DIR}/README)
set(CPACK_SET_DESTDIR ON)
if(WIN32_AND_NOT_CYGWIN)
  set(CPACK_GENERATOR ZIP)
else(WIN32_AND_NOT_CYGWIN)
  set(CPACK_GENERATOR TGZ)
endif(WIN32_AND_NOT_CYGWIN)
set(
  CPACK_SOURCE_PACKAGE_FILE_NAME
  "plplot-${PLPLOT_VERSION}"
  CACHE INTERNAL "tarball basename"
  )
set(CPACK_SOURCE_GENERATOR TGZ)
# The following components are regex's to match anywhere (unless anchored)
# in absolute path + filename to find files or directories to be excluded
# from source tarball.
set(CPACK_SOURCE_IGNORE_FILES
  "\\\\#.*$"
  "~$"
  "^${PROJECT_SOURCE_DIR}.*/\\\\.git/"
  "^${PROJECT_SOURCE_DIR}/old/"
  "^${PROJECT_SOURCE_DIR}/sys/mac"
  "^${PROJECT_SOURCE_DIR}/sys/os2"
  "^${PROJECT_SOURCE_DIR}/sys/unix"
  "^${PROJECT_SOURCE_DIR}/sys/dos/msc"
  "^${PROJECT_SOURCE_DIR}/sys/dos/bcc"
  "^${PROJECT_SOURCE_DIR}/sys/dos/emx"
  "^${PROJECT_SOURCE_DIR}/sys/win-tk"
  "^${PROJECT_SOURCE_DIR}/sys/win32"
  "^${PROJECT_SOURCE_DIR}/debian"
  )
#message("CPACK_SOURCE_IGNORE_FILES = ${CPACK_SOURCE_IGNORE_FILES}")
include(CPack)

# Path to native build for executables required in the build process.
# This is only required for cross compiling
if(CMAKE_CROSSCOMPILING)
  set(CMAKE_NATIVE_BINARY_DIR NATIVEDIR-NOTFOUND CACHE FILEPATH "Point to the native build directory")
endif(CMAKE_CROSSCOMPILING)
