#!/usr/bin/env python

#	Histogram demo.

import math
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

    plinit()

    # Fill up data points

    delta = 2.0 * math.pi / NPTS
    data = zeros(NPTS,'d')
    for i in range(NPTS):
	data[i] = math.sin(i * delta)

    plcol(1)
    plhist(data, -1.1, 1.1, 44, 0)
    plcol(2)
    pllab("#frValue", "#frFrequency",
	   "#frPLplot Example 5 - Probability function of Oscillator")

    pleop()
    plend()

main()
