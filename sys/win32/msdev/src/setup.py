#!/usr/bin/env python

from distutils.core import setup, Extension

include_dirs       = ['.', '..\\tmp','c:\\Python21\\Include\\Numeric']
extra_compile_args = ['/DWIN32']
library_dirs       = ['..\\lib']
libraries          = ['plplot', 'kernel32', 'user32', 'gdi32', 'winspool', 'comdlg32', 'advapi32', 'shell32', 'ole32', 'oleaut32', 'uuid', 'odbc32', 'odbccp32']


module1 = Extension( "plplotcmodule", 
		     extra_compile_args = extra_compile_args,
                     include_dirs = include_dirs,
		     library_dirs = library_dirs,
		     libraries = libraries,
		     sources = ["plplotcmodule.c"]
		     )

module2 = Extension( "plplot_widgetmodule", 
		     extra_compile_args = extra_compile_args,
                     include_dirs = include_dirs,
		     library_dirs = library_dirs,
		     libraries = libraries,
		     sources = ["plplot_widgetmodule.c"]
		     )

setup( name = "plplot",
       version = "5.1",
       description = "PLplot Python extension modules",
       author = "Alan W. Irwin",
       author_email = "irwin@beluga.phys.uvic.ca",
       url = "http://www.plplot.org",
       license = "LGPL",
       ext_modules = [module1,module2]
       )
