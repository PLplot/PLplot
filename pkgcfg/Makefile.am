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

### Where?

pkgconfigdir = $(libdir)/pkgconfig

### What?

pkgconfig_DATA = plplot@LIB_TAG@.pc
INSTALL_LIBS = plplot@LIB_TAG@:src/.libs/libplplot@LIB_TAG@

if enable_tcl
pkgconfig_DATA += plplot@LIB_TAG@-tcl.pc
INSTALL_LIBS += plplot@LIB_TAG@-tcl:bindings/tcl/.libs/libplplottcltk@LIB_TAG@
endif

if enable_cxx
pkgconfig_DATA += plplot@LIB_TAG@-c++.pc
INSTALL_LIBS += plplot@LIB_TAG@-c++:bindings/c++/.libs/libplplotcxx@LIB_TAG@
endif

if enable_f77
pkgconfig_DATA += plplot@LIB_TAG@-f77.pc
INSTALL_LIBS += plplot@LIB_TAG@-f77:bindings/f77/.libs/libplplotf77@LIB_TAG@
endif

### How?

$(pkgconfig_DATA): pc-stamp
pc-stamp: gen-pc-files.sh plplot-master-pc
	./gen-pc-files.sh
	touch pc-stamp

install-data-hook:
	for i in $(INSTALL_LIBS) ; do \
	    pcfile=`echo $$i | sed "s/:.*$$//"` ; \
	    laifile=$(top_builddir)/`echo $$i | sed "s/^.*://"`.lai ; \
	    deplibs= ; \
	    for i in `grep ^dependency_libs $$laifile \
	              | sed "s/^dependency_libs='//" | sed "s/'//"` ; do \
	        lib=`echo $$i | sed 's:^/.*/lib:-l:' | sed 's/.la$$//'` ; \
	        deplibs="$$deplibs $$lib" ; \
	    done ; \
	    sed -i "s:#DEPLIBS#:$$deplibs:" \
	        $(DESTDIR)$(pkgconfigdir)/$$pcfile.pc ; \
	done

EXTRA_DIST = gen-pc-files.sh README
CLEANFILES = *.pc pc-stamp


