#----------------------------------------------------------------------#
# $Id$
# Makefile for building tstGT, a test code for the plplot GadTools-based
# interface.  Designed to be used with the "make" utility ported from
# the mod.sources posting.
#----------------------------------------------------------------------#

CC		= sc
LDC		= slink 
LIBC		= lib:sc.lib lib:amiga.lib
plplotGT_H	= plplotGT.h 
#GST		= gst=plplotGT.gst

MATHFLAGS	= math=ieee
LIBM		= lib:scmieee.lib
DBL_FLAG_C	= 

CFLAGS		= $(MATHFLAGS) $(DBL_FLAG_C) nover dbg=symbol \
		  ign=84,89,161 $(GST)

LDCFLAGS	= 
STARTUP 	= lib:c.o
TO		= to

PL_OBJECTS	= plplotGT.o
PLLIB_LDC	= lib $(LIBC) $(LIBM)

#----------------------------------------------------------------------#
# Rules
# Placed here to ensure all macros are defined.

.c.o:
	$(CC) $(CFLAGS) $*.c

#----------------------------------------------------------------------#
# Targets

tstGT:	tstGT.o $(PL_OBJECTS)
	$(LDC) $(STARTUP) tstGT.o $(PL_OBJECTS) $(PLLIB_LDC) \
		$(TO) $@ $(LDCFLAGS)

#----------------------------------------------------------------------#
# Object file dependencies.

tstGT.o:	$(plplotGT_H) tstGT.c 
plplotGT.o:	$(plplotGT_H) plplotGT.c 

plplotGT.gst :		plplotGT.h
	$(CC) makegst plplotGT.gst $(CFLAGS) plplotGT.c
