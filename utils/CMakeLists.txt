# utils/CMakeLists.txt for PLplot
###
### Process this file with cmake to produce Makefile
###
# Copyright (C) 2006-2016 Alan W. Irwin
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
  )

if(USE_RPATH)
  get_target_property(LIB_INSTALL_RPATH plplot INSTALL_RPATH)
endif(USE_RPATH)

if(PLD_plmeta)
  add_executable(plrender plrender.c)
  if(BUILD_SHARED_LIBS)
    set_target_properties(plrender PROPERTIES
      COMPILE_DEFINITIONS "USINGDLL"
      )
  endif(BUILD_SHARED_LIBS)

  target_link_libraries(plrender plplot)

  if(USE_RPATH)
    set_target_properties(plrender
      PROPERTIES
      INSTALL_RPATH "${LIB_INSTALL_RPATH}"
      )
  endif(USE_RPATH)

  install(TARGETS plrender DESTINATION ${BIN_DIR})
endif(PLD_plmeta)

add_executable(pltek pltek.c)
if(BUILD_SHARED_LIBS)
  set_target_properties(pltek PROPERTIES
    COMPILE_DEFINITIONS "USINGDLL"
    )
endif(BUILD_SHARED_LIBS)

target_link_libraries(pltek plplot)

if(USE_RPATH)
  set_target_properties(pltek
    PROPERTIES
    INSTALL_RPATH "${LIB_INSTALL_RPATH}"
    )
endif(USE_RPATH)

install(TARGETS pltek DESTINATION ${BIN_DIR})

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
  add_executable(wxPLViewer WIN32 ${wxPLViewer_SRCS})
  if(BUILD_SHARED_LIBS)
    set_target_properties(wxPLViewer PROPERTIES
      COMPILE_DEFINITIONS "USINGDLL"
      )
  endif(BUILD_SHARED_LIBS)
  target_link_libraries(wxPLViewer plplotwxwidgets plplotcxx ${wxwidgets_LINK_FLAGS} ${MATH_LIB} ${RT_LIB})

  if(USE_RPATH)
    # Assume that plplotwxwidgets and plplotcxx have the same install location as plplot
    set_target_properties(wxPLViewer
      PROPERTIES
      INSTALL_RPATH "${LIB_INSTALL_RPATH}"
      )
  endif(USE_RPATH)

  install(TARGETS wxPLViewer DESTINATION ${BIN_DIR})
endif(ENABLE_wxwidgets AND NOT OLD_WXWIDGETS)
