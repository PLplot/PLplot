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

if with_double
   libplplot = ../src/libplplotd.la
   libplplottcltk = ../bindings/tcl/libplplottcltkd.la
else
   libplplot = ../src/libplplot.la
   libplplottcltk = ../bindings/tcl/libplplottcltk.la
endif

AM_CPPFLAGS = @INCLTDL@ -I../bindings/tcl @TCLINCCMD@ @ITCLINCCMD@

if enable_tcl
bin_PROGRAMS = plrender pltek pltcl
else
bin_PROGRAMS = plrender pltek
endif

plrender_SOURCES = plrender.c
plrender_LDFLAGS = -rpath $(libdir) -no-undefined
plrender_LDADD = $(libplplot)

pltek_SOURCES = pltek.c
pltek_LDFLAGS = -rpath $(libdir) -no-undefined
pltek_LDADD = $(libplplot)

pltcl_SOURCES = pltcl.c
pltcl_LDFLAGS = -rpath $(libdir) -no-undefined @TCLLIBCMD@ @ITCLLIBCMD@
pltcl_LDADD = $(libplplottcltk)
