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

# Some utility macros.

define(if_aix,	    {ifdef({AIX},	{$1},{$2})})dnl
define(if_dgux,	    {ifdef({DGUX},	{$1},{$2})})dnl
define(if_hpux,	    {ifdef({HPUX},	{$1},{$2})})dnl
define(if_sysv,	    {ifdef({SYSV},	{$1},{$2})})dnl
define(if_bsd,	    {ifdef({BSD},	{$1},{$2})})dnl
define(if_unicos,   {ifdef({UNICOS},	{$1},{$2})})dnl
define(if_sunos,    {ifdef({SUNOS},	{$1},{$2})})dnl
define(if_irix,     {ifdef({IRIX},	{$1},{$2})})dnl
define(if_ultrix,   {ifdef({ULTRIX},	{$1},{$2})})dnl
define(if_next,	    {ifdef({NEXT},	{$1},{$2})})dnl
define(if_linux,    {ifdef({LINUX},	{$1},{$2})})dnl
define(if_sx,	    {ifdef({SX},	{$1},{$2})})dnl
define(if_alphaosf, {ifdef({ALPHAOSF},	{$1},{$2})})dnl

if_aix(     {define({UNIX},)})dnl
if_hpux(    {define({UNIX},)})dnl
if_dgux(    {define({UNIX},)})dnl
if_sysv(    {define({UNIX},)})dnl
if_bsd(     {define({UNIX},)})dnl
if_irix(    {define({UNIX},)})dnl
if_unicos(  {define({UNIX},)})dnl
if_sunos(   {define({UNIX},)})dnl
if_next(    {define({UNIX},)})dnl
if_linux(   {define({UNIX},)define({NO_FORTRAN},)})dnl
if_ultrix(  {define({UNIX},)define({SUNOS})})dnl
if_sx(      {define({UNIX},)})dnl
if_alphaosf({define({UNIX},)})dnl

define(if_ranlib,{ifdef({RANLIB},{$1},{$2})})dnl
if_sunos( {define({RANLIB},)})dnl
if_next(  {define({RANLIB},)})dnl
if_bsd(   {define({RANLIB},)})dnl
if_linux( {define({RANLIB},)})dnl

define(if_unix,  {ifdef({UNIX},  {$1},{$2})})dnl
define(if_amiga, {ifdef({AMIGA}, {$1},{$2})})dnl

ifdef({NO_X},{dnl
define({NO_TK})dnl
define(if_xwin,	 {$2})dnl
},{dnl
define(if_xwin,	 {ifdef({XWIN},  {$1},{$2})})dnl
})dnl

ifdef({NO_TK},{dnl
define(if_tk,    {$2})dnl
},{dnl
define(if_tk,    {ifdef({TK},    {$1},{$2})})dnl
})dnl

define(if_debug,{ifdef({DEBUG},		{$1},{$2})})dnl
define(if_dbl,	{ifdef({DOUBLE},	{$1},{$2})})dnl
define(if_opt,	{ifdef({OPT},		{$1},{$2})})dnl
define(if_prof,	{ifdef({PROFILE},	{$1},{$2})})dnl
define(if_shr,	{ifdef({NO_SHARE},	{$2},{$1})})dnl

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
# a list of defines for the C preprocessor.  When installing plplot it's a
# good idea to exclude devices not present on your system in order to reduce
# screen clutter.  The default setting is to include all supported devices.
# 
# Problems/bugs:
#
# 1. When editing from the tmp directory, some editors do not update the
#    pointed-to source, but instead make a copy in the tmp directory.
#    Editors known to work correctly include vi and GNU emacs (18.58 or
#    later). 
#
# 2. Some versions of m4 (like the one distributed with the NeXT, or the
#    default SunOS version) don't recognize the -D option.  Either get a
#    better version of m4 or use the following trick
#
#      echo 'define(NEXT)' | cat - makefile.m4 | m4 > makefile
#
#    to generate the makefile.  On SunOS, use /usr/5bin/m4.
#
# 3. Some versions of m4 (e.g. on the Amiga) require both input and output
#    redirection:
#	m4 -DAMIGA <makefile.m4 >makefile
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
#    Note that there is no standard way to convert a Fortran code to
#    double precision to the command line.  Compilers under SunOS, HPUX,
#    and A/IX do offer a way to do this however, and building the double
#    precision library including the fortran stubs should work fine on
#    these platforms.  For other systems, you must add the appropriate
#    compiler flag.
#
# 3. To link the library without the fortran stub routines (in case
#    you don't have a fortran compiler handy), specify -DNO_FORTRAN
#    on the m4 command line.
#
# 4. To get debug settings, specify -DDEBUG on the m4 command line.
#
# 5. To get profiling, specify -DPROFILE on the m4 command line.
#
##############################################################################

##############################################################################
#
# Setup macros for a Unix-like system.  These can be changed in the section
# for your system.
#
##############################################################################

