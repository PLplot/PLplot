#!/usr/bin/python

#	Simple line plot and multiple windows demo.

import math
import sys
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

	global xscale, yscale, xoff, yoff

	plinit()

	# Set up the data
	# Original case

	xscale = 6.
	yscale = 1.
	xoff = 0.
	yoff = 0.

	# Do a plot

	plot1()
 
	# Set up the data

	xscale = 1.
	yscale = 0.0014
	yoff = 0.0185

	# Do a plot

	digmax = 5
	plsyax(digmax, 0)
	plot1()

	plot2()

	plot3()

	plend()

# ===============================================================

def plot1():

	global x, y, xscale, yscale, xoff, yoff, xs, ys

	x = zeros(60,'d'); y = zeros(60,'d')
	for i in range(60):
	    x[i] = xoff + xscale * (i + 1) / 60.0
	    y[i] = yoff + yscale * pow(x[i], 2.)

	xmin = x[0]
	xmax = x[59]
	ymin = y[0]
	ymax = y[59]

	xs = zeros(6,'d'); ys = zeros(6,'d')
	for i in range(6):
	    xs[i] = x[i * 10 + 3]
	    ys[i] = y[i * 10 + 3]

	# Set up the viewport and window using pl.env. The range in X
	# is 0.0 to 6.0, and the range in Y is 0.0 to 30.0. The axes
	# are scaled separately (just = 0), and we just draw a
	# labelled box (axis = 0).

	plcol(1)
	plenv(xmin, xmax, ymin, ymax, 0, 0)
	plcol(2)
	pllab("(x)", "(y)", "#frPLplot Example 1 - y=x#u2")

	# Plot the data points

	plcol(4)
	plpoin(xs, ys, 9)

	# Draw the line through the data

	plcol(3)
	plline(x, y)

	pleop()

# ===============================================================

def plot2():

	global x, y, xscale, yscale, xoff, yoff, xs, ys

	# Set up the viewport and window using pl.env. The range in X
	# is -2.0 to 10.0, and the range in Y is -0.4 to 2.0. The axes
	# are scaled separately (just = 0), and we draw a box with
	# axes (axis = 1).

	plcol(1)
	plenv(-2.0, 10.0, -0.4, 1.2, 0, 1)
	plcol(2)
	pllab("(x)", "sin(x)/x", "#frPLplot Example 1 - Sinc Function")

	# Fill up the arrays

	x = zeros(100,'d'); y = zeros(100,'d')
	for i in range(100):
	    x[i] = (i - 19.0) / 6.0
	    if x[i] == 0.0:
		y[i] = 1.
	    else:
		y[i] = math.sin(x[i]) / x[i]

	# Draw the line

	plcol(3)
	plline(x, y)

	pleop()

# ===============================================================

def plot3():

	global x, y, xscale, yscale, xoff, yoff, xs, ys

	# For the final graph we wish to override the default tick
	# intervals, so do not use pl.env

	pladv(0)

	# Use standard viewport, and define X range from 0 to 360
	# degrees, Y range from -1.2 to 1.2.

	plvsta()
	plwind(0.0, 360.0, -1.2, 1.2)

	# Draw a box with ticks spaced 60 degrees apart in X, and 0.2 in Y.

	plcol(1)
	plbox("bcnst", 60.0, 2, "bcnstv", 0.2, 2)

	# Superimpose a dashed line grid, with 1.5 mm marks and spaces. 
	# plstyl expects a pointer!! 

	#w.plstyl([1500], [1500])
	plcol(2)
	plbox("g", 30.0, 0, "g", 0.2, 0)
	#w.plstyl([], [])

	plcol(3)
	pllab("Angle (degrees)", "sine",
	       "#frPLplot Example 1 - Sine function")

	x = zeros(101,'d'); y = zeros(101,'d')
	for i in range(101):
	    x[i] = 3.6 * i
	    y[i] = math.sin(x[i] * math.pi / 180.0)

	plcol(4)
	plline(x, y)

	pleop()

main()
