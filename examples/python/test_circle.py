#!/usr/bin/env python

# Append to effective python path so that can find plplot modules.
from plplot_python_start import *

import sys
from plplot import *
from numpy import *

x = [0.5]
y = [0.5]
ifunicode = True

# Parse and process command line arguments
plparseopts(sys.argv, PL_PARSE_FULL)

# Initialize plplot
plinit()
plcol0(2)
for kind in range(4):
  pladv(0)
  plvpor(0.1, 0.9, 0.1, 0.9)
  plwind(0., 1., 0., 1.)
  # Just to show edges of viewport
  plbox("bc", 0., 0, "bc", 0., 0)
  k=0
  for size in 2.**arange(2,-3,-1):
    k+=1
    plcol0(k)
    if kind==0:
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
    elif kind ==1:
      # Print an asterisk using plptex
      plschr(0., 8.*size)
      plptex(0.5, 0.5, 1., 0., 0.5, "#(728)")
    elif kind ==2:
      # Print an asterisk using plsym
      plssym(0., 8.*size)
      plsym(x, y, 728)
    elif kind ==3:
      # Print an asterisk using plpoin
      plssym(0., 8.*size)
      plpoin(x, y, 3)
# Terminate plplot
plend()

