# Microsoft Developer Studio Generated NMAKE File, Format Version 4.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

!IF "$(CFG)" == ""
CFG=x19c - Win32 Debug
!MESSAGE No configuration specified.  Defaulting to x19c - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "Examples - Win32 Release" && "$(CFG)" !=\
 "Examples - Win32 Debug" && "$(CFG)" != "x01c - Win32 Release" && "$(CFG)" !=\
 "x01c - Win32 Debug" && "$(CFG)" != "x02c - Win32 Release" && "$(CFG)" !=\
 "x02c - Win32 Debug" && "$(CFG)" != "x03c - Win32 Release" && "$(CFG)" !=\
 "x03c - Win32 Debug" && "$(CFG)" != "x04c - Win32 Release" && "$(CFG)" !=\
 "x04c - Win32 Debug" && "$(CFG)" != "x05c - Win32 Release" && "$(CFG)" !=\
 "x05c - Win32 Debug" && "$(CFG)" != "x06c - Win32 Release" && "$(CFG)" !=\
 "x06c - Win32 Debug" && "$(CFG)" != "x07c - Win32 Release" && "$(CFG)" !=\
 "x07c - Win32 Debug" && "$(CFG)" != "x08c - Win32 Release" && "$(CFG)" !=\
 "x08c - Win32 Debug" && "$(CFG)" != "x09c - Win32 Release" && "$(CFG)" !=\
 "x09c - Win32 Debug" && "$(CFG)" != "x10c - Win32 Release" && "$(CFG)" !=\
 "x10c - Win32 Debug" && "$(CFG)" != "x11c - Win32 Release" && "$(CFG)" !=\
 "x11c - Win32 Debug" && "$(CFG)" != "x12c - Win32 Release" && "$(CFG)" !=\
 "x12c - Win32 Debug" && "$(CFG)" != "x13c - Win32 Release" && "$(CFG)" !=\
 "x13c - Win32 Debug" && "$(CFG)" != "x15c - Win32 Release" && "$(CFG)" !=\
 "x15c - Win32 Debug" && "$(CFG)" != "x16c - Win32 Release" && "$(CFG)" !=\
 "x16c - Win32 Debug" && "$(CFG)" != "x18c - Win32 Release" && "$(CFG)" !=\
 "x18c - Win32 Debug" && "$(CFG)" != "x19c - Win32 Release" && "$(CFG)" !=\
 "x19c - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE on this makefile
!MESSAGE by defining the macro CFG on the command line.  For example:
!MESSAGE 
!MESSAGE NMAKE /f "Examples.mak" CFG="x19c - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Examples - Win32 Release" (based on\
 "Win32 (x86) Console Application")
!MESSAGE "Examples - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE "x01c - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "x01c - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE "x02c - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "x02c - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE "x03c - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "x03c - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE "x04c - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "x04c - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE "x05c - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "x05c - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE "x06c - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "x06c - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE "x07c - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "x07c - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE "x08c - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "x08c - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE "x09c - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "x09c - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE "x10c - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "x10c - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE "x11c - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "x11c - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE "x12c - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "x12c - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE "x13c - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "x13c - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE "x15c - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "x15c - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE "x16c - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "x16c - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE "x18c - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "x18c - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE "x19c - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "x19c - Win32 Debug" (based on "Win32 (x86) Console Application")
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
# PROP Target_Last_Scanned "x01c - Win32 Debug"
RSC=rc.exe
CPP=cl.exe

!IF  "$(CFG)" == "Examples - Win32 Release"

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

ALL : 

CLEAN : 
	-@erase 

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /YX /c
CPP_PROJ=/nologo /ML /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE"\
 /Fp"$(INTDIR)/Examples.pch" /YX /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\Release/
CPP_SBRS=
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/Examples.bsc" 
BSC32_SBRS=
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386 /out:"c:\plplot\tmp\x01c.exe"
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib /nologo /subsystem:console /incremental:no\
 /pdb:"$(OUTDIR)/x01c.pdb" /machine:I386 /out:"c:\plplot\tmp\x01c.exe" 
LINK32_OBJS=

!ELSEIF  "$(CFG)" == "Examples - Win32 Debug"

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

ALL : 

CLEAN : 
	-@erase 

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
CPP_PROJ=/nologo /MLd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE"\
 /Fp"$(INTDIR)/Examples.pch" /YX /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\Debug/
CPP_SBRS=
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/Examples.bsc" 
BSC32_SBRS=
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /out:"c:\plplot\tmp\x01c.exe"
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib /nologo /subsystem:console /incremental:yes\
 /pdb:"$(OUTDIR)/x01c.pdb" /debug /machine:I386 /out:"c:\plplot\tmp\x01c.exe" 
LINK32_OBJS=

!ELSEIF  "$(CFG)" == "x01c - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "x01c\Release"
# PROP BASE Intermediate_Dir "x01c\Release"
# PROP BASE Target_Dir "x01c"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "x01c\Release"
# PROP Intermediate_Dir "x01c\Release"
# PROP Target_Dir "x01c"
OUTDIR=.\x01c\Release
INTDIR=.\x01c\Release

ALL : "$(OUTDIR)\x01c.exe"

CLEAN : 
	-@erase ".\x01c\Release\x01c.exe"
	-@erase ".\x01c\Release\x01c.obj"
	-@erase ".\x01c\Release\plplot.res"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /YX /c
CPP_PROJ=/nologo /ML /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE"\
 /Fp"$(INTDIR)/x01c.pch" /YX /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\x01c\Release/
CPP_SBRS=
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/plplot.res" /d "NDEBUG" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/x01c.bsc" 
BSC32_SBRS=
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib /nologo /subsystem:console /incremental:no\
 /pdb:"$(OUTDIR)/x01c.pdb" /machine:I386 /out:"$(OUTDIR)/x01c.exe" 
LINK32_OBJS= \
	"$(INTDIR)/x01c.obj" \
	"$(INTDIR)/plplot.res" \
	"..\..\..\..\lib\plplot.lib"

"$(OUTDIR)\x01c.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "x01c - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "x01c\Debug"
# PROP BASE Intermediate_Dir "x01c\Debug"
# PROP BASE Target_Dir "x01c"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "x01c\Debug"
# PROP Intermediate_Dir "x01c\Debug"
# PROP Target_Dir "x01c"
OUTDIR=.\x01c\Debug
INTDIR=.\x01c\Debug

ALL : "$(OUTDIR)\x01c.exe"

CLEAN : 
	-@erase ".\x01c\Debug\vc40.pdb"
	-@erase ".\x01c\Debug\vc40.idb"
	-@erase ".\x01c\Debug\x01c.pdb"
	-@erase ".\x01c\Debug\x01c.obj"
	-@erase ".\x01c\Debug\plplot.res"
	-@erase ".\x01c\Debug\x01c.exe"
	-@erase ".\x01c\Debug\x01c.ilk"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
CPP_PROJ=/nologo /MLd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE"\
 /Fp"$(INTDIR)/x01c.pch" /YX /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\x01c\Debug/
CPP_SBRS=
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/plplot.res" /d "_DEBUG" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/x01c.bsc" 
BSC32_SBRS=
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib /nologo /subsystem:console /incremental:yes\
 /pdb:"$(OUTDIR)/x01c.pdb" /debug /machine:I386 /out:"$(OUTDIR)/x01c.exe" 
LINK32_OBJS= \
	"$(INTDIR)/x01c.obj" \
	"$(INTDIR)/plplot.res" \
	"..\..\..\..\lib\plplot.lib"

"$(OUTDIR)\x01c.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "x02c - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "x02c\Release"
# PROP BASE Intermediate_Dir "x02c\Release"
# PROP BASE Target_Dir "x02c"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "x02c\Release"
# PROP Intermediate_Dir "x02c\Release"
# PROP Target_Dir "x02c"
OUTDIR=.\x02c\Release
INTDIR=.\x02c\Release

ALL : "$(OUTDIR)\x02c.exe"

CLEAN : 
	-@erase ".\x02c\Release\x02c.exe"
	-@erase ".\x02c\Release\x02c.obj"
	-@erase ".\x02c\Release\plplot.res"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /YX /c
CPP_PROJ=/nologo /ML /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE"\
 /Fp"$(INTDIR)/x02c.pch" /YX /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\x02c\Release/
CPP_SBRS=
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/plplot.res" /d "NDEBUG" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/x02c.bsc" 
BSC32_SBRS=
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib /nologo /subsystem:console /incremental:no\
 /pdb:"$(OUTDIR)/x02c.pdb" /machine:I386 /out:"$(OUTDIR)/x02c.exe" 
LINK32_OBJS= \
	"$(INTDIR)/x02c.obj" \
	"$(INTDIR)/plplot.res" \
	"..\..\..\..\lib\plplot.lib"

"$(OUTDIR)\x02c.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "x02c - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "x02c\Debug"
# PROP BASE Intermediate_Dir "x02c\Debug"
# PROP BASE Target_Dir "x02c"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "x02c\Debug"
# PROP Intermediate_Dir "x02c\Debug"
# PROP Target_Dir "x02c"
OUTDIR=.\x02c\Debug
INTDIR=.\x02c\Debug

ALL : "$(OUTDIR)\x02c.exe"

CLEAN : 
	-@erase ".\x02c\Debug\vc40.pdb"
	-@erase ".\x02c\Debug\vc40.idb"
	-@erase ".\x02c\Debug\x02c.exe"
	-@erase ".\x02c\Debug\x02c.obj"
	-@erase ".\x02c\Debug\plplot.res"
	-@erase ".\x02c\Debug\x02c.ilk"
	-@erase ".\x02c\Debug\x02c.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
CPP_PROJ=/nologo /MLd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE"\
 /Fp"$(INTDIR)/x02c.pch" /YX /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\x02c\Debug/
CPP_SBRS=
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/plplot.res" /d "_DEBUG" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/x02c.bsc" 
BSC32_SBRS=
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib /nologo /subsystem:console /incremental:yes\
 /pdb:"$(OUTDIR)/x02c.pdb" /debug /machine:I386 /out:"$(OUTDIR)/x02c.exe" 
LINK32_OBJS= \
	"$(INTDIR)/x02c.obj" \
	"$(INTDIR)/plplot.res" \
	"..\..\..\..\lib\plplot.lib"

