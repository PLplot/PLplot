# examples/c++/Makefile.am for PLplot
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

if enable_cxx

AM_CPPFLAGS = $(INCLTDL)
examples_cxxdir = ${prefix}/$(DATA_DIR)/../examples/c++
examples_cxx_DATA = \
	.samename/Makefile \
	x01cc.cc

.samename/Makefile: Makefile.examples
	mkdir -p .samename
	cp Makefile.examples .samename/Makefile

if with_double
libplplotcxx = -L../../bindings/c++ -lplplotcxxd
else
libplplotcxx = -L../../bindings/c++ -lplplotcxx
endif

check_PROGRAMS = \
	x01cc

x01cc_CXXFLAGS = -I../../bindings/c++
x01cc_SOURCES = x01cc.cc
x01cc_LDFLAGS = $(libplplotcxx)

EXTRA_DIST = $(examples_cxx_DATA) Makefile.examples.in
DISTCLEANFILES = Makefile.examples .samename/Makefile

# enable_cxx
endif
