##############################################################################
#
# Makefile for PLPLOT on Unix-like systems.
#
# $Id$
#
##############################################################################
#
# Set up the m4 macros.

changequote({,})dnl
define(ignore)dnl

# Some utility macros.

define(if_aix,   {ifdef({AIX},   {$1},{$2})})dnl
define(if_dgux,  {ifdef({DGUX},  {$1},{$2})})dnl
define(if_hpux,  {ifdef({HPUX},  {$1},{$2})})dnl
define(if_sysv,  {ifdef({SYSV},  {$1},{$2})})dnl
define(if_bsd,   {ifdef({BSD},   {$1},{$2})})dnl
define(if_unicos,{ifdef({UNICOS},{$1},{$2})})dnl
define(if_sunos, {ifdef({SUNOS}, {$1},{$2})})dnl
define(if_ultrix,{ifdef({ULTRIX},{$1},{$2})})dnl
define(if_next,  {ifdef({NEXT},  {$1},{$2})})dnl
define(if_linux,  {ifdef({LINUX},  {$1},{$2})})dnl

if_aix(   {define({UNIX})})dnl
if_hpux(  {define({UNIX})})dnl
if_dgux(  {define({UNIX})})dnl
if_sysv(  {define({UNIX})})dnl
if_bsd(   {define({UNIX})})dnl
if_unicos({define({UNIX})})dnl
if_sunos( {define({UNIX})})dnl
if_next(  {define({UNIX})})dnl
if_linux(  {define({UNIX},)define({NO_FORTRAN},)})dnl
if_ultrix({define({UNIX})define({SUNOS})})dnl

define(if_ranlib,{ifdef({RANLIB},{$1},{$2})})dnl
if_sunos( {define({RANLIB})})dnl
if_next(  {define({RANLIB})})dnl
if_bsd(   {define({RANLIB})})dnl
if_linux(   {define({RANLIB},)})dnl

define(if_unix,  {ifdef({UNIX},  {$1},{$2})})dnl
define(if_amiga, {ifdef({AMIGA}, {$1},{$2})})dnl

define(if_debug,   {ifdef({DEBUG},     {$1},{$2})})dnl
define(if_dbl,     {ifdef({DOUBLE},    {$1},{$2})})dnl
define(if_opt,     {ifdef({OPT},       {$1},{$2})})dnl

##############################################################################
#
# Documentation
#
# This makefile uses softlinks between all files and a temporary directory,
# to keep the make reasonably straightforward while not disturbing the nice
# organization of the package.  When installation is finished, simply delete
# the contents of the PLTMP directory. 
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
# The available output devices are controlled by the PLDEVICES variable,
# a list of defines for the C preprocessor (affecting only dispatch.c).
# When installing plplot you may wish to exclude devices not present
# on your system in order to reduce screen clutter.  The default setting
# is to include all supported devices.
# 
# Problems/bugs:
#
# 1. When editing from the PLTMP directory, you must use the true path name
#    (not that of the softlink) and the timestamp on the softlink will be
#    automatically changed.  Otherwise the "permanent" sources may not
#    be changed (depends on editor; emacs makes a copy, but vi updates
#    the original).
#
# 2. The dependencies on the header files have been omitted, for simplicity.
#    If you change any of the header files, assume that the whole package 
#    must be rebuilt.
#
# 3. Some versions of m4 (like the one distributed with the NeXT) don't
#    recognize the -D option.  Either get a better version of m4 or use the
#    following trick
#      echo 'define(NEXT)' | cat - makefile.m4 | m4 > makefile
#    to generate the makefile.
#
# Other notes:
#
# 1. The font locating code looks in the following places for the fonts:
#
#	current directory
#	$(HOME)/lib/
#	$(PLFONTS)
#	PLFONTDEV1	(/usr/local/lib/)
#	PLFONTDEV2	(/usr/local/lib/plplot/)
#	PLFONTDEV3	(/usr/local/plplot/)
#
#    It is recommended that you store the fonts in a local directory during
#    installation.  When you have verified the package works as expected,
#    simply move the fonts to the appropriate system directory and then all
#    users will have access to the fonts (no recompilation necessary).  If
#    none of the three given hardwired locations is satisfactory, you can
#    change it by passing in your desired location on the build line for
#    plfont.c (done separately for this purpose), e.g.
#
#	FONTFLAG= '-DPLFONTDEV1="/usr/lib/plplot/"'
#
#    (quotes and trailing slash required).
#
# 2. To explicitly force the double precision library to be built, create
#    makefile with:
#
#	% m4 -D<sys> -DDOUBLE makefile.m4 >makefile
#
#    The double precision library will typically end with a 'd', the
#    single precision library with an 'f'.  On 32-bit machines, typically
#    the single precision library is built since this speeds development
#    (and usually is simpler, since a REAL will usually be 32 bits).  On
#    64 bit systems typically we build the double precision library.
#    (In other words, the libraries linked with are the same as long as
#    you stick with the default Fortran precision.)  
#
#    Note that there is no good, standard way to convert a Fortran code
#    to double precision to the command line.  If your system's compiler
#    has an option to do this, you must set
#
# 3. To link the library without the fortran stub routines (in case
#    you don't have a fortran compiler handy), specify -DNO_FORTRAN
#    on the m4 command line.
#
##############################################################################

