#! /usr/local/bin/python
#
#	x11c.c
#
#	Mesh plot demo.

import math
import pl
import sys

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

	# Parse and process command line arguments

	pl.ParseOpts(sys.argv, pl.PARSE_FULL)

	# Initialize plplot

	pl.init()

	x = []
	y = []
	z = []

	for i in range(XPTS):
		x.append(float(i - (XPTS / 2)) / float(XPTS / 2))

	for i in range(YPTS):
		y.append(float(i - (YPTS / 2)) / float(YPTS / 2))

	for i in range(XPTS):
		xx = x[i]
		zz = []
		for j in range(YPTS):
			yy = y[j]
			zz.append(math.cos(2.0 * math.pi * xx) *
				  math.sin(2.0 * math.pi * yy))
		z.append(zz)

	for k in range(4):
		pl.adv(0)
		pl.col(1)
		pl.vpor(0.0, 1.0, 0.0, 0.8)
		pl.wind(-1.0, 1.0, -1.0, 1.5)

		pl.w3d(1.0, 1.0, 1.2, -1.0, 1.0, -1.0, 1.0, -1.5, 1.5,
		       alt[k], az[k])
		pl.box3("bnstu", "x axis", 0.0, 0,
			"bnstu", "y axis", 0.0, 0,
			"bcdmnstuv", "z axis", 0.0, 4)

		pl.col(2)
		pl.mesh(x, y, z, opt[k])
		pl.col(3)
		pl.mtex("t", 1.0, 0.5, 0.5, title[k])

	pl.end()

main()
