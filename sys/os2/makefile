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
FONTFLAG= "-DPLFONTDEV1=\"c:\\lib\\ \""

CC	= cl
CFLAGS	= /FPi87  /nologo /Od /Zi /AL
#CFLAGS	= /FPi87  /nologo /Ox /G2 /AL
F77	= fl
FFLAGS	= /FPi87  /nologo /Od /Zi /AL
#FFLAGS	= /FPi87  /nologo /Ox /G2 /AL

# OS/2	Enable -DOS2PM if you have purchassed this add-on driver.
#	Also, you'll need to enable it in the driver list below...
DRIVERS	= -DPS -DLJII -DPLMETA -DHP7470 -DIMP #-DOS2PM

# DOS
# DRIVERS	= -DPS -DLJII -DPLMETA -DHP7470 -DIMP -DVGA

# Rules
.SUFFIXES:
.SUFFIXES: .o .c .for

.c.o:
	$(CC) -c $(CFLAGS) /Fo$*.o $*.c 
.for.o:
	$(F77) -c $(FFLAGS) /Fo$*.o $*.for

# Object file macros

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

OS2_OBJ = \
	dispatch.o \
	pdfutils.o \
	hp7470.o \
	hp7580.o \
	impress.o \
	ljii.o \
	ps.o \
	tek.o \
	plmeta.o

# DOS
# Add dosvga.o to the above list.
# (Just copy the next line to right above plmeta.o, and strip the leading #.
#	dosvga.o \

# OS/2
# Take dosvga.o out of the list and add os2pm.o if you've purchassed it.
# (Just copy the next line to right above plmeta.o, and strip the leading #.
#	os2pm.o \


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
	sfstubs.o

# Library dependencies

libs:	libc libf
libc:	$(PLLIBS)\plplot.lib
libf:	$(PLLIBS)\plstub.lib

$(PLLIBS)\plplot.lib:	$(OBJ) $(OS2_OBJ)
	@echo Ready to build plplot.lib
	-@del plplot.lib
	lib @<<
plplot
y
+ $(OBJ:.o =.o &
+) &
+ $(OS2_OBJ:.o =.o &
+) ;
<<

$(PLLIBS)\plstub.lib:	$(CSTUB_OBJ) $(FSTUB_OBJ)
	@echo Ready to build plstub.lib
	-@del plstub.lib
	lib @<<
plstub
y
+ $(CSTUB_OBJ:.o =.o &
+) &
+ $(FSTUB_OBJ:.o =.o &
+) ;
<<

# In case you REALLY mean it!

force: links libs

# First time only, should be executed from $(PLTMP)

links: linkmain linkstubs

linkmain:
	copy ..\src\*.c .
	copy ..\include\*.h .
	copy ..\examples\c\*.c .
	copy ..\examples\f77\*.f .
	copy ..\drivers\*.* .
	copy ..\fonts\*.* .
	copy ..\utils\*.* .
	ren *.f *.for

linkstubs:	
	copy ..\sys\os2\stubf\*.* .
	copy ..\src\stubc\*.* .
	ren *.f *.for

# The metafile renderer.

plrender:
	cl $(CFLAGS) plrender.c getopt.c plplot.lib

#----------------------------------------------------------------------#
# Font files.
# Note 6 font files actually created.
# sfont - standard font
# xfont - extended fonts

fonts:	sfont xfont
	copy *.fnt $(PLFNT_DIR)

sfont:	$(PLFNT_DIR)plstnd.fnt
xfont:	$(PLFNT_DIR)plxtnd.fnt

$(PLFNT_DIR)plstnd.fnt:
	$(CC) $(CFLAGS:AL=AH) /Gt16 stndfont.c font*.c
	-stndfont

$(PLFNT_DIR)plxtnd.fnt:
	$(CC) $(CFLAGS:AL=AH) /Gt16 xtndfont.c font*.obj
	-xtndfont

#----------------------------------------------------------------------#
# Explicit rules
#
# plfont.c may have font flags passed in

plfont.o: plfont.c
	$(CC) -c $(CFLAGS) $(FONTFLAG)	/Fo$*.o $*.c

# dispatch.c and all the drivers need to know $(PLDEVICES).  The guts
# of the driver routine are not compiled if its name is not present in
# the device list.  You may want to leave drivers for specific systems
# (Amiga, MS-DOS, OS/2, etc) out of this list.

