@echo off
rem -------------------------------------------
rem NOTE:
rem This file must be in DOS-format!
rem Otherwise it will not work under Windows 95
rem -------------------------------------------
rem
rem Configuration of PLplot:
rem - Create the jim interpreter
rem - Run the configuration dialogue
rem - Implement the choices
rem
cd jim
if exist Release\jim.exe goto next
echo Making the jim interpreter ....
echo ...
nmake -f Makefile.vc all
:next
cd ..

rem
rem Run the configuration dialogue
rem
if exist config2.bat del config2.bat
jim\Release\jim.exe configure.tcl
