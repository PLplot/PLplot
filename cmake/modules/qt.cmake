# cmake/modules/qt.cmake
#
# Copyright (C) 2009-2016 Alan W. Irwin
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
# PLPLOT_USE_Qt5          - ON means the Qt5 libraries are used rather than the default Qt4.
# pc_qt_COMPILE_FLAGS	  - Space-separated Qt-related COMPILE_FLAGS used strictly
#                           just for the pkg-config configuration case.
# pc_qt_LIBRARIES_LIST    - List of Qt-related libraries used strictly
#                           just for the pkg-config configuration case.  Note it is the same as
#                           QT_LIBRARIES for the Qt4 case so for src/CMakeLists.txt
#                           logic is required to avoid this redundancy.
# qt_COMPILE_FLAGS	  - empty.  Not needed. COMPILE_FLAGS are taken care of explicitly in the
#                           Qt5 case by a special target_link_libraries call and taken care of
#                           explicitly in the Qt4 case by the function
#                           set_qt4_target_properties(<target>) defined by "include(ndp_UseQt4)".
# qt_LINK_FLAGS	  	  - empty.  Not needed. Linking is taken care of explicitly in the
#                           Qt5 case by a special target_link_libraries call and taken care of
#                           explicitly in the Qt4 case by the QT_LIBRARIES variable that is defined
#                           by "include(ndp_UseQt4)".
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
# ENABLE_pyqt5		  - ON means the plplot_pyqt5 Python extension module
# 			    is enabled.
# ENABLE_smoke            - ON means the smoke plplotqt library is enabled.
# SIP_EXECUTABLE	  - full path for sip
# PYQT_SIP_DIR		  - sip system directory
# PYQT_SIP_FLAGS	  - sip command flags

if(DEFAULT_NO_BINDINGS)
  option(ENABLE_qt "Enable Qt binding" OFF)
  option(ENABLE_pyqt4 "Enable pyqt4 Python extension module" OFF)
  option(ENABLE_pyqt5 "Enable pyqt5 Python extension module" OFF)
  option(ENABLE_smoke "Enable smoke Qt binding" OFF)
else(DEFAULT_NO_BINDINGS)
  option(ENABLE_qt "Enable Qt binding" ON)
  option(ENABLE_pyqt4 "Enable pyqt4 Python extension module" ON)
  option(ENABLE_pyqt5 "Enable pyqt5 Python extension module" ON)
  # Still experimental so this should default to OFF, but the user
  # has the option to turn it ON.
  option(ENABLE_smoke "Enable smoke Qt binding" OFF)
endif(DEFAULT_NO_BINDINGS)

