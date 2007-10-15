# bindings/octave/demos/CMakeLists.txt for PLplot
###
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

# Install octave examples.
set(examples_octave
p1.m
p10.m
p11.m
p12.m
p13.m
p14.m
p15.m
p16.m
p17.m
p18.m
p19.m
p2.m
p20.m
p21.m
p3.m
p4.m
p5.m
p6.m
p7.m
p8.m
p9.m
plplot_octave_demo.m
x01c.m
x02c.m
x03c.m
x04c.m
x05c.m
x06c.m
x07c.m
x08c.m
x09c.m
x10c.m
x11c.m
x12c.m
x13c.m
x14c.m
x15c.m
x16c.m
x17c.m
x18c.m
x19c.m
x20c.m
x21c.m
x22c.m
x23c.m
x24c.m
x25c.m
x26c.m
x28c.m
lena.img
)

install(FILES ${examples_octave} DESTINATION ${DATA_DIR}/examples/octave)

install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/lena.img
DESTINATION ${DATA_DIR}/examples
)
