# examples/ada/CMakeLists.txt
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

if(BUILD_TEST)

  add_custom_command(
  OUTPUT
  ${CMAKE_CURRENT_BINARY_DIR}/ada_plplot
  COMMAND
  ${CMAKE_COMMAND} -E copy_if_different
  ${CMAKE_CURRENT_SOURCE_DIR}/ada_plplot.adb
  ${CMAKE_CURRENT_BINARY_DIR}/ada_plplot.adb
  COMMAND
  ${CMAKE_COMMAND} -E copy_if_different
  ${CMAKE_CURRENT_SOURCE_DIR}/x01.adb
  ${CMAKE_CURRENT_BINARY_DIR}/x01.adb
  COMMAND
  ${CMAKE_COMMAND} -E copy_if_different
  ${CMAKE_CURRENT_SOURCE_DIR}/x01.ads
  ${CMAKE_CURRENT_BINARY_DIR}/x01.ads
  COMMAND
  ${GNATMAKE_EXECUTABLE}
  -I${CMAKE_BINARY_DIR}/bindings/ada
  ada_plplot
  -largs -L${CMAKE_BINARY_DIR}/bindings/ada -lplplotada${LIB_TAG}
  DEPENDS
  ${CMAKE_CURRENT_SOURCE_DIR}/ada_plplot.adb
  ${CMAKE_CURRENT_SOURCE_DIR}/x01.adb
  ${CMAKE_CURRENT_SOURCE_DIR}/x01.ads
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  )
  
  add_custom_target(ada_plplot_target ALL
  DEPENDS
  ${CMAKE_CURRENT_BINARY_DIR}/ada_plplot
  )
  
endif(BUILD_TEST)