"$(OUTDIR)\x02c.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "x03c - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "x03c\Release"
# PROP BASE Intermediate_Dir "x03c\Release"
# PROP BASE Target_Dir "x03c"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "x03c\Release"
# PROP Intermediate_Dir "x03c\Release"
# PROP Target_Dir "x03c"
OUTDIR=.\x03c\Release
INTDIR=.\x03c\Release

ALL : "$(OUTDIR)\x03c.exe"

CLEAN : 
	-@erase ".\x03c\Release\x03c.exe"
	-@erase ".\x03c\Release\x03c.obj"
	-@erase ".\x03c\Release\plplot.res"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /YX /c
CPP_PROJ=/nologo /ML /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE"\
 /Fp"$(INTDIR)/x03c.pch" /YX /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\x03c\Release/
CPP_SBRS=
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/plplot.res" /d "NDEBUG" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/x03c.bsc" 
BSC32_SBRS=
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib /nologo /subsystem:console /incremental:no\
 /pdb:"$(OUTDIR)/x03c.pdb" /machine:I386 /out:"$(OUTDIR)/x03c.exe" 
LINK32_OBJS= \
	"$(INTDIR)/x03c.obj" \
	"$(INTDIR)/plplot.res" \
	"..\..\..\..\lib\plplot.lib"

"$(OUTDIR)\x03c.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "x03c - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "x03c\Debug"
# PROP BASE Intermediate_Dir "x03c\Debug"
# PROP BASE Target_Dir "x03c"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "x03c\Debug"
# PROP Intermediate_Dir "x03c\Debug"
# PROP Target_Dir "x03c"
OUTDIR=.\x03c\Debug
INTDIR=.\x03c\Debug

ALL : "$(OUTDIR)\x03c.exe"

CLEAN : 
	-@erase ".\x03c\Debug\vc40.pdb"
	-@erase ".\x03c\Debug\vc40.idb"
	-@erase ".\x03c\Debug\x03c.exe"
	-@erase ".\x03c\Debug\x03c.obj"
	-@erase ".\x03c\Debug\plplot.res"
	-@erase ".\x03c\Debug\x03c.ilk"
	-@erase ".\x03c\Debug\x03c.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
CPP_PROJ=/nologo /MLd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE"\
 /Fp"$(INTDIR)/x03c.pch" /YX /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\x03c\Debug/
CPP_SBRS=
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/plplot.res" /d "_DEBUG" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/x03c.bsc" 
BSC32_SBRS=
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib /nologo /subsystem:console /incremental:yes\
 /pdb:"$(OUTDIR)/x03c.pdb" /debug /machine:I386 /out:"$(OUTDIR)/x03c.exe" 
LINK32_OBJS= \
	"$(INTDIR)/x03c.obj" \
	"$(INTDIR)/plplot.res" \
	"..\..\..\..\lib\plplot.lib"

"$(OUTDIR)\x03c.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "x04c - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "x04c\Release"
# PROP BASE Intermediate_Dir "x04c\Release"
# PROP BASE Target_Dir "x04c"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "x04c\Release"
# PROP Intermediate_Dir "x04c\Release"
# PROP Target_Dir "x04c"
OUTDIR=.\x04c\Release
INTDIR=.\x04c\Release

ALL : "$(OUTDIR)\x04c.exe"

CLEAN : 
	-@erase ".\x04c\Release\x04c.exe"
	-@erase ".\x04c\Release\x04c.obj"
	-@erase ".\x04c\Release\plplot.res"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /YX /c
CPP_PROJ=/nologo /ML /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE"\
 /Fp"$(INTDIR)/x04c.pch" /YX /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\x04c\Release/
CPP_SBRS=
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/plplot.res" /d "NDEBUG" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/x04c.bsc" 
BSC32_SBRS=
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib /nologo /subsystem:console /incremental:no\
 /pdb:"$(OUTDIR)/x04c.pdb" /machine:I386 /out:"$(OUTDIR)/x04c.exe" 
LINK32_OBJS= \
	"$(INTDIR)/x04c.obj" \
	"$(INTDIR)/plplot.res" \
	"..\..\..\..\lib\plplot.lib"

"$(OUTDIR)\x04c.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "x04c - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "x04c\Debug"
# PROP BASE Intermediate_Dir "x04c\Debug"
# PROP BASE Target_Dir "x04c"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "x04c\Debug"
# PROP Intermediate_Dir "x04c\Debug"
# PROP Target_Dir "x04c"
OUTDIR=.\x04c\Debug
INTDIR=.\x04c\Debug

ALL : "$(OUTDIR)\x04c.exe"

CLEAN : 
	-@erase ".\x04c\Debug\vc40.pdb"
	-@erase ".\x04c\Debug\vc40.idb"
	-@erase ".\x04c\Debug\x04c.exe"
	-@erase ".\x04c\Debug\x04c.obj"
	-@erase ".\x04c\Debug\plplot.res"
	-@erase ".\x04c\Debug\x04c.ilk"
	-@erase ".\x04c\Debug\x04c.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
CPP_PROJ=/nologo /MLd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE"\
 /Fp"$(INTDIR)/x04c.pch" /YX /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\x04c\Debug/
CPP_SBRS=
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/plplot.res" /d "_DEBUG" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/x04c.bsc" 
BSC32_SBRS=
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib /nologo /subsystem:console /incremental:yes\
 /pdb:"$(OUTDIR)/x04c.pdb" /debug /machine:I386 /out:"$(OUTDIR)/x04c.exe" 
LINK32_OBJS= \
	"$(INTDIR)/x04c.obj" \
	"$(INTDIR)/plplot.res" \
	"..\..\..\..\lib\plplot.lib"

"$(OUTDIR)\x04c.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "x05c - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "x05c\Release"
# PROP BASE Intermediate_Dir "x05c\Release"
# PROP BASE Target_Dir "x05c"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "x05c\Release"
# PROP Intermediate_Dir "x05c\Release"
# PROP Target_Dir "x05c"
OUTDIR=.\x05c\Release
INTDIR=.\x05c\Release

ALL : "$(OUTDIR)\x05c.exe"

CLEAN : 
	-@erase ".\x05c\Release\x05c.exe"
	-@erase ".\x05c\Release\x05c.obj"
	-@erase ".\x05c\Release\plplot.res"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /YX /c
CPP_PROJ=/nologo /ML /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE"\
 /Fp"$(INTDIR)/x05c.pch" /YX /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\x05c\Release/
CPP_SBRS=
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/plplot.res" /d "NDEBUG" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/x05c.bsc" 
BSC32_SBRS=
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib /nologo /subsystem:console /incremental:no\
 /pdb:"$(OUTDIR)/x05c.pdb" /machine:I386 /out:"$(OUTDIR)/x05c.exe" 
LINK32_OBJS= \
	"$(INTDIR)/x05c.obj" \
	"$(INTDIR)/plplot.res" \
	"..\..\..\..\lib\plplot.lib"

"$(OUTDIR)\x05c.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "x05c - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "x05c\Debug"
# PROP BASE Intermediate_Dir "x05c\Debug"
# PROP BASE Target_Dir "x05c"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "x05c\Debug"
# PROP Intermediate_Dir "x05c\Debug"
# PROP Target_Dir "x05c"
OUTDIR=.\x05c\Debug
INTDIR=.\x05c\Debug

ALL : "$(OUTDIR)\x05c.exe"

CLEAN : 
	-@erase ".\x05c\Debug\vc40.pdb"
	-@erase ".\x05c\Debug\vc40.idb"
	-@erase ".\x05c\Debug\x05c.exe"
	-@erase ".\x05c\Debug\x05c.obj"
	-@erase ".\x05c\Debug\plplot.res"
	-@erase ".\x05c\Debug\x05c.ilk"
	-@erase ".\x05c\Debug\x05c.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
CPP_PROJ=/nologo /MLd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE"\
 /Fp"$(INTDIR)/x05c.pch" /YX /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\x05c\Debug/
CPP_SBRS=
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/plplot.res" /d "_DEBUG" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/x05c.bsc" 
BSC32_SBRS=
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib /nologo /subsystem:console /incremental:yes\
 /pdb:"$(OUTDIR)/x05c.pdb" /debug /machine:I386 /out:"$(OUTDIR)/x05c.exe" 
LINK32_OBJS= \
	"$(INTDIR)/x05c.obj" \
	"$(INTDIR)/plplot.res" \
	"..\..\..\..\lib\plplot.lib"

"$(OUTDIR)\x05c.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "x06c - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "x06c\Release"
# PROP BASE Intermediate_Dir "x06c\Release"
# PROP BASE Target_Dir "x06c"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "x06c\Release"
# PROP Intermediate_Dir "x06c\Release"
# PROP Target_Dir "x06c"
OUTDIR=.\x06c\Release
INTDIR=.\x06c\Release

ALL : "$(OUTDIR)\x06c.exe"

CLEAN : 
	-@erase ".\x06c\Release\x06c.exe"
	-@erase ".\x06c\Release\x06c.obj"
	-@erase ".\x06c\Release\plplot.res"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /YX /c
CPP_PROJ=/nologo /ML /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE"\
 /Fp"$(INTDIR)/x06c.pch" /YX /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\x06c\Release/
CPP_SBRS=
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/plplot.res" /d "NDEBUG" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/x06c.bsc" 
BSC32_SBRS=
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib /nologo /subsystem:console /incremental:no\
 /pdb:"$(OUTDIR)/x06c.pdb" /machine:I386 /out:"$(OUTDIR)/x06c.exe" 
LINK32_OBJS= \
	"$(INTDIR)/x06c.obj" \
	"$(INTDIR)/plplot.res" \
	"..\..\..\..\lib\plplot.lib"

"$(OUTDIR)\x06c.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "x06c - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "x06c\Debug"
# PROP BASE Intermediate_Dir "x06c\Debug"
# PROP BASE Target_Dir "x06c"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "x06c\Debug"
# PROP Intermediate_Dir "x06c\Debug"
# PROP Target_Dir "x06c"
OUTDIR=.\x06c\Debug
INTDIR=.\x06c\Debug

ALL : "$(OUTDIR)\x06c.exe"

CLEAN : 
	-@erase ".\x06c\Debug\vc40.pdb"
	-@erase ".\x06c\Debug\vc40.idb"
	-@erase ".\x06c\Debug\x06c.exe"
	-@erase ".\x06c\Debug\x06c.obj"
	-@erase ".\x06c\Debug\plplot.res"
	-@erase ".\x06c\Debug\x06c.ilk"
	-@erase ".\x06c\Debug\x06c.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
