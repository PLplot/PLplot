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

if with_double
EXTRA_LTLIBRARIES = \
	cgmd_drv.la \
	dg300d_drv.la \
	gdd_drv.la \
	gnomed_drv.la \
	hpgld_drv.la \
	impressd_drv.la \
	linuxvgad_drv.la \
	ljiid_drv.la \
	ljiipd_drv.la \
	ntkd_drv.la \
	nulld_drv.la \
	pbmd_drv.la \
	plmetad_drv.la \
	psd_drv.la \
	pstexd_drv.la \
	tekd_drv.la \
	tkd_drv.la \
	tkwind_drv.la \
	xfigd_drv.la \
	xwind_drv.la

else
EXTRA_LTLIBRARIES = \
	cgm_drv.la \
	dg300_drv.la \
	gd_drv.la \
	gnome_drv.la \
	hpgl_drv.la \
	impress_drv.la \
	linuxvga_drv.la \
	ljii_drv.la \
	ljiip_drv.la \
	ntk_drv.la \
	null_drv.la \
	pbm_drv.la \
	plmeta_drv.la \
	ps_drv.la \
	pstex_drv.la \
	tek_drv.la \
	tk_drv.la \
	tkwin_drv.la \
	xfig_drv.la \
	xwin_drv.la

endif

driversdir = ${prefix}/@DATA_DIR@/../drivers
drivers_LTLIBRARIES = @DYNAMIC_DRIVERS@

if with_double
drivers_DATA = driversd.db
libplplot = -L../src -lplplotd
libplplottcltk = -L../bindings/tcl -lplplottcltkd
else
drivers_DATA = drivers.db
libplplot = -L../src -lplplot
libplplottcltk = -L../bindings/tcl -lplplottcltk
endif

# All -I preprocessor flags apparently must be lumped together for all source
# in this directory.  Some day when automake grows up, convert these
# to xwin_drv_la_AM_CPPFLAGS, etc.  For now, use individual CFLAGS instead
# which strictly speaking is a misuse since CFLAGS are supposed to be
# processor options, not preprocessor.

cgm_drv_la_CFLAGS = @CDINCCMD@
cgmd_drv_la_CFLAGS = $(cgm_drv_la_CFLAGS)
gd_drv_la_CFLAGS = @GDINCCMD@
gdd_drv_la_CFLAGS = $(gd_drv_la_CFLAGS)
gnome_drv_la_CFLAGS = @GTK_CFLAGS@ @GNOME_INCLUDEDIR@
gnomed_drv_la_CFLAGS = $(gnome_drv_la_CFLAGS)
ntk_drv_la_CFLAGS = \
	-I../bindings/tcl \
	-I../bindings/tk \
	@TCLINCCMD@ \
	@TKINCCMD@

ntkd_drv_la_CFLAGS = $(ntk_drv_la_CFLAGS)
tk_drv_la_CFLAGS = \
	-I../bindings/tcl \
	-I../bindings/tk \
	@TCLINCCMD@ \
	@TKINCCMD@

tkd_drv_la_CFLAGS = $(tk_drv_la_CFLAGS)
tkwin_drv_la_CFLAGS = \
	-I../bindings/tcl \
	-I../bindings/tk \
	-I../bindings//tk-x-plat \
	@TCLINCCMD@ \
	@TKINCCMD@

tkwind_drv_la_CFLAGS = $(tkwin_drv_la_CFLAGS)
xwin_drv_la_CFLAGS = @X_CFLAGS@
xwind_drv_la_CFLAGS = $(xwin_drv_la_CFLAGS)

# Set overall preprocessor options
AM_CPPFLAGS = @INCLTDL@

ldflags = -rpath $(driversdir) -no-undefined -module -avoid-version 

# Single precision versions of the dynamic drivers.
# The LDFLAGS variables have been set very carefully for each driver
# to insure all their undefined symbols are resolved by the link step

cgm_drv_la_SOURCES = cgm.c
cgm_drv_la_LDFLAGS = $(ldflags) $(libplplot) @CDLIBCMD@

dg300_drv_la_SOURCES = dg300.c
dg300_drv_la_LDFLAGS = $(ldflags) $(libplplot)

gd_drv_la_SOURCES = gd.c
gd_drv_la_LDFLAGS = $(ldflags) $(libplplot) @GDLIBCMD@

if enable_gnome
lmflag = -lm
gnome_drv_la_SOURCES = gnome.c
gnome_drv_la_LDFLAGS = $(ldflags) $(libplplot) @GNOMELIBS@ $(lmflag)
endif

