#               
# Makefile for PLPLOT
#
#
# Questions or problems, send (e)mail to:
#
#	Paul Casteels   casteels@uia.ac.be
#	University of Antwerp   Dpt Physics
#	Belgium

PLDIR   = c:\plplot\  # 
PLLIBS  = .  #
PLFONTS = $(PLDIR)plfonts\  # 
PLFONT_DIR= $(PLDIR)  #
PLTMP   = $(PLDIR)tmp\  # 
PLLIBDIR = .  #

# Windows or DOS
WIN3 = TRUE

CC      = bcc
!if $d(WIN3)
CFLAGS = -3 -Od -ml -P -v 
!else
#CFLAGS = -f287 -Od -ml -P -v -DMSDOS -D__cplusplus
CFLAGS = -3 -f287 -Od -ml -P -v 
!endif
BRC = brc
BRCFLAGS =

# DOS & Windows
!if $d(WIN3)
DRIVERS = -DPS -DPLMETA -DHP7470 -DIMP -DWIN3
LIBS =
!else
DRIVERS = -DPS -DPLMETA -DHP7470 -DIMP -DBGI
LIBS = graphics.lib
!endif

# Rules
.SUFFIXES:
.SUFFIXES: .obj .c .for

.c.obj:
	$(CC) -c $(CFLAGS) $*.c 
.for.o:
	$(F77) -c $(FFLAGS) -o$*.o $*.for
.rc.res:
	$(BRC) -r $(BRCFLAGS) $*.rc
!if $d(WIN3)
.c.exe:
	$(CC) -c $(CFLAGS) -W $*.c
	tlink @&&|
/v c0wl $*.obj
$*,$*
plplot.lib import.lib mathwl.lib cwl.lib
default.def
|
	brc plplot.rc $*.exe
!else
.c.exe:
	$(CC) -c $(CFLAGS) $*.c
	tlink @&&|
/v c0l $*.obj
$*,$*
plplot.lib graphics.lib mathl.lib fp87.lib cl.lib
|
!endif

# Object file macros

OBJ =   \
	plbox.obj \
	plcont.obj \
	pldtik.obj \
	plfill.obj \
	plfont.obj \
	plhist.obj \
	plline.obj \
	plot3d.obj \
	plsdef.obj \
	plsym.obj \
	pltick.obj \
	plvpor.obj \
	plwind.obj \
	plcore.obj \
	plargs.obj \
	plbuf.obj \
	plctrl.obj \
	plcvt.obj \
	plpage.obj \
	plstream.obj \
	plstring.obj \
	plctest.obj \

DRV_OBJ = \
	pdfutils.obj \
	hpgl.obj \
	impress.obj \
	ljii.obj \
	ps.obj \
	tek.obj \
	dosbgi.obj \
	win3.obj \
	plmeta.obj

# Support files for font generators.

FONT_OBJ = \
	font01.obj \
	font02.obj \
	font03.obj \
	font04.obj \
	font05.obj \
	font06.obj \
	font07.obj \
	font08.obj \
	font09.obj \
	font10.obj \
	font11.obj

# C language stubs for linking Plplot to Fortran.

CSTUB_OBJ = \
	long2str.obj \
	sc3d.obj \
	scconf.obj \
	scstubs.obj

# Fortran language stubs for linking Plplot to Fortran.

FSTUB_OBJ = \
	sfstubs.obj

# Library dependencies

#libs:   libc libf
libs:   libc
libc:   $(PLLIBS)\plplot.lib
libf:   $(PLLIBS)\plstub.lib

#$(PLLIBS)\plplot.lib: $(OBJ) $(DRV_OBJ)
#        @echo Ready to build plplot.lib
#        -@del plplot.lib
#        tlib @&&!
#plplot\
#+$(OBJ:.o=.o +)\
#$(DRV_OBJ:.o=.o +)\
#,plplot.lst
#!

$(PLLIBS)\plplot.lib: $(OBJ) $(DRV_OBJ)
	@echo Ready to build plplot.lib
	-@del plplot.lib
	tlib @&&!
plplot\
+$(OBJ:.obj=.obj +)\
$(DRV_OBJ:.obj=.obj +)\
,plplot.lst
!

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
	copy ..\sys\dos\bcc\*.h .
	copy ..\sys\dos\bcc\*.c .
	copy ..\sys\dos\bcc\*.rc .
	copy ..\sys\dos\bcc\*.def .

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

fonts:  sfont xfont
	echo $(PLDIR)
	echo $(PLFONT_DIR)
	copy *.fnt $(PLFONT_DIR)
	echo Put this in autoexec.bat !!
	set PLFONTS=$(PLFONT_DIR)

sfont:  $(PLFONT_DIR)plstnd.fnt
xfont:  $(PLFONT_DIR)plxtnd.fnt

$(PLFONT_DIR)plstnd.fnt:
	$(CC) $(CFLAGS:ml=mh) -Ff=16 stndfont.c font*.c pdfutils.c
	-stndfont

$(PLFONT_DIR)plxtnd.fnt:
	$(CC) $(CFLAGS:ml=mh) -Ff=16 xtndfont.c font*.obj pdfutils.c
	-xtndfont

#----------------------------------------------------------------------#
# Explicit rules
#
# plfont.c may have font flags passed in

plfont.obj: plfont.c
	$(CC) -c $(CFLAGS) $(FONTFLAG)  $*.c

# dispatch.c and all the drivers need to know $(PLDEVICES).  The guts
# of the driver routine are not compiled if its name is not present in
# the device list.  You may want to leave drivers for specific systems
# (Amiga, MS-DOS, OS/2, etc) out of this list.

dispatch.obj: dispatch.c
	$(CC) -c $(CFLAGS) $(DRIVERS) $*.c 

hpgl.obj:       hpgl.c
	$(CC) -c $(CFLAGS) $(DRIVERS) $*.c 

impress.obj:      impress.c
	$(CC) -c $(CFLAGS) $(DRIVERS) $*.c 

ljii.obj: ljii.c
	$(CC) -c $(CFLAGS) $(DRIVERS) $*.c 

ps.obj:   ps.c
	$(CC) -c $(CFLAGS) $(DRIVERS) $*.c 

win3.obj: win3.c
	$(CC) -c $(CFLAGS) $(DRIVERS) $*.c 

tek.obj:  tek.c
	$(CC) -c $(CFLAGS) $(DRIVERS) $*.c 

plmeta.obj:       plmeta.c
	$(CC) -c $(CFLAGS) $(DRIVERS) $*.c 

plcore.obj:       plcore.c
	$(CC) -c $(CFLAGS) $(DRIVERS) $*.c 

dosbgi.obj:  dosbgi.c
	$(CC) -c $(CFLAGS) $(DRIVERS) $(BGIDRIVERS) $*.c 

#----------------------------------------------------------------------#
# Build all the demos

demos : cdemos fdemos

# Example programs, in c.

cdemos: x01c x02c x03c x04c x05c x06c x07c x08c x09c x10c x11c \
	x12c x13c x15c tutor

x01c:   x01c.exe 
x02c:   x02c.exe
x03c:   x03c.exe
x04c:   x04c.exe
x05c:   x05c.exe
x06c:   x06c.exe
x07c:   x07c.exe
x08c:   x08c.exe
x09c:   x09c.exe
x10c:   x10c.exe
x11c:   x11c.exe
x12c:   x12c.exe
x13c:   x13c.exe
# only x-windows !!!
#x14c:   x14c.c
#        $(CC) $(CFLAGS) x14c.c plplot.lib $(LIBS) 
x15c:   x15c.exe
tutor:  tutor.exe