CPP_PROJ=/nologo /MLd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE"\
 /Fp"$(INTDIR)/x06c.pch" /YX /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\x06c\Debug/
CPP_SBRS=
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/plplot.res" /d "_DEBUG" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/x06c.bsc" 
BSC32_SBRS=
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib /nologo /subsystem:console /incremental:yes\
 /pdb:"$(OUTDIR)/x06c.pdb" /debug /machine:I386 /out:"$(OUTDIR)/x06c.exe" 
LINK32_OBJS= \
	"$(INTDIR)/x06c.obj" \
	"$(INTDIR)/plplot.res" \
	"..\..\..\..\lib\plplot.lib"

"$(OUTDIR)\x06c.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "x07c - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "x07c\Release"
# PROP BASE Intermediate_Dir "x07c\Release"
# PROP BASE Target_Dir "x07c"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "x07c\Release"
# PROP Intermediate_Dir "x07c\Release"
# PROP Target_Dir "x07c"
OUTDIR=.\x07c\Release
INTDIR=.\x07c\Release

ALL : "$(OUTDIR)\x07c.exe"

CLEAN : 
	-@erase ".\x07c\Release\x07c.exe"
	-@erase ".\x07c\Release\x07c.obj"
	-@erase ".\x07c\Release\plplot.res"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /YX /c
CPP_PROJ=/nologo /ML /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE"\
 /Fp"$(INTDIR)/x07c.pch" /YX /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\x07c\Release/
CPP_SBRS=
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/plplot.res" /d "NDEBUG" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/x07c.bsc" 
BSC32_SBRS=
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib /nologo /subsystem:console /incremental:no\
 /pdb:"$(OUTDIR)/x07c.pdb" /machine:I386 /out:"$(OUTDIR)/x07c.exe" 
LINK32_OBJS= \
	"$(INTDIR)/x07c.obj" \
	"$(INTDIR)/plplot.res" \
	"..\..\..\..\lib\plplot.lib"

"$(OUTDIR)\x07c.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "x07c - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "x07c\Debug"
# PROP BASE Intermediate_Dir "x07c\Debug"
# PROP BASE Target_Dir "x07c"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "x07c\Debug"
# PROP Intermediate_Dir "x07c\Debug"
# PROP Target_Dir "x07c"
OUTDIR=.\x07c\Debug
INTDIR=.\x07c\Debug

ALL : "$(OUTDIR)\x07c.exe"

CLEAN : 
	-@erase ".\x07c\Debug\vc40.pdb"
	-@erase ".\x07c\Debug\vc40.idb"
	-@erase ".\x07c\Debug\x07c.exe"
	-@erase ".\x07c\Debug\x07c.obj"
	-@erase ".\x07c\Debug\plplot.res"
	-@erase ".\x07c\Debug\x07c.ilk"
	-@erase ".\x07c\Debug\x07c.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
CPP_PROJ=/nologo /MLd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE"\
 /Fp"$(INTDIR)/x07c.pch" /YX /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\x07c\Debug/
CPP_SBRS=
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/plplot.res" /d "_DEBUG" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/x07c.bsc" 
BSC32_SBRS=
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib /nologo /subsystem:console /incremental:yes\
 /pdb:"$(OUTDIR)/x07c.pdb" /debug /machine:I386 /out:"$(OUTDIR)/x07c.exe" 
LINK32_OBJS= \
	"$(INTDIR)/x07c.obj" \
	"$(INTDIR)/plplot.res" \
	"..\..\..\..\lib\plplot.lib"

"$(OUTDIR)\x07c.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "x08c - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "x08c\Release"
# PROP BASE Intermediate_Dir "x08c\Release"
# PROP BASE Target_Dir "x08c"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "x08c\Release"
# PROP Intermediate_Dir "x08c\Release"
# PROP Target_Dir "x08c"
OUTDIR=.\x08c\Release
INTDIR=.\x08c\Release

ALL : "$(OUTDIR)\x08c.exe"

CLEAN : 
	-@erase ".\x08c\Release\x08c.exe"
	-@erase ".\x08c\Release\x08c.obj"
	-@erase ".\x08c\Release\plplot.res"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /YX /c
CPP_PROJ=/nologo /ML /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE"\
 /Fp"$(INTDIR)/x08c.pch" /YX /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\x08c\Release/
CPP_SBRS=
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/plplot.res" /d "NDEBUG" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/x08c.bsc" 
BSC32_SBRS=
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib /nologo /subsystem:console /incremental:no\
 /pdb:"$(OUTDIR)/x08c.pdb" /machine:I386 /out:"$(OUTDIR)/x08c.exe" 
LINK32_OBJS= \
	"$(INTDIR)/x08c.obj" \
	"$(INTDIR)/plplot.res" \
	"..\..\..\..\lib\plplot.lib"

"$(OUTDIR)\x08c.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "x08c - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "x08c\Debug"
# PROP BASE Intermediate_Dir "x08c\Debug"
# PROP BASE Target_Dir "x08c"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "x08c\Debug"
# PROP Intermediate_Dir "x08c\Debug"
# PROP Target_Dir "x08c"
OUTDIR=.\x08c\Debug
INTDIR=.\x08c\Debug

ALL : "$(OUTDIR)\x08c.exe"

CLEAN : 
	-@erase ".\x08c\Debug\vc40.pdb"
	-@erase ".\x08c\Debug\vc40.idb"
	-@erase ".\x08c\Debug\x08c.exe"
	-@erase ".\x08c\Debug\x08c.obj"
	-@erase ".\x08c\Debug\plplot.res"
	-@erase ".\x08c\Debug\x08c.ilk"
	-@erase ".\x08c\Debug\x08c.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
CPP_PROJ=/nologo /MLd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE"\
 /Fp"$(INTDIR)/x08c.pch" /YX /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\x08c\Debug/
CPP_SBRS=
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/plplot.res" /d "_DEBUG" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/x08c.bsc" 
BSC32_SBRS=
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib /nologo /subsystem:console /incremental:yes\
 /pdb:"$(OUTDIR)/x08c.pdb" /debug /machine:I386 /out:"$(OUTDIR)/x08c.exe" 
LINK32_OBJS= \
	"$(INTDIR)/x08c.obj" \
	"$(INTDIR)/plplot.res" \
	"..\..\..\..\lib\plplot.lib"

"$(OUTDIR)\x08c.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "x09c - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "x09c\Release"
# PROP BASE Intermediate_Dir "x09c\Release"
# PROP BASE Target_Dir "x09c"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "x09c\Release"
# PROP Intermediate_Dir "x09c\Release"
# PROP Target_Dir "x09c"
OUTDIR=.\x09c\Release
INTDIR=.\x09c\Release

ALL : "$(OUTDIR)\x09c.exe"

CLEAN : 
	-@erase ".\x09c\Release\x09c.exe"
	-@erase ".\x09c\Release\x09c.obj"
	-@erase ".\x09c\Release\plplot.res"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /YX /c
CPP_PROJ=/nologo /ML /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE"\
 /Fp"$(INTDIR)/x09c.pch" /YX /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\x09c\Release/
CPP_SBRS=
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/plplot.res" /d "NDEBUG" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/x09c.bsc" 
BSC32_SBRS=
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib /nologo /subsystem:console /incremental:no\
 /pdb:"$(OUTDIR)/x09c.pdb" /machine:I386 /out:"$(OUTDIR)/x09c.exe" 
LINK32_OBJS= \
	"$(INTDIR)/x09c.obj" \
	"$(INTDIR)/plplot.res" \
	"..\..\..\..\lib\plplot.lib"

"$(OUTDIR)\x09c.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "x09c - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "x09c\Debug"
# PROP BASE Intermediate_Dir "x09c\Debug"
# PROP BASE Target_Dir "x09c"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "x09c\Debug"
# PROP Intermediate_Dir "x09c\Debug"
# PROP Target_Dir "x09c"
OUTDIR=.\x09c\Debug
INTDIR=.\x09c\Debug

ALL : "$(OUTDIR)\x09c.exe"

CLEAN : 
	-@erase ".\x09c\Debug\vc40.pdb"
	-@erase ".\x09c\Debug\vc40.idb"
	-@erase ".\x09c\Debug\x09c.exe"
	-@erase ".\x09c\Debug\x09c.obj"
	-@erase ".\x09c\Debug\plplot.res"
	-@erase ".\x09c\Debug\x09c.ilk"
	-@erase ".\x09c\Debug\x09c.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
CPP_PROJ=/nologo /MLd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE"\
 /Fp"$(INTDIR)/x09c.pch" /YX /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\x09c\Debug/
CPP_SBRS=
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/plplot.res" /d "_DEBUG" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/x09c.bsc" 
BSC32_SBRS=
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib /nologo /subsystem:console /incremental:yes\
 /pdb:"$(OUTDIR)/x09c.pdb" /debug /machine:I386 /out:"$(OUTDIR)/x09c.exe" 
LINK32_OBJS= \
	"$(INTDIR)/x09c.obj" \
	"$(INTDIR)/plplot.res" \
	"..\..\..\..\lib\plplot.lib"

"$(OUTDIR)\x09c.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "x10c - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "x10c\Release"
# PROP BASE Intermediate_Dir "x10c\Release"
# PROP BASE Target_Dir "x10c"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "x10c\Release"
# PROP Intermediate_Dir "x10c\Release"
# PROP Target_Dir "x10c"
OUTDIR=.\x10c\Release
INTDIR=.\x10c\Release

ALL : "$(OUTDIR)\x10c.exe"

CLEAN : 
	-@erase ".\x10c\Release\x10c.exe"
	-@erase ".\x10c\Release\x10c.obj"
	-@erase ".\x10c\Release\plplot.res"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /YX /c
CPP_PROJ=/nologo /ML /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE"\
 /Fp"$(INTDIR)/x10c.pch" /YX /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\x10c\Release/
CPP_SBRS=
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/plplot.res" /d "NDEBUG" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/x10c.bsc" 
BSC32_SBRS=
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib /nologo /subsystem:console /incremental:no\
 /pdb:"$(OUTDIR)/x10c.pdb" /machine:I386 /out:"$(OUTDIR)/x10c.exe" 
