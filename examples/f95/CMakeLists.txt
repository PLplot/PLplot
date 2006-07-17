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

IF (ENABLE_f95)

set(f95_SRCS
x01f.f90 
x02f.f90 
x03f.f90 
x04f.f90 
x05f.f90 
x06f.f90 
x07f.f90 
x08f.f90 
x09f.f90 
x10f.f90 
x11f.f90 
x12f.f90 
x13f.f90 
x14f.f90 
x15f.f90 
x16f.f90 
x16af.f90 
x17f.f90 
x18f.f90 
x19f.f90 
x22f.f90 
x23f.f90 
)

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

ENDIF (ENABLE_f95)
