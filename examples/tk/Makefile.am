# examples/tk/Makefile.am for PLplot
###
### Process this file with automake to produce Makefile.in
###
# Copyright (C) 2002, 2004  Alan W. Irwin
# Copyright (C) 2004  Rafael Laboissiere
# 
# This file is part of PLplot.
#
# This file is free software; you can redistribute it and/or modify
# it under the terms of the GNU Library General Public License as published
# by the Free Software Foundation; version 2 of the License.
#
# This file is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Library General Public License for more details.
#
# You should have received a copy of the GNU Library General Public License
# along with the file; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307  USA

if enable_tk

examples_tkdir = $(DATA_DIR)/examples/tk

if enable_itcl
mkindex = $(top_builddir)/scripts/mktclIndex -itcl
else
mkindex = $(top_builddir)/scripts/mktclIndex -tcl
endif

check_DATA = \
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

### x01.tcl stands symbolically for x??.tcl.  If specify $(check_DATA)
### instead, then for loop below is usually incorrectly repeated for each file
### in check_DATA (depending on coarseness of timer).
x01.tcl: \
	../tcl/x01.tcl \
	../tcl/x02.tcl \
	../tcl/x03.tcl \
	../tcl/x04.tcl \
	../tcl/x05.tcl \
	../tcl/x06.tcl \
	../tcl/x07.tcl \
	../tcl/x08.tcl \
	../tcl/x09.tcl \
	../tcl/x10.tcl \
	../tcl/x11.tcl \
	../tcl/x12.tcl \
	../tcl/x13.tcl \
	../tcl/x14.tcl \
	../tcl/x15.tcl \
	../tcl/x16.tcl \
	../tcl/x17.tcl \
	../tcl/x18.tcl \
	../tcl/x19.tcl
	( cd ../tcl ; \
	  for file in x??.tcl ; do \
	    rm -f ../tk/$$file; $(LN_S) ../tcl/$$file ../tk/$$file ; \
	  done )

data_files = \
	README.tkdemos \
	runAllDemos.tcl \
	runExtendedDemos.tcl \
	tkdemos.tcl \
	xtk01.c \
	xtk02.c \
	xtk04.c

dist_examples_tk_DATA = $(data_files)

nodist_examples_tk_DATA = Makefile.examples tclIndex

tclIndex:
	$(mkindex)

examples_tk_SCRIPTS = \
	tk01 \
	tk02 \
	tk03 \
	tk04

libplplottcltk = $(top_builddir)/bindings/tcl/libplplottcltk$(LIB_TAG).la

check_PROGRAMS = \
	xtk01 \
	xtk02 \
	xtk04

cflags = -I$(top_builddir)/bindings/tk -I$(top_builddir)/bindings/tcl $(TCLINCCMD) $(TKINCCMD)
xtk01_CFLAGS = $(cflags)
xtk02_CFLAGS = $(cflags)
xtk04_CFLAGS = $(cflags)

xtk01_LDFLAGS = $(libplplottcltk)
xtk02_LDFLAGS = $(libplplottcltk)
xtk04_LDFLAGS = $(libplplottcltk)

# enable_tk
endif

### Assumes tcl install is done before tk install.
install-data-hook:
if enable_tk
	( cd $(DESTDIR)$(examples_tkdir) ; \
	  mv -f Makefile.examples Makefile ; \
	  cd ../tcl ; \
	  for file in x??.tcl ; do \
	    rm -f ../tk/$$file; $(LN_S) ../tcl/$$file ../tk/$$file ; \
	  done )
endif

EXTRA_DIST = $(data_files) Makefile.examples.in $(examples_tk_SCRIPTS)
CLEANFILES = tclIndex $(check_DATA) check_stamp
DISTCLEANFILES = Makefile.examples