LINK32_OBJS= \
	"$(INTDIR)/x10c.obj" \
	"$(INTDIR)/plplot.res" \
	"..\..\..\..\lib\plplot.lib"

"$(OUTDIR)\x10c.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "x10c - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "x10c\Debug"
# PROP BASE Intermediate_Dir "x10c\Debug"
# PROP BASE Target_Dir "x10c"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "x10c\Debug"
# PROP Intermediate_Dir "x10c\Debug"
# PROP Target_Dir "x10c"
OUTDIR=.\x10c\Debug
INTDIR=.\x10c\Debug

ALL : "$(OUTDIR)\x10c.exe"

CLEAN : 
	-@erase ".\x10c\Debug\vc40.pdb"
	-@erase ".\x10c\Debug\vc40.idb"
	-@erase ".\x10c\Debug\x10c.exe"
	-@erase ".\x10c\Debug\x10c.obj"
	-@erase ".\x10c\Debug\plplot.res"
	-@erase ".\x10c\Debug\x10c.ilk"
	-@erase ".\x10c\Debug\x10c.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
CPP_PROJ=/nologo /MLd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE"\
 /Fp"$(INTDIR)/x10c.pch" /YX /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\x10c\Debug/
CPP_SBRS=
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/plplot.res" /d "_DEBUG" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/x10c.bsc" 
BSC32_SBRS=
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib /nologo /subsystem:console /incremental:yes\
 /pdb:"$(OUTDIR)/x10c.pdb" /debug /machine:I386 /out:"$(OUTDIR)/x10c.exe" 
LINK32_OBJS= \
	"$(INTDIR)/x10c.obj" \
	"$(INTDIR)/plplot.res" \
	"..\..\..\..\lib\plplot.lib"

"$(OUTDIR)\x10c.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "x11c - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "x11c\Release"
# PROP BASE Intermediate_Dir "x11c\Release"
# PROP BASE Target_Dir "x11c"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "x11c\Release"
# PROP Intermediate_Dir "x11c\Release"
# PROP Target_Dir "x11c"
OUTDIR=.\x11c\Release
INTDIR=.\x11c\Release

ALL : "$(OUTDIR)\x11c.exe"

CLEAN : 
	-@erase ".\x11c\Release\x11c.exe"
	-@erase ".\x11c\Release\x11c.obj"
	-@erase ".\x11c\Release\plplot.res"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /YX /c
CPP_PROJ=/nologo /ML /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE"\
 /Fp"$(INTDIR)/x11c.pch" /YX /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\x11c\Release/
CPP_SBRS=
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/plplot.res" /d "NDEBUG" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/x11c.bsc" 
BSC32_SBRS=
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib /nologo /subsystem:console /incremental:no\
 /pdb:"$(OUTDIR)/x11c.pdb" /machine:I386 /out:"$(OUTDIR)/x11c.exe" 
LINK32_OBJS= \
	"$(INTDIR)/x11c.obj" \
	"$(INTDIR)/plplot.res" \
	"..\..\..\..\lib\plplot.lib"

"$(OUTDIR)\x11c.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "x11c - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "x11c\Debug"
# PROP BASE Intermediate_Dir "x11c\Debug"
# PROP BASE Target_Dir "x11c"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "x11c\Debug"
# PROP Intermediate_Dir "x11c\Debug"
# PROP Target_Dir "x11c"
OUTDIR=.\x11c\Debug
INTDIR=.\x11c\Debug

ALL : "$(OUTDIR)\x11c.exe"

CLEAN : 
	-@erase ".\x11c\Debug\vc40.pdb"
	-@erase ".\x11c\Debug\vc40.idb"
	-@erase ".\x11c\Debug\x11c.exe"
	-@erase ".\x11c\Debug\x11c.obj"
	-@erase ".\x11c\Debug\plplot.res"
	-@erase ".\x11c\Debug\x11c.ilk"
	-@erase ".\x11c\Debug\x11c.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
CPP_PROJ=/nologo /MLd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE"\
 /Fp"$(INTDIR)/x11c.pch" /YX /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\x11c\Debug/
CPP_SBRS=
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/plplot.res" /d "_DEBUG" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/x11c.bsc" 
BSC32_SBRS=
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib /nologo /subsystem:console /incremental:yes\
 /pdb:"$(OUTDIR)/x11c.pdb" /debug /machine:I386 /out:"$(OUTDIR)/x11c.exe" 
LINK32_OBJS= \
	"$(INTDIR)/x11c.obj" \
	"$(INTDIR)/plplot.res" \
	"..\..\..\..\lib\plplot.lib"

"$(OUTDIR)\x11c.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "x12c - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "x12c\Release"
# PROP BASE Intermediate_Dir "x12c\Release"
# PROP BASE Target_Dir "x12c"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "x12c\Release"
# PROP Intermediate_Dir "x12c\Release"
# PROP Target_Dir "x12c"
OUTDIR=.\x12c\Release
INTDIR=.\x12c\Release

ALL : "$(OUTDIR)\x12c.exe"

CLEAN : 
	-@erase ".\x12c\Release\x12c.exe"
	-@erase ".\x12c\Release\x12c.obj"
	-@erase ".\x12c\Release\plplot.res"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /YX /c
CPP_PROJ=/nologo /ML /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE"\
 /Fp"$(INTDIR)/x12c.pch" /YX /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\x12c\Release/
CPP_SBRS=
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/plplot.res" /d "NDEBUG" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/x12c.bsc" 
BSC32_SBRS=
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib /nologo /subsystem:console /incremental:no\
 /pdb:"$(OUTDIR)/x12c.pdb" /machine:I386 /out:"$(OUTDIR)/x12c.exe" 
LINK32_OBJS= \
	"$(INTDIR)/x12c.obj" \
	"$(INTDIR)/plplot.res" \
	"..\..\..\..\lib\plplot.lib"

"$(OUTDIR)\x12c.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "x12c - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "x12c\Debug"
# PROP BASE Intermediate_Dir "x12c\Debug"
# PROP BASE Target_Dir "x12c"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "x12c\Debug"
# PROP Intermediate_Dir "x12c\Debug"
# PROP Target_Dir "x12c"
OUTDIR=.\x12c\Debug
INTDIR=.\x12c\Debug

ALL : "$(OUTDIR)\x12c.exe"

CLEAN : 
	-@erase ".\x12c\Debug\vc40.pdb"
	-@erase ".\x12c\Debug\vc40.idb"
	-@erase ".\x12c\Debug\x12c.exe"
	-@erase ".\x12c\Debug\x12c.obj"
	-@erase ".\x12c\Debug\plplot.res"
	-@erase ".\x12c\Debug\x12c.ilk"
	-@erase ".\x12c\Debug\x12c.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
CPP_PROJ=/nologo /MLd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE"\
 /Fp"$(INTDIR)/x12c.pch" /YX /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\x12c\Debug/
CPP_SBRS=
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/plplot.res" /d "_DEBUG" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/x12c.bsc" 
BSC32_SBRS=
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib /nologo /subsystem:console /incremental:yes\
 /pdb:"$(OUTDIR)/x12c.pdb" /debug /machine:I386 /out:"$(OUTDIR)/x12c.exe" 
LINK32_OBJS= \
	"$(INTDIR)/x12c.obj" \
	"$(INTDIR)/plplot.res" \
	"..\..\..\..\lib\plplot.lib"

"$(OUTDIR)\x12c.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "x13c - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "x13c\Release"
# PROP BASE Intermediate_Dir "x13c\Release"
# PROP BASE Target_Dir "x13c"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "x13c\Release"
# PROP Intermediate_Dir "x13c\Release"
# PROP Target_Dir "x13c"
OUTDIR=.\x13c\Release
INTDIR=.\x13c\Release

ALL : "$(OUTDIR)\x13c.exe"

CLEAN : 
	-@erase ".\x13c\Release\x13c.exe"
	-@erase ".\x13c\Release\x13c.obj"
	-@erase ".\x13c\Release\plplot.res"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /YX /c
CPP_PROJ=/nologo /ML /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE"\
 /Fp"$(INTDIR)/x13c.pch" /YX /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\x13c\Release/
CPP_SBRS=
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/plplot.res" /d "NDEBUG" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/x13c.bsc" 
BSC32_SBRS=
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib /nologo /subsystem:console /incremental:no\
 /pdb:"$(OUTDIR)/x13c.pdb" /machine:I386 /out:"$(OUTDIR)/x13c.exe" 
LINK32_OBJS= \
	"$(INTDIR)/x13c.obj" \
	"$(INTDIR)/plplot.res" \
	"..\..\..\..\lib\plplot.lib"

"$(OUTDIR)\x13c.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "x13c - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "x13c\Debug"
# PROP BASE Intermediate_Dir "x13c\Debug"
# PROP BASE Target_Dir "x13c"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "x13c\Debug"
# PROP Intermediate_Dir "x13c\Debug"
# PROP Target_Dir "x13c"
OUTDIR=.\x13c\Debug
INTDIR=.\x13c\Debug

ALL : "$(OUTDIR)\x13c.exe"

CLEAN : 
	-@erase ".\x13c\Debug\vc40.pdb"
	-@erase ".\x13c\Debug\vc40.idb"
	-@erase ".\x13c\Debug\x13c.exe"
	-@erase ".\x13c\Debug\x13c.obj"
	-@erase ".\x13c\Debug\plplot.res"
	-@erase ".\x13c\Debug\x13c.ilk"
	-@erase ".\x13c\Debug\x13c.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
CPP_PROJ=/nologo /MLd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE"\
 /Fp"$(INTDIR)/x13c.pch" /YX /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\x13c\Debug/
CPP_SBRS=
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/plplot.res" /d "_DEBUG" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/x13c.bsc" 
BSC32_SBRS=
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib /nologo /subsystem:console /incremental:yes\
 /pdb:"$(OUTDIR)/x13c.pdb" /debug /machine:I386 /out:"$(OUTDIR)/x13c.exe" 
LINK32_OBJS= \
	"$(INTDIR)/x13c.obj" \
	"$(INTDIR)/plplot.res" \
	"..\..\..\..\lib\plplot.lib"

"$(OUTDIR)\x13c.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "x15c - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "x15c\Release"
# PROP BASE Intermediate_Dir "x15c\Release"
# PROP BASE Target_Dir "x15c"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "x15c\Release"
# PROP Intermediate_Dir "x15c\Release"
# PROP Target_Dir "x15c"
OUTDIR=.\x15c\Release
INTDIR=.\x15c\Release

