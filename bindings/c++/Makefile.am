# bindings/c++/Makefile.am for PLplot
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
libplplot = -L../../src/ -lplplotd
else
libplplot = -L../../src/ -lplplot
endif

if enable_cxx

pkginclude_HEADERS = plstream.h

if with_double
lib_LTLIBRARIES = libplplotcxxd.la
else
lib_LTLIBRARIES = libplplotcxx.la
endif

endif

AM_CPPFLAGS = @INCLTDL@
version = -version-info @SOVERSION@

sources = plstream.cc

libplplotcxx_la_SOURCES  = $(sources)
libplplotcxx_la_LDFLAGS  = $(version) -rpath $(libdir) -no-undefined $(libplplot)

libplplotcxxd_la_SOURCES = $(libplplotcxx_la_SOURCES)
libplplotcxxd_la_LDFLAGS = $(libplplotcxx_la_LDFLAGS)

