# bindings/octave/Makefile.am for PLplot
###
### Process this file with automake to produce Makefile.in
###
# Copyright (C) 2002 Alan W. Irwin
# Copyright (C) 2002 Joao Cardoso
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

SUBDIRS = PLplot demos misc

EXTRA_DIST = \
  BUGS \
  COPYING \
  FGA \
  INSTALL \
  README \
  ToDo \
  USAGE \
  globals-in-scripts \
  massage.c \
  plplot_octave_org.h \
  plplot_octave_rej.h \
  etc \
  matwrap

if enable_octave

EXTRA_DIST += doc-stamp plplot_octave_txt

if with_double
sed_command = 's/REPLACE_ME/typedef double PLFLT;/'
else
sed_command = 's/REPLACE_ME/typedef float PLFLT;/'
endif

octave_libs = -L../../src/.libs -lplplot$(LIB_TAG)

# If the Makefile changes, because of "configure" beeing rerun, recompile
#plplot_octave.h: plplot_octave_org.h Makefile
#	sed -e $(sed_command) plplot_octave_org.h > plplot_octave.h

# Extra sed step is a correction to allow gcc 3.2 to compile.  This correction
# also still works on gcc 2.95
plplot_octave.cc tmp_stub: plplot_octave_org.h
	$(MATWRAP) -language octave -o plplot_octave.cc	-stub tmp_stub \
	-cpp_ignore plplot_octave_rej.h ../../config.h \
	-cpp c++ -E -C plplot_octave_org.h
	mv -f plplot_octave.cc tmp_plplot_octave.cc
	sed 's/ string / std::string /' tmp_plplot_octave.cc > plplot_octave.cc

plplot_octave.oct plplot_octave.o: plplot_octave.cc
	LD_RUN_PATH=`pwd`/../../src/.libs $(MKOCTFILE) -v -I. \
	   plplot_octave.cc $(octave_libs)

if with_rpath
SET_LDRPATH = LD_RUN_PATH=$(libdir)
else
SET_LDRPATH =
endif

plplot_octave-libdir.oct: plplot_octave.o
	$(SET_LDRPATH) $(MKOCTFILE) -v -I. plplot_octave.o \
	  -o plplot_octave-libdir.oct $(octave_libs)

install-data-hook: plplot_octave-libdir.oct
	$(mkinstalldirs) $(DESTDIR)$(octavesharedir)
	$(INSTALL) plplot_octave-libdir.oct \
	  $(DESTDIR)$(octavesharedir)/plplot_octave.oct

octavesharedir = $(OCTAVE_OCT_DIR)
octaveshare_SCRIPTS = plplot_octave.oct

noinst_PROGRAMS = massage
massage_SOURCES = massage.c

# This should be refined with more comprehensive dependencies, a clean
# target to clean out of all the generated files at clean time, etc.  There
# are undoubtedly cross-platform issues as well.  But it is good enough for
# now for a Linux proof-of-concept.

doc-stamp: etc/plplot.doc \
	   $(top_srcdir)/doc/docbook/bin/api2text.pl \
	   $(top_srcdir)/doc/docbook/src/plplotdoc.xml.in \
	   $(top_srcdir)/doc/docbook/src/api.xml
	mkdir -p plplot_octave_txt
	cp etc/plplot.doc plplot_octave_txt/plplot.doc
	( cd plplot_octave_txt ; \
	  ../$(top_srcdir)/doc/docbook/bin/api2text.pl \
	  ../$(top_srcdir)/doc/docbook/src/plplotdoc.xml.in \
	  ../$(top_srcdir)/doc/docbook/src/api.xml ) \
	&& touch doc-stamp

plplot_stub.m: doc-stamp tmp_stub massage
	@head -5 tmp_stub > plplot_stub.m
	@echo "# It was also massaged to add online documentation" >> plplot_stub.m
	@echo "# extracted from some PLplot distribution files" >> plplot_stub.m
	@echo   >> plplot_stub.m
	@echo "1;" >> plplot_stub.m
	@echo >> plplot_stub.m
	./massage >> plplot_stub.m 2> missing_help

plplot_octavedir = $(PLPLOT_OCTAVE_DIR)
plplot_octave_DATA = plplot_stub.m

docdir = $(DOCDIR)

doc_DATA =       \
  BUGS.octave    \
  INSTALL.octave \
  FGA.octave     \
  ToDo.octave    \
  USAGE.octave   \
  README.octave

%.octave: %
	  cp $< $@

else

install-data-hook:

#enable_octave
endif

CLEANFILES = plplot_octave.h tmp_plplot_octave.cc \
	plplot_octave.cc plplot_octave.o tmp_stub plplot_octave*.oct \
	plplot_stub.m missing_help $(doc_DATA) 

MAINTAINERCLEANFILES = doc-stamp plplot_octave_txt/*
