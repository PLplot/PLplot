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
   libplplot = ../src/libplplotd.la
   libplplottcltk = ../bindings/tcl/libplplottcltkd.la
else
   drivers_DATA = drivers.db
   libplplot = ../src/libplplot.la
   libplplottcltk = ../bindings/tcl/libplplottcltk.la
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

cgm_drv_la_SOURCES = cgm.c
cgm_drv_la_LDFLAGS = $(ldflags)
cgm_drv_la_LIBADD = $(libplplot) @CDLIBCMD@

dg300_drv_la_SOURCES = dg300.c
dg300_drv_la_LDFLAGS = $(ldflags)
dg300_drv_la_LIBADD = $(libplplot)

gd_drv_la_SOURCES = gd.c
gd_drv_la_LDFLAGS = $(ldflags)
gd_drv_la_LIBADD = $(libplplot) @GDLIBCMD@

gnome_drv_la_SOURCES = gnome.c
gnome_drv_la_LDFLAGS = $(ldflags)
gnome_drv_la_LIBADD = $(libplplot) @GNOMELIBS@

hpgl_drv_la_SOURCES = hpgl.c
hpgl_drv_la_LDFLAGS = $(ldflags)
hpgl_drv_la_LIBADD = $(libplplot)

impress_drv_la_SOURCES = impress.c
impress_drv_la_LDFLAGS = $(ldflags)
impress_drv_la_LIBADD = $(libplplot)

linuxvga_drv_la_SOURCES = linuxvga.c
linuxvga_drv_la_LDFLAGS = $(ldflags)
linuxvga_drv_la_LIBADD = $(libplplot) @LINUXVGALIBS@

ljii_drv_la_SOURCES = ljii.c
ljii_drv_la_LDFLAGS = $(ldflags)
ljii_drv_la_LIBADD = $(libplplot)

ljiip_drv_la_SOURCES = ljiip.c
ljiip_drv_la_LDFLAGS = $(ldflags)
ljiip_drv_la_LIBADD = $(libplplot)

ntk_drv_la_SOURCES = ntk.c
ntk_drv_la_LDFLAGS = $(ldflags)
ntk_drv_la_LIBADD = $(libplplot) @TCLLIBCMD@ @TKLIBCMD@

null_drv_la_SOURCES = null.c
null_drv_la_LDFLAGS = $(ldflags)
null_drv_la_LIBADD = $(libplplot)

pbm_drv_la_SOURCES = pbm.c
pbm_drv_la_LDFLAGS = $(ldflags)
pbm_drv_la_LIBADD = $(libplplot)

plmeta_drv_la_SOURCES = plmeta.c
plmeta_drv_la_LDFLAGS = $(ldflags)
plmeta_drv_la_LIBADD = $(libplplot)

ps_drv_la_SOURCES = ps.c pstex.c
ps_drv_la_LDFLAGS = $(ldflags)
ps_drv_la_LIBADD = $(libplplot)

pstex_drv_la_SOURCES = ps.c pstex.c
pstex_drv_la_LDFLAGS = $(ldflags)
pstex_drv_la_LIBADD = $(libplplot)

tek_drv_la_SOURCES = tek.c
tek_drv_la_LDFLAGS = $(ldflags)
tek_drv_la_LIBADD = $(libplplot)

tk_drv_la_SOURCES = tk.c
tk_drv_la_LDFLAGS = $(ldflags)
tk_drv_la_LIBADD = $(libplplottcltk) @TCLLIBCMD@ @TKLIBCMD@

tkwin_drv_la_SOURCES = \
	tkwin.c \
	../bindings/tk-x-plat/Plplotter_Init.c \
	../bindings/tk-x-plat/plplotter.c

tkwin_drv_la_LDFLAGS = $(ldflags)
tkwin_drv_la_LIBADD = $(libplplottcltk) @TCLLIBCMD@ @TKLIBCMD@

xfig_drv_la_SOURCES = xfig.c
xfig_drv_la_LDFLAGS = $(ldflags)
xfig_drv_la_LIBADD = $(libplplot)

xwin_drv_la_SOURCES = xwin.c
xwin_drv_la_LDFLAGS = $(ldflags)
xwin_drv_la_LIBADD = $(libplplot) @X_LIBS@ -lX11

# Double precision versions of the dynamic drivers.

cgmd_drv_la_SOURCES = $(cgm_drv_la_SOURCES)
cgmd_drv_la_LDFLAGS = $(cgm_drv_la_LDFLAGS)
cgmd_drv_la_LIBADD = $(cgm_drv_la_LIBADD)

dg300d_drv_la_SOURCES = $(dg300_drv_la_SOURCES)
dg300d_drv_la_LDFLAGS = $(dg300_drv_la_LDFLAGS)
dg300d_drv_la_LIBADD = $(dg300_drv_la_LIBADD)