if_unix({

# Directory structure.  Note most of these aren't used at present.
# See right before rule declaration for plot library specifications.

INSTALL_DIR	= /usr/local/plplot
PLLIB_DIR	= ../lib
PLFNT_DIR	= ../lib
PLLIB_PATH	= $(PLLIB_DIR)/
PLFNT_PATH	= $(PLFNT_DIR)/
FONTFLAG	=
TK_INCDIR	= /usr/local/{include}
TK_LINKDIR	= /usr/local/lib
SYS_LIBS	=

# Note there is no "standard" way to invoke double precision in Fortran
# from the command line, although several systems do provide a way (see
# sections for SUNOS, A/IX).

if_dbl({
DBL_FLAG_C	= -DPL_DOUBLE
DBL_FLAG_F      =
},{
DBL_FLAG_C	=
DBL_FLAG_F      =
})

if_prof({
PROFILE_FLAG_C	= -p
PROFILE_FLAG_LC	= -p
},{
PROFILE_FLAG_C	= 
PROFILE_FLAG_LC	= 
})
PROFILE_FLAG_F	=
PROFILE_FLAG_LF	=

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

# Enable X driver by default if a Unix system.

if_unix({define({XWIN},)})

LIB_XWIN  = -lX11
LIB_TK    = -ltk -ltcl

define(DEF_TK,   {if_tk({-DTK})})
define(DEF_XWIN, {if_xwin({-DXWIN})})

PLDEVICES = -DPLMETA -DNULLDEV -DXTERM -DXWIN -DTEK4010 -DTEK4107 -DDG300 \
	    -DPS -DXFIG -DLJII -DHP7470 -DHP7580 -DIMP DEF_XWIN() DEF_TK()

# Compiler/linker macros.
# These are pretty generic to many unix systems and may work as-is.
# The device list represents all system-independent output devices.
# (i.e. not specifically requiring a particular CPU)

SYS_FLAGS_C =
SYS_FLAGS_F = 
CFLAGS	= -c $(DBL_FLAG_C) $(DEBUG_FLAG_C) $(OPT_FLAG_C) $(SYS_FLAGS_C) \
	     $(PROFILE_FLAG_C)

FFLAGS	= -c $(DBL_FLAG_F) $(DEBUG_FLAG_F) $(OPT_FLAG_F) $(SYS_FLAGS_F) \
	     $(PROFILE_FLAG_F)

LIBS	= if_tk({$(LIB_TK)}) if_xwin({$(LIB_XWIN)}) 

LDCFLAGS= $(PROFILE_FLAG_LC) $(LIBS) -lm
LDFFLAGS= $(PROFILE_FLAG_LF) $(LIBS) -lm

CC	= cc
F77	= f77
BUILD	= ar q
LIBSUF	= a
LDC	= $(CC)
LDF	= $(F77)
LN	= ln -s
TO	= -o
STARTUP =

})

##############################################################################
#
# System specific defines.
#
##############################################################################