ALL : "$(OUTDIR)\x15c.exe"

CLEAN : 
	-@erase ".\x15c\Release\x15c.exe"
	-@erase ".\x15c\Release\x15c.obj"
	-@erase ".\x15c\Release\plplot.res"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /YX /c
CPP_PROJ=/nologo /ML /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE"\
 /Fp"$(INTDIR)/x15c.pch" /YX /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\x15c\Release/
CPP_SBRS=
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/plplot.res" /d "NDEBUG" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/x15c.bsc" 
BSC32_SBRS=
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib /nologo /subsystem:console /incremental:no\
 /pdb:"$(OUTDIR)/x15c.pdb" /machine:I386 /out:"$(OUTDIR)/x15c.exe" 
LINK32_OBJS= \
	"$(INTDIR)/x15c.obj" \
	"$(INTDIR)/plplot.res" \
	"..\..\..\..\lib\plplot.lib"

"$(OUTDIR)\x15c.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "x15c - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "x15c\Debug"
# PROP BASE Intermediate_Dir "x15c\Debug"
# PROP BASE Target_Dir "x15c"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "x15c\Debug"
# PROP Intermediate_Dir "x15c\Debug"
# PROP Target_Dir "x15c"
OUTDIR=.\x15c\Debug
INTDIR=.\x15c\Debug

ALL : "$(OUTDIR)\x15c.exe"

CLEAN : 
	-@erase ".\x15c\Debug\vc40.pdb"
	-@erase ".\x15c\Debug\vc40.idb"
	-@erase ".\x15c\Debug\x15c.exe"
	-@erase ".\x15c\Debug\x15c.obj"
	-@erase ".\x15c\Debug\plplot.res"
	-@erase ".\x15c\Debug\x15c.ilk"
	-@erase ".\x15c\Debug\x15c.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
CPP_PROJ=/nologo /MLd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE"\
 /Fp"$(INTDIR)/x15c.pch" /YX /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\x15c\Debug/
CPP_SBRS=
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/plplot.res" /d "_DEBUG" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/x15c.bsc" 
BSC32_SBRS=
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib /nologo /subsystem:console /incremental:yes\
 /pdb:"$(OUTDIR)/x15c.pdb" /debug /machine:I386 /out:"$(OUTDIR)/x15c.exe" 
LINK32_OBJS= \
	"$(INTDIR)/x15c.obj" \
	"$(INTDIR)/plplot.res" \
	"..\..\..\..\lib\plplot.lib"

"$(OUTDIR)\x15c.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "x16c - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "x16c\Release"
# PROP BASE Intermediate_Dir "x16c\Release"
# PROP BASE Target_Dir "x16c"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "x16c\Release"
# PROP Intermediate_Dir "x16c\Release"
# PROP Target_Dir "x16c"
OUTDIR=.\x16c\Release
INTDIR=.\x16c\Release

ALL : "$(OUTDIR)\x16c.exe"

CLEAN : 
	-@erase ".\x16c\Release\x16c.exe"
	-@erase ".\x16c\Release\x16c.obj"
	-@erase ".\x16c\Release\plplot.res"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /YX /c
CPP_PROJ=/nologo /ML /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE"\
 /Fp"$(INTDIR)/x16c.pch" /YX /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\x16c\Release/
CPP_SBRS=
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/plplot.res" /d "NDEBUG" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/x16c.bsc" 
BSC32_SBRS=
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib /nologo /subsystem:console /incremental:no\
 /pdb:"$(OUTDIR)/x16c.pdb" /machine:I386 /out:"$(OUTDIR)/x16c.exe" 
LINK32_OBJS= \
	"$(INTDIR)/x16c.obj" \
	"$(INTDIR)/plplot.res" \
	"..\..\..\..\lib\plplot.lib"

"$(OUTDIR)\x16c.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "x16c - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "x16c\Debug"
# PROP BASE Intermediate_Dir "x16c\Debug"
# PROP BASE Target_Dir "x16c"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "x16c\Debug"
# PROP Intermediate_Dir "x16c\Debug"
# PROP Target_Dir "x16c"
OUTDIR=.\x16c\Debug
INTDIR=.\x16c\Debug

ALL : "$(OUTDIR)\x16c.exe"

CLEAN : 
	-@erase ".\x16c\Debug\vc40.pdb"
	-@erase ".\x16c\Debug\vc40.idb"
	-@erase ".\x16c\Debug\x16c.exe"
	-@erase ".\x16c\Debug\x16c.obj"
	-@erase ".\x16c\Debug\plplot.res"
	-@erase ".\x16c\Debug\x16c.ilk"
	-@erase ".\x16c\Debug\x16c.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
CPP_PROJ=/nologo /MLd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE"\
 /Fp"$(INTDIR)/x16c.pch" /YX /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\x16c\Debug/
CPP_SBRS=
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/plplot.res" /d "_DEBUG" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/x16c.bsc" 
BSC32_SBRS=
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib /nologo /subsystem:console /incremental:yes\
 /pdb:"$(OUTDIR)/x16c.pdb" /debug /machine:I386 /out:"$(OUTDIR)/x16c.exe" 
LINK32_OBJS= \
	"$(INTDIR)/x16c.obj" \
	"$(INTDIR)/plplot.res" \
	"..\..\..\..\lib\plplot.lib"

"$(OUTDIR)\x16c.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "x18c - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "x18c\Release"
# PROP BASE Intermediate_Dir "x18c\Release"
# PROP BASE Target_Dir "x18c"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "x18c\Release"
# PROP Intermediate_Dir "x18c\Release"
# PROP Target_Dir "x18c"
OUTDIR=.\x18c\Release
INTDIR=.\x18c\Release

ALL : "$(OUTDIR)\x18c.exe"

CLEAN : 
	-@erase ".\x18c\Release\x18c.exe"
	-@erase ".\x18c\Release\x18c.obj"
	-@erase ".\x18c\Release\plplot.res"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /YX /c
CPP_PROJ=/nologo /ML /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE"\
 /Fp"$(INTDIR)/x18c.pch" /YX /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\x18c\Release/
CPP_SBRS=
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/plplot.res" /d "NDEBUG" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/x18c.bsc" 
BSC32_SBRS=
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib /nologo /subsystem:console /incremental:no\
 /pdb:"$(OUTDIR)/x18c.pdb" /machine:I386 /out:"$(OUTDIR)/x18c.exe" 
LINK32_OBJS= \
	"$(INTDIR)/x18c.obj" \
	"$(INTDIR)/plplot.res" \
	"..\..\..\..\lib\plplot.lib"

"$(OUTDIR)\x18c.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "x18c - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "x18c\Debug"
# PROP BASE Intermediate_Dir "x18c\Debug"
# PROP BASE Target_Dir "x18c"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "x18c\Debug"
# PROP Intermediate_Dir "x18c\Debug"
# PROP Target_Dir "x18c"
OUTDIR=.\x18c\Debug
INTDIR=.\x18c\Debug

ALL : "$(OUTDIR)\x18c.exe"

CLEAN : 
	-@erase ".\x18c\Debug\vc40.pdb"
	-@erase ".\x18c\Debug\vc40.idb"
	-@erase ".\x18c\Debug\x18c.exe"
	-@erase ".\x18c\Debug\x18c.obj"
	-@erase ".\x18c\Debug\plplot.res"
	-@erase ".\x18c\Debug\x18c.ilk"
	-@erase ".\x18c\Debug\x18c.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
CPP_PROJ=/nologo /MLd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE"\
 /Fp"$(INTDIR)/x18c.pch" /YX /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\x18c\Debug/
CPP_SBRS=
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/plplot.res" /d "_DEBUG" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/x18c.bsc" 
BSC32_SBRS=
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib /nologo /subsystem:console /incremental:yes\
 /pdb:"$(OUTDIR)/x18c.pdb" /debug /machine:I386 /out:"$(OUTDIR)/x18c.exe" 
LINK32_OBJS= \
	"$(INTDIR)/x18c.obj" \
	"$(INTDIR)/plplot.res" \
	"..\..\..\..\lib\plplot.lib"

"$(OUTDIR)\x18c.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "x19c - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "x19c\Release"
# PROP BASE Intermediate_Dir "x19c\Release"
# PROP BASE Target_Dir "x19c"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "x19c\Release"
# PROP Intermediate_Dir "x19c\Release"
# PROP Target_Dir "x19c"
OUTDIR=.\x19c\Release
INTDIR=.\x19c\Release

ALL : "$(OUTDIR)\x19c.exe"

CLEAN : 
	-@erase ".\x19c\Release\x19c.exe"
	-@erase ".\x19c\Release\x19c.obj"
	-@erase ".\x19c\Release\plplot.res"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /YX /c
CPP_PROJ=/nologo /ML /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE"\
 /Fp"$(INTDIR)/x19c.pch" /YX /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\x19c\Release/
CPP_SBRS=
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/plplot.res" /d "NDEBUG" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/x19c.bsc" 
BSC32_SBRS=
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib /nologo /subsystem:console /incremental:no\
 /pdb:"$(OUTDIR)/x19c.pdb" /machine:I386 /out:"$(OUTDIR)/x19c.exe" 
LINK32_OBJS= \
	"$(INTDIR)/x19c.obj" \
	"$(INTDIR)/plplot.res" \
	"..\..\..\..\lib\plplot.lib"

"$(OUTDIR)\x19c.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "x19c - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "x19c\Debug"
# PROP BASE Intermediate_Dir "x19c\Debug"
# PROP BASE Target_Dir "x19c"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "x19c\Debug"
# PROP Intermediate_Dir "x19c\Debug"
# PROP Target_Dir "x19c"
OUTDIR=.\x19c\Debug
INTDIR=.\x19c\Debug

ALL : "$(OUTDIR)\x19c.exe"

CLEAN : 
	-@erase ".\x19c\Debug\vc40.pdb"
	-@erase ".\x19c\Debug\vc40.idb"
	-@erase ".\x19c\Debug\x19c.exe"
	-@erase ".\x19c\Debug\x19c.obj"
	-@erase ".\x19c\Debug\plplot.res"
	-@erase ".\x19c\Debug\x19c.ilk"
	-@erase ".\x19c\Debug\x19c.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
