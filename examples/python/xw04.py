#!/usr/bin/env python

#	Log plot demo.

import math
import sys
import os

module_dir = "@MODULE_DIR@"

if module_dir[0] == '@':
	module_dir = os.getcwd ()

sys.path.insert (0, module_dir)

from Numeric import *
from pl import *

# main
# 
# Illustration of logarithmic axes, and redefinition of window.

def main():

    # Parse and process command line arguments

    plParseOpts(sys.argv, PARSE_FULL)

    # Initialize plplot

    plinit()

    pladv(0)
    plfont(2)

    f0 = 1000.0
    freql = zeros(101,'d'); ampl = zeros(101,'d'); phase=zeros(101,'d')
    for i in range(101):
	freql[i] = 1.0 + i / 20.0
	freq = pow( 10.0, freql[i] )
	ampl[i] = 20.0 * math.log10(1.0 / math.sqrt(1.0 + pow((freq / f0), 2.)))
	phase[i] = -(180.0 / math.pi) * math.atan(freq / f0)

    plvpor(0.15, 0.85, 0.1, 0.9)
    plwind(1.0, 6.0, -80.0, 0.0)
    plcol(1)
    plbox("bclnst", 0.0, 0, "bnstv", 0.0, 0)
    plcol(2)
    plline(freql, ampl)
    plcol(1)
    plptex(5.0, -30.0, 1.0, -20.0, 0.5, "-20 dB/decade")

    plwind(1.0, 6.0, -100.0, 0.0)
    plbox("", 0.0, 0, "cmstv", 30.0, 3)
    plcol(3)
    plline(freql, phase)

    plcol(1)
    plmtex("b", 3.2, 0.5, 0.5, "Frequency")
    plmtex("t", 2.0, 0.5, 0.5, "Single Pole Low-Pass Filter")
    plcol(2)
    plmtex("l", 5.0, 0.5, 0.5, "Amplitude (dB)")
    plcol(3)
    plmtex("r", 5.0, 0.5, 0.5, "Phase shift (degrees)")

    pleop()

main()
