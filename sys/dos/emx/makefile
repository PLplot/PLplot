# Makefile for PLPLOT
#
# This makefile is for building PLPLOT using emx0.8h + gcc
#  - DOS+OS/2 port of GNU CC by Eberhard Mattes
#
# This makefile uses DMAKE (as advised in the emx documentaion)
#
# Note that Fortran support in is the makefile and uses f2c and gcc,
# but has not been tested
#
# Questions or problems, send (e)mail to:
#	<insert supporter's name here>

PLLIB	= plplt
PLSTB	= plstb

CC	= gcc
CFLAGS	= -ansi -Wall # -O2
F77	=
FFLAGS	=
LDC	= gcc
LDF	= gcc
LIBC	=  -L../lib -llib$(PLLIB) -lm -lgraph -lvesa
LIBF	=  -L../lib -llib$(PLSTB) -llib$(PLLIB) -lgraph -lvesa -lf2c -lm
BINDFLAGS= -aq -acim
BINDCMD=emxbind -bs -v

# PLDEVICES	= -DNULLDEV -DPS -DLJII -DPLMETA -DHP7470 -DIMP -DEMXVESA
PLDEVICES = -DPLMETA -DHP7470 -DEMXVESA

# Rules
.SUFFIXES:
.SUFFIXES: .o .c .f

.c.o:
	$(CC) -c $(CFLAGS) $*.c

.f.o:
#	$(F77) -c $(FFLAGS) $*.f
	-f2c -A $*.f
	$(CC) -c $(CFLAGS) $*.c
	-$(RM) $(RMFLAGS) $*.c

.c.exe:
	$(CC) $(CFLAGS) $*.c -o $* $(LIBC)
	$(BINDCMD) $* $(BINDFLAGS)
	-$(RM) $(RMFLAGS) $*

.f.exe:
#	$(F77) -c $(FFLAGS) $*.f
	f2c -A $*.f
	$(CC) $(CFLAGS) $*.c -o $* $(LIBC)
	$(BINDCMD) $* $(BINDFLAGS)
	-$(RM) $(RMFLAGS) $* $*.c


# Object file macros

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

DRIVERS_OBJ = \
	hpgl.o \
	impress.o \
	ljii.o \
	null.o \
	plbuf.o \
	plmeta.o \
	ps.o \
	tek.o \
	xfig.o \
	emxvga.o

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
libc:	../lib/lib$(PLLIB).a
libf:	../lib/lib$(PLSTB).a

../lib/lib$(PLLIB).a:	$(OBJ) $(DRIVERS_OBJ)
	ar rvc lib$(PLLIB).a @$(mktmp $(^:t"\n")\n)
	+copy lib$(PLLIB).a ..\lib\lib$(PLLIB).a
	ar rvs ../lib/lib$(PLLIB).a

../lib/lib$(PLSTB).a:	$(CSTUB_OBJ) $(FSTUB_OBJ)
	ar rvc lib$(PLLIB).a @$(mktmp $(^:t"\n")\n)
	+copy lib$(PLSTB).a ..\lib\lib$(PLSTB).a
	ar rvs ../lib/lib$(PLSTB).a

# In case you REALLY mean it!

force: links libs

# First time only, should be executed from $(PLDIR)tmp\

links: linkmain linkstubs

linkmain:
	+copy ..\src\*.c .
	+copy ..\include\*.h .
	+copy ..\examples\c\*.c .
	+copy ..\drivers\*.c .
	+copy ..\fonts\*.* .
	+copy ..\utils\*.* .
	+copy ..\lib\*.fnt .
	+copy ..\sys\dos\emx\*.c .

linkstubs:	
	+copy ..\src\stubc\*.c .
	+copy ..\src\stubf\sgl\*.f .
	+copy ..\examples\f77\sgl\*.f .

#----------------------------------------------------------------------#
# Font files.
# Note 6 font files actually created.
# sfont - standard font
# xfont - extended fonts

fonts:	sfont xfont
	+copy *.fnt ..\lib

sfont:	../lib/plstnd.fnt
xfont:	../lib/plxtnd.fnt

../lib/plstnd.fnt: stndfont.o $(FONT_OBJ)
	$(CC) $(CFLAGS) -o stndfont @$(mktmp $(^:t"\n")\n) $(LIBC)
	$(BINDCMD) stndfont $(BINDFLAGS)
	-$(RM) $(RMFLAGS) stndfont

../lib/plxtnd.fnt: xtndfont.o $(FONT_OBJ)
	$(CC) $(CFLAGS) -o xtndfont @$(mktmp $(^:t"\n")\n) $(LIBC)
	$(BINDCMD) xtndfont $(BINDFLAGS)

# 	-$(RM) $(RMFLAGS) $*

##############################################################################
#
# Object file dependencies.

# source files

plargs.o:	plargs.c plplotP.h plplot.h plstream.h
plbox.o:	plbox.c plplotP.h plplot.h
plcont.o:	plcont.c plplotP.h plplot.h
plctest.o:	plctest.c plplotP.h plplot.h
plcvt.o:	plcvt.c plplotP.h plplot.h
pldtik.o:	pldtik.c plplotP.h plplot.h
plfill.o:	plfill.c plplotP.h plplot.h
plhist.o:	plhist.c plplotP.h plplot.h
plline.o:	plline.c plplotP.h plplot.h
plot3d.o:	plot3d.c plplotP.h plplot.h
plpage.o:	plpage.c plplotP.h plplot.h
plsdef.o:	plsdef.c plplotP.h plplot.h
plshade.o:	plshade.c plplotP.h plplot.h
plstream.o:	plstream.c plplotP.h plplot.h plstream.h
plstring.o:	plstring.c plplotP.h plplot.h
plsym.o:	plsym.c plplotP.h plplot.h
pltick.o:	pltick.c plplotP.h plplot.h
plvpor.o:	plvpor.c plplotP.h plplot.h
plwind.o:	plwind.c plplotP.h plplot.h
pdfutils.o:	pdfutils.c plplotP.h plplot.h

