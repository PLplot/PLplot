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

if with_double
sed_command = 's/REPLACE_ME/typedef double PLFLT;/'
octave_libs = -lplplotd
else
sed_command = 's/REPLACE_ME/typedef float PLFLT;/'
octave_libs = -lplplot
endif

plplot_octave.h: plplot_octave_org.h
	sed plplot_octave_org.h -e $(sed_command) > plplot_octave.h

# Extra sed step is a correction to allow gcc 3.2 to compile.  This correction
# also still works on gcc 2.95
plplot_octave.cc tmp_stub: plplot_octave.h
	@MATWRAP@ -language octave plplot_octave.h -o plplot_octave.cc \
	-stub tmp_stub
	mv -f plplot_octave.cc tmp_plplot_octave.cc
	sed 's/ string / std::string /' tmp_plplot_octave.cc > plplot_octave.cc

# 'mkoctfile' does not accept -rpath, use LD_RUN_PATH
plplot_octave.oct plplot_octave.o: plplot_octave.cc
	LD_RUN_PATH=$(libdir) @MKOCTFILE@ -v -I. -L../../src/.libs \
	plplot_octave.cc $(octave_libs)

octavesharedir = $(datadir)/plplot_octave

# Ultimately, we may want to switch to the normal LT method of building
# shared objects cross-platform, using 
# octaveshare_LTLIBRARIES = plplot_octave.la
# but octave may not be able to handle libtool-created .la files for now
# and the old-fashioned way with octave's attempt to be cross-platform
# (using the mkoctfile command) is good enough for now.
# SCRIPTS rather than DATA because you want to preserve the permissions.
octaveshare_SCRIPTS = plplot_octave.oct

#5 From Joao's notes:
plplot_stub.m: etc/plplot.doc tmp_stub massage
	@head -5 tmp_stub > plplot_stub.m
	@echo "# It was also massaged to add online documentation" >> plplot_stub.m
	@echo "# extracted from some PLplot distribution files" >> plplot_stub.m
	@echo   >> plplot_stub.m
	@echo "1;" >> plplot_stub.m
	@echo >> plplot_stub.m
	./massage >> plplot_stub.m 2> missing_help

CLEANFILES = plplot_octave.h tmp_plplot_octave.cc plplot_octave.cc \
	plplot_octave.o tmp_stub plplot_octave.oct

#enable_octave
endif