#----------------------------------------------------------------------#
if_sunos({
#	SUNOS definitions (also good for Ultrix 4.3)
#	Pick between gcc and acc the old-fashioned way.
#
# WARNING!
# You MUST use /usr/5bin/m4 to create the makefile, and not /usr/bin/m4,
# which is usually the default.  The latter does NOT WORK RIGHT.
#
# OPENWIN_DIR defaults to -L/usr/openwin/lib, and is needed to find the
# X libraries for most machines.  This may be a problem on other machines
# if the openwin directory is absent.  If this is the case, you must
# specify -DNO_OPENWIN when building the makefile with m4.

define({TK},)

CC = gcc
#CC = acc

PLDEVICES = -DPLMETA -DNULLDEV -DXTERM -DTEK4010 -DTEK4107 -DPS -DXFIG \
	    DEF_XWIN() DEF_TK()
SYS_FLAGS_C =

if_dbl({dnl
DBL_FLAG_F      = -r8
})dnl

ifdef({NO_OPENWIN},{dnl
OPENWIN_DIR = -L/usr/openwin/lib
},{dnl
OPENWIN_DIR =
})

if_prof({
PROFILE_FLAG_LC = -p -static
})

CFLAGS	= -c $(DBL_FLAG_C) $(DEBUG_FLAG_C) $(OPT_FLAG_C) $(SYS_FLAGS_C) \
	     $(PROFILE_FLAG_C)

FFLAGS	= -c $(DBL_FLAG_F) $(DEBUG_FLAG_F) $(OPT_FLAG_F) $(SYS_FLAGS_F) \
	     $(PROFILE_FLAG_F)

LIBS	= if_tk({$(LIB_TK)}) if_xwin({$(LIB_XWIN)}) 
LDCFLAGS= $(OPENWIN_DIR) $(PROFILE_FLAG_LC) $(LIBS) -lm
LDFFLAGS= $(OPENWIN_DIR) $(PROFILE_FLAG_LF) $(LIBS) -lm

#----------------------------------------------------------------------#
})if_hpux({
#		HP-UX definitions
#
# The default is to build a shareable library including the TK driver.

define({TK},)

PLDEVICES = -DXTERM -DPLMETA -DNULLDEV -DTEK4010 -DTEK4107 -DPS -DXFIG \
	     DEF_XWIN() DEF_TK()

if_dbl({dnl
DBL_FLAG_F      = -R8
})dnl

if_prof({
PROFILE_FLAG_C	= -G
PROFILE_FLAG_F	= -G
PROFILE_FLAG_LC	= -G
PROFILE_FLAG_LF	= -G
})

CC	= c89
F77	= fort77

SYS_FLAGS_C	= 

if_shr({
BUILD		= ld -b -o
LIBSUF		= sl
SHARE_FLAG_C	= +z
SHARE_FLAG_F	= +z
},{
SHARE_FLAG_C	=
SHARE_FLAG_F	=
})

LIBC	= if_tk({$(LIB_TK)}) if_xwin({$(LIB_XWIN)})
LIBF	= if_tk({$(LIB_TK)}) if_xwin({$(LIB_XWIN)}) 

CFLAGS	= -c $(DBL_FLAG_C) $(DEBUG_FLAG_C) $(OPT_FLAG_C) $(SYS_FLAGS_C) \
	     $(PROFILE_FLAG_C) $(SHARE_FLAG_C)

FFLAGS	= -c $(DBL_FLAG_F) $(DEBUG_FLAG_F) $(OPT_FLAG_F) $(SYS_FLAGS_F) \
	     $(PROFILE_FLAG_F) $(SHARE_FLAG_F)

LDCFLAGS= $(PROFILE_FLAG_LC) $(LIBC) -lm -g
LDFFLAGS= $(PROFILE_FLAG_LF) $(LIBF) -lm -g

#----------------------------------------------------------------------#
})if_sysv({
#		SYSV definitions
# Do we need to have different sections for SVR4 vs older versions?

PLDEVICES = -DPLMETA -DNULLDEV -DXTERM -DTEK4010 -DTEK4107 -DDG300 -DPS
	    -DXFIG -DLJII -DHP7470 -DHP7580 -DIMP DEF_XWIN() DEF_TK()

CFLAGS	= -c $(DBL_FLAG_C) $(DEBUG_FLAG_C) $(OPT_FLAG_C) $(SYS_FLAGS_C) \
	     $(PROFILE_FLAG_C)

FFLAGS	= -c $(DBL_FLAG_F) $(DEBUG_FLAG_F) $(OPT_FLAG_F) $(SYS_FLAGS_F) \
	     $(PROFILE_FLAG_F)

LIBC	= if_tk({$(LIB_TK)}) if_xwin({$(LIB_XWIN)}) \
	 -lsocket -lsockdns -lsockhost -lnsl
LIBF	= if_tk({$(LIB_TK)}) if_xwin({$(LIB_XWIN)}) \
	 -lsocket -lSOCkdns

LDCFLAGS= $(PROFILE_FLAG_LC) $(LIBC) -lm
LDFFLAGS= $(PROFILE_FLAG_LF) $(LIBF) -lm

#----------------------------------------------------------------------#
})if_irix({
#		IRIX definitions (SGI machines)

PLDEVICES = -DPLMETA -DNULLDEV -DXTERM -DTEK4010 -DTEK4107 -DDG300 -DPS \
	    -DXFIG -DLJII -DHP7470 -DHP7580 -DIMP DEF_XWIN() DEF_TK()

CFLAGS	= -c -ansi $(DBL_FLAG_C) $(DEBUG_FLAG_C) $(OPT_FLAG_C) $(SYS_FLAGS_C) \
	     $(PROFILE_FLAG_C)

FFLAGS	= -c $(DBL_FLAG_F) $(DEBUG_FLAG_F) $(OPT_FLAG_F) $(SYS_FLAGS_F) \
	     $(PROFILE_FLAG_F)

LIBC	= if_tk({$(LIB_TK)}) if_xwin({$(LIB_XWIN)})
LIBF	= if_tk({$(LIB_TK)}) if_xwin({$(LIB_XWIN)}) 

LDCFLAGS= $(PROFILE_FLAG_LC) $(LIBC) -lm
LDFFLAGS= $(PROFILE_FLAG_LF) $(LIBF) -lm

#----------------------------------------------------------------------#
})if_sx({
#		NEC Super-UX definitions

SYS_FLAGS_C = -hansi

PLDEVICES = -DPLMETA -DNULLDEV -DXTERM -DTEK4010 -DTEK4107 -DDG300 -DPS \
	    -DXFIG -DLJII -DHP7470 -DHP7580 -DIMP DEF_XWIN() DEF_TK()

CFLAGS	= -c $(DBL_FLAG_C) $(DEBUG_FLAG_C) $(OPT_FLAG_C) $(SYS_FLAGS_C) \
	     $(PROFILE_FLAG_C)

FFLAGS	= -c $(DBL_FLAG_F) $(DEBUG_FLAG_F) $(OPT_FLAG_F) $(SYS_FLAGS_F) \
	     $(PROFILE_FLAG_F)

LIBC	= if_tk({$(LIB_TK)}) if_xwin({$(LIB_XWIN)})
LIBF	= if_tk({$(LIB_TK)}) if_xwin({$(LIB_XWIN)})

LDCFLAGS= $(PROFILE_FLAG_LC) $(LIBC) -lm
LDFFLAGS= $(PROFILE_FLAG_LF) $(LIBF) -lm

#----------------------------------------------------------------------#
})if_dgux({
#		DG/UX definitions

PLDEVICES = -DPLMETA -DNULLDEV -DXTERM -DTEK4010 -DTEK4107 -DPS -DXFIG \
	     DEF_XWIN() DEF_TK()
SYS_FLAGS_C = -Dunix -DSTUB_LINKAGE=STUB_LAU -ansi
SYS_FLAGS_F = -novms

F77	= ghf77
CFLAGS	= -c $(DBL_FLAG_C) $(DEBUG_FLAG_C) $(OPT_FLAG_C) $(SYS_FLAGS_C) \
	     $(PROFILE_FLAG_C)

FFLAGS	= -c $(DBL_FLAG_F) $(DEBUG_FLAG_F) $(OPT_FLAG_F) $(SYS_FLAGS_F) \
	     $(PROFILE_FLAG_F)

LIBC	= if_tk({$(LIB_TK)}) if_xwin({$(LIB_XWIN)})
LIBF	= if_tk({$(LIB_TK)}) if_xwin({$(LIB_XWIN)})

LDCFLAGS= -ansi $(PROFILE_FLAG_LC) $(LIBC) -lm
LDFFLAGS= -ansi $(PROFILE_FLAG_LF) $(LIBF) -lm

#----------------------------------------------------------------------#
})if_linux({
#		LINUX definitions

define({TK},)

PLDEVICES = -DPLMETA -DNULLDEV -DPS -DLJII DEF_XWIN() DEF_TK()
SYS_FLAGS_C =

CC	= gcc
F77	= 
CFLAGS	= -c $(DBL_FLAG_C) $(DEBUG_FLAG_C) $(OPT_FLAG_C) $(SYS_FLAGS_C) \
	     $(PROFILE_FLAG_C)

FFLAGS	= -c $(DBL_FLAG_F) $(DEBUG_FLAG_F) $(OPT_FLAG_F) $(SYS_FLAGS_F) \
	     $(PROFILE_FLAG_F)

LIBC	= if_tk({$(LIB_TK)}) if_xwin({$(LIB_XWIN)})
LIBF	= if_tk({$(LIB_TK)}) if_xwin({$(LIB_XWIN)})

LDCFLAGS= -ansi $(PROFILE_FLAG_LC) $(LIBC) -lm
LDFFLAGS= -ansi $(PROFILE_FLAG_LF) $(LIBF) -lm

#----------------------------------------------------------------------#
})if_next({
#               NeXT definitions

PLDEVICES = -DNEXT -DPS -DPLMETA -DNULLDEV

# You can use the NOBRAINDEAD option here if you wish
# If you use it, you may want to define NOBRAINDEAD in plplot.h
# before moving it to it's permanent location.

#SYS_FLAGS_C = -Dunix -DSTUB_LAU -DNOBRAINDEAD
SYS_FLAGS_C = -Dunix -DSTUB_LAU

# Suggested locations:
#  libplplotf.a in /usr/local/lib/
#  fonts        in /usr/local/lib/plplot/
#  plrender     in /usr/local/bin/
#  *.h          in /usr/local/include/
#
# cc will automatically look in these locations for the libraries and
# include files, so you can compile with a minimum of command line
# options (just -lplplotf).

CC      = cc
F77	= f77	# I've tested Fortran compatibility using f2c only - amr

CFLAGS	= -c $(DBL_FLAG_C) $(DEBUG_FLAG_C) $(OPT_FLAG_C) $(SYS_FLAGS_C) \
	     $(PROFILE_FLAG_C)

FFLAGS	= -c $(DBL_FLAG_F) $(DEBUG_FLAG_F) $(OPT_FLAG_F) $(SYS_FLAGS_F) \
	     $(PROFILE_FLAG_F)

LIBC	= if_tk({$(LIB_TK)}) if_xwin({$(LIB_XWIN)})
LIBF	= if_tk({$(LIB_TK)}) if_xwin({$(LIB_XWIN)})

LDCFLAGS= $(LIBC) -lm
LDFFLAGS= $(LIBF) -lm

#----------------------------------------------------------------------#
})if_aix({
#		A/IX definitions

define({TK},)

PLDEVICES = -DPLMETA -DNULLDEV -DXTERM -DTEK4010 -DTEK4107 -DPS -DXFIG \
	     DEF_XWIN() DEF_TK()
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
CFLAGS	= -c $(DBL_FLAG_C) $(DEBUG_FLAG_C) $(OPT_FLAG_C) $(SYS_FLAGS_C) \
	     $(PROFILE_FLAG_C)

FFLAGS	= -c $(DBL_FLAG_F) $(DEBUG_FLAG_F) $(OPT_FLAG_F) $(SYS_FLAGS_F) \
	     $(PROFILE_FLAG_F)

LIBC	= if_tk({$(LIB_TK)}) if_xwin({$(LIB_XWIN)})
LIBF	= if_tk({$(LIB_TK)}) if_xwin({$(LIB_XWIN)})

LDCFLAGS= $(PROFILE_FLAG_C) $(LIBC) -lm
LDFFLAGS= $(PROFILE_FLAG_F) $(LIBF) -lm

#----------------------------------------------------------------------#
})if_unicos({
#	UNICOS defs.

define({TK},)

PLDEVICES = -DPLMETA -DNULLDEV -DXTERM -DXWIN -DTEK4010 -DTEK4107 -DDG300 \
	    -DPS -DXFIG -DLJII -DHP7470 -DHP7580 -DIMP DEF_XWIN() DEF_TK()

SYS_FLAGS_C =

# These settings are appropriate for UNICOS 6.x.
# Here we assume that 'cc' is the standard C compiler.

F77	= cf77
LDF	= segldr

if_prof({
PROFILE_FLAG_C = -Gp
PROFILE_FLAG_LC = -lprof -lsci
})

OPT_FLAG_F	= 

CFLAGS	= -c $(DBL_FLAG_C) $(DEBUG_FLAG_C) $(OPT_FLAG_C) $(SYS_FLAGS_C) \
	     $(PROFILE_FLAG_C)

FFLAGS	= -c $(DBL_FLAG_F) $(DEBUG_FLAG_F) $(OPT_FLAG_F) $(SYS_FLAGS_F) \
	     $(PROFILE_FLAG_F)

LIBC	= if_tk({$(LIB_TK)}) if_xwin({$(LIB_XWIN)})
LIBF	= if_tk({$(LIB_TK)}) if_xwin({$(LIB_XWIN)})

LDCFLAGS= $(PROFILE_FLAG_C) $(LIBC) -lm -lnet
LDFFLAGS= $(PROFILE_FLAG_F) $(LIBF) -lm -lnet

#----------------------------------------------------------------------#
})if_alphaosf({
# DEC Alpha AXP OSF/1 (tested on version 1.2).  Note that the c optimize
# flag is set to -O1, it appears that higher levels of optimization will
# mess up some diagonal dashed lines.  The double precision flags have not
# been tested in an actual library build.

PLDEVICES = -DPLMETA -DNULLDEV -DXTERM -DTEK4010 -DTEK4107 -DPS -DXFIG \
	     DEF_XWIN() DEF_TK()

SYS_FLAGS_C = -std

if_opt({
OPT_FLAG_C = -O1
})

if_dbl({dnl
DBL_FLAG_F      = -r8
})dnl

if_prof({
PROFILE_FLAG_F = -p
PROFILE_FLAG_LF = -p
})

CFLAGS	= -c $(DBL_FLAG_C) $(DEBUG_FLAG_C) $(OPT_FLAG_C) $(SYS_FLAGS_C) \
	     $(PROFILE_FLAG_C)

FFLAGS	= -c $(DBL_FLAG_F) $(DEBUG_FLAG_F) $(OPT_FLAG_F) $(SYS_FLAGS_F) \
	     $(PROFILE_FLAG_F)

LIBC	= if_tk({$(LIB_TK)}) if_xwin({$(LIB_XWIN)})
LIBF	= if_tk({$(LIB_TK)}) if_xwin({$(LIB_XWIN)})

LDCFLAGS= $(OPENWIN_DIR) $(PROFILE_FLAG_LC) $(LIBC) -lm
LDFFLAGS= $(OPENWIN_DIR) $(PROFILE_FLAG_LF) $(LIBF) -lm

})
#----------------------------------------------------------------------#
# Finish up definitions for Unix-like systems.
# These may depend on system-specific settings.

