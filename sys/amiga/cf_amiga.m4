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

SYS_LIBS=

# Switch on floating point format.  Choices: 
#	FFP	Motorola fast floating point
#	IEEEF	IEEE floating point, using libs:mathieeedoubbas.library
#	IEEED	As above, but double precision
#
# Default is: IEEEF

ifdef({MATH},,define(MATH,IEEEF))
ifdef({COMPILER},,define(COMPILER,SAS6.X))

if_dbl({define({MATH},IEEED)})

#-------- Compiler/linker macros for Lattice C 5.x. -------------------------

ifelse(COMPILER,LATTICE5.X,{

#CC		= sc5		# These can be used with SAS C/6.x to be 
#LDC		= slink 	# compatible with the 5.x compiler.
CC		= lc
LDC		= blink 
LIBC		= lib:lc.lib lib:amiga.lib

if_debug({
DEBUG_FLAG_C	= -d2
})

ifelse(MATH,FFP,{
MATHFLAGS	= -ff
LIBM		= lib:lcmffp.lib
})

ifelse(MATH,IEEEF,{
MATHFLAGS	= -fi
LIBM		= lib:lcmieee.lib
})

ifelse(MATH,IEEED,{
MATHFLAGS	= -fi
LIBM		= lib:lcmieee.lib
DBL_FLAG_C	= -DDOUBLE
})

CC_FLAGS	= $(MATHFLAGS) $(DBL_FLAG_C) -v $(OPT_FLAG_C) \
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
})

ifelse(MATH,IEEEF,{
MATHFLAGS	= math=ieee
LIBM		= lib:scmieee.lib
})

ifelse(MATH,IEEED,{
MATHFLAGS	= math=ieee
LIBM		= lib:scmieee.lib
DBL_FLAG_C	= -DDOUBLE
})

CC_FLAGS	= $(MATHFLAGS) $(DBL_FLAG_C) nover $(DEBUG_FLAG_C) \
		  $(OPT_FLAG_C) ign=84,88,89,147,161 ifdef({GST},{gst=GST})

# Sigh

PLDEVICES 	= def PLMETA def NULLDEV def PS def IFF

})

LDC_FLAGS	= 
STARTUP 	= lib:c.o
TO		= to
LN		= copy

# Need to fill these in correctly if ever use f2c as a fortran compiler

F77		= f77
F77_FLAGS	= -c
LDF		= $(F77)
LDF_FLAGS	= 

##############################################################################
#
# Set up inference rules
#
##############################################################################

# Clear the suffix list, to avoid any possible problems with funny suffixes.
# I can add them back as necessary.

.SUFFIXES:

# The order here is important.  
# If the C++ compiler doesn't like .cc file suffixes, define CXX_PREFER_CSUF.

.SUFFIXES: .o .cc .c .p .f .q .h .i .ccm4 .cm4 .pm4 .hm4 .im4

.c.o:
	$(CC) $(CC_FLAGS) $*.c

ifdef({CXX_PREFER_CSUF},{
.cc.o:
	-@ln -s $*.cc $*.C
	$(CXX) $(CXX_FLAGS) $*.C
},{
.cc.o:
	$(CXX) $(CXX_FLAGS) $*.cc
})

.p.o:
	$(MPPL) $(MPPL_FLAGS) $*.p >$*.f
	$(F77) $(F77_FLAGS) $*.f

.f.o:
	$(F77) $(F77_FLAGS) $*.f

.q.o:
	$(MPPL) $(MPPL_FLAGS) $*.q >$*.f
	$(F77) $(F77_FLAGS) $*.f

.hm4.h:
	$(M4) $(M4_FLAGS) $< >$*.h

.im4.i:
	$(M4) $(M4_FLAGS) $< >$*.i

.ccm4.cc:
	$(M4) $(M4_FLAGS) $< >$*.cc

.cm4.c:
	$(M4) $(M4_FLAGS) $< >$*.c

.pm4.p:
	$(M4) $(M4_FLAGS) $< >$*.p

