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
# device driver, the plplotqt library, and the plplot_pyqt4 Python extension
# module.
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
# PLD_memqt               - ON means the memqt device is enabled.
# pc_qt_COMPILE_FLAGS	  - Space-separated Qt4-related COMPILE_FLAGS required
#                           for the pkg-config configuration case
#                           where "include(${QT_USE_FILE})" has no effect.
# qt_COMPILE_FLAGS	  - empty.  Use "include(${QT_USE_FILE})" instead
#                           (but only in directories where it is needed).
# qt_LINK_FLAGS	  	  - empty.  Use "include(${QT_USE_FILE})" instead 
#                           (but only in directories where it is needed).
#                           That command defines QT_LIBRARIES which should
#                           be used instead of the (now empty) qt_LINK_FLAGS.
# qt_TARGETS		  - list of targets which the qt dynamic device
#			    depends on.
# qt_RPATH	       	  - RPATH directory list for qt device driver.
# DRIVERS_LINK_FLAGS  	  - list of device LINK_FLAGS for case when
#			    ENABLE_DYNDRIVERS OFF.  (qt_TARGETS not included
#			    since source code used instead of this target
#			    for ENABLE_DYNDRIVERS OFF.)
# qt_SOURCE		  - list of source files to be appended to qt.cpp.
# ENABLE_qt		  - ON means the plplot_qt library is enabled.
# ENABLE_pyqt4		  - ON means the plplot_pyqt4 Python extension module
# 			    is enabled.
# SIP_EXECUTABLE	  - full path for sip
# PYQT_SIP_DIR		  - sip system directory
# PYQT_SIP_FLAGS	  - sip command flags

if(PLD_bmpqt OR PLD_jpgqt OR PLD_pngqt OR PLD_ppmqt OR PLD_tiffqt OR PLD_epsqt OR PLD_pdfqt OR PLD_qtwidget OR PLD_svgqt OR PLD_extqt OR PLD_memqt)
  set(ANY_QT_DEVICE ON)
endif(PLD_bmpqt OR PLD_jpgqt OR PLD_pngqt OR PLD_ppmqt OR PLD_tiffqt OR PLD_epsqt OR PLD_pdfqt OR PLD_qtwidget OR PLD_svgqt OR PLD_extqt OR PLD_memqt)

if(ANY_QT_DEVICE)
  if(NOT CMAKE_CXX_COMPILER_WORKS)
    message(STATUS "WARNING: no working C++ compiler so "
      "disabling all qt devices."
      )
    set(ANY_QT_DEVICE OFF)
  endif(NOT CMAKE_CXX_COMPILER_WORKS)
endif(ANY_QT_DEVICE)

if(DEFAULT_NO_BINDINGS)
  option(ENABLE_qt "Enable Qt bindings" OFF)
  option(ENABLE_pyqt4 "Enable pyqt4 Python extension module" OFF)
else(DEFAULT_NO_BINDINGS)
  option(ENABLE_qt "Enable Qt bindings" ON)
  option(ENABLE_pyqt4 "Enable pyqt4 Python extension module" ON)
endif(DEFAULT_NO_BINDINGS)

# ENABLE_qt depends on PLD_extqt
if(NOT ANY_QT_DEVICE)
    set(PLD_extqt OFF CACHE BOOL "Enable Qt ext device" FORCE)
endif(NOT ANY_QT_DEVICE)

if(ENABLE_qt AND NOT PLD_extqt)
  message(STATUS
    "WARNING: PLD_extqt is OFF so "
    "setting ENABLE_qt to OFF."
    )
  set(ENABLE_qt OFF CACHE BOOL "Enable Qt bindings" FORCE)
endif(ENABLE_qt AND NOT PLD_extqt)

# All qt devices depend on ENABLE_qt
if(NOT ENABLE_qt)
  message(STATUS
    "WARNING: ENABLE_qt is OFF so "
    "setting all qt devices to OFF."
    )
  set(ANY_QT_DEVICE OFF)
endif(NOT ENABLE_qt)

if(NOT ANY_QT_DEVICE)
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
    set(PLD_memqt OFF CACHE BOOL "Enable Qt mem device" FORCE)
endif(NOT ANY_QT_DEVICE)

if(ANY_QT_DEVICE)
  # Use a minimum version corresponding to the version installed by
  # Debian Wheezy.  I assume all other non-enterprise Linux distros
  # have this version of Qt4 or later.
  find_package(Qt4 4.8.2 REQUIRED QtCore QtGui QtSvg)

  # QT4_FOUND is defined to be true or false by find_package(Qt4 ...)
  if(QT4_FOUND)
    # Do not include(${QT_USE_FILE}) here because it contaminates ALL
    # compile properties with Qt flags from (this) top-level directory
    # on down.  Use this alternative instead which includes a function
    # set_qt_target_properties which should be called with the
    # appropriate target argument whenever a Qt4-related target is built.
    include(ndp_UseQt4)

    # QT_COMPILE_DEFINITIONS (used only for pc_qt_COMPILE_FLAGS below),
    # QT_INCLUDE_DIRECTORIES (used only for pc_qt_COMPILE_FLAGS below),
    # NP_COMPILE_DEFINITIONS (used only in set_qt_properties function),
    # NP_QT_INCLUDE_DIRECTORIES (used only in set_qt_properties function), and
    # QT_LIBRARIES (used wherever link with qt libraries is needed)
    # are now defined.

    # Only used for pkg-config case.
    set(pc_qt_COMPILE_FLAGS ${QT_COMPILE_DEFINES} ${QT_INCLUDE_DIRECTORIES})
    string(REGEX REPLACE ";" " " pc_qt_COMPILE_FLAGS "${pc_qt_COMPILE_FLAGS}")
    # Work around pkg-config issues (see bug report
    # <https://bugs.freedesktop.org/show_bug.cgi?id=72584>) with
    # multiple -isystem tags by replacing them with "-I"
    string(REGEX REPLACE "-isystem " "-I" pc_qt_COMPILE_FLAGS "${pc_qt_COMPILE_FLAGS}")
    message(STATUS "pc_qt_COMPILE_FLAGS = ${pc_qt_COMPILE_FLAGS}")

    # These two must be explicitly empty since other alternative means
    # are used.
    set(qt_COMPILE_FLAGS)
    set(qt_LINK_FLAGS)

    # ${QT_LIBRARY_DIR} defined by above find_package(Qt4 ...) call.
    set(qt_RPATH ${QT_LIBRARY_DIR})
    filter_rpath(qt_RPATH)
    #message("qt_LIBRARY_DIR = ${qt_LIBRARY_DIR}")
  else(QT4_FOUND)
    message(STATUS "WARNING: Qt4 development environment not found so "
      "disabling all qt devices."
      )
    set(ANY_QT_DEVICE OFF)
  endif(QT4_FOUND)
