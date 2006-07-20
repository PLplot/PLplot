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

if (PLD_png)
  include(FindPNG)
  if (NOT PNG_FOUND)
    set(PLD_png OFF)
  else (NOT PNG_FOUND)
    set(GDINCCMD ${GDINCCMD} ${PNG_INCLUDE_DIR})
    set(GDLIBCMD ${GDLIBCMD} ${PNG_LIBRARIES})
  endif (NOT PNG_FOUND)
endif(PLD_png)

if (PLD_jpeg)
  include(FindJPEG)
  if (NOT JPEG_FOUND)
    set(PLD_jpeg OFF)
  else (NOT JPEG_FOUND)
    set(GDINCCMD ${GDINCCMD} ${JPEG_INCLUDE_DIR})
    set(GDLIBCMD ${GDLIBCMD} ${JPEG_LIBRARIES})
  endif (NOT JPEG_FOUND)
endif(PLD_jpeg)

if(PLD_png OR PLD_jpeg OR PLD_gif)
  # Strip duplicate include directory entries out
  set(GDINCTRIM)
  foreach(DIR ${GDINCCMD})
    set(FOUND OFF)
    foreach(TRIMMED ${GDINCTRIM})
      if ("${DIR}" STREQUAL "${TRIMMED}")
	SET(FOUND ON)
      endif ("${DIR}" STREQUAL "${TRIMMED}")
    endforeach(TRIMMED ${GDINCTRIM})
    if (NOT FOUND)
      set(GDINCTRIM "${GDINCTRIM}" "${DIR}")
    endif (NOT FOUND)
  endforeach(DIR ${GDINCCMD})
  # Convert directory list to compile flags
  set(gd_COMPILE_FLAGS)
  foreach(DIR ${GDINCTRIM})
    set(gd_COMPILE_FLAGS ${gd_COMPILE_FLAGS} "-I${DIR}")
  endforeach(DIR ${GDINCTRIM})

  set(GDLIBCMDTRIM)
  set(GDLIBSTRIM)
  set(GDLIBDIRS)
  # Generate trimmed list of library directories and list of libraries
  foreach(LIB ${GDLIBCMD})
    get_filename_component(LIBDIR ${LIB} PATH)
    get_filename_component(LIBNAME ${LIB} NAME_WE)
    set(FOUND OFF)
    foreach(TRIMMED ${GDLIBSTRIM})
      if ("${LIBDIR}" STREQUAL "${TRIMMED}")
	set(FOUND ON)
      endif ("${LIBDIR}" STREQUAL "${TRIMMED}")
    endforeach(TRIMMED ${GDLIBSTRIM})
    if (NOT FOUND)
      set(GDLIBSTRIM ${GDLIBSTRIM} ${LIBDIR})
      set(gd_LINK_FLAGS "${gd_LINK_FLAGS} -L${LIBDIR}")
    endif (NOT FOUND)
    string(REGEX REPLACE "^lib" "-l" LIBSHORT ${LIBNAME})
    set(gd_LINK_FLAGS "${gd_LINK_FLAGS} ${LIBSHORT}")    
  endforeach(LIB ${GDLIBCMD})
  set(DRIVERS_LINK_FLAGS ${DRIVERS_LINK_FLAGS} ${gd_LINK_FLAGS})
endif(PLD_png OR PLD_jpeg OR PLD_gif)

