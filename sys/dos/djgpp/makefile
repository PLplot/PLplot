#
# Makefile for PLPLOT
#
# This makefile builds PLPLOT using the DOS port of GNU CC 386 by DJ Delorie.
# In keeping with the GNU CC package, this makefile is written for use
# with NDMAKE.	See the GNU CC docs for info on obtaining these tools.
#
# Note that Fortran support in is the makefile and uses f2c and gcc,
#
# Questions or problems, send (e)mail to:
#	Geoffrey Furnish
#	furnish@fusion.ph.utexas.edu
#	Institute for Fusion Studies
#	Department of Physics
#	The University of Texas at Austin
#	Austin, Texas	78712

PLDIR	= c:\plplotp\ 
PLLIBS	= ..\lib
PLFONTS	= $(PLDIR)plfonts\ 
PLFNT_DIR=..\lib
PLTMP	= $(PLDIR)tmp\ 
FONTFLAG=
PLLIB	= plplt
PLSTB	= plstb

CC	= gcc
C	= -c
CFLAGS	= -DGNU386 -DNO_ANSI_LIBC
CC_FLAGS= $(C) $(CFLAGS)
F77	= 
FFLAGS	= 
FC_FLAGS	= $(C) $(FFLAGS)
LDC	= gcc
LDF	= gcc
LIBC	=  -L../lib -l$(PLLIB) -lm -lgrx
LIBF	=  -L../lib -l$(PLSTB) -l$(PLLIB) -lgrx -lf2c -lm

PLDEVICES	= -DNULLDEV -DPS -DLJII -DPLMETA -DHP7470 -DIMP -DGNUSVGA

# Rules
.SUFFIXES:
.SUFFIXES: .o .c .f

.c.o:
	$(CC) $(CC_FLAGS) $*.c 

.f.o:
#	$(F77) $(FC_FLAGS) $*.f
	f2c -A $*.f
	$(CC) $(CC_FLAGS) $*.c
	del $*.c

# Object file macros

OBJ =	\
	pdfutils.o \
	plargs.o \
	plbox.o \
	plcont.o \
	plcore.o \
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

DRIVERS_OBJ = \
	hp7470.o \
	hp7580.o \
	impress.o \
	ljii.o \
	null.o \
	plbuf.o \
	plmeta.o \
	ps.o \
	tek.o \
	xfig.o \
	gnusvga.o

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

# Library dependencies

default:	libc plrender libf

libs:	libc libf
libc:	$(PLLIBS)\lib$(PLLIB).a
libf:	$(PLLIBS)\lib$(PLSTB).a

$(PLLIBS)\lib$(PLLIB).a:	$(OBJ) $(DRIVERS_OBJ)
	@echo Ready to build lib$(PLLIB).a
	@>lib$(PLLIB).rf $?
	ar rvc lib$(PLLIB).a @lib$(PLLIB).rf
	copy lib$(PLLIB).a ..\lib
	ar rvs ..\lib\lib$(PLLIB).a

$(PLLIBS)\lib$(PLSTB).a:	$(CSTUB_OBJ) $(FSTUB_OBJ)
	@echo Ready to build lib$(PLSTB).a
	@>lib$(PLSTB).rf $?
	ar rvc lib$(PLSTB).a @lib$(PLSTB).rf
	copy lib$(PLSTB).a ..\lib
	ar rvs ..\lib\lib$(PLSTB).a

# In case you REALLY mean it!

force: links libs

# First time only, should be executed from $(PLTMP)

links: linkmain linkstubs

linkmain:
	copy ..\src\*.c .
	copy ..\include\*.h .
	copy ..\examples\c\*.c .
	copy ..\drivers\*.c .
	copy ..\fonts\*.* .
	copy ..\utils\*.* .
	copy ..\lib\*.fnt .
#	ren *.f *.for

linkstubs:	
	copy ..\src\stubc\*.c .
	copy ..\src\stubf\sgl\*.f .
	copy ..\examples\f77\sgl\*.f .
#	ren *.f *.for

#----------------------------------------------------------------------#
# Font files.
# Note 6 font files actually created.
# sfont - standard font
# xfont - extended fonts

fonts:	sfont xfont
	copy *.fnt $(PLFNT_DIR)

sfont:	$(PLFNT_DIR)plstnd.fnt
xfont:	$(PLFNT_DIR)plxtnd.fnt

$(PLFNT_DIR)plstnd.fnt: stndfont.o $(FONT_OBJ)
	@>stnd.rf stndfont.o $(FONT_OBJ)
	$(CC) $(CFLAGS) -o stndfont @stnd.rf $(LIBC)
	aout2exe stndfont
	-stndfont

