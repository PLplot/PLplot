#
# This makefile copies all source files to PLTMP and
# creates a makefile in this directory.
# Additional drivers should be uncommented from pldevs.h
#
# Questions/problems,send email to :
#       Paul Casteels   casteels@uia.ua.ac.be
#       University of Antwerp   Dpt. Physics
#       Belgium
#


#
# Location of plplot directories
#
PLDIR = ..\..\..
PLTMP = $(PLDIR)\tmp
PLLIB = $(PLDIR)\lib
PLBIN = $(PLDIR)\bin

#
# Device selection
#
META = PLD_plmeta
PS   = PLD_ps
CPS  = PLD_cps
NULL = PLD_null
#PBM  = PLD_pbm
#XFIG = PLD_xfig
#LJII = PLD_ljii
#HPGL = PLD_hp7470
#HPGLL = PLD_hp7580
#LJHPGL = PLD_lj_hpgl

#BGI = PLD_bgi
WIN3 = PLD_win3
.IGNORE:
default: links common
links:
#
# Copy the necessary files to the temp dir
#
  @mkdir $(PLTMP)\plplot
  @echo ``
  @echo `Copying source files to $(PLTMP)`
  @echo ``
  @copy $(PLDIR)\src\*.c $(PLTMP)\*.cpp
  @copy $(PLDIR)\include\*.h $(PLTMP)\plplot
  @copy $(PLDIR)\examples\c\*.c $(PLTMP)\*.cpp
  @copy $(PLDIR)\examples\c\*.h $(PLTMP)\plplot
  @copy $(PLDIR)\drivers\*.c $(PLTMP)\*.cpp
  @copy $(PLDIR)\utils\*.c $(PLTMP)\*.cpp
  @copy $(PLDIR)\lib\*.fnt $(PLTMP)\*.fnt
  @copy $(PLDIR)\lib\*.map $(PLTMP)\*.map
#
# Copy the local source files to temp\
#
#  @copy src\*.c $(PLTMP)\*.cpp
  @copy src\win3.cpp $(PLTMP)
  @copy src\plstub.cpp $(PLTMP)
  @copy src\resource.h $(PLTMP)\plplot
#  @copy src\*.rc $(PLTMP)
#  @copy src\*.ico $(PLTMP)
#  @copy src\*.def $(PLTMP)
#
# Convert from Unix -> Dos
#
#  @echo ``
#  @echo `Converting Unix-2-Dos (add CRs)`
#  @echo ``
#  @crlf -fs $(PLTMP)\*.cpp
#  @crlf -fs $(PLTMP)\*.h
#  @crlf -fs $(PLTMP)\*.rc
#  @crlf -fs $(PLTMP)\*.def
common:
#
# Create plconfig.h file
#
  @echo ``
  @echo `Creating $(PLTMP)\plplot\plconfig.h`
  @echo <<$(PLTMP)\plplot\plconfig.h
#ifndef __PLCONFIG_H__
#define __PLCONFIG_H__
#define LIB_DIR "$(PLLIB:\=\\)"
#define BIN_DIR "$(PLBIN:\=\\)"
#define PLPLOT_VERSION "5.0.4"
#endif
<<keep
# Create pldevs.h file
#
  @echo ``
  @echo `Creating $(PLTMP)\plplot\pldevs.h`
  @echo <<$(PLTMP)\plplot\pldevs.h
!ifdef META
#define PLD_plmeta
!endif
!ifdef PS
#define PLD_ps
!endif
!ifdef CPS
#define PLD_cps
!endif
!ifdef PBM
#define PLD_pbm
!endif
!ifdef XFIG 
#define PLD_xfig
!endif
!ifdef LJII 
#define PLD_ljii
!endif
!ifdef HPGL
#define PLD_hp7470
!endif
!ifdef HPGLL
#define PLD_hp7580
!endif
!ifdef LJHPGL
#define PLD_lj_hpgl
!endif
!ifdef NULL
#define PLD_null
!endif
!ifdef WIN3
#define PLD_win3
!endif
<<keep
install:
        @copy $(PLDIR)\sys\win32\msdev\plplib\plplot.lib $(PLLIB)
#        @copy $(PLDIR)\sys\win32\msdev\plpdll\Debug\plplotd.lib $(PLLIB)
#        @copy $(PLDIR)\sys\win32\msdev\plpdll\Release\plplotd.lib $(PLLIB)
#        @copy $(PLDIR)\sys\win32\msdev\plpdll\plplotd.dll $(PLBIN)
