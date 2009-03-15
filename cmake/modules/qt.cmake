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
# qt_COMPILE_FLAGS	  - individual COMPILE_FLAGS required to compile wxwidgets
# 			    device.
# qt_LINK_FLAGS	  	  - list of full path names of libraries and
# 			    linker flags for dynamic wxwidgets device driver.
# qt_RPATH	       	  - RPATH directory list for qt device driver.
# DRIVERS_LINK_FLAGS  	  - list of device LINK_FLAGS for case
# 			    when ENABLE_DYNDRIVERS OFF.

find_package(Qt4)
if(PLD_bmpqt OR PLD_jpgqt OR PLD_pngqt OR PLD_ppmqt OR PLD_tiffqt OR PLD_epsqt OR PLD_pdfqt OR PLD_qtwidget OR PLD_svgqt)
  if(QT4_FOUND)
    if(PLD_svgqt AND ${QT_VERSION_MINOR} GREATER 2)
      set(QT_USE_QTSVG 1)
    endif(PLD_svgqt AND ${QT_VERSION_MINOR} GREATER 2)
    include(${QT_USE_FILE})
    set(qt_COMPILE_FLAGS)
    foreach(DIR ${QT_INCLUDES})
      set(qt_COMPILE_FLAGS "${qt_COMPILE_FLAGS} -I${DIR}")
    endforeach(DIR ${QT_INCLUDES})
    
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
  endif(QT4_FOUND)
endif(PLD_bmpqt OR PLD_jpgqt OR PLD_pngqt OR PLD_ppmqt OR PLD_tiffqt OR PLD_epsqt OR PLD_pdfqt OR PLD_qtwidget OR PLD_svgqt)
