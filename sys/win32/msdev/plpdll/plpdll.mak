# Microsoft Developer Studio Generated NMAKE File, Format Version 4.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

!IF "$(CFG)" == ""
CFG=plpdll - Win32 Release
!MESSAGE No configuration specified.  Defaulting to plpdll - Win32 Release.
!ENDIF 

!IF "$(CFG)" != "plpdll - Win32 Release" && "$(CFG)" != "plpdll - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE on this makefile
!MESSAGE by defining the macro CFG on the command line.  For example:
!MESSAGE 
!MESSAGE NMAKE /f "plpdll.mak" CFG="plpdll - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "plpdll - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "plpdll - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
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
# PROP Target_Last_Scanned "plpdll - Win32 Debug"
MTL=mktyplib.exe
RSC=rc.exe
CPP=cl.exe

!IF  "$(CFG)" == "plpdll - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
OUTDIR=.\Release
INTDIR=.\Release

ALL : "$(OUTDIR)\plplotd.dll"

CLEAN : 
	-@erase ".\plplotd.dll"
	-@erase ".\Release\plconsole.obj"
	-@erase ".\Release\plctrl.obj"
	-@erase ".\Release\plot3d.obj"
	-@erase ".\Release\plbox.obj"
	-@erase ".\Release\ps.obj"
	-@erase ".\Release\pltick.obj"
	-@erase ".\Release\pdfutils.obj"
	-@erase ".\Release\plfill.obj"
	-@erase ".\Release\plstub.obj"
	-@erase ".\Release\plpage.obj"
	-@erase ".\Release\plmeta.obj"
	-@erase ".\Release\xfig.obj"
	-@erase ".\Release\plwind.obj"
	-@erase ".\Release\plcvt.obj"
	-@erase ".\Release\plbuf.obj"
	-@erase ".\Release\plsdef.obj"
	-@erase ".\Release\plsym.obj"
	-@erase ".\Release\win3.obj"
	-@erase ".\Release\plmap.obj"
	-@erase ".\Release\plargs.obj"
	-@erase ".\Release\plline.obj"
	-@erase ".\Release\plhist.obj"
	-@erase ".\Release\plvpor.obj"
	-@erase ".\Release\plcont.obj"
	-@erase ".\Release\plshade.obj"
	-@erase ".\Release\pldtik.obj"
	-@erase ".\Release\plcore.obj"
	-@erase ".\Release\hpgl.obj"
	-@erase ".\Release\null.obj"
	-@erase ".\Release\plplot.res"
	-@erase ".\Release\plplotd.lib"
	-@erase ".\Release\plplotd.exp"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /YX /c
CPP_PROJ=/nologo /MT /W3 /GX /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS"\
 /Fp"$(INTDIR)/plpdll.pch" /YX /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\Release/
CPP_SBRS=
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /win32
MTL_PROJ=/nologo /D "NDEBUG" /win32 
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/plplot.res" /d "NDEBUG" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/plpdll.bsc" 
BSC32_SBRS=
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386 /out:"plplotd.dll"
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib /nologo /subsystem:windows /dll /incremental:no\
 /pdb:"$(OUTDIR)/plplotd.pdb" /machine:I386 /def:"\plplot\tmp\plplotd.def"\
 /out:"plplotd.dll" /implib:"$(OUTDIR)/plplotd.lib" 
DEF_FILE= \
	"..\..\..\..\tmp\plplotd.def"
