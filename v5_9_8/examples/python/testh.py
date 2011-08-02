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
# Like yellow lines better.
plcol0(2)
pladv(0)
plvpor(0.1, 0.9, 0.1, 0.9)
plwind(0., 1., 0., 1.)
x=0.*arange(2)
y=1.*arange(2)
plline(x,y)
x=0.1*arange(2)
y=0.5+0.*arange(2)
plline(x,y)
plschr(0., 4.)
#plmtex("l", 0., 0.5, 0.5, "#[0x00d7]#[0x00d7]#[0x00d7]#[0x00d7]#[0x00d7]#[0x00d7]#[0x00d7]")
plmtex("l", 0., 0.5, 0.5, "HHHHHHH")

# Terminate plplot
plend()

