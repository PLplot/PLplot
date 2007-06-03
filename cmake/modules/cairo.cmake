# cmake/modules/psttf.cmake
#
# Copyright (C) 2007  Hazen Babcock
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

# Configuration for the psttf device driver (supporting the psttf and 
# psttfc devices).
#
# The following variables are set / modified
#
# PLD_xwinttf           - ON means the xwintff device is enabled.
# cairo_COMPILE_FLAGS	- Blank-delimited COMPILE_FLAGS required to
#			                compile cairo device drivers.
# cairo_LINK_FLAGS	    - LINK_FLAGS (string) for dynamic cairo device drivers.
# DRIVERS_LINK_FLAGS	- list of device LINK_FLAGS and TARGETS for case
#			                when ENABLE_DYNDRIVERS OFF.

# Include file searches use FindPath. To add extra search directories
# set the environment variable CMAKE_INCLUDE_PATH.
# Library searches use FindLibrary. To add extra search directories
# set the environment variable CMAKE_LIBRARY_PATH.
# See cmake documentation for further details.

# Look for xwinttf headers and libraries with pkg-config
if(PLD_xwinttf)
  if(NOT PKG_CONFIG_EXECUTABLE)
    message(STATUS 
    "WARNING: pkg-config not found. Setting PLD_xwinttf to OFF."
    )
    set(PLD_xwinttf OFF CACHE BOOL "Enable xwinttf device" FORCE)
  endif(NOT PKG_CONFIG_EXECUTABLE)
endif(PLD_xwinttf)

if(PLD_xwinttf)
  pkg_check_pkgconfig("pangocairo;pango;cairo" includedir libdir linkflags cflags _XWINTTF)
  if(linkflags)
    # Blank-delimited required.
    string(REGEX REPLACE ";" " " cairo_COMPILE_FLAGS "${cflags} -I${X11_INCLUDE_DIR}")
    set(cairo_LINK_FLAGS "${linkflags} -L${X11_LIBRARY_DIR} ${X11_LIBRARIES}")
    message("cairo_COMPILE_FLAGS = ${cairo_COMPILE_FLAGS}")

    set(DRIVERS_LINK_FLAGS
    ${DRIVERS_LINK_FLAGS}
    ${cairo_LINK_FLAGS}
    ${cairo_TARGETS}
    )
  else(linkflags)
    #message("includedir = ${includedir}")
    #message("libdir = ${libdir}")
    #message("linkflags = ${linkflags}")
    #message("cflags = ${cflags}")
    message(STATUS
       "WARNING: pango or cairo not found with pkg-config.\n"
    "   Setting PLD_xwintff to OFF.  Please install all of these packages\n"
    "   and/or set the environment variable PKG_CONFIG_PATH appropriately."
    )
    set(PLD_xwinttf OFF CACHE BOOL "Enable xwinttf device" FORCE)
  endif(linkflags)
endif(PLD_xwinttf)