endif(ANY_QT_DEVICE)

if(ENABLE_pyqt4 AND NOT ENABLE_python)
  message(STATUS
    "WARNING: ENABLE_python is OFF so "
    "setting ENABLE_pyqt4 to OFF."
    )
  set(ENABLE_pyqt4 OFF CACHE BOOL "Enable pyqt4 Python extension module " FORCE)
endif(ENABLE_pyqt4 AND NOT ENABLE_python)

if(ENABLE_pyqt4 AND NOT ENABLE_qt)
  message(STATUS
    "WARNING: ENABLE_qt is OFF so "
    "setting ENABLE_pyqt4 to OFF."
    )
  set(ENABLE_pyqt4 OFF CACHE BOOL "Enable pyqt4 Python extension module " FORCE)
endif(ENABLE_pyqt4 AND NOT ENABLE_qt)

if(ENABLE_qt)
  set(qt_gui_true "")
else(ENABLE_qt)
  set(qt_gui_true "#")
endif(ENABLE_qt)

if(ANY_QT_DEVICE)
  if(ENABLE_DYNDRIVERS)
    if(ENABLE_qt)
      set(qt_SOURCE)
      set(qt_TARGETS plplotqt${LIB_TAG})
    else(ENABLE_qt)
      # if qt disabled, then must include full source and forget
      # qt_TARGETS for this dynamic device.
      set(qt_SOURCE ${CMAKE_SOURCE_DIR}/bindings/qt_gui/plqt.cpp)
    endif(ENABLE_qt)
  else(ENABLE_DYNDRIVERS)
    # N.B. no qt_TARGETS here since use appropriate source code (see below)
    # instead to break circular linking.
    set(DRIVERS_LINK_FLAGS ${DRIVERS_LINK_FLAGS} ${qt_LINK_FLAGS})
    set(qt_SOURCE ${CMAKE_SOURCE_DIR}/bindings/qt_gui/plqt.cpp)
  endif(ENABLE_DYNDRIVERS)
endif(ANY_QT_DEVICE)

if(ENABLE_pyqt4)
  find_program(SIP_EXECUTABLE sip)
  if(NOT SIP_EXECUTABLE)
    message(STATUS
      "WARNING: sip not found so setting ENABLE_pyqt4 to OFF."
      )
    set(ENABLE_pyqt4 OFF CACHE BOOL "Enable pyqt4 Python extension module " FORCE)
  endif(NOT SIP_EXECUTABLE)
endif(ENABLE_pyqt4)

if(ENABLE_pyqt4)
  execute_process(
    COMMAND ${PYTHON_EXECUTABLE} -c "from PyQt4 import pyqtconfig; print pyqtconfig.Configuration().pyqt_sip_dir"
    OUTPUT_VARIABLE PYQT_SIP_DIR
    RESULT_VARIABLE PYQT_SIP_DIR_ERR
    OUTPUT_STRIP_TRAILING_WHITESPACE
    )
  if(PYQT_SIP_DIR_ERR)
    message(STATUS
      "WARNING: could not find sip directory so setting ENABLE_pyqt4 to OFF."
      )
    set(ENABLE_pyqt4 OFF CACHE BOOL "Enable pyqt4 Python extension module " FORCE)
  endif(PYQT_SIP_DIR_ERR)
endif(ENABLE_pyqt4)

if(ENABLE_pyqt4)
  execute_process(
    COMMAND ${PYTHON_EXECUTABLE} -c "from PyQt4 import pyqtconfig; print pyqtconfig.Configuration().pyqt_sip_flags"
    OUTPUT_VARIABLE PYQT_SIP_FLAGS
    RESULT_VARIABLE PYQT_SIP_FLAGS_ERR
    OUTPUT_STRIP_TRAILING_WHITESPACE
    )
  if(PYQT_SIP_FLAGS_ERR)
    message(STATUS
      "WARNING: could not find sip flags so setting ENABLE_pyqt4 to OFF."
      )
    set(ENABLE_pyqt4 OFF CACHE BOOL "Enable pyqt4 Python extension module " FORCE)
  endif(PYQT_SIP_FLAGS_ERR)
  # Must change from blank-delimited string to CMake list so that sip
  # COMMAND will work properly with these flags later on.
  string(REGEX REPLACE " " ";" PYQT_SIP_FLAGS "${PYQT_SIP_FLAGS}") 
endif(ENABLE_pyqt4)
