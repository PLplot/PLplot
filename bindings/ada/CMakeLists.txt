# bindings/ada/CMakeLists.txt
### Process this file with cmake to produce Makefile
###
# Copyright (C) 2007 Alan W. Irwin
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

if(ENABLE_ada)

  add_custom_command(
  OUTPUT
  ${CMAKE_CURRENT_BINARY_DIR}/libplplotada${LIB_TAG}.so
  COMMAND
  ${CMAKE_COMMAND} -E copy_if_different
  ${CMAKE_CURRENT_SOURCE_DIR}/plplot.adb
  ${CMAKE_CURRENT_BINARY_DIR}/plplot.adb
  COMMAND
  ${CMAKE_COMMAND} -E copy_if_different
  ${CMAKE_CURRENT_SOURCE_DIR}/plplot.ads
  ${CMAKE_CURRENT_BINARY_DIR}/plplot.ads
  COMMAND
  ${CMAKE_COMMAND} -E copy_if_different
  ${CMAKE_CURRENT_SOURCE_DIR}/plplotthin.adb
  ${CMAKE_CURRENT_BINARY_DIR}/plplotthin.adb
  COMMAND
  ${CMAKE_COMMAND} -E copy_if_different
  ${CMAKE_CURRENT_SOURCE_DIR}/plplotthin.ads
  ${CMAKE_CURRENT_BINARY_DIR}/plplotthin.ads
  COMMAND
  ${GNATMAKE_EXECUTABLE} -c -fPIC plplot.adb
  COMMAND
  gcc -fPIC -shared
  ${CMAKE_CURRENT_BINARY_DIR}/plplot.o
  ${CMAKE_CURRENT_BINARY_DIR}/plplotthin.o
  -Wl,-soname,libplplotada${LIB_TAG}.so.${plplotada_SOVERSION}
  -Wl,-rpath,${CMAKE_BINARY_DIR}/src
  -o libplplotada${LIB_TAG}.so.${plplotada_VERSION}
  -L${GNAT_DIR} -lgnat
  -L${CMAKE_BINARY_DIR}/src -lplplot${LIB_TAG}
  COMMAND
  ${CMAKE_COMMAND} -E cmake_symlink_library
  libplplotada${LIB_TAG}.so.${plplotada_VERSION}
  libplplotada${LIB_TAG}.so.${plplotada_SOVERSION}
  libplplotada${LIB_TAG}.so
#  COMMAND
#  rm -f 
#  ${CMAKE_CURRENT_BINARY_DIR}/plplot.o
#  ${CMAKE_CURRENT_BINARY_DIR}/plplotthin.o
  DEPENDS
  ${CMAKE_CURRENT_SOURCE_DIR}/plplot.adb
  ${CMAKE_CURRENT_SOURCE_DIR}/plplotthin.adb
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  )
  
  add_custom_target(plplotada${LIB_TAG} ALL
  DEPENDS
  ${CMAKE_CURRENT_BINARY_DIR}/libplplotada${LIB_TAG}.so
  )
  
#  set(plplotthinada{LIB_TAG}_LIB_SRCS
#  ${CMAKE_CURRENT_BINARY_DIR}/plplotthin.o
#  )

#  add_library(plplotthinada${LIB_TAG} ${plplotthinada${LIB_TAG}_LIB_SRCS})

#  target_link_libraries(plplotthinada${LIB_TAG}  plplot${LIB_TAG})

#  get_target_property(LIB_INSTALL_RPATH plplot${LIB_TAG} INSTALL_RPATH)
#  set_target_properties(plplotthinada${LIB_TAG}
#  PROPERTIES
#  SOVERSION ${plplotthinada_SOVERSION}
#  VERSION ${plplotthinada_VERSION}
#  INSTALL_RPATH "${LIB_INSTALL_RPATH}"
#  INSTALL_NAME_DIR "${LIB_DIR}"
#  )

#  install(TARGETS plplotthinada${LIB_TAG}
#  ARCHIVE DESTINATION ${LIB_DIR}
#  LIBRARY DESTINATION ${LIB_DIR}
#  RUNTIME DESTINATION ${BIN_DIR}
#  )

endif(ENABLE_ada)