if(ENABLE_qt)
  # Our experience is Qt4 is better than Qt5 in terms of character
  # alignment (e.g., -dev svgqt results) and memory management.  So
  # by default we search for Qt4 first then Qt5, but the user
  # has the option to force Qt5 if they so desire.
  option(PLPLOT_USE_QT5 "Use Qt5" OFF)

  if(NOT PLPLOT_USE_QT5)
    # MAINTENANCE 2016-11.
    # Use a minimum version corresponding to the version installed by
    # Debian stable.  I assume all other non-enterprise Linux distros,
    # Mac OS X, and Windows platforms also give access to this version
    # of Qt4 or later.
    find_package(Qt4 4.8.6 COMPONENTS QtCore QtGui QtSvg)

    # QT4_FOUND is defined to be true or false by find_package(Qt4 ...)
    if(QT4_FOUND)
      # Do not include(${QT_USE_FILE}) here because it contaminates ALL
      # compile properties with Qt flags from (this) top-level directory
      # on down.  Use this alternative instead which includes a function
      # set_qt4_target_properties which should be called with the
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
      message(STATUS "Qt4 pc_qt_COMPILE_FLAGS = ${pc_qt_COMPILE_FLAGS}")

      # ${QT_LIBRARY_DIR} defined by above find_package(Qt4 ...) call.
      set(qt_RPATH ${QT_LIBRARY_DIR})
      filter_rpath(qt_RPATH)
      #message("qt_LIBRARY_DIR = ${qt_LIBRARY_DIR}")
      set(pc_qt_LIBRARIES_LIST "${QT_LIBRARIES}")
      message(STATUS "Qt4 pc_qt_LIBRARIES_LIST = ${pc_qt_LIBRARIES_LIST}")
      if(NOT ENABLE_DYNDRIVERS)
	list(APPEND DRIVERS_LINK_FLAGS "${QT_LIBRARIES}")
      endif(NOT ENABLE_DYNDRIVERS)
    else(QT4_FOUND)
      message(STATUS "WARNING: Suitable Qt4 development environment not found so trying Qt5 instead."
	)
      set(PLPLOT_USE_QT5 ON CACHE BOOL "Use Qt5" FORCE)
    endif(QT4_FOUND)
  endif(NOT PLPLOT_USE_QT5)

  if(PLPLOT_USE_QT5)
    # We are using the latest Qt5 support method, see
    # <http://doc.qt.io/qt-5/cmake-manual.html>.

    # Find needed components of Qt5.  Minimum value of the version is
    # 5.3.1 because there were significant text alignment bugs in prior versions.
    find_package(Qt5 5.3.1 COMPONENTS Svg Gui PrintSupport)
    if(Qt5_FOUND)
      message(STATUS "Attempting to use Qt5 so have set PLD_epsqt to OFF since Qt5 does not support PostScript")
      set(PLD_epsqt OFF CACHE BOOL "Enable Qt EPS device" FORCE)

      # Calculate Qt5_library_COMPILE_FLAGS and Qt5_library_LINK_FLAGS
      # to be used for the pkg-config case.

      # Note that theoretically you could use execute_process and cmake
      # --find-package option to determine these flags, but
      # --find-package is not maintained now and might well disappear in
      # the future, there is currently a Qt5 CMake support file bug
      # (badly named GL library) that needs to be addressed for
      # --find-package to work even at the moment, and also there is a
      # Qt5 CMake support file bug such that the required macro option
      # settings are not returned by this approach.

      # Another alternative would be to use the plplotqt target properties,
      # but translating those into simple compile and link flags is non-trivial
      # and in fact is the issue that still needs to be addressed
      # in order for --find-package to work properly for all varieties
      # of target properties.

      # Therefore, use a brute-force approach (determined by looking at
      # the compile flags and link flags that CMake finally figures out
      # to build the plplotqt library).

      # MAINTENANCE: This list will need revision whenever the PLplot
      # dependencies on the Qt5 components are revised.
      set(Qt5_library_name_list
	Core
	Gui
	PrintSupport
	Widgets
	Svg
	)

      set(pc_qt_COMPILE_FLAGS)
      foreach(Qt5_library_name ${Qt5_library_name_list})
	string(TOUPPER ${Qt5_library_name} macro_core_name)
	# Set required macros so headers will be found.
	set(pc_qt_COMPILE_FLAGS "${pc_qt_COMPILE_FLAGS} -DQT_${macro_core_name}_LIB")
	find_file(${Qt5_library_name}_header_directory Qt${Qt5_library_name} HINTS ${Qt5${Qt5_library_name}_INCLUDE_DIRS})
	if(${Qt5_library_name}_header_directory)
	  if(${Qt5_library_name} STREQUAL "Core")
	    get_filename_component(parent_directory ${${Qt5_library_name}_header_directory} DIRECTORY)
	    set(pc_qt_COMPILE_FLAGS "${pc_qt_COMPILE_FLAGS} -I${parent_directory}")
	  endif(${Qt5_library_name} STREQUAL "Core")
	  set(pc_qt_COMPILE_FLAGS "${pc_qt_COMPILE_FLAGS} -I${${Qt5_library_name}_header_directory}")
	else(${Qt5_library_name}_header_directory)
	  message(STATUS "${Qt5_library_name}_header_directory = ${${Qt5_library_name}_header_directory}")
	  message(FATAL_ERROR "${Qt5_library_name} header_directory not found.")
	endif(${Qt5_library_name}_header_directory)
      endforeach(Qt5_library_name ${Qt5_library_name_list})
      message(STATUS "Qt5 pc_qt_COMPILE_FLAGS = ${pc_qt_COMPILE_FLAGS}")

      set(pc_qt_LIBRARIES_LIST)
      foreach(Qt5_library_name ${Qt5_library_name_list})
	set(Qt5_library_fullpath ${Qt5${Qt5_library_name}_LIBRARIES})
	if(Qt5_library_fullpath MATCHES "^Qt5::")
	  # This is the Qt5 convention for imported library names, and
          # according to <http://doc.qt.io/qt-5/cmake-manual.html> the
          # corresponding locations can be found as follows (and noting
          # from the CMP0026 documentation and cmake-devel list discussion
          # from Nils Gladitz that that policy only
          # excludes using _build_ target LOCATION properties and not
          # _imported_ target LOCATION properties like this one):
	  get_target_property(Qt5_library_fullpath ${Qt5_library_fullpath} LOCATION)
	  if(Qt5_library_fullpath)
	    list(APPEND pc_qt_LIBRARIES_LIST ${Qt5_library_fullpath})
	  endif(Qt5_library_fullpath)
	else(Qt5_library_fullpath MATCHES "^Qt5::")
	  list(APPEND pc_qt_LIBRARIES_LIST ${Qt5_library_fullpath})
	endif(Qt5_library_fullpath MATCHES "^Qt5::")
      endforeach(Qt5_library_name ${Qt5_library_name_list})

      message(STATUS "Qt5 pc_qt_LIBRARIES_LIST = ${pc_qt_LIBRARIES_LIST}")
    else(Qt5_FOUND)
      message(STATUS
	"WARNING: Suitable Qt5 development environment not found so disabling qt"
	)
      set(ENABLE_qt OFF CACHE BOOL "Enable Qt binding" FORCE)
    endif(Qt5_FOUND)
  endif(PLPLOT_USE_QT5)
