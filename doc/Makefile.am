core_manpages = plm2gif.1 plpr.1 pltek.1 pstex2eps.1 plplot_libtool.1
tcl_manpages  = plrender.1 plserver.1 pltcl.1

manpages = $(core_manpages)
if enable_tcl
manpages += $(tcl_manpages)
endif

man_MANS = $(manpages)

EXTRA_DIST = \
  $(core_manpages) $(tcl_manpages) \
  README.local Xauthority abs.txt

SUBDIRS = docbook
