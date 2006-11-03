# cmake/modules/cgm.cmake
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

# Configuration for the cgm device driver (supporting the cgm device).
#
# The following variables are set / modified
#
# PLD_cgm            - ON means the cgm devic is enabled.
# cgm_COMPILE_FLAGS  - COMPILE_FLAGS (string) required to compile cgm
# 		         device driver.
# cgm_LINK_FLAGS     - LINK_FLAGS (string) for dynamic cgm device driver.
# DRIVERS_LINK_FLAGS - list of LINK_FLAGS for all static device drivers.
#
# Include file searches use FindPath. To add extra search directories
# set the environment variable CMAKE_INCLUDE_PATH.
# Library searches use FindLibrary. To add extra search directories
# set the environment variable CMAKE_LIBRARY_PATH.
# See cmake documentation for further details.

if(PLD_cgm)
  find_package(CD)
  if(CD_FOUND)
    set(cgm_COMPILE_FLAGS "-I${CD_INCLUDE_DIRS}")
    set(cgm_LINK_FLAGS ${CD_LIBRARIES})
    set(DRIVERS_LINK_FLAGS ${DRIVERS_LINK_FLAGS} ${cgm_LINK_FLAGS})
  else(CD_FOUND)
    message(STATUS 
    "WARNING: cd.h and/or libcd.a not found.  Setting PLD_cgm to OFF."
    )
    set(PLD_cgm OFF CACHE BOOL "Enable cgm device" FORCE)    
  endif(CD_FOUND)
endif(PLD_cgm)
