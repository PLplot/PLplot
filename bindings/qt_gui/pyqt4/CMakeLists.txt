# bindings/python/CMakeLists.txt
### Process this file with cmake to produce Makefile
###
# Copyright (C) 2009 Hazen Babcock
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
# along with PLplot; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA

if(ENABLE_pyqt4)

  set(plplot_pyqt4_HDR ${CMAKE_CURRENT_BINARY_DIR}/sipAPIplplot_pyqt4.h)

  set_source_files_properties(
    ${plplot_pyqt4_HDR}
    PROPERTIES
    GENERATED ON
    )

  set(plplot_pyqt4_SRC
    ${CMAKE_CURRENT_BINARY_DIR}/sipplplot_pyqt4cmodule.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/sipplplot_pyqt4QtExtWidget.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/sipplplot_pyqt4QtPLDriver.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/sipplplot_pyqt4QtPLWidget.cpp
    )
  set_source_files_properties(
    ${plplot_pyqt4_SRC}
    PROPERTIES
    GENERATED ON
    COMPILE_FLAGS "-DUSINGDLL"
    )

  #message("DEBUG: PYQT_SIP_DIR = ${PYQT_SIP_DIR}")
  #message("DEBUG: PYQT_SIP_FLAGS = ${PYQT_SIP_FLAGS}")
  
  add_custom_command(
    OUTPUT ${plplot_pyqt4_HDR} ${plplot_pyqt4_SRC}
    COMMAND ${SIP_EXECUTABLE} -c . -b plplot_pyqt4.sbf -I${PYQT_SIP_DIR} ${PYQT_SIP_FLAGS} ${CMAKE_CURRENT_SOURCE_DIR}/plplot_pyqt4.sip
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/plplot_pyqt4.sip
    )
  add_custom_target(generate_pyqt4_source
    DEPENDS ${plplot_pyqt4_HDR} ${plplot_pyqt4_SRC}
    )

  # Assume sip.h can be found in PYTHON_INCLUDE_PATH
  include_directories(
    ${CMAKE_SOURCE_DIR}/drivers
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/lib/qsastime
    ${CMAKE_BINARY_DIR}
    ${CMAKE_BINARY_DIR}/include
    ${PYTHON_INCLUDE_PATH}
    )

  add_library(
    plplot_pyqt4
    MODULE
    ${plplot_pyqt4_SRC}
    )

  set_target_properties(plplot_pyqt4 PROPERTIES PREFIX "")

  target_link_libraries(
    plplot_pyqt4
    plplotqt${LIB_TAG}
    plplot${LIB_TAG}
    ${QT_LIBRARIES}    
    ${PYTHON_LIBRARIES}
    )
  add_dependencies(plplot_pyqt4 generate_pyqt4_source)

  if(USE_RPATH)
    set_target_properties(
      plplot_pyqt4
      PROPERTIES
      INSTALL_RPATH "${LIB_INSTALL_RPATH}"
      INSTALL_NAME_DIR "${PYTHON_INSTDIR}"
      )
  else(USE_RPATH)
    set_target_properties(
      plplot_pyqt4
      PROPERTIES
      INSTALL_NAME_DIR "${PYTHON_INSTDIR}"
      )
  endif(USE_RPATH)

  install(
    TARGETS plplot_pyqt4
    LIBRARY
    DESTINATION ${PYTHON_INSTDIR}
    )

endif(ENABLE_pyqt4)
