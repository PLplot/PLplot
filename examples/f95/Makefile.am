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
	x01f.f

AM_CPPFLAGS = -I$(top_srcdir)/include $(INCLTDL)

if enable_f95

examples_f95dir = $(DATA_DIR)/examples/f95
examples_f95_DATA = $(f95_examples) Makefile.examples

libplplot = $(top_builddir)/src/libplplot$(LIB_TAG).la
libplplotf95c = $(top_builddir)/bindings/f95/libplplotf95c$(LIB_TAG).la
libplplotf95 = $(top_builddir)/bindings/f95/libplplotf95$(LIB_TAG).la

check_PROGRAMS = \
	x01f

x01f_SOURCES = x01f.f

LDADD = $(libplplot) $(libplplotf95c) $(libplplotf95)

install-data-hook:
	( cd $(DESTDIR)$(examples_f95dir) ; mv -f Makefile.examples Makefile )

else

# The following empty rule is needed because we are in the "else" part of
# an AM_CONDITIONAL
install-data-hook:

# enable_f95
endif
