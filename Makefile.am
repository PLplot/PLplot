# Makefile.am for PLplot
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

src_dirs = lib src include data bindings

if enable_dyndrivers
drivers_src_dirs = $(src_dirs) drivers
else
drivers_src_dirs = drivers $(src_dirs)
endif

SUBDIRS = libltdl $(drivers_src_dirs) examples test utils scripts doc \
          pkgcfg

bin_SCRIPTS = plplot_libtool

plplot_libtool: libtool
	cp libtool plplot_libtool

docdir = $(datadir)/doc/plplot

doc_DATA = \
	AUTHORS \
	COPYING.LIB \
	ChangeLog \
	Copyright \
	FAQ \
	NEWS \
	PROBLEMS \
	README

CLEANFILES = plplot_libtool reconfig

EXTRA_DIST = debian fonts rpm sys

dist-hook:
	rm -rf `find $(distdir) -name CVS -o -name .cvsignore`