endif(ENABLE_qt)

# MAINTENANCE: mention every qt device here.
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

# All qt devices depend on ENABLE_qt
if(NOT ENABLE_qt AND ANY_QT_DEVICE)
  message(STATUS
    "WARNING: ENABLE_qt is OFF so "
    "setting all qt devices to OFF."
    )
  set(ANY_QT_DEVICE OFF)
endif(NOT ENABLE_qt AND ANY_QT_DEVICE)

# MAINTENANCE: must mention all qt devices here.
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

# The libplplotqt build depends on MOC file generation which
# depends on at least some qt devices being
# enabled.  Therefore, disable the library build if there
# are no qt devices.  (In any case, I think that library build
# would not be useful for much without devices.)

if(NOT ANY_QT_DEVICE AND ENABLE_qt)
  message(STATUS
    "WARNING: ANY_QT_DEVICE is OFF so "
    "setting ENABLE_qt to OFF."
    )
  set(ENABLE_qt OFF CACHE BOOL "Enable Qt binding" FORCE)
endif(NOT ANY_QT_DEVICE AND ENABLE_qt)

if(ENABLE_qt)
  # These two must be explicitly empty since other alternative means
  # are used.
  set(qt_COMPILE_FLAGS)
  set(qt_LINK_FLAGS)
endif(ENABLE_qt)

if(PLPLOT_USE_QT5)
  # Calculate what will be the AUTOMOC_MOC_OPTIONS property
  # for targets where the AUTOMOC property needs to be set.
  set(PLPLOT_AUTOMOC_MOC_OPTIONS)
  if(PLD_bmpqt)
    list(APPEND PLPLOT_AUTOMOC_MOC_OPTIONS -DPLD_bmpqt)
  endif(PLD_bmpqt)
  if(PLD_jpgqt)
    list(APPEND PLPLOT_AUTOMOC_MOC_OPTIONS -DPLD_jpgqt)
  endif(PLD_jpgqt)
  if(PLD_pngqt)
    list(APPEND PLPLOT_AUTOMOC_MOC_OPTIONS -DPLD_pngqt)
  endif(PLD_pngqt)
  if(PLD_ppmqt)
    list(APPEND PLPLOT_AUTOMOC_MOC_OPTIONS -DPLD_ppmqt)
  endif(PLD_ppmqt)
  if(PLD_tiffqt)
    list(APPEND PLPLOT_AUTOMOC_MOC_OPTIONS -DPLD_tiffqt)
  endif(PLD_tiffqt)
  if(PLD_svgqt)
    list(APPEND PLPLOT_AUTOMOC_MOC_OPTIONS -DPLD_svgqt)
  endif(PLD_svgqt)
  if(PLD_epsqt)
    list(APPEND PLPLOT_AUTOMOC_MOC_OPTIONS -DPLD_epsqt)
  endif(PLD_epsqt)
  if(PLD_pdfqt)
    list(APPEND PLPLOT_AUTOMOC_MOC_OPTIONS -DPLD_pdfqt)
  endif(PLD_pdfqt)
  if(PLD_qtwidget)
    list(APPEND PLPLOT_AUTOMOC_MOC_OPTIONS -DPLD_qtwidget)
  endif(PLD_qtwidget)
  if(PLD_extqt)
    list(APPEND PLPLOT_AUTOMOC_MOC_OPTIONS -DPLD_extqt)
  endif(PLD_extqt)
  if(PLD_memqt)
    list(APPEND PLPLOT_AUTOMOC_MOC_OPTIONS -DPLD_memqt)
  endif(PLD_memqt)
