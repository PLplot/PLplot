#!/usr/bin/env python
#Have not yet converted to arrays.

#	Bar chart demo.

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
# Does a simple bar chart, using color fill.  If color fill is
# unavailable, pattern fill is used instead (automatic).

def main():

    # Parse and process command line arguments

    plParseOpts(sys.argv, PARSE_FULL)

    # Initialize plplot

    plinit()

    pladv(0)
    plvsta()
    plwind(1980.0, 1990.0, 0.0, 35.0)
    plbox("bc", 1.0, 0, "bcnv", 10.0, 0)
    plcol0(2)
    pllab("Year", "Widget Sales (millions)", "#frPLplot Example 12")

    y0 = [5, 15, 12, 24, 28, 30, 20, 8, 12, 3]

    for i in range(10):
	plcol0(i + 1)
	plpsty(0)
	fbox( (1980. + i), y0[i] )
	string = `y0[i]`
	plptex((1980. + i + .5), (y0[i] + 1.), 1.0, 0.0, .5, string)
	string = `1980 + i`
	plmtex("b", 1.0, ((i + 1) * .1 - .05), 0.5, string)

    pleop()
    plend()

def fbox( x0, y0 ):

    x = [x0, x0, x0 + 1., x0 + 1.]
    y = [0., y0, y0, 0.]
    plfill(x, y)
    plcol0(1)
    pllsty(1)
    plline(x, y)

main()