##############################################################################
#
# Set up compiler & linker macros, to make invocation look as similar as
# possible from system to system.
#
##############################################################################

# Generic macros for make.  If any of these do not fit your particular
# system, add the appropriate defines in a section for your system.

# Directory structure.  Note most of these aren't used at present.
# See right before rule declaration for plot library specifications.

PLLIB_DIR	= ../lib
PLFNT_DIR	= ../lib
PLLIB_PATH	= $(PLLIB_DIR)/
PLFNT_PATH	= $(PLFNT_DIR)/
FONTFLAG	=

SYS_LIBS=

# Note there is no "standard" way to invoke double precision in Fortran
# from the command line, although several systems do provide a way (see
# sections for SUNOS, A/IX).

if_dbl({
DBL_FLAG_C	= -DPL_DOUBLE
DBL_FLAG_F      =
PLLIB_MAIN	= $(PLLIB_PATH)libplplotd.a
},{
DBL_FLAG_C	=
DBL_FLAG_F      =
PLLIB_MAIN	= $(PLLIB_PATH)libplplotf.a
})

if_debug({
DEBUG_FLAG_C	= -g
DEBUG_FLAG_F	= -g
},{
DEBUG_FLAG_C	= 
DEBUG_FLAG_F	=
})

if_opt({
OPT_FLAG_C	= -O
OPT_FLAG_F	= -O
},{
OPT_FLAG_C	= 
OPT_FLAG_F	= 
})

PLLIB_C		= $(PLLIB_MAIN)
PLLIB_F		= $(PLLIB_MAIN)
PLLIB_LDC	= $(PLLIB_C)

# Compiler/linker macros.
# These are pretty generic to many unix systems and may work as-is.
# The device list represents all system-independent output devices.
# (i.e. not specifically requiring a particular CPU)

SYS_FLAGS_C =
SYS_FLAGS_F = 
CFLAGS	= -c $(DBL_FLAG_C) $(DEBUG_FLAG_C) $(OPT_FLAG_C) $(SYS_FLAGS_C)
FFLAGS	= -c $(DBL_FLAG_F) $(DEBUG_FLAG_F) $(OPT_FLAG_F) $(SYS_FLAGS_F)
LDCFLAGS= -lm -lX11
LDFFLAGS= -lm -lX11

CC	= cc
F77	= f77
LDC	= $(CC)
LDF	= $(F77)
LN	= ln -s
TO	= -o
STARTUP =

PLDEVICES = -DPLMETA -DXTERM -DXWIN -DTEK -DDG300 -DPS -DXFIG \
	    -DLJII -DHP7470 -DHP7580 -DIMP

##############################################################################
#
# System specific defines.
#
# Define appropriate STUB_ flag to enable proper C<->Fortran linkage
# See plstubs.h, you may have to modify for your system.
# Current choices:
#	STUB_LAU	lower-case, append underscore
#	STUB_L		lower-case
#
# If no choice is made, stubs remain unaltered (upper case).
#
##############################################################################