LINK32_OBJS= \
	".\Release\plconsole.obj" \
	".\Release\plctrl.obj" \
	".\Release\plot3d.obj" \
	".\Release\plbox.obj" \
	".\Release\ps.obj" \
	".\Release\pltick.obj" \
	".\Release\pdfutils.obj" \
	".\Release\plfill.obj" \
	".\Release\plstub.obj" \
	".\Release\plpage.obj" \
	".\Release\plmeta.obj" \
	".\Release\xfig.obj" \
	".\Release\plwind.obj" \
	".\Release\plcvt.obj" \
	".\Release\plbuf.obj" \
	".\Release\plsdef.obj" \
	".\Release\plsym.obj" \
	".\Release\win3.obj" \
	".\Release\plmap.obj" \
	".\Release\plargs.obj" \
	".\Release\plline.obj" \
	".\Release\plhist.obj" \
	".\Release\plvpor.obj" \
	".\Release\plcont.obj" \
	".\Release\plshade.obj" \
	".\Release\pldtik.obj" \
	".\Release\plcore.obj" \
	".\Release\hpgl.obj" \
	".\Release\null.obj" \
	".\Release\plplot.res"

"$(OUTDIR)\plplotd.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "plpdll - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
OUTDIR=.\Debug
INTDIR=.\Debug

ALL : "$(OUTDIR)\plplotd.dll"

CLEAN : 
	-@erase ".\Debug\vc40.pdb"
	-@erase ".\Debug\vc40.idb"
	-@erase ".\plplotd.dll"
	-@erase ".\Debug\pdfutils.obj"
	-@erase ".\Debug\plot3d.obj"
	-@erase ".\Debug\plbox.obj"
	-@erase ".\Debug\ps.obj"
	-@erase ".\Debug\plstub.obj"
	-@erase ".\Debug\plmeta.obj"
	-@erase ".\Debug\plconsole.obj"
	-@erase ".\Debug\plbuf.obj"
	-@erase ".\Debug\plsdef.obj"
	-@erase ".\Debug\plmap.obj"
	-@erase ".\Debug\plargs.obj"
	-@erase ".\Debug\plctrl.obj"
	-@erase ".\Debug\pltick.obj"
	-@erase ".\Debug\plfill.obj"
	-@erase ".\Debug\plshade.obj"
	-@erase ".\Debug\plpage.obj"
	-@erase ".\Debug\plcont.obj"
	-@erase ".\Debug\xfig.obj"
	-@erase ".\Debug\plwind.obj"
	-@erase ".\Debug\plcvt.obj"
	-@erase ".\Debug\hpgl.obj"
	-@erase ".\Debug\pldtik.obj"
	-@erase ".\Debug\plcore.obj"
	-@erase ".\Debug\plsym.obj"
	-@erase ".\Debug\null.obj"
	-@erase ".\Debug\win3.obj"
	-@erase ".\Debug\plline.obj"
	-@erase ".\Debug\plhist.obj"
	-@erase ".\Debug\plvpor.obj"
	-@erase ".\Debug\plplot.res"
	-@erase ".\plplotd.ilk"
	-@erase ".\Debug\plplotd.lib"
	-@erase ".\Debug\plplotd.exp"
	-@erase ".\Debug\plplotd.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /YX /c
CPP_PROJ=/nologo /MTd /W3 /Gm /GX /Zi /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS"\
 /Fp"$(INTDIR)/plpdll.pch" /YX /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\Debug/
CPP_SBRS=
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /win32
MTL_PROJ=/nologo /D "_DEBUG" /win32 
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/plplot.res" /d "_DEBUG" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/plpdll.bsc" 
BSC32_SBRS=
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /debug /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"plplotd.dll"
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib /nologo /subsystem:windows /dll /incremental:yes\
 /pdb:"$(OUTDIR)/plplotd.pdb" /debug /machine:I386\
 /def:"\plplot\tmp\plplotd.def" /out:"plplotd.dll"\
 /implib:"$(OUTDIR)/plplotd.lib" 
DEF_FILE= \
	"..\..\..\..\tmp\plplotd.def"
