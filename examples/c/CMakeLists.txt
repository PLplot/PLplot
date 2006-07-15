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

set(c_SRCS
plcdemos.h
tutor.c
x01c.c
x02c.c
x03c.c
x04c.c
x05c.c
x06c.c
x07c.c
x08c.c
x09c.c
x10c.c
x11c.c
x12c.c
x13c.c
x14c.c
x15c.c
x16c.c
x17c.c
x18c.c
x19c.c
x20c.c
x21c.c
x22c.c
x23c.c
x24c.c
x25c.c
x26c.c
)

set(plplotcanvas_SRCS
plplotcanvas_demo.c
plplotcanvas_animation.c
)

set(plplotcanvas_DOCS
README.plplotcanvas
)

install(FILES ${c_SRCS} ${plplotcanvas_SRCS} ${plplotcanvas_DOCS}
DESTINATION ${DATA_DIR}/examples/c
)

set(CC ${CMAKE_C_COMPILER})
configure_file(
${CMAKE_CURRENT_SOURCE_DIR}/Makefile.examples.in
${CMAKE_CURRENT_BINARY_DIR}/Makefile.examples
)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/Makefile.examples 
DESTINATION ${DATA_DIR}/examples/c
RENAME Makefile
)
