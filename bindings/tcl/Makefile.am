# bindings/c++/Makefile.am for PLplot
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

if with_double
  libplplot = ../../src/libplplotd.la
  libmatrix = libtclmatrixd.la
else
  libplplot = ../../src/libplplot.la
  libmatrix = libtclmatrix.la
endif

if enable_tcl
if with_double
    lib_LTLIBRARIES = libtclmatrixd.la libplplottcld.la 
else
    lib_LTLIBRARIES = libtclmatrix.la libplplottcl.la
endif
endif

pkginclude_HEADERS = pltcl.h tclMatrix.h

AM_CPPFLAGS = @TCLINCCMD@ @INCLTDL@
version = -version-info @SOVERSION@

libtclmatrix_la_SOURCES  = tclMatrix.c matrixInit.c
libtclmatrix_la_LDFLAGS  = $(version) -rpath $(libdir) -no-undefined @TCLLIBCMD@
libtclmatrix_la_LIBADD = $(libplplot)

libplplottcl_la_SOURCES  = \
	tclAPI.c \
	tclMain.c

tclAPI.c: tclgen.h tclgen_s.h tclgen.c

tclgen.h tclgen_s.h tclgen.c : pltclgen plapi.tpl tclcmd.tpl
	perl pltclgen

EXTRA_DIST = pltclgen plapi.tpl tclcmd.tpl
CLEANFILES = tclgen.h tclgen_s.h tclgen.c

libplplottcl_la_LDFLAGS  = $(version) -rpath $(libdir) -no-undefined @TCLLIBCMD@
libplplottcl_la_LIBADD = $(libplplot) $(libmatrix)

libtclmatrixd_la_SOURCES = $(libtclmatrix_la_SOURCES)
libtclmatrixd_la_LDFLAGS = $(libtclmatrix_la_LDFLAGS)
libtclmatrixd_la_LIBADD  = $(libtclmatrix_la_LIBADD)

libplplottcld_la_SOURCES = $(libplplottcl_la_SOURCES)
libplplottcld_la_LDFLAGS = $(libplplottcl_la_LDFLAGS)
libplplottcld_la_LIBADD  = $(libplplottcl_la_LIBADD)