#----------------------------------------------------------------------#
if_sunos({
#	SUNOS definitions (also good for Ultrix 4.3)
#
# WARNING!
# You MUST use /usr/5bin/m4 to create the makefile, and not /usr/bin/m4,
# which is usually the default.  The latter does NOT WORK RIGHT.
#
# OPENWIN_DIR defaults to -L/usr/openwin/lib, and is needed to find the
# X libraries for most machines.  This may be a problem on other machines
# if the openwin directory is absent.  If this is the case, you must
# specify -DNO_OPENWIN when building the makefile with m4.

CC = gcc

PLDEVICES = -DPLMETA -DXTERM -DXWIN -DTEK -DPS -DXFIG
SYS_FLAGS_C =

if_dbl({dnl
DBL_FLAG_F      = -r8
})dnl

ifdef({NO_OPENWIN},{dnl
OPENWIN_DIR = -L/usr/openwin/lib
},{dnl
OPENWIN_DIR =
})

CFLAGS	= -c $(DBL_FLAG_C) $(DEBUG_FLAG_C) $(OPT_FLAG_C) $(SYS_FLAGS_C)
FFLAGS	= -c $(DBL_FLAG_F) $(DEBUG_FLAG_F) $(OPT_FLAG_F) $(SYS_FLAGS_F)

LDCFLAGS= $(OPENWIN_DIR) -lm -lX11
LDFFLAGS= $(OPENWIN_DIR) -lm -lX11

#----------------------------------------------------------------------#
})if_hpux({
#		HP-UX definitions

PLDEVICES = -DXTERM -DXWIN -DPLMETA -DTEK -DPS -DXFIG
SYS_FLAGS_C =

if_dbl({dnl
DBL_FLAG_F      = -R8
})dnl

CC	= c89
CFLAGS	= -c $(DBL_FLAG_C) $(DEBUG_FLAG_C) $(OPT_FLAG_C) $(SYS_FLAGS_C)
FFLAGS	= -c $(DBL_FLAG_F) $(DEBUG_FLAG_F) $(OPT_FLAG_F) $(SYS_FLAGS_F)

LDCFLAGS= -L/usr/lib/X11R4 -lm -lX11 -g
LDFFLAGS= /usr/lib/X11R4/libX11.a -lm -g

#----------------------------------------------------------------------#
})if_dgux({
#		DG/UX definitions

PLDEVICES = -DPLMETA -DXTERM -DXWIN -DTEK -DPS -DXFIG
SYS_FLAGS_C = -Dunix -DSTUB_LAU -ansi
SYS_FLAGS_F = -novms

F77	= ghf77
CFLAGS	= -c $(DBL_FLAG_C) $(DEBUG_FLAG_C) $(OPT_FLAG_C) $(SYS_FLAGS_C)
FFLAGS	= -c $(DBL_FLAG_F) $(DEBUG_FLAG_F) $(OPT_FLAG_F) $(SYS_FLAGS_F)

LDCFLAGS= -ansi -lm -lX11
LDFFLAGS= -ansi -lm -lX11

#----------------------------------------------------------------------#
})if_linux({
#		LINUX definitions

PLDEVICES = -DPLMETA -DPS -DLJII -DXWIN
SYS_FLAGS_C =

CC	= gcc
F77	= 
CFLAGS	= -c $(DBL_FLAG_C) $(DEBUG_FLAG_C) $(OPT_FLAG_C) $(SYS_FLAGS_C)
FFLAGS	= 

LDCFLAGS= -ansi -lX11 -lm
LDFFLAGS= -ansi -lX11 -lm

# Note that the ljii.c driver doesn't work yet, but we need to have it
# in if it's ever gonna get fixed.

#----------------------------------------------------------------------#
})if_next({
#               NeXT definitions

PLDEVICES = -DNEXT -DPS -DPLMETA

# You can use the NOBRAINDEAD option here if you wish
# If you use it, you may want to define NOBRAINDEAD in plplot.h
# before moving it to it's permanent location.

#SYS_FLAGS_C = -Dunix -DSTUB_LAU -DNOBRAINDEAD
SYS_FLAGS_C = -Dunix -DSTUB_LAU

# Suggested locations:
#  libplplotf.a in /usr/local/lib/
#  fonts        in /usr/local/lib/plplot/
#  plrender     in /usr/local/bin/
#  plplot.h     in /usr/local/include/
#  chdr.h       in /usr/local/include/
# cc will automatically look in these locations for the libraries and
# include files, so you can compile with a minimum of command line
# options (just -lplplotf).

CC      = cc

CFLAGS	= -c $(DBL_FLAG_C) $(DEBUG_FLAG_C) $(OPT_FLAG_C) $(SYS_FLAGS_C)

# I've tested Fortran compatibility using f2c only - amr

F77	= f77
FFLAGS	= -c $(DBL_FLAG_F) $(DEBUG_FLAG_F) $(OPT_FLAG_F) $(SYS_FLAGS_F)

LDCFLAGS= -lm
LDFFLAGS= 

#----------------------------------------------------------------------#
})if_aix({
#		A/IX definitions

PLDEVICES = -DPLMETA -DXTERM -DXWIN -DTEK -DPS -DXFIG
SYS_FLAGS_C =

# Note that A/IX 3.0 has a bug in that getenv() calls in a C routine
# linked with a Fortran main cause a core dump.  If this occurs, you 
# cannot include the search in $(HOME)/lib.  In this case you must
# specify '-DAIX_3_0' on the command line and hardwire in some directory
# ($(HOME)/lib/ is ok) for the fonts.  This problem was fixed in A/IX 3.1.

if_dbl({dnl
DBL_FLAG_F      = -qAUTODBL=DBLPAD
})dnl

F77	= xlf
CC	= xlc
CFLAGS	= -c $(DBL_FLAG_C) $(DEBUG_FLAG_C) $(OPT_FLAG_C) $(SYS_FLAGS_C)
FFLAGS	= -c $(DBL_FLAG_F) $(DEBUG_FLAG_F) $(OPT_FLAG_F) $(SYS_FLAGS_F)

#FONTFLAG= '-DPLFONTDEV1="$(HOME)/lib/"'		# deal with 3.0 bug

#----------------------------------------------------------------------#
})if_unicos({
#	UNICOS defs.

SYS_FLAGS_C =

# These settings are appropriate for UNICOS 6.x.
# Here we assume that 'cc' is the standard C compiler.

F77	= cf77
LDF	= segldr

CFLAGS	= -c $(DBL_FLAG_C) $(DEBUG_FLAG_C) $(OPT_FLAG_C) $(SYS_FLAGS_C)
FFLAGS	= -c $(DBL_FLAG_F) $(DEBUG_FLAG_F) $(OPT_FLAG_F) $(SYS_FLAGS_F)

LDCFLAGS= -lX11,net,m
LDFFLAGS= -lX11,net,m

#----------------------------------------------------------------------#
})if_amiga({
#	Amiga defs.

# Note this makefile for the PD make utility ported by Steve Walton.
# The 'make links' command is still required, although here it just does
# file copies since soft links not yet officially supported by the OS.
# Alternately, if you are an SKsh user, you can use the 'makelinks' script
# to do actual links.

PLLIB_DIR = /lib
PLFNT_DIR = /lib
PLLIB_PATH = $(PLLIB_DIR)/
PLFNT_PATH = $(PLFNT_DIR)/

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

CFLAGS		= $(MATHFLAGS) $(DBL_FLAG_C) -i/{include} -v -d2 -j89i
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

PLDEVICES = -DPLMETA -DXTERM -DTEK -DDG300 -DPS -DLJII \
	    -DHP7470 -DHP7580 -DIMP -DIFF -DAEGIS
})
#----------------------------------------------------------------------#
# Object file macros
# Main sources.

