# Makefile.am for PLplot (lib/csa/)
###
### Process this file with automake to produce Makefile.in
###
# Copyright (C) 2003 Joao Cardoso
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

lib_LTLIBRARIES = libcsa.la
libcsa_la_SOURCES = csa.c
libcsa_la_LDFLAGS = -version 0:0:0 -lm
AM_CPPFLAGS = -I.

docdir = $(DOCDIR)
docfiles = README README.1st

install-data-hook:
	$(mkinstalldirs) $(DESTDIR)$(docdir)
	for i in $(docfiles) ; do \
	  $(INSTALL) $$i $(DESTDIR)$(docdir)/$$i.csa ; \
	done

EXTRA_DIST = csa.h csa_internal.h nan.h version.h $(docfiles)
