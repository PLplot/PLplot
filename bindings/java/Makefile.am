# bindings/java/Makefile.am for PLplot
###
### Process this file with automake to produce Makefile.in
###
# Copyright (C) 2002, 2003  Alan W. Irwin
# Copyright (C) 2003  Rafael Laboissiere
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


swigjavafiles = \
	SWIGTYPE_p_p_char.java \
	plplotjavac.java \
	plplotjavacJNI.java

swigcfiles = plplotjavac_wrap.c

swiggenfiles = $(swigjavafiles) $(swigcfiles)

javafiles = \
	PLStreamc.java \
	config.java \
	$(swigjavafiles)

### FIXME: Brute force inclusion in dist tarball.  The files below may
### be treated in a different way for installation [RL, 2003-03-06]
EXTRA_DIST = \
	PLStream.java \
	PLStreamc.java \
	README.javaAPI \
	javabind.c \
	plplotcapi.i \
	plplotjavac.i \
	$(swiggenfiles)

$(swiggenfiles): plplotjavac.i plplotcapi.i
	$(SWIG) -java -package plplot.core -DPL_DOUBLE plplotjavac.i

# Must have "exec" in the name.
ourexecjavadir = $(libdir)/java/plplot/core

libplplot = ../../src/libplplot$(LIB_TAG).la

AM_CPPFLAGS = $(INCLTDL) $(JAVAINCCMD)

nodist_plplotjavac_wrap_la_SOURCES  = plplotjavac_wrap.c

# Do not use -no-undefined here since symbols should be resolved by
# java when it dynamically loads this shared object.
plplotjavac_wrap_la_LDFLAGS = \
	-rpath $(ourexecjavadir) \
	-module \
	-avoid-version \
	$(libplplot)

if enable_java

# Use javaJAVA for gcj support once gcc-3.2 use becomes
# widespread.
ourexecjava_DATA = README.javaAPI $(javafiles)

ourexecjava_LTLIBRARIES = plplotjavac_wrap.la

# if enable_java
endif

### Use install-hook to remove the unneeded (by java) *.a and *.la files
### and also compile the java files that have been installed.

install-exec-hook:
if enable_java
	( cd $(DESTDIR)$(ourexecjavadir) ; \
	  rm -f plplotjavac_wrap*.a plplotjavac_wrap*.la ; \
	  cd $(DESTDIR)$(libdir) ; \
	  javac -classpath java -d java java/plplot/core/*.java)
endif

MAINTAINERCLEANFILES = $(swiggenfiles)
