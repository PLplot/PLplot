##############################################################################
#
# Makefile for PLplot under Unix.
#
# This just cd's to the tmp directory and runs make from there, for people
# who prefer to run configure and make from the root directory ala GNU
# software.  As such, it only supports the most commonly used targets in
# tmp/Makefile.  It would sure be nice if make had a way to pass target
# info to recursive makes.
#
# Note: if the default configure settings are acceptable, then you can just
# type "make" and configure will be run for you.  In general however it is
# best to run configure yourself to ensure all options are set correctly.
#
##############################################################################

default:	tmp/Makefile
	cd tmp; $(MAKE) default

libs:		tmp/Makefile
	cd tmp; $(MAKE) libs

x01c:		tmp/Makefile
	cd tmp; $(MAKE) x01c

x01f:		tmp/Makefile
	cd tmp; $(MAKE) x01f

cdemos:		tmp/Makefile
	cd tmp; $(MAKE) cdemos

fdemos:		tmp/Makefile
	cd tmp; $(MAKE) fdemos

tkdemos:	tmp/Makefile
	cd tmp; $(MAKE) tkdemos

install:	tmp/Makefile
	cd tmp; $(MAKE) install

clean:		tmp/Makefile
	cd tmp; $(MAKE) clean

realclean:	tmp/Makefile
	cd tmp; $(MAKE) realclean

makefile:	tmp/Makefile
	cd tmp; $(MAKE) makefile

tmp/Makefile:
	./configure
