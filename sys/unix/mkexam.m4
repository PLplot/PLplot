changequote({,})dnl
define(ignore)dnl
#----------------------------------------------------------------------#
#
# Makefile for building PLPLOT example programs on Unix-type systems.
# Maurice LeBrun, IFS, University of Texas, Mar 6, 1992.
#
# This file expects the PLPLOT library to be in the current directory
# by default (change the default for LIB_DIR if you don't like that).
# This can be handled on a Unix system simply by a soft link.  You can
# handle the include files the same way.
#
# System dependencies are handled via m4 macros.  Simply run this file
# through m4 with the appropriate system definition, e.g.
#
#	% m4 -DAIX <makefile.m4 >makefile
#
# to get a makefile that works on A/IX systems.  This can handle virtually
# any system in principle, although you may need to insert the necessary
# definitions (for compiler, etc) for it to work on yours.
#
#----------------------------------------------------------------------#
# Some utility macros.

define(if_dbl,   {ifdef({DOUBLE},    {$1},{$2})})dnl

define(if_aix,   {ifdef({AIX},   {$1},{$2})})dnl
define(if_dgux,  {ifdef({DGUX},  {$1},{$2})})dnl
define(if_hpux,  {ifdef({HPUX},  {$1},{$2})})dnl
define(if_sysv,  {ifdef({SYSV},  {$1},{$2})})dnl
define(if_bsd,   {ifdef({BSD},   {$1},{$2})})dnl
define(if_unicos,{ifdef({UNICOS},{$1},{$2})})dnl
define(if_sunos, {ifdef({SUNOS}, {$1},{$2})})dnl
define(if_ultrix,{ifdef({ULTRIX},{$1},{$2})})dnl

if_aix(   {define({UNIX})})dnl
if_hpux(  {define({UNIX})})dnl
if_dgux(  {define({UNIX})})dnl
if_sysv(  {define({UNIX})})dnl
if_bsd(   {define({UNIX})})dnl
if_unicos({define({UNIX})})dnl
if_sunos( {define({UNIX})})dnl
if_ultrix({define({UNIX})})dnl

define(if_unix,  {ifdef({UNIX},  {$1},{$2})})dnl
define(if_amiga, {ifdef({AMIGA}, {$1},{$2})})dnl

#----------------------------------------------------------------------#
# Generic macros for make.  If any of these do not fit your particular
# system, add the appropriate defines in a section for your system.

# Directory structure.  Note most of these aren't used at present.
# See right before rule declaration for plot library specifications.

PLLIB_DIR	= .
PLLIB_PATH	= $(PLLIB_DIR)/
FONTFLAG	=

SYS_LIBS=

if_dbl({
DBL_FLAG_C	= -DDOUBLE_PREC
DBL_FLAG_F      =
PLLIB_MAIN	= $(PLLIB_PATH)libplplotd.a
},{
DBL_FLAG_C	=
DBL_FLAG_F      =
PLLIB_MAIN	= $(PLLIB_PATH)libplplotf.a
})

PLLIB_C		= $(PLLIB_MAIN)
PLLIB_F		= $(PLLIB_MAIN)
PLLIB_LDC	= $(PLLIB_C)

# Compiler/linker macros.
# These are pretty generic to many unix systems and may work as-is.
# The device list represents ALL supported output devices.

CC	= cc
CFLAGS	= -c -Dunix -DSTUB_LAU $(DBL_FLAG_C)
F77	= f77
FFLAGS	= -c $(DBL_FLAG_F)
LDC	= $(CC)
LDCFLAGS= -lm -lX11
LDF	= $(F77)
LDFFLAGS= -lm -lX11
LN	= ln -s
TO	= -o
STARTUP =

PLDEVICES = -DPLMETA -DXTERM -DXWIN -DTEKT -DDG300 -DTEKF -DPS -DXFIG \
	    -DLJII -DHP7470 -DHP7580 -DIMP

#----------------------------------------------------------------------#
# System specific defines.
#
# Define appropriate STUB_ flag to enable proper C<->Fortran linkage
# See plstubs.h, you may have to modify for your system.
# Current choices:
#	STUB_LAU	lower-case, append underscore
#	STUB_L		lower-case
#
# If no choice is made, stubs remain unaltered (upper case).

if_hpux({
#----------------------------------------------------------------------#
#		HP-UX definitions

#CFLAGS	= -c -g -Dunix -DSTUB_L $(DBL_FLAG_C) -DHPUX
CC	= c89
CFLAGS	= -c -g -Dunix -DSTUB_L $(DBL_FLAG_C) -DHPUX -D_HPUX_SOURCE
FFLAGS	= -c $(DBL_FLAG_F)

LDCFLAGS= -L/usr/lib/X11R4 -lm -lX11 -g
LDFFLAGS= /usr/lib/X11R4/libX11.a -lm -g

PLDEVICES = -DXTERM -DXWIN -DPLMETA -DTEKT -DTEKF -DXFIG
})

