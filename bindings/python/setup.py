#!/usr/bin/env python

from distutils.core import setup

setup( name = "pl",
       version = "5.0",
       description = "PLplot Python extension module",
       author = "Geoffrey Furnish",
       author_email = "furnish@actel.com",
       url = "http://www.plplot.org",

       extra_path = 'PLplot',
       py_modules = ["Plframe", "TclSup" ],


       ext_modules = [( 'pl',
                        {'sources' : ['plmodule.c',
                                      'plmodule2.c']
                         })
                      ],
       )
