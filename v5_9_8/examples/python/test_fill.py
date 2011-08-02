#!/usr/bin/env python
# Simple test of plfill for the case where the polygon is just outside the
# window.

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
xmin,xmax,ymin,ymax = (0., 1., 0., 1.)
plwind(xmin, xmax, ymin, ymax)
# Just to show edges of viewport
plbox("bc", 0., 0, "bc", 0., 0)
plcol0(1)
eps = 0.001
xmin,xmax,ymin,ymax = (xmin - eps, xmax + eps, ymin - eps, ymax + eps)
x = (xmin, xmax, xmax, xmin)
y = (ymin, ymin, ymax, ymax)
plfill(x,y)
plend()