endif(PLPLOT_USE_QT5)


if(ENABLE_pyqt4 AND PLPLOT_USE_QT5)
  message(STATUS
    "WARNING: PLPLOT_USE_QT5 is ON so "
    "setting ENABLE_pyqt4 to OFF."
    )
  set(ENABLE_pyqt4 OFF CACHE BOOL "Enable pyqt4 Python extension module " FORCE)
endif(ENABLE_pyqt4 AND PLPLOT_USE_QT5)

if(ENABLE_pyqt5 AND NOT PLPLOT_USE_QT5)
  message(STATUS
    "WARNING: PLPLOT_USE_QT5 is OFF so "
    "setting ENABLE_pyqt5 to OFF."
    )
  set(ENABLE_pyqt5 OFF CACHE BOOL "Enable pyqt5 Python extension module " FORCE)
endif(ENABLE_pyqt5 AND NOT PLPLOT_USE_QT5)

if(ENABLE_pyqt4 AND NOT ENABLE_python)
  message(STATUS
    "WARNING: ENABLE_python is OFF so "
    "setting ENABLE_pyqt4 to OFF."
    )
  set(ENABLE_pyqt4 OFF CACHE BOOL "Enable pyqt4 Python extension module " FORCE)
endif(ENABLE_pyqt4 AND NOT ENABLE_python)

if(ENABLE_pyqt5 AND NOT ENABLE_python)
  message(STATUS
    "WARNING: ENABLE_python is OFF so "
    "setting ENABLE_pyqt5 to OFF."
    )
  set(ENABLE_pyqt5 OFF CACHE BOOL "Enable pyqt5 Python extension module " FORCE)
endif(ENABLE_pyqt5 AND NOT ENABLE_python)


if(ENABLE_pyqt4 AND NOT PLD_extqt)
  message(STATUS
    "WARNING: PLD_extqt is OFF so "
    "setting ENABLE_pyqt4 to OFF."
    )
  set(ENABLE_pyqt4 OFF CACHE BOOL "Enable pyqt4 Python extension module " FORCE)
endif(ENABLE_pyqt4 AND NOT PLD_extqt)

if(ENABLE_pyqt5 AND NOT PLD_extqt)
  message(STATUS
    "WARNING: PLD_extqt is OFF so "
    "setting ENABLE_pyqt5 to OFF."
    )
  set(ENABLE_pyqt5 OFF CACHE BOOL "Enable pyqt5 Python extension module " FORCE)
endif(ENABLE_pyqt5 AND NOT PLD_extqt)


if(ENABLE_qt)
  set(qt_gui_true "")
else(ENABLE_qt)
  set(qt_gui_true "#")
endif(ENABLE_qt)

