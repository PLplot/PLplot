# bindings/f77/Makefile.am for PLplot
###
### Process this file with automake to produce Makefile.in
###
# Copyright (C) 2002 Rafael Laboissiere
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

if with_double
libplplot = -L../../src -lplplotd
else
libplplot = -L../../src -lplplot
endif

if enable_f77
if with_double
lib_LTLIBRARIES = libplplotf77d.la
else
lib_LTLIBRARIES = libplplotf77.la
endif
endif

AM_CPPFLAGS = $(INCLTDL)
version = -version-info $(SOVERSION)

sources = \
	plstubs.h \
	sc3d.c \
	sccont.c \
	scstubs.c \
	strutil.f \
	sfstubs.f

libplplotf77_la_SOURCES  = $(sources)
libplplotf77_la_LDFLAGS  = $(version) -rpath $(libdir) -no-undefined $(libplplot)

libplplotf77d_la_SOURCES = $(libplplotf77_la_SOURCES)
libplplotf77d_la_LDFLAGS = $(libplplotf77_la_LDFLAGS)

EXTRA_DIST = fmacs.m4 sfstubs.fm4

# Must be explicit here with dependencies.  Otherwise solaris "make" chokes.
sfstubs.f: fmacs.m4 sfstubs.fm4

.fm4.f:
	$(M4) $(M4_FLAGS) $< >$*.f

CLEANFILES = sfstubs.f
SUFFIXES = .fm4
