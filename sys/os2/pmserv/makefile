#
# Makefile for PLPLOT
#
# This makefile is intended to be used with Microsft NMAKE, as supplied with
# MS C 6.0 Professional Development System (it's not actually very expensive).
# It is currently set up to be run in an OS/2 session, and will build the
# PLPLOT system of libraries, utilities and examples for execution under either
# OS/2 native mode or under DOS (and the OS/2 DOS box).  
#
# To build the DOS version, you will need to modify the makefile in a couple
# of places, in order to include the DOSVGA driver.  These changes are marked
# in the makefile with comments starting with "# DOS".
#
# If you wish to actually execute the makefile from DOS, you might need to
# change a few things.	(I always run in OS/2 sessions, even when building
# for DOS).  In particular, the move commands in the links target would
# need to be reformulated.
#
# Questions or problems, send (e)mail to:
#	Geoffrey Furnish
#	furnish@fusion.ph.utexas.edu
#	Institute for Fusion Studies
#	Department of Physics
#	The University of Texas at Austin
#	Austin, Texas	78712

PLDIR	= d:\plplot\ 
PLLIBS	= .
PLFONTS	= $(PLDIR)plfonts\ 
PLFNT_DIR=\lib
PLTMP	= $(PLDIR)tmp\ 
PLLIBDIR = .

CC	= cl
#CFLAGS	= /FPi87  /nologo /Od /Zi /AL
CFLAGS	= /FPi87  /nologo /Ox /G2 /AL
F77	= fl
FFLAGS	= /FPi87  /nologo /Od /Zi /AL

# Rules
.SUFFIXES:
.SUFFIXES: .o .c

.c.o:
	$(CC) -c $(CFLAGS) /Fo$*.o $*.c 

help:
	@echo * Get down to the tmp dir, and then type "nmake links"
	@echo * in order to obtain the PLPLOT files you need to 
	@echo * continue this development. Then, "make plrender", etc.

# Links.  Get the files we need for this development.

links:
	copy $(PLDIR)utils\plrender.c .
	copy $(PLDIR)utils\getopt.c .
	copy $(PLDIR)include\metadefs.h .
	copy $(PLDIR)include\plplot.h .
	copy $(PLDIR)include\chdr.h .
	copy $(PLDIR)include\dispatch.h .
	copy ..\os2pm.c .
	copy ..\pmdefs.h .
	copy ..\pmserv.c .
	copy ..\pmserv.def .
	copy ..\pmserv.rc .
	copy ..\read.me .
	
# The os2pm driver.

os2pm.o : os2pm.c pmdefs.h

# The metafile renderer.

plrender:
	cl $(CFLAGS) plrender.c getopt.c plplot.lib

# The Presentation Manager Server.

pmserv: pmserv.c pmdefs.h pmserv.res
	cl -c -Alfw -G2sw -W3 /MT pmserv.c
	link pmserv, /align:16, NUL, /nod llibcmt os2, pmserv;
	rc pmserv.res

pmserv.res: pmserv.rc pmdefs.h
	rc -r pmserv

# Make the distribution

dist:
	-@del os2pmdst.zoo
	zoo -add os2pmdst os2pm.c pmdefs.h pmserv.exe read.me
	uue os2pmdst.zoo os2pmdst.uue
