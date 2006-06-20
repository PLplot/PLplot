# test/Makefile.am for PLplot
###
### Process this file with automake to produce Makefile.in
###
# Copyright (C) 2002  Alan W. Irwin
# Copyright (C) 2004  Rafael Laboissiere
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
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA

examplesdir = $(DATA_DIR)/examples

scripts = test_c.sh

if enable_cxx
scripts += test_cxx.sh
endif

if enable_f77
scripts += test_f77.sh
endif

if enable_f95
scripts += test_f95.sh
endif

if enable_java
scripts += test_java.sh
endif

if enable_octave
scripts += test_octave.sh
endif

if enable_python
scripts += test_python.sh
endif

if enable_tcl
scripts += test_tcl.sh
endif

if enable_pdl
scripts += test_perl.sh
endif

examples_SCRIPTS = $(scripts) plplot-test.sh

# The variables below allow "make check" to work from both the build and
# install trees.  Notice that the environment variable EXAMPLES_DIR must be
# absolute, otherwise test_tcl.sh and test_python.sh will fail.  The
# construct below works because top_builddir is always relative.

TESTS = plplot-test.sh
TESTS_ENVIRONMENT = EXAMPLES_DIR=$(top_builddir)/examples 
TESTS_ENVIRONMENT += SRC_EXAMPLES_DIR=$(top_srcdir)/examples 
TESTS_ENVIRONMENT += PLPLOT_LIB=$(top_srcdir)/data
if enable_java
TESTS_ENVIRONMENT += PLPLOT_JAVA_WRAP_DIR=$(top_builddir)/bindings/java/.libs/ PLPLOT_CLASSPATH=$(top_builddir)/bindings/java/ 
endif
TESTS_ENVIRONMENT += $(SHELL)


EXTRA_DIST = test_single_python.sh \
             test_single_tcl.sh

CLEANFILES = *.ps *.png* *.jpeg* *.gif* *.cgm *.fig* *.plmeta