dispatch.o: dispatch.c
	$(CC) -c $(CFLAGS) $(DRIVERS) /Fo$*.o $*.c 

hp7470.o:	hp7470.c
	$(CC) -c $(CFLAGS) $(DRIVERS) /Fo$*.o $*.c 

hp7580.o:	hp7580.c
	$(CC) -c $(CFLAGS) $(DRIVERS) /Fo$*.o $*.c 

impress.o:	impress.c
	$(CC) -c $(CFLAGS) $(DRIVERS) /Fo$*.o $*.c 

ljii.o:	ljii.c
	$(CC) -c $(CFLAGS) $(DRIVERS) /Fo$*.o $*.c 

ps.o:	ps.c
	$(CC) -c $(CFLAGS) $(DRIVERS) /Fo$*.o $*.c 

tek.o:	tek.c
	$(CC) -c $(CFLAGS) $(DRIVERS) /Fo$*.o $*.c 

plmeta.o:	plmeta.c
	$(CC) -c $(CFLAGS) $(DRIVERS) /Fo$*.o $*.c 

#----------------------------------------------------------------------#
# Build all the demos

demos : cdemos fdemos

# Example programs, in c.

cdemos:	x01c x02c x03c x04c x05c x06c x07c x08c x09c x10c x11c \
	x12c x13c x14c tutor

x01c:	x01c.c
	$(CC) $(CFLAGS) x01c.c plplot.lib
x02c:	x02c.c
	$(CC) $(CFLAGS) x02c.c plplot.lib
x03c:	x03c.c
	$(CC) $(CFLAGS) x03c.c plplot.lib
x04c:	x04c.c
	$(CC) $(CFLAGS) x04c.c plplot.lib
x05c:	x05c.c
	$(CC) $(CFLAGS) x05c.c plplot.lib
x06c:	x06c.c
	$(CC) $(CFLAGS) x06c.c plplot.lib
x07c:	x07c.c
	$(CC) $(CFLAGS) x07c.c plplot.lib
x08c:	x08c.c
	$(CC) $(CFLAGS) x08c.c plplot.lib
x09c:	x09c.c
	$(CC) $(CFLAGS) x09c.c plplot.lib
x10c:	x10c.c
	$(CC) $(CFLAGS) x10c.c plplot.lib
x11c:	x11c.c
	$(CC) $(CFLAGS) x11c.c plplot.lib
x12c:	x12c.c
	$(CC) $(CFLAGS) x12c.c plplot.lib
x13c:	x13c.c
	$(CC) $(CFLAGS) x13c.c plplot.lib
x14c:	x14c.c
	$(CC) $(CFLAGS) x14c.c plplot.lib
tutor:	tutor.c
	$(CC) $(CFLAGS) tutor.c plplot.lib

# Example programs, in Fortran.

fdemos:	x01f x02f x03f x04f x05f x06f x07f x08f x09f x10f x11f x12f x13f

x01f:
	$(F77) $(FFLAGS) x01f.for plplot.lib plstub.lib
x02f:
	$(F77) $(FFLAGS) x02f.for plplot.lib plstub.lib
x03f:
	$(F77) $(FFLAGS) x03f.for plplot.lib plstub.lib
x04f:
	$(F77) $(FFLAGS) x04f.for plplot.lib plstub.lib
x05f:
	$(F77) $(FFLAGS) x05f.for plplot.lib plstub.lib
x06f:
	$(F77) $(FFLAGS) x06f.for plplot.lib plstub.lib
x07f:
	$(F77) $(FFLAGS) x07f.for plplot.lib plstub.lib
x08f:
	$(F77) $(FFLAGS) x08f.for plplot.lib plstub.lib
x09f:
	$(F77) $(FFLAGS) /Gt512 x09f.for plplot.lib plstub.lib
x10f:
	$(F77) $(FFLAGS) x10f.for plplot.lib plstub.lib
x11f:
	$(F77) $(FFLAGS) x11f.for plplot.lib plstub.lib
x12f:
	$(F77) $(FFLAGS) x12f.for plplot.lib plstub.lib
x13f:
	$(F77) $(FFLAGS) x13f.for plplot.lib plstub.lib