if_unix({

# Library names
#
# Single precision: libplplotf
# Double precision: libplplotd
# Add suffix of:
#	b	for baseline library (no X or TK)
#	X	with Xlib included
#	tk	with Xlib and tk included
#
# Build those best suited to your system and softlink to libplplotf
# and libplplotd to create the system default.

if_dbl({
TAG_PREC	= d
},{
TAG_PREC	= f
})

if_tk({
TAG_INCLUDE	= tk
},{
if_xwin({
TAG_INCLUDE	= X
},{
TAG_INCLUDE	= b
})})

PLLIB_MAIN	= $(PLLIB_PATH)libplplot$(TAG_PREC)$(TAG_INCLUDE).$(LIBSUF)
PLLIB_LINK	= -lplplot$(TAG_PREC)$(TAG_INCLUDE)

PLLIB_C		= $(PLLIB_MAIN)
PLLIB_F		= $(PLLIB_MAIN)

PLLIB_LDC	= $(PLLIB_MAIN) 
PLLIB_LDF	= $(PLLIB_MAIN) 

# These are used for making the install versions of plrender & plserver

LIB_SEARCH	= -L$(INSTALL_DIR) if_tk({-L$(TK_LINKDIR)}) 
LIB_INSTALL	= $(LIB_SEARCH) $(PLLIB_LINK)

})

