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
# Choose 5 here so there is room for non-exponential notation for an axis
# label of +0.08.
plsyax(5, 0)
pladv(0)
plvpor(0.1, 0.9, 0.1, 0.9)
plwind(0.00, 1.00, 0.00, 0.08)
plbox("bcnst", 0.0, 0, "bcnstv", 0.0, 0);
plend()
