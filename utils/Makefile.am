# drivers/Makefile.am for PLplot
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

libplplot = ../src/libplplot$(LIB_TAG).la
libplplottcltk = ../bindings/tcl/libplplottcltk$(LIB_TAG).la

AM_CPPFLAGS = $(INCLTDL) -I../bindings/tcl $(TCLINCCMD) $(ITCLINCCMD)

if enable_tcl
bin_PROGRAMS = plrender pltek pltcl
else
bin_PROGRAMS = plrender pltek
endif

plrender_SOURCES = plrender.c
plrender_LDFLAGS = $(RPATH) -no-undefined
plrender_LDADD = $(libplplot)

pltek_SOURCES = pltek.c
pltek_LDFLAGS = $(RPATH) -no-undefined
pltek_LDADD = $(libplplot)

pltcl_SOURCES = pltcl.c
pltcl_LDFLAGS = $(RPATH) -no-undefined $(TCLLIBCMD) $(ITCLLIBCMD)
pltcl_LDADD = $(libplplottcltk)

### FIXME: Brute force inclusion in dist tarball.  The files below may
### be treated in a different way for installation [RL, 2003-03-06]
EXTRA_DIST = \
	tektest.c \
	xform.c