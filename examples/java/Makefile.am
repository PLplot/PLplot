# examples/java/Makefile.am for PLplot
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

javafiles = \
	x01.java \
	x02.java \
	x03.java \
	x04.java \
	x05.java \
	x06.java \
	x07.java \
	x08.java \
	x09.java \
	x10.java \
	x11.java \
	x12.java \
	x13.java \
	x14.java \
	x15.java \
	x16.java \
	x17.java \
	x18.java \
	x19.java

javademos = README.javademos $(javafiles)

if enable_java

# Must have "exec" in the name.
ourexecjavadir = $(libdir)/java/plplot/examples

ourexecjava_DATA = $(javademos)

endif

### Use install-hook to compile the java files that have been installed.

install-exec-hook:
if enable_java
	( cd $(DESTDIR)$(libdir) ; \
	  javac -classpath java -d java java/plplot/examples/*.java)
endif
### FIXME: Brute force inclusion in dist tarball.  The files below may
### be treated in a different way for installation [RL, 2003-03-06]
EXTRA_DIST = $(javademos)
