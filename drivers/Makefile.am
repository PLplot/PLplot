# drivers/Makefile.am for PLplot
###
### Process this file with automake to produce Makefile.in
###
# Copyright (C) 2002 Rafael Laboissiere
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

noinst_LTLIBRARIES = libpldrv.la

libpldrv_la_SOURCES = 
libpldrv_la_LIBADD = @STATIC_DRIVERS@
libpldrv_la_DEPENDENCIES = @STATIC_DRIVERS@
EXTRA_libpldrv_la_SOURCES = \
  null.c \
  plmeta.c \
  ps.c \
  tek.c \
  xfig.c

# what
if with_double
   EXTRA_LTLIBRARIES = xwind_drv.la psd_drv.la
else
   EXTRA_LTLIBRARIES = xwin_drv.la ps_drv.la
endif
# to
driversdir = ${prefix}/@DATA_DIR@/../drivers
drivers_LTLIBRARIES = @MODULES@
if with_double
   drivers_DATA = driversd.db
   libplplot = ../src/libplplotd.la
else
   drivers_DATA = drivers.db
   libplplot = ../src/libplplot.la
endif

# All -I compile flags apparently must be lumped together for all source
# in this directory.
AM_CPPFLAGS = @X_CFLAGS@ @INCLTDL@

# Single precision versions of everything
xwin_drv_la_SOURCES = xwin.c
#xwin_drv_la_INCLUDES = @X_CFLAGS@
xwin_drv_la_LDFLAGS = -rpath $(driversdir) -no-undefined -module -avoid-version 
xwin_drv_la_LIBADD = $(libplplot) @X_LIBS@ -lX11
ps_drv_la_SOURCES = ps.c
ps_drv_la_LDFLAGS = -rpath $(driversdir) -no-undefined -module -avoid-version 
ps_drv_la_LIBADD = $(libplplot)

# Double precision versions of everything
xwind_drv_la_SOURCES = $(xwin_drv_la_SOURCES)
#xwind_drv_la_INCLUDES = $(xwin_drv_la_INCLUDES)
xwind_drv_la_LDFLAGS = $(xwin_drv_la_LDFLAGS)
xwind_drv_la_LIBADD = $(xwin_drv_la_LIBADD)
psd_drv_la_SOURCES = $(ps_drv_la_SOURCES)
psd_drv_la_LDFLAGS = $(ps_drv_la_LDFLAGS)
psd_drv_la_LIBADD = $(ps_drv_la_LIBADD)