#----------------------------------------------------------------------#
if_amiga({
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

})
#----------------------------------------------------------------------#
# Object file macros
# Main sources.

OBJ =	\
	pdfutils.o \
	plargs.o \
	plbox.o \
	plcont.o \
	plcore.o \
	plctest.o \
	plctrl.o \
	plcvt.o \
	pldtik.o \
	plfill.o \
	plfont.o \
	plhist.o \
	plline.o \
	plot3d.o \
	plpage.o \
	plsdef.o \
	plshade.o \
	plstream.o \
	plstring.o \
	plsym.o \
	pltick.o \
	plvpor.o \
	plwind.o 

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
	sc3d.o \
	sccont.o \
	scstubs.o

# Fortran language stubs for linking Plplot to Fortran.

FSTUB_OBJ = \
	strutil.o \
	sfstubs.o 

# System-specific files.

# TK

TK_OBJ = \
	tk.o \
	tkshell.o

# TK server

SERVER_OBJ = \
	plserver.o \
	plframe.o \
	plr.o

# Amiga

AMIGA_OBJ = \
	pla_wn.o \
	pla_pr.o \
	pla_iff.o \
	pla_menu.o \
	plsupport.o 

# Drivers

DRIVERS_OBJ = \
	if_tk({$(TK_OBJ)}) \
	dg300.o \
	hp7470.o \
	hp7580.o \
	impress.o \
	ljii.o \
	next.o \
	null.o \
	plbuf.o \
	plmeta.o \
	ps.o \
	tek.o \
	xfig.o \
	xwin.o

#----------------------------------------------------------------------#
# Rules
# Placed here to ensure all macros are defined.

.c.o:
	$(CC) $(CFLAGS) $*.c
.f.o:
	$(F77) $(FFLAGS) $*.f

#----------------------------------------------------------------------#
# Targets
# The default is now to make all the object libraries, plrender, and
# plserver (if the TK driver is being used).  Other targets:
#
#	links		make soft links to source files (must be done first)
#	libs		object libraries
#	plrender
#	plserver
#	cdemos		all the C demos
#	fdemos		all the Fortran demos
#	x??[cf]		demo number ??, C or fortran version
#	clean		cleans up demo executables, output files
#	realclean	cleans up object files, soft-links
#	install		installs files user will need into public location

default: plrender if_tk({plserver}) libs

everything: plrender if_tk({plserver}) libs

libs:	$(PLLIB_MAIN)

# This serves to make soft links to the source files automatically the
# first time you run make.  Once the link to plplotP.h is made, it will
# never be made again since ".dummy" is older.

