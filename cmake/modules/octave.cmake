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
if(DEFAULT_NO_BINDINGS)
  option(ENABLE_octave "Enable Octave bindings" OFF)
else(DEFAULT_NO_BINDINGS)
  option(ENABLE_octave "Enable Octave bindings" ON)
endif(DEFAULT_NO_BINDINGS)

if(ENABLE_octave AND NOT BUILD_SHARED_LIBS)
  message(STATUS "WARNING: "
    "Octave requires shared libraries. Disabling octave bindings")
  set(ENABLE_octave OFF CACHE BOOL "Enable Octave bindings" FORCE)
endif(ENABLE_octave AND NOT BUILD_SHARED_LIBS)

if(ENABLE_octave AND NOT PERL_FOUND)
  message(STATUS "WARNING: "
    "The octave build requires perl. Disabling octave bindings")
  set(ENABLE_octave OFF CACHE BOOL "Enable Octave bindings" FORCE)
endif(ENABLE_octave AND NOT PERL_FOUND)

if(ENABLE_octave)
  find_program(OCTAVE octave)
  if(OCTAVE)
    message(STATUS "OCTAVE = ${OCTAVE}")
  else(OCTAVE)
    message(STATUS "WARNING: "
    "octave not found. Disabling octave bindings")
    set(ENABLE_octave OFF CACHE BOOL "Enable Octave bindings" FORCE)
  endif(OCTAVE)
endif(ENABLE_octave)

if(ENABLE_octave)
  find_program(MKOCTFILE mkoctfile)
  if(MKOCTFILE)
    message(STATUS "MKOCTFILE = ${MKOCTFILE}")
  else(MKOCTFILE)
    message(STATUS "WARNING: "
    "mkoctfile not found. Disabling octave bindings")
    set(ENABLE_octave OFF CACHE BOOL "Enable Octave bindings" FORCE)
  endif(MKOCTFILE)
endif(ENABLE_octave)

if(ENABLE_octave)
  #OCTAVE_VERSION is the (dotted triplet) octave version.
  execute_process(
  COMMAND ${OCTAVE} --version
  OUTPUT_VARIABLE _OCTAVE_VERSION
  )
  string(REGEX REPLACE
  "^.*version ([0-9]\\.[0-9]\\.[0-9]*).*$" 
  "\\1"
  OCTAVE_VERSION
  ${_OCTAVE_VERSION}
  )
  message(STATUS "OCTAVE_VERSION = ${OCTAVE_VERSION}")

  # The following if block is required to keep from repeated pre-pending
  # of OCTAVE_INCLUDE_PATH_TRIMMED onto OCTAVE_INCLUDE_PATH which would
  # create 2^n components to OCTAVE_INCLUDE_PATH for n cmake calls.
  # Note, all externally used results (OCTAVE_INCLUDE_PATH, OCTAVE_LIBRARIES,
  # OCTINTERP_LIBRARIES, and ENABLE_octave) are cached so skipping everything
  # if OCTAVE_INCLUDE_PATH is defined from the previous cmake run should be
  # fine.
  if(NOT DEFINED OCTAVE_INCLUDE_PATH)
    find_path(
    OCTAVE_INCLUDE_PATH
    oct.h
    PATH_SUFFIXES octave-${OCTAVE_VERSION}/octave
    )

    find_library(
    OCTAVE_LIBRARIES
    octave
    PATH_SUFFIXES octave-${OCTAVE_VERSION}
    )
    
    find_library(
    OCTINTERP_LIBRARIES
    octinterp
    PATH_SUFFIXES octave-${OCTAVE_VERSION}
    )

    if(OCTAVE_INCLUDE_PATH AND OCTAVE_LIBRARIES AND OCTINTERP_LIBRARIES)
      # If first octave include path has trailing /octave, then must have
      # second include path without that trailing /octave.
      string(REGEX REPLACE "/octave$" ""
      OCTAVE_INCLUDE_PATH_TRIMMED
      ${OCTAVE_INCLUDE_PATH}
      )
      if(NOT OCTAVE_INCLUDE_PATH_TRIMMED STREQUAL "${OCTAVE_INCLUDE_PATH}")
        set(OCTAVE_INCLUDE_PATH 
	${OCTAVE_INCLUDE_PATH_TRIMMED} ${OCTAVE_INCLUDE_PATH}
	CACHE INTERNAL ""
	)
      endif(NOT OCTAVE_INCLUDE_PATH_TRIMMED STREQUAL "${OCTAVE_INCLUDE_PATH}")
    else(OCTAVE_INCLUDE_PATH AND OCTAVE_LIBRARIES AND OCTINTERP_LIBRARIES)
      message(STATUS "WARNING: "
      "octave headers and/or library not found. Disabling octave bindings")
      set(ENABLE_octave OFF CACHE BOOL "Enable Octave bindings" FORCE)
    endif(OCTAVE_INCLUDE_PATH AND OCTAVE_LIBRARIES AND OCTINTERP_LIBRARIES) 
  endif(NOT DEFINED OCTAVE_INCLUDE_PATH)
  message(STATUS "OCTAVE_LIBRARIES = ${OCTAVE_LIBRARIES}")
  message(STATUS "OCTINTERP_LIBRARIES = ${OCTINTERP_LIBRARIES}")
  message(STATUS "OCTAVE_INCLUDE_PATH = ${OCTAVE_INCLUDE_PATH}")
endif(ENABLE_octave)

