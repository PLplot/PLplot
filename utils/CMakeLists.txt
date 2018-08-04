# utils/CMakeLists.txt for PLplot
# Copyright (C) 2006-2018 Alan W. Irwin
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

include_directories(
  ${CMAKE_SOURCE_DIR}/include
  ${CMAKE_SOURCE_DIR}/lib/qsastime
  ${CMAKE_BINARY_DIR}
  ${CMAKE_BINARY_DIR}/include
  ${CMAKE_BINARY_DIR}/lib/qsastime
  )

if(USE_RPATH)
  get_target_property(LIB_INSTALL_RPATH PLPLOT::plplot INSTALL_RPATH)
endif(USE_RPATH)

if(PLD_plmeta)
  configure_executable_build(plrender "plrender.c" "PLPLOT::plplot")
  if(BUILD_SHARED_LIBS)
    set_target_properties(${WRITEABLE_TARGET}plrender
      PROPERTIES
      COMPILE_DEFINITIONS "USINGDLL"
      )
  endif(BUILD_SHARED_LIBS)
  if(USE_RPATH)
    set_target_properties(${WRITEABLE_TARGET}plrender
      PROPERTIES
      INSTALL_RPATH "${LIB_INSTALL_RPATH}"
      )
  endif(USE_RPATH)
  install(TARGETS ${WRITEABLE_TARGET}plrender EXPORT export_plplot DESTINATION ${BIN_DIR})
endif(PLD_plmeta)

configure_executable_build(pltek "pltek.c" "PLPLOT::plplot")
if(BUILD_SHARED_LIBS)
  set_target_properties(${WRITEABLE_TARGET}pltek
    PROPERTIES
    COMPILE_DEFINITIONS "USINGDLL"
    )
endif(BUILD_SHARED_LIBS)
if(USE_RPATH)
  set_target_properties(${WRITEABLE_TARGET}pltek
    PROPERTIES
    INSTALL_RPATH "${LIB_INSTALL_RPATH}"
    )
endif(USE_RPATH)
install(TARGETS ${WRITEABLE_TARGET}pltek EXPORT export_plplot DESTINATION ${BIN_DIR})

# Build simple executable to check parity bits of all characters read
# by stdin.
add_executable(parity_bit_check parity_bit_check.c)

if(ENABLE_wxwidgets AND NOT OLD_WXWIDGETS)
# Build wxwidgets applications with same wxwidgets compile and link flags
# as used with the PLplot wxwidgets device driver.

  set(wxPLViewer_SRCS wxplviewer.cpp wxplframe.cpp ${CMAKE_SOURCE_DIR}/drivers/wxwidgets_comms.cpp)
  set_source_files_properties(${wxPLViewer_SRCS}
    PROPERTIES COMPILE_FLAGS "${wxwidgets_COMPILE_FLAGS}")
  include_directories(
    ${CMAKE_SOURCE_DIR}/bindings/wxwidgets
    ${CMAKE_BINARY_DIR}/bindings/wxwidgets
    ${CMAKE_SOURCE_DIR}/bindings/c++
    )

  set(wxPLViewer_link_libraries PLPLOT::plplotwxwidgets PLPLOT::plplotcxx ${wxwidgets_LINK_FLAGS} ${MATH_LIB} ${RT_LIB})
  configure_executable_build(wxPLViewer "${wxPLViewer_SRCS}" "${wxPLViewer_link_libraries}")

  # Set this property rather than using the WIN32 signature of add_executable
  # inside the configure_executable_build function.
  set_target_properties(${WRITEABLE_TARGET}wxPLViewer
    PROPERTIES
    WIN32_EXECUTABLE "ON"
    )
  if(BUILD_SHARED_LIBS)
    set_target_properties(${WRITEABLE_TARGET}wxPLViewer
      PROPERTIES
      COMPILE_DEFINITIONS "USINGDLL"
      )
  endif(BUILD_SHARED_LIBS)
  if(USE_RPATH)
    # Assume that plplotwxwidgets and plplotcxx have the same install location as plplot
    set_target_properties(${WRITEABLE_TARGET}wxPLViewer
      PROPERTIES
      INSTALL_RPATH "${LIB_INSTALL_RPATH}"
      )
  endif(USE_RPATH)

  install(TARGETS ${WRITEABLE_TARGET}wxPLViewer EXPORT export_plplot DESTINATION ${BIN_DIR})
endif(ENABLE_wxwidgets AND NOT OLD_WXWIDGETS)
