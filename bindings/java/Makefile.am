# bindings/java/Makefile.am for PLplot
###
### Process this file with automake to produce Makefile.in
###
# Copyright (C) 2002, 2003, 2004  Alan W. Irwin
# Copyright (C) 2003, 2004  Rafael Laboissiere
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

JAVAC = javac

SWIG_SUPPORT_DIR = $(top_srcdir)/bindings/swig-support
PLPLOTCAPI_I = $(SWIG_SUPPORT_DIR)/plplotcapi.i

SWIG_CMD = $(SWIG) -I$(SWIG_SUPPORT_DIR) -DPL_DOUBLE -DSWIG_JAVA -java 

swigjavafiles = \
	plplotjavacJNI.java \
	SWIGTYPE_p_p_char.java \
	plplotjavac.java 

# This required for swig-1.3.21, but not for swig-1.3.17
swigjavafiles += plplotjavacConstants.java

swigcfiles = plplotjavac_wrap.c

swiggenfiles = $(swigjavafiles) $(swigcfiles)

javafiles = \
	config.java \
	PLStreamc.java \
	Plplot.java \
	$(swigjavafiles)

# Note : the order of these is important since there is no formal 
# dependency checking.
javaclasses = \
	plplot/core/config.class \
	plplot/core/PLStreamc.class \
	plplot/core/plplotjavacJNI.class \
	plplot/core/SWIGTYPE_p_p_char.class\
	plplot/core/plplotjavacConstants.class \
	plplot/core/plplotjavac.class \
	plplot/core/Plplot.class

### FIXME: Brute force inclusion in dist tarball.  The files below may
### be treated in a different way for installation [RL, 2003-03-06]
EXTRA_DIST = \
	PLStream.java \
	PLStreamc.java \
	Plplot.java \
	README.javaAPI \
	javabind.c \
	plplotjavac.i \
	$(swiggenfiles)

$(swiggenfiles): plplotjavac.i $(PLPLOTCAPI_I)
	$(SWIG_CMD) -package plplot.core plplotjavac.i

# Must have "exec" in the name.
ourexecjavadir = $(DATA_DIR)/java/plplot/core
execjavawrapperdir = $(JAVAWRAPPER_DIR)

libplplot = $(top_builddir)/src/libplplot$(LIB_TAG).la

AM_CPPFLAGS = $(INCLTDL) $(JAVAINCCMD)

nodist_plplotjavac_wrap_la_SOURCES  = plplotjavac_wrap.c

# no-undefined should work on all platforms here since libplplot should
# resolve all symbols.
plplotjavac_wrap_la_LDFLAGS = \
	-rpath $(execjavawrapperdir) \
	-module \
	-avoid-version \
	-no-undefined \
	$(libplplot)

if enable_java

# Use javaJAVA for gcj support once gcc-3.2 use becomes
# widespread.

# Can't use java support for now since jikes doesn't handle dependencies 
# properly - instead do it using DATA and with an explicit rule.
#noinst_JAVA = $(javafiles)
#plplotjavadir = $(DATA_DIR)/java/
#plplotjava_DATA = plplot.jar

ourexecjava_DATA = $(javafiles) $(javaclasses) README.javaAPI

execjavawrapper_LTLIBRARIES = plplotjavac_wrap.la

# if enable_java
endif

plplot/core/%.class : %.java
	$(JAVAC) $(AM_JAVACFLAGS) $(JAVACFLAGS) $< -d . -classpath .

clean-local:
	rm -rf plplot

MAINTAINERCLEANFILES = $(swiggenfiles)
