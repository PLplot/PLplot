# doc/Makefile.am for PLplot
###
### Process this file with automake to produce Makefile.in
###
# Copyright (C) 2002  Alan W. Irwin
# Copyright (C) 2003  Rafael Laboissiere
#
#This file is part of PLplot.
#
#PLplot is free software; you can redistribute it and/or modify
#it under the terms of the GNU Library General Public License as published by
#the Free Software Foundation; version 2 of the License.
#
#PLplot is distributed in the hope that it will be useful,
#but WITHOUT ANY WARRANTY; without even the implied warranty of
#MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#GNU Library General Public License for more details.
#
#You should have received a copy of the GNU Library General Public License
#along with the file PLplot; if not, write to the Free Software
#Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA

core_manpages = plm2gif.1 plpr.1 pltek.1 pstex2eps.1 plplot_libtool.1
tcl_manpages  = plrender.1 plserver.1 pltcl.1

manpages = $(core_manpages)
if enable_tcl
manpages += $(tcl_manpages)
endif

man_MANS = $(manpages)

EXTRA_DIST = \
  $(core_manpages) $(tcl_manpages) \
  README.local Xauthority abs.txt .indent.pro

SUBDIRS = docbook