if(ENABLE_octave)
  check_perl_modules(PERL_XML_PARSER XML::Parser)
  check_perl_modules(PERL_XML_DOM XML::DOM)
  #MATWRAP is the path+filename of the matwrap script.
  set(MATWRAP "${CMAKE_CURRENT_SOURCE_DIR}/bindings/octave/matwrap/matwrap")
  message(STATUS "MATWRAP = ${MATWRAP}")
  #MATWRAP_PATH is the path of the matwrap script
  get_filename_component(MATWRAP_PATH ${MATWRAP} PATH)
  message(STATUS "MATWRAP_PATH = ${MATWRAP_PATH}")

  # PLPLOT_OCTAVE_DIR is the directory for installation of the PLplot_Octave
  # specific m files
  set(PLPLOT_OCTAVE_DIR ${CMAKE_INSTALL_DATADIR}/plplot_octave)
  message(STATUS "PLPLOT_OCTAVE_DIR = ${PLPLOT_OCTAVE_DIR}")
  
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
  #message(STATUS "OCTAVE_PREFIX = ${OCTAVE_PREFIX}")
  
  # Logic that depends on octave version
  transform_version(NUMERICAL_OCTAVE_TESTING_MINIMUM_VERSION "2.1.0")
  transform_version(NUMERICAL_OCTAVE_VERSION "${OCTAVE_VERSION}")
  if(
   NUMERICAL_OCTAVE_VERSION
   LESS
   "${NUMERICAL_OCTAVE_TESTING_MINIMUM_VERSION}"
   )
    # octave-2.0 logic.
    #_OCTAVE_M_DIR
    file(WRITE ${CMAKE_BINARY_DIR}/octave_command
    "printf(octave_config_info(\"localfcnfilepath\"));"
    )
    execute_process(
    COMMAND ${OCTAVE} -q -f octave_command
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    OUTPUT_VARIABLE __OCTAVE_M_DIR
    )
    #message(STATUS "__OCTAVE_M_DIR = ${__OCTAVE_M_DIR}")    
    string(REGEX REPLACE
    "^.*:([^:][^:]*)//$"
    "\\1"
    _OCTAVE_M_DIR
    ${__OCTAVE_M_DIR}
    )
    #message(STATUS "_OCTAVE_M_DIR = ${_OCTAVE_M_DIR}")    
    #OCTAVE_OCT_DIR
    file(WRITE ${CMAKE_BINARY_DIR}/octave_command
    "printf(octave_config_info(\"localoctfilepath\"));"
    )
    execute_process(
    COMMAND ${OCTAVE} -q -f octave_command
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    OUTPUT_VARIABLE _OCTAVE_OCT_DIR
    )
    #message(STATUS "_OCTAVE_OCT_DIR = ${_OCTAVE_OCT_DIR}")    
    string(REGEX REPLACE
    "^.*:([^:][^:]*)//$"
    "\\1"
    OCTAVE_OCT_DIR
    ${_OCTAVE_OCT_DIR}
    )
    #message(STATUS "OCTAVE_OCT_DIR = ${OCTAVE_OCT_DIR}")    
  else(
   NUMERICAL_OCTAVE_VERSION
   LESS
   "${NUMERICAL_OCTAVE_TESTING_MINIMUM_VERSION}"
   )
    # octave-2.1 (or higher) logic.
    #_OCTAVE_M_DIR
    file(WRITE ${CMAKE_BINARY_DIR}/octave_command
    "printf(octave_config_info(\"localfcnfiledir\"));"
    )
    execute_process(
    COMMAND ${OCTAVE} -q -f octave_command
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    OUTPUT_VARIABLE _OCTAVE_M_DIR
    )
    #OCTAVE_OCT_DIR
    file(WRITE ${CMAKE_BINARY_DIR}/octave_command
    "printf(octave_config_info(\"localoctfiledir\"));"
    )
    execute_process(
    COMMAND ${OCTAVE} -q -f octave_command
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    OUTPUT_VARIABLE OCTAVE_OCT_DIR
    )
  endif(
   NUMERICAL_OCTAVE_VERSION
   LESS
   "${NUMERICAL_OCTAVE_TESTING_MINIMUM_VERSION}"
   )
  # Replace the OCTAVE_PREFIX with the PLplot prefix in OCTAVE_M_DIR
  string(REPLACE
  "${OCTAVE_PREFIX}" 
  "${CMAKE_INSTALL_PREFIX}"
  OCTAVE_M_DIR
  ${_OCTAVE_M_DIR}
  )
  message(STATUS "OCTAVE_M_DIR = ${OCTAVE_M_DIR}")
  
  # Transform OCTAVE_OCT_DIR if prefixes not the same.
  if(NOT CMAKE_INSTALL_PREFIX STREQUAL "${OCTAVE_PREFIX}")
    set(OCTAVE_OCT_DIR ${PLPLOT_OCTAVE_DIR})
  endif(NOT CMAKE_INSTALL_PREFIX STREQUAL "${OCTAVE_PREFIX}") 
  message(STATUS "OCTAVE_OCT_DIR = ${OCTAVE_OCT_DIR}")
  
  # Use DEFINE_PL_DOUBLE in bindings/octave/plplot_octave.h.in
  # (I [AWI] presume this is to simplify the #include's).
  if(PL_DOUBLE)
    set(DEFINE_PL_DOUBLE "#define PL_DOUBLE")
  endif(PL_DOUBLE)
endif(ENABLE_octave)
