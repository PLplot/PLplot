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

if enable_dyndrivers

EXTRA_LTLIBRARIES = \
	cgm.la \
	dg300.la \
	gd.la \
	gnome.la \
	hpgl.la \
	impress.la \
	linuxvga.la \
	ljii.la \
	ljiip.la \
	mem.la \
	ntk.la \
	null.la \
	pbm.la \
	plmeta.la \
	ps.la \
	pstex.la \
	tek.la \
	tk.la \
	tkwin.la \
	xfig.la \
	xwin.la

driversdir = ${prefix}/$(DATA_DIR)/$(DRV_DIR)
drivers_LTLIBRARIES = $(DYNAMIC_DRIVERS)

# The variable below will force building of the dyndrivers info files
# (<driver>.rc) and their installation in the drivers directory

drivers_DATA = $(drivers_LTLIBRARIES:.la=.rc)

# get-drv-info is a program that, at build time, will generate the driver's
# info files <driver>.rc

noinst_PROGRAMS = get-drv-info
get_drv_info_SOURCES = get-drv-info.c
get_drv_info_LDADD = $(LIBLTDL)
get_drv_info_CFLAGS = $(INCLTDL)

%.rc: %.la get-drv-info
	./get-drv-info $(<:.la=) > $@

libplplot = ../src/libplplot$(LIB_TAG).la
libplplottcltk = ../bindings/tcl/libplplottcltk$(LIB_TAG).la

# All -I preprocessor flags apparently must be lumped together for all source
# in this directory.  Some day when automake grows up, convert these
# to xwin_la_AM_CPPFLAGS, etc.  For now, use individual CFLAGS instead
# which strictly speaking is a misuse since CFLAGS are supposed to be
# processor options, not preprocessor.

cgm_la_CFLAGS = $(CDINCCMD)
gd_la_CFLAGS = $(GDINCCMD)
gnome_la_CFLAGS = $(GTK_CFLAGS) $(GNOME_INCLUDEDIR)
ntk_la_CFLAGS = \
	-I../bindings/tcl \
	-I../bindings/tk \
	$(TCLINCCMD) \
	$(TKINCCMD)

tk_la_CFLAGS = \
	-I../bindings/tcl \
	-I../bindings/tk \
	$(TCLINCCMD) \
	$(TKINCCMD)

tkwin_la_CFLAGS = \
	-I../bindings/tcl \
	-I../bindings/tk \
	-I../bindings//tk-x-plat \
	$(TCLINCCMD) \
	$(TKINCCMD)

xwin_la_CFLAGS = $(X_CFLAGS)

# Set overall preprocessor options
AM_CPPFLAGS = $(FREETYPEINCCMD) $(INCLTDL)

ldflags = -rpath $(driversdir) -no-undefined -module -avoid-version \
          $(FREETYPELIBCMD)

# Single precision versions of the dynamic drivers.
# The LDFLAGS variables have been set very carefully for each driver
# to insure all their undefined symbols are resolved by the link step

cgm_la_SOURCES = cgm.c
cgm_la_LDFLAGS = $(ldflags) $(libplplot) $(CDLIBCMD)

dg300_la_SOURCES = dg300.c
dg300_la_LDFLAGS = $(ldflags) $(libplplot)

gd_la_SOURCES = gd.c
gd_la_LDFLAGS = $(ldflags) $(libplplot) $(GDLIBCMD)

if enable_gnome
lmflag = -lm
gnome_la_SOURCES = gnome.c
gnome_la_LDFLAGS = $(ldflags) $(libplplot) $(GNOMELIBS) $(lmflag)
endif

hpgl_la_SOURCES = hpgl.c
hpgl_la_LDFLAGS = $(ldflags) $(libplplot)

impress_la_SOURCES = impress.c
impress_la_LDFLAGS = $(ldflags) $(libplplot)

linuxvga_la_SOURCES = linuxvga.c
linuxvga_la_LDFLAGS = $(ldflags) $(libplplot) $(LINUXVGALIBS)

if enable_ljii
lmflag = -lm
ljii_la_SOURCES = ljii.c
ljii_la_LDFLAGS = $(ldflags) $(libplplot) $(lmflag)
endif

if enable_ljiip
lmflag = -lm
ljiip_la_SOURCES = ljiip.c
ljiip_la_LDFLAGS = $(ldflags) $(libplplot) $(lmflag)
endif