if_unix({
plplotP.h: .dummy
	make links
})

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
	$(BUILD) $(PLLIB_MAIN) $(OBJ) $(DRIVERS_OBJ)
	if_ranlib({ranlib $(PLLIB_MAIN)})
},{
$(PLLIB_MAIN):	$(OBJ) $(DRIVERS_OBJ) $(CSTUB_OBJ) $(FSTUB_OBJ)
	-rm $(PLLIB_MAIN)
	$(BUILD) $(PLLIB_MAIN) $(OBJ) $(DRIVERS_OBJ) $(CSTUB_OBJ) $(FSTUB_OBJ)
	if_ranlib({ranlib $(PLLIB_MAIN)})
})})

if_amiga({
$(PLLIB_MAIN):	GST $(OBJ) $(DRIVERS_OBJ) $(AMIGA_OBJ)
	skshbin:join $(OBJ)         t:plobj.lib
	skshbin:join $(DRIVERS_OBJ) t:pldrivers.lib
	skshbin:join $(AMIGA_OBJ)   t:plamiga.lib
	skshbin:join t:plobj.lib t:pldrivers.lib t:plamiga.lib $(PLLIB_MAIN)
	-delete t:plobj.lib t:pldrivers.lib t:plamiga.lib
})
#----------------------------------------------------------------------#
# Font files.
# Note it is no longer necessary to actually make these since as of
# PLPLOT 4.99a the font files are portable.
#
# sfont - standard font
# xfont - extended fonts

fonts:	sfont xfont

sfont:	$(PLFNT_PATH)plstnd5.fnt
xfont:	$(PLFNT_PATH)plxtnd5.fnt

$(PLFNT_PATH)plstnd.fnt: stndfont.o pdfutils.o $(FONT_OBJ)
	$(LDC) -o stndfont stndfont.o pdfutils.o $(FONT_OBJ) $(LDCFLAGS)
	-./stndfont
	mv *.fnt $(PLFNT_DIR)

$(PLFNT_PATH)plxtnd.fnt: xtndfont.o pdfutils.o $(FONT_OBJ)
	$(LDC) -o xtndfont xtndfont.o pdfutils.o $(FONT_OBJ) $(LDCFLAGS)
	-./xtndfont
	mv *.fnt $(PLFNT_DIR)

#----------------------------------------------------------------------#
# Object file dependencies.

# source files

plargs.o:	plplotP.h plplot.h plstream.h plargs.c
plbox.o:	plplotP.h plplot.h plbox.c 
plcont.o:	plplotP.h plplot.h plcont.c 
plctest.o:	plplotP.h plplot.h plctest.c 
plctrl.o:	plplotP.h plplot.h plctrl.c 
plcvt.o:	plplotP.h plplot.h plcvt.c 
pldtik.o:	plplotP.h plplot.h pldtik.c 
plfill.o:	plplotP.h plplot.h plfill.c 
plfont.o:	plplotP.h plplot.h plfont.c 
plhist.o:	plplotP.h plplot.h plhist.c 
plline.o:	plplotP.h plplot.h plline.c 
plot3d.o:	plplotP.h plplot.h plot3d.c 
plpage.o:	plplotP.h plplot.h plpage.c 
plsdef.o:	plplotP.h plplot.h plsdef.c 
plshade.o:	plplotP.h plplot.h plshade.c 
plstream.o:	plplotP.h plplot.h plstream.h plstream.c
plstring.o:	plplotP.h plplot.h plstring.c 
plsym.o:	plplotP.h plplot.h plsym.c 
pltick.o:	plplotP.h plplot.h pltick.c 
plvpor.o:	plplotP.h plplot.h plvpor.c 
plwind.o:	plplotP.h plplot.h plwind.c 
pdfutils.o:	plplotP.h plplot.h pdfutils.c

# C language stubs for linking Plplot to Fortran.

sc3d.o:		plstubs.h plplotP.h plplot.h sc3d.c
sccont.o:	plstubs.h plplotP.h plplot.h sccont.c
scstubs.o:	plstubs.h plplotP.h plplot.h scstubs.c

# Fortran language stubs for linking Plplot to Fortran.

strutil.o:	strutil.f
sfstubs.o:	sfstubs.f

# Amiga

if_amiga({

pla_pr.o:	plplotP.h plplot.h drivers.h plamiga.h pla_pr.c
pla_iff.o:	plplotP.h plplot.h drivers.h plamiga.h pla_iff.c
pla_menu.o:	plplotP.h plplot.h plamiga.h pla_menu.c
plsupport.o:	plplotP.h plplot.h plamiga.h plsupport.c
pla_wn.o:	plplotP.h plplot.h drivers.h plamiga.h pla_wn.c

ifdef({GST},{
GST :		plplotP.h plplot.h plamiga.h
	gst unload GST
	$(CC) makegst GST $(CFLAGS) $(PLDEVICES) pla_gstskel.c
})
})

#----------------------------------------------------------------------#
# Explicit rules
#
# plfont.c may have font flags passed in

plfont.o:	plplotP.h plplot.h plfont.c
	$(CC) $(CFLAGS) $(FONTFLAG) plfont.c

# plcore.c and all the drivers need to know $(PLDEVICES).  The guts
# of the driver routine are not compiled if its name is not present in
# the device list.  You may want to leave drivers for specific systems
# (Amiga, MS-DOS, OS/2, etc) out of this list.

plcore.o:	plplotP.h plplot.h plcore.h drivers.h plstream.h plcore.c
	$(CC) $(CFLAGS) $(PLDEVICES) plcore.c

dg300.o:	plplotP.h plplot.h plstream.h drivers.h dg300.c
	$(CC) $(CFLAGS) $(PLDEVICES) dg300.c

