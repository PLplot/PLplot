# Copyright 2002 Gary Bishop
# This file is part of PLplot.

# This file is free software; you can redistribute it and/or modify
# it under the terms of the GNU Library General Public License as published by
# the Free Software Foundation; version 2 of the License.

# This file is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Library General Public License for more details.

# You should have received a copy of the GNU Library General Public License
# along with the file; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307  USA

# Wrap raw python interface to C API, plplotc, with this user-friendly version
# which implements some useful variations of the argument lists.

from plplotc import *
import types
import Numeric

#  Allowable syntaxes:

#  plcont( z, kx, lx, ky, ly, clev, pltr, xg, yg, wrap )
#  plcont( z, clev, pltr, xg, yg, wrap )

#  where in both cases, parameters from pltr to the end are optional.

#  First case, arg count = 6 +3? +1?
#  Second case, arg count = 2 +3? +1?
#
#  The only way I see to disambiguate this is to either be able to
#  nondestructively hand parse the arg list, or to mandate fewer valid forms.
#  Since I cannot find info on how to do a nondestructive trial parse, we opt
#  for the second.  Thus, by fiat, I declare the following forms to be
#  admissible:
#
#  plcont( z, kx, lx, ky, ly, clev, [pltr, xg, yg, wrap] )
#  plcont( z, clev, pltr, xg, yg, wrap )

# redefine plcont to have the funky interface
_plcont = plcont
def plcont(z, *args):
  z = Numeric.asarray(z)
  
  if len(args) > 4 and type(args[0]) == types.IntType:
    for i in range(1,4):
      if type(args[i]) != types.IntType:
        raise ValueError, 'Expected 4 ints for kx,lx,ky,ly'
    else:
      # these 4 args are the kx, lx, ky, ly ints
      default = 0
      kx,lx,ky,ly = args[0:4]
      args = args[4:]
  else:
    default = 1

  if len(args) > 0:
    clev = args[0]
    args = args[1:]
  else:
    raise ValueError, 'missing clev argument'

  # default values for these
  pltr = pltr0
  xg = None
  yg = None
  wrap = None

  try:
    pltr = args[0]
    xg = Numeric.asarray(args[1])
    yg = Numeric.asarray(args[2])
    wrap = args[3]
  except IndexError:
    pass

  # handle wrap
  if wrap == 1:
    z = Numeric.resize(z, (z.shape[0]+1, z.shape[1]))
    if xg:
      xg = Numeric.resize(xg, (xg.shape[0]+1, xg.shape[1]))
    if yg:
      yg = Numeric.resize(yg, (yg.shape[0]+1, yg.shape[1]))
  elif wrap == 2:
    z = Numeric.transpose(Numeric.resize(Numeric.transpose(z), (z.shape[1]+1, z.shape[0])))
    if xg:
      xg =  Numeric.transpose(Numeric.resize(Numeric.transpose(xg), (xg.shape[1]+1, xg.shape[0])))
    if yg:
      yg =  Numeric.transpose(Numeric.resize(Numeric.transpose(yg), (yg.shape[1]+1, yg.shape[0])))
  elif wrap != 0:
    raise ValueError, "Invalid wrap specifier, must be 0, 1 or 2."

  # handle the string names for the callbacks though the function works fine too
  if type(pltr) == types.StringType:
    if pltr == 'pltr0':
      pltr = pltr0
    elif pltr == 'pltr1':
      pltr = pltr1
    elif pltr == 'pltr2':
      pltr = pltr2
    else:
      raise ValueError, 'pltr is unrecognized'

  if default:
    kx = 1
    lx = z.shape[0]
    ky = 1
    ly = z.shape[1]

  _plcont(z, kx, lx, ky, ly, clev, pltr, (xg, yg))
plcont.__doc__ = _plcont.__doc__
  
_plstyl = plstyl
def plstyl(*args):
  if len(args) == 3:
    n,m,s = args
  else:
    m,s = args
    n = 1

  if n == 0:
    m = []
    s = []
  if type(m) == types.IntType:
    m = [m]
  if type(s) == types.IntType:
    s = [s]

  _plstyl(m,s)
plstyl.__doc__ = _plstyl.__doc__

_plshades = plshades
def plshades(z, xmin, xmax, ymin, ymax, clevel, fill_width, cont_color, cont_width, rect,
             *args):
  pltr = pltr0
  xg = None
  yg = None
  if len(args) >= 3:
    pltr, xg, yg = args[0:3]
    args = args[3:]

  wrap = 0
  if len(args) == 1:
    wrap = args[0]
  elif len(args) != 0:
    raise 'ValueError', 'too many arguments'

  if wrap == 1:
    z = Numeric.resize(z, (z.shape[0]+1, z.shape[1]))
    if xg:
      xg = Numeric.resize(xg, (xg.shape[0]+1, xg.shape[1]))
    if yg:
      yg = Numeric.resize(yg, (yg.shape[0]+1, yg.shape[1]))
  elif wrap == 2:
    z = Numeric.transpose(Numeric.resize(Numeric.transpose(z), (z.shape[1]+1, z.shape[0])))
    if xg:
      xg =  Numeric.transpose(Numeric.resize(Numeric.transpose(xg), (xg.shape[1]+1, xg.shape[0])))
    if yg:
      yg =  Numeric.transpose(Numeric.resize(Numeric.transpose(yg), (yg.shape[1]+1, yg.shape[0])))
  elif wrap != 0:
    raise ValueError, "Invalid wrap specifier, must be 0, 1 or 2."

  # handle the string names for the callbacks though the function works fine too
  if type(pltr) == types.StringType:
    if pltr == 'pltr0':
      pltr = pltr0
    elif pltr == 'pltr1':
      pltr = pltr1
    elif pltr == 'pltr2':
      pltr = pltr2
    else:
      raise ValueError, 'pltr is unrecognized'

  pltr_data = None
  if xg and yg:
    pltr_data = (xg, yg)
  else:
    pltr_data = None

  _plshades(z,  xmin, xmax, ymin, ymax, clevel, fill_width, cont_color, cont_width,
            rect, pltr, pltr_data)
plshades.__doc__ = _plshades.__doc__
  
