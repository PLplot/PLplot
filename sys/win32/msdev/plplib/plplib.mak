# Microsoft Developer Studio Generated NMAKE File, Format Version 4.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

!IF "$(CFG)" == ""
CFG=plplib - Win32 Release
!MESSAGE No configuration specified.  Defaulting to plplib - Win32 Release.
!ENDIF 

!IF "$(CFG)" != "plplib - Win32 Release" && "$(CFG)" != "plplib - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE on this makefile
!MESSAGE by defining the macro CFG on the command line.  For example:
!MESSAGE 
!MESSAGE NMAKE /f "plplib.mak" CFG="plplib - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "plplib - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "plplib - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 
################################################################################
# Begin Project
# PROP Target_Last_Scanned "plplib - Win32 Debug"
CPP=cl.exe

!IF  "$(CFG)" == "plplib - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ""
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
OUTDIR=.
INTDIR=.\Release

ALL : "$(OUTDIR)\plplot.lib"

CLEAN : 
	-@erase ".\plplot.lib"
	-@erase ".\Release\pltick.obj"
	-@erase ".\Release\plfill.obj"
	-@erase ".\Release\ps.obj"
	-@erase ".\Release\plmeta.obj"
	-@erase ".\Release\win3.obj"
	-@erase ".\Release\plcvt.obj"
	-@erase ".\Release\plsdef.obj"
	-@erase ".\Release\plsym.obj"
	-@erase ".\Release\plargs.obj"
	-@erase ".\Release\plctrl.obj"
	-@erase ".\Release\plhist.obj"
	-@erase ".\Release\plvpor.obj"
	-@erase ".\Release\plpage.obj"
	-@erase ".\Release\plcont.obj"
	-@erase ".\Release\plbox.obj"
	-@erase ".\Release\hpgl.obj"
	-@erase ".\Release\plwind.obj"
	-@erase ".\Release\pldtik.obj"
	-@erase ".\Release\null.obj"
	-@erase ".\Release\plcore.obj"
	-@erase ".\Release\plline.obj"
	-@erase ".\Release\plbuf.obj"
	-@erase ".\Release\plshade.obj"
	-@erase ".\Release\plmap.obj"
	-@erase ".\Release\plot3d.obj"
	-@erase ".\Release\xfig.obj"
	-@erase ".\Release\pdfutils.obj"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
CPP_PROJ=/nologo /ML /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS"\
 /Fp"$(INTDIR)/plplib.pch" /YX /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\Release/
CPP_SBRS=
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/plplib.bsc" 
BSC32_SBRS=
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"plplot.lib"
LIB32_FLAGS=/nologo /out:"$(OUTDIR)/plplot.lib" 
LIB32_OBJS= \
	"$(INTDIR)/pltick.obj" \
	"$(INTDIR)/plfill.obj" \
	"$(INTDIR)/ps.obj" \
	"$(INTDIR)/plmeta.obj" \
	"$(INTDIR)/win3.obj" \
	"$(INTDIR)/plcvt.obj" \
	"$(INTDIR)/plsdef.obj" \
	"$(INTDIR)/plsym.obj" \
	"$(INTDIR)/plargs.obj" \
	"$(INTDIR)/plctrl.obj" \
	"$(INTDIR)/plhist.obj" \
	"$(INTDIR)/plvpor.obj" \
	"$(INTDIR)/plpage.obj" \
	"$(INTDIR)/plcont.obj" \
	"$(INTDIR)/plbox.obj" \
	"$(INTDIR)/hpgl.obj" \
	"$(INTDIR)/plwind.obj" \
	"$(INTDIR)/pldtik.obj" \
	"$(INTDIR)/null.obj" \
	"$(INTDIR)/plcore.obj" \
	"$(INTDIR)/plline.obj" \
	"$(INTDIR)/plbuf.obj" \
	"$(INTDIR)/plshade.obj" \
	"$(INTDIR)/plmap.obj" \
	"$(INTDIR)/plot3d.obj" \
	"$(INTDIR)/xfig.obj" \
	"$(INTDIR)/pdfutils.obj"

"$(OUTDIR)\plplot.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ELSEIF  "$(CFG)" == "plplib - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
OUTDIR=.
INTDIR=.\Debug

