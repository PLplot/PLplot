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

if enable_octave

SUBDIRS = PLplot demos misc

if with_double
sed_command = 's/REPLACE_ME/typedef double PLFLT;/'
else
sed_command = 's/REPLACE_ME/typedef float PLFLT;/'
endif

octave_libs = -lplplot$(LIB_TAG)

# If the Makefile changes, because of "configure" beeing rerun, recompile
plplot_octave.h: plplot_octave_org.h Makefile
	sed -e $(sed_command) plplot_octave_org.h > plplot_octave.h

# Extra sed step is a correction to allow gcc 3.2 to compile.  This correction
# also still works on gcc 2.95
plplot_octave.cc tmp_stub: plplot_octave.h
	$(MATWRAP) -language octave plplot_octave.h -o plplot_octave.cc \
	-stub tmp_stub
	mv -f plplot_octave.cc tmp_plplot_octave.cc
	sed 's/ string / std::string /' tmp_plplot_octave.cc > plplot_octave.cc


# 'mkoctfile' does not accept -rpath, use LD_RUN_PATH
if mkoctfile_accepts_c_flag
MKOCTFILE_SOURCE = plplot_octave.o

plplot_octave.o: plplot_octave.cc
	$(MKOCTFILE) -v -I. --compile plplot_octave.cc

else
MKOCTFILE_SOURCE = plplot_octave.cc
endif

plplot_octave.oct: $(MKOCTFILE_SOURCE)
	LD_RUN_PATH=`pwd`/../../src/.libs $(MKOCTFILE) -v -I. \
	   $(MKOCTFILE_SOURCE) -L../../src/.libs $(octave_libs)

install-exec-local: plplot_octave-libdir.oct
	$(mkinstalldirs) $(DESTDIR)$(octavesharedir)
	$(INSTALL) plplot_octave-libdir.oct \
	  $(DESTDIR)$(octavesharedir)/plplot_octave.oct

plplot_octave-libdir.oct: $(MKOCTFILE_SOURCE)
	LD_RUN_PATH=$(libdir) $(MKOCTFILE) -v -I. $(MKOCTFILE_SOURCE) \
	  -o plplot_octave-libdir.oct -L$(libdir) $(octave_libs)

octavesharedir = $(OCTAVE_OCT_DIR)

# Ultimately, we may want to switch to the normal LT method of building
# shared objects cross-platform, using
# octaveshare_LTLIBRARIES = plplot_octave.la
# but octave may not be able to handle libtool-created .la files
# SCRIPTS rather than DATA because you want to preserve the permissions.
octaveshare_SCRIPTS = plplot_octave.oct

noinst_PROGRAMS = massage
massage_SOURCES = massage.c

# This should be refined with more comprehensive dependencies, a clean
# target to clean out of all the generated files at clean time, etc.  There
# are undoubtedly cross-platform issues as well.  But it is good enough for
# now for a Linux proof-of-concept.

plplot_octave_txt/plplot.doc: etc/plplot.doc
	mkdir -p plplot_octave_txt
	cp etc/plplot.doc plplot_octave_txt/plplot.doc
	cd plplot_octave_txt; \
	../../../doc/docbook/bin/api2text.pl \
	../../../doc/docbook/src/plplotdoc.xml.in \
	../../../doc/docbook/src/api.xml

plplot_stub.m: plplot_octave_txt/plplot.doc tmp_stub massage
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

CLEANFILES = plplot_octave.h tmp_plplot_octave.cc \
	plplot_octave.cc plplot_octave.o tmp_stub plplot_octave*.oct \
	plplot_stub.m missing_help $(doc_DATA)

#enable_octave
endif
