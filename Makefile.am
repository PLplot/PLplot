if enable_dyndrivers
SUBDIRS = libltdl src include lib bindings drivers examples test utils
else
SUBDIRS = libltdl drivers src include lib bindings examples test utils
endif

bin_SCRIPTS = plplot_libtool

plplot_libtool: libtool
	cp libtool plplot_libtool

CLEANFILES = plplot_libtool