LINK32_OBJS= \
	".\Debug\pdfutils.obj" \
	".\Debug\plot3d.obj" \
	".\Debug\plbox.obj" \
	".\Debug\ps.obj" \
	".\Debug\plstub.obj" \
	".\Debug\plmeta.obj" \
	".\Debug\plconsole.obj" \
	".\Debug\plbuf.obj" \
	".\Debug\plsdef.obj" \
	".\Debug\plmap.obj" \
	".\Debug\plargs.obj" \
	".\Debug\plctrl.obj" \
	".\Debug\pltick.obj" \
	".\Debug\plfill.obj" \
	".\Debug\plshade.obj" \
	".\Debug\plpage.obj" \
	".\Debug\plcont.obj" \
	".\Debug\xfig.obj" \
	".\Debug\plwind.obj" \
	".\Debug\plcvt.obj" \
	".\Debug\hpgl.obj" \
	".\Debug\pldtik.obj" \
	".\Debug\plcore.obj" \
	".\Debug\plsym.obj" \
	".\Debug\null.obj" \
	".\Debug\win3.obj" \
	".\Debug\plline.obj" \
	".\Debug\plhist.obj" \
	".\Debug\plvpor.obj" \
	".\Debug\plplot.res"

"$(OUTDIR)\plplotd.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
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

# Name "plpdll - Win32 Release"
# Name "plpdll - Win32 Debug"

!IF  "$(CFG)" == "plpdll - Win32 Release"

!ELSEIF  "$(CFG)" == "plpdll - Win32 Debug"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=\plplot\tmp\xfig.cpp
DEP_CPP_XFIG_=\
	".\..\..\..\..\..\plplot\tmp\plDevs.h"\
	".\..\..\..\..\..\plplot\tmp\plplotP.h"\
	".\..\..\..\..\..\plplot\tmp\drivers.h"\
	".\..\..\..\..\..\plplot\tmp\plConfig.h"\
	".\..\..\..\..\..\plplot\tmp\plplot.h"\
	".\..\..\..\..\..\plplot\tmp\plstrm.h"\
	".\..\..\..\..\..\plplot\tmp\pldebug.h"\
	".\..\..\..\..\..\plplot\tmp\pdf.h"\
	

!IF  "$(CFG)" == "plpdll - Win32 Release"


"$(INTDIR)\xfig.obj" : $(SOURCE) $(DEP_CPP_XFIG_) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "plpdll - Win32 Debug"


"$(INTDIR)\xfig.obj" : $(SOURCE) $(DEP_CPP_XFIG_) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\plplot\tmp\win3.cpp

!IF  "$(CFG)" == "plpdll - Win32 Release"

DEP_CPP_WIN3_=\
	".\..\..\..\..\..\plplot\tmp\plDevs.h"\
	".\..\..\..\..\..\plplot\tmp\plplotP.h"\
	".\..\..\..\..\..\plplot\tmp\drivers.h"\
	".\..\..\..\..\..\plplot\tmp\plConfig.h"\
	".\..\..\..\..\..\plplot\tmp\plplot.h"\
	".\..\..\..\..\..\plplot\tmp\plstrm.h"\
	".\..\..\..\..\..\plplot\tmp\pldebug.h"\
	".\..\..\..\..\..\plplot\tmp\pdf.h"\
	

"$(INTDIR)\win3.obj" : $(SOURCE) $(DEP_CPP_WIN3_) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "plpdll - Win32 Debug"

DEP_CPP_WIN3_=\
	".\..\..\..\..\..\plplot\tmp\plDevs.h"\
	".\..\..\..\..\..\plplot\tmp\plplotP.h"\
	".\..\..\..\..\..\plplot\tmp\drivers.h"\
	".\..\..\..\..\..\plplot\tmp\plConfig.h"\
	".\..\..\..\..\..\plplot\tmp\plplot.h"\
	".\..\..\..\..\..\plplot\tmp\plstrm.h"\
	".\..\..\..\..\..\plplot\tmp\pldebug.h"\
	".\..\..\..\..\..\plplot\tmp\pdf.h"\
	
NODEP_CPP_WIN3_=\
	".\..\..\..\..\tmp\CreatePalette"\
	".\..\..\..\..\tmp\dev"\
	".\..\..\..\..\tmp\LocalAlloc"\
	

