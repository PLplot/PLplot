# examples/java/Makefile.am for PLplot ###
### Process this file with automake to produce Makefile.in
###
# Copyright (C) 2002, 2004  Alan W. Irwin
# Copyright (C) 2004  Andrew Ross
#
# This file is part of PLplot.
#
# PLplot is free software; you can redistribute it and/or modify
# it under the terms of the GNU Library General Public License as published
# by the Free Software Foundation; version 2 of the License.
#
# PLplot is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Library General Public License for more details.
#
# You should have received a copy of the GNU Library General Public License
# along with the file PLplot; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307  USA

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
	x19.java \
	x22.java

javademos = README.javademos $(javafiles) Makefile.examples

# These are for make check compilation of examples.
JAVAC = javac
PLPLOT_CLASSPATH = `pwd`/$(top_builddir)/bindings/java/plplot.jar

examples_jar = plplot-examples.jar

example_classes = \
                 plplot/examples/x01.class \
                 plplot/examples/x02.class \
                 plplot/examples/x03.class \
                 plplot/examples/x04.class \
                 plplot/examples/x05.class \
                 plplot/examples/x06.class \
                 plplot/examples/x07.class \
                 plplot/examples/x08.class \
                 plplot/examples/x09.class \
                 plplot/examples/x10.class \
                 plplot/examples/x11.class \
                 plplot/examples/x12.class \
                 plplot/examples/x13.class \
                 plplot/examples/x14.class \
                 plplot/examples/x15.class \
                 plplot/examples/x16.class \
                 plplot/examples/x17.class \
                 plplot/examples/x18.class \
                 plplot/examples/x19.class \
                 plplot/examples/x22.class

if enable_java

# Duplicate this so the source appears in the examples/java directory for
# the user to read
examples_javadir = $(DATA_DIR)/examples/java
examples_java_DATA = $(javademos)

check-local: $(examples_jar)

$(examples_jar): $(example_classes)
	jar -cf $(examples_jar) plplot/examples/*.class

endif

install-data-hook:
	( cd $(DESTDIR)$(examples_javadir) ; mv -f Makefile.examples Makefile )

clean-local:
	rm -rf plplot $(examples_jar)

plplot/examples/%.class : %.java
	$(JAVAC) $(AM_JAVACFLAGS) $(JAVACFLAGS) $< -d . -classpath $(PLPLOT_CLASSPATH)

### FIXME: Brute force inclusion in dist tarball.  The files below may
### be treated in a different way for installation [RL, 2003-03-06]
EXTRA_DIST = $(javademos)
