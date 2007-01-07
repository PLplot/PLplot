@rem **************************************************************
@rem * This batch file create binary packages for plplot (windows)
@rem *
@rem * Before you run this file modify the configuration part
@rem *
@rem * It is assumed, that before running this batch file, that
@rem * you have already passed the cmake configuration step,
@rem * and have run "make" and "make install" or equivalent
@rem **************************************************************

@rem **************************************************************
@rem * Configuration
@rem **************************************************************

setlocal
rem set this to 1 if you have mingw
set MINGW=0

set VERSION=5.7.1

if %MINGW%==1 (
  @rem *** MINGW SETTINGS ***
  @rem * INSTALLDIR is the absolute path to the installed plplot directory (make install)
  set INSTALLDIR=Z:\DevZone\plplot\buildmingw\local

  @rem * GDDLLDIR is the absolute path to bgd.dll
  set GDDLLDIR=Z:\DevZone\3p_plplot_mingw\lib

  @rem * HPDFDLLDIR is the absolute path to libhpdf.dll
  set HPDFDLLDIR=Z:\DevZone\3p_plplot_mingw\lib

  @rem * WXDLLDIR is the absolute path to wxwidgets dlls
  set WXDLLDIR=%WXWIN%\lib\gcc_dll

  @rem * Name of the package
  set PACKAGE_NAME=plplot-%VERSION%-mingw-binary
) else (
  @rem *** VISUALC SETTINGS ***
  @rem * INSTALLDIR is the absolute path to the installed plplot directory (make install)
  set INSTALLDIR=Z:\DevZone\plplot\buildnmake\local

  @rem * GDDLLDIR is the absolute path to bgd.dll
  set GDDLLDIR=Z:\DevZone\3p_plplot_vc\lib

  @rem * HPDFDLLDIR is the absolute path to libhpdf.dll
  set HPDFDLLDIR=Z:\DevZone\3p_plplot_vc\lib

  @rem * WXDLLDIR is the absolute path to wxwidgets dlls
  set WXDLLDIR=%WXWIN%\lib\vc_dll

  @rem * Name of the package
  set PACKAGE_NAME=plplot-%VERSION%-visualc-binary
)

@rem **************************************************************
@rem * Create Zip package
@rem * You need the zip tool for this step (gnuwin32.sf.net)
@rem **************************************************************

echo Creating package %PACKAGE_NAME%.zip

@rem * delete package if it already exists
if exist %PACKAGE_NAME%.zip (del %PACKAGE_NAME%.zip)

@rem * cd into the install directory
pushd %INSTALLDIR%

@rem * copy 3rd party dlls if they exist
if exist %GDDLLDIR%\bgd.dll (copy %GDDLLDIR%\bgd.dll lib)
if exist %HPDFDLLDIR%\libhpdf.dll (copy %HPDFDLLDIR%\libhpdf.dll lib)
if %MINGW%==1 (
  if exist %WXDLLDIR%\wxbase26_gcc_custom.dll (copy %WXDLLDIR%\wxbase26_gcc_custom.dll lib)
  if exist %WXDLLDIR%\wxmsw26_core_gcc_custom.dll (copy %WXDLLDIR%\wxmsw26_core_gcc_custom.dll lib)
) else (
  if exist %WXDLLDIR%\wxbase26u_vc_custom.dll (copy %WXDLLDIR%\wxbase26u_vc_custom.dll lib)
  if exist %WXDLLDIR%\wxmsw26u_core_vc_custom.dll (copy %WXDLLDIR%\wxmsw26u_core_vc_custom.dll lib)
)
@rem strip debug information from libraries
if %MINGW%==1 (strip lib\*.dll)

@rem * create base package
zip -r9 %PACKAGE_NAME%.zip *.*

@rem * get back to old directory
popd

@rem * move package into current folder
if exist %INSTALLDIR%\%PACKAGE_NAME%.zip (move %INSTALLDIR%\%PACKAGE_NAME%.zip %PACKAGE_NAME%.zip)

goto end

:error
echo There was an error!
goto :eof

:end
echo.
echo The package %PACKAGE_NAME%.zip was created!
echo.
dir %PACKAGE_NAME%.zip

