#!/usr/bin/env python

# Run all python plplot examples non-interactively.

# Append to effective python path so that can find plplot modules.
from plplot_python_start import *

import sys
from plplot import *

# Parse and process command line arguments
plParseOpts(sys.argv, PL_PARSE_FULL)

# Initialize plplot
plinit()

import xw01
import xw02
import xw03
import xw04
import xw05
import xw06
import xw07
import xw08
import xw09
import xw10
import xw11
import xw12
import xw13
# xw14.py must be standalone so cannot be run from pythondemos.py
import xw15
import xw16
# xw17.py must be standalone and interactive so cannot be run from pythondemos.py
import xw18
import xw19

# Terminate plplot
plend()
