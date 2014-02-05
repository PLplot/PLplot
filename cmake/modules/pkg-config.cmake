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
# pkgconfig macro using the pkg_check_modules macro;
# pkg_config_link_flags to process CMake link flags into 
# pkg-config standard form for the configured output *.pc files, and
# cmake_link_flags to process input link flags delivered
# by pkg-config into CMake standard form.

# The following variables are set:
# PKG_CONFIG_EXECUTABLE	  - name of pkg-config executable, but can also be
# 			    used for logical control with, e.g.,
# 			    if(PKG_CONFIG_EXECUTABLE)
# PKG_CONFIG_DIR	  - install location for configured PLplot
#  			    pkg-config files.
# PKG_CONFIG_ENV	  - the string PKG_CONFIG_PATH=
#                           ${PKG_CONFIG_DIR}:$ENV{PKG_CONFIG_PATH} which
#  			    is used in example builds.
include(FindPkgConfig)

if(PKG_CONFIG_EXECUTABLE)
  message(STATUS "Looking for pkg-config - found")
  set(PKG_CONFIG_DIR "${LIB_DIR}/pkgconfig")
  set(env_PKG_CONFIG_PATH $ENV{PKG_CONFIG_PATH})
  if(env_PKG_CONFIG_PATH)
    set(_pkg_config_path "${PKG_CONFIG_DIR};${env_PKG_CONFIG_PATH}")
  else(env_PKG_CONFIG_PATH)
    set(_pkg_config_path "${PKG_CONFIG_DIR}")
  endif(env_PKG_CONFIG_PATH)

  if(MINGW)
    determine_msys_path(_pkg_config_path "${_pkg_config_path}")
  endif(MINGW)

  set(PKG_CONFIG_ENV PKG_CONFIG_PATH="${_pkg_config_path}")

else(PKG_CONFIG_EXECUTABLE)
  message(STATUS "Looking for pkg-config - not found")
  message(STATUS
  "WARNING: Makefile+pkg-config version of examples build in the install tree will not work.")
endif(PKG_CONFIG_EXECUTABLE)

macro(pkg_check_pkgconfig _package _include_DIR _link_DIR _link_FLAGS _cflags _version)
  # Similar to legacy pkgconfig only these results are derived from
  # pkg_check_modules and therefore are returned as lists rather than
  # blank-delimited strings.  Also, the _link_FLAGS value is converted
  # to the list of full library paths preferred by CMake via the
  # cmake_link_flags macro.

  # N.B. if using this macro in more than one context, cache clashes will
  # occur unless optional trailing prefix argument is specified to distinguish
  # different contexts.
  set(_prefix ${ARGN})
  if(NOT _prefix)
    set(_prefix "_PKGCONFIG_TMP")
  endif(NOT _prefix)
  
  if(FORCE_EXTERNAL_STATIC)
    set(_xprefix ${_prefix}_STATIC)
  else(FORCE_EXTERNAL_STATIC)
    set(_xprefix ${_prefix})
  endif(FORCE_EXTERNAL_STATIC)
  
  _pkg_check_modules_internal(0 0 ${_prefix} "${_package}")
  if(${_prefix}_FOUND)
    cmake_link_flags(${_link_FLAGS} "${${_xprefix}_LDFLAGS}")
    # If libraries cannot be not found, then that is equivalent to whole
    # pkg-config module not being found.
    if(NOT ${_link_FLAGS})
      set(${_prefix}_FOUND)
    endif(NOT ${_link_FLAGS})
  endif(${_prefix}_FOUND)

  if(${_prefix}_FOUND)
    set(${_include_DIR} ${${_xprefix}_INCLUDE_DIRS})
    set(${_link_DIR}    ${${_xprefix}_LIBRARY_DIRS})
    set(${_cflags}      ${${_xprefix}_CFLAGS})
    # N.B. must use ${_prefix}_VERSION} rather than ${_xprefix}_VERSION}
    # because the latter is not defined.
    set(${_version}     ${${_prefix}_VERSION})
    set(_return_VALUE 0)
  else(${_prefix}_FOUND)
    set(${_include_DIR})
    set(${_link_DIR})
    set(${_link_FLAGS})
    set(${_cflags})
    set(${_version})
    set(_return_VALUE 1)
  endif(${_prefix}_FOUND)
  #message("${_prefix}_FOUND = ${${_prefix}_FOUND}")
  #message("${_include_DIR} = ${${_include_DIR}}")
  #message("${_link_DIR} = ${${_link_DIR}}")
  #message("${_link_FLAGS} = ${${_link_FLAGS}}")
  #message("${_cflags} = ${${_cflags}}")
  #message("${_version} = ${${_version}}")
endmacro(pkg_check_pkgconfig)

