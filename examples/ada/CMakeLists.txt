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

  # N.B. The following special RPATH_SPEC logic and custom command and custom
  # target are all required because add_executable only compiles (and does not
  # gnatbind or gnatlink).  The reason for this limited Ada language support
  # for add_executable is <SOURCE> is not currently available in the 
  # link_executable context of CMake.

  if(CMAKE_SHARED_LIBRARY_RUNTIME_Ada_FLAG AND
  CMAKE_SHARED_LIBRARY_RUNTIME_Ada_FLAG_SEP
  )
    set(RPATH_SPEC
    "${CMAKE_SHARED_LIBRARY_RUNTIME_Ada_FLAG}${CMAKE_BINARY_DIR}/src${CMAKE_SHARED_LIBRARY_RUNTIME_Ada_FLAG_SEP}${CMAKE_BINARY_DIR}/bindings/ada"
    )
  endif(CMAKE_SHARED_LIBRARY_RUNTIME_Ada_FLAG AND
  CMAKE_SHARED_LIBRARY_RUNTIME_Ada_FLAG_SEP
  )

  # -aI must point to location of libplplotata(d) spec (*.ads) files.
  # -aL must point to location of libplplotada(d) *.ali files.
  # N.B. that latter location works for cmake-2.4.6 and will presumably
  # be fine for quite a while, but that location is not guaranteed
  # indefinitely for all future versions of cmake.
  add_custom_command(
  OUTPUT
  ${CMAKE_CURRENT_BINARY_DIR}/ada_plplot
  COMMAND
  ${GNATMAKE_EXECUTABLE}
  -aI${CMAKE_SOURCE_DIR}/bindings/ada
  -aL${CMAKE_BINARY_DIR}/bindings/ada/CMakeFiles/plplotada${LIB_TAG}.dir
  ${CMAKE_CURRENT_SOURCE_DIR}/ada_plplot.adb
  -largs -L${CMAKE_BINARY_DIR}/bindings/ada -lplplotada${LIB_TAG}
  ${RPATH_SPEC}
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
  add_dependencies(ada_plplot_target plplotada${LIB_TAG})
  
endif(BUILD_TEST)
