# examples/c/Makefile.am for PLplot
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

AM_CPPFLAGS = @INCLTDL@
examples_f77dir = ${prefix}/@DATA_DIR@/../examples/f77
f77m4_source = \
  fmacs.m4 \
  x01f.fm4 \
  x02f.fm4 \
  x03f.fm4 \
  x04f.fm4 \
  x05f.fm4 \
  x06f.fm4 \
  x07f.fm4 \
  x08f.fm4 \
  x09f.fm4 \
  x10f.fm4 \
  x11f.fm4 \
  x12f.fm4 \
  x13f.fm4 \
  x16f.fm4

examples_f77_DATA = \
  Makefile.examples \
  x01f.f \
  x02f.f \
  x03f.f \
  x04f.f \
  x05f.f \
  x06f.f \
  x07f.f \
  x08f.f \
  x09f.f \
  x10f.f \
  x11f.f \
  x12f.f \
  x13f.f \
  x16f.f

# Since these are all generated....
CLEANFILES = $(examples_f77_DATA)
# Need explicit dependencies for Solaris make.
x01f.f: x01f.fm4 fmacs.m4

x02f.f: x02f.fm4 fmacs.m4

x03f.f: x03f.fm4 fmacs.m4

x04f.f: x04f.fm4 fmacs.m4

x05f.f: x05f.fm4 fmacs.m4

x06f.f: x06f.fm4 fmacs.m4

x07f.f: x07f.fm4 fmacs.m4

x08f.f: x08f.fm4 fmacs.m4

x09f.f: x09f.fm4 fmacs.m4

x10f.f: x10f.fm4 fmacs.m4

x11f.f: x11f.fm4 fmacs.m4

x12f.f: x12f.fm4 fmacs.m4

x13f.f: x13f.fm4 fmacs.m4

x16f.f: x16f.fm4 fmacs.m4

SUFFIXES = .fm4

.fm4.f:
	$(M4) @M4_FLAGS@ $< >$*.f

EXTRA_DIST = $(f77m4_source) Makefile.examples.in
