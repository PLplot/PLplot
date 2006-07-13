# pkgcfg/CMakeLists.txt for PLplot
###
### Process this file with cmake to produce Makefile
###
# Copyright (C) 2006 Alan W. Irwin
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

#    "f77:F77:Fortran77 bindings, :${libplplot${LIB_TAG}_LINK_FLAGS}:${libplplot${LIB_TAG}_COMPILE_FLAGS}"
#    "f95:F95:Fortran95 bindings, :${libplplot${LIB_TAG}_LINK_FLAGS}:${libplplot${LIB_TAG}_COMPILE_FLAGS}"
#    "gnome2:Gnome:PLplot for Gnome/GTK, :${libplplot${LIB_TAG}_LINK_FLAGS}:${libplplot${LIB_TAG}_COMPILE_FLAGS}"
#    "tcl:Tcl/Tk:Tcl/Tk bindings, :${libplplot${LIB_TAG}_LINK_FLAGS}:${libplplot${LIB_TAG}_COMPILE_FLAGS}"
#
#pkgconfigdir = $(PKG_CONFIG_DIR)
#docdir = $(DOC_DIR)
#
#### What?
#
#if with_pkg_config
#
#pkgconfig_DATA = plplot@LIB_TAG@.pc
#
#if enable_tcl
#pkgconfig_DATA += plplot@LIB_TAG@-tcl.pc
#endif
#
#if enable_cxx
#pkgconfig_DATA += plplot@LIB_TAG@-c++.pc
#endif
#
#if enable_f77
#pkgconfig_DATA += plplot@LIB_TAG@-f77.pc
#endif
#
#if enable_f95
#pkgconfig_DATA += plplot@LIB_TAG@-f95.pc
#endif
#
#if enable_gcw
#pkgconfig_DATA += plplot@LIB_TAG@-gnome2.pc
#endif
#
#### How?
#
#$(pkgconfig_DATA): pc-stamp
#pc-stamp: gen-pc-files.pl
#	$(PERL) gen-pc-files.pl
#	touch pc-stamp
#
#endif
#
#install-data-hook:
#if with_pkg_config
#	$(mkinstalldirs) $(DESTDIR)$(docdir)
#	$(INSTALL) $(top_srcdir)/pkgcfg/README $(DESTDIR)$(docdir)/README.pkg-config
#endif
#
#EXTRA_DIST = README
#CLEANFILES = *.pc pc-stamp
