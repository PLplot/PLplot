# examples/f77/CMakeLists.txt
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
"28"
)

foreach(STRING_INDEX ${f77_STRING_INDICES})
  set(f77_SRCS ${f77_SRCS} ${CMAKE_CURRENT_BINARY_DIR}/x${STRING_INDEX}f.f)
endforeach(STRING_INDEX ${c_STRING_INDICES})

# Process .fm4 files to set PLFLT to real / double.
if(PL_DOUBLE)
  foreach(STRING_INDEX ${f77_STRING_INDICES})
    add_custom_command(
      OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/x${STRING_INDEX}f.f
      COMMAND ${CMAKE_COMMAND} 
      -E copy ${CMAKE_CURRENT_SOURCE_DIR}/x${STRING_INDEX}f.fm4 ${CMAKE_CURRENT_BINARY_DIR}/x${STRING_INDEX}f.f
      DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/x${STRING_INDEX}f.fm4
    )
  endforeach(STRING_INDEX ${f77_STRING_INDICES})
else(PL_DOUBLE)
  foreach(STRING_INDEX ${f77_STRING_INDICES})
    add_custom_command(
      OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/x${STRING_INDEX}f.f
      COMMAND sed
      -f ${CMAKE_CURRENT_SOURCE_DIR}/double2single.sed ${CMAKE_CURRENT_SOURCE_DIR}/x${STRING_INDEX}f.fm4 > ${CMAKE_CURRENT_BINARY_DIR}/x${STRING_INDEX}f.f
      DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/x${STRING_INDEX}f.fm4
    )
  endforeach(STRING_INDEX ${f77_STRING_INDICES})
endif(PL_DOUBLE)

if(BUILD_TEST)
  remove_definitions("-DHAVE_CONFIG_H")
  include_directories(${CMAKE_SOURCE_DIR}/include ${CMAKE_BINARY_DIR}/include)
endif(BUILD_TEST)
foreach(STRING_INDEX ${f77_STRING_INDICES})
  if(BUILD_TEST)
    add_executable(x${STRING_INDEX}f ${CMAKE_CURRENT_BINARY_DIR}/x${STRING_INDEX}f.f)
    target_link_libraries(x${STRING_INDEX}f plplotf77${LIB_TAG})
  endif(BUILD_TEST)
endforeach(STRING_INDEX ${f77_STRING_INDICES})

add_custom_target(f77_examples ALL DEPENDS ${f77_SRCS})

install(FILES ${f77_SRCS} 
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
