#----------------------------------------------------------------------#
#
# Makefile for PLPLOT under VMS.
# Maurice LeBrun, IFS, University of Texas, Mar 1, 1991.
#
# Intended for use with Todd Aven's MAKE/VMS.
#
# The available output devices are controlled by the PLDEVICES variable,
# a list of defines for the C preprocessor (affecting only dispatch.c).
# When installing plplot you may wish to exclude devices not present
# on your system in order to reduce screen clutter.  The default setting
# is to include all supported devices.
#
# Problems/bugs:
#
# 1. There are no soft links in VMS, so we just copy all files to temporary
#    directory instead.  If you edit them, please remember to update the
#    master copy in the correct directory.
#
# 2. The dependencies on the header files have been omitted, for simplicity.
#    If you change any of the header files, assume that the whole package 
#    must be rebuilt.
#
# 3. The plot library has been broken into 3 parts, consisting of:
#    main sources & drivers, C-stubs, Fortran-stubs.  This came about 
#    because of problems with long command lines and the fact that you
#    can't concatenate object files made by VaxC with those made by Vax
#    Fortran.
#
# 4. The X window driver is not included, as I don't know what libraries
#    to link with when building the executable, nor if these libraries are
#    present on all vaxen.  If anyone wants to try their hand, feel free.
#
# Other notes:
#
# 1. The font locating code looks in the following places for the fonts:
#
#	current directory
#	lib:
#	sys$login:	(home directory)
#	PLFONTDIR
#
#    It is recommended that you store the fonts in a local directory during
#    installation.  When you have verified the package works as expected,
#    simply move the fonts to the appropriate system directory given by
#    PLFONTDIR and then all users will have access to the fonts (no
#    recompilation necessary).  PLFONTDIR should be a site-independent
#    directory specification, such as "sys$sysroot:[sysfont.plplot]".
#
# 2. Specify "debug" as the first target to set debugging flags, e.g.
#	$ make debug <target>
#
#----------------------------------------------------------------------#
# Directory structure.
# See right before rule declaration for plot library specifications.

PLFONTDIR="sys$sysroot:[sysfont.plplot]"

PLDIR_DIR= [-]
PLLIB_DIR= [-.LIB]
PLFNT_DIR= LIB:
PLTMP_DIR= [-.TMP]

SYS_LIBS= sys$library:vaxcrtl/lib

PLDEVICES = /define=(META, XTERM, XWIN, TEKT, DG300, TEKF, PS, LJII, \
	  HP7470, HP7580, IMP)

PLDEVICES = /define=(META, XTERM, TEKT, TEKF, PS)

#----------------------------------------------------------------------#
# Compiler/linker macros.
#
# The 'fflags', 'cflags' and 'lflags' symbols are for debugging.  
# VMS make lets us specify these easily from the command line as a separate
# target ("debug"), but it must come first, e.g. $ make debug libs.

M4	= m4
CC	= cc 'cflags'
CFLAGS	= /define=(BRAINDEAD)
FONTFLAG=

F77	= fortran 'fflags'
FFLAGS	= 
LD	= link 'lflags'
LINK	= $(LD)
LFLAGS = 
CLFLAGS = 
FLFLAGS = 

#----------------------------------------------------------------------#
# Object file macros
# Main sources.

OBJ =	\
	base.obj, \
	convert.obj, \
	genlin.obj, \
	move.obj, \
	pladv.obj, \
	plaxes.obj, \
	plbeg.obj, \
	plbin.obj, \
	plbox.obj, \
	plbox3.obj, \
	plccal.obj, \
	plcntr.obj, \
	plcol.obj, \
	plcont.obj, \
	plconf.obj, \
	pldtik.obj, \
	plend.obj, \
	plenv.obj, \
	plerr.obj, \
	plexit.obj, \
	plfill.obj, \
	plfont.obj, \
	plform.obj, \
	plgra.obj, \
	plgrid3.obj, \
	plgspa.obj, \
	plhist.obj, \
	pljoin.obj, \
	pllab.obj, \
	pllclp.obj, \
	plline.obj, \
	pllsty.obj, \
	plmesh.obj, \
	plmtex.obj, \
	plnxtv.obj, \
	plot3d.obj, \
	plpat.obj, \
	plpsty.obj, \
	plptex.obj, \
	plrot.obj, \
	plsdef.obj, \
	plside3.obj, \
	plstar.obj, \
	plstyl.obj, \
	plsvpa.obj, \
	plsym.obj, \
	plt3zz.obj, \
	pltick.obj, \
	plvasp.obj, \
	plvpas.obj, \
	plvpor.obj, \
	plvsta.obj, \
	plw3d.obj, \
	plwid.obj, \
	plwind.obj, \
	plxybx.obj, \
	plzbx.obj, \
	string.obj 

