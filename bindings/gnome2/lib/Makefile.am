# bindings/gnome2/lib/Makefile.am for PLplot
###
### Process this file with automake to produce Makefile.in
###
# Copyright (C) 2005  Alan W. Irwin
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

AM_CPPFLAGS = -I$(top_srcdir)/include

libplplot = $(top_builddir)/src/libplplot$(LIB_TAG).la
if enable_gcw
lib_LTLIBRARIES = libplplotgnome2@LIB_TAG@.la
libplplotgnome2@LIB_TAG@_la_SOURCES = \
	plplotcanvas.c \
	gcw-lib.c
libplplotgnome2@LIB_TAG@_la_LDFLAGS = \
	-version-info $(PLPLOTGNOME_SOVERSION) $(RPATH) -no-undefined \
	$(libplplot) $(GCW_LIBS) $(lmflag)
endif