"$(INTDIR)\win3.obj" : $(SOURCE) $(DEP_CPP_WIN3_) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\plplot\tmp\ps.cpp
DEP_CPP_PS_CP=\
	".\..\..\..\..\..\plplot\tmp\plDevs.h"\
	".\..\..\..\..\..\plplot\tmp\plplotP.h"\
	".\..\..\..\..\..\plplot\tmp\drivers.h"\
	".\..\..\..\..\..\plplot\tmp\plConfig.h"\
	".\..\..\..\..\..\plplot\tmp\plplot.h"\
	".\..\..\..\..\..\plplot\tmp\plstrm.h"\
	".\..\..\..\..\..\plplot\tmp\pldebug.h"\
	".\..\..\..\..\..\plplot\tmp\pdf.h"\
	

"$(INTDIR)\ps.obj" : $(SOURCE) $(DEP_CPP_PS_CP) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=\plplot\tmp\plwind.cpp
DEP_CPP_PLWIN=\
	".\..\..\..\..\..\plplot\tmp\plplotP.h"\
	".\..\..\..\..\..\plplot\tmp\plConfig.h"\
	".\..\..\..\..\..\plplot\tmp\plplot.h"\
	".\..\..\..\..\..\plplot\tmp\plstrm.h"\
	".\..\..\..\..\..\plplot\tmp\pldebug.h"\
	".\..\..\..\..\..\plplot\tmp\pdf.h"\
	

"$(INTDIR)\plwind.obj" : $(SOURCE) $(DEP_CPP_PLWIN) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=\plplot\tmp\plvpor.cpp
DEP_CPP_PLVPO=\
	".\..\..\..\..\..\plplot\tmp\plplotP.h"\
	".\..\..\..\..\..\plplot\tmp\plConfig.h"\
	".\..\..\..\..\..\plplot\tmp\plplot.h"\
	".\..\..\..\..\..\plplot\tmp\plstrm.h"\
	".\..\..\..\..\..\plplot\tmp\pldebug.h"\
	".\..\..\..\..\..\plplot\tmp\pdf.h"\
	

"$(INTDIR)\plvpor.obj" : $(SOURCE) $(DEP_CPP_PLVPO) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=\plplot\tmp\pltick.cpp
DEP_CPP_PLTIC=\
	".\..\..\..\..\..\plplot\tmp\plplotP.h"\
	".\..\..\..\..\..\plplot\tmp\plConfig.h"\
	".\..\..\..\..\..\plplot\tmp\plplot.h"\
	".\..\..\..\..\..\plplot\tmp\plstrm.h"\
	".\..\..\..\..\..\plplot\tmp\pldebug.h"\
	".\..\..\..\..\..\plplot\tmp\pdf.h"\
	

"$(INTDIR)\pltick.obj" : $(SOURCE) $(DEP_CPP_PLTIC) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=\plplot\tmp\plsym.cpp
DEP_CPP_PLSYM=\
	".\..\..\..\..\..\plplot\tmp\plplotP.h"\
	".\..\..\..\..\..\plplot\tmp\plConfig.h"\
	".\..\..\..\..\..\plplot\tmp\plplot.h"\
	".\..\..\..\..\..\plplot\tmp\plstrm.h"\
	".\..\..\..\..\..\plplot\tmp\pldebug.h"\
	".\..\..\..\..\..\plplot\tmp\pdf.h"\
	

"$(INTDIR)\plsym.obj" : $(SOURCE) $(DEP_CPP_PLSYM) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=\plplot\tmp\plstub.cpp
DEP_CPP_PLSTU=\
	".\..\..\..\..\..\plplot\tmp\plplot.h"\
	

"$(INTDIR)\plstub.obj" : $(SOURCE) $(DEP_CPP_PLSTU) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=\plplot\tmp\plshade.cpp
DEP_CPP_PLSHA=\
	".\..\..\..\..\..\plplot\tmp\plplotP.h"\
	".\..\..\..\..\..\plplot\tmp\plConfig.h"\
	".\..\..\..\..\..\plplot\tmp\plplot.h"\
	".\..\..\..\..\..\plplot\tmp\plstrm.h"\
	".\..\..\..\..\..\plplot\tmp\pldebug.h"\
	".\..\..\..\..\..\plplot\tmp\pdf.h"\
	