# Drivers
# For VMS, use Unix drivers.

UNIX_OBJ = \
	dg300.obj, \
	dispatch.obj, \
	hp7470.obj, \
	hp7580.obj, \
	impress.obj, \
	ljii.obj, \
	ps.obj, \
	tek.obj, \
	plmeta.obj, \
	pdfutils.obj, \
	xfig.obj, \
	xterm.obj

# Support files for font generators.

FONT_OBJ = \
	font01.obj, \
	font02.obj, \
	font03.obj, \
	font04.obj, \
	font05.obj, \
	font06.obj, \
	font07.obj, \
	font08.obj, \
	font09.obj, \
	font10.obj, \
	font11.obj

# C language stubs for linking Plplot to Fortran.

CSTUB_OBJ = \
	long2str.obj, \
	sc3d.obj, \
	scconf.obj, \
	scstubs.obj

# Fortran language stubs for linking Plplot to Fortran.

FSTUB_OBJ = \
	islen.obj, \
	sfstubs.obj, \
	str2int.obj

#----------------------------------------------------------------------#
# Plot library specifications.

PLLIB_MAIN=	$(PLLIB_DIR)libplplotf1.obj
PLLIB_CSTUB=	$(PLLIB_DIR)libplplotf2.obj
PLLIB_FSTUB=	$(PLLIB_DIR)libplplotf3.obj

TERM_OBJ=	$(UNIX_OBJ)

PLLIB_C=	$(PLLIB_MAIN)

PLLIB_ALL=	$(PLLIB_FSTUB), \
		$(PLLIB_CSTUB), \
		$(PLLIB_MAIN)

#----------------------------------------------------------------------#
# Rules
# Placed here to ensure all macros are defined.

*.obj:	*.c
	write sys$output "  Compiling $*.c"
	$(CC) $(CFLAGS) $*.c

*.obj:	*.f
	write sys$output "  Compiling $*.f"
	$(F77) $(FFLAGS) /object=$@  $*.f

# The .exe rule is for all example programs.
# Just link with all the plot libraries to keep things simple.

*.exe: *.obj
	write sys$output "  Linking $*.obj"
	$(LINK) $(LFLAGS) /exe=$@ \
		$*.obj, $(PLLIB_ALL), $(SYS_LIBS)

#----------------------------------------------------------------------#
# Targets
# The default is to make all the object libraries.
# Abbreviations are used so that invocation from command line is short.
# You can make everything (except demos) by typing 'make everything'
# but you must create the links ('make links') beforehand.

default: libs
everything: libs fonts plrender scan

libs:	libc libfs
libc:	$(PLLIB_MAIN) $(PLLIB_CSTUB)
libfs:	$(PLLIB_FSTUB)

#----------------------------------------------------------------------#
# Plot libraries.
#
# Note -- don't actually use libraries, just concatenated object files.
# Libraries cause lots of grief on some systems because often (reason
# unknown) a structure that needs to be initialized is not (happens on 
# A/IX, VMS).  Fortunately package is reasonably small, especially if
# you compile with debugging off.

$(PLLIB_MAIN):	$(OBJ), $(TERM_OBJ)
	write sys$output "Updating $(PLLIB_MAIN)"
	copy	$(OBJ)		$(PLLIB_MAIN)
	append	$(TERM_OBJ)	$(PLLIB_MAIN)
	append	$(CSTUB_OBJ)	$(PLLIB_CSTUB)

$(PLLIB_FSTUB):	$(FSTUB_OBJ)
	write sys$output "Updating $(PLLIB_FSTUB)"
	copy	$(FSTUB_OBJ)	$(PLLIB_FSTUB)

#----------------------------------------------------------------------#
# Font files.
# Note 6 font files actually created.
# sfont - standard font
# xfont - extended fonts

fonts:	sfont xfont

sfont:	plstnd.fnt
xfont:	plxtnd.fnt

plstnd.fnt: stndfont.obj, $(FONT_OBJ)
	$(LINK) $(LFLAGS) \
	 /exe=stndfont.exe stndfont.obj, $(FONT_OBJ), $(SYS_LIBS)
	run stndfont.exe
	rename *.fnt,*.idx,*.lkp $(PLFNT_DIR)

plxtnd.fnt: xtndfont.obj, $(FONT_OBJ)
	$(LINK) $(LFLAGS) \
	 /exe=xtndfont.exe xtndfont.obj, $(FONT_OBJ), $(SYS_LIBS)
	run xtndfont.exe
	rename *.fnt,*.idx,*.lkp $(PLFNT_DIR)

