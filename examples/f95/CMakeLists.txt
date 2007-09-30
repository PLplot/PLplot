# examples/f95/CMakeLists.txt
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

set(f95_SRCS)

set(f95_STRING_INDICES
"01"
"02"
"03"
"04"
"05"
"06"
"07"
"08"
"09"
"10"
"11"
"12"
"13"
"14"
"15"
"16"
"16a"
"17"
"18"
"19"
"22"
"23"
"28"
)

if(BUILD_TEST)
  remove_definitions("-DHAVE_CONFIG_H")
  # N.B. This is required because it's the location of plplot.mod
  # generated as a by-product of building libplplotf95${LIB_TAG}.
  include_directories(${CMAKE_BINARY_DIR})

  # Temporary workaround for module handling bug in CMake 2.4.3.  Create a
  # file with arbitrary contents called plplot.mod.proxy in top-level
  # build tree.  As a temporary measure we use cmake_workaround.f as the
  # generator of this file.  (cmake_workaround.f was previously compiled,
  # but that is no longer the case.)  Note, once CMake is fixed this comment
  # the following configure_file command and cmake_workaround.f should all
  # just disappear.
  configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/cmake_workaround.f
  ${CMAKE_BINARY_DIR}/plplot.mod.proxy
  COPYONLY
  )
endif(BUILD_TEST)
foreach(STRING_INDEX ${f95_STRING_INDICES})
  set(f95_SRCS ${f95_SRCS} x${STRING_INDEX}f.f90)
  if(BUILD_TEST)
    add_executable(x${STRING_INDEX}f x${STRING_INDEX}f.f90)
    target_link_libraries(x${STRING_INDEX}f plplotf95${LIB_TAG})
  endif(BUILD_TEST)
endforeach(STRING_INDEX ${f95_STRING_INDICES})

install(FILES ${f95_SRCS} 
DESTINATION ${DATA_DIR}/examples/f95
)

set(FC ${CMAKE_Fortran_COMPILER})
configure_file(
${CMAKE_CURRENT_SOURCE_DIR}/Makefile.examples.in
${CMAKE_CURRENT_BINARY_DIR}/Makefile.examples
)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/Makefile.examples 
DESTINATION ${DATA_DIR}/examples/f95
RENAME Makefile
)