if_dgux({
#----------------------------------------------------------------------#
#		DG/UX definitions

CFLAGS	= -c -Dunix -ansi -DSTUB_LAU $(DBL_FLAG_C) 
F77	= ghf77
FFLAGS	= -c $(DBL_FLAG_F) -novms
LDCFLAGS= -ansi -lm -lX11
LDFFLAGS= -ansi -lm -lX11
PLDEVICES = -DPLMETA -DXTERM -DXWIN -DTEKT -DTEKF -DPS -DXFIG
})

if_sunos({
#----------------------------------------------------------------------#
#	SUNOS definitions (also good for Ultrix 4.3)
#
# WARNING!
# You MUST use /usr/5bin/m4 to create the makefile, and not /usr/bin/m4,
# which is usually the default.  The latter does NOT WORK RIGHT.

if_dbl({dnl
DBL_FLAG_F      = -r8
})dnl

CFLAGS	= -c -Dunix -DSTUB_LAU -Dsun $(DBL_FLAG_C) 
F77	= f77
FFLAGS	= -c $(DBL_FLAG_F)
PLDEVICES = -DPLMETA -DXTERM -DXWIN -DTEKT -DTEKF -DPS -DXFIG

ifdef({SUNX},{#		"normal" MIT X Windows setup
LDCFLAGS= -lm -lX11
LDFFLAGS= -lm -lX11
},{#			Open Windows setup [default]
LDCFLAGS= -L/usr/openwin/lib -lm -lX11
LDFFLAGS= -L/usr/openwin/lib -lm -lX11
})})

if_aix({
#----------------------------------------------------------------------#
#		A/IX definitions

# Note that A/IX 3.0 has a bug in that getenv() calls from fortran
# cause a core dump.  If this occurs, you cannot include the search
# in $(HOME)/lib, so to do this specify '-DAIX_3.0' on the command
# line and hardwire in some directory ($(HOME)/lib/ is ok) for the fonts.

if_dbl({dnl
DBL_FLAG_F      = -qAUTODBL=DBLPAD
})dnl

F77	= xlf
CC	= xlc
CFLAGS	= -c -Dunix -DAIX -DSTUB_L $(DBL_FLAG_C)
FFLAGS	= -c $(DBL_FLAG_F)

#FONTFLAG= '-DPLFONTDEV1="$(HOME)/lib/"'
#FONTFLAG= '-DPLFONTDEV1="/usr/hagar/local/lib/"'	# sigh
PLDEVICES = -DPLMETA -DXTERM -DXWIN -DTEKT -DTEKF -DPS -DXFIG
})

if_unicos({
#----------------------------------------------------------------------#
#	UNICOS defs.

# These settings are appropriate for UNICOS 6.x.
# Here we assume that 'scc' is the standard C compiler.
# If this is not the case for your system, you will find out
# soon enough :-), and so use -DSCC=cc on the m4 command line (assuming 'cc'
# isn't so good, because it this invokes pcc rather than scc on some systems,
# which you may not catch right away).

F77	= cft77
CC	= ifdef({SCC},SCC,scc)
CFLAGS	= -c -Dunix
FFLAGS	=
LDF	= segldr
LDCFLAGS= -lX11,net,m
LDFFLAGS= -lX11,net,m
})

if_amiga({
#----------------------------------------------------------------------#
#	Amiga defs.

# Note this makefile for the PD make utility ported by Steve Walton.
# The 'make links' command is still required, although here it just does
# file copies since soft links not yet officially supported by the OS.

PLLIB_DIR = lib:
PLLIB_PATH = lib:

SYS_LIBS=

# Compiler/linker macros for Lattice C.

# Switch on floating point format.  Choices: 
#	FFP	Motorola fast floating point
#	IEEEF	IEEE floating point, using libs:mathieeedoubbas.library
#	IEEED	As above, but double precision
#
# Default is: IEEEF

ifdef({MATH},,define(MATH,IEEEF))

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

CFLAGS		= $(MATHFLAGS) $(DBL_FLAG_C) -i/{include} -v -d
CC		= lc
LDC		= blink 
LDCFLAGS	= 
STARTUP 	= lib:c.o
TO		= to
LIBC		= lib:lc.lib

F77		= f77
FFLAGS		= -c
LDF		= $(F77)
LDFFLAGS	= 
LN		= copy

PLLIB_C		= $(PLLIB_MAIN)
PLLIB_LDC	= lib $(LIBM) $(PLLIB_C) $(LIBC)

PLDEVICES = -DPLMETA -DXTERM -DTEKT -DDG300 -DTEKF -DPS -DLJII \
	    -DHP7470 -DHP7580 -DIMP
})
#----------------------------------------------------------------------#
# Rules
# Placed here to ensure all macros are defined.

.c.o:
	$(CC) $(CFLAGS) $*.c
.f.o:
	$(F77) $(FFLAGS) $*.f

#----------------------------------------------------------------------#
# Example programs, in c.

CDEMOS= x01c x02c x03c x04c x05c x06c x07c x08c x09c x10c x11c \
	x12c x13c x14c tutor

cdemos:	$(CDEMOS)

x01c:	x01c.o $(PLLIB_C)
	$(LDC) $(STARTUP) x01c.o $(PLLIB_LDC) $(TO) $@ $(LDCFLAGS)

