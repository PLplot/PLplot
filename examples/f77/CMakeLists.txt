# examples/c/CMakeLists.txt
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

set(f77_SRCS)

set(f77_STRING_INDICES
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

foreach(STRING_INDEX ${f77_STRING_INDICES})
  set(f77_SRCS ${f77_SRCS} x${STRING_INDEX}f.f)
endforeach(STRING_INDEX ${c_STRING_INDICES})

if(PL_DOUBLE)
  foreach(srcfile ${f77_SRCS})
    add_custom_command(
      OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${srcfile}
      COMMAND ${CMAKE_COMMAND} 
      ARGS -E copy ${CMAKE_CURRENT_SOURCE_DIR}/${srcfile}m4 ${CMAKE_CURRENT_BINARY_DIR}/${srcfile}
      DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/${srcfile}m4
    )
  endforeach(srcfile ${f77_SRCS})
else(PL_DOUBLE)
  foreach(srcfile ${f77_SRCS})
    add_custom_command(
      OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${srcfile}
      COMMAND sed
      ARGS -f ${CMAKE_CURRENT_SOURCE_DIR}/double2single.sed ${CMAKE_CURRENT_SOURCE_DIR}/${srcfile}m4 > ${CMAKE_CURRENT_BINARY_DIR}/${srcfile}
      DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/${srcfile}m4
    )
  endforeach(srcfile ${f77_SRCS})
endif(PL_DOUBLE)

#set(ENABLE_BUILDTREE_EXAMPLES ON)
if(ENABLE_BUILDTREE_EXAMPLES)
  remove_definitions("-DHAVE_CONFIG_H")
  include_directories(${CMAKE_SOURCE_DIR}/include ${CMAKE_BINARY_DIR}/include)
endif(ENABLE_BUILDTREE_EXAMPLES)
foreach(STRING_INDEX ${f77_STRING_INDICES})
  if(ENABLE_BUILDTREE_EXAMPLES)
    add_executable(x${STRING_INDEX}f ${CMAKE_CURRENT_BINARY_DIR}/x${STRING_INDEX}f.f)
    target_link_libraries(x${STRING_INDEX}f plplotf77${LIB_TAG} m)
  endif(ENABLE_BUILDTREE_EXAMPLES)
endforeach(STRING_INDEX ${f77_STRING_INDICES})

# List of generated fortran files with full path names
# Need this otherwise cmake will look in the source directory for the
# .f files.
string( REGEX REPLACE "(x[A-za-z0-9]*)\\.f" "${CMAKE_CURRENT_BINARY_DIR}/\\1.f" f77_full_SRCS "${f77_SRCS}" )

add_custom_target(f77_examples ALL DEPENDS ${f77_full_SRCS})

install(FILES ${f77_full_SRCS} 
DESTINATION ${DATA_DIR}/examples/f77
)

set(F77 ${CMAKE_Fortran_COMPILER})
configure_file(
${CMAKE_CURRENT_SOURCE_DIR}/Makefile.examples.in
${CMAKE_CURRENT_BINARY_DIR}/Makefile.examples
)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/Makefile.examples 
DESTINATION ${DATA_DIR}/examples/f77
RENAME Makefile
)
