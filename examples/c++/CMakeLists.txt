# examples/c/CMakeLists.txt
### Process this file with cmake to produce Makefile
###
# Copyright (C) 2006 Andrew Ross
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

set(cxx_SRCS
plc++demos.h
x01cc.cc
x01.cc
x02.cc
x03.cc
x04.cc
x05.cc
x06.cc
x07.cc
x08.cc
x09.cc
x10.cc
x11.cc
x12.cc
x13.cc
x14.cc
x15.cc
x16.cc
x17.cc
x18.cc
x19.cc
x20.cc
x21.cc
x22.cc
x23.cc
x24.cc
x26.cc
)

install(FILES ${cxx_SRCS} DESTINATION ${DATA_DIR}/examples/c++)

set(CXX ${CMAKE_CXX_COMPILER})
configure_file(
${CMAKE_CURRENT_SOURCE_DIR}/Makefile.examples.in
${CMAKE_CURRENT_BINARY_DIR}/Makefile.examples
)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/Makefile.examples 
DESTINATION ${DATA_DIR}/examples/c++
RENAME Makefile
)
