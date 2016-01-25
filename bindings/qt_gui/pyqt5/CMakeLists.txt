# bindings/qt_gui/pyqt5/CMakeLists.txt
### Process this file with cmake to produce Makefile
###
# Copyright (C) 2016 Hazen Babcock
# Copyright (C) 2009-2013 Alan W. Irwin
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
# along with PLplot; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA

if(ENABLE_pyqt5)

  set(plplot_pyqt5_HDR ${CMAKE_CURRENT_BINARY_DIR}/sipAPIplplot_pyqt5.h)

  set_source_files_properties(
    ${plplot_pyqt5_HDR}
    PROPERTIES
    GENERATED ON
    )

  set(plplot_pyqt5_SRC
    ${CMAKE_CURRENT_BINARY_DIR}/sipplplot_pyqt5cmodule.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/sipplplot_pyqt5QtExtWidget.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/sipplplot_pyqt5QtPLDriver.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/sipplplot_pyqt5QtPLWidget.cpp
    )
  set_source_files_properties(
    ${plplot_pyqt5_SRC}
    PROPERTIES
    GENERATED ON
    COMPILE_FLAGS "-DUSINGDLL"
    )

  #message("DEBUG: PYQT_SIP_DIR = ${PYQT_SIP_DIR}")
  #message("DEBUG: PYQT_SIP_FLAGS = ${PYQT_SIP_FLAGS}")
  
  add_custom_command(
    OUTPUT ${plplot_pyqt5_HDR} ${plplot_pyqt5_SRC}
    #COMMAND ${SIP_EXECUTABLE} -c . -b plplot_pyqt5.sbf -I${PYQT_SIP_DIR} ${PYQT_SIP_FLAGS} ${CMAKE_CURRENT_SOURCE_DIR}/plplot_pyqt5.sip
    COMMAND ${SIP_EXECUTABLE} -c . -b plplot_pyqt5.sbf -I/usr/share/sip/PyQt5/ ${PYQT_SIP_FLAGS} ${CMAKE_CURRENT_SOURCE_DIR}/plplot_pyqt5.sip    
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/plplot_pyqt5.sip
    )
  add_custom_target(generate_pyqt5_source
    DEPENDS ${plplot_pyqt5_HDR} ${plplot_pyqt5_SRC}
    )

  # Assume sip.h can be found in PYTHON_INCLUDE_PATH
  include_directories(
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/lib/qsastime
    ${CMAKE_BINARY_DIR}
    ${CMAKE_BINARY_DIR}/include
    ${PYTHON_INCLUDE_PATH}
    )

  add_library(
    plplot_pyqt5
    MODULE
    ${plplot_pyqt5_SRC}
    )

  if(WIN32_OR_CYGWIN)
    set_target_properties(
      plplot_pyqt5
      PROPERTIES
      PREFIX ""
      SUFFIX ".pyd"
      )
  else(WIN32_OR_CYGWIN)
    set_target_properties(plplot_pyqt5 PROPERTIES PREFIX "")
  endif(WIN32_OR_CYGWIN)

  target_link_libraries(
    plplot_pyqt5
    plplotqt
    plplot
    ${PYTHON_LIBRARIES}
    ${QT_LIBRARIES}
    )
  add_dependencies(plplot_pyqt5 generate_pyqt5_source)

  # Update the target COMPILE_DEFINITIONS and INCLUDE_DIRECTORIES
  #set_qt_target_properties(plplot_pyqt5)

  if(USE_RPATH)
    set_target_properties(
      plplot_pyqt5
      PROPERTIES
      INSTALL_RPATH "${LIB_INSTALL_RPATH}"
      INSTALL_NAME_DIR "${PYTHON_INSTDIR}"
      )
  else(USE_RPATH)
    set_target_properties(
      plplot_pyqt5
      PROPERTIES
      INSTALL_NAME_DIR "${PYTHON_INSTDIR}"
      )
  endif(USE_RPATH)

  install(
    TARGETS plplot_pyqt5
    EXPORT export_plplot
    LIBRARY DESTINATION ${PYTHON_INSTDIR}
    )

endif(ENABLE_pyqt5)