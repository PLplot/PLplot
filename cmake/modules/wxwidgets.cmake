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
  if(NOT CMAKE_CXX_COMPILER_WORKS)
    message(STATUS "WARNING: no working C++ compiler so "
      "setting all wxwidgets devices to OFF."
      )
    set(PLD_wxwidgets OFF CACHE BOOL "Enable wxwidgets device" FORCE)
    set(PLD_wxpng OFF CACHE BOOL "Enable wxwidgets png device" FORCE)
  endif(NOT CMAKE_CXX_COMPILER_WORKS)
endif(PLD_wxwidgets OR PLD_wxpng)

if(PLD_wxwidgets OR PLD_wxpng)
  if(FORCE_EXTERNAL_STATIC)
    set(wxWidgets_USE_STATIC ON)
  endif(FORCE_EXTERNAL_STATIC)
  find_package(wxWidgets COMPONENTS base core QUIET)
  # Convert wxWidgets_LIBRARIES to full pathname form.
  cmake_link_flags(wxwidgets_LINK_FLAGS "${wxWidgets_LIBRARIES}")
  if(NOT wxWidgets_FOUND OR NOT wxwidgets_LINK_FLAGS)
    message(STATUS
      "WARNING: wxWidgets or its libraries not found so "
      "setting all wxwidgets devices to OFF."
      )
    set(PLD_wxwidgets OFF CACHE BOOL "Enable wxwidgets device" FORCE)
    set(PLD_wxpng OFF CACHE BOOL "Enable wxwidgets png device" FORCE)
  else(NOT wxWidgets_FOUND OR NOT wxwidgets_LINK_FLAGS)  
    # Check if stdint.h can be used from c++ (PL_HAVE_CXX_STDINT_H)
    include(TestForStdintCXX)
  endif(NOT wxWidgets_FOUND OR NOT wxwidgets_LINK_FLAGS)  
endif(PLD_wxwidgets OR PLD_wxpng)

if(PLD_wxwidgets OR PLD_wxpng)
  string(REGEX REPLACE ";" " -I" 
    wxwidgets_COMPILE_FLAGS
    "-I${wxWidgets_INCLUDE_DIRS}"
    )
  string(REGEX REPLACE ";" " -D" 
    wxWidgets_DEFINITIONS_wD
    "-D${wxWidgets_DEFINITIONS}"
    )
  set(wxWidgets_DEFINITIONS_DEBUG_wD "") 
  if(wxWidgets_DEFINITIONS_DEBUG)
    string(REGEX REPLACE ";" " -D" 
      wxWidgets_DEFINITIONS_DEBUG_wD
      "-D${wxWidgets_DEFINITIONS_DEBUG}"
      )
  endif(wxWidgets_DEFINITIONS_DEBUG)
  string(REGEX REPLACE ";" " " 
    wxwidgets_COMPILE_FLAGS
    ${wxwidgets_COMPILE_FLAGS}
    " ${wxWidgets_DEFINITIONS_wD} ${wxWidgets_DEFINITIONS_DEBUG_wD}"
    )
  message(STATUS "wxWidgets found")
  message(STATUS "wxwidgets_COMPILE_FLAGS = ${wxwidgets_COMPILE_FLAGS}")
  message(STATUS "wxwidgets_LINK_FLAGS = ${wxwidgets_LINK_FLAGS}")
  if(WITH_FREETYPE)
    include(agg)
    if(HAVE_AGG)
      set(
	wxwidgets_COMPILE_FLAGS
	"${wxwidgets_COMPILE_FLAGS} -I${AGG_INCLUDE_DIRS}"
	)
    else(HAVE_AGG)
      message(STATUS "WARNING: wxwidgets driver and bindings components depending on AGG library have been dropped.")
    endif(HAVE_AGG)
    set(
      wxwidgets_COMPILE_FLAGS
      "${wxwidgets_COMPILE_FLAGS} -I${FREETYPE_INCLUDE_DIR}"
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

if(ENABLE_wxwidgets AND NOT CMAKE_CXX_COMPILER_WORKS)
  message(STATUS "WARNING: no working C++ compiler so "
    "setting ENABLE_wxwidgets to OFF."
    )
  set(ENABLE_wxwidgets OFF CACHE BOOL "Enable wxwidgets bindings" FORCE)
endif(ENABLE_wxwidgets AND NOT CMAKE_CXX_COMPILER_WORKS)

if(ENABLE_wxwidgets AND NOT PLD_wxwidgets)
  message(STATUS
    "WARNING: PLD_wxwidgets is OFF so "
    "setting ENABLE_wxwidgets to OFF."
    )
  set(ENABLE_wxwidgets OFF CACHE BOOL "Enable wxwidgets bindings" FORCE)
endif(ENABLE_wxwidgets AND NOT PLD_wxwidgets)

if(ENABLE_wxwidgets)
  set(wxwidgets_true "")
else(ENABLE_wxwidgets)
  set(wxwidgets_true "#")
endif(ENABLE_wxwidgets)
