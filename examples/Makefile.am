SUBDIRS =  c  c++  f77 python tcl tk java perl

examplesdir = $(DATA_DIR)/examples

examples_DATA = Makefile.examples

install-data-hook:
	( cd $(DESTDIR)/$(examplesdir) ; mv -f Makefile.examples Makefile )

EXTRA_DIST = Makefile.examples.in
