@echo off
REM -----------------------------------------------------------------------
REM Example Makefile build script for DOS.
REM -----------------------------------------------------------------------

@echo "creating tmp\Makefile"

set enable_f77="no"

set plcf=../../../cf

REM Makefile initialization

cat cf/init.in		>tmp\Makefile 

REM Default target, core source and object file lists

cat %plcf%/dist.in	>>tmp\Makefile 

REM Copy source file lists and add djgpp specific source to makefile

cat cf/djgpp.in	>>tmp\Makefile 

REM Optional packages

if %enable_f77% == "yes" cat %plcf%/pkg_f77.in	>>tmp\Makefile 

REM Library targets

cat cf/initlib.in	>>tmp\Makefile
cat cf/lib.in		>>tmp\Makefile

REM Program and demo file dependencies, targets

cat %plcf%/exes.in	>>tmp\Makefile 
cat %plcf%/demos.in	>>tmp\Makefile 

REM Installation and miscellaneous.

cat cf/install.in	>>tmp\Makefile 
cat cf/Misc.in		>>tmp\Makefile 

REM Object file dependencies

cat %plcf%/objs.in	>>tmp\Makefile 

REM -----------------------------------------------------------------------
REM Now build Makedemo.in.
REM Makedemo is a stand-alone makefile for the demo programs.
REM Note: it links against the installed PLplot library.
REM -----------------------------------------------------------------------

echo "creating tmp\Makedemo"

cat cf/init.in		>tmp\Makedemo
cat cf/initdemo.in	>>tmp\Makedemo

cat %plcf%/demos.in	>>tmp\Makedemo
cat %plcf%/Miscdemo.in	>>tmp\Makedemo

