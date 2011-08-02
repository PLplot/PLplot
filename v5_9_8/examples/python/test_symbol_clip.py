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
plssym(0., 10.)
plenv(0,1,0,1,0,0)
plpoin([1.0],[0.5],0)
plend()
