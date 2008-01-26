# cmake/modules/pkg-config.cmake
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

# Module for determining pkg-config configuration variables related to the
# install-tree build of the examples.
# Also create useful macros called pkg_check_pkgconfig to emulate the
# pkgconfig macro using the pkg_check_modules macro,
# cmake_to_pkg_config_link_flags to process CMake link flags into 
# pkg-config standard form for the configured output *.pc files, and
# pkg_config_to_cmake_link_flags to process input link flags delivered
# by pkg-config into CMake standard form.

# The following variables are set:
# PKG_CONFIG_EXECUTABLE	  - name of pkg-config executable, but can also be
# 			    used for logical control with, e.g.,
# 			    if(PKG_CONFIG_EXECUTABLE)
# pkg_config_true	  - if null string, then example builds done with
#  			    pkg-config method.  if "#" then that method is
#			    commented out.
# pkg_config_false	  - always "#" to comment out example builds with
#  			    plplot_config method which does not exist for
#			    cmake build system.
# PKG_CONFIG_DIR	  - install location for configured PLplot
#  			    pkg-config files.
# PKG_CONFIG_ENV	  - the string PKG_CONFIG_PATH=${PKG_CONFIG_DIR} which
#  			    is used in example builds.

include(FindPkgConfig)

set(pkg_config_false "#")

if(PKG_CONFIG_EXECUTABLE)
  message(STATUS "Looking for pkg-config - found")
  set(pkg_config_true "")
  set(PKG_CONFIG_DIR ${LIB_DIR}/pkgconfig)
  set(PKG_CONFIG_ENV PKG_CONFIG_PATH=${PKG_CONFIG_DIR})
else(PKG_CONFIG_EXECUTABLE)
  message(STATUS "Looking for pkg-config - not found")
  message(STATUS
  "WARNING: Install-tree build will be disabled.")
  # Turn off pkg-config build.  (This means both build methods will be
  # commented out.)
  set(pkg_config_true "#")
endif(PKG_CONFIG_EXECUTABLE)

macro(pkg_check_pkgconfig _package _include_DIR _link_DIR _link_FLAGS _cflags)
  # Similar to legacy pkgconfig only these results are derived
  # from pkg_check_modules and therefore are returned as lists rather than
  # blank-delimited strings.  Also, the _link_FLAGS value is converted
  # to the preferred CMake form via the cmake_link_flags macro.

  # N.B. if using this macro in more than one context, cache clashes will
  # occur unless optional trailing prefix argument is specified to distinguish
  # different contexts.
  set(_prefix ${ARGN})
  if(NOT _prefix)
    set(_prefix "_PKGCONFIG_TMP")
  endif(NOT _prefix)
  
  _pkg_check_modules_internal(0 0 ${_prefix} "${_package}")
  if (${_prefix}_FOUND)
    set(${_include_DIR} ${${_prefix}_INCLUDE_DIRS})
    set(${_link_DIR}    ${${_prefix}_LIBRARY_DIRS})
    cmake_link_flags(${_link_FLAGS}  "${${_prefix}_LDFLAGS}")
    set(${_cflags}      ${${_prefix}_CFLAGS})
    set(_return_VALUE 0)
  else(${_prefix}_FOUND)
    set(${_include_DIR})
    set(${_link_DIR})
    set(${_link_FLAGS})
    set(${_cflags})
    set(_return_VALUE 1)
  endif(${_prefix}_FOUND)
  #message("${_prefix}_FOUND = ${${_prefix}_FOUND}")
  #message("${_include_DIR} = ${${_include_DIR}}")
  #message("${_link_DIR} = ${${_link_DIR}}")
  #message("${_link_FLAGS} = ${${_link_FLAGS}}")
  #message("${_cflags} = ${${_cflags}}")
endmacro(pkg_check_pkgconfig)