hpgl_drv_la_SOURCES = hpgl.c
hpgl_drv_la_LDFLAGS = $(ldflags) $(libplplot)

impress_drv_la_SOURCES = impress.c
impress_drv_la_LDFLAGS = $(ldflags) $(libplplot)

linuxvga_drv_la_SOURCES = linuxvga.c
linuxvga_drv_la_LDFLAGS = $(ldflags) $(libplplot) @LINUXVGALIBS@

if enable_ljii
lmflag = -lm
ljii_drv_la_SOURCES = ljii.c
ljii_drv_la_LDFLAGS = $(ldflags) $(libplplot) $(lmflag)
endif

if enable_ljiip
lmflag = -lm
ljiip_drv_la_SOURCES = ljiip.c
ljiip_drv_la_LDFLAGS = $(ldflags) $(libplplot) $(lmflag)
endif

ntk_drv_la_SOURCES = ntk.c
ntk_drv_la_LDFLAGS = $(ldflags) $(libplplot) @TCLLIBCMD@ @TKLIBCMD@

null_drv_la_SOURCES = null.c
null_drv_la_LDFLAGS = $(ldflags) $(libplplot)

if enable_pbm
lmflag = -lm
pbm_drv_la_SOURCES = pbm.c
pbm_drv_la_LDFLAGS = $(ldflags) $(libplplot) $(lmflag)
endif

plmeta_drv_la_SOURCES = plmeta.c
plmeta_drv_la_LDFLAGS = $(ldflags) $(libplplot)

if enable_ps
lmflag = -lm
ps_drv_la_SOURCES = ps.c pstex.c
ps_drv_la_LDFLAGS = $(ldflags) $(libplplot) $(lmflag)
endif

if enable_pstex
lmflag = -lm
pstex_drv_la_SOURCES = ps.c pstex.c
pstex_drv_la_LDFLAGS = $(ldflags) $(libplplot) $(lmflag)
endif

tek_drv_la_SOURCES = tek.c
tek_drv_la_LDFLAGS = $(ldflags) $(libplplot)

tk_drv_la_SOURCES = tk.c
tk_drv_la_LDFLAGS = $(ldflags) $(libplplottcltk) @TCLLIBCMD@ @TKLIBCMD@

tkwin_drv_la_SOURCES = \
	tkwin.c \
	../bindings/tk-x-plat/Plplotter_Init.c \
	../bindings/tk-x-plat/plplotter.c

tkwin_drv_la_LDFLAGS = $(ldflags) $(libplplottcltk) @TCLLIBCMD@ @TKLIBCMD@

if enable_xfig
lmflag = -lm
xfig_drv_la_SOURCES = xfig.c
xfig_drv_la_LDFLAGS = $(ldflags) $(libplplot) $(lmflag)
endif

if enable_xwin
xflags = @X_LIBS@ -lX11
lmflag = -lm
xwin_drv_la_SOURCES = xwin.c
xwin_drv_la_LDFLAGS = $(ldflags) $(libplplot) $(xflags) $(lmflag)
endif

# Double precision versions of the dynamic drivers.

cgmd_drv_la_SOURCES = $(cgm_drv_la_SOURCES)
cgmd_drv_la_LDFLAGS = $(cgm_drv_la_LDFLAGS)

dg300d_drv_la_SOURCES = $(dg300_drv_la_SOURCES)
dg300d_drv_la_LDFLAGS = $(dg300_drv_la_LDFLAGS)

gdd_drv_la_SOURCES = $(gd_drv_la_SOURCES)
gdd_drv_la_LDFLAGS = $(gd_drv_la_LDFLAGS)

gnomed_drv_la_SOURCES = $(gnome_drv_la_SOURCES)
gnomed_drv_la_LDFLAGS = $(gnome_drv_la_LDFLAGS)

hpgld_drv_la_SOURCES = $(hpgl_drv_la_SOURCES)
hpgld_drv_la_LDFLAGS = $(hpgl_drv_la_LDFLAGS)

impressd_drv_la_SOURCES = $(impress_drv_la_SOURCES)
impressd_drv_la_LDFLAGS = $(impress_drv_la_LDFLAGS)

linuxvgad_drv_la_SOURCES = $(linuxvga_drv_la_SOURCES)
linuxvgad_drv_la_LDFLAGS = $(linuxvga_drv_la_LDFLAGS)

ljiid_drv_la_SOURCES = $(ljii_drv_la_SOURCES)
ljiid_drv_la_LDFLAGS = $(ljii_drv_la_LDFLAGS)

