# examples/c/Makefile.am for PLplot
###
### Process this file with automake to produce Makefile.in
###
# Copyright (C) 2002 Alan W. Irwin
# 
#This file is part of PLplot.
#
#This file is free software; you can redistribute it and/or modify
#it under the terms of the GNU Library General Public License as published by
#the Free Software Foundation; version 2 of the License.
#
#This file is distributed in the hope that it will be useful,
#but WITHOUT ANY WARRANTY; without even the implied warranty of
#MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#GNU Library General Public License for more details.
#
#You should have received a copy of the GNU Library General Public License
#along with the file; if not, write to the Free Software
#Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307  USA

AM_CPPFLAGS = $(INCLTDL)
examples_cdir = ${prefix}/$(DATA_DIR)/../examples/c
examples_c_DATA = \
	.samename/Makefile \
	plcdemos.h \
	lena.pgm \
	tutor.c \
	x01c.c \
	x02c.c \
	x03c.c \
	x04c.c \
	x05c.c \
	x06c.c \
	x07c.c \
	x08c.c \
	x09c.c \
	x10c.c \
	x11c.c \
	x12c.c \
	x13c.c \
	x14c.c \
	x15c.c \
	x16c.c \
	x17c.c \
	x18c.c \
	x19c.c \
	x20c.c

.samename/Makefile: Makefile.examples
	mkdir -p .samename
	cp Makefile.examples .samename/Makefile

libplplot = ../../src/libplplot$(LIB_TAG).la

check_PROGRAMS = \
	x01c \
	x02c \
	x03c \
	x04c \
	x05c \
	x06c \
	x07c \
	x08c \
	x09c \
	x10c \
	x11c \
	x12c \
	x13c \
	x14c \
	x15c \
	x16c \
	x17c \
	x18c \
	x19c \
	x20c

x01c_LDFLAGS = $(libplplot)
x02c_LDFLAGS = $(libplplot)
x03c_LDFLAGS = $(libplplot)
x04c_LDFLAGS = $(libplplot)
x05c_LDFLAGS = $(libplplot)
x06c_LDFLAGS = $(libplplot)
x07c_LDFLAGS = $(libplplot)
x08c_LDFLAGS = $(libplplot)
x09c_LDFLAGS = $(libplplot)
x10c_LDFLAGS = $(libplplot)
x11c_LDFLAGS = $(libplplot)
x12c_LDFLAGS = $(libplplot)
x13c_LDFLAGS = $(libplplot)
x14c_LDFLAGS = $(libplplot)
x15c_LDFLAGS = $(libplplot)
x16c_LDFLAGS = $(libplplot)
x17c_LDFLAGS = $(libplplot)
x18c_LDFLAGS = $(libplplot)
x19c_LDFLAGS = $(libplplot)
x20c_LDFLAGS = $(libplplot)

EXTRA_DIST = $(examples_c_DATA) Makefile.examples.in
DISTCLEANFILES = Makefile.examples .samename/Makefile
