# examples/f77/Makefile.am for PLplot
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

if enable_f77

AM_CPPFLAGS = $(INCLTDL)
examples_f77dir = ${prefix}/$(DATA_DIR)/../examples/f77
f77m4_source = \
	fmacs.m4 \
	x01f.fm4 \
	x02f.fm4 \
	x03f.fm4 \
	x04f.fm4 \
	x05f.fm4 \
	x06f.fm4 \
	x07f.fm4 \
	x08f.fm4 \
	x09f.fm4 \
	x10f.fm4 \
	x11f.fm4 \
	x12f.fm4 \
	x13f.fm4 \
	x16f.fm4

examples_f77_DATA = \
	.samename/Makefile \
	x01f.f \
	x02f.f \
	x03f.f \
	x04f.f \
	x05f.f \
	x06f.f \
	x07f.f \
	x08f.f \
	x09f.f \
	x10f.f \
	x11f.f \
	x12f.f \
	x13f.f \
	x16f.f

.samename/Makefile: Makefile.examples
	mkdir -p .samename
	cp Makefile.examples .samename/Makefile

# Since these are all generated....
CLEANFILES = $(examples_f77_DATA)
# Need explicit dependencies for Solaris make.
x01f.f: x01f.fm4 fmacs.m4

x02f.f: x02f.fm4 fmacs.m4

x03f.f: x03f.fm4 fmacs.m4

x04f.f: x04f.fm4 fmacs.m4

x05f.f: x05f.fm4 fmacs.m4

x06f.f: x06f.fm4 fmacs.m4

x07f.f: x07f.fm4 fmacs.m4

x08f.f: x08f.fm4 fmacs.m4

x09f.f: x09f.fm4 fmacs.m4

x10f.f: x10f.fm4 fmacs.m4

x11f.f: x11f.fm4 fmacs.m4

x12f.f: x12f.fm4 fmacs.m4

x13f.f: x13f.fm4 fmacs.m4

x16f.f: x16f.fm4 fmacs.m4

SUFFIXES = .fm4

.fm4.f:
	$(M4) $(M4_FLAGS) $< >$*.f

if with_double
libplplotf77 = -L../../bindings/f77 -lplplotf77d
else
libplplotf77 = -L../../bindings/f77 -lplplotf77
endif

check_PROGRAMS = \
	x01f \
	x02f \
	x03f \
	x04f \
	x05f \
	x06f \
	x07f \
	x08f \
	x09f \
	x10f \
	x11f \
	x12f \
	x13f \
	x16f

x01f_SOURCES = x01f.f
x02f_SOURCES = x02f.f
x03f_SOURCES = x03f.f
x04f_SOURCES = x04f.f
x05f_SOURCES = x05f.f
x06f_SOURCES = x06f.f
x07f_SOURCES = x07f.f
x08f_SOURCES = x08f.f
x09f_SOURCES = x09f.f
x10f_SOURCES = x10f.f
x11f_SOURCES = x11f.f
x12f_SOURCES = x12f.f
x13f_SOURCES = x13f.f
x16f_SOURCES = x16f.f

x01f_LDFLAGS = $(libplplotf77)
x02f_LDFLAGS = $(libplplotf77)
x03f_LDFLAGS = $(libplplotf77)
x04f_LDFLAGS = $(libplplotf77)
x05f_LDFLAGS = $(libplplotf77)
x06f_LDFLAGS = $(libplplotf77)
x07f_LDFLAGS = $(libplplotf77)
x08f_LDFLAGS = $(libplplotf77)
x09f_LDFLAGS = $(libplplotf77)
x10f_LDFLAGS = $(libplplotf77)
x11f_LDFLAGS = $(libplplotf77)
x12f_LDFLAGS = $(libplplotf77)
x13f_LDFLAGS = $(libplplotf77)
x16f_LDFLAGS = $(libplplotf77)

EXTRA_DIST = $(f77m4_source) Makefile.examples.in
DISTCLEANFILES = Makefile.examples .samename/Makefile

# enable_f77
endif
