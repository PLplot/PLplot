# bindings/tk/Makefile.am for PLplot
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

tcldir = ${prefix}/$(DATA_DIR)/../tcl

libplplottcltk = ../tcl/libplplottcltk$(LIB_TAG).la

if enable_tk

# ITKINCCMD same as TCLINCCMD so not defined in top-level configure.in
AM_CPPFLAGS = \
	-I../tcl/ \
	$(INCLTDL) \
	$(TCLINCCMD) \
	$(TKINCCMD)

pkginclude_HEADERS = pltk.h
bin_PROGRAMS = plserver
plserver_SOURCES = plserver.c
plserver_LDFLAGS = -rpath $(libdir) -no-undefined
plserver_LDADD = $(libplplottcltk)

tcldata = \
	FileSelector.tcl \
	Pltkwin.tcl \
	about.tcl \
	help_gui.tcl \
	help_keys.tcl \
	help_tcltk.tcl \
	plclient.tcl \
	plcolor.tcl \
	plconfig.tcl \
	pldefaults.tcl \
	plplot.tcl \
	plserver.tcl \
	pltools.tcl \
	plwidget.tcl

itkdata = \
	PLWin.itk \
	PLXWin.itk

if enable_itcl
mkindex = ../../scripts/mktclIndex -itcl
else
mkindex = ../../scripts/mktclIndex -tcl
endif

tclIndex: $(tcldata) $(itkdata)
	$(mkindex)

paldata = \
	cmap0a.pal \
	cmap1a.pal \
	cmap1a1.pal \
	cmap1b.pal \
	cmap1c.pal \
	cmap1d.pal

tcl_DATA = $(tcldata) $(itkdata) tclIndex $(paldata)

endif

EXTRA_DIST = pltk.h plserver.h tcpip.h
