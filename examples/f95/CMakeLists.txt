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
)

#set(ENABLE_BUILDTREE_EXAMPLES ON)
if(BUILD_TEST AND ENABLE_BUILDTREE_EXAMPLES)
  remove_definitions("-DHAVE_CONFIG_H")
  include_directories(${CMAKE_SOURCE_DIR}/include ${CMAKE_BINARY_DIR}/include)
endif(BUILD_TEST AND ENABLE_BUILDTREE_EXAMPLES)
foreach(STRING_INDEX ${f95_STRING_INDICES})
  set(f95_SRCS ${f95_SRCS} x${STRING_INDEX}f.f90)
  if(BUILD_TEST AND ENABLE_BUILDTREE_EXAMPLES)
    add_executable(x${STRING_INDEX}f x${STRING_INDEX}f.f90)
    target_link_libraries(x${STRING_INDEX}f plplotf95${LIB_TAG})
  endif(BUILD_TEST AND ENABLE_BUILDTREE_EXAMPLES)
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
