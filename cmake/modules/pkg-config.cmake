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
# pkgconfig macro using the pkg_check_modules macro and 
# pc_transform_link_flags to process link flags into standard form for
# the configured *.pc files.

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
  # blank-delimited strings.

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
    set(${_link_FLAGS}  ${${_prefix}_LDFLAGS})
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

macro(pc_transform_link_flags _link_flags_out _link_flags_in)
  # Transform link flags into a form that is suitable to be used in
  # pkg-config (*.pc) files.
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

endmacro(pc_transform_link_flags)
