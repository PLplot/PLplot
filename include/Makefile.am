# include/Makefile.am for PLplot
###
### Process this file with automake to produce Makefile.in
###
# Copyright (C) 2002 Rafael Laboissiere
# Copyright (C) 2002 Alan W. Irwin
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
#Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307  USA

# Installed header files that should be distributed
dist_pkginclude_HEADERS = \
	disptab.h \
	drivers.h \
	pdf.h \
	pldebug.h \
	plevent.h \
	plplot.h \
	plplotP.h \
	plstrm.h \
	plxwd.h

# Installed header files that should not be distributed
nodist_pkginclude_HEADERS = \
	plConfig.h \
	plDevs.h \
	plhershey-unicode.h

# Uninstalled header files that should be distributed
EXTRA_DIST = \
	metadefs.h \
	plcore.h \
	plfreetype.h \
	ps.h \
	plhershey-unicode.h

BUILT_SOURCES = plhershey-unicode.h
MAINTAINERCLEANFILES = plhershey-unicode.h

plhershey-unicode.h: $(top_builddir)/fonts/plhershey-unicode-gen \
	$(top_srcdir)/fonts/plhershey-unicode.csv
	$(top_builddir)/fonts/plhershey-unicode-gen \
	$(top_srcdir)/fonts/plhershey-unicode.csv plhershey-unicode.h