"$(INTDIR)\plshade.obj" : $(SOURCE) $(DEP_CPP_PLSHA) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=\plplot\tmp\plsdef.cpp
DEP_CPP_PLSDE=\
	".\..\..\..\..\..\plplot\tmp\plplotP.h"\
	".\..\..\..\..\..\plplot\tmp\plConfig.h"\
	".\..\..\..\..\..\plplot\tmp\plplot.h"\
	".\..\..\..\..\..\plplot\tmp\plstrm.h"\
	".\..\..\..\..\..\plplot\tmp\pldebug.h"\
	".\..\..\..\..\..\plplot\tmp\pdf.h"\
	

"$(INTDIR)\plsdef.obj" : $(SOURCE) $(DEP_CPP_PLSDE) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=\plplot\tmp\plpage.cpp
DEP_CPP_PLPAG=\
	".\..\..\..\..\..\plplot\tmp\plplotP.h"\
	".\..\..\..\..\..\plplot\tmp\plConfig.h"\
	".\..\..\..\..\..\plplot\tmp\plplot.h"\
	".\..\..\..\..\..\plplot\tmp\plstrm.h"\
	".\..\..\..\..\..\plplot\tmp\pldebug.h"\
	".\..\..\..\..\..\plplot\tmp\pdf.h"\
	

"$(INTDIR)\plpage.obj" : $(SOURCE) $(DEP_CPP_PLPAG) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=\plplot\tmp\plot3d.cpp
DEP_CPP_PLOT3=\
	".\..\..\..\..\..\plplot\tmp\plplotP.h"\
	".\..\..\..\..\..\plplot\tmp\plConfig.h"\
	".\..\..\..\..\..\plplot\tmp\plplot.h"\
	".\..\..\..\..\..\plplot\tmp\plstrm.h"\
	".\..\..\..\..\..\plplot\tmp\pldebug.h"\
	".\..\..\..\..\..\plplot\tmp\pdf.h"\
	

"$(INTDIR)\plot3d.obj" : $(SOURCE) $(DEP_CPP_PLOT3) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=\plplot\tmp\plmeta.cpp
DEP_CPP_PLMET=\
	".\..\..\..\..\..\plplot\tmp\plDevs.h"\
	".\..\..\..\..\..\plplot\tmp\plplotP.h"\
	".\..\..\..\..\..\plplot\tmp\drivers.h"\
	".\..\..\..\..\..\plplot\tmp\metadefs.h"\
	".\..\..\..\..\..\plplot\tmp\plConfig.h"\
	".\..\..\..\..\..\plplot\tmp\plplot.h"\
	".\..\..\..\..\..\plplot\tmp\plstrm.h"\
	".\..\..\..\..\..\plplot\tmp\pldebug.h"\
	".\..\..\..\..\..\plplot\tmp\pdf.h"\
	

"$(INTDIR)\plmeta.obj" : $(SOURCE) $(DEP_CPP_PLMET) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=\plplot\tmp\plmap.cpp
DEP_CPP_PLMAP=\
	".\..\..\..\..\..\plplot\tmp\plplotP.h"\
	".\..\..\..\..\..\plplot\tmp\plConfig.h"\
	".\..\..\..\..\..\plplot\tmp\plplot.h"\
	".\..\..\..\..\..\plplot\tmp\plstrm.h"\
	".\..\..\..\..\..\plplot\tmp\pldebug.h"\
	".\..\..\..\..\..\plplot\tmp\pdf.h"\
	