if(ANY_QT_DEVICE)
  if(ENABLE_DYNDRIVERS)
    if(ENABLE_qt)
      set(qt_SOURCE)
      set(qt_TARGETS plplotqt)
    else(ENABLE_qt)
      # if qt disabled, then must include full source and forget
      # qt_TARGETS for this dynamic device.
      set(qt_SOURCE ${CMAKE_SOURCE_DIR}/bindings/qt_gui/plqt.cpp)
    endif(ENABLE_qt)
  else(ENABLE_DYNDRIVERS)
    # N.B. no qt_TARGETS here since use appropriate source code (see below)
    # instead to break circular linking.
    list(APPEND DRIVERS_LINK_FLAGS ${qt_LINK_FLAGS})
    set(qt_SOURCE ${CMAKE_SOURCE_DIR}/bindings/qt_gui/plqt.cpp)
  endif(ENABLE_DYNDRIVERS)
endif(ANY_QT_DEVICE)

if(ENABLE_pyqt4 OR ENABLE_pyqt5)
  find_program(SIP_EXECUTABLE sip)
  message(STATUS "pyqt: SIP_EXECUTABLE = ${SIP_EXECUTABLE}")
  if(NOT SIP_EXECUTABLE)
    message(STATUS
      "WARNING: sip not found so setting ENABLE_pyqt4 / ENABLE_pyqt5 to OFF."
      )
    set(ENABLE_pyqt4 OFF CACHE BOOL "Enable pyqt4 Python extension module " FORCE)
    set(ENABLE_pyqt5 OFF CACHE BOOL "Enable pyqt5 Python extension module " FORCE)
  endif(NOT SIP_EXECUTABLE)
endif(ENABLE_pyqt4 OR ENABLE_pyqt5)

if(ENABLE_pyqt4)
  execute_process(
    COMMAND ${PYTHON_EXECUTABLE} -c "from PyQt4 import pyqtconfig; print pyqtconfig.Configuration().pyqt_sip_dir"
    OUTPUT_VARIABLE PYQT_SIP_DIR
    RESULT_VARIABLE PYQT_SIP_DIR_ERR
    OUTPUT_STRIP_TRAILING_WHITESPACE
    )
  message(STATUS "pyqt4: PYQT_SIP_DIR = ${PYQT_SIP_DIR}")
  if(PYQT_SIP_DIR_ERR)
    message(STATUS
      "WARNING: could not find sip directory so setting ENABLE_pyqt4 to OFF."
      )
    set(ENABLE_pyqt4 OFF CACHE BOOL "Enable pyqt4 Python extension module " FORCE)
  endif(PYQT_SIP_DIR_ERR)
endif(ENABLE_pyqt4)

if(ENABLE_pyqt5)

  # Hope that the .sip files are in the standard place as PyQt5
  # does not provide a way to determine where they are.
  execute_process(
    COMMAND ${PYTHON_EXECUTABLE} -c "import sys; print sys.prefix"
    OUTPUT_VARIABLE PYQT_SIP_DIR
    RESULT_VARIABLE PYQT_SIP_DIR_ERR
    OUTPUT_STRIP_TRAILING_WHITESPACE
    )

  # FIXME: What about OS-X?
  if (WIN32 AND NOT CYGWIN)
    # FIXME: This is almost surely wrong.
    set(PYQT_SIP_DIR "${PYQT_SIP_DIR}/sip/PyQt5/")
  else (WIN32 AND NOT CYGWIN)
    set(PYQT_SIP_DIR "${PYQT_SIP_DIR}/share/sip/PyQt5/")
  endif (WIN32 AND NOT CYGWIN)

  message(STATUS "pyqt5: PYQT_SIP_DIR = ${PYQT_SIP_DIR}")
  if(PYQT_SIP_DIR_ERR OR NOT EXISTS "${PYQT_SIP_DIR}/QtWidgets/QtWidgetsmod.sip")
    message(STATUS
      "WARNING: could not find sip directory so setting ENABLE_pyqt5 to OFF."
      )
    set(ENABLE_pyqt5 OFF CACHE BOOL "Enable pyqt5 Python extension module " FORCE)
  endif(PYQT_SIP_DIR_ERR OR NOT EXISTS "${PYQT_SIP_DIR}/QtWidgets/QtWidgetsmod.sip")
endif(ENABLE_pyqt5)

