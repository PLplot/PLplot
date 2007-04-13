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
# DRIVERS_LINK_FLAGS  	  - list of LINK_FLAGS for all static devices.

# Find wxWidgets needed for driver and bindings
SET(wxWidgets_USE_LIBS core base)
find_package(wxWidgets QUIET)

if( NOT wxWidgets_FOUND )
  message(STATUS
  "WARNING: wxWidgets not found so "
  "setting PLD_wxwidgets to OFF."
  )
  set(PLD_wxwidgets OFF CACHE BOOL "Enable wxwidgets device" FORCE)
endif( NOT wxWidgets_FOUND )  

if(PLD_wxwidgets)
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
  include(agg)
  if(HAVE_AGG)
    set(
    wxwidgets_COMPILE_FLAGS
    "${wxwidgets_COMPILE_FLAGS} -I${AGG_INCLUDE_DIR}"
    )
    set(
    wxwidgets_LINK_FLAGS
    ${wxwidgets_LINK_FLAGS}
    ${AGG_LIBRARIES}
    )
  endif(HAVE_AGG)
  set(DRIVERS_LINK_FLAGS
  ${DRIVERS_LINK_FLAGS} 
  ${wxwidgets_LINK_FLAGS}
  )
endif(PLD_wxwidgets)

if(DEFAULT_NO_BINDINGS)
  option(ENABLE_wxwidgets "Enable wxwidgets bindings" OFF)
else(DEFAULT_NO_BINDINGS)
  option(ENABLE_wxwidgets "Enable wxwidgets bindings" ON)
endif(DEFAULT_NO_BINDINGS)

if(ENABLE_wxwidgets AND NOT PLD_wxwidgets)
  message(STATUS
  "WARNING: PLD_wxwidgets is OFF so "
  "setting ENABLE_wxwidgets to OFF."
  )
  set(ENABLE_wxwidgets OFF CACHE BOOL "Enable wxwidgets bindings" FORCE)
endif(ENABLE_wxwidgets AND NOT PLD_wxwidgets)

if(ENABLE_wxwidgets AND NOT ENABLE_cxx)
  message(STATUS
  "WARNING: ENABLE_cxx is OFF so "
  "setting ENABLE_wxwidgets to OFF."
  )
  set(ENABLE_wxwidgets OFF CACHE BOOL "Enable wxwidgets bindings" FORCE)
endif(ENABLE_wxwidgets AND NOT ENABLE_cxx)
if(ENABLE_wxwidgets)
  set(wxwidgets_true "")
else(ENABLE_wxwidgets)
  set(wxwidgets_true "#")
endif(ENABLE_wxwidgets)
