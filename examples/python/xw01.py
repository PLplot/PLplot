#!/usr/bin/env python

#	Simple line plot and multiple windows demo.

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
# Generates several simple line plots.  Demonstrates:
#   - subwindow capability
#   - setting up the window, drawing plot, and labelling
#   - changing the color
#   - automatic axis rescaling to exponential notation
#   - placing the axes in the middle of the box
#   - gridded coordinate axes

def main():

	# Parse and process command line arguments

	plParseOpts(sys.argv, PARSE_FULL)

	# Initialize plplot

	plinit()

	# Do a plot with one range of data

	plot1(6., 1., 0., 0.)
 
	# Do a plot with another range of data

	digmax = 5
	plsyax(digmax, 0)
	plot1(1., 0.0014, 0., 0.0185)

	plot2()

	plot3()

	plend()

# ===============================================================

def plot1(xscale, yscale, xoff, yoff):

	x = xoff + (xscale/60.)*(1+arrayrange(60))
	y = yoff + yscale*pow(x,2.)

	xmin = x[0]
	xmax = x[59]
	ymin = y[0]
	ymax = y[59]

	xs = x[3::10]
	ys = y[3::10]

	# Set up the viewport and window using pl.env. The range in X
	# is 0.0 to 6.0, and the range in Y is 0.0 to 30.0. The axes
	# are scaled separately (just = 0), and we just draw a
	# labelled box (axis = 0).

	plcol0(1)
	plenv(xmin, xmax, ymin, ymax, 0, 0)
	plcol0(2)
	pllab("(x)", "(y)", "#frPLplot Example 1 - y=x#u2")

	# Plot the data points

	plcol0(4)
	plpoin(xs, ys, 9)

	# Draw the line through the data

	plcol0(3)
	plline(x, y)

	pleop()

# ===============================================================

def plot2():

	# Set up the viewport and window using pl.env. The range in X
	# is -2.0 to 10.0, and the range in Y is -0.4 to 2.0. The axes
	# are scaled separately (just = 0), and we draw a box with
	# axes (axis = 1).

	plcol0(1)
	plenv(-2.0, 10.0, -0.4, 1.2, 0, 1)
	plcol0(2)
	pllab("(x)", "sin(x)/x", "#frPLplot Example 1 - Sinc Function")

	# Fill up the arrays

	x = (arrayrange(100)-19)/6.0
	# Numeric doesn't handle floating point exceptions, yet, so this 
	# creates a Nan for the zero value.
	y = sin(x)/x
	# replace Nan by correct value.
	if 0.0 in x:
	    y[list(x).index(0.0)] = 1.

	# Draw the line

	plcol0(3)
	plline(x, y)

	pleop()

# ===============================================================

def plot3():

	# For the final graph we wish to override the default tick
	# intervals, so do not use pl.env

	pladv(0)

	# Use standard viewport, and define X range from 0 to 360
	# degrees, Y range from -1.2 to 1.2.

	plvsta()
	plwind(0.0, 360.0, -1.2, 1.2)

	# Draw a box with ticks spaced 60 degrees apart in X, and 0.2 in Y.

	plcol0(1)
	plbox("bcnst", 60.0, 2, "bcnstv", 0.2, 2)

	# Superimpose a dashed line grid, with 1.5 mm marks and spaces. 
	# plstyl expects a pointer!! 

	#w.plstyl([1500], [1500])
	plcol0(2)
	plbox("g", 30.0, 0, "g", 0.2, 0)
	#w.plstyl([], [])

	plcol0(3)
	pllab("Angle (degrees)", "sine",
	       "#frPLplot Example 1 - Sine function")

	x = 3.6*arrayrange(101)
	y = sin((pi/180.)*x)

	plcol0(4)
	plline(x, y)

	pleop()

main()