$(PLFNT_DIR)plxtnd.fnt: xtndfont.o $(FONT_OBJ)
	@>xtnd.rf xtndfont.o $(FONT_OBJ)
	$(CC) $(CFLAGS) -o xtndfont @xtnd.rf $(LIBC)
	aout2exe xtndfont
	-xtndfont

##############################################################################
#
# Object file dependencies.

# source files

plargs.o:	plplotP.h plplot.h plstream.h plargs.c
plbox.o:	plplotP.h plplot.h plbox.c 
plcont.o:	plplotP.h plplot.h plcont.c 
plcvt.o:	plplotP.h plplot.h plcvt.c 
pldtik.o:	plplotP.h plplot.h pldtik.c 
plfill.o:	plplotP.h plplot.h plfill.c 
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
scstubs.o:	plstubs.h plplotP.h plplot.h scstubs.c
sccont.o:	plstubs.h plplotP.h plplot.h sccont.c


# Fortran language stubs for linking Plplot to Fortran.

strutil.o:	strutil.f
sfstubs.o:	sfstubs.f

#----------------------------------------------------------------------#
# Explicit rules
#
# plfont.c and plctrl.c need to know the install directory
# Also, plfont.c may have font flags passed in

plfont.o:	plplotP.h plplot.h plfont.c 
	$(CC) $(CC_FLAGS) plfont.c

plctrl.o:	plplotP.h plplot.h plctrl.c 
	$(CC) $(CC_FLAGS) plctrl.c

# plcore.c and all the drivers need to know $(PLDEVICES).  The guts
# of the driver routine are not compiled if its name is not present in
# the device list.  You may want to leave drivers for specific systems
# (Amiga, MS-DOS, OS/2, etc) out of this list.

plcore.o:	plplotP.h plplot.h plcore.h drivers.h plstream.h plcore.c
	$(CC) $(CC_FLAGS) $(PLDEVICES) plcore.c

dg300.o:	plplotP.h plplot.h plstream.h drivers.h dg300.c
	$(CC) $(CC_FLAGS) $(PLDEVICES) dg300.c

hp7470.o:	plplotP.h plplot.h plstream.h drivers.h hp7470.c
	$(CC) $(CC_FLAGS) $(PLDEVICES) hp7470.c

hp7580.o:	plplotP.h plplot.h plstream.h drivers.h hp7580.c
	$(CC) $(CC_FLAGS) $(PLDEVICES) hp7580.c

impress.o:	plplotP.h plplot.h plstream.h drivers.h impress.c
	$(CC) $(CC_FLAGS) $(PLDEVICES) impress.c

ljii.o:		plplotP.h plplot.h plstream.h drivers.h ljii.c
	$(CC) $(CC_FLAGS) $(PLDEVICES) ljii.c

null.o:		plplotP.h plplot.h plstream.h drivers.h null.c
	$(CC) $(CC_FLAGS) $(PLDEVICES) null.c

ps.o:		plplotP.h plplot.h plstream.h drivers.h ps.c
	$(CC) $(CC_FLAGS) $(PLDEVICES) ps.c

tek.o:		plplotP.h plplot.h plstream.h drivers.h tek.c
	$(CC) $(CC_FLAGS) $(PLDEVICES) tek.c

plbuf.o:	plplotP.h plplot.h plstream.h drivers.h metadefs.h plbuf.c
	$(CC) $(CC_FLAGS) plbuf.c

plmeta.o:	plplotP.h plplot.h plstream.h drivers.h metadefs.h plmeta.c
	$(CC) $(CC_FLAGS) $(PLDEVICES) plmeta.c

xfig.o:		plplotP.h plplot.h plstream.h drivers.h xfig.c
	$(CC) $(CC_FLAGS) $(PLDEVICES) xfig.c

gnusvga.o:	plplotP.h plplot.h plstream.h drivers.h gnusvga.c
	$(CC) $(CC_FLAGS) $(PLDEVICES) gnusvga.c

##############################################################################
#
# Utility programs.

plrender.o:	plplotP.h plplot.h metadefs.h pdf.h plrender.c

plrender:	plrender.o
	$(LDC) plrender.o -o $@ $(LIBC)
	strip plrender
	aout2exe plrender

#----------------------------------------------------------------------#
# Build all the demos

# To verify that PLPLOT works with C++, copy x01c.c to xa.cc and make xa

xa:	xa.cc
	$(CC) $(CFLAGS) -g $*.cc -o $@ -L../lib -l$(PLLIB) -lm -lgr -lpc
	strip $@
	aout2exe $@

# Example programs, in c.

cdemos:	x01c x02c x03c x04c x05c x06c x07c x08c x09c x10c x11c \
	x12c x13c x15c x16c tutor

x01c:	x01c.c
	$(CC) $(CFLAGS) $*.c -o $@ $(LIBC)
	strip $@
	aout2exe $@