# C language stubs for linking Plplot to Fortran.

sc3d.o:	sc3d.c plstubs.h plplotP.h plplot.h
scstubs.o:	scstubs.c plstubs.h plplotP.h plplot.h
sccont.o:	sccont.c plstubs.h plplotP.h plplot.h


# Fortran language stubs for linking Plplot to Fortran.

strutil.o:	strutil.f
sfstubs.o:	sfstubs.f

#----------------------------------------------------------------------#
# Explicit rules
#
# plfont.c and plctrl.c need to know the install directory
# Also, plfont.c may have font flags passed in

plfont.o:	plfont.c plplotP.h plplot.h
	$(CC) -c $(CFLAGS) $*.c

plctrl.o:	plctrl.c plplotP.h plplot.h
	$(CC) -c $(CFLAGS) $*.c

# plcore.c and all the drivers need to know $(PLDEVICES).  The guts
# of the driver routine are not compiled if its name is not present in
# the device list.  You may want to leave drivers for specific systems
# (Amiga, MS-DOS, OS/2, etc) out of this list.

plcore.o:	plcore.c plcore.h plplotP.h plplot.h plstream.h drivers.h
	$(CC) -c $(CFLAGS) $(PLDEVICES) $*.c

dg300.o:	dg300.c plplotP.h plplot.h plstream.h drivers.h
	$(CC) -c $(CFLAGS) $(PLDEVICES) $*.c

hpgl.o:	hpgl.c plplotP.h plplot.h plstream.h drivers.h
	$(CC) -c $(CFLAGS) $(PLDEVICES) $*.c

impress.o:	impress.c plplotP.h plplot.h plstream.h drivers.h
	$(CC) -c $(CFLAGS) $(PLDEVICES) $*.c

ljii.o:		ljii.c plplotP.h plplot.h plstream.h drivers.h
	$(CC) -c $(CFLAGS) $(PLDEVICES) $*.c

null.o:		null.c plplotP.h plplot.h plstream.h drivers.h
	$(CC) -c $(CFLAGS) $(PLDEVICES) $*.c

ps.o:		ps.c plplotP.h plplot.h plstream.h drivers.h
	$(CC) -c $(CFLAGS) $(PLDEVICES) $*.c

tek.o:		tek.c plplotP.h plplot.h plstream.h drivers.h
	$(CC) -c $(CFLAGS) $(PLDEVICES) $*.c

plbuf.o:	plbuf.c plplotP.h plplot.h plstream.h drivers.h metadefs.h
	$(CC) -c $(CFLAGS) $*.c

plmeta.o:	plmeta.c plplotP.h plplot.h plstream.h drivers.h metadefs.h
	$(CC) -c $(CFLAGS) $(PLDEVICES) $*.c

xfig.o:		xfig.c plplotP.h plplot.h plstream.h drivers.h
	$(CC) -c $(CFLAGS) $(PLDEVICES) $*.c

emxvga.o:	emxvga.c plplotP.h plplot.h plstream.h drivers.h
	$(CC) -c $(CFLAGS) $(PLDEVICES) $*.c

##############################################################################
#
# Utility programs.

plrender.o:	plrender.c plplotP.h plplot.h metadefs.h pdf.h

plrender:	plrender.exe

plrender.exe:	plrender.o
	$(LDC) plrender.o -o plrender $(LIBC)
	$(BINDCMD) plrender $(BINDFLAGS)
	-$(RM) $(RMFLAGS) $*

#----------------------------------------------------------------------#
# Build all the demos

# To verify that PLPLOT works with C++, copy x01c.c to xa.cc and make xa

xa:	xa.cc
	$(CC) $(CFLAGS) -g $*.cc -o $* -L../lib -llib$(PLLIB) -lm -lgr -lpc
	$(BINDCMD) $* $(BINDFLAGS)
	-$(RM) $(RMFLAGS) $*

# Example programs, in c.

cdemos: x01c.exe x02c.exe x03c.exe x04c.exe\
        x05c.exe x06c.exe x07c.exe x08c.exe\
        x09c.exe x10c.exe x11c.exe x12c.exe\
        x13c.exe x15c.exe tutor.exe

##############################################################################
#
# Example programs, in Fortran.

fdemos: x01f.exe x02f.exe x03f.exe x04f.exe\
        x05f.exe x06f.exe x07f.exe x08f.exe\
        x09f.exe x10f.exe x11f.exe x12f.exe\
        x13f.exe

##############################################################################

# cleanup targets

clean:
	-$(RM) $(RMFLAGS) $(CDEMOS) $(FDEMOS) *.plm *.tek *.ps *.exe

realclean:
	-$(RM) $(RMFLAGS) $(CDEMOS) $(FDEMOS) *.o *.c *.h *.f *.plm *.tek *.ps \
	 *.fnt Makefile plrender ..\lib\libpl*.* *.exe