"$(INTDIR)\plmap.obj" : $(SOURCE) $(DEP_CPP_PLMAP) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=\plplot\tmp\plline.cpp
DEP_CPP_PLLIN=\
	".\..\..\..\..\..\plplot\tmp\plplotP.h"\
	".\..\..\..\..\..\plplot\tmp\plConfig.h"\
	".\..\..\..\..\..\plplot\tmp\plplot.h"\
	".\..\..\..\..\..\plplot\tmp\plstrm.h"\
	".\..\..\..\..\..\plplot\tmp\pldebug.h"\
	".\..\..\..\..\..\plplot\tmp\pdf.h"\
	

"$(INTDIR)\plline.obj" : $(SOURCE) $(DEP_CPP_PLLIN) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=\plplot\tmp\plhist.cpp
DEP_CPP_PLHIS=\
	".\..\..\..\..\..\plplot\tmp\plplotP.h"\
	".\..\..\..\..\..\plplot\tmp\plConfig.h"\
	".\..\..\..\..\..\plplot\tmp\plplot.h"\
	".\..\..\..\..\..\plplot\tmp\plstrm.h"\
	".\..\..\..\..\..\plplot\tmp\pldebug.h"\
	".\..\..\..\..\..\plplot\tmp\pdf.h"\
	

"$(INTDIR)\plhist.obj" : $(SOURCE) $(DEP_CPP_PLHIS) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=\plplot\tmp\plfill.cpp

!IF  "$(CFG)" == "plpdll - Win32 Release"

DEP_CPP_PLFIL=\
	".\..\..\..\..\..\plplot\tmp\plplotP.h"\
	".\..\..\..\..\..\plplot\tmp\plConfig.h"\
	".\..\..\..\..\..\plplot\tmp\plplot.h"\
	".\..\..\..\..\..\plplot\tmp\plstrm.h"\
	".\..\..\..\..\..\plplot\tmp\pldebug.h"\
	".\..\..\..\..\..\plplot\tmp\pdf.h"\
	

"$(INTDIR)\plfill.obj" : $(SOURCE) $(DEP_CPP_PLFIL) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "plpdll - Win32 Debug"

DEP_CPP_PLFIL=\
	".\..\..\..\..\..\plplot\tmp\plplotP.h"\
	".\..\..\..\..\..\plplot\tmp\plConfig.h"\
	".\..\..\..\..\..\plplot\tmp\plplot.h"\
	".\..\..\..\..\..\plplot\tmp\plstrm.h"\
	".\..\..\..\..\..\plplot\tmp\pldebug.h"\
	".\..\..\..\..\..\plplot\tmp\pdf.h"\
	
NODEP_CPP_PLFIL=\
	"..\..\..\..\tmp\n"\
	"..\..\..\..\tmp\0"\
	"..\..\..\..\tmp\plP_fill"\
	"..\..\..\..\tmp\void"\
	"..\..\..\..\tmp\("\
	

"$(INTDIR)\plfill.obj" : $(SOURCE) $(DEP_CPP_PLFIL) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\plplot\tmp\pldtik.cpp
DEP_CPP_PLDTI=\
	".\..\..\..\..\..\plplot\tmp\plplotP.h"\
	".\..\..\..\..\..\plplot\tmp\plConfig.h"\
	".\..\..\..\..\..\plplot\tmp\plplot.h"\
	".\..\..\..\..\..\plplot\tmp\plstrm.h"\
	".\..\..\..\..\..\plplot\tmp\pldebug.h"\
	".\..\..\..\..\..\plplot\tmp\pdf.h"\
	

"$(INTDIR)\pldtik.obj" : $(SOURCE) $(DEP_CPP_PLDTI) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=\plplot\tmp\plcvt.cpp
DEP_CPP_PLCVT=\
	".\..\..\..\..\..\plplot\tmp\plplotP.h"\
	".\..\..\..\..\..\plplot\tmp\plConfig.h"\
	".\..\..\..\..\..\plplot\tmp\plplot.h"\
	".\..\..\..\..\..\plplot\tmp\plstrm.h"\
	".\..\..\..\..\..\plplot\tmp\pldebug.h"\
	".\..\..\..\..\..\plplot\tmp\pdf.h"\
	