x02c:	x02c.c
	$(CC) $(CFLAGS) $*.c -o $@ $(LIBC)
	strip $@
	aout2exe $@

x03c:	x03c.c
	$(CC) $(CFLAGS) $*.c -o $@ $(LIBC)
	strip $@
	aout2exe $@

x04c:	x04c.c
	$(CC) $(CFLAGS) $*.c -o $@ $(LIBC)
	strip $@
	aout2exe $@

x05c:	x05c.c
	$(CC) $(CFLAGS) $*.c -o $@ $(LIBC)
	strip $@
	aout2exe $@

x06c:	x06c.c
	$(CC) $(CFLAGS) $*.c -o $@ $(LIBC)
	strip $@
	aout2exe $@

x07c:	x07c.c
	$(CC) $(CFLAGS) $*.c -o $@ $(LIBC)
	strip $@
	aout2exe $@

x08c:	x08c.c
	$(CC) $(CFLAGS) $*.c -o $@ $(LIBC)
	strip $@
	aout2exe $@

x09c:	x09c.c
	$(CC) $(CFLAGS) $*.c -o $@ $(LIBC)
	strip $@
	aout2exe $@

x10c:	x10c.c
	$(CC) $(CFLAGS) $*.c -o $@ $(LIBC)
	strip $@
	aout2exe $@

x11c:	x11c.c
	$(CC) $(CFLAGS) $*.c -o $@ $(LIBC)
	strip $@
	aout2exe $@

x12c:	x12c.c
	$(CC) $(CFLAGS) $*.c -o $@ $(LIBC)
	strip $@
	aout2exe $@

x13c:	x13c.c
	$(CC) $(CFLAGS) $*.c -o $@ $(LIBC)
	strip $@
	aout2exe $@

x15c:	x15c.c
	$(CC) $(CFLAGS) $*.c -o $@ $(LIBC)
	strip $@
	aout2exe $@

x16c:	x16c.c
	$(CC) $(CFLAGS) $*.c -o $@ $(LIBC)
	strip $@
	aout2exe $@

tutor:	tutor.c
	$(CC) $(CFLAGS) $*.c -o $@ $(LIBC)
	strip $@
	aout2exe $@

##############################################################################
#
# Example programs, in Fortran.

fdemos:	x01f x02f x03f x04f x05f x06f x07f x08f x09f x10f x11f \
	x12f x13f x16f

x01f:	x01f.o
	$(LDF) $(FFLAGS) $@.o -o $@ $(LIBF)
	strip $@
	aout2exe $@

x02f:	x02f.o
	$(LDF) $(FFLAGS) $@.o -o $@ $(LIBF)
	strip $@
	aout2exe $@

x03f:	x03f.o
	$(LDF) $(FFLAGS) $@.o -o $@ $(LIBF)
	strip $@
	aout2exe $@

x04f:	x04f.o
	$(LDF) $(FFLAGS) $@.o -o $@ $(LIBF)
	strip $@
	aout2exe $@

x05f:	x05f.o
	$(LDF) $(FFLAGS) $@.o -o $@ $(LIBF)
	strip $@
	aout2exe $@

x06f:	x06f.o
	$(LDF) $(FFLAGS) $@.o -o $@ $(LIBF)
	strip $@
	aout2exe $@

x07f:	x07f.o
	$(LDF) $(FFLAGS) $@.o -o $@ $(LIBF)
	strip $@
	aout2exe $@

x08f:	x08f.o
	$(LDF) $(FFLAGS) $@.o -o $@ $(LIBF)
	strip $@
	aout2exe $@

x09f:	x09f.o
	$(LDF) $(FFLAGS) $@.o -o $@ $(LIBF)
	strip $@
	aout2exe $@

x10f:	x10f.o
	$(LDF) $(FFLAGS) $@.o -o $@ $(LIBF)
	strip $@
	aout2exe $@

x11f:	x11f.o
	$(LDF) $(FFLAGS) $@.o -o $@ $(LIBF)
	strip $@
	aout2exe $@

x12f:	x12f.o
	$(LDF) $(FFLAGS) $@.o -o $@ $(LIBF)
	strip $@
	aout2exe $@

x13f:	x13f.o
	$(LDF) $(FFLAGS) $@.o -o $@ $(LIBF)
	strip $@
	aout2exe $@

x16f:	x16f.o
	$(LDF) $(FFLAGS) $@.o -o $@ $(LIBF)
	strip $@
	aout2exe $@

##############################################################################

# cleanup targets

clean:
	-rm $(CDEMOS) $(FDEMOS) *.plm *.tek *.ps *.exe

realclean:
	-rm $(CDEMOS) $(FDEMOS) *.o *.c *.h *.f *.plm *.tek *.ps \
	 *.fnt Makefile plrender ..\lib\libpl*.* *.exe


