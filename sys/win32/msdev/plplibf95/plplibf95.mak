# Microsoft Developer Studio Generated NMAKE File, Based on plplib.dsp
!IF "$(CFG)" == ""
CFG=plplib - Win32 Debug
!MESSAGE No configuration specified. Defaulting to plplib - Win32 Debug.
!ENDIF

!IF "$(CFG)" != "plplib - Win32 Release" && "$(CFG)" != "plplib - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE
!MESSAGE NMAKE /f "plplib.mak" CFG="plplib - Win32 Debug"
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

CPP=cl.exe
F90=ifort.exe
F90=df.exe
RSC=rc.exe

BINDDIR=..\..\..\..\bindings
MANGLING=/D STUB_LINKAGE=STUB_U

!IF  "$(CFG)" == "plplib - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
TMPDIR=..\..\..\..\tmp
LIBDIR=..\..\..\..\lib
BUILD_DIR="\"tmp\""

ALL : $(LIBDIR)\plplotf95.lib


CLEAN :
	-@erase "$(INTDIR)\plflt.obj"
	-@erase ".\plplotf95.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

F90_PROJ=/compile_only /include:"$(INTDIR)\\" /nologo /warn:nofileopt /module:"Release/" /object:"Release/"
F90_OBJS=.\Release/
CPP_PROJ=/nologo /Ox /MD /W3 /GX /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /D "BUILD_DIR=$(BUILD_DIR)" /I..\..\..\..\..\plplot-cvs-build\include /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\plplib.bsc"
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"plplotf95.lib"
LIB32_OBJS= \
	"$(INTDIR)\plflt.obj"

$(LIBDIR)\plplotf95.lib : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<
    copy .\plplotf95.lib $(LIBDIR)\plplotf95.lib

!ELSEIF  "$(CFG)" == "plplib - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
TMPDIR=..\..\..\..\tmp
LIBDIR=..\..\..\..\lib
BUILD_DIR="\"tmp\""
# End Custom Macros

ALL : "$(OUTDIR)\plplibf95d.lib"


CLEAN :
	-@erase "$(INTDIR)\plflt.obj"
	-@erase "$(INTDIR)\plflt.exe"
	-@erase "$(INTDIR)\plflt.inc"
	-@erase "$(INTDIR)\sc3d.obj"
	-@erase "$(INTDIR)\sccont.obj"
	-@erase "$(INTDIR)\scstubs.obj"
	-@erase "$(INTDIR)\sfstubsf95.obj"
	-@erase "$(OUTDIR)\plplibf95d.lib"
	-@erase "$(OUTDIR)\plplib.pch"
	-@erase "$(OUTDIR)\vc70.idb"
	-@erase "$(OUTDIR)\vc70.pdb"


"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

F90_PROJ=/check:bounds /compile_only /debug:full /include:"$(INTDIR)\\" /nologo /traceback /warn:argument_checking /warn:nofileopt /module:"Debug/" /object:"Debug/" /pdbfile:"Debug/DF60.PDB"
F90_OBJS=.\Debug/
CPP_PROJ=/nologo /MLd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /D "BUILD_DIR=$(BUILD_DIR)" /I$(TMPDIR) /Fp"$(INTDIR)\plplib.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ  /c
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\plplib.bsc"
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\plplibf95d.lib"
LIB32_OBJS= \
	"$(INTDIR)\strutil.obj" \
	"$(INTDIR)\sc3d.obj" \
	"$(INTDIR)\sccont.obj" \
	"$(INTDIR)\scstubs.obj" \
	"$(INTDIR)\sfstubsf95.obj"

"$(OUTDIR)\plplibf95d.lib" : "$(OUTDIR)" $(DEF_FILE) "$(INTDIR)/plflt.inc" $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ENDIF

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $<
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $<
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $<
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $<
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $<
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $<
<<

.SUFFIXES: .fpp

.for{$(F90_OBJS)}.obj:
   $(F90) $(F90_PROJ) $<

.f{$(F90_OBJS)}.obj:
   $(F90) $(F90_PROJ) $<

.f90{$(F90_OBJS)}.obj:
   $(F90) $(F90_PROJ) $<

.fpp{$(F90_OBJS)}.obj:
   $(F90) $(F90_PROJ) $<


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("plplib.dep")
!INCLUDE "plplib.dep"
!ELSE
!MESSAGE Warning: cannot find "plplib.dep"
!ENDIF
!ENDIF


!IF "$(CFG)" == "plplib - Win32 Release" || "$(CFG)" == "plplib - Win32 Debug"

SOURCE=$(BINDDIR)\f95\plflt.c

"$(INTDIR)\plflt.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)

"$(INTDIR)\plflt.exe" : "$(INTDIR)\plflt.obj"
	link "$(INTDIR)\plflt.obj" /out:"$(INTDIR)\plflt.exe"

"$(INTDIR)\plflt.inc" : "$(INTDIR)\plflt.exe"
	cd "$(INTDIR)"
	plflt

SOURCE=$(BINDDIR)\f95\strutil.f95

"$(INTDIR)\strutil.obj" : $(SOURCE) "$(INTDIR)"
	$(F90) $(F90_PROJ) $(SOURCE)

SOURCE=$(BINDDIR)\f95\sc3d.c

"$(INTDIR)\sc3d.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(MANGLING) $(SOURCE)

SOURCE=$(BINDDIR)\f95\sccont.c

"$(INTDIR)\sccont.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(MANGLING) $(SOURCE)

SOURCE=$(BINDDIR)\f95\scstubs.c

"$(INTDIR)\scstubs.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(MANGLING) $(SOURCE)

SOURCE=$(BINDDIR)\f95\sfstubsf95.f95

"$(INTDIR)\sfstubsf95.obj" : $(SOURCE) "$(INTDIR)"
	$(F90) $(F90_PROJ) $(SOURCE)


!ENDIF

