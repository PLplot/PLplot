# examples/tk/Makefile.am for PLplot
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

examples_tkdir = ${prefix}/@DATA_DIR@/../examples/tk

if enable_tk

if enable_itcl
mkindex = ../../scripts/mktclIndex -itcl
else
mkindex = ../../scripts/mktclIndex -itcl
endif

generated_files = \
	tclIndex \
	x01.tcl \
	x02.tcl \
	x03.tcl \
	x04.tcl \
	x05.tcl \
	x06.tcl \
	x07.tcl \
	x08.tcl \
	x09.tcl \
	x10.tcl \
	x11.tcl \
	x12.tcl \
	x13.tcl \
	x14.tcl \
	x15.tcl \
	x16.tcl \
	x17.tcl \
	x18.tcl \
	x19.tcl
data_files = \
	README.tkdemos \
	runAllDemos.tcl \
	runExtendedDemos.tcl \
	tkdemos.tcl \
	xtk01.c \
	xtk02.c \
	xtk04.c

examples_tk_DATA = $(data_files) Makefile.examples $(generated_files)
examples_tk_SCRIPTS = \
	tk01 \
	tk02 \
	tk03 \
	tk04

$(generated_files): 
	cp ../tcl/x??.tcl .
	$(mkindex)

endif

EXTRA_DIST = $(data_files) $(examples_tk_SCRIPTS) $(mkindex)
CLEANFILES = $(generated_files)
DISTCLEANFILES = Makefile.examples
