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

docdir = $(DOCDIR)

docfiles = \
  BUGS \
  FGA \
  INSTALL \
  README \
  ToDo \
  USAGE

EXTRA_DIST = \
  COPYING \
  $(docfiles) \
  globals-in-scripts \
  massage.c \
  plplot_octave_org.h \
  plplot_octave_rej.h \
  plplot_octave_def \
  etc \
  matwrap \
  doc-stamp \
  plplot_octave_txt

doc-stamp plplot_octave_txt: etc/plplot.doc \
	   $(top_srcdir)/doc/docbook/bin/api2text.pl \
	   $(top_srcdir)/doc/docbook/src/plplotdoc.xml.in \
	   $(top_srcdir)/doc/docbook/src/api.xml
if has_xml_pm
	mkdir -p plplot_octave_txt
	cp etc/plplot.doc plplot_octave_txt/plplot.doc
	( cd plplot_octave_txt ; \
	  $(PERL) ../$(top_srcdir)/doc/docbook/bin/api2text.pl \
	  ../$(top_srcdir)/doc/docbook/src/plplotdoc.xml.in \
	  ../$(top_srcdir)/doc/docbook/src/api.xml ) \
	&& touch doc-stamp
else
	@echo "Perl modules not available: cannot generate full online help for plplot_octave"
endif


if enable_octave

octave_libs = -L../../src/.libs -lplplot$(LIB_TAG)

# Extra sed step is a correction to allow gcc 3.2 to compile.  This correction
# also still works on gcc 2.95
plplot_octave.cc tmp_stub: plplot_octave_org.h ${top_builddir}/config.h
	$(PERL) $(MATWRAP) -language octave -o plplot_octave.cc	-stub tmp_stub \
	-cpp_ignore plplot_octave_rej.h ${top_builddir}/config.h \
	-cpp c++ -E -C plplot_octave_org.h
	mv -f plplot_octave.cc tmp_plplot_octave.cc
	sed 's/ string / std::string /' tmp_plplot_octave.cc > plplot_octave.cc

plplot_octave.oct plplot_octave.o: plplot_octave.cc
	LD_RUN_PATH=`pwd`/../../src/.libs $(MKOCTFILE_VARS) \
	    $(MKOCTFILE) -v -I. plplot_octave.cc $(octave_libs)

if with_rpath
SET_LDRPATH = LD_RUN_PATH=$(libdir)
else
SET_LDRPATH =
endif

plplot_octave-libdir.oct: plplot_octave.o
	$(SET_LDRPATH) $(MKOCTFILE_VARS) \
	    $(MKOCTFILE) -v -I. plplot_octave.o \
	    -o plplot_octave-libdir.oct $(octave_libs)

install-data-hook: plplot_octave-libdir.oct
	$(mkinstalldirs) $(DESTDIR)$(octavesharedir)
	$(INSTALL) plplot_octave-libdir.oct \
	  $(DESTDIR)$(octavesharedir)/plplot_octave.oct
	$(mkinstalldirs) $(DESTDIR)$(docdir)
	for i in $(docfiles) ; do \
	  $(INSTALL) $$i $(DESTDIR)$(docdir)/$$i.octave ; \
	done

octavesharedir = $(OCTAVE_OCT_DIR)
octaveshare_SCRIPTS = plplot_octave.oct

noinst_PROGRAMS = massage
massage_SOURCES = massage.c

plplot_stub.m: doc-stamp tmp_stub massage
	@head -5 tmp_stub > plplot_stub.m
	@echo "# It was also massaged to add online documentation" >> plplot_stub.m
	@echo "# extracted from some PLplot distribution files" >> plplot_stub.m
	@echo   >> plplot_stub.m
	@echo "1;" >> plplot_stub.m
	@echo >> plplot_stub.m
	@cat plplot_octave_def >> plplot_stub.m
	@echo >> plplot_stub.m
	./massage >> plplot_stub.m 2> missing_help

plplot_octavedir = $(PLPLOT_OCTAVE_DIR)
plplot_octave_DATA = plplot_stub.m

else

install-data-hook:

#enable_octave
endif

CLEANFILES = plplot_octave.h tmp_plplot_octave.cc \
	plplot_octave.cc plplot_octave.o tmp_stub plplot_octave*.oct \
	plplot_stub.m missing_help

MAINTAINERCLEANFILES = doc-stamp plplot_octave_txt/*
