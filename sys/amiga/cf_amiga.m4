##############################################################################
#
# Setup macros for the Amiga.
#
# $Id$
#
# Note this makefile for the PD make utility ported by Steve Walton.
# The 'make links' command is still required, although here it just does
# file copies since soft links not yet officially supported by the OS.
# Alternately, if you are an SKsh user, you can use the 'makelinks' script
# to do actual links.
#
##############################################################################

define({AMIGA},)dnl

PLLIB_DIR = /lib
PLFNT_DIR = /lib
PLLIB_PATH = $(PLLIB_DIR)/
PLFNT_PATH = $(PLFNT_DIR)/

SYS_LIBS=

# Switch on floating point format.  Choices: 
#	FFP	Motorola fast floating point
#	IEEEF	IEEE floating point, using libs:mathieeedoubbas.library
#	IEEED	As above, but double precision
#
# Default is: IEEEF

ifdef({MATH},,define(MATH,IEEEF))
ifdef({COMPILER},,define(COMPILER,SAS6.X))

#-------- Compiler/linker macros for Lattice C 5.x. -------------------------

ifelse(COMPILER,LATTICE5.X,{

#CC		= sc5
#LDC		= slink 
CC		= lc
LDC		= blink 
LIBC		= lib:lc.lib lib:amiga.lib

if_debug({
DEBUG_FLAG_C	= -d2
})

if_opt({
OPT_FLAG_C	= -O
})

ifelse(MATH,FFP,{
MATHFLAGS	= -ff
LIBM		= lib:lcmffp.lib
DBL_FLAG_C	=
PLLIB_MAIN	= $(PLLIB_PATH)plplotffp.lib
})

ifelse(MATH,IEEEF,{
MATHFLAGS	= -fi
LIBM		= lib:lcmieee.lib
DBL_FLAG_C	= 
PLLIB_MAIN	= $(PLLIB_PATH)plplotf.lib
})

ifelse(MATH,IEEED,{
MATHFLAGS	= -fi
LIBM		= lib:lcmieee.lib
DBL_FLAG_C	= -DDOUBLE_PREC
PLLIB_MAIN	= $(PLLIB_PATH)plplotd.lib
})

CFLAGS		= $(MATHFLAGS) $(DBL_FLAG_C) -v $(OPT_FLAG_C) \
		  $(DEBUG_FLAG_C) -j89i

PLDEVICES	= -DPLMETA -DNULLDEV -DTEK4010 -DTEK4107 -DPS -DLJII \
	    	  -DHP7470 -DHP7580 -DIMP -DIFF
})

#-------- Compiler/linker macros for SAS C 6.x. -----------------------------

ifelse(COMPILER,SAS6.X,{

CC		= sc
LDC		= slink 
LIBC		= lib:sc.lib lib:amiga.lib

if_debug({
DEBUG_FLAG_C	= dbg=symbol
})

if_opt({
OPT_FLAG_C	= optimize
})

define(GST,{plplot.gst})

ifelse(MATH,FFP,{
MATHFLAGS	= math=ffp
LIBM		= lib:scmffp.lib
DBL_FLAG_C	=
PLLIB_MAIN	= $(PLLIB_PATH)plplotffp.lib
})

ifelse(MATH,IEEEF,{
MATHFLAGS	= math=ieee
LIBM		= lib:scmieee.lib
DBL_FLAG_C	= 
PLLIB_MAIN	= $(PLLIB_PATH)plplotf.lib
})

ifelse(MATH,IEEED,{
MATHFLAGS	= math=ieee
LIBM		= lib:scmieee.lib
DBL_FLAG_C	= -DDOUBLE_PREC
PLLIB_MAIN	= $(PLLIB_PATH)plplotd.lib
})

CFLAGS		= $(MATHFLAGS) $(DBL_FLAG_C) nover $(DEBUG_FLAG_C) \
		  $(OPT_FLAG_C) ign=84,89,147,161 ifdef({GST},{gst=GST})

# Sigh

PLDEVICES 	= def PLMETA def NULLDEV def PS def IFF

})

LDCFLAGS	= 
STARTUP 	= lib:c.o
TO		= to

F77		= f77
FFLAGS		= -c
LDF		= $(F77)
LDFFLAGS	= 
LN		= copy

PLLIB_C		= $(PLLIB_MAIN)
PLLIB_LDC	= lib $(PLLIB_C) $(LIBM) $(LIBC) \
		  lib:reqtools.lib 
