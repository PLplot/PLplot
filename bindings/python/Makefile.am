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

# N.B.  This treatment probably will need changes or else a complete
# replacement to work cross-platform as indicated by ?cross-platform?
# comment below, but it works quite nicely on Linux.

if enable_python

ourpythondir = ${prefix}/@PYTHON_INSTDIR@

if with_double
libplplot = -L../../src -lplplotd

### Only swig version 1.3.11 through 1.3.13 will work here because of a
### swig API change for 1.3.14 which we have not adjusted to yet.
### Ordinarily this detail only matters for CVS users because the tarball
### version of PLplot always has both plplotcmodule_p_double.c and
### plplotcmodule_p_single.c pre-generated.
plplotcmodule_p_double.c: plplotcmodule.i 
	swig -python -o plplotcmodule_p_double.c -c++ -DPL_DOUBLE \
	plplotcmodule.i

plplotcmodule_p.c: plplotcmodule_p_double.c
	cp plplotcmodule_p_double.c plplotcmodule_p.c
else
libplplot = -L../../src -lplplot
plplotcmodule_p_single.c: plplotcmodule.i 
	swig -python -o plplotcmodule_p_single.c -c++ \
	plplotcmodule.i

plplotcmodule_p.c: plplotcmodule_p_single.c
	cp plplotcmodule_p_single.c plplotcmodule_p.c
endif

# swig-1.3.11+ does not handle documentation strings properly so run this
# script to make documentation strings.
plplotcmodule.c: plplotcmodule_p.c makedocstrings.py
	@PYTHON@ makedocstrings.py plplotcmodule_p.c plplotcmodule.c

ourpython_DATA = plplot.py
ourpython_LTLIBRARIES = plplotcmodule.la plplot_widgetmodule.la


# PYTHON_NUM_DIR only required until we quit supporting python 1.5
AM_CPPFLAGS = @INCLTDL@ -I/@PYTHON_INC_DIR@ -I/@PYTHON_NUM_DIR@

plplotcmodule_la_SOURCES  = plplotcmodule.c
plplotcmodule_la_LDFLAGS  = \
	-rpath $(ourpythondir) \
	-no-undefined \
	-module \
	-avoid-version \
	$(libplplot)

plplot_widgetmodule_la_SOURCES  = plplot_widgetmodule.c
plplot_widgetmodule_la_LDFLAGS = $(plplotcmodule_la_LDFLAGS)

CLEANFILES = \
	plplotcmodule_p_double.c \
	plplotcmodule_p_single.c \
	plplotcmodule_p.c \
	plplotcmodule.c \
	setup.py 
  
EXTRA_DIST = \
	setup.py.in \
	plplotcmodule.i \
	makedocstrings.py \
	plplot_widgetmodule.c \
	plplot.py

# enable_python
endif
