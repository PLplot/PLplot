# cmake/modules/linuxvga.cmake
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

# Configuration for the linuxvga device driver (supporting the linuxvga
# device).
#
# The following variables are set / modified
#
# PLD_linuxvga            - ON means the linuxvga device is enabled.
# linuxvga_COMPILE_FLAGS  - COMPILE_FLAGS (string) required to compile linuxvga
# 		  	    device driver.
# linuxvga_LINK_FLAGS     - LINK_FLAGS (string) for dynamic linuxvga device
# 			    driver.
# DRIVERS_LINK_FLAGS - list of LINK_FLAGS for all static device drivers.
#
# Include file searches use FindPath. To add extra search directories
# set the environment variable CMAKE_INCLUDE_PATH.
# Library searches use FindLibrary. To add extra search directories
# set the environment variable CMAKE_LIBRARY_PATH.
# See cmake documentation for further details.

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
  if(PLD_linuxvga)
    find_package(VGA)
    if(VGA_FOUND)
      set(linuxvga_COMPILE_FLAGS "-I${VGA_INCLUDE_DIRS}")
      set(linuxvga_LINK_FLAGS "-L${VGA_LIBRARY_DIRS} -lvga")
      set(DRIVERS_LINK_FLAGS ${DRIVERS_LINK_FLAGS} ${linuxvga_LINK_FLAGS})
    else(VGA_FOUND)
      message(STATUS 
      "WARNING: vga.h and/or the vga library not found.\n"
      "Setting PLD_linuxvga to OFF."
      )
      set(PLD_linuxvga OFF CACHE BOOL "Enable linuxvga device" FORCE)    
    endif(VGA_FOUND)
  endif(PLD_linuxvga)
else(CMAKE_SYSTEM_NAME STREQUAL "Linux")
  set(PLD_linuxvga OFF CACHE BOOL "Enable linuxvga device" FORCE)    
endif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