mem_la_SOURCES = mem.c
mem_la_LDFLAGS = $(ldflags) $(libplplot)

ntk_la_SOURCES = ntk.c
ntk_la_LDFLAGS = $(ldflags) $(libplplot) $(TCLLIBCMD) $(TKLIBCMD)

null_la_SOURCES = null.c
null_la_LDFLAGS = $(ldflags) $(libplplot)

if enable_pbm
lmflag = -lm
pbm_la_SOURCES = pbm.c
pbm_la_LDFLAGS = $(ldflags) $(libplplot) $(lmflag)
endif

plmeta_la_SOURCES = plmeta.c
plmeta_la_LDFLAGS = $(ldflags) $(libplplot)

if enable_ps
lmflag = -lm
ps_la_SOURCES = ps.c
ps_la_LDFLAGS = $(ldflags) $(libplplot) $(lmflag)
endif

if enable_pstex
lmflag = -lm
pstex_la_SOURCES = ps.c pstex.c
pstex_la_LDFLAGS = $(ldflags) $(libplplot) $(lmflag)
endif

tek_la_SOURCES = tek.c
tek_la_LDFLAGS = $(ldflags) $(libplplot)

tk_la_SOURCES = tk.c
tk_la_LDFLAGS = $(ldflags) $(libplplottcltk) $(TCLLIBCMD) $(TKLIBCMD)

tkwin_la_SOURCES = \
	tkwin.c \
	../bindings/tk-x-plat/Plplotter_Init.c \
	../bindings/tk-x-plat/plplotter.c

tkwin_la_LDFLAGS = $(ldflags) $(libplplottcltk) $(TCLLIBCMD) $(TKLIBCMD)

if enable_xfig
lmflag = -lm
xfig_la_SOURCES = xfig.c
xfig_la_LDFLAGS = $(ldflags) $(libplplot) $(lmflag)
endif

if enable_xwin
xflags = $(X_LIBS) -lX11 $(PTHREADLIBS)
lmflag = -lm
xwin_la_SOURCES = xwin.c
xwin_la_LDFLAGS = $(ldflags) $(libplplot) $(xflags) $(lmflag)
endif

# end of enable_dyndrivers
else
# start of static driver alternative which is implemented using
# a convenience library to be included in libplplot.

noinst_LTLIBRARIES = libplplotdrv.la

if enable_tk
# All source that is in libplplottcltk.
tksources = \
	../bindings/tcl/tclAPI.c \
	../bindings/tcl/tclMain.c \
	../bindings/tk/Pltk_Init.c \
	../bindings/tk/plframe.c \
	../bindings/tk/plr.c \
	../bindings/tk/tcpip.c \
	../bindings/tk/tkMain.c

endif

# This is an effective duplicate of what occurs in bindings/tcl to generate
# include files for tclAPI.c.  So this duplicate work required duplicate
# maintenance, but this is required because of the special nature of static
# drivers and their linking constraints.

plapi.tpl: ../bindings/tcl/plapi.tpl
	cp ../bindings/tcl/plapi.tpl plapi.tpl

tclcmd.tpl: ../bindings/tcl/tclcmd.tpl
	cp ../bindings/tcl/tclcmd.tpl tclcmd.tpl

tclgen.h tclgen_s.h tclgen.c : ../bindings/tcl/pltclgen plapi.tpl tclcmd.tpl
	perl ../bindings/tcl/pltclgen

../bindings/tcl/tclAPI.c: tclgen.h tclgen_s.h tclgen.c

if enable_tcl
# All source that is in libtclmatrix.
matrixsources = \
	../bindings/tcl/tclMatrix.c \
	../bindings/tcl/matrixInit.c
endif

if enable_tkwin

if enable_tk
# These additions required for tkwin driver if already have additions for
# tk driver in place.  These correspond to objects normally included in 
# the tkwin driver in any case.

tkwinsources = \
	../bindings/tk-x-plat/Plplotter_Init.c \
	../bindings/tk-x-plat/plplotter.c
else
# tkwin driver on its own without tk driver (but tcl must be enabled so
# that in this case the matrix source is already in.  Thus, only need to
# put in the tksource plus the two extra above.

tkwinsources = \
	../bindings/tcl/tclAPI.c \
	../bindings/tcl/tclMain.c \
	../bindings/tk/Pltk_Init.c \
	../bindings/tk/plframe.c \
	../bindings/tk/plr.c \
	../bindings/tk/tcpip.c \
	../bindings/tk/tkMain.c \
	../bindings/tk-x-plat/Plplotter_Init.c \
	../bindings/tk-x-plat/plplotter.c

