# test/Makefile.am for PLplot
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

examplesdir = ${prefix}/$(DATA_DIR)/../examples

scripts = test_c.sh

if enable_cxx
scripts += test_cxx.sh
endif

if enable_f77
scripts += test_f77.sh
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

examples_SCRIPTS = $(scripts) plplot-test.sh

EXTRA_DIST = test_*.sh plplot-test.sh.in
DISTCLEANFILES = plplot-test.sh
