#!/usr/bin/python

#	Mesh plot demo.

import math
import sys
from Numeric import *
from pl import *

XPTS = 35		# Data points in x
YPTS = 46		# Data points in y

opt = [1, 2, 3, 3]

alt = [60.0, 20.0, 60.0, 60.0]

az = [30.0, 60.0, 120.0, 160.0]

title = ["#frPLplot Example 11 - Alt=60, Az=30, Opt=1",
	 "#frPLplot Example 11 - Alt=20, Az=60, Opt=2",
	 "#frPLplot Example 11 - Alt=60, Az=120, Opt=3",
	 "#frPLplot Example 11 - Alt=60, Az=160, Opt=3"]

# main
#
# Does a series of mesh plots for a given data set, with different
# viewing options in each plot.

def main():

    plinit()

    x = zeros(XPTS,'d'); y = zeros(YPTS,'d')
    z = reshape( zeros( XPTS*YPTS, 'd' ), (XPTS, YPTS) )

    for i in range(XPTS):
	x[i] = float(i - (XPTS / 2)) / float(XPTS / 2)

    for i in range(YPTS):
	y[i] = float(i - (YPTS / 2)) / float(YPTS / 2)

    for i in range(XPTS):
	xx = x[i]
	zz = []
	for j in range(YPTS):
	    yy = y[j]
	    z[i,j] = math.cos(2.0 * math.pi * xx) * \
		     math.sin(2.0 * math.pi * yy)

    for k in range(4):
	pladv(0)
	plcol(1)
	plvpor(0.0, 1.0, 0.0, 0.8)
	plwind(-1.0, 1.0, -1.0, 1.5)

	plw3d(1.0, 1.0, 1.2, -1.0, 1.0, -1.0, 1.0, -1.5, 1.5,
	       alt[k], az[k])
	plbox3("bnstu", "x axis", 0.0, 0,
		"bnstu", "y axis", 0.0, 0,
		"bcdmnstuv", "z axis", 0.0, 4)

	plcol(2)
	plmesh(x, y, z, opt[k])
	plcol(3)
	plmtex("t", 1.0, 0.5, 0.5, title[k])

	pleop()

    plend()

main()