CPP_PROJ=/nologo /MLd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE"\
 /Fp"$(INTDIR)/x19c.pch" /YX /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\x19c\Debug/
CPP_SBRS=
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/plplot.res" /d "_DEBUG" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/x19c.bsc" 
BSC32_SBRS=
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib /nologo /subsystem:console /incremental:yes\
 /pdb:"$(OUTDIR)/x19c.pdb" /debug /machine:I386 /out:"$(OUTDIR)/x19c.exe" 
LINK32_OBJS= \
	"$(INTDIR)/x19c.obj" \
	"$(INTDIR)/plplot.res" \
	"..\..\..\..\lib\plplot.lib"

"$(OUTDIR)\x19c.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
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

# Name "Examples - Win32 Release"
# Name "Examples - Win32 Debug"

!IF  "$(CFG)" == "Examples - Win32 Release"

!ELSEIF  "$(CFG)" == "Examples - Win32 Debug"

!ENDIF 

# End Target
################################################################################
# Begin Target

# Name "x01c - Win32 Release"
# Name "x01c - Win32 Debug"

!IF  "$(CFG)" == "x01c - Win32 Release"

!ELSEIF  "$(CFG)" == "x01c - Win32 Debug"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=\plplot\tmp\x01c.cpp
DEP_CPP_X01C_=\
	".\..\..\..\..\tmp\plcdemos.h"\
	".\..\..\..\..\tmp\plevent.h"\
	".\..\..\..\..\tmp\plplot.h"\
	

"$(INTDIR)\x01c.obj" : $(SOURCE) $(DEP_CPP_X01C_) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=\plplot\tmp\plplot.rc

!IF  "$(CFG)" == "x01c - Win32 Release"

DEP_RSC_PLPLO=\
	".\..\..\..\..\tmp\plplot.ico"\
	

"$(INTDIR)\plplot.res" : $(SOURCE) $(DEP_RSC_PLPLO) "$(INTDIR)"
   $(RSC) /l 0x409 /fo"$(INTDIR)/plplot.res" /i "\plplot\tmp" /d "NDEBUG"\
 $(SOURCE)


!ELSEIF  "$(CFG)" == "x01c - Win32 Debug"

DEP_RSC_PLPLO=\
	".\..\..\..\..\tmp\plplot.ico"\
	

"$(INTDIR)\plplot.res" : $(SOURCE) $(DEP_RSC_PLPLO) "$(INTDIR)"
   $(RSC) /l 0x409 /fo"$(INTDIR)/plplot.res" /i "\plplot\tmp" /d "_DEBUG"\
 $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\plplot\lib\plplot.lib

!IF  "$(CFG)" == "x01c - Win32 Release"

!ELSEIF  "$(CFG)" == "x01c - Win32 Debug"

!ENDIF 

# End Source File
# End Target
################################################################################
# Begin Target

# Name "x02c - Win32 Release"
# Name "x02c - Win32 Debug"

!IF  "$(CFG)" == "x02c - Win32 Release"

!ELSEIF  "$(CFG)" == "x02c - Win32 Debug"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=\plplot\tmp\x02c.cpp
DEP_CPP_X02C_=\
	".\..\..\..\..\tmp\plcdemos.h"\
	".\..\..\..\..\tmp\plplot.h"\
	

"$(INTDIR)\x02c.obj" : $(SOURCE) $(DEP_CPP_X02C_) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=\plplot\lib\plplot.lib

!IF  "$(CFG)" == "x02c - Win32 Release"

!ELSEIF  "$(CFG)" == "x02c - Win32 Debug"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\plplot\tmp\plplot.rc

!IF  "$(CFG)" == "x02c - Win32 Release"

DEP_RSC_PLPLO=\
	".\..\..\..\..\tmp\plplot.ico"\
	

"$(INTDIR)\plplot.res" : $(SOURCE) $(DEP_RSC_PLPLO) "$(INTDIR)"
   $(RSC) /l 0x409 /fo"$(INTDIR)/plplot.res" /i "\plplot\tmp" /d "NDEBUG"\
 $(SOURCE)


!ELSEIF  "$(CFG)" == "x02c - Win32 Debug"

DEP_RSC_PLPLO=\
	".\..\..\..\..\tmp\plplot.ico"\
	

"$(INTDIR)\plplot.res" : $(SOURCE) $(DEP_RSC_PLPLO) "$(INTDIR)"
   $(RSC) /l 0x409 /fo"$(INTDIR)/plplot.res" /i "\plplot\tmp" /d "_DEBUG"\
 $(SOURCE)


!ENDIF 

# End Source File
# End Target
################################################################################
# Begin Target

# Name "x03c - Win32 Release"
# Name "x03c - Win32 Debug"

!IF  "$(CFG)" == "x03c - Win32 Release"

!ELSEIF  "$(CFG)" == "x03c - Win32 Debug"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=\plplot\tmp\x03c.cpp
DEP_CPP_X03C_=\
	".\..\..\..\..\tmp\plcdemos.h"\
	".\..\..\..\..\tmp\plplot.h"\
	

"$(INTDIR)\x03c.obj" : $(SOURCE) $(DEP_CPP_X03C_) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=\plplot\lib\plplot.lib

!IF  "$(CFG)" == "x03c - Win32 Release"

!ELSEIF  "$(CFG)" == "x03c - Win32 Debug"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\plplot\tmp\plplot.rc

!IF  "$(CFG)" == "x03c - Win32 Release"

DEP_RSC_PLPLO=\
	".\..\..\..\..\tmp\plplot.ico"\
	

"$(INTDIR)\plplot.res" : $(SOURCE) $(DEP_RSC_PLPLO) "$(INTDIR)"
   $(RSC) /l 0x409 /fo"$(INTDIR)/plplot.res" /i "\plplot\tmp" /d "NDEBUG"\
 $(SOURCE)


!ELSEIF  "$(CFG)" == "x03c - Win32 Debug"

DEP_RSC_PLPLO=\
	".\..\..\..\..\tmp\plplot.ico"\
	

"$(INTDIR)\plplot.res" : $(SOURCE) $(DEP_RSC_PLPLO) "$(INTDIR)"
   $(RSC) /l 0x409 /fo"$(INTDIR)/plplot.res" /i "\plplot\tmp" /d "_DEBUG"\
 $(SOURCE)


!ENDIF 

# End Source File
# End Target
################################################################################
# Begin Target

# Name "x04c - Win32 Release"
# Name "x04c - Win32 Debug"

!IF  "$(CFG)" == "x04c - Win32 Release"

!ELSEIF  "$(CFG)" == "x04c - Win32 Debug"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=\plplot\tmp\x04c.cpp
DEP_CPP_X04C_=\
	".\..\..\..\..\tmp\plcdemos.h"\
	".\..\..\..\..\tmp\plplot.h"\
	

"$(INTDIR)\x04c.obj" : $(SOURCE) $(DEP_CPP_X04C_) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=\plplot\lib\plplot.lib

!IF  "$(CFG)" == "x04c - Win32 Release"

!ELSEIF  "$(CFG)" == "x04c - Win32 Debug"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\plplot\tmp\plplot.rc

!IF  "$(CFG)" == "x04c - Win32 Release"

DEP_RSC_PLPLO=\
	".\..\..\..\..\tmp\plplot.ico"\
	

"$(INTDIR)\plplot.res" : $(SOURCE) $(DEP_RSC_PLPLO) "$(INTDIR)"
   $(RSC) /l 0x409 /fo"$(INTDIR)/plplot.res" /i "\plplot\tmp" /d "NDEBUG"\
 $(SOURCE)


!ELSEIF  "$(CFG)" == "x04c - Win32 Debug"

DEP_RSC_PLPLO=\
	".\..\..\..\..\tmp\plplot.ico"\
	

"$(INTDIR)\plplot.res" : $(SOURCE) $(DEP_RSC_PLPLO) "$(INTDIR)"
   $(RSC) /l 0x409 /fo"$(INTDIR)/plplot.res" /i "\plplot\tmp" /d "_DEBUG"\
 $(SOURCE)


!ENDIF 

# End Source File
# End Target
################################################################################
# Begin Target

# Name "x05c - Win32 Release"
# Name "x05c - Win32 Debug"

!IF  "$(CFG)" == "x05c - Win32 Release"

!ELSEIF  "$(CFG)" == "x05c - Win32 Debug"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=\plplot\tmp\x05c.cpp
DEP_CPP_X05C_=\
	".\..\..\..\..\tmp\plcdemos.h"\
	".\..\..\..\..\tmp\plplot.h"\
	

"$(INTDIR)\x05c.obj" : $(SOURCE) $(DEP_CPP_X05C_) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=\plplot\lib\plplot.lib

!IF  "$(CFG)" == "x05c - Win32 Release"

!ELSEIF  "$(CFG)" == "x05c - Win32 Debug"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\plplot\tmp\plplot.rc

!IF  "$(CFG)" == "x05c - Win32 Release"

DEP_RSC_PLPLO=\
	".\..\..\..\..\tmp\plplot.ico"\
	

"$(INTDIR)\plplot.res" : $(SOURCE) $(DEP_RSC_PLPLO) "$(INTDIR)"
   $(RSC) /l 0x409 /fo"$(INTDIR)/plplot.res" /i "\plplot\tmp" /d "NDEBUG"\
 $(SOURCE)


!ELSEIF  "$(CFG)" == "x05c - Win32 Debug"

DEP_RSC_PLPLO=\
	".\..\..\..\..\tmp\plplot.ico"\
	

"$(INTDIR)\plplot.res" : $(SOURCE) $(DEP_RSC_PLPLO) "$(INTDIR)"
   $(RSC) /l 0x409 /fo"$(INTDIR)/plplot.res" /i "\plplot\tmp" /d "_DEBUG"\
 $(SOURCE)


!ENDIF 

# End Source File
# End Target
################################################################################
# Begin Target

# Name "x06c - Win32 Release"
# Name "x06c - Win32 Debug"

!IF  "$(CFG)" == "x06c - Win32 Release"

!ELSEIF  "$(CFG)" == "x06c - Win32 Debug"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=\plplot\tmp\x06c.cpp
DEP_CPP_X06C_=\
	".\..\..\..\..\tmp\plcdemos.h"\
	".\..\..\..\..\tmp\plplot.h"\
	

