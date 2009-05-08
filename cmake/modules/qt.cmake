# cmake/modules/qt.cmake
#
# Copyright (C) 2009 Alan W. Irwin
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

# Module for determining all configuration variables related to the qt
# device driver.
# The following variables are set/modified:
# PLD_bmpqt		  - ON means the bmpqt device is enabled.
# PLD_jpgqt		  - ON means the jpgqt device is enabled.
# PLD_pngqt		  - ON means the pngqt device is enabled.
# PLD_ppmqt		  - ON means the ppmqt device is enabled.
# PLD_tiffqt		  - ON means the tiffqt device is enabled.
# PLD_epsqt		  - ON means the epsqt device is enabled.
# PLD_pdfqt		  - ON means the pdfqt device is enabled.
# PLD_qtwidget		  - ON means the qtwidget device is enabled.
# PLD_svgqt		  - ON means the svgqt device is enabled.
# PLD_extqt		  - ON means the extqt device is enabled.
# qt_COMPILE_FLAGS	  - individual COMPILE_FLAGS required to compile wxwidgets
# 			    device.
# qt_LINK_FLAGS	  	  - list of full path names of libraries and
# 			    linker flags for dynamic wxwidgets device driver.
# qt_RPATH	       	  - RPATH directory list for qt device driver.
# DRIVERS_LINK_FLAGS  	  - list of device LINK_FLAGS for case
# 			    when ENABLE_DYNDRIVERS OFF.

find_package(Qt4)
if(PLD_bmpqt OR PLD_jpgqt OR PLD_pngqt OR PLD_ppmqt OR PLD_tiffqt OR PLD_epsqt OR PLD_pdfqt OR PLD_qtwidget OR PLD_svgqt OR PLD_extqt)
  if(QT4_FOUND)
    if(PLD_svgqt AND ${QT_VERSION_MINOR} GREATER 2)
      set(QT_USE_QTSVG 1)
    endif(PLD_svgqt AND ${QT_VERSION_MINOR} GREATER 2)
    include(${QT_USE_FILE})
    set(qt_COMPILE_FLAGS)
    foreach(DIR ${QT_INCLUDES})
      set(qt_COMPILE_FLAGS "${qt_COMPILE_FLAGS} -I${DIR}")
    endforeach(DIR ${QT_INCLUDES})
    
    split_libraries_list(QT_LIBRARIES QT_LIBRARIES_general QT_LIBRARIES_debug QT_LIBRARIES_optimized)

    # There is a complicated interpretation issue to be considered here.
    # FindQt4.cmake associates "debug" with the Qt4 library DEBUG suffix (with
    # currently unknown criteria for which libraries are labelled that way),
    # and "optimized" with the Qt4 library RELEASE suffix (again with 
    # currently unknown criteria).  Furthermore, CMake documentation is
    # is not completely clear on how the debug and optimized keywords are to
    # be interpreted by target_link_libraries for the Debug, Release,
    # RelWithDebInfo and MinSizeRes possibilities for CMAKE_BUILD_TYPE.  For
    # example with gcc, the C options are Debug=-g, Release=-O3,
    # RelWithDebInfo = -g -O2, and MinSizeRes=-Os.  For that compiler, some of
    # the options are clearly debug (Debug), some of the options are clearly
    # optimized (Release), but the rest are ambiguous.  This decision may
    # be changed in the future, but for now choose the debug or optimized
    # Qt4 libraries only for the cases which are clearly justified, and 
    # otherwise just use the general Qt4 libraries.
    message(STATUS "QT_LIBRARIES (mixed for all CMAKE_BUILD_TYPES) = ${QT_LIBRARIES}")
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
      set(QT_LIBRARIES ${QT_LIBRARIES_debug})
    elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
      set(QT_LIBRARIES ${QT_LIBRARIES_optimized})
    else(CMAKE_BUILD_TYPE STREQUAL "Debug")
      set(QT_LIBRARIES ${QT_LIBRARIES_general})
    endif(CMAKE_BUILD_TYPE STREQUAL "Debug")
    message(STATUS "specific QT_LIBRARIES results used for this CMAKE_BUILD_TYPE = ${QT_LIBRARIES}")

    set(qt_LINK_FLAGS ${QT_LIBRARIES})
    #message("qt_LINK_FLAGS = ${qt_LINK_FLAGS}")
    set(qt_RPATH ${QT_LIBRARY_DIR})
    #message("qt_LIBRARY_DIR = ${qt_LIBRARY_DIR}")
    set(DRIVERS_LINK_FLAGS ${DRIVERS_LINK_FLAGS} ${qt_LINK_FLAGS})
  else(QT4_FOUND)
    set(PLD_bmpqt OFF CACHE BOOL "Enable Qt Windows bmp device" FORCE)
    set(PLD_jpgqt OFF CACHE BOOL "Enable Qt jpg device" FORCE)
    set(PLD_pngqt OFF CACHE BOOL "Enable Qt png device" FORCE)
    set(PLD_ppmqt OFF CACHE BOOL "Enable Qt ppm device" FORCE)
    set(PLD_tiffqt OFF CACHE BOOL "Enable Qt tiff device" FORCE)
    set(PLD_epsqt OFF CACHE BOOL "Enable Qt EPS device" FORCE)
    set(PLD_pdfqt OFF CACHE BOOL "Enable Qt PDF device" FORCE)
    set(PLD_qtwidget OFF CACHE BOOL "Enable Qt interactive device" FORCE)
    set(PLD_svgqt OFF CACHE BOOL "Enable Qt SVG device" FORCE)
    set(PLD_extqt OFF CACHE BOOL "Enable Qt ext device" FORCE)
  endif(QT4_FOUND)
endif(PLD_bmpqt OR PLD_jpgqt OR PLD_pngqt OR PLD_ppmqt OR PLD_tiffqt OR PLD_epsqt OR PLD_pdfqt OR PLD_qtwidget OR PLD_svgqt OR PLD_extqt)

if(PLD_extqt)
  set(qt_gui_true "")
else(PLD_extqt)
  set(qt_gui_true "#")
endif(PLD_extqt)