#----------------------------------------------------------------------#
# Explicit rules
#
# plfontld.c is the only file which needs to know $(PLFONTDIR)

plfontld.obj: plfontld.c
	$(CC) $(CFLAGS) $(FONTFLAG) plfontld.c

# dispatch.c is the only file which needs to know $(PLDEVICES)

dispatch.obj: dispatch.c
	$(CC) $(CFLAGS) $(PLDEVICES) dispatch.c

#----------------------------------------------------------------------#
# Utility programs.

scan:	scan.exe
scan.exe:	scan.c
	$(CC) $(CFLAGS) scan.c
	$(LINK) /exe=scan.exe \
		scan.obj, sys$library:vaxcrtl/lib

plrender: plrender.exe
plrender.exe: plrender.obj, getopt.obj
	$(LINK) /exe=plrender.exe \
		plrender.obj, getopt.obj, $(PLLIB_C), sys$library:vaxcrtl/lib

#----------------------------------------------------------------------#
# Example programs, in c.

cdemos:	x01c x02c x03c x04c x05c x06c x07c x08c x09c x10c x11c x12c x13c

x01c:	x01c.exe
x02c:	x02c.exe
x03c:	x03c.exe
x04c:	x04c.exe
x05c:	x05c.exe
x06c:	x06c.exe
x07c:	x07c.exe
x08c:	x08c.exe
x09c:	x09c.exe
x10c:	x10c.exe
x11c:	x11c.exe
x12c:	x12c.exe
x13c:	x13c.exe

x01c.exe: x01c.obj $(PLLIB_ALL)
x02c.exe: x02c.obj $(PLLIB_ALL)
x03c.exe: x03c.obj $(PLLIB_ALL)
x04c.exe: x04c.obj $(PLLIB_ALL)
x05c.exe: x05c.obj $(PLLIB_ALL)
x06c.exe: x06c.obj $(PLLIB_ALL)
x07c.exe: x07c.obj $(PLLIB_ALL)
x08c.exe: x08c.obj $(PLLIB_ALL)
x09c.exe: x09c.obj $(PLLIB_ALL)
x10c.exe: x10c.obj $(PLLIB_ALL)
x11c.exe: x11c.obj $(PLLIB_ALL)
x12c.exe: x12c.obj $(PLLIB_ALL)
x13c.exe: x13c.obj $(PLLIB_ALL)

#----------------------------------------------------------------------#
# Example programs, in Fortran.

fdemos: x01f x02f x03f x04f x05f x06f x07f x08f x09f x10f x11f x12f x13f

x01f:	x01f.exe
x02f:	x02f.exe
x03f:	x03f.exe
x04f:	x04f.exe
x05f:	x05f.exe
x06f:	x06f.exe
x07f:	x07f.exe
x08f:	x08f.exe
x09f:	x09f.exe
x10f:	x10f.exe
x11f:	x11f.exe
x12f:	x12f.exe
x13f:	x13f.exe

x01f.exe: x01f.obj $(PLLIB_ALL)
x02f.exe: x02f.obj $(PLLIB_ALL)
x03f.exe: x03f.obj $(PLLIB_ALL)
x04f.exe: x04f.obj $(PLLIB_ALL)
x05f.exe: x05f.obj $(PLLIB_ALL)
x06f.exe: x06f.obj $(PLLIB_ALL)
x07f.exe: x07f.obj $(PLLIB_ALL)
x08f.exe: x08f.obj $(PLLIB_ALL)
x09f.exe: x09f.obj $(PLLIB_ALL)
x10f.exe: x10f.obj $(PLLIB_ALL)
x11f.exe: x11f.obj $(PLLIB_ALL)
x12f.exe: x12f.obj $(PLLIB_ALL)
x13f.exe: x13f.obj $(PLLIB_ALL)

#----------------------------------------------------------------------#
# First time only, should be executed from $(PLTMP_DIR)

links:
	copy \
		[-.src]*.c, \
		[-.src.stubc]*.c, \
		[-.src.stubf]*.f, \
		[-.utils]*.c, \
		[-.fonts]*.c, \
		[-.include]*.h, \
		[-.drivers]*.c, \
		[-.examples.C]*.c, \
		[-.examples.f77]*.f \
		$(PLTMP_DIR)

#----------------------------------------------------------------------#
# Compiler and linker flags for debug option.
# These are done via DCL symbols to allow them to be changed easily from
# the make command line.  Specify "debug" as a target before others.

debug:
	fflags := /debug/noopt
	cflags := /debug/noopt
	lflags := /debug
