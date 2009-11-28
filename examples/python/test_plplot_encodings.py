#!/usr/bin/env python
# -*- coding: utf-8; -*-
# Test whether utf8 and PLplot #[0x...] unicode encoding give same result.
# Also test whether #<FCI-COMMAND-STRING/> form works to change font
# characteristics on the fly.
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
plcol0(1)
plschr(0., 2.)
plptex(0.5, 0.5, 1., 0., 0.5, "#<sans-serif/>#[0x222e] is a path integral sign.")
# semitransparent green.
plscol0a(2, 0, 255, 0, 0.5)
plcol0(2)
plptex(0.5, 0.5, 1., 0., 0.5, "#<sans-serif/>∮ is a path integral sign.")
plend()
