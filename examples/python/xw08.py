#!/usr/bin/env python

#	3-d plot demo.

import math
import sys
import os

module_dir = "@MODULE_DIR@"

if module_dir[0] == '@':
	module_dir = os.getcwd ()

sys.path.insert (0, module_dir)

from Numeric import *
from pl import *

XPTS = 35		# Data points in x
YPTS = 46		# Data points in y

opt = [1, 2, 3, 3]

alt = [60.0, 20.0, 60.0, 60.0]

az = [30.0, 60.0, 120.0, 160.0]

title = ["#frPLplot Example 8 - Alt=60, Az=30, Opt=1",
	 "#frPLplot Example 8 - Alt=20, Az=60, Opt=2",
	 "#frPLplot Example 8 - Alt=60, Az=120, Opt=3",
	 "#frPLplot Example 8 - Alt=60, Az=160, Opt=3"]

# main
#
# Does a series of 3-d plots for a given data set, with different
# viewing options in each plot.

def main():

    plinit()

    x = zeros( XPTS, 'd' ); y = zeros( YPTS, 'd' )
    z = reshape( zeros( XPTS*YPTS, 'd' ), (XPTS, YPTS) )

    for i in range(XPTS):
	x[i] = float(i - (XPTS / 2)) / float(XPTS / 2)

    for i in range(YPTS):
	y[i] = float(i - (YPTS / 2)) / float(YPTS / 2)

    for i in range(XPTS):
	xx = x[i]
	for j in range(YPTS):
	    yy = y[j]
	    r = math.sqrt(xx * xx + yy * yy)
	    z[i,j] = math.exp(-r * r) * math.cos(2.0 * math.pi * r)

    for k in range(4):
	pladv(0)
	plvpor(0.0, 1.0, 0.0, 0.9)
	plwind(-1.0, 1.0, -0.9, 1.1)
	plcol(1)
	plw3d(1.0, 1.0, 1.0, -1.0, 1.0, -1.0, 1.0, -1.0, 1.0,
		alt[k], az[k])
	plbox3("bnstu", "x axis", 0.0, 0,
		 "bnstu", "y axis", 0.0, 0,
		 "bcdmnstuv", "z axis", 0.0, 0)

	plcol(2)
	plot3d(x, y, z, opt[k], 1)
	plcol(3)
	plmtex("t", 1.0, 0.5, 0.5, title[k])

	pleop()
	
    plend()

main()
