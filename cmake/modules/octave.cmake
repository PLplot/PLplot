# cmake/modules/octave.cmake
#
# Copyright (C) 2006  Alan W. Irwin
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
# along with the file PLplot; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA

# Module for determining octave bindings configuration options

# Options to enable Octave bindings
option(ENABLE_octave "Enable Octave bindings" OFF)

if(ENABLE_octave AND NOT BUILD_SHARED_LIBS)
  message(STATUS "WARNING: "
    "Octave requires shared libraries. Disabling octave bindings")
  set(ENABLE_octave OFF CACHE BOOL "Enable Octave bindings" FORCE)
endif(ENABLE_octave AND NOT BUILD_SHARED_LIBS)

if(ENABLE_octave)
#  find_program(OCTAVE octave)
  find_program(OCTAVE octave2.0)
  if(NOT OCTAVE)
    message(STATUS "WARNING: "
    "octave not found. Disabling octave bindings")
    set(ENABLE_octave OFF CACHE BOOL "Enable Octave bindings" FORCE)
  endif(NOT OCTAVE)
endif(ENABLE_octave)

if(ENABLE_octave)
  find_program(MKOCTFILE mkoctfile)
  if(NOT MKOCTFILE)
    message(STATUS "WARNING: "
    "mkoctfile not found. Disabling octave bindings")
    set(ENABLE_octave OFF CACHE BOOL "Enable Octave bindings" FORCE)
  endif(NOT MKOCTFILE)
endif(ENABLE_octave)

if(ENABLE_octave)
  #MATWRAP is the path+filename of the matwrap script.
  set(MATWRAP "${CMAKE_CURRENT_SOURCE_DIR}/bindings/octave/matwrap/matwrap")
  message(STATUS "Using built-in ${MATWRAP}")

  #OCTAVE_VERSION is the (dotted triplet) octave version.
  execute_process(
  COMMAND ${OCTAVE} --version
  OUTPUT_VARIABLE _OCTAVE_VERSION
  )
  string(REGEX REPLACE
  "^.*version ([0-9]\\.[0-9]\\.[0-9][0-9]).*$" 
  "\\1"
  OCTAVE_VERSION
  ${_OCTAVE_VERSION}
  )
  message(STATUS "OCTAVE_VERSION = ${OCTAVE_VERSION}")
  
  # PLPLOT_OCTAVE_DIR is the directory for installation of the PLplot_Octave
  # specific m files
  set(PLPLOT_OCTAVE_DIR ${CMAKE_INSTALL_PREFIX}/share/plplot_octave)
  
  # OCTAVE_PREFIX is the prefix where octave was installed.
  # N.B. this file method is really clunky, but we are forced to use
  # this method because as far as I know there is no method
  # of invoking octave scripts from the octave command line other than
  # with a file.
  
  file(WRITE ${CMAKE_BINARY_DIR}/octave_command
  "printf(octave_config_info(\"prefix\"));"
  )
  execute_process(
  COMMAND ${OCTAVE} -q -f octave_command
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
  OUTPUT_VARIABLE OCTAVE_PREFIX
  )
  message(STATUS "OCTAVE_PREFIX = ${OCTAVE_PREFIX}")
  
  #OCTAVE_M_DIR
  # N.B. I have found that localfcnfiledir works fine for Debian stable
  # versions of both octave 2.0.x and 2.1.x.  Therefore, unless somebody
  # complains will keep this simple logic rather than doing more complicated
  # localfcnfilepath logic for octave 2.0.x that occurs for ABS system.
  file(WRITE ${CMAKE_BINARY_DIR}/octave_command
  "printf(octave_config_info(\"localfcnfiledir\"));"
  )
  execute_process(
  COMMAND ${OCTAVE} -q -f octave_command
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
  OUTPUT_VARIABLE OCTAVE_M_DIR
  )
  message(STATUS "OCTAVE_M_DIR = ${OCTAVE_M_DIR}")

  set(OCTAVE_TESTING_MINIMUM_VERSION "2.1.0")
  transform_version(
  NUMERICAL_OCTAVE_TESTING_MINIMUM_VERSION
  ${OCTAVE_TESTING_MINIMUM_VERSION}
  )
  transform_version(NUMERICAL_OCTAVE_VERSION ${OCTAVE_VERSION})
  if(
   NUMERICAL_OCTAVE_VERSION
   LESS
   "${NUMERICAL_OCTAVE_TESTING_MINIMUM_VERSION}"
   )
    # Further processing of OCTAVE_M_DIR required if octave 2.0.x
  endif(
   NUMERICAL_OCTAVE_VERSION
   LESS
   "${NUMERICAL_OCTAVE_TESTING_MINIMUM_VERSION}"
   )
#  message(FATAL_ERROR "")
endif(ENABLE_octave)
