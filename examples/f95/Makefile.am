# examples/f95/Makefile.am for PLplot
###
### Process this file with automake to produce Makefile.in
###
# Copyright (C) 2002, 2004  Alan W. Irwin
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
# along with PLplot; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA

# x19f.f excluded from this list because the plmap and plmeridians fortran
# API is not implemented as part of the common API (and probably won't be
# for a long time because of having to work around the mapform type of
# argument for these functions).

f95_examples = \
	x01f.f90 \
	x02f.f90 \
	x03f.f90 \
	x04f.f90 \
	x05f.f90 \
	x06f.f90 \
	x07f.f90 \
	x08f.f90 \
	x09f.f90 \
	x10f.f90 \
	x11f.f90 \
	x12f.f90 \
	x13f.f90 \
	x14f.f90 \
	x15f.f90 \
	x16af.f90 \
	x16f.f90 \
	x17f.f90 \
	x18f.f90 \
	x19f.f90 \
	x22f.f90 \
	x23f.f90

AM_FCFLAGS = -I$(top_srcdir)/include $(INCLTDL) -I$(top_builddir)/bindings/f95

if enable_f95

examples_f95dir = $(DATA_DIR)/examples/f95
examples_f95_DATA = $(f95_examples) Makefile.examples

libplplot = $(top_builddir)/src/libplplot$(LIB_TAG).la
libplplotf95c = $(top_builddir)/bindings/f95/libplplotf95c$(LIB_TAG).la
libplplotf95 = $(top_builddir)/bindings/f95/libplplotf95$(LIB_TAG).la

check_PROGRAMS = \
	x01f \
	x02f \
	x03f \
	x04f \
	x05f \
	x06f \
	x07f \
	x08f \
	x09f \
	x10f \
	x11f \
	x12f \
	x13f \
	x14f \
	x15f \
	x16af \
	x16f \
	x17f \
	x18f \
	x19f \
	x22f \
	x23f

x01f_SOURCES = x01f.f90
x02f_SOURCES = x02f.f90
x03f_SOURCES = x03f.f90
x04f_SOURCES = x04f.f90
x05f_SOURCES = x05f.f90
x06f_SOURCES = x06f.f90
x07f_SOURCES = x07f.f90
x08f_SOURCES = x08f.f90
x09f_SOURCES = x09f.f90
x10f_SOURCES = x10f.f90
x11f_SOURCES = x11f.f90
x12f_SOURCES = x12f.f90
x13f_SOURCES = x13f.f90
x14f_SOURCES = x14f.f90
x15f_SOURCES = x15f.f90
x16f_SOURCES = x16f.f90
x16af_SOURCES = x16af.f90
x17f_SOURCES = x17f.f90
x18f_SOURCES = x18f.f90
x19f_SOURCES = x19f.f90
x22f_SOURCES = x22f.f90
x23f_SOURCES = x23f.f90

LDADD = $(libplplot) $(libplplotf95c) $(libplplotf95)

install-data-hook:
	( cd $(DESTDIR)$(examples_f95dir) ; mv -f Makefile.examples Makefile )

else

# The following empty rule is needed because we are in the "else" part of
# an AM_CONDITIONAL
install-data-hook:

# enable_f95
endif
