# Makefile.am for PLplot (directory pkgcfg/)
###
### Process this file with automake to produce Makefile.in
###
# Copyright (C) 2003 Rafael Laboissiere
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

pkgconfigdir = $(libdir)/pkgconfig

if with_double
pkgconfig_DATA = plplot.pc
else
pkgconfig_DATA = plplot-float.pc
endif

LIBPLPLOT_LAI = $(top_builddir)/src/.libs/libplplot@LIB_TAG@.lai

install-data-hook:
	deplibs= ; \
	for i in `grep ^dependency_libs $(LIBPLPLOT_LAI) \
	         | sed "s/^dependency_libs='//" | sed "s/'//"` ; do \
	    lib=`echo $$i | sed 's:^.*/lib:-l:' | sed 's/.la$$//'` ; \
	    deplibs="$$deplibs $$lib" ; \
	done ; \
	sed -i "s/#DEPLIBS#/$$deplibs/" \
	    $(DESTDIR)$(pkgconfigdir)/$(pkgconfig_DATA)
