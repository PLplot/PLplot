# bindings/f77/Makefile.am for PLplot
###
### Process this file with automake to produce Makefile.in
###
# Copyright (C) 2002, 2003  Rafael Laboissiere
# Copyright (C) 2002, 2004  Alan W. Irwin
#
# This file is part of PLplot.
#
# This file is free software; you can redistribute it and/or modify it
# under the terms of the GNU Library General Public License as published by
# the Free Software Foundation; version 2 of the License.
#
# This file is distributed in the hope that it will be useful, but WITHOUT
# ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
# FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Library General Public
# License for more details.
#
# You should have received a copy of the GNU Library General Public License
# along with the file; if not, write to the Free Software Foundation, Inc.,
# 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA

libplplot = $(top_builddir)/src/libplplot$(LIB_TAG).la

if enable_f77
lib_LTLIBRARIES = libplplotf77@LIB_TAG@.la
endif

AM_CPPFLAGS = $(INCLTDL)
version = -version-info $(SOVERSION)

sources = \
	plstubs.h \
	sc3d.c \
	sccont.c \
	scstubs.c \
	strutil.f \
	sfstubs.h \
	sfstubs.f \
	configurable.f

libplplotf77@LIB_TAG@_la_SOURCES  = $(sources)
libplplotf77@LIB_TAG@_la_LDFLAGS  = $(version) $(RPATH) -no-undefined $(libplplot)
libplplotf77@LIB_TAG@_la_LIBADD  = $(FLIBS)

# Must be explicit here with dependencies.  Otherwise solaris "make" chokes.
sfstubs.f: sfstubs.h

# cp for the double-precision case, convert with sed for the 
# single-precision case.
if with_double
.fm4.f:
	cp $< $*.f
else
.fm4.f:
	sed -f double2single.sed $< > $*.f
endif

EXTRA_DIST = sfstubs.fm4 double2single.sed README.f77API
CLEANFILES = sfstubs.f

SUFFIXES = .fm4
