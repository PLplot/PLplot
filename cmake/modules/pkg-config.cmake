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

# Module for determining all configuration variables related to pkg-config

# The following variables are set:
# PKGCONFIG_EXECUTABLE	  - name of pkg-config executable, but can also be
# 			    used for logical control with, e.g.,
# 			    if(PKGCONFIG_EXECUTABLE)
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

message(STATUS "Looking for pkg-config")
find_program(PKGCONFIG_EXECUTABLE NAMES pkg-config PATHS /usr/local/bin)

set(pkg_config_false "#")

if(PKGCONFIG_EXECUTABLE)
  MESSAGE(STATUS "Looking for pkg-config - found")
  set(pkg_config_true "")
  set(PKG_CONFIG_DIR ${LIB_DIR}/pkgconfig)
  set(PKG_CONFIG_ENV PKG_CONFIG_PATH=${PKG_CONFIG_DIR})
else(PKGCONFIG_EXECUTABLE)
  message(STATUS "Looking for pkg-config - not found")
  message(STATUS "WARNING: Build of examples will not work.")
  # Turn off pkg-config build.  (This means both build methods will be
  # commented out.)
  set(pkg_config_true "#")
endif(PKGCONFIG_EXECUTABLE)