hp7470.o:	plplotP.h plplot.h plstream.h drivers.h hp7470.c
	$(CC) $(CFLAGS) $(PLDEVICES) hp7470.c

hp7580.o:	plplotP.h plplot.h plstream.h drivers.h hp7580.c
	$(CC) $(CFLAGS) $(PLDEVICES) hp7580.c

impress.o:	plplotP.h plplot.h plstream.h drivers.h impress.c
	$(CC) $(CFLAGS) $(PLDEVICES) impress.c

ljii.o:		plplotP.h plplot.h plstream.h drivers.h ljii.c
	$(CC) $(CFLAGS) $(PLDEVICES) ljii.c

next.o:		plplotP.h plplot.h plstream.h drivers.h next.c
	$(CC) $(CFLAGS) $(PLDEVICES) next.c

null.o:		plplotP.h plplot.h plstream.h drivers.h null.c
	$(CC) $(CFLAGS) $(PLDEVICES) null.c

ps.o:		plplotP.h plplot.h plstream.h drivers.h ps.c
	$(CC) $(CFLAGS) $(PLDEVICES) ps.c

tek.o:		plplotP.h plplot.h plstream.h drivers.h tek.c
	$(CC) $(CFLAGS) $(PLDEVICES) tek.c

plbuf.o:	plplotP.h plplot.h plstream.h drivers.h metadefs.h plbuf.c
	$(CC) $(CFLAGS) plbuf.c

plmeta.o:	plplotP.h plplot.h plstream.h drivers.h metadefs.h plmeta.c
	$(CC) $(CFLAGS) $(PLDEVICES) plmeta.c

xfig.o:		plplotP.h plplot.h plstream.h drivers.h xfig.c
	$(CC) $(CFLAGS) $(PLDEVICES) xfig.c

xwin.o:		plplotP.h plplot.h plstream.h drivers.h xwin.h xwin.c
	$(CC) $(CFLAGS) $(PLDEVICES) xwin.c

# TK driver

tk.o:		plserver.h plplotP.h plplot.h plstream.h \
		drivers.h metadefs.h pdf.h plevent.h tk.c
	$(CC) -I $(TK_INCDIR) $(CFLAGS) $(PLDEVICES) tk.c

tkshell.o:	plserver.h plplotP.h plplot.h plstream.h tkshell.c
	$(CC) -I $(TK_INCDIR) $(CFLAGS) $(PLDEVICES) tkshell.c

#----------------------------------------------------------------------#
# Utility programs.

plrender.o:	plplotP.h plplot.h metadefs.h pdf.h plrender.c

pltek:		pltek.o
	$(LDC) $(STARTUP) pltek.o $(PLLIB_LDC) $(TO) $@ $(LDCFLAGS)

plrender:	$(PLLIB_MAIN) plrender.o
	$(LDC) $(STARTUP) plrender.o $(PLLIB_LDC) $(TO) $@ $(LDCFLAGS)

plserver.o:	plserver.h plplotP.h plplot.h plstream.h plserver.c
	$(CC) -I $(TK_INCDIR) $(CFLAGS) plserver.c

plframe.o:	plserver.h plplotP.h plplot.h plstream.h xwin.h plframe.c
	$(CC) -I $(TK_INCDIR) $(CFLAGS) plframe.c

plr.o:		plserver.h plplotP.h plplot.h plstream.h \
		metadefs.h pdf.h plevent.h plr.c
	$(CC) -I $(TK_INCDIR) $(CFLAGS) plr.c

plserver:	$(PLLIB_MAIN) $(SERVER_OBJ)
	$(LDC) $(STARTUP) $(SERVER_OBJ) $(PLLIB_LDC) $(TO) $@ $(LDCFLAGS)

#----------------------------------------------------------------------#
# Example programs, in c.

CDEMOS= x01c x02c x03c x04c x05c x06c x07c x08c x09c x10c x11c \
	x12c x13c x14c tutor

cdemos:	$(CDEMOS)

x01c:	$(PLLIB_C) x01c.o
	$(LDC) $(STARTUP) x01c.o $(PLLIB_LDC) $(TO) $@ $(LDCFLAGS)

x02c:	$(PLLIB_C) x02c.o
	$(LDC) $(STARTUP) x02c.o $(PLLIB_LDC) $(TO) $@ $(LDCFLAGS)

x03c:	$(PLLIB_C) x03c.o
	$(LDC) $(STARTUP) x03c.o $(PLLIB_LDC) $(TO) $@ $(LDCFLAGS)

x04c:	$(PLLIB_C) x04c.o
	$(LDC) $(STARTUP) x04c.o $(PLLIB_LDC) $(TO) $@ $(LDCFLAGS)

x05c:	$(PLLIB_C) x05c.o
	$(LDC) $(STARTUP) x05c.o $(PLLIB_LDC) $(TO) $@ $(LDCFLAGS)

x06c:	$(PLLIB_C) x06c.o
	$(LDC) $(STARTUP) x06c.o $(PLLIB_LDC) $(TO) $@ $(LDCFLAGS)

x07c:	$(PLLIB_C) x07c.o
	$(LDC) $(STARTUP) x07c.o $(PLLIB_LDC) $(TO) $@ $(LDCFLAGS)

x08c:	$(PLLIB_C) x08c.o
	$(LDC) $(STARTUP) x08c.o $(PLLIB_LDC) $(TO) $@ $(LDCFLAGS)

x09c:	$(PLLIB_C) x09c.o
	$(LDC) $(STARTUP) x09c.o $(PLLIB_LDC) $(TO) $@ $(LDCFLAGS)

