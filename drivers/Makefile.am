# drivers/Makefile.am for PLplot
###
### Process this file with automake to produce Makefile.in
###
# Copyright (C) 2001 Rafael Laboissiere
# 
# This file is free software; you can redistribute it and/or modify it
# under the terms of the GNU General Public License as published by the
# Free Software Foundation; either version 2, or (at your option) any
# later version.
# 
# This file is distributed in the hope that it will be useful, but WITHOUT
# ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
# FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
# for more details.
# 
# You should have received a copy of the GNU General Public License
# along with Octave; see the file COPYING.  If not, write to the Free
# Software Foundation, 59 Temple Place - Suite 330, Boston, MA
# 02111-1307, USA. 

noinst_LTLIBRARIES = libpldrv.la

libpldrv_la_SOURCES = 
libpldrv_la_LIBADD = @DRIVERS@
libpldrv_la_DEPENDENCIES = @DRIVERS@
EXTRA_libpldrv_la_SOURCES = \
  dg300.c \
  gd.c \
  gnome.c \
  hpgl.c \
  impress.c \
  linuxvga.c \
  ljii.c \
  ljiip.c \
  next.c \
  null.c \
  pbm.c \
  plbuf.c \
  plmeta.c \
  ps.c \
  tek.c \
  tk.c \
  xfig.c \
  xwin.c

# what
EXTRA_LTLIBRARIES = xwin.la
# to
pkglib_LTLIBRARIES = @MODULES@
# from
xwin_la_SOURCES = xwin.c
# how
INCLUDES = @X_CFLAGS@
xwin_la_LDFLAGS = -rpath $(pkglibdir) -no-undefined -module -avoid-version 
xwin_la_LIBADD = @X_LIBS@ -lX11
