# cmake/modules/cairo.cmake
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

# Configuration for the cairo device driver (supporting the memcairo,
# pdfcairo, pngcairo, pscairo, svgcairo, memcairo, and xcairo devices).
#
# The following variables are set / modified
#
# PLD_memcairo            - ON means the memcairo device is enabled.
# PLD_pdfcairo		  - ON means the pdfcairo device is enabled.
# PLD_pngcairo		  - ON means the pngcairo device is enabled.
# PLD_pscairo		  - ON means the pscairo device is enabled.
# PLD_svgcairo		  - ON means the svgcairo device is enabled.
# PLD_xcairo		  - ON means the xcairo device is enabled.
# cairo_COMPILE_FLAGS	  - Blank-delimited COMPILE_FLAGS required to
# 			    compile cairo device drivers.
# cairo_LINK_FLAGS	  - list of full path names of libraries and
# 			    linker flags for dynamic cairo device driver.
# cairo_RPATH	       	  - RPATH directory list for cairo device driver.
# 			    current assumption is the list only has one 
#			    element corresponding to the pkg-config libdir
#			    variable for pangocairo.
# DRIVERS_LINK_FLAGS	  - list of device LINK_FLAGS for case
# 			    when ENABLE_DYNDRIVERS OFF.

# Include file searches use FindPath. To add extra search directories
# set the environment variable CMAKE_INCLUDE_PATH.
# Library searches use FindLibrary. To add extra search directories
# set the environment variable CMAKE_LIBRARY_PATH.
# See cmake documentation for further details.

# Look for cairo headers and libraries with pkg-config
if(
   PLD_memcairo
OR PLD_pdfcairo
OR PLD_pngcairo
OR PLD_pscairo
OR PLD_svgcairo 
OR PLD_xcairo
)
  if(NOT PKG_CONFIG_EXECUTABLE)
    message(STATUS 
    "WARNING: pkg-config not found. Setting cairo drivers to OFF."
    )
    set(PLD_memcairo OFF CACHE BOOL "Enable memcairo device" FORCE)
    set(PLD_pdfcairo OFF CACHE BOOL "Enable pdfcairo device" FORCE)
    set(PLD_pngcairo OFF CACHE BOOL "Enable pngcairo device" FORCE)
    set(PLD_pscairo OFF CACHE BOOL "Enable pscairo device" FORCE)
    set(PLD_svgcairo OFF CACHE BOOL "Enable svgcairo device" FORCE)
    set(PLD_xcairo OFF CACHE BOOL "Enable xcairo device" FORCE)
  endif(NOT PKG_CONFIG_EXECUTABLE)
endif(
   PLD_memcairo
OR PLD_pdfcairo
OR PLD_pngcairo
OR PLD_pscairo
OR PLD_svgcairo 
OR PLD_xcairo
)

if(
   PLD_memcairo
OR PLD_pdfcairo
OR PLD_pngcairo
OR PLD_pscairo
OR PLD_svgcairo 
OR PLD_xcairo
)
  pkg_check_pkgconfig(
  pangocairo
  includedir
  linkdir
  linkflags 
  cflags
  _CAIRO
  )
  if(linkflags)
    set(cairo_RPATH ${linkdir})
    if(PLD_xcairo AND X11_COMPILE_FLAGS)
      # Blank-delimited required.
      string(REGEX REPLACE ";" " " 
      cairo_COMPILE_FLAGS "${cflags} ${X11_COMPILE_FLAGS}"
      )
      # Convert X linker information to preferred CMake form before appending
      # it to already converted ${linkflags}
      cmake_link_flags(
      cairo_X_LINK_FLAGS
      "-L${X11_LIBRARY_DIR};${X11_LIBRARIES}"
      )
      set(cairo_LINK_FLAGS ${linkflags} ${cairo_X_LINK_FLAGS})
    else(PLD_xcairo AND X11_COMPILE_FLAGS)
      message(STATUS 
       "WARNING: X windows not found. Setting xcairo driver to OFF."
      )
      # Blank-delimited required.
      set(PLD_xcairo OFF CACHE BOOL "Enable xcairo device" FORCE)
      # now deal with remaining cairo devices.
      string(REGEX REPLACE ";" " " cairo_COMPILE_FLAGS "${cflags}")
      set(cairo_LINK_FLAGS ${linkflags})
    endif(PLD_xcairo AND X11_COMPILE_FLAGS)
    
    #message("cairo_COMPILE_FLAGS = ${cairo_COMPILE_FLAGS}")
    #message("cairo_LINK_FLAGS = ${cairo_LINK_FLAGS}")

    list(APPEND DRIVERS_LINK_FLAGS ${cairo_LINK_FLAGS})
  else(linkflags)
    message("includedir = ${includedir}")
    message("libdir = ${libdir}")
    message("linkflags = ${linkflags}")
    message("cflags = ${cflags}")
    message(STATUS
       "WARNING: pango and/or cairo not found with pkg-config.\n"
    "   Disabling cairo drivers.  To enable these drivers you must install\n"
    "   development versions of pango and cairo and/or set\n"
    "   the environment variable PKG_CONFIG_PATH appropriately."
    )
    set(PLD_memcairo OFF CACHE BOOL "Enable memcairo device" FORCE)
    set(PLD_pdfcairo OFF CACHE BOOL "Enable pdfcairo device" FORCE)
    set(PLD_pngcairo OFF CACHE BOOL "Enable pngcairo device" FORCE)
    set(PLD_pscairo OFF CACHE BOOL "Enable pscairo device" FORCE)
    set(PLD_svgcairo OFF CACHE BOOL "Enable svgcairo device" FORCE)
    set(PLD_xcairo OFF CACHE BOOL "Enable xcairo device" FORCE)
  endif(linkflags)
endif(
   PLD_memcairo
OR PLD_pdfcairo
OR PLD_pngcairo
OR PLD_pscairo
OR PLD_svgcairo 
OR PLD_xcairo
)
