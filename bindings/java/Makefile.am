# bindings/java/Makefile.am for PLplot
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

if enable_java

libplplot = ../../src/libplplot$(LIB_TAG).la

lib_LTLIBRARIES = libplplotjava@LIB_TAG@.la

AM_CPPFLAGS = $(INCLTDL) $(JAVAINCCMD)
version = -version-info $(SOVERSION)

sources = plplotjavac_wrap.c

plplotjavac_wrap.c plplotjavac.java: plplotjavac.i plplotcapi.i
	$(SWIG) -java -package plplot.core -DPL_DOUBLE plplotjavac.i

nodist_libplplotjava@LIB_TAG@_la_SOURCES  = $(sources)
libplplotjava@LIB_TAG@_la_LDFLAGS  = $(version) $(RPATH) -no-undefined $(libplplot)

javadir = $(libdir)/java/plplot/core
# At some point specify javaJAVA for gcj support when gcc-3.2 becomes
# widespread.
java_DATA = \
	README.javaAPI \
	SWIGTYPE_p_p_char.java \
	java/plplot/core/SWIGTYPE_p_p_char.class \
	PLStreamc.java \
	java/plplot/core/PLStreamc.class \
	config.java \
	java/plplot/core/config.class \
	plplotjavac.java \
	java/plplot/core/plplotjavac.class \
	plplotjavacJNI.java \
	java/plplot/core/plplotjavacJNI.class

java/plplot/core/SWIGTYPE_p_p_char.class java/plplot/core/PLStreamc.class \
	java/plplot/core/config.class \
	java/plplot/core/plplotjavac.class \
	java/plplot/core/plplotjavacJNI.class: \
	SWIGTYPE_p_p_char.java \
	PLStreamc.java config.java plplotjavac.java plplotjavacJNI.java
	mkdir -p java/plplot/core
	cd java/plplot/core ; ln -sf ../../../*.java .
	javac -d java SWIGTYPE_p_p_char.java PLStreamc.java \
	config.java plplotjavac.java plplotjavacJNI.java 

CLEANFILES = \
	java/plplot/core/SWIGTYPE_p_p_char.class \
	java/plplot/core/PLStreamc.class \
	java/plplot/core/config.class \
	java/plplot/core/plplotjavac.class \
	java/plplot/core/plplotjavacJNI.class

MAINTAINERCLEANFILES = \
	SWIGTYPE_p_p_char.java \
	plplotjavac.java \
	plplotjavacJNI.java \
	plplotjavac_wrap.c

# if enable_java
endif