"$(INTDIR)\plcvt.obj" : $(SOURCE) $(DEP_CPP_PLCVT) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=\plplot\tmp\plctrl.cpp
DEP_CPP_PLCTR=\
	".\..\..\..\..\..\plplot\tmp\plplotP.h"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	".\..\..\..\..\..\plplot\tmp\plConfig.h"\
	".\..\..\..\..\..\plplot\tmp\plplot.h"\
	".\..\..\..\..\..\plplot\tmp\plstrm.h"\
	".\..\..\..\..\..\plplot\tmp\pldebug.h"\
	".\..\..\..\..\..\plplot\tmp\pdf.h"\
	

"$(INTDIR)\plctrl.obj" : $(SOURCE) $(DEP_CPP_PLCTR) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=\plplot\tmp\plcore.cpp
DEP_CPP_PLCOR=\
	".\..\..\..\..\..\plplot\tmp\plcore.h"\
	".\..\..\..\..\..\plplot\tmp\plplotP.h"\
	".\..\..\..\..\..\plplot\tmp\drivers.h"\
	".\..\..\..\..\..\plplot\tmp\plDevs.h"\
	".\..\..\..\..\..\plplot\tmp\pldebug.h"\
	".\..\..\..\..\..\plplot\tmp\plConfig.h"\
	".\..\..\..\..\..\plplot\tmp\plplot.h"\
	".\..\..\..\..\..\plplot\tmp\plstrm.h"\
	".\..\..\..\..\..\plplot\tmp\pdf.h"\
	

"$(INTDIR)\plcore.obj" : $(SOURCE) $(DEP_CPP_PLCOR) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=\plplot\tmp\plcont.cpp
DEP_CPP_PLCON=\
	".\..\..\..\..\..\plplot\tmp\plplotP.h"\
	".\..\..\..\..\..\plplot\tmp\plConfig.h"\
	".\..\..\..\..\..\plplot\tmp\plplot.h"\
	".\..\..\..\..\..\plplot\tmp\plstrm.h"\
	".\..\..\..\..\..\plplot\tmp\pldebug.h"\
	".\..\..\..\..\..\plplot\tmp\pdf.h"\
	

"$(INTDIR)\plcont.obj" : $(SOURCE) $(DEP_CPP_PLCON) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=\plplot\tmp\plconsole.cpp

"$(INTDIR)\plconsole.obj" : $(SOURCE) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=\plplot\tmp\plbuf.cpp
DEP_CPP_PLBUF=\
	".\..\..\..\..\..\plplot\tmp\plplotP.h"\
	".\..\..\..\..\..\plplot\tmp\drivers.h"\
	".\..\..\..\..\..\plplot\tmp\metadefs.h"\
	".\..\..\..\..\..\plplot\tmp\plConfig.h"\
	".\..\..\..\..\..\plplot\tmp\plplot.h"\
	".\..\..\..\..\..\plplot\tmp\plstrm.h"\
	".\..\..\..\..\..\plplot\tmp\pldebug.h"\
	".\..\..\..\..\..\plplot\tmp\pdf.h"\
	

"$(INTDIR)\plbuf.obj" : $(SOURCE) $(DEP_CPP_PLBUF) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=\plplot\tmp\plbox.cpp
DEP_CPP_PLBOX=\
	".\..\..\..\..\..\plplot\tmp\plplotP.h"\
	".\..\..\..\..\..\plplot\tmp\plConfig.h"\
	".\..\..\..\..\..\plplot\tmp\plplot.h"\
	".\..\..\..\..\..\plplot\tmp\plstrm.h"\
	".\..\..\..\..\..\plplot\tmp\pldebug.h"\
	".\..\..\..\..\..\plplot\tmp\pdf.h"\
	

