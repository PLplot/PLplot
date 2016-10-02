# Locate Lua executable, library, and headers.
# This module supersedes FindLua50 and FindLua51 (and also supports
# Lua-5.2) by using the
# lua -v command to determine version.
# This module defines
#  LUA_FOUND, if false, do not try to link to Lua
#  LUA_EXECUTABLE, full path of Lua executable used to determine LUA_VERSION
#  LUA_VERSION
#  LUA_LIBRARIES
#  LUA_INCLUDE_DIR, where to find lua.h
#
# Note that the expected include convention is
#  #include "lua.h"
# and not
#  #include <lua/lua.h>
# This is because, the lua location is not standardized and may exist
# in locations other than lua/

#=============================================================================
# CMake - Cross Platform Makefile Generator
# Copyright 2000-2011 Kitware, Inc., Insight Software Consortium
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
#
# * Redistributions of source code must retain the above copyright
#   notice, this list of conditions and the following disclaimer.
#
# * Redistributions in binary form must reproduce the above copyright
#   notice, this list of conditions and the following disclaimer in the
#   documentation and/or other materials provided with the distribution.
#
# * Neither the names of Kitware, Inc., the Insight Software Consortium,
#   nor the names of their contributors may be used to endorse or promote
#   products derived from this software without specific prior written
#   permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
# A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
# HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
# SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
# LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
# DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
# THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#
# ------------------------------------------------------------------------------
# The above copyright and license notice applies to distributions of
# CMake in source and binary form.  Some source files contain additional
# notices of original copyright by their contributors; see each source
# for details.  Third-party software packages supplied with CMake under
# compatible licenses provide their own copyright notices documented in
# corresponding subdirectories.
#
# ------------------------------------------------------------------------------
#
# CMake was initially developed by Kitware with the following sponsorship:
#
#  * National Library of Medicine at the National Institutes of Health
#    as part of the Insight Segmentation and Registration Toolkit (ITK).
#
#  * US National Labs (Los Alamos, Livermore, Sandia) ASC Parallel
#    Visualization Initiative.
#
#  * National Alliance for Medical Image Computing (NAMIC) is funded by the
#    National Institutes of Health through the NIH Roadmap for Medical Research,
#    Grant U54 EB005149.
#
#  * Kitware, Inc.

# No PATHS or anything else to make sure to not find Linux version
# on Wine.
find_program(LUA_EXECUTABLE lua
  )

if(LUA_EXECUTABLE)
  #N.B. lua -v outputs results to stderr (at least for lua-5.1) so
  #look for version string results in both stderr and stdout.
  execute_process(
    COMMAND ${LUA_EXECUTABLE} -v
    RESULT_VARIABLE LUA_ERR
    OUTPUT_VARIABLE LUA_VERSION_STRING
    ERROR_VARIABLE LUA_VERSION_STRING
    OUTPUT_STRIP_TRAILING_WHITESPACE
    ERROR_STRIP_TRAILING_WHITESPACE
    )
  if(LUA_ERR)
    message(WARNING "Lua executable broken.  Could not determine version string without an error")
    set(LUA_EXECUTABLE OFF CACHE BOOL "Lua executable location" FORCE)
  endif(LUA_ERR)
endif(LUA_EXECUTABLE)

if(LUA_EXECUTABLE)
  string(REGEX MATCH "5\\.[0-9]" LUA_VERSION "${LUA_VERSION_STRING}")
  message(STATUS "LUA_VERSION = ${LUA_VERSION}")
  string(REGEX MATCH "[0-9]$" LUA_VERSION_SUFFIX "${LUA_VERSION}")
  set(LUA_INCLUDE_SUFFIX_VERSIONS include/lua5${LUA_VERSION_SUFFIX} include/lua5.${LUA_VERSION_SUFFIX})
  set(LUA_LIBRARY_NAME_VERSIONS lua5${LUA_VERSION_SUFFIX} lua5.${LUA_VERSION_SUFFIX} lua-5.${LUA_VERSION_SUFFIX})
  if(NOT LUA_VERSION MATCHES "5\\.[0-9]")
    message(WARNING "Could not parse Lua version string")
    set(LUA_EXECUTABLE OFF CACHE BOOL "Lua executable location" FORCE)
  endif(NOT LUA_VERSION MATCHES "5\\.[0-9]")
endif(LUA_EXECUTABLE)

if(LUA_EXECUTABLE)
  find_path(LUA_INCLUDE_DIR lua.h
    HINTS
    $ENV{LUA_DIR}
    PATH_SUFFIXES ${LUA_INCLUDE_SUFFIX_VERSIONS} include/lua include
    PATHS
    ~/Library/Frameworks
    /Library/Frameworks
    /usr/local
    /usr
    /sw # Fink
    /opt/local # DarwinPorts
    /opt/csw # Blastwave
    /opt
    )
  message(STATUS "LUA_INCLUDE_DIR = ${LUA_INCLUDE_DIR}")

  find_library(LUA_LIBRARY
    NAMES ${LUA_LIBRARY_NAME_VERSIONS} lua
    HINTS
    $ENV{LUA_DIR}
    PATH_SUFFIXES lib64 lib
    PATHS
    ~/Library/Frameworks
    /Library/Frameworks
    /usr/local
    /usr
    /sw
    /opt/local
    /opt/csw
    /opt
    )

  if(LUA_LIBRARY)
    # include the math library for Unix
    if(UNIX AND NOT APPLE)
      find_library(LUA_MATH_LIBRARY m)
      set( LUA_LIBRARIES "${LUA_LIBRARY};${LUA_MATH_LIBRARY}" CACHE STRING "Lua Libraries")
      # For Windows and Mac, don't need to explicitly include the math library
    else(UNIX AND NOT APPLE)
      set( LUA_LIBRARIES "${LUA_LIBRARY}" CACHE STRING "Lua Libraries")
    endif(UNIX AND NOT APPLE)
  endif(LUA_LIBRARY)
  message(STATUS "LUA_LIBRARIES = ${LUA_LIBRARIES}")
endif(LUA_EXECUTABLE)

include(FindPackageHandleStandardArgs)
# handle the QUIET and REQUIRED arguments and set LUA_FOUND to TRUE if
# all listed variables are TRUE
find_package_handle_standard_args(Lua  DEFAULT_MSG  LUA_EXECUTABLE LUA_VERSION LUA_LIBRARIES LUA_INCLUDE_DIR)

mark_as_advanced(LUA_INCLUDE_DIR LUA_LIBRARIES LUA_LIBRARY LUA_MATH_LIBRARY)
