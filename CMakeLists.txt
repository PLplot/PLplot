# Top-level CMakeLists.txt for PLplot
###
### Process this file with cmake to produce Makefile
###
# Copyright (C) 2006-2017 Alan W. Irwin
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

# MAINTENANCE 2016-12-12
# We prefer to support only recent CMake versions and latest CMake
# policies to keep our build system as simple and as bug free as
# possible.  However, as a special concession for our modern (not
# enterprise or LTS) Linux distro users we allow them to use the
# system version of CMake which at this time is typically 3.0.2 (on
# Debian stable = Jessie) or later.  So we adopt 3.0.2 for the minimum
# CMake version on Linux and also that version for CMake policies on
# all platforms for CMake policy uniformity.  For non-Linux platforms
# we adopt 3.6.2 for the minimum version at this time.  3.6.2 is the
# version of CMake that is supplied by Cygwin at this time and note
# that rolling releases such as Debian testing and also
# MinGW-w64/MSYS2 already supply CMake-3.7.0.  Note, that if you
# are using a platform that does not supply CMake-3.0.2 (Linux)
# or CMake-3.6.2 (non-Linux) or later, then your best bet is
# to build a recent version of CMake for yourself or even download
# a binary version of it from Kitware that is suitable for your
# platform.

# Additional notes for Linux users of older CMake versions.  There is
# a CMake bug for 3.0 (a permissions issue for file(GENERATE...), see
# cmake/modules/plplot_functions.cmake for details) that has a
# workaround that works on Linux but that bug was fixed as of CMake
# 3.2.  In addition, CMake 3.3.2 solves a fundamental and quite nasty
# CMake find regression that occurred from 3.2.0 through 3.3.1. See
# threads entitled "[BUG] HINTS not correctly handled in find_program"
# and "find_program HINTS no longer preferred over PATH" on the
# cmake-devel list.  So for Linux users we warn of potential find
# issues if the actual CMake version they are running is in the range
# from 3.2.0 through 3.3.1.

# Adapted method suggested by Brad King for using
# different minimum versions for different platforms.

cmake_minimum_required(VERSION 3.0.2 FATAL_ERROR)
cmake_policy(VERSION 3.0.2)
project(plplot NONE)
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
  if(NOT CMAKE_VERSION VERSION_LESS 3.2 AND CMAKE_VERSION VERSION_LESS 3.3.2)
    message(STATUS "WARNING: CMAKE_VERSION = ${CMAKE_VERSION} is in the range from 3.2 through 3.3.1 which has
   a compromised find ability that was fixed in 3.3.2.  Please upgrade to 3.3.2 or greater.")
  endif(NOT CMAKE_VERSION VERSION_LESS 3.2 AND CMAKE_VERSION VERSION_LESS 3.3.2)
else(CMAKE_SYSTEM_NAME STREQUAL "Linux")
  # Latest CMake version as of 2016-12-12 for all platforms
  # other than Linux.
  cmake_minimum_required(VERSION 3.6.2 FATAL_ERROR)
  # Keep policy consistent for all platforms.
  cmake_policy(VERSION 3.0.2)
endif(CMAKE_SYSTEM_NAME STREQUAL "Linux")

# It is a fatal error if no working C compiler is available to build
# the PLplot core C library and core C examples.  All other compilers
# required by our bindings are optional in that if no working compiler
# of the kind needed is available, the associated bindings and
# examples are disabled.
enable_language(C)

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
    "Path to MinGW import libraries")
endif(MINGW)
if(BORLAND)
  get_filename_component(BORLANDBINPATH ${CMAKE_C_COMPILER} PATH)
  set(BORLANDLIBPATH ${BORLANDBINPATH}/../Lib/PSDK
    CACHE FILEPATH
    "Path to Borland import libraries")
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

  # Use same BUILDNAME logic as the CTest module except
  # we allow a BUILDNAME suffix whose value is specified
  # by the user as PLPLOT_BUILDNAME_SUFFIX.  In order
  # for this logic to work it must precede including the
  # CTest module which configures DartConfiguration.tcl
  # with BUILDNAME as determined here.

  if(NOT BUILDNAME)
    set(DART_COMPILER "${CMAKE_CXX_COMPILER}")
    if(NOT DART_COMPILER)
      set(DART_COMPILER "${CMAKE_C_COMPILER}")
    endif()
    if(NOT DART_COMPILER)
      set(DART_COMPILER "unknown")
    endif()
    if(WIN32)
      set(DART_NAME_COMPONENT "NAME_WE")
    else()
      set(DART_NAME_COMPONENT "NAME")
    endif()
    if(NOT BUILD_NAME_SYSTEM_NAME)
      set(BUILD_NAME_SYSTEM_NAME "${CMAKE_SYSTEM_NAME}")
    endif()
    if(WIN32)
      set(BUILD_NAME_SYSTEM_NAME "Win32")
    endif()
    if(UNIX OR BORLAND)
      get_filename_component(DART_COMPILER_NAME
        "${DART_COMPILER}" ${DART_NAME_COMPONENT})
    else()
      get_filename_component(DART_COMPILER_NAME
        "${CMAKE_MAKE_PROGRAM}" ${DART_NAME_COMPONENT})
    endif()
    if(DART_COMPILER_NAME MATCHES "devenv")
      GET_VS_VERSION_STRING("${CMAKE_GENERATOR}" DART_COMPILER_NAME)
    endif()
    set(BUILDNAME "${BUILD_NAME_SYSTEM_NAME}-${DART_COMPILER_NAME}")
  endif()
  # This is only logic that is different.
  set(BUILDNAME "${BUILDNAME}${PLPLOT_BUILDNAME_SUFFIX}")

  # According to
  # <https://cmake.org/Wiki/CMake_Testing_With_CTest>
  # the following command executes the required "enable_testing()"
  # command and also enable the creation and submission of a dashboard
  # (which is defined as the result of a test run, reformatted for easy review)
  # to our dashboard server (the PLplot_git "project" at my.cdash.org whose
  # details are given by CTestConfig.cmake and whose dashboard viewer
  # URL is <http://my.cdash.org/index.php?project=PLplot_git>).
  include(CTest)

  # Customize default values set by include(CTest).  Result must be in
  # build tree so copy it there.
  configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/CTestCustom.ctest.in
    ${CMAKE_CURRENT_BINARY_DIR}/CTestCustom.ctest
    COPYONLY
    )
  # N.B. we execute add_test(...) in the plplot_test
  # subdirectory to implement the various tests that are
  # run by ctest.
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
  README.cumulated_release
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
# process configurable files in those directories.  N.B. Order is
# important here at the cmake stage because targets must be processed
# by cmake in a specific order e.g., due to get_target_property
# invocations or if(TARGET...) commands.  Note, this order has nothing
# to do with the order which make processes these subdirectories at
# build time.  That build-time order is determined by the dependencies
# between targets and also by file dependencies that are established
# by the cmake configuration files.

add_subdirectory(fonts)
add_subdirectory(lib)
add_subdirectory(include)
add_subdirectory(src)
add_subdirectory(data)
add_subdirectory(bindings)
add_subdirectory(drivers)
add_subdirectory(utils)
# plplot_test before examples because of workaround_file_generate_bug target
add_subdirectory(plplot_test)
add_subdirectory(examples)
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
  # Ignore Mac OS X generated file/directory attribute storage files.
  "\\\\.DS_Store"
  "\\\\._\\\\.DS_Store"
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