"$(INTDIR)\plbox.obj" : $(SOURCE) $(DEP_CPP_PLBOX) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=\plplot\tmp\plargs.cpp
DEP_CPP_PLARG=\
	".\..\..\..\..\..\plplot\tmp\plplotP.h"\
	".\..\..\..\..\..\plplot\tmp\plConfig.h"\
	".\..\..\..\..\..\plplot\tmp\plplot.h"\
	".\..\..\..\..\..\plplot\tmp\plstrm.h"\
	".\..\..\..\..\..\plplot\tmp\pldebug.h"\
	".\..\..\..\..\..\plplot\tmp\pdf.h"\
	

"$(INTDIR)\plargs.obj" : $(SOURCE) $(DEP_CPP_PLARG) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=\plplot\tmp\pdfutils.cpp
DEP_CPP_PDFUT=\
	".\..\..\..\..\..\plplot\tmp\plplotP.h"\
	".\..\..\..\..\..\plplot\tmp\plConfig.h"\
	".\..\..\..\..\..\plplot\tmp\plplot.h"\
	".\..\..\..\..\..\plplot\tmp\plstrm.h"\
	".\..\..\..\..\..\plplot\tmp\pldebug.h"\
	".\..\..\..\..\..\plplot\tmp\pdf.h"\
	

"$(INTDIR)\pdfutils.obj" : $(SOURCE) $(DEP_CPP_PDFUT) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=\plplot\tmp\null.cpp
DEP_CPP_NULL_=\
	".\..\..\..\..\..\plplot\tmp\plDevs.h"\
	".\..\..\..\..\..\plplot\tmp\plplotP.h"\
	".\..\..\..\..\..\plplot\tmp\drivers.h"\
	".\..\..\..\..\..\plplot\tmp\plConfig.h"\
	".\..\..\..\..\..\plplot\tmp\plplot.h"\
	".\..\..\..\..\..\plplot\tmp\plstrm.h"\
	".\..\..\..\..\..\plplot\tmp\pldebug.h"\
	".\..\..\..\..\..\plplot\tmp\pdf.h"\
	

"$(INTDIR)\null.obj" : $(SOURCE) $(DEP_CPP_NULL_) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=\plplot\tmp\hpgl.cpp
DEP_CPP_HPGL_=\
	".\..\..\..\..\..\plplot\tmp\plDevs.h"\
	".\..\..\..\..\..\plplot\tmp\plplotP.h"\
	".\..\..\..\..\..\plplot\tmp\drivers.h"\
	".\..\..\..\..\..\plplot\tmp\plConfig.h"\
	".\..\..\..\..\..\plplot\tmp\plplot.h"\
	".\..\..\..\..\..\plplot\tmp\plstrm.h"\
	".\..\..\..\..\..\plplot\tmp\pldebug.h"\
	".\..\..\..\..\..\plplot\tmp\pdf.h"\
	

"$(INTDIR)\hpgl.obj" : $(SOURCE) $(DEP_CPP_HPGL_) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=\plplot\tmp\plplotd.def

!IF  "$(CFG)" == "plpdll - Win32 Release"

!ELSEIF  "$(CFG)" == "plpdll - Win32 Debug"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\plplot\tmp\plplot.rc
DEP_RSC_PLPLO=\
	".\..\..\..\..\..\plplot\tmp\plplot.ico"\
	

!IF  "$(CFG)" == "plpdll - Win32 Release"


"$(INTDIR)\plplot.res" : $(SOURCE) $(DEP_RSC_PLPLO) "$(INTDIR)"
   $(RSC) /l 0x409 /fo"$(INTDIR)/plplot.res" /i "\plplot\tmp" /d "NDEBUG"\
 $(SOURCE)


!ELSEIF  "$(CFG)" == "plpdll - Win32 Debug"


"$(INTDIR)\plplot.res" : $(SOURCE) $(DEP_RSC_PLPLO) "$(INTDIR)"
   $(RSC) /l 0x409 /fo"$(INTDIR)/plplot.res" /i "\plplot\tmp" /d "_DEBUG"\
 $(SOURCE)


!ENDIF 

# End Source File
# End Target
# End Project
################################################################################
