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

AM_CPPFLAGS = @INCLTDL@
examples_cdir = ${prefix}/@DATA_DIR@/../examples/c
examples_c_DATA = \
	Makefile.examples \
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

if with_double
libplplot = ../../src/libplplotd.la
else
libplplot = ../../src/libplplot.la
endif

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

x01c_LDADD = $(libplplot)
x02c_LDADD = $(libplplot)
x03c_LDADD = $(libplplot)
x04c_LDADD = $(libplplot)
x05c_LDADD = $(libplplot)
x06c_LDADD = $(libplplot)
x07c_LDADD = $(libplplot)
x08c_LDADD = $(libplplot)
x09c_LDADD = $(libplplot)
x10c_LDADD = $(libplplot)
x11c_LDADD = $(libplplot)
x12c_LDADD = $(libplplot)
x13c_LDADD = $(libplplot)
x14c_LDADD = $(libplplot)
x15c_LDADD = $(libplplot)
x16c_LDADD = $(libplplot)
x17c_LDADD = $(libplplot)
x18c_LDADD = $(libplplot)
x19c_LDADD = $(libplplot)
x20c_LDADD = $(libplplot)

EXTRA_DIST = $(examples_c_DATA) Makefile.examples.in
DISTCLEANFILES = Makefile.examples