gdd_drv_la_SOURCES = $(gd_drv_la_SOURCES)
gdd_drv_la_LDFLAGS = $(gd_drv_la_LDFLAGS)
gdd_drv_la_LIBADD = $(gd_drv_la_LIBADD)

gnomed_drv_la_SOURCES = $(gnome_drv_la_SOURCES)
gnomed_drv_la_LDFLAGS = $(gnome_drv_la_LDFLAGS)
gnomed_drv_la_LIBADD = $(gnome_drv_la_LIBADD)

hpgld_drv_la_SOURCES = $(hpgl_drv_la_SOURCES)
hpgld_drv_la_LDFLAGS = $(hpgl_drv_la_LDFLAGS)
hpgld_drv_la_LIBADD = $(hpgl_drv_la_LIBADD)

impressd_drv_la_SOURCES = $(impress_drv_la_SOURCES)
impressd_drv_la_LDFLAGS = $(impress_drv_la_LDFLAGS)
impressd_drv_la_LIBADD = $(impress_drv_la_LIBADD)

linuxvgad_drv_la_SOURCES = $(linuxvga_drv_la_SOURCES)
linuxvgad_drv_la_LDFLAGS = $(linuxvga_drv_la_LDFLAGS)
linuxvgad_drv_la_LIBADD = $(linuxvga_drv_la_LIBADD)

ljiid_drv_la_SOURCES = $(ljii_drv_la_SOURCES)
ljiid_drv_la_LDFLAGS = $(ljii_drv_la_LDFLAGS)
ljiid_drv_la_LIBADD = $(ljii_drv_la_LIBADD)

ljiipd_drv_la_SOURCES = $(ljiip_drv_la_SOURCES)
ljiipd_drv_la_LDFLAGS = $(ljiip_drv_la_LDFLAGS)
ljiipd_drv_la_LIBADD = $(ljiip_drv_la_LIBADD)

ntkd_drv_la_SOURCES = $(ntk_drv_la_SOURCES)
ntkd_drv_la_LDFLAGS = $(ntk_drv_la_LDFLAGS)
ntkd_drv_la_LIBADD = $(ntk_drv_la_LIBADD)

nulld_drv_la_SOURCES = $(null_drv_la_SOURCES)
nulld_drv_la_LDFLAGS = $(null_drv_la_LDFLAGS)
nulld_drv_la_LIBADD = $(null_drv_la_LIBADD)

pbmd_drv_la_SOURCES = $(pbm_drv_la_SOURCES)
pbmd_drv_la_LDFLAGS = $(pbm_drv_la_LDFLAGS)
pbmd_drv_la_LIBADD = $(pbm_drv_la_LIBADD)

plmetad_drv_la_SOURCES = $(plmeta_drv_la_SOURCES)
plmetad_drv_la_LDFLAGS = $(plmeta_drv_la_LDFLAGS)
plmetad_drv_la_LIBADD = $(plmeta_drv_la_LIBADD)

psd_drv_la_SOURCES = $(ps_drv_la_SOURCES)
psd_drv_la_LDFLAGS = $(ps_drv_la_LDFLAGS)
psd_drv_la_LIBADD = $(ps_drv_la_LIBADD)

pstexd_drv_la_SOURCES = $(pstex_drv_la_SOURCES)
pstexd_drv_la_LDFLAGS = $(pstex_drv_la_LDFLAGS)
pstexd_drv_la_LIBADD = $(pstex_drv_la_LIBADD)

tekd_drv_la_SOURCES = $(tek_drv_la_SOURCES)
tekd_drv_la_LDFLAGS = $(tek_drv_la_LDFLAGS)
tekd_drv_la_LIBADD = $(tek_drv_la_LIBADD)

tkd_drv_la_SOURCES = $(tk_drv_la_SOURCES)
tkd_drv_la_LDFLAGS = $(tk_drv_la_LDFLAGS)
tkd_drv_la_LIBADD = $(tk_drv_la_LIBADD)

tkwind_drv_la_SOURCES = $(tkwin_drv_la_SOURCES)
tkwind_drv_la_LDFLAGS = $(tkwin_drv_la_LDFLAGS)
tkwind_drv_la_LIBADD = $(tkwin_drv_la_LIBADD)

xfigd_drv_la_SOURCES = $(xfig_drv_la_SOURCES)
xfigd_drv_la_LDFLAGS = $(xfig_drv_la_LDFLAGS)
xfigd_drv_la_LIBADD = $(xfig_drv_la_LIBADD)

xwind_drv_la_SOURCES = $(xwin_drv_la_SOURCES)
xwind_drv_la_LDFLAGS = $(xwin_drv_la_LDFLAGS)
xwind_drv_la_LIBADD = $(xwin_drv_la_LIBADD)

#enable_dyndrivers
endif
