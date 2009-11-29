#!/usr/bin/env python
# Simple test of plgradient for a complicated polygon.

# Append to effective python path so that can find plplot modules.
from plplot_python_start import *

import sys
from plplot import *
from numpy import *
from math import *

# Parse and process command line arguments
plparseopts(sys.argv, PL_PARSE_FULL)

xmin,xmax,ymin,ymax = (0., 1., 0., 1.)
x = (0., 0.2, 1., 0.3)
y = (0., 1., 0.2, 0.6)
#x = (0., 0., 1., 1.)
#y = (0., 1., 1., 0.)
# Initialize plplot
plinit()
for i in range(1):
    plcol0(2)
    pladv(0)
    plvpor(0.1, 0.9, 0.1, 0.9)
    plwind(xmin, xmax, ymin, ymax)
# Just to show edges of viewport
    plbox("bc", 0., 0, "bc", 0., 0)
    plcol0(1)
    plgradient(x,y, 45. + i)
plend()
