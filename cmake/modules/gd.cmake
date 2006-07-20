# cmake/modules/gd.cmake
#
# Copyright (C) 2006  Andrew Ross
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

# Configuration for the gd device (supporting png, gif and/or jpeg drivers)
#
# The following variables are set / modified
#
# PLD_png             - ON means the png device is enabled
# PLD_jpeg            - ON means the jpeg device is enabled
# PLD_gif             - ON means the gif device is enabled
# gd_COMPILE_FLAGS    - individual COMPILE_FLAGS required to compile gd device.
# gd_LINK_FLAGS       - individual LINK_FLAGS for dynamic gd device.
# DRIVERS_LINK_FLAGS  - list of LINK_FLAGS for all static devices.
#
# Include file searches use FindPath. To add extra search directories to 
# set the environment variable CMAKE_INCLUDE_PATH or the CMake variable 
# CMAKE_INCLUDE_PATH (using cmake -D option).
# Library searches use FindLibrary. To add extra search directories to 
# set the environment variable CMAKE_LIBRARY_PATH or the CMake variable 
# CMAKE_LIBRARY_PATH (using cmake -D option).
# See cmake documentation for further details.

# Look for gd libraries
if(PLD_png OR PLD_jpeg OR PLD_gif)
  include(FindGD)
  if (NOT GD_FOUND)
    set(PLD_png OFF)
    set(PLD_jpeg OFF)
    set(PLD_gif OFF)
  else (NOT GD_FOUND)

    set(GDINCCMD ${GD_INCLUDE_DIR})
    set(GDLIBCMD ${GD_LIBRARIES})
  endif (NOT GD_FOUND)
endif(PLD_png OR PLD_jpeg OR PLD_gif)

if (GD_FOUND)
  if (NOT GD_SUPPORTS_PNG)
    set(PLD_png OFF)
  endif(NOT GD_SUPPORTS_PNG)
  
  if (NOT GD_SUPPORTS_JPEG)
    set(PLD_jpeg OFF)
  endif(NOT GD_SUPPORTS_JPEG)
  
  if (NOT GD_SUPPORTS_GIF)
    set(PLD_gif OFF)
  endif(NOT GD_SUPPORTS_GIF)
endif (GD_FOUND)

if(PLD_png OR PLD_jpeg OR PLD_gif)
  # Convert directory list to compile flags
  set(gd_COMPILE_FLAGS)
  foreach(DIR ${GD_INCLUDE_DIR})
    set(gd_COMPILE_FLAGS "${gd_COMPILE_FLAGS} -I${DIR}")
  endforeach(DIR ${GD_INCLUDE_DIR})

  # Generate library link commands
  set(gd_LINK_FLAGS)
  foreach(DIR ${GD_LIBRARY_DIR})
    set(gd_LINK_FLAGS "${gd_LINK_FLAGS} -L${DIR}")
  endforeach(DIR ${GD_LIBRARY_DIR})
  foreach(LIB ${GD_LIBRARIES})
    get_filename_component(LIBNAME ${LIB} NAME_WE)
    string(REGEX REPLACE "^lib" "-l" LIBSHORT ${LIBNAME})
    set(gd_LINK_FLAGS "${gd_LINK_FLAGS} ${LIBSHORT}")    
  endforeach(LIB ${GD_LIBRARIES})
  set(DRIVERS_LINK_FLAGS ${DRIVERS_LINK_FLAGS} ${gd_LINK_FLAGS})
  MESSAGE("${gd_COMPILE_FLAGS}")
  MESSAGE("${gd_LINK_FLAGS}")
endif(PLD_png OR PLD_jpeg OR PLD_gif)