"$(INTDIR)\x06c.obj" : $(SOURCE) $(DEP_CPP_X06C_) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=\plplot\lib\plplot.lib

!IF  "$(CFG)" == "x06c - Win32 Release"

!ELSEIF  "$(CFG)" == "x06c - Win32 Debug"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\plplot\tmp\plplot.rc

!IF  "$(CFG)" == "x06c - Win32 Release"

DEP_RSC_PLPLO=\
	".\..\..\..\..\tmp\plplot.ico"\
	

"$(INTDIR)\plplot.res" : $(SOURCE) $(DEP_RSC_PLPLO) "$(INTDIR)"
   $(RSC) /l 0x409 /fo"$(INTDIR)/plplot.res" /i "\plplot\tmp" /d "NDEBUG"\
 $(SOURCE)


!ELSEIF  "$(CFG)" == "x06c - Win32 Debug"

DEP_RSC_PLPLO=\
	".\..\..\..\..\tmp\plplot.ico"\
	

"$(INTDIR)\plplot.res" : $(SOURCE) $(DEP_RSC_PLPLO) "$(INTDIR)"
   $(RSC) /l 0x409 /fo"$(INTDIR)/plplot.res" /i "\plplot\tmp" /d "_DEBUG"\
 $(SOURCE)


!ENDIF 

# End Source File
# End Target
################################################################################
# Begin Target

# Name "x07c - Win32 Release"
# Name "x07c - Win32 Debug"

!IF  "$(CFG)" == "x07c - Win32 Release"

!ELSEIF  "$(CFG)" == "x07c - Win32 Debug"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=\plplot\tmp\x07c.cpp
DEP_CPP_X07C_=\
	".\..\..\..\..\tmp\plcdemos.h"\
	".\..\..\..\..\tmp\plplot.h"\
	

"$(INTDIR)\x07c.obj" : $(SOURCE) $(DEP_CPP_X07C_) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=\plplot\lib\plplot.lib

!IF  "$(CFG)" == "x07c - Win32 Release"

!ELSEIF  "$(CFG)" == "x07c - Win32 Debug"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\plplot\tmp\plplot.rc

!IF  "$(CFG)" == "x07c - Win32 Release"

DEP_RSC_PLPLO=\
	".\..\..\..\..\tmp\plplot.ico"\
	

"$(INTDIR)\plplot.res" : $(SOURCE) $(DEP_RSC_PLPLO) "$(INTDIR)"
   $(RSC) /l 0x409 /fo"$(INTDIR)/plplot.res" /i "\plplot\tmp" /d "NDEBUG"\
 $(SOURCE)


!ELSEIF  "$(CFG)" == "x07c - Win32 Debug"

DEP_RSC_PLPLO=\
	".\..\..\..\..\tmp\plplot.ico"\
	

"$(INTDIR)\plplot.res" : $(SOURCE) $(DEP_RSC_PLPLO) "$(INTDIR)"
   $(RSC) /l 0x409 /fo"$(INTDIR)/plplot.res" /i "\plplot\tmp" /d "_DEBUG"\
 $(SOURCE)


!ENDIF 

# End Source File
# End Target
################################################################################
# Begin Target

# Name "x08c - Win32 Release"
# Name "x08c - Win32 Debug"

!IF  "$(CFG)" == "x08c - Win32 Release"

!ELSEIF  "$(CFG)" == "x08c - Win32 Debug"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=\plplot\tmp\x08c.cpp
DEP_CPP_X08C_=\
	".\..\..\..\..\tmp\plcdemos.h"\
	".\..\..\..\..\tmp\plplot.h"\
	

"$(INTDIR)\x08c.obj" : $(SOURCE) $(DEP_CPP_X08C_) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=\plplot\lib\plplot.lib

!IF  "$(CFG)" == "x08c - Win32 Release"

!ELSEIF  "$(CFG)" == "x08c - Win32 Debug"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\plplot\tmp\plplot.rc

!IF  "$(CFG)" == "x08c - Win32 Release"

DEP_RSC_PLPLO=\
	".\..\..\..\..\tmp\plplot.ico"\
	

"$(INTDIR)\plplot.res" : $(SOURCE) $(DEP_RSC_PLPLO) "$(INTDIR)"
   $(RSC) /l 0x409 /fo"$(INTDIR)/plplot.res" /i "\plplot\tmp" /d "NDEBUG"\
 $(SOURCE)


!ELSEIF  "$(CFG)" == "x08c - Win32 Debug"

DEP_RSC_PLPLO=\
	".\..\..\..\..\tmp\plplot.ico"\
	

"$(INTDIR)\plplot.res" : $(SOURCE) $(DEP_RSC_PLPLO) "$(INTDIR)"
   $(RSC) /l 0x409 /fo"$(INTDIR)/plplot.res" /i "\plplot\tmp" /d "_DEBUG"\
 $(SOURCE)


!ENDIF 

# End Source File
# End Target
################################################################################
# Begin Target

# Name "x09c - Win32 Release"
# Name "x09c - Win32 Debug"

!IF  "$(CFG)" == "x09c - Win32 Release"

!ELSEIF  "$(CFG)" == "x09c - Win32 Debug"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=\plplot\tmp\x09c.cpp
DEP_CPP_X09C_=\
	".\..\..\..\..\tmp\plcdemos.h"\
	".\..\..\..\..\tmp\plplot.h"\
	

"$(INTDIR)\x09c.obj" : $(SOURCE) $(DEP_CPP_X09C_) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=\plplot\lib\plplot.lib

!IF  "$(CFG)" == "x09c - Win32 Release"

!ELSEIF  "$(CFG)" == "x09c - Win32 Debug"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\plplot\tmp\plplot.rc

!IF  "$(CFG)" == "x09c - Win32 Release"

DEP_RSC_PLPLO=\
	".\..\..\..\..\tmp\plplot.ico"\
	

"$(INTDIR)\plplot.res" : $(SOURCE) $(DEP_RSC_PLPLO) "$(INTDIR)"
   $(RSC) /l 0x409 /fo"$(INTDIR)/plplot.res" /i "\plplot\tmp" /d "NDEBUG"\
 $(SOURCE)


!ELSEIF  "$(CFG)" == "x09c - Win32 Debug"

DEP_RSC_PLPLO=\
	".\..\..\..\..\tmp\plplot.ico"\
	

"$(INTDIR)\plplot.res" : $(SOURCE) $(DEP_RSC_PLPLO) "$(INTDIR)"
   $(RSC) /l 0x409 /fo"$(INTDIR)/plplot.res" /i "\plplot\tmp" /d "_DEBUG"\
 $(SOURCE)


!ENDIF 

# End Source File
# End Target
################################################################################
# Begin Target

# Name "x10c - Win32 Release"
# Name "x10c - Win32 Debug"

!IF  "$(CFG)" == "x10c - Win32 Release"

!ELSEIF  "$(CFG)" == "x10c - Win32 Debug"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=\plplot\tmp\x10c.cpp
DEP_CPP_X10C_=\
	".\..\..\..\..\tmp\plcdemos.h"\
	".\..\..\..\..\tmp\plplot.h"\
	

"$(INTDIR)\x10c.obj" : $(SOURCE) $(DEP_CPP_X10C_) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=\plplot\lib\plplot.lib

!IF  "$(CFG)" == "x10c - Win32 Release"

!ELSEIF  "$(CFG)" == "x10c - Win32 Debug"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\plplot\tmp\plplot.rc

!IF  "$(CFG)" == "x10c - Win32 Release"

DEP_RSC_PLPLO=\
	".\..\..\..\..\tmp\plplot.ico"\
	

"$(INTDIR)\plplot.res" : $(SOURCE) $(DEP_RSC_PLPLO) "$(INTDIR)"
   $(RSC) /l 0x409 /fo"$(INTDIR)/plplot.res" /i "\plplot\tmp" /d "NDEBUG"\
 $(SOURCE)


!ELSEIF  "$(CFG)" == "x10c - Win32 Debug"

DEP_RSC_PLPLO=\
	".\..\..\..\..\tmp\plplot.ico"\
	

"$(INTDIR)\plplot.res" : $(SOURCE) $(DEP_RSC_PLPLO) "$(INTDIR)"
   $(RSC) /l 0x409 /fo"$(INTDIR)/plplot.res" /i "\plplot\tmp" /d "_DEBUG"\
 $(SOURCE)


!ENDIF 

# End Source File
# End Target
################################################################################
# Begin Target

# Name "x11c - Win32 Release"
# Name "x11c - Win32 Debug"

!IF  "$(CFG)" == "x11c - Win32 Release"

!ELSEIF  "$(CFG)" == "x11c - Win32 Debug"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=\plplot\tmp\x11c.cpp
DEP_CPP_X11C_=\
	".\..\..\..\..\tmp\plcdemos.h"\
	".\..\..\..\..\tmp\plplot.h"\
	

"$(INTDIR)\x11c.obj" : $(SOURCE) $(DEP_CPP_X11C_) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=\plplot\lib\plplot.lib

!IF  "$(CFG)" == "x11c - Win32 Release"

!ELSEIF  "$(CFG)" == "x11c - Win32 Debug"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\plplot\tmp\plplot.rc

!IF  "$(CFG)" == "x11c - Win32 Release"

DEP_RSC_PLPLO=\
	".\..\..\..\..\tmp\plplot.ico"\
	

"$(INTDIR)\plplot.res" : $(SOURCE) $(DEP_RSC_PLPLO) "$(INTDIR)"
   $(RSC) /l 0x409 /fo"$(INTDIR)/plplot.res" /i "\plplot\tmp" /d "NDEBUG"\
 $(SOURCE)


!ELSEIF  "$(CFG)" == "x11c - Win32 Debug"

DEP_RSC_PLPLO=\
	".\..\..\..\..\tmp\plplot.ico"\
	

"$(INTDIR)\plplot.res" : $(SOURCE) $(DEP_RSC_PLPLO) "$(INTDIR)"
   $(RSC) /l 0x409 /fo"$(INTDIR)/plplot.res" /i "\plplot\tmp" /d "_DEBUG"\
 $(SOURCE)


!ENDIF 

# End Source File
# End Target
################################################################################
# Begin Target

# Name "x12c - Win32 Release"
# Name "x12c - Win32 Debug"

!IF  "$(CFG)" == "x12c - Win32 Release"

