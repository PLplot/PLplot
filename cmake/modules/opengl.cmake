# cmake/modules/opengl.cmake
#
# Copyright (C) 2014  Hazen Babcock
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

# Configuration for the gd device driver (supporting png, gif and/or jpeg
# devices.)
#
# The following variables are set / modified
#
# PLD_opengl          - ON means the opengl device is enabled
# opengl_COMPILE_FLAGS    - COMPILE_FLAGS required to compile gd device driver.
# opengl_LINK_FLAGS       - LINK_FLAGS for dynamic gd device driver.
# DRIVERS_LINK_FLAGS  - list of LINK_FLAGS for all static device drivers.
#
# Include file searches use FindPath. To add extra search directories
# set the environment variable CMAKE_INCLUDE_PATH.
# Library searches use FindLibrary. To add extra search directories
# set the environment variable CMAKE_LIBRARY_PATH.
# See cmake documentation for further details.

# Look for gd libraries
if(PLD_opengl)
  find_package(OPENGL)
  if (NOT OPENGL_FOUND)
    set(PLD_opengl OFF CACHE BOOL "Enable opengl device" FORCE)
  else (NOT OPENGL_FOUND)
endif(PLD_opengl)

if(PLD_opengl)
  # Convert directory list to compile flags
  set(opengl_COMPILE_FLAGS)
  foreach(DIR ${OPENGL_INCLUDE_DIR})
    set(opengl_COMPILE_FLAGS "${opengl_COMPILE_FLAGS} -I${DIR}")
  endforeach(DIR ${OPENGL_INCLUDE_DIR})

  # Generate library link commands
  set(opengl_LINK_FLAGS ${OPENGL_LIBRARIES})
  set(DRIVERS_LINK_FLAGS ${DRIVERS_LINK_FLAGS} ${opengl_LINK_FLAGS})
endif(PLD_opengl)
