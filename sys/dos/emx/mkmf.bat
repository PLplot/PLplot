@echo off
:# -----------------------------------------------------------------------
:# Example Makefile build script for DOS.
:# -----------------------------------------------------------------------

echo creating tmp\Makefile

set enable_f77="no"
set plcf=..\..\..\cf

:# Makefile initialization

type cf\init.in		>tmp\Makefile

:# Default target, core source and object file lists

type %plcf%\dist.in	>>tmp\Makefile 

:# Copy source file lists and add compiler specific source to makefile

type cf\emx.in	>>tmp\Makefile

:# Optional packages

if %enable_f77% == "yes" type %plcf%\pkg_f77.in	>>tmp\Makefile

:# Library targets

type cf\initlib.in	>>tmp\Makefile
type cf\lib.in		>>tmp\Makefile

:# Program and demo file dependencies, targets

type %plcf%\exes.in	>>tmp\Makefile 
type %plcf%\demos.in	>>tmp\Makefile 

:# Installation and miscellaneous.

type cf\install.in	>>tmp\Makefile 
type cf\Misc.in		>>tmp\Makefile 

:#  Object file dependencies

type %plcf%\objs.in	>>tmp\Makefile 

:# make list of objects for the response file

echo creating tmp\libfiles.tmp

type cf\all_o.rsp >tmp\libfiles.tmp
if %enable_f77% == "yes" type cf\fstubs_o.rsp >>tmp\libfiles.tmp

:# -----------------------------------------------------------------------
:# Now build Makedemo.in.
:# Makedemo is a stand-alone makefile for the demo programs.
:# Note: it links against the installed PLplot library.
:# -----------------------------------------------------------------------

echo creating tmp\Makedemo

type cf\init.in		>tmp\Makedemo
type cf\initdemo.in	>>tmp\Makedemo

type %plcf%\demos.in	>>tmp\Makedemo
type cf\Miscdemo.in	>>tmp\Makedemo

:# unset local variables
set enable_f77=
set plcf=