x02c:	x02c.o $(PLLIB_C)
	$(LDC) $(STARTUP) x02c.o $(PLLIB_LDC) $(TO) $@ $(LDCFLAGS)

x03c:	x03c.o $(PLLIB_C)
	$(LDC) $(STARTUP) x03c.o $(PLLIB_LDC) $(TO) $@ $(LDCFLAGS)

x04c:	x04c.o $(PLLIB_C)
	$(LDC) $(STARTUP) x04c.o $(PLLIB_LDC) $(TO) $@ $(LDCFLAGS)

x05c:	x05c.o $(PLLIB_C)
	$(LDC) $(STARTUP) x05c.o $(PLLIB_LDC) $(TO) $@ $(LDCFLAGS)

x06c:	x06c.o $(PLLIB_C)
	$(LDC) $(STARTUP) x06c.o $(PLLIB_LDC) $(TO) $@ $(LDCFLAGS)

x07c:	x07c.o $(PLLIB_C)
	$(LDC) $(STARTUP) x07c.o $(PLLIB_LDC) $(TO) $@ $(LDCFLAGS)

x08c:	x08c.o $(PLLIB_C)
	$(LDC) $(STARTUP) x08c.o $(PLLIB_LDC) $(TO) $@ $(LDCFLAGS)

x09c:	x09c.o $(PLLIB_C)
	$(LDC) $(STARTUP) x09c.o $(PLLIB_LDC) $(TO) $@ $(LDCFLAGS)

x10c:	x10c.o $(PLLIB_C)
	$(LDC) $(STARTUP) x10c.o $(PLLIB_LDC) $(TO) $@ $(LDCFLAGS)

x11c:	x11c.o $(PLLIB_C)
	$(LDC) $(STARTUP) x11c.o $(PLLIB_LDC) $(TO) $@ $(LDCFLAGS)

x12c:	x12c.o $(PLLIB_C)
	$(LDC) $(STARTUP) x12c.o $(PLLIB_LDC) $(TO) $@ $(LDCFLAGS)

x13c:	x13c.o $(PLLIB_C)
	$(LDC) $(STARTUP) x13c.o $(PLLIB_LDC) $(TO) $@ $(LDCFLAGS)

x14c:	x14c.o $(PLLIB_C)
	$(LDC) $(STARTUP) x14c.o $(PLLIB_LDC) $(TO) $@ $(LDCFLAGS)

tutor:	tutor.o $(PLLIB_C)
	$(LDC) $(STARTUP) tutor.o $(PLLIB_LDC) $(TO) $@ $(LDCFLAGS)

#----------------------------------------------------------------------#
# Example programs, in Fortran.

FDEMOS = x01f x02f x03f x04f x05f x06f x07f x08f x09f x10f x11f x12f x13f

fdemos:	$(FDEMOS)

x01f:	x01f.o $(PLLIB_F)
	$(LDF) x01f.o $(PLLIB_F) $(TO) $@ $(LDFFLAGS)

x02f:	x02f.o $(PLLIB_F)
	$(LDF) x02f.o $(PLLIB_F) $(TO) $@ $(LDFFLAGS)

x03f:	x03f.o $(PLLIB_F)
	$(LDF) x03f.o $(PLLIB_F) $(TO) $@ $(LDFFLAGS)

x04f:	x04f.o $(PLLIB_F)
	$(LDF) x04f.o $(PLLIB_F) $(TO) $@ $(LDFFLAGS)

x05f:	x05f.o $(PLLIB_F)
	$(LDF) x05f.o $(PLLIB_F) $(TO) $@ $(LDFFLAGS)

x06f:	x06f.o $(PLLIB_F)
	$(LDF) x06f.o $(PLLIB_F) $(TO) $@ $(LDFFLAGS)

x07f:	x07f.o $(PLLIB_F)
	$(LDF) x07f.o $(PLLIB_F) $(TO) $@ $(LDFFLAGS)

x08f:	x08f.o $(PLLIB_F)
	$(LDF) x08f.o $(PLLIB_F) $(TO) $@ $(LDFFLAGS)

x09f:	x09f.o $(PLLIB_F)
	$(LDF) x09f.o $(PLLIB_F) $(TO) $@ $(LDFFLAGS)

x10f:	x10f.o $(PLLIB_F)
	$(LDF) x10f.o $(PLLIB_F) $(TO) $@ $(LDFFLAGS)

x11f:	x11f.o $(PLLIB_F)
	$(LDF) x11f.o $(PLLIB_F) $(TO) $@ $(LDFFLAGS)

x12f:	x12f.o $(PLLIB_F)
	$(LDF) x12f.o $(PLLIB_F) $(TO) $@ $(LDFFLAGS)

x13f:	x13f.o $(PLLIB_F)
	$(LDF) x13f.o $(PLLIB_F) $(TO) $@ $(LDFFLAGS)

#----------------------------------------------------------------------#
if_unix({
clean:
	-rm $(CDEMOS) $(FDEMOS) *.plm* *.tek*
})
