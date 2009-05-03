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
# wxwidgets_LINK_FLAGS	  - list of full path names of libraries and
# 			    linker flags for dynamic wxwidgets device driver.
# DRIVERS_LINK_FLAGS  	  - list of device LINK_FLAGS for case
# 			    when ENABLE_DYNDRIVERS OFF.

# Find wxWidgets needed for driver and bindings
if(PLD_wxwidgets OR PLD_wxpng)
  find_package(wxWidgets COMPONENTS base core QUIET)
  if(NOT wxWidgets_FOUND)
    message(STATUS
      "WARNING: wxWidgets not found so "
      "setting PLD_wxwidgets to OFF."
      )
    set(PLD_wxwidgets OFF CACHE BOOL "Enable wxwidgets device" FORCE)
    set(PLD_wxpng OFF CACHE BOOL "Enable wxwidgets png device" FORCE)
  else(NOT wxWidgets_FOUND)  
    # Check if stdint.h can be used from c++ (HAVE_CXX_STDINT_H)
    include(TestForStdintCXX)
  endif(NOT wxWidgets_FOUND)  
endif(PLD_wxwidgets OR PLD_wxpng)
if(PLD_wxwidgets OR PLD_wxpng)
  string(REGEX REPLACE ";" " -I" 
    wxwidgets_COMPILE_FLAGS
    "-I${wxWidgets_INCLUDE_DIRS}"
    )
  # For case (cvs version of CMake as of 2008-03-23, but not cmake-2.4.8)
  # when wxWidgets_DEFINITIONS is a list.
  string(REGEX REPLACE ";" " " 
    wxwidgets_COMPILE_FLAGS
    ${wxwidgets_COMPILE_FLAGS}
    " ${wxWidgets_DEFINITIONS}"
    )
  # Convert wxWidgets_LIBRARIES to full pathname form.
  cmake_link_flags(wxwidgets_LINK_FLAGS "${wxWidgets_LIBRARIES}")
  message(STATUS "wxWidgets found")
  message(STATUS "wxwidgets_COMPILE_FLAGS = ${wxwidgets_COMPILE_FLAGS}")
  message(STATUS "wxwidgets_LINK_FLAGS = ${wxwidgets_LINK_FLAGS}")
  include(agg)
  if(HAVE_AGG)
    set(
      wxwidgets_COMPILE_FLAGS
      "${wxwidgets_COMPILE_FLAGS} -I${AGG_INCLUDE_DIRS}"
      )
    set(
      wxwidgets_LINK_FLAGS
      ${wxwidgets_LINK_FLAGS}
      ${AGG_LIBRARIES}
      )
  endif(HAVE_AGG)
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
  set(DRIVERS_LINK_FLAGS
    ${DRIVERS_LINK_FLAGS} 
    ${wxwidgets_LINK_FLAGS}
    )
endif(PLD_wxwidgets OR PLD_wxpng)
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
