#!/usr/bin/env python

#	Histogram demo.

import sys
import os

module_dir = "@MODULE_DIR@"

if module_dir[0] == '@':
	module_dir = os.getcwd ()

sys.path.insert (0, module_dir)

from Numeric import *
from pl import *

NPTS = 2047

# main
#
# Draws a histogram from sample data.

def main():

    # Parse and process command line arguments

    plParseOpts(sys.argv, PARSE_FULL)

    # Initialize plplot

    plinit()

    # Fill up data points

    delta = 2.0 * pi / NPTS
    data = sin(delta*arrayrange(NPTS))

    plcol0(1)
    plhist(data, -1.1, 1.1, 44, 0)
    plcol0(2)
    pllab("#frValue", "#frFrequency",
	   "#frPLplot Example 5 - Probability function of Oscillator")

    pleop()
    plend()

main()