OBJ =	\
	base.o \
	convert.o \
	genlin.o \
	move.o \
	pladv.o \
	plaxes.o \
	plbeg.o \
	plbin.o \
	plbox.o \
	plbox3.o \
	plccal.o \
	plcntr.o \
	plcol.o \
	plcont.o \
	plconf.o \
	pldtik.o \
	plend.o \
	plenv.o \
	plerr.o \
	plexit.o \
	plfill.o \
	plfont.o \
	plform.o \
	plgra.o \
	plgrid3.o \
	plgspa.o \
	plhist.o \
	pljoin.o \
	pllab.o \
	pllclp.o \
	plline.o \
	pllsty.o \
	plmesh.o \
	plmtex.o \
	plnxtv.o \
	plot3d.o \
	plpat.o \
	plpsty.o \
	plptex.o \
	plrot.o \
	plsdef.o \
	plside3.o \
	plstar.o \
	plstyl.o \
	plsvpa.o \
	plsym.o \
	plt3zz.o \
	pltick.o \
	plvasp.o \
	plvpas.o \
	plvpor.o \
	plvsta.o \
	plw3d.o \
	plwid.o \
	plwind.o \
	plxybx.o \
	plzbx.o \
	string.o 

# Drivers

DRIVERS_OBJ = \
	dg300.o \
	dispatch.o \
	hp7470.o \
	hp7580.o \
	impress.o \
	ljii.o \
	next.o \
	ps.o \
	tek.o \
	plmeta.o \
	pdfutils.o \
	xfig.o \
	xwin.o \
	xterm.o

