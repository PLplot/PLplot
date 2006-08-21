# utils/CMakeLists.txt for PLplot
###
### Process this file with cmake to produce Makefile
###
# Copyright (C) 2006 Alan W. Irwin
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
${CMAKE_BINARY_DIR}
${CMAKE_BINARY_DIR}/include
)

add_executable(plrender plrender.c)

if(WIN32 AND BUILD_SHARED_LIBS)
  SET_SOURCE_FILES_PROPERTIES(plrender.c
      PROPERTIES COMPILE_FLAGS "-DUSINGPLDLL" )
endif(WIN32 AND BUILD_SHARED_LIBS)
    
target_link_libraries(plrender plplot${LIB_TAG})

set(LIB_INSTALL_RPATH ${LIB_DIR})
if(ENABLE_DYNDRIVERS)
  set(LIB_INSTALL_RPATH ${LIB_INSTALL_RPATH} ${libplplot${LIB_TAG}_RPATH})
endif(ENABLE_DYNDRIVERS)

set_target_properties(plrender
PROPERTIES
INSTALL_RPATH "${LIB_INSTALL_RPATH}"
)

install(TARGETS plrender DESTINATION ${BIN_DIR})

add_executable(pltek pltek.c)

if(WIN32 AND BUILD_SHARED_LIBS)
  SET_SOURCE_FILES_PROPERTIES(pltek.c
      PROPERTIES COMPILE_FLAGS "-DUSINGPLDLL" )
endif(WIN32 AND BUILD_SHARED_LIBS)

target_link_libraries(pltek plplot${LIB_TAG})

set_target_properties(pltek
PROPERTIES
INSTALL_RPATH "${LIB_INSTALL_RPATH}"
)

install(TARGETS pltek DESTINATION ${BIN_DIR})

if(ENABLE_tcl)
  include_directories(
  ${CMAKE_SOURCE_DIR}/bindings/tcl
  ${TCL_INCLUDE_PATH}
  )

  add_executable(pltcl pltcl.c)

  target_link_libraries(pltcl plplot${LIB_TAG} plplottcltk${LIB_TAG})

  set_target_properties(pltcl
  PROPERTIES
  INSTALL_RPATH "${LIB_INSTALL_RPATH}"
  )

  install(TARGETS pltcl DESTINATION ${BIN_DIR})
endif(ENABLE_tcl)