endif
endif

# For AM_CONDITIONALS must use SOURCES, not LIBADD
libplplotdrv_la_SOURCES = $(tksources) $(matrixsources) $(tkwinsources)
EXTRA_libplplotdrv_la_SOURCES = \
	cgm.c \
	dg300.c \
	gd.c \
	gnome.c \
	hpgl.c \
	impress.c \
	linuxvga.c \
	ljii.c \
	ljiip.c \
	mem.c \
	ntk.c \
	null.c \
	pbm.c \
	plmeta.c \
	ps.c \
	pstex.c \
	tek.c \
	tk.c \
	tkwin.c \
	xfig.c \
	xwin.c \
	$(extra_tksources) \
	$(extra_tkwinsources)

# All -I preprocessor flags apparently must be lumped together for all source
# in this directory.  Some day when automake grows up, convert these
# to xwin_lo_AM_CPPFLAGS, etc.  For now, use individual CFLAGS instead
# which strictly speaking is a misuse since CFLAGS are supposed to be
# processor options, not preprocessor.
# Double precision versions not needed since these flags are currently
# ignored by automake so the approach must be to concatanate them
# together to form one giant lump libplplotdrv_la_CFLAGS
# (which is going to slow the build down with the enormous number of
# gnome compiler flags, but those who use static drivers get what they
# deserve....;-))

cgm_lo_CFLAGS = $(CDINCCMD)
gd_lo_CFLAGS = $(GDINCCMD)
gnome_lo_CFLAGS = $(GTK_CFLAGS) $(GNOME_INCLUDEDIR)
ntk_lo_CFLAGS = \
	-I../bindings/tcl \
	-I../bindings/tk \
	$(TCLINCCMD) \
	$(TKINCCMD)

tk_lo_CFLAGS = \
	-I../bindings/tcl \
	-I../bindings/tk \
	$(TCLINCCMD) \
	$(TKINCCMD)

tkwin_lo_CFLAGS = \
	-I../bindings/tcl \
	-I../bindings/tk \
	-I../bindings//tk-x-plat \
	$(TCLINCCMD) \
	$(TKINCCMD)

xwin_lo_CFLAGS = $(X_CFLAGS)

# tkwin covers ntk, tk, and tkwin
# Following line did not work so try lumped alternative with AM_CPPFLAGS
# libplplotdrv_la_CFLAGS = \
# Also add itcl compile flags just in case the tk extra objects are compiled.
AM_CPPFLAGS = \
	$(cgm_lo_CFLAGS) \
	$(gd_lo_CFLAGS) \
	$(gnome_lo_CFLAGS) \
	$(tkwin_lo_CFLAGS) \
	$(xwin_lo_CFLAGS) \
	$(ITCLINCCMD)

libplplotdrv_la_LIBADD = $(STATIC_DRIVERS) 
# Concatanate all LDFLAGS for individual drivers above together

if enable_gnome
lmflag = -lm
endif

if enable_ljii
lmflag = -lm
endif

if enable_ljiip
lmflag = -lm
endif

if enable_pbm
lmflag = -lm
endif

if enable_ps
lmflag = -lm
endif

if enable_pstex
lmflag = -lm
endif

if enable_xfig
lmflag = -lm
endif

if enable_xwin
xflags = $(X_LIBS) -lX11
lmflag = -lm
endif

libplplotdrv_la_LDFLAGS = \
	$(CDLIBCMD) \
	$(GDLIBCMD) \
	$(GNOMELIBS) \
	$(LINUXVGALIBS) \
	$(TCLLIBCMD) \
	$(TKLIBCMD) \
	$(ITCLLIBCMD) \
	$(ITKLIBCMD) \
	$(xflags) $(lmflag)

# These dependencies required for some reason if using the LIBADD method.
libplplotdrv_la_DEPENDENCIES = $(libplplotdrv_la_LIBADD)

# end of static driver case.
endif

install-data-hook:
if enable_dyndrivers
	rm -f $(DESTDIR)$(driversdir)/*.a
endif

CLEANFILES = plapi.tpl tclcmd.tpl tclgen.h tclgen_s.h tclgen.c *.rc
DISTCLEANFILES = $(drivers_DATA)


