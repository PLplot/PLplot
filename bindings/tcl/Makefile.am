# bindings/tcl/Makefile.am for PLplot
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

tcldir = ${prefix}/@DATA_DIR@/../tcl

if with_double
libplplot = -L../../src -lplplotd
libmatrix = -L. -ltclmatrixd
else
libplplot = -L../../src -lplplot
libmatrix = -L. -ltclmatrix
endif

if enable_tcl
generated_scripts = plplot.tcl
tcl_DATA = $(generated_scripts) tclIndex

if enable_itcl
mkindex = ../../scripts/mktclIndex -itcl
else
mkindex = ../../scripts/mktclIndex -tcl
endif

if with_double
lib_LTLIBRARIES = libtclmatrixd.la libplplottcltkd.la 
else
lib_LTLIBRARIES = libtclmatrix.la libplplottcltk.la
endif

endif

plplot.tcl: ../tk/plplot.tcl
	rm -f plplot.tcl
	cp ../tk/plplot.tcl plplot.tcl

tclIndex: $(generated_scripts)
	$(mkindex)

pkginclude_HEADERS = pltcl.h tclMatrix.h

tclAPI.c: tclgen.h tclgen_s.h tclgen.c

tclgen.h tclgen_s.h tclgen.c : pltclgen plapi.tpl tclcmd.tpl
	perl pltclgen

EXTRA_DIST = pltclgen plapi.tpl tclcmd.tpl ../tk/plplot.tcl
CLEANFILES = $(generated_scripts) tclIndex
MAINTAINERCLEANFILES = tclgen.h tclgen_s.h tclgen.c
# ITKINCCMD same as TCLINCCMD so not defined in top-level configure.in
AM_CPPFLAGS = @INCLTDL@ @TCLINCCMD@ @ITCLINCCMD@ @TKINCCMD@
version = -version-info @SOVERSION@

libtclmatrix_la_SOURCES  = \
	tclMatrix.c \
	matrixInit.c

libtclmatrix_la_LDFLAGS  = $(version) -rpath $(libdir) -no-undefined \
	@TCLLIBCMD@

if enable_tcl
tclsources = \
	tclAPI.c \
	tclMain.c
endif

if enable_tk
tksources = \
	../tk/Pltk_Init.c \
	../tk/plframe.c \
	../tk/plr.c \
	../tk/tcpip.c \
	../tk/tkMain.c
endif

## This library should probably be broken up further into at least a tcl
## and tk component.

libplplottcltk_la_SOURCES  = $(tclsources) $(tksources)

libplplottcltk_la_LDFLAGS  = \
	$(version) \
	-rpath \
	$(libdir) \
	-no-undefined \
	@TCLLIBCMD@ \
        @ITCLLIBCMD@ \
	@ITKLIBCMD@ \
	@TKLIBCMD@ \
	$(libplplot) \
	$(libmatrix)

libtclmatrixd_la_SOURCES = $(libtclmatrix_la_SOURCES)
libtclmatrixd_la_LDFLAGS = $(libtclmatrix_la_LDFLAGS)

libplplottcltkd_la_SOURCES = $(libplplottcltk_la_SOURCES)
libplplottcltkd_la_LDFLAGS = $(libplplottcltk_la_LDFLAGS)