if(ENABLE_pyqt4)
  execute_process(
    COMMAND ${PYTHON_EXECUTABLE} -c "from PyQt4 import pyqtconfig; print pyqtconfig.Configuration().pyqt_sip_flags"
    OUTPUT_VARIABLE PYQT_SIP_FLAGS
    RESULT_VARIABLE PYQT_SIP_FLAGS_ERR
    OUTPUT_STRIP_TRAILING_WHITESPACE
    )
  # Must change from blank-delimited string to CMake list so that sip
  # COMMAND will work properly with these flags later on.
  string(REGEX REPLACE " " ";" PYQT_SIP_FLAGS "${PYQT_SIP_FLAGS}")
  message(STATUS "pyqt4: PYQT_SIP_FLAGS = ${PYQT_SIP_FLAGS}")

  if(PYQT_SIP_FLAGS_ERR)
    message(STATUS
      "WARNING: could not find sip flags so setting ENABLE_pyqt4 to OFF."
      )
    set(ENABLE_pyqt4 OFF CACHE BOOL "Enable pyqt4 Python extension module " FORCE)
  endif(PYQT_SIP_FLAGS_ERR)
endif(ENABLE_pyqt4)

if(ENABLE_pyqt5)
  execute_process(
    COMMAND ${PYTHON_EXECUTABLE} -c "from PyQt5.QtCore import PYQT_CONFIGURATION; print PYQT_CONFIGURATION['sip_flags']"
    OUTPUT_VARIABLE PYQT_SIP_FLAGS
    RESULT_VARIABLE PYQT_SIP_FLAGS_ERR
    OUTPUT_STRIP_TRAILING_WHITESPACE
    )
  # Must change from blank-delimited string to CMake list so that sip
  # COMMAND will work properly with these flags later on.
  string(REGEX REPLACE " " ";" PYQT_SIP_FLAGS "${PYQT_SIP_FLAGS}")
  message(STATUS "pyqt5: PYQT_SIP_FLAGS = ${PYQT_SIP_FLAGS}")

  if(PYQT_SIP_FLAGS_ERR)
    message(STATUS
      "WARNING: could not find sip flags so setting ENABLE_pyqt5 to OFF."
      )
    set(ENABLE_pyqt5 OFF CACHE BOOL "Enable pyqt5 Python extension module " FORCE)
  endif(PYQT_SIP_FLAGS_ERR)
endif(ENABLE_pyqt5)

if(ENABLE_smoke)
  if(PLPLOT_USE_QT5)
    # The smoke-qt5 possibility is discussed extensively at
    # <https://github.com/ryanmelt/qtbindings/issues/131> and as of
    # 2015-11 there didn't seem to be much chance of a Qt5
    # version of smoke being developed any time soon (if ever).
    message(STATUS
      "WARNING: smoke only works with Qt4 so setting ENABLE_smoke to OFF."
      )
    set(ENABLE_smoke OFF CACHE BOOL "Enable smoke Qt4 extension module " FORCE)
  else(PLPLOT_USE_QT5)
    find_package(Smoke QUIET COMPONENTS QtCore QtGui)
    message(STATUS "SMOKE_QTCORE_FOUND = ${SMOKE_QTCORE_FOUND}")
    message(STATUS "SMOKE_QTGUI_FOUND = ${SMOKE_QTGUI_FOUND}")
    message(STATUS "SMOKE_GEN_BIN = ${SMOKE_GEN_BIN}")
    message(STATUS "SMOKE_GEN_SHARED = ${SMOKE_GEN_SHARED}")
    message(STATUS "SMOKE_QTCORE_LIBRARY = ${SMOKE_QTCORE_LIBRARY}")
    message(STATUS "SMOKE_QTGUI_LIBRARY = ${SMOKE_QTGUI_LIBRARY}")
    if(SMOKE_QTCORE_FOUND AND SMOKE_QTGUI_FOUND)
      message(STATUS "smoke installation found")
    else(SMOKE_QTCORE_FOUND AND SMOKE_QTGUI_FOUND)
      message(STATUS
	"WARNING: smoke installation not found so setting ENABLE_smoke to OFF."
	)
      set(ENABLE_smoke OFF CACHE BOOL "Enable smoke Qt4 extension module " FORCE)
    endif(SMOKE_QTCORE_FOUND AND SMOKE_QTGUI_FOUND)
  endif(PLPLOT_USE_QT5)
endif(ENABLE_smoke)
