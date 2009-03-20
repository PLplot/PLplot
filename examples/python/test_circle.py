#!/usr/bin/env python

# Append to effective python path so that can find plplot modules.
from plplot_python_start import *

import sys
from plplot import *
from numpy import *

# Parse and process command line arguments
plparseopts(sys.argv, PL_PARSE_FULL)

# Initialize plplot
plinit()
plcol0(2)
pladv(0)
plvpor(0.1, 0.9, 0.1, 0.9)
plwind(0., 1., 0., 1.)
# Just to show edges of viewport
plbox("bc", 0., 0, "bc", 0., 0)
x = [0.5]
y = [0.5]
ifplptex = True
ifunicode = True

k=0
for size in 2.**arange(2,-3,-1):
  k+=1
  plcol0(k)
  if ifplptex:
    if ifunicode:
      plschr(0., 4.*size)
      # LARGE CIRCLE has a centre of symmetry which is about one-eighth the
      # radius below the centre of symmetry of the box for DejaVu Sans.
      plptex(0.5, 0.5, 1., 0., 0.5, "#[0x25ef]")
      # BOX DRAWINGS LIGHT DIAGONAL CROSS is one of the best centred symmetrical
      # glyphs I have found using gucharmap for DejaVu Sans.
      plptex(0.5, 0.5, 1., 0., 0.5, "#[0x2573]")
    else:
      plschr(0., 4.*size)
      plptex(0.5, 0.5, 1., 0., 0.5, "#(907)")
#  plcol0(3)
#  plssym(0., size)
#  plsym(x, y, 907)

# Terminate plplot
plend()