ALL : "$(OUTDIR)\plplot.lib"

CLEAN : 
	-@erase ".\plplot.lib"
	-@erase ".\Debug\plsdef.obj"
	-@erase ".\Debug\plbuf.obj"
	-@erase ".\Debug\plargs.obj"
	-@erase ".\Debug\hpgl.obj"
	-@erase ".\Debug\plmap.obj"
	-@erase ".\Debug\plctrl.obj"
	-@erase ".\Debug\null.obj"
	-@erase ".\Debug\pltick.obj"
	-@erase ".\Debug\plpage.obj"
	-@erase ".\Debug\plcont.obj"
	-@erase ".\Debug\plwind.obj"
	-@erase ".\Debug\plcvt.obj"
	-@erase ".\Debug\pldtik.obj"
	-@erase ".\Debug\plshade.obj"
	-@erase ".\Debug\plcore.obj"
	-@erase ".\Debug\xfig.obj"
	-@erase ".\Debug\plsym.obj"
	-@erase ".\Debug\plline.obj"
	-@erase ".\Debug\plhist.obj"
	-@erase ".\Debug\plvpor.obj"
	-@erase ".\Debug\win3.obj"
	-@erase ".\Debug\plot3d.obj"
	-@erase ".\Debug\plbox.obj"
	-@erase ".\Debug\plfill.obj"
	-@erase ".\Debug\pdfutils.obj"
	-@erase ".\Debug\plmeta.obj"
	-@erase ".\Debug\ps.obj"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

# ADD BASE CPP /nologo /W3 /GX /Z7 /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /W3 /GX /Z7 /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /c
CPP_PROJ=/nologo /MLd /W3 /GX /Z7 /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /Fp"$(INTDIR)/plplib.pch" /YX /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\Debug/
CPP_SBRS=
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/plplib.bsc" 
BSC32_SBRS=
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"plplot.lib"
LIB32_FLAGS=/nologo /out:"$(OUTDIR)/plplot.lib" 
LIB32_OBJS= \
	"$(INTDIR)/plsdef.obj" \
	"$(INTDIR)/plbuf.obj" \
	"$(INTDIR)/plargs.obj" \
	"$(INTDIR)/hpgl.obj" \
	"$(INTDIR)/plmap.obj" \
	"$(INTDIR)/plctrl.obj" \
	"$(INTDIR)/null.obj" \
	"$(INTDIR)/pltick.obj" \
	"$(INTDIR)/plpage.obj" \
	"$(INTDIR)/plcont.obj" \
	"$(INTDIR)/plwind.obj" \
	"$(INTDIR)/plcvt.obj" \
	"$(INTDIR)/pldtik.obj" \
	"$(INTDIR)/plshade.obj" \
	"$(INTDIR)/plcore.obj" \
	"$(INTDIR)/xfig.obj" \
	"$(INTDIR)/plsym.obj" \
	"$(INTDIR)/plline.obj" \
	"$(INTDIR)/plhist.obj" \
	"$(INTDIR)/plvpor.obj" \
	"$(INTDIR)/win3.obj" \
	"$(INTDIR)/plot3d.obj" \
	"$(INTDIR)/plbox.obj" \
	"$(INTDIR)/plfill.obj" \
	"$(INTDIR)/pdfutils.obj" \
	"$(INTDIR)/plmeta.obj" \
	"$(INTDIR)/ps.obj"

"$(OUTDIR)\plplot.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ENDIF 