x10c:	$(PLLIB_C) x10c.o
	$(LDC) $(STARTUP) x10c.o $(PLLIB_LDC) $(TO) $@ $(LDCFLAGS)

x11c:	$(PLLIB_C) x11c.o
	$(LDC) $(STARTUP) x11c.o $(PLLIB_LDC) $(TO) $@ $(LDCFLAGS)

x12c:	$(PLLIB_C) x12c.o
	$(LDC) $(STARTUP) x12c.o $(PLLIB_LDC) $(TO) $@ $(LDCFLAGS)

x13c:	$(PLLIB_C) x13c.o
	$(LDC) $(STARTUP) x13c.o $(PLLIB_LDC) $(TO) $@ $(LDCFLAGS)

x14c:	$(PLLIB_C) x14c.o
	$(LDC) $(STARTUP) x14c.o $(PLLIB_LDC) $(TO) $@ $(LDCFLAGS)

tutor:	$(PLLIB_C) tutor.o
	$(LDC) $(STARTUP) tutor.o $(PLLIB_LDC) $(TO) $@ $(LDCFLAGS)

#----------------------------------------------------------------------#
# Example programs, in Fortran.

FDEMOS = x01f x02f x03f x04f x05f x06f x07f x08f x09f x10f x11f x12f x13f

fdemos:	$(FDEMOS)

x01f:	$(PLLIB_F) x01f.o
	$(LDF) x01f.o $(PLLIB_LDF) $(TO) $@ $(LDFFLAGS)

x02f:	$(PLLIB_F) x02f.o
	$(LDF) x02f.o $(PLLIB_LDF) $(TO) $@ $(LDFFLAGS)

x03f:	$(PLLIB_F) x03f.o
	$(LDF) x03f.o $(PLLIB_LDF) $(TO) $@ $(LDFFLAGS)

x04f:	$(PLLIB_F) x04f.o
	$(LDF) x04f.o $(PLLIB_LDF) $(TO) $@ $(LDFFLAGS)

x05f:	$(PLLIB_F) x05f.o
	$(LDF) x05f.o $(PLLIB_LDF) $(TO) $@ $(LDFFLAGS)

x06f:	$(PLLIB_F) x06f.o
	$(LDF) x06f.o $(PLLIB_LDF) $(TO) $@ $(LDFFLAGS)

x07f:	$(PLLIB_F) x07f.o
	$(LDF) x07f.o $(PLLIB_LDF) $(TO) $@ $(LDFFLAGS)

x08f:	$(PLLIB_F) x08f.o
	$(LDF) x08f.o $(PLLIB_LDF) $(TO) $@ $(LDFFLAGS)

x09f:	$(PLLIB_F) x09f.o
	$(LDF) x09f.o $(PLLIB_LDF) $(TO) $@ $(LDFFLAGS)

x10f:	$(PLLIB_F) x10f.o
	$(LDF) x10f.o $(PLLIB_LDF) $(TO) $@ $(LDFFLAGS)

x11f:	$(PLLIB_F) x11f.o
	$(LDF) x11f.o $(PLLIB_LDF) $(TO) $@ $(LDFFLAGS)

x12f:	$(PLLIB_F) x12f.o
	$(LDF) x12f.o $(PLLIB_LDF) $(TO) $@ $(LDFFLAGS)

x13f:	$(PLLIB_F) x13f.o
	$(LDF) x13f.o $(PLLIB_LDF) $(TO) $@ $(LDFFLAGS)

#----------------------------------------------------------------------#
# First time only, should be executed from $(PLTMP_DIR)

if_unix({
links:
	-$(LN) \
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
		../{include}/*.h \
		../drivers/*.c \
		../drivers/tk/*.c \
		../drivers/tk/*.h \
		../drivers/tk/*.tcl \
		../drivers/tk/tclIndex \
		.
})
if_amiga({
links:
	copy /src/\#?.c ""
	copy /drivers/\#?.c ""
	copy /utils/\#?.c ""
	copy /examples/C/\#?.c ""
	copy /sys/amiga/src/\#?.c ""
})

if_unix({
clean:
	-rm $(CDEMOS) $(FDEMOS) *.plm* *.tek* *.ps

realclean:
	-rm $(CDEMOS) $(FDEMOS) *.o *.c *.h *.f *.plm* *.tek* *.ps \
	*.tcl tclIndex makefile
})

#----------------------------------------------------------------------#
# "install" target
# It is usually not difficult to get a system administrator to give you
# ownership of a directory under /usr/local, such as /usr/local/plplot.
# Since this is the most common case I make an install target for it.
# There must also be directories /usr/local/plplot/include and 
# /usr/local/plplot/tcl.

install:
	-cp ../lib/libplplot* ../lib/*.fnt $(INSTALL_DIR)
	-cd ..; cp README* Changes.log COPYRIGHTS ToDo $(INSTALL_DIR)
	-cd ../{include}; \
		cp plplotP.h plplot.h plplotio.h plevent.h plstream.h pdf.h \
		$(INSTALL_DIR)/{include}
if_shr({	$(LDC) plrender.o $(LIB_INSTALL) -o plrender $(LDCFLAGS)
if_tk({		$(LDC) $(SERVER_OBJ) $(LIB_INSTALL) -o plserver $(LDCFLAGS)
})})	-strip plrender
	-cp plrender $(INSTALL_DIR)
if_tk({	-strip plserver
	-cp plserver $(INSTALL_DIR)
	-cp ../drivers/tk/*.tcl ../drivers/tk/tclIndex $(INSTALL_DIR)/tcl
})
