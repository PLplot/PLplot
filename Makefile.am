if enable_dyndrivers
SUBDIRS = libltdl src include lib bindings drivers examples test utils
else
SUBDIRS = libltdl drivers src include lib bindings examples test utils
endif

bin_SCRIPTS = plplot_libtool

plplot_libtool: libtool
	cp libtool plplot_libtool

docdir = $(datadir)/doc/plplot

doc_DATA = \
	AUTHORS \
	COPYING \
	COPYING.LIB \
	ChangeLog \
	Copyright \
	FAQ \
	NEWS \
	PROBLEMS \
	README

CLEANFILES = plplot_libtool
