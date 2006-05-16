# bindings/f95/Makefile.am for PLplot
###
### Process this file with automake to produce Makefile.in
###
# Copyright (C) 2002, 2003, 2004  Rafael Laboissiere
# Copyright (C) 2002, 2004  Alan W. Irwin
#
# This file is part of PLplot.
#
# PLplot is free software; you can redistribute it and/or modify it
# under the terms of the GNU Library General Public License as published by
# the Free Software Foundation; version 2 of the License.
#
# PLplot is distributed in the hope that it will be useful, but WITHOUT
# ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
# FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Library General Public
# License for more details.
#
# You should have received a copy of the GNU Library General Public License
# along with the file PLplot; if not, write to the Free Software Foundation, Inc.,
# 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA

libplplot = $(top_builddir)/src/libplplot$(LIB_TAG).la

if enable_f95
# order is important here since libplplotf95c@LIB_TAG@.la must be
# created first (since libplplotf95@LIB_TAG@.la depends upon it).
lib_LTLIBRARIES = libplplotf95c@LIB_TAG@.la libplplotf95@LIB_TAG@.la
pkginclude_HEADERS = \
	plplot.mod \
	plplotp.mod \
	plplot_flt.mod

endif

AM_CPPFLAGS = -I$(top_srcdir)/include $(INCLTDL)
AM_FCFLAGS = -I$(srcdir)

version = -version-info $(F95_SOVERSION)

# Keep Fortran and C libraries completely separate so libplplot will know how
# to link them without relying on poorly known (for most platforms)
# FLIBS information detailing how to combine C and fortran source in the
# same library.

fortran_sources = \
	strutil.f90 \
	configurable.f90 \
	sfstubsf95.f90 \
	sfstubs.h

generated_fortran_sources = plflt.inc

c_sources = \
	plstubs.h \
	sc3d.c \
	sccont.c \
	scstubs.c

# This creates the fortran source part of the fortran interface to libplplot.
# It wraps calls to libplplotf95c@LIB_TAG@.la
libplplotf95@LIB_TAG@_la_SOURCES = $(fortran_sources)
nodist_libplplotf95@LIB_TAG@_la_SOURCES = $(generated_fortran_sources)
libplplotf95@LIB_TAG@_la_LDFLAGS  = $(version) $(RPATH) -no-undefined libplplotf95c@LIB_TAG@.la

# This creates the C source part of the fortran interface to libplplot.
# libplplotf95@LIB_TAG@.la wraps libplplotf95c@LIB_TAG@.la which wraps
# libplplot.  Thus, these two libraries together provide
# a fortran wrapper for the "common" API part of libplplot.
libplplotf95c@LIB_TAG@_la_SOURCES  = $(c_sources)
libplplotf95c@LIB_TAG@_la_LDFLAGS  = $(version) $(RPATH) -no-undefined $(libplplot)

# Must be explicit here with dependencies.  Otherwise solaris "make" chokes.
sfstubsf95.f90: plflt.inc sfstubs.f90 sfstubs.h

EXTRA_DIST = readme_f95.txt sfstubs.f90

BUILT_SOURCES = $(generated_fortran_sources)
MAINTAINERCLEANFILES = $(generated_fortran_sources)

noinst_PROGRAMS = plflt
plflt_SOURCES = plflt.c

plflt.inc: plflt
	./plflt
