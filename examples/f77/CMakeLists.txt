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

IF (ENABLE_f77)

set(f77_SRCS
x01f.f 
x02f.f 
x03f.f
x04f.f
x05f.f
x06f.f
x07f.f
x08f.f
x09f.f
x10f.f
x11f.f
x12f.f
x13f.f
x14f.f
x15f.f
x16f.f
x16af.f
x17f.f
x18f.f
x19f.f
x22f.f
x23f.f
)

IF(PL_DOUBLE)
  FOREACH(srcfile ${f77_SRCS})
    ADD_CUSTOM_COMMAND(
      OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${srcfile}
      COMMAND ${CMAKE_COMMAND} 
      ARGS -E copy ${CMAKE_CURRENT_SOURCE_DIR}/${srcfile}m4 ${CMAKE_CURRENT_BINARY_DIR}/${srcfile}
      DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/${srcfile}m4
    )
  ENDFOREACH(srcfile ${f77_SRCS})
ELSE(PL_DOUBLE)
  FOREACH(srcfile ${f77_SRCS})
    ADD_CUSTOM_COMMAND(
      OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${srcfile}
      COMMAND sed
      ARGS -f ${CMAKE_CURRENT_SOURCE_DIR}/double2single.sed ${CMAKE_CURRENT_SOURCE_DIR}/${srcfile}m4 > ${CMAKE_CURRENT_BINARY_DIR}/${srcfile}
      DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/${srcfile}m4
    )
  ENDFOREACH(srcfile ${f77_SRCS})
ENDIF(PL_DOUBLE)

# List of generated fortran files with full path names
# Need this otherwise cmake will look in the source directory for the
# .f files.
STRING( REGEX REPLACE "(x[A-za-z0-9]*)\\.f" "${CMAKE_CURRENT_BINARY_DIR}/\\1.f" f77_full_SRCS "${f77_SRCS}" )

ADD_CUSTOM_TARGET(f77_examples ALL DEPENDS ${f77_full_SRCS})

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

ENDIF (ENABLE_f77)
