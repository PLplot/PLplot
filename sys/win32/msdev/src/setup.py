#!/usr/bin/env python

from distutils.core import setup

setup( name = "pl",
       version = "5.1.0",
       description = "PLplot Python extension module",
       author = "Geoffrey Furnish",
       author_email = "furnish@actel.com",
       url = "http://www.plplot.org",

       extra_path = 'PLplot',
       #py_modules = ["Plframe", "TclSup" ],


       ext_modules = [( 'pl',
                        {'sources' : ['plmodule.c',
                                      ],
                         'include_dirs' : ['.', 
'..\\tmp','c:\\Python21\\Include\\Numeric'],
                         'library_dirs' : ['..\\lib'],
                         'libraries' : ['plplot', 'kernel32', 'user32',
'gdi32', 'winspool', 'comdlg32', 'advapi32', 'shell32', 'ole32',
'oleaut32', 'uuid', 'odbc32', 'odbccp32'],
                         }
                       ),
                      ( 'pyqt_pl',
                        {'sources' : ['pyqt_plmodule.c',
                                     ],
                         'include_dirs' : ['.',
'..\\tmp','c:\\Python21\\Include\\Numeric'],
                         'library_dirs' : ['..\\lib'],
                         'libraries' : ['plplot', 'kernel32', 'user32',
'gdi32', 'winspool', 'comdlg32', 'advapi32', 'shell32', 'ole32',
'oleaut32', 'uuid', 'odbc32', 'odbccp32'],
                         }
                       )                                  
                      
                      ],
       )