.c{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.c{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

################################################################################
# Begin Target

# Name "plplib - Win32 Release"
# Name "plplib - Win32 Debug"

!IF  "$(CFG)" == "plplib - Win32 Release"

!ELSEIF  "$(CFG)" == "plplib - Win32 Debug"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=\plplot\tmp\xfig.cpp
DEP_CPP_XFIG_=\
	".\..\..\..\..\tmp\plDevs.h"\
	".\..\..\..\..\tmp\plplotP.h"\
	".\..\..\..\..\tmp\drivers.h"\
	".\..\..\..\..\tmp\plConfig.h"\
	".\..\..\..\..\tmp\plplot.h"\
	".\..\..\..\..\tmp\plstrm.h"\
	".\..\..\..\..\tmp\pldebug.h"\
	".\..\..\..\..\tmp\pdf.h"\
	

"$(INTDIR)\xfig.obj" : $(SOURCE) $(DEP_CPP_XFIG_) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=\plplot\tmp\win3.cpp

!IF  "$(CFG)" == "plplib - Win32 Release"

DEP_CPP_WIN3_=\
	".\..\..\..\..\tmp\plDevs.h"\
	".\..\..\..\..\tmp\plplotP.h"\
	".\..\..\..\..\tmp\drivers.h"\
	".\..\..\..\..\tmp\plConfig.h"\
	".\..\..\..\..\tmp\plplot.h"\
	".\..\..\..\..\tmp\plstrm.h"\
	".\..\..\..\..\tmp\pldebug.h"\
	".\..\..\..\..\tmp\pdf.h"\
	

"$(INTDIR)\win3.obj" : $(SOURCE) $(DEP_CPP_WIN3_) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "plplib - Win32 Debug"

DEP_CPP_WIN3_=\
	".\..\..\..\..\tmp\plDevs.h"\
	".\..\..\..\..\tmp\plplotP.h"\
	".\..\..\..\..\tmp\drivers.h"\
	".\..\..\..\..\tmp\plConfig.h"\
	".\..\..\..\..\tmp\plplot.h"\
	".\..\..\..\..\tmp\plstrm.h"\
	".\..\..\..\..\tmp\pldebug.h"\
	".\..\..\..\..\tmp\pdf.h"\
	
NODEP_CPP_WIN3_=\
	".\..\..\..\..\tmp\PrintDlg"\
	

"$(INTDIR)\win3.obj" : $(SOURCE) $(DEP_CPP_WIN3_) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\plplot\tmp\ps.cpp
DEP_CPP_PS_CP=\
	".\..\..\..\..\tmp\plDevs.h"\
	".\..\..\..\..\tmp\plplotP.h"\
	".\..\..\..\..\tmp\drivers.h"\
	".\..\..\..\..\tmp\plConfig.h"\
	".\..\..\..\..\tmp\plplot.h"\
	".\..\..\..\..\tmp\plstrm.h"\
	".\..\..\..\..\tmp\pldebug.h"\
	".\..\..\..\..\tmp\pdf.h"\
	

"$(INTDIR)\ps.obj" : $(SOURCE) $(DEP_CPP_PS_CP) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=\plplot\tmp\plwind.cpp
DEP_CPP_PLWIN=\
	".\..\..\..\..\tmp\plplotP.h"\
	".\..\..\..\..\tmp\plConfig.h"\
	".\..\..\..\..\tmp\plplot.h"\
	".\..\..\..\..\tmp\plstrm.h"\
	".\..\..\..\..\tmp\pldebug.h"\
	".\..\..\..\..\tmp\pdf.h"\
	

"$(INTDIR)\plwind.obj" : $(SOURCE) $(DEP_CPP_PLWIN) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=\plplot\tmp\plvpor.cpp
DEP_CPP_PLVPO=\
	".\..\..\..\..\tmp\plplotP.h"\
	".\..\..\..\..\tmp\plConfig.h"\
	".\..\..\..\..\tmp\plplot.h"\
	".\..\..\..\..\tmp\plstrm.h"\
	".\..\..\..\..\tmp\pldebug.h"\
	".\..\..\..\..\tmp\pdf.h"\
	

"$(INTDIR)\plvpor.obj" : $(SOURCE) $(DEP_CPP_PLVPO) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=\plplot\tmp\pltick.cpp
DEP_CPP_PLTIC=\
	".\..\..\..\..\tmp\plplotP.h"\
	".\..\..\..\..\tmp\plConfig.h"\
	".\..\..\..\..\tmp\plplot.h"\
	".\..\..\..\..\tmp\plstrm.h"\
	".\..\..\..\..\tmp\pldebug.h"\
	".\..\..\..\..\tmp\pdf.h"\
	

"$(INTDIR)\pltick.obj" : $(SOURCE) $(DEP_CPP_PLTIC) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=\plplot\tmp\plsym.cpp
DEP_CPP_PLSYM=\
	".\..\..\..\..\tmp\plplotP.h"\
	".\..\..\..\..\tmp\plConfig.h"\
	".\..\..\..\..\tmp\plplot.h"\
	".\..\..\..\..\tmp\plstrm.h"\
	".\..\..\..\..\tmp\pldebug.h"\
	".\..\..\..\..\tmp\pdf.h"\
	

"$(INTDIR)\plsym.obj" : $(SOURCE) $(DEP_CPP_PLSYM) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=\plplot\tmp\plshade.cpp
DEP_CPP_PLSHA=\
	".\..\..\..\..\tmp\plplotP.h"\
	".\..\..\..\..\tmp\plConfig.h"\
	".\..\..\..\..\tmp\plplot.h"\
	".\..\..\..\..\tmp\plstrm.h"\
	".\..\..\..\..\tmp\pldebug.h"\
	".\..\..\..\..\tmp\pdf.h"\
	

"$(INTDIR)\plshade.obj" : $(SOURCE) $(DEP_CPP_PLSHA) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=\plplot\tmp\plsdef.cpp
DEP_CPP_PLSDE=\
	".\..\..\..\..\tmp\plplotP.h"\
	".\..\..\..\..\tmp\plConfig.h"\
	".\..\..\..\..\tmp\plplot.h"\
	".\..\..\..\..\tmp\plstrm.h"\
	".\..\..\..\..\tmp\pldebug.h"\
	".\..\..\..\..\tmp\pdf.h"\
	

"$(INTDIR)\plsdef.obj" : $(SOURCE) $(DEP_CPP_PLSDE) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=\plplot\tmp\plpage.cpp
DEP_CPP_PLPAG=\
	".\..\..\..\..\tmp\plplotP.h"\
	".\..\..\..\..\tmp\plConfig.h"\
	".\..\..\..\..\tmp\plplot.h"\
	".\..\..\..\..\tmp\plstrm.h"\
	".\..\..\..\..\tmp\pldebug.h"\
	".\..\..\..\..\tmp\pdf.h"\
	

"$(INTDIR)\plpage.obj" : $(SOURCE) $(DEP_CPP_PLPAG) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=\plplot\tmp\plot3d.cpp
DEP_CPP_PLOT3=\
	".\..\..\..\..\tmp\plplotP.h"\
	".\..\..\..\..\tmp\plConfig.h"\
	".\..\..\..\..\tmp\plplot.h"\
	".\..\..\..\..\tmp\plstrm.h"\
	".\..\..\..\..\tmp\pldebug.h"\
	".\..\..\..\..\tmp\pdf.h"\
	

"$(INTDIR)\plot3d.obj" : $(SOURCE) $(DEP_CPP_PLOT3) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=\plplot\tmp\plmeta.cpp
DEP_CPP_PLMET=\
	".\..\..\..\..\tmp\plDevs.h"\
	".\..\..\..\..\tmp\plplotP.h"\
	".\..\..\..\..\tmp\drivers.h"\
	".\..\..\..\..\tmp\metadefs.h"\
	".\..\..\..\..\tmp\plConfig.h"\
	".\..\..\..\..\tmp\plplot.h"\
	".\..\..\..\..\tmp\plstrm.h"\
	".\..\..\..\..\tmp\pldebug.h"\
	".\..\..\..\..\tmp\pdf.h"\
	

"$(INTDIR)\plmeta.obj" : $(SOURCE) $(DEP_CPP_PLMET) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=\plplot\tmp\plmap.cpp
DEP_CPP_PLMAP=\
	".\..\..\..\..\tmp\plplotP.h"\
	".\..\..\..\..\tmp\plConfig.h"\
	".\..\..\..\..\tmp\plplot.h"\
	".\..\..\..\..\tmp\plstrm.h"\
	".\..\..\..\..\tmp\pldebug.h"\
	".\..\..\..\..\tmp\pdf.h"\
	

"$(INTDIR)\plmap.obj" : $(SOURCE) $(DEP_CPP_PLMAP) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=\plplot\tmp\plline.cpp
DEP_CPP_PLLIN=\
	".\..\..\..\..\tmp\plplotP.h"\
	".\..\..\..\..\tmp\plConfig.h"\
	".\..\..\..\..\tmp\plplot.h"\
	".\..\..\..\..\tmp\plstrm.h"\
	".\..\..\..\..\tmp\pldebug.h"\
	".\..\..\..\..\tmp\pdf.h"\
	

"$(INTDIR)\plline.obj" : $(SOURCE) $(DEP_CPP_PLLIN) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=\plplot\tmp\plhist.cpp
DEP_CPP_PLHIS=\
	".\..\..\..\..\tmp\plplotP.h"\
	".\..\..\..\..\tmp\plConfig.h"\
	".\..\..\..\..\tmp\plplot.h"\
	".\..\..\..\..\tmp\plstrm.h"\
	".\..\..\..\..\tmp\pldebug.h"\
	".\..\..\..\..\tmp\pdf.h"\
	

"$(INTDIR)\plhist.obj" : $(SOURCE) $(DEP_CPP_PLHIS) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=\plplot\tmp\plfill.cpp
DEP_CPP_PLFIL=\
	".\..\..\..\..\tmp\plplotP.h"\
	".\..\..\..\..\tmp\plConfig.h"\
	".\..\..\..\..\tmp\plplot.h"\
	".\..\..\..\..\tmp\plstrm.h"\
	".\..\..\..\..\tmp\pldebug.h"\
	".\..\..\..\..\tmp\pdf.h"\
	

"$(INTDIR)\plfill.obj" : $(SOURCE) $(DEP_CPP_PLFIL) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=\plplot\tmp\pldtik.cpp
DEP_CPP_PLDTI=\
	".\..\..\..\..\tmp\plplotP.h"\
	".\..\..\..\..\tmp\plConfig.h"\
	".\..\..\..\..\tmp\plplot.h"\
	".\..\..\..\..\tmp\plstrm.h"\
	".\..\..\..\..\tmp\pldebug.h"\
	".\..\..\..\..\tmp\pdf.h"\
	

"$(INTDIR)\pldtik.obj" : $(SOURCE) $(DEP_CPP_PLDTI) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=\plplot\tmp\plcvt.cpp
DEP_CPP_PLCVT=\
	".\..\..\..\..\tmp\plplotP.h"\
	".\..\..\..\..\tmp\plConfig.h"\
	".\..\..\..\..\tmp\plplot.h"\
	".\..\..\..\..\tmp\plstrm.h"\
	".\..\..\..\..\tmp\pldebug.h"\
	".\..\..\..\..\tmp\pdf.h"\
	

"$(INTDIR)\plcvt.obj" : $(SOURCE) $(DEP_CPP_PLCVT) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=\plplot\tmp\plctrl.cpp
DEP_CPP_PLCTR=\
	".\..\..\..\..\tmp\plplotP.h"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	".\..\..\..\..\tmp\plConfig.h"\
	".\..\..\..\..\tmp\plplot.h"\
	".\..\..\..\..\tmp\plstrm.h"\
	".\..\..\..\..\tmp\pldebug.h"\
	".\..\..\..\..\tmp\pdf.h"\
	

"$(INTDIR)\plctrl.obj" : $(SOURCE) $(DEP_CPP_PLCTR) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=\plplot\tmp\plcore.cpp
DEP_CPP_PLCOR=\
	".\..\..\..\..\tmp\plcore.h"\
	".\..\..\..\..\tmp\plplotP.h"\
	".\..\..\..\..\tmp\drivers.h"\
	".\..\..\..\..\tmp\plDevs.h"\
	".\..\..\..\..\tmp\pldebug.h"\
	".\..\..\..\..\tmp\plConfig.h"\
	".\..\..\..\..\tmp\plplot.h"\
	".\..\..\..\..\tmp\plstrm.h"\
	".\..\..\..\..\tmp\pdf.h"\
	

"$(INTDIR)\plcore.obj" : $(SOURCE) $(DEP_CPP_PLCOR) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=\plplot\tmp\plcont.cpp
DEP_CPP_PLCON=\
	".\..\..\..\..\tmp\plplotP.h"\
	".\..\..\..\..\tmp\plConfig.h"\
	".\..\..\..\..\tmp\plplot.h"\
	".\..\..\..\..\tmp\plstrm.h"\
	".\..\..\..\..\tmp\pldebug.h"\
	".\..\..\..\..\tmp\pdf.h"\
	

"$(INTDIR)\plcont.obj" : $(SOURCE) $(DEP_CPP_PLCON) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=\plplot\tmp\plbuf.cpp
DEP_CPP_PLBUF=\
	".\..\..\..\..\tmp\plplotP.h"\
	".\..\..\..\..\tmp\drivers.h"\
	".\..\..\..\..\tmp\metadefs.h"\
	".\..\..\..\..\tmp\plConfig.h"\
	".\..\..\..\..\tmp\plplot.h"\
	".\..\..\..\..\tmp\plstrm.h"\
	".\..\..\..\..\tmp\pldebug.h"\
	".\..\..\..\..\tmp\pdf.h"\
	

"$(INTDIR)\plbuf.obj" : $(SOURCE) $(DEP_CPP_PLBUF) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=\plplot\tmp\plbox.cpp
DEP_CPP_PLBOX=\
	".\..\..\..\..\tmp\plplotP.h"\
	".\..\..\..\..\tmp\plConfig.h"\
	".\..\..\..\..\tmp\plplot.h"\
	".\..\..\..\..\tmp\plstrm.h"\
	".\..\..\..\..\tmp\pldebug.h"\
	".\..\..\..\..\tmp\pdf.h"\
	

"$(INTDIR)\plbox.obj" : $(SOURCE) $(DEP_CPP_PLBOX) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=\plplot\tmp\plargs.cpp
DEP_CPP_PLARG=\
	".\..\..\..\..\tmp\plplotP.h"\
	".\..\..\..\..\tmp\plConfig.h"\
	".\..\..\..\..\tmp\plplot.h"\
	".\..\..\..\..\tmp\plstrm.h"\
	".\..\..\..\..\tmp\pldebug.h"\
	".\..\..\..\..\tmp\pdf.h"\
	

"$(INTDIR)\plargs.obj" : $(SOURCE) $(DEP_CPP_PLARG) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=\plplot\tmp\pdfutils.cpp
DEP_CPP_PDFUT=\
	".\..\..\..\..\tmp\plplotP.h"\
	".\..\..\..\..\tmp\plConfig.h"\
	".\..\..\..\..\tmp\plplot.h"\
	".\..\..\..\..\tmp\plstrm.h"\
	".\..\..\..\..\tmp\pldebug.h"\
	".\..\..\..\..\tmp\pdf.h"\
	

"$(INTDIR)\pdfutils.obj" : $(SOURCE) $(DEP_CPP_PDFUT) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=\plplot\tmp\null.cpp
DEP_CPP_NULL_=\
	".\..\..\..\..\tmp\plDevs.h"\
	".\..\..\..\..\tmp\plplotP.h"\
	".\..\..\..\..\tmp\drivers.h"\
	".\..\..\..\..\tmp\plConfig.h"\
	".\..\..\..\..\tmp\plplot.h"\
	".\..\..\..\..\tmp\plstrm.h"\
	".\..\..\..\..\tmp\pldebug.h"\
	".\..\..\..\..\tmp\pdf.h"\
	

"$(INTDIR)\null.obj" : $(SOURCE) $(DEP_CPP_NULL_) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=\plplot\tmp\hpgl.cpp
DEP_CPP_HPGL_=\
	".\..\..\..\..\tmp\plDevs.h"\
	".\..\..\..\..\tmp\plplotP.h"\
	".\..\..\..\..\tmp\drivers.h"\
	".\..\..\..\..\tmp\plConfig.h"\
	".\..\..\..\..\tmp\plplot.h"\
	".\..\..\..\..\tmp\plstrm.h"\
	".\..\..\..\..\tmp\pldebug.h"\
	".\..\..\..\..\tmp\pdf.h"\
	

"$(INTDIR)\hpgl.obj" : $(SOURCE) $(DEP_CPP_HPGL_) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
# End Target
# End Project
################################################################################