!ELSEIF  "$(CFG)" == "x12c - Win32 Debug"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=\plplot\tmp\x12c.cpp
DEP_CPP_X12C_=\
	".\..\..\..\..\tmp\plcdemos.h"\
	".\..\..\..\..\tmp\plplot.h"\
	

"$(INTDIR)\x12c.obj" : $(SOURCE) $(DEP_CPP_X12C_) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=\plplot\lib\plplot.lib

!IF  "$(CFG)" == "x12c - Win32 Release"

!ELSEIF  "$(CFG)" == "x12c - Win32 Debug"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\plplot\tmp\plplot.rc

!IF  "$(CFG)" == "x12c - Win32 Release"

DEP_RSC_PLPLO=\
	".\..\..\..\..\tmp\plplot.ico"\
	

"$(INTDIR)\plplot.res" : $(SOURCE) $(DEP_RSC_PLPLO) "$(INTDIR)"
   $(RSC) /l 0x409 /fo"$(INTDIR)/plplot.res" /i "\plplot\tmp" /d "NDEBUG"\
 $(SOURCE)


!ELSEIF  "$(CFG)" == "x12c - Win32 Debug"

DEP_RSC_PLPLO=\
	".\..\..\..\..\tmp\plplot.ico"\
	

"$(INTDIR)\plplot.res" : $(SOURCE) $(DEP_RSC_PLPLO) "$(INTDIR)"
   $(RSC) /l 0x409 /fo"$(INTDIR)/plplot.res" /i "\plplot\tmp" /d "_DEBUG"\
 $(SOURCE)


!ENDIF 

# End Source File
# End Target
################################################################################
# Begin Target

# Name "x13c - Win32 Release"
# Name "x13c - Win32 Debug"

!IF  "$(CFG)" == "x13c - Win32 Release"

!ELSEIF  "$(CFG)" == "x13c - Win32 Debug"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=\plplot\tmp\x13c.cpp
DEP_CPP_X13C_=\
	".\..\..\..\..\tmp\plcdemos.h"\
	".\..\..\..\..\tmp\plplot.h"\
	

"$(INTDIR)\x13c.obj" : $(SOURCE) $(DEP_CPP_X13C_) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=\plplot\lib\plplot.lib

!IF  "$(CFG)" == "x13c - Win32 Release"

!ELSEIF  "$(CFG)" == "x13c - Win32 Debug"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\plplot\tmp\plplot.rc

!IF  "$(CFG)" == "x13c - Win32 Release"

DEP_RSC_PLPLO=\
	".\..\..\..\..\tmp\plplot.ico"\
	

"$(INTDIR)\plplot.res" : $(SOURCE) $(DEP_RSC_PLPLO) "$(INTDIR)"
   $(RSC) /l 0x409 /fo"$(INTDIR)/plplot.res" /i "\plplot\tmp" /d "NDEBUG"\
 $(SOURCE)


!ELSEIF  "$(CFG)" == "x13c - Win32 Debug"

DEP_RSC_PLPLO=\
	".\..\..\..\..\tmp\plplot.ico"\
	

"$(INTDIR)\plplot.res" : $(SOURCE) $(DEP_RSC_PLPLO) "$(INTDIR)"
   $(RSC) /l 0x409 /fo"$(INTDIR)/plplot.res" /i "\plplot\tmp" /d "_DEBUG"\
 $(SOURCE)


!ENDIF 

# End Source File
# End Target
################################################################################
# Begin Target

# Name "x15c - Win32 Release"
# Name "x15c - Win32 Debug"

!IF  "$(CFG)" == "x15c - Win32 Release"

!ELSEIF  "$(CFG)" == "x15c - Win32 Debug"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=\plplot\tmp\x15c.cpp
DEP_CPP_X15C_=\
	".\..\..\..\..\tmp\plcdemos.h"\
	".\..\..\..\..\tmp\plplot.h"\
	

"$(INTDIR)\x15c.obj" : $(SOURCE) $(DEP_CPP_X15C_) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=\plplot\lib\plplot.lib

!IF  "$(CFG)" == "x15c - Win32 Release"

!ELSEIF  "$(CFG)" == "x15c - Win32 Debug"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\plplot\tmp\plplot.rc

!IF  "$(CFG)" == "x15c - Win32 Release"

DEP_RSC_PLPLO=\
	".\..\..\..\..\tmp\plplot.ico"\
	

"$(INTDIR)\plplot.res" : $(SOURCE) $(DEP_RSC_PLPLO) "$(INTDIR)"
   $(RSC) /l 0x409 /fo"$(INTDIR)/plplot.res" /i "\plplot\tmp" /d "NDEBUG"\
 $(SOURCE)


!ELSEIF  "$(CFG)" == "x15c - Win32 Debug"

DEP_RSC_PLPLO=\
	".\..\..\..\..\tmp\plplot.ico"\
	

"$(INTDIR)\plplot.res" : $(SOURCE) $(DEP_RSC_PLPLO) "$(INTDIR)"
   $(RSC) /l 0x409 /fo"$(INTDIR)/plplot.res" /i "\plplot\tmp" /d "_DEBUG"\
 $(SOURCE)


!ENDIF 

# End Source File
# End Target
################################################################################
# Begin Target

# Name "x16c - Win32 Release"
# Name "x16c - Win32 Debug"

!IF  "$(CFG)" == "x16c - Win32 Release"

!ELSEIF  "$(CFG)" == "x16c - Win32 Debug"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=\plplot\tmp\x16c.cpp
DEP_CPP_X16C_=\
	".\..\..\..\..\tmp\plcdemos.h"\
	".\..\..\..\..\tmp\plplot.h"\
	

"$(INTDIR)\x16c.obj" : $(SOURCE) $(DEP_CPP_X16C_) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=\plplot\lib\plplot.lib

!IF  "$(CFG)" == "x16c - Win32 Release"

!ELSEIF  "$(CFG)" == "x16c - Win32 Debug"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\plplot\tmp\plplot.rc

!IF  "$(CFG)" == "x16c - Win32 Release"

DEP_RSC_PLPLO=\
	".\..\..\..\..\tmp\plplot.ico"\
	

"$(INTDIR)\plplot.res" : $(SOURCE) $(DEP_RSC_PLPLO) "$(INTDIR)"
   $(RSC) /l 0x409 /fo"$(INTDIR)/plplot.res" /i "\plplot\tmp" /d "NDEBUG"\
 $(SOURCE)


!ELSEIF  "$(CFG)" == "x16c - Win32 Debug"

DEP_RSC_PLPLO=\
	".\..\..\..\..\tmp\plplot.ico"\
	

"$(INTDIR)\plplot.res" : $(SOURCE) $(DEP_RSC_PLPLO) "$(INTDIR)"
   $(RSC) /l 0x409 /fo"$(INTDIR)/plplot.res" /i "\plplot\tmp" /d "_DEBUG"\
 $(SOURCE)


!ENDIF 

# End Source File
# End Target
################################################################################
# Begin Target

# Name "x18c - Win32 Release"
# Name "x18c - Win32 Debug"

!IF  "$(CFG)" == "x18c - Win32 Release"

!ELSEIF  "$(CFG)" == "x18c - Win32 Debug"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=\plplot\tmp\x18c.cpp
DEP_CPP_X18C_=\
	".\..\..\..\..\tmp\plcdemos.h"\
	".\..\..\..\..\tmp\plplot.h"\
	

"$(INTDIR)\x18c.obj" : $(SOURCE) $(DEP_CPP_X18C_) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=\plplot\lib\plplot.lib

!IF  "$(CFG)" == "x18c - Win32 Release"

!ELSEIF  "$(CFG)" == "x18c - Win32 Debug"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\plplot\tmp\plplot.rc

!IF  "$(CFG)" == "x18c - Win32 Release"

DEP_RSC_PLPLO=\
	".\..\..\..\..\tmp\plplot.ico"\
	

"$(INTDIR)\plplot.res" : $(SOURCE) $(DEP_RSC_PLPLO) "$(INTDIR)"
   $(RSC) /l 0x409 /fo"$(INTDIR)/plplot.res" /i "\plplot\tmp" /d "NDEBUG"\
 $(SOURCE)


!ELSEIF  "$(CFG)" == "x18c - Win32 Debug"

DEP_RSC_PLPLO=\
	".\..\..\..\..\tmp\plplot.ico"\
	

"$(INTDIR)\plplot.res" : $(SOURCE) $(DEP_RSC_PLPLO) "$(INTDIR)"
   $(RSC) /l 0x409 /fo"$(INTDIR)/plplot.res" /i "\plplot\tmp" /d "_DEBUG"\
 $(SOURCE)


!ENDIF 

# End Source File
# End Target
################################################################################
# Begin Target

# Name "x19c - Win32 Release"
# Name "x19c - Win32 Debug"

!IF  "$(CFG)" == "x19c - Win32 Release"

!ELSEIF  "$(CFG)" == "x19c - Win32 Debug"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=\plplot\tmp\x19c.cpp
DEP_CPP_X19C_=\
	".\..\..\..\..\tmp\plcdemos.h"\
	".\..\..\..\..\tmp\plplot.h"\
	

"$(INTDIR)\x19c.obj" : $(SOURCE) $(DEP_CPP_X19C_) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=\plplot\lib\plplot.lib

!IF  "$(CFG)" == "x19c - Win32 Release"

!ELSEIF  "$(CFG)" == "x19c - Win32 Debug"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\plplot\tmp\plplot.rc

!IF  "$(CFG)" == "x19c - Win32 Release"

DEP_RSC_PLPLO=\
	".\..\..\..\..\tmp\plplot.ico"\
	

"$(INTDIR)\plplot.res" : $(SOURCE) $(DEP_RSC_PLPLO) "$(INTDIR)"
   $(RSC) /l 0x409 /fo"$(INTDIR)/plplot.res" /i "\plplot\tmp" /d "NDEBUG"\
 $(SOURCE)


!ELSEIF  "$(CFG)" == "x19c - Win32 Debug"

DEP_RSC_PLPLO=\
	".\..\..\..\..\tmp\plplot.ico"\
	

"$(INTDIR)\plplot.res" : $(SOURCE) $(DEP_RSC_PLPLO) "$(INTDIR)"
   $(RSC) /l 0x409 /fo"$(INTDIR)/plplot.res" /i "\plplot\tmp" /d "_DEBUG"\
 $(SOURCE)


!ENDIF 

# End Source File
# End Target
# End Project
################################################################################