macro(pkg_config_link_flags _link_flags_out _link_flags_in)
  # Transform link flags into a form that is suitable to be used for
  # output pkg-config (*.pc) files.
  # N.B. ${_link_flags_in} must be a string and not a list.

  #message("(original link flags) = ${_link_flags_in}")
  # Convert link flags to a blank-delimited string.
  string(REGEX REPLACE ";" " " ${_link_flags_out} "${_link_flags_in}")
  #message("(blanks) ${_link_flags_out} = ${${_link_flags_out}}")

  # Replace actual library names with the -LPATHNAME and -lLIBRARYNAME form
  # since it appears pkg-config handles that latter form much better (with
  # regard to keeping the correct order and eliminating duplicates).

  # These REGEX REPLACE's won't actually replace anything on bare windows since
  # library names are not of this form on that platform.  Something to be
  # considered later if we decide to use pkg-config on bare windows.

  # This logic will need to be expanded for Unix platforms other than
  # Mac OS X and Linux.
  if(APPLE)
    set(suffix_list ".so" ".a" ".dylib")
  else(APPLE)
    set(suffix_list ".so" ".a")
  endif(APPLE)

  foreach(suffix ${suffix_list})
    string(
    REGEX REPLACE "(/[^ ]*)/lib([^ ]*)\\${suffix}" "-L\\1 -l\\2"
    ${_link_flags_out}
    "${${_link_flags_out}}"
    )
    #message("(${suffix}) ${_link_flags_out} = ${${_link_flags_out}}")
  endforeach(suffix ${suffix_list})

  if(APPLE)
    # For Mac OS X transform frameworks information into correct form.
    string(
    REGEX REPLACE
    "/System/Library/Frameworks/([^ ]*)\\.framework"
    "-framework \\1"
    ${_link_flags_out}
    ${${_link_flags_out}}
    )
    #message("(frameworks) ${_link_flags_out} = ${${_link_flags_out}}")
  endif(APPLE)

endmacro(pkg_config_link_flags)

macro(cmake_link_flags _link_flags_out _link_flags_in)
  # Transform link flags delivered by pkg-config into the best form
  # for CMake.
  # N.B. may need revision for windows since the current assumption
  # is pkg-config delivers link flags using the -L and -l options which
  # may not be the case for windows.
  # N.B. ${_link_flags_in} must be a string and not a list.

  if("${_link_flags_in}" STREQUAL "")
    set(${_link_flags_out})
  else("${_link_flags_in}" STREQUAL ""})
    #message("(original link flags) = ${_link_flags_in}")
    # Convert link flags to a list.
    string(REGEX REPLACE " " ";" _link_flags_list "${_link_flags_in}")
    # Extract list of directories from -L options.
    list(LENGTH _link_flags_list _link_flags_length)
    math(EXPR _link_flags_length "${_link_flags_length} - 1")
    set(_index_list)
    set(_link_directory_list)
    foreach(_list_index RANGE ${_link_flags_length})
      list(GET _link_flags_list ${_list_index} _list_element)
      string(REGEX REPLACE "^-L" "" _list_element1 ${_list_element})
      if(_list_element STREQUAL "-L${_list_element1}")
        list(APPEND _index_list ${_list_index})
        list(APPEND _link_directory_list ${_list_element1})
      endif(_list_element STREQUAL "-L${_list_element1}")
    endforeach(_list_index RANGE ${_link_flags_length})
    #message("_index_list = ${_index_list}")
    if("${_index_list}" STREQUAL "")
    else("${_index_list}" STREQUAL "")
      # Remove -L options from list.
      list(REMOVE_AT _link_flags_list ${_index_list})
    endif("${_index_list}" STREQUAL "")
    #message("_link_directory_list = ${_link_directory_list}")
    #message("_link_flags_list (without -L options) = ${_link_flags_list}")
  
    # Derive ${_link_flags_out} from _link_flags_list with -l options 
    # replaced by complete pathname of library.
    list(LENGTH _link_flags_list _link_flags_length)
    math(EXPR _link_flags_length "${_link_flags_length} - 1")
    set(${_link_flags_out})
    foreach(_list_index RANGE ${_link_flags_length})
      list(GET _link_flags_list ${_list_index} _list_element)
      string(REGEX REPLACE "^-l" "" _list_element1 ${_list_element})
      if(_list_element STREQUAL "-l${_list_element1}")
        set(_library_pathname "_library_pathname-NOTFOUND")
        find_library(
         _library_pathname 
         ${_list_element1}
         PATHS ${_link_directory_list}
         NO_DEFAULT_PATH
        )
	# Try second time (without NO_DEFAULT_PATH) just in case pkg-config
	# specified some system libraries without corresponding -L options.
        find_library(
         _library_pathname 
         ${_list_element1}
         PATHS ${_link_directory_list}
        )
        if(NOT _library_pathname)
          message(
  	"Cannot find library corresponding to linker option ${_list_element}"
  	)
          message(
  	"original link flags delivered by pkg-config = ${_link_flags_in}"
  	)
          message(FATAL_ERROR "FATAL ERROR in cmake_link_flags macro")
        endif(NOT _library_pathname)
        list(APPEND ${_link_flags_out} ${_library_pathname})
      else(_list_element STREQUAL "-L${_list_element1}")
        # link options that are not -L or -l passed through in correct order
        # in ${_link_flags_out}.
        list(APPEND ${_link_flags_out} ${_list_element})
      endif(_list_element STREQUAL "-l${_list_element1}")
    endforeach(_list_index RANGE ${_link_flags_length})
    #message("${_link_flags_out} = ${${_link_flags_out}}")
  endif("${_link_flags_in}" STREQUAL "")
endmacro(cmake_link_flags)
