##############################################################################
#
# Makefile for plplot on the Amiga.
# You have to build the makefile explicitly, e.g.
#
#	m4 [flags] <makefile.m4 >makefile
#
# You no longer have to specify -DAMIGA; this define is made in
# cf_amiga.m4. Eventually a rexx configure script should be written.
#
# $Id$
#
# $Log$
# Revision 1.2  1994/05/23 22:11:09  mjl
# Hack to allow include files to be found.
#
# Revision 1.1  1993/10/21  19:31:20  mjl
# Initial checkin.  Based on the Unix makefile.m4.
#
##############################################################################

# Set up the base m4 macros.

include(cf_basic.m4)

# Set up local definitions
# Note: these are all in cf_amiga.m4 for now.

# Read config files and other setup

include(cf_amiga.m4)

##############################################################################

# Dependent directories
# Note the include dir should be specified since search path resolution
# with SAS/C 6.x doesn't seem to work with softlinked files.

PLINC_DIR = /include
PLLIB_DIR = /lib
PLFNT_DIR = /lib
PLLIB_PATH = $(PLLIB_DIR)/
PLFNT_PATH = $(PLFNT_DIR)/

# The main plplot library.  Used in dependencies.

ifelse(MATH,FFP,{
PLLIB		= $(PLLIB_PATH)plplotffp.lib
})
ifelse(MATH,IEEEF,{
PLLIB		= $(PLLIB_PATH)plplotf.lib
})
ifelse(MATH,IEEED,{
PLLIB		= $(PLLIB_PATH)plplotd.lib
})

# This is what we link against.

PLLIB_LDC	= lib $(PLLIB) $(LIBM) $(LIBC) \
		  lib:reqtools.lib 

# Include dependencies and main targets.

include(cf_depend.m4)

##############################################################################

# "install" target
#
# You're on you own here.  Under Unix I typically put plplot in
# /usr/local/plplot, or $HOME/local/plplot if I cannot get a directory in
# /usr/local assigned to me.
#
# On the Amiga, there are far more choices.
# I have lib: directory that is assigned to usr:lib (local libraries) +
# sc:lib (SAS/C libraries) that I put libplplot*.lib in.  Or perhaps give
# the plplot libraries + plrender their own directory, e.g. usr:plplot.
# A good place for the fonts is in fonts:plplot.

##############################################################################