ljiipd_drv_la_SOURCES = $(ljiip_drv_la_SOURCES)
ljiipd_drv_la_LDFLAGS = $(ljiip_drv_la_LDFLAGS)

ntkd_drv_la_SOURCES = $(ntk_drv_la_SOURCES)
ntkd_drv_la_LDFLAGS = $(ntk_drv_la_LDFLAGS)

nulld_drv_la_SOURCES = $(null_drv_la_SOURCES)
nulld_drv_la_LDFLAGS = $(null_drv_la_LDFLAGS)

pbmd_drv_la_SOURCES = $(pbm_drv_la_SOURCES)
pbmd_drv_la_LDFLAGS = $(pbm_drv_la_LDFLAGS)

plmetad_drv_la_SOURCES = $(plmeta_drv_la_SOURCES)
plmetad_drv_la_LDFLAGS = $(plmeta_drv_la_LDFLAGS)

psd_drv_la_SOURCES = $(ps_drv_la_SOURCES)
psd_drv_la_LDFLAGS = $(ps_drv_la_LDFLAGS)

pstexd_drv_la_SOURCES = $(pstex_drv_la_SOURCES)
pstexd_drv_la_LDFLAGS = $(pstex_drv_la_LDFLAGS)

tekd_drv_la_SOURCES = $(tek_drv_la_SOURCES)
tekd_drv_la_LDFLAGS = $(tek_drv_la_LDFLAGS)

tkd_drv_la_SOURCES = $(tk_drv_la_SOURCES)
tkd_drv_la_LDFLAGS = $(tk_drv_la_LDFLAGS)

tkwind_drv_la_SOURCES = $(tkwin_drv_la_SOURCES)
tkwind_drv_la_LDFLAGS = $(tkwin_drv_la_LDFLAGS)

xfigd_drv_la_SOURCES = $(xfig_drv_la_SOURCES)
xfigd_drv_la_LDFLAGS = $(xfig_drv_la_LDFLAGS)

xwind_drv_la_SOURCES = $(xwin_drv_la_SOURCES)
xwind_drv_la_LDFLAGS = $(xwin_drv_la_LDFLAGS)

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

cgm_lo_CFLAGS = @CDINCCMD@
#cgmd_lo_CFLAGS = $(cgm_lo_CFLAGS)
gd_lo_CFLAGS = @GDINCCMD@
#gdd_lo_CFLAGS = $(gd_lo_CFLAGS)
gnome_lo_CFLAGS = @GTK_CFLAGS@ @GNOME_INCLUDEDIR@
#gnomed_lo_CFLAGS = $(gnome_lo_CFLAGS)
ntk_lo_CFLAGS = \
	-I../bindings/tcl \
	-I../bindings/tk \
	@TCLINCCMD@ \
	@TKINCCMD@

#ntkd_lo_CFLAGS = $(ntk_lo_CFLAGS)
tk_lo_CFLAGS = \
	-I../bindings/tcl \
	-I../bindings/tk \
	@TCLINCCMD@ \
	@TKINCCMD@

#tkd_lo_CFLAGS = $(tk_lo_CFLAGS)
tkwin_lo_CFLAGS = \
	-I../bindings/tcl \
	-I../bindings/tk \
	-I../bindings//tk-x-plat \
	@TCLINCCMD@ \
	@TKINCCMD@

#tkwind_lo_CFLAGS = $(tkwin_lo_CFLAGS)
xwin_lo_CFLAGS = @X_CFLAGS@
#xwind_lo_CFLAGS = $(xwin_lo_CFLAGS)

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
	@ITCLINCCMD@

libplplotdrv_la_LIBADD = @STATIC_DRIVERS@ 
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
xflags = @X_LIBS@ -lX11
lmflag = -lm
endif

libplplotdrv_la_LDFLAGS = \
	@CDLIBCMD@ \
	@GDLIBCMD@ \
	@GNOMELIBS@ \
	@LINUXVGALIBS@ \
	@TCLLIBCMD@ \
	@TKLIBCMD@ \
	@ITCLLIBCMD@ \
	@ITKLIBCMD@ \
	$(xflags) $(lmflag)

# These dependencies required for some reason if using the LIBADD method.
libplplotdrv_la_DEPENDENCIES = $(libplplotdrv_la_LIBADD)

# end of static driver case.
endif
CLEANFILES = plapi.tpl tclcmd.tpl tclgen.h tclgen_s.h tclgen.c 
