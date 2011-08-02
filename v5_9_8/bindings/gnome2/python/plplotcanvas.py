"""plplotcanvas.py - Python front-end for the plplotcanvas

  Copyright (C) 2004, 2005 Thomas J. Duck
  All rights reserved.

  Thomas J. Duck <tom.duck@dal.ca>
  Department of Physics and Atmospheric Science,
  Dalhousie University, Halifax, Nova Scotia, Canada, B3H 3J5


NOTICE

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301
  USA 
"""


import plplot
import cplplotcanvas
import exceptions

class Canvas(cplplotcanvas.Canvas):

    def __init__(self):
        cplplotcanvas.Canvas.__init__(self)
        self.devinit()

    def __getattr__(self,name):

        # Select the stream
        Nstream = self.get_stream_number()
        plplot.plsstrm(Nstream)

        # Try to get the function using all of the available prefixes
        if 'pl'+name in dir(plplot):
            return eval('plplot.' + 'pl' + name)
        elif '' + name in dir(plplot):
            return eval('plplot.' + '' + name)
        elif 'pl_' + name in dir(plplot):
            return eval('plplot.' + 'pl_' + name)
        else:
            msg = "'Canvas' object has no attribute '%s'" % (name)
            raise exceptions.AttributeError, msg
