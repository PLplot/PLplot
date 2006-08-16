# cmake/modules/wxwidgets.cmake
#
# Copyright (C) 2006  Werner Smekal
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

# Module for determining all configuration variables related to the wxwidgets
# device driver.
# The following variables are set/modified:
# PLD_wxwidgets		  - ON means the wxwidgets device is enabled.
# wxwidgets_COMPILE_FLAGS	  - individual COMPILE_FLAGS required to compile wxwidgets
# 			    device.
# wxwidgets_LINK_FLAGS	  - individual LINK_FLAGS for dynamic wxwidgets device.
# DRIVERS_LINK_FLAGS	  - list of LINK_FLAGS for all static devices.

# Must have dynamic devices enabled since wxwidgets is written in C++ which
# in the static devices case is put in with C objects in libplplot(d)
# which won't work for some platforms.  However, as an experiment we are
# allowing this for WIN32 (bare windows, Cygwin, and MinGW) platforms.
if(PLD_wxwidgets AND NOT ENABLE_DYNDRIVERS)
  if(NOT WIN32)
    message(STATUS 
       "WARNING: This device requires ENABLE_DYNDRIVERS ON, but it is OFF.\n"
    "   Setting PLD_wxwidgets to OFF."
    )
    set(PLD_wxwidgets OFF CACHE BOOL "Enable wxwidgets device" FORCE)
  endif(NOT WIN32)
endif(PLD_wxwidgets AND NOT ENABLE_DYNDRIVERS)

if(PLD_wxwidgets)
  find_package(wxWidgets)
  if(wxWidgets_FOUND)
    string(REGEX REPLACE ";" " -I" 
    wxwidgets_COMPILE_FLAGS
    "-I${wxWidgets_INCLUDE_DIRS} ${wxWidgets_DEFINITIONS}"
    )
    set(wxwidgets_LINK_FLAGS ${wxWidgets_LIBRARIES})
    if(WITH_FREETYPE)
      set(
      wxwidgets_COMPILE_FLAGS
      "${wxwidgets_COMPILE_FLAGS} -I${FREETYPE_INCLUDE_DIR}"
      )
      set(
      wxwidgets_LINK_FLAGS
      ${wxwidgets_LINK_FLAGS}
      ${FREETYPE_LIBRARIES}
      )
    endif(WITH_FREETYPE)
    set(DRIVERS_LINK_FLAGS ${DRIVERS_LINK_FLAGS} ${wxwidgets_LINK_FLAGS})
  else(wxWidgets_FOUND)
    set(PLD_wxwidgets OFF CACHE BOOL "Enable wxwidgets device" FORCE)
  endif(wxWidgets_FOUND)
endif(PLD_wxwidgets)