# Support files for font generators.

FONT_OBJ = \
	font01.o \
	font02.o \
	font03.o \
	font04.o \
	font05.o \
	font06.o \
	font07.o \
	font08.o \
	font09.o \
	font10.o \
	font11.o

# C language stubs for linking Plplot to Fortran.

CSTUB_OBJ = \
	long2str.o \
	sc3d.o \
	scconf.o \
	scstubs.o

# Fortran language stubs for linking Plplot to Fortran.

FSTUB_OBJ = \
	islen.o \
	sfstubs.o \
	str2int.o

# System-specific files.

# Amiga

AMIGA_OBJ = \
	aegis.o \
	amiwn.o \
	amipr.o \
	iff.o \
	plmenu.o \
	plprefs.o \
	plsupport.o \
	plwindow.o 

#----------------------------------------------------------------------#
# Rules
# Placed here to ensure all macros are defined.

.c.o:
	$(CC) $(CFLAGS) $*.c
.f.o:
	$(F77) $(FFLAGS) $*.f

#----------------------------------------------------------------------#
# Targets
# The default is to make all the object libraries.
# You can make everything (except demos) by typing 'make everything'
# but you must create the links ('make links') beforehand.

default: libs
everything: libs fonts plrender pltek

libs:	$(PLLIB_MAIN)

#----------------------------------------------------------------------#
# Plot libraries.
#
# Note -- don't use link libraries but ar libraries instead.
# Link libraries cause lots of grief on some systems because often (reason
# unknown) a structure that needs to be initialized is not (happens on 
# A/IX, VMS).  Fortunately package is reasonably small, especially if
# you compile with debugging off.

if_unix({ifdef({NO_FORTRAN},{
$(PLLIB_MAIN):	$(OBJ) $(DRIVERS_OBJ)
	-rm $(PLLIB_MAIN)
	ar q $(PLLIB_MAIN) $(OBJ) $(DRIVERS_OBJ)
	if_ranlib({ranlib $(PLLIB_MAIN)})
},{
$(PLLIB_MAIN):	$(OBJ) $(DRIVERS_OBJ) $(CSTUB_OBJ) $(FSTUB_OBJ)
	-rm $(PLLIB_MAIN)
	ar q $(PLLIB_MAIN) $(OBJ) $(DRIVERS_OBJ) $(CSTUB_OBJ) $(FSTUB_OBJ)
	if_ranlib({ranlib $(PLLIB_MAIN)})
})})

if_amiga({
$(PLLIB_MAIN):	$(OBJ) $(DRIVERS_OBJ) $(AMIGA_OBJ)
	-delete $(PLLIB_MAIN)
	join \#?.o as $(PLLIB_MAIN)
})
#----------------------------------------------------------------------#
# Font files.
# Note 6 font files actually created.
# sfont - standard font
# xfont - extended fonts

