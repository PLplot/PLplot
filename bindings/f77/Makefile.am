# bindings/f77/Makefile.am for PLplot
###
### Process this file with automake to produce Makefile.in
###
# Copyright (C) 2001 Rafael Laboissiere
# 
# This file is free software; you can redistribute it and/or modify it
# under the terms of the GNU General Public License as published by the
# Free Software Foundation; either version 2, or (at your option) any
# later version.
# 
# This file is distributed in the hope that it will be useful, but WITHOUT
# ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
# FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
# for more details.
# 
# You should have received a copy of the GNU General Public License
# along with Octave; see the file COPYING.  If not, write to the Free
# Software Foundation, 59 Temple Place - Suite 330, Boston, MA
# 02111-1307, USA. 

if enable_f77
if with_double
lib_LTLIBRARIES = libplf77d.la
else
lib_LTLIBRARIES = libplf77.la
endif
endif

version = -version-info @SOVERSION@

sources = \
  plstubs.h \
  sc3d.c \
  sccont.c \
  scstubs.c \
  strutil.f

libplf77_la_SOURCES  = $(sources)
libplf77d_la_SOURCES = $(sources)

libplf77_la_DEPENDENCIES  = sfstubs.lo
libplf77d_la_DEPENDENCIES = sfstubs.lo

libplf77_la_LDFLAGS  = $(version)
libplf77d_la_LDFLAGS = $(version)

EXTRA_DIST = sfstubs.fm4

.fm4.f:
	@M4@ @M4_FLAGS@ $< >$*.f

SUFFIXES = .fm4

