# src/Makefile.am for PLplot
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

if with_double
lib_LTLIBRARIES = libplplotd.la
else
lib_LTLIBRARIES = libplplot.la
endif

version = -version-info @SOVERSION@

sources = \
  pdfutils.c \
  plargs.c \
  plbox.c \
  plcont.c \
  plcore.c \
  plctrl.c \
  plcvt.c \
  pldtik.c \
  plfill.c \
  plhist.c \
  plline.c \
  plmap.c \
  plot3d.c \
  plpage.c \
  plsdef.c \
  plshade.c \
  plstripc.c \
  plsym.c \
  pltick.c \
  plvpor.c \
  plwind.c

libplplot_la_SOURCES  = $(sources)
libplplotd_la_SOURCES = $(sources)

libplplot_la_LDFLAGS  = $(version)
libplplotd_la_LDFLAGS = $(version)

libdrv = ../drivers/libpldrv.la

libplplot_la_LIBADD  = $(libdrv)
libplplotd_la_LIBADD = $(libdrv)