macro(pkg_config_link_flags _link_flags_out _link_flags_in)
  # Transform link flags into a form that is suitable to be used for
  # output pkg-config (*.pc) files.
  # N.B. ${_link_flags_in} must be in quoted "${list_variable}" form
  # where list_variable is a CMake list.

  # First strip out optimized / debug options which are not needed
  # Currently only FindQt4 seems to need this.
  if(CMAKE_BUILD_TYPE MATCHES "Debug")
    # Get rid of the optimized keyword and immediately following library as
    # well as the debug keyword anywhere such patterns occur in the list.
    string(REGEX REPLACE "(^|;)optimized;[^;]*;" "\\1" ${_link_flags_out} "${_link_flags_in}")
    string(REGEX REPLACE "(^|;)debug;" "\\1" ${_link_flags_out} "${${_link_flags_out}}")
  else(CMAKE_BUILD_TYPE MATCHES "Debug")
    # Get rid of the debug keyword and immediately following library as
    # well as the optimized keyword anywhere such patterns occur in the list.
    string(REGEX REPLACE "(^|;)debug;[^;]*;" "\\1" ${_link_flags_out} "${_link_flags_in}")
    string(REGEX REPLACE "(^|;)optimized;" "\\1" ${_link_flags_out} "${${_link_flags_out}}")
  endif(CMAKE_BUILD_TYPE MATCHES "Debug")

  #message("(original link flags) = ${_link_flags_in}")
  # Convert link flags to a blank-delimited string.
  string(REGEX REPLACE ";" " " ${_link_flags_out} "${${_link_flags_out}}")
  #message("(blanks) ${_link_flags_out} = ${${_link_flags_out}}")

  # Replace actual library names with the -LPATHNAME and -lLIBRARYNAME form
  # since it appears pkg-config handles that latter form much better (with
  # regard to keeping the correct order and eliminating duplicates).

  # This logic appears to work fine on Linux, Mac OS X, and MinGW/MSYS
  # but it may need some generalization on other platforms such as
  # Cygwin.
  if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    set(suffix_list "\\.so" "\\.a")
  elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    set(suffix_list "\\.so" "\\.a" "\\.dylib")
  elseif(WIN32_OR_CYGWIN)
    # Order is important here.
    set(suffix_list "\\.dll\\.a" "\\.a")
  else(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    # Probably a non-Linux, non-Mac OS X, Unix platform
    # For this case we assume the same as Linux.
    set(suffix_list "\\.so" "\\.a")
  endif(CMAKE_SYSTEM_NAME STREQUAL "Linux")

  foreach(suffix ${suffix_list})
    if(WIN32_OR_CYGWIN)
      # Look for colon-delimited drive-letter form on these platforms.
      string(
	REGEX REPLACE "([a-zA-Z]:/[^ ]*)/lib([^ ]*)${suffix}" "-L\\1 -l\\2"
	${_link_flags_out}
	"${${_link_flags_out}}"
	)
      #message("(${suffix}) ${_link_flags_out} = ${${_link_flags_out}}")
    endif(WIN32_OR_CYGWIN)
    # Look for form starting with "/" on all platforms.
    string(
      REGEX REPLACE "(/[^ ]*)/lib([^ ]*)${suffix}" "-L\\1 -l\\2"
      ${_link_flags_out}
      "${${_link_flags_out}}"
      )
    #message("(${suffix}) ${_link_flags_out} = ${${_link_flags_out}}")
  endforeach(suffix ${suffix_list})

  if(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    # For Mac OS X transform frameworks information into correct form.
    string(
    REGEX REPLACE
    "/System/Library/Frameworks/([^ ]*)\\.framework"
    "-framework \\1"
    ${_link_flags_out}
    ${${_link_flags_out}}
    )
    #message("(frameworks) ${_link_flags_out} = ${${_link_flags_out}}")
  endif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")

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
  else("${_link_flags_in}" STREQUAL "")
    #message("(original link flags) = ${_link_flags_in}")
    # Convert link flags to a list.  Note some link flags are blank-delimited
    # (such as "-framework whatever") so preserve those by splitting into
    # separate list elements only if the next element starts with a hyphen.
    string(REGEX REPLACE " -" ";-" _link_flags_list "${_link_flags_in}")
    # Extract list of directories from -L options.
    list(LENGTH _link_flags_list _link_flags_length)
    math(EXPR _link_flags_length "${_link_flags_length} - 1")
    set(_index_list)
    set(_link_directory_list)
    foreach(_list_index RANGE ${_link_flags_length})
      list(GET _link_flags_list ${_list_index} _list_element)
      string(REGEX REPLACE "^-L" "" _list_element1 "${_list_element}")
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
    set(_success ON)
    foreach(_list_index RANGE ${_link_flags_length})
      list(GET _link_flags_list ${_list_index} _list_element)
      string(REGEX REPLACE "^-l" "" _list_element1 "${_list_element}")
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
	  set(_success OFF)
        endif(NOT _library_pathname)
        list(APPEND ${_link_flags_out} ${_library_pathname})
      else(_list_element STREQUAL "-L${_list_element1}")
        # link options that are not -L or -l passed through in correct order
        # in ${_link_flags_out}.
        list(APPEND ${_link_flags_out} ${_list_element})
      endif(_list_element STREQUAL "-l${_list_element1}")
    endforeach(_list_index RANGE ${_link_flags_length})
    #message("${_link_flags_out} = ${${_link_flags_out}}")

    # If one or more of the libraries cannot be found, then return an
    # empty ${_link_flags_out} as a sign of that failure.
    if(NOT _success)
      set(${_link_flags_out})
    endif(NOT _success)
  endif("${_link_flags_in}" STREQUAL "")
endmacro(cmake_link_flags)

