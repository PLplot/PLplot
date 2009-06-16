# bindings/python/CMakeLists.txt
### Process this file with cmake to produce Makefile
###
# Copyright (C) 2009 Hazen Babcock
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
  #message(" ENABLE_pyqt4")
  file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/pyqt4_config)

  set(PYQT4_CONFIG_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/pyqt4_config)

  add_custom_command(
    OUTPUT ${PYQT4_CONFIG_DIRECTORY}/config.py.in
    COMMAND ${CMAKE_COMMAND} -E touch ${PYQT4_CONFIG_DIRECTORY}/config.py.in
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/plplot_pyqt.sip ${PYQT4_CONFIG_DIRECTORY}/plplot_pyqt.sip
    COMMAND python ${CMAKE_CURRENT_SOURCE_DIR}/config.py
    WORKING_DIRECTORY ${PYQT4_CONFIG_DIRECTORY}/
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/config.py ${CMAKE_CURRENT_SOURCE_DIR}/plplot_pyqt.sip
    )
  add_custom_target(generate_pyqt_source
    DEPENDS ${PYQT4_CONFIG_DIRECTORY}/config.py.in
    )

  set_source_files_properties(
    ${PYQT4_CONFIG_DIRECTORY}/sipAPIplplot_pyqt.h
    ${PYQT4_CONFIG_DIRECTORY}/sipplplot_pyqtcmodule.cpp
    ${PYQT4_CONFIG_DIRECTORY}/sipplplot_pyqtQtExtWidget.cpp
    ${PYQT4_CONFIG_DIRECTORY}/sipplplot_pyqtQtPLDriver.cpp
    ${PYQT4_CONFIG_DIRECTORY}/sipplplot_pyqtQtPLWidget.cpp
    PROPERTIES GENERATED ON
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
    plplot_pyqt 
    MODULE
    ${PYQT4_CONFIG_DIRECTORY}/sipplplot_pyqtcmodule.cpp
    ${PYQT4_CONFIG_DIRECTORY}/sipplplot_pyqtQtExtWidget.cpp
    ${PYQT4_CONFIG_DIRECTORY}/sipplplot_pyqtQtPLDriver.cpp
    ${PYQT4_CONFIG_DIRECTORY}/sipplplot_pyqtQtPLWidget.cpp
    )

  set_target_properties(plplot_pyqt PROPERTIES PREFIX "")

  if(ENABLE_DYNDRIVERS)
    target_link_libraries(
      plplot_pyqt
      plplotqt${LIB_TAG}
      plplot${LIB_TAG}
      ${QT_LIBRARIES}    
      ${PYTHON_LIBRARIES}
      )
  else(ENABLE_DYNDRIVERS)
    target_link_libraries(
      plplot_pyqt
      plplot${LIB_TAG}
      ${QT_LIBRARIES}
      ${PYTHON_LIBRARIES}
      )
  endif(ENABLE_DYNDRIVERS)
  add_dependencies(plplot_pyqt generate_pyqt_source)

  #message(${PYTHON_INSTDIR})

  install(
    TARGETS plplot_pyqt
    LIBRARY
    DESTINATION ${PYTHON_INSTDIR}
    )

endif(ENABLE_pyqt4)
