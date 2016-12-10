# bindings/qt_gui/pyqt5/CMakeLists.txt
### Process this file with cmake to produce Makefile
###
# Copyright (C) 2016 Hazen Babcock
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
    )

  #message("DEBUG: PYQT_SIP_DIR = ${PYQT_SIP_DIR}")
  #message("DEBUG: PYQT_SIP_FLAGS = ${PYQT_SIP_FLAGS}")

  add_custom_command(
    OUTPUT ${plplot_pyqt5_HDR} ${plplot_pyqt5_SRC}
    COMMAND ${SIP_EXECUTABLE} -c . -b plplot_pyqt5.sbf -I${PYQT_SIP_DIR} ${PYQT_SIP_FLAGS} ${CMAKE_CURRENT_SOURCE_DIR}/plplot_pyqt5.sip
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

  add_library(plplot_pyqt5 MODULE ${plplot_pyqt5_SRC})
  if(BUILD_SHARED_LIBS)
    set_target_properties(plplot_pyqt5 PROPERTIES
      COMPILE_DEFINITIONS "USINGDLL"
      )
  endif(BUILD_SHARED_LIBS)

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

  if(ENABLE_DYNDRIVERS)
    target_link_libraries(
      plplot_pyqt5
      plplotqt
      plplot
      ${PYTHON_LIBRARIES}
      Qt5::Svg Qt5::Gui Qt5::PrintSupport
      )
  else(ENABLE_DYNDRIVERS)
    # With static device drivers, all the plplotqt code is
    # in the plplot library so plplotqt becomes redundant.
    target_link_libraries(
      plplot_pyqt5
      plplot
      ${PYTHON_LIBRARIES}
      Qt5::Svg Qt5::Gui Qt5::PrintSupport
      )
  endif(ENABLE_DYNDRIVERS)
  add_dependencies(plplot_pyqt5 generate_pyqt5_source)

  # FIXME: Does this do anything important? It did not work so I
  #        just commented it out without any apparent side effects.
  # Update the target COMPILE_DEFINITIONS and INCLUDE_DIRECTORIES
  #set_qt4_target_properties(plplot_pyqt5)

  if(USE_RPATH)
    set_target_properties(
      plplot_pyqt5
      PROPERTIES
      INSTALL_RPATH "${LIB_INSTALL_RPATH}"
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