fonts:	sfont xfont

sfont:	$(PLFNT_DIR)plstnd.fnt
xfont:	$(PLFNT_DIR)plxtnd.fnt

$(PLFNT_DIR)plstnd.fnt: stndfont.o $(FONT_OBJ)
	$(LDC) -o stndfont stndfont.o $(FONT_OBJ) $(LDCFLAGS)
	-./stndfont
	mv *.fnt $(PLFNT_DIR)

$(PLFNT_DIR)plxtnd.fnt: xtndfont.o $(FONT_OBJ)
	$(LDC) -o xtndfont xtndfont.o $(FONT_OBJ) $(LDCFLAGS)
	-./xtndfont
	mv *.fnt $(PLFNT_DIR)

#----------------------------------------------------------------------#
# Explicit rules
#
# plfont.c may have font flags passed in

plfont.o:	plfont.c
	$(CC) $(CFLAGS) $(FONTFLAG) plfont.c

# dispatch.c and all the drivers need to know $(PLDEVICES).  The guts
# of the driver routine are not compiled if its name is not present in
# the device list.  You may want to leave drivers for specific systems
# (Amiga, MS-DOS, OS/2, etc) out of this list.

dispatch.o:	dispatch.c
	$(CC) $(CFLAGS) $(PLDEVICES) dispatch.c

dg300.o:	dg300.c
	$(CC) $(CFLAGS) $(PLDEVICES) dg300.c

hp7470.o:	hp7470.c
	$(CC) $(CFLAGS) $(PLDEVICES) hp7470.c

hp7580.o:	hp7580.c
	$(CC) $(CFLAGS) $(PLDEVICES) hp7580.c

impress.o:	impress.c
	$(CC) $(CFLAGS) $(PLDEVICES) impress.c

ljii.o:	ljii.c
	$(CC) $(CFLAGS) $(PLDEVICES) ljii.c

next.o:	next.c
	$(CC) $(CFLAGS) $(PLDEVICES) next.c

ps.o:	ps.c
	$(CC) $(CFLAGS) $(PLDEVICES) ps.c

tek.o:	tek.c
	$(CC) $(CFLAGS) $(PLDEVICES) tek.c

plmeta.o:	plmeta.c
	$(CC) $(CFLAGS) $(PLDEVICES) plmeta.c

xfig.o:	xfig.c
	$(CC) $(CFLAGS) $(PLDEVICES) xfig.c

xwin.o:	xwin.c
	$(CC) $(CFLAGS) $(PLDEVICES) xwin.c

xterm.o:	xterm.c
	$(CC) $(CFLAGS) $(PLDEVICES) xterm.c

#----------------------------------------------------------------------#
# Utility programs.

pltek:	pltek.o
	$(LDC) $(STARTUP) pltek.o $(PLLIB_LDC) $(TO) $@ $(LDCFLAGS)

plrender: plrender.o 
	$(LDC) $(STARTUP) plrender.o $(PLLIB_LDC) $(TO) $@ $(LDCFLAGS)

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
# First time only, should be executed from $(PLTMP_DIR)

if_unix({
links:
	$(LN) \
		../src/*.c \
		../src/stubc/*.c \
if_hpux({dnl
if_dbl({dnl
		../src/stubf/dbl/*.f \
		../examples/f77/dbl/*.f \
},{dnl
		../src/stubf/sgl/*.f \
		../examples/f77/sgl/*.f \
})dnl
},{dnl
		../src/stubf/sgl/*.f \
		../examples/f77/sgl/*.f \
})dnl
		../examples/C/*.c \
		../utils/*.c \
		../fonts/*.c \
		../{include}/*.h \
		../drivers/*.c \
		.
})
if_amiga({
links:
	copy /src/\#?.c ""
	copy /drivers/\#?.c ""
	copy /utils/\#?.c ""
	copy /fonts/\#?.c ""
	copy /examples/C/\#?.c ""
	copy /sys/amiga/src/\#?.c ""
})

if_unix({
clean:
	-rm $(CDEMOS) $(FDEMOS) *.plm* *.tek*
})
