#! /usr/local/bin/python
#
#	x03c.c
#
#	Polar plot demo.

import math
import pl
import sys

# main
#
# Generates polar plot, with 1-1 scaling.

def main():

	dtr = math.pi / 180.0
	x0 = []
	y0 = []
	for i in range(361):
		x0.append(math.cos(dtr * i))
		y0.append(math.sin(dtr * i))

	# Parse and process command line arguments

	pl.ParseOpts(sys.argv, pl.PARSE_FULL)

	# Initialize plplot

	pl.init()

	# Set up viewport and window, but do not draw box

	pl.env(-1.3, 1.3, -1.3, 1.3, 1, -2)

	x = []
	y = []
	for i in range(11):
		for j in range(361):
			x.append(0.1 * i * x0[j])
			y.append(0.1 * i * y0[j])

		# Draw circles for polar grid

		pl.line(x, y)

	pl.col(2)
	for i in range(12):
		theta = 30.0 * i
		dx = math.cos(dtr * theta)
		dy = math.sin(dtr * theta)

		# Draw radial spokes for polar grid

		pl.join(0.0, 0.0, dx, dy)

		# Write labels for angle

		text = `int(theta)`
		if dx >= 0:
			pl.ptex(dx, dy, dx, dy, -0.15, text)
		else:
			pl.ptex(dx, dy, -dx, -dy, 1.15, text)

	# Draw the graph

	x = []
	y = []
	for i in range(361):
		r = math.sin(dtr * (5 * i))
		x.append(x0[i] * r)
		y.append(y0[i] * r)

	pl.col(3)
	pl.line(x, y)

	pl.col(4)
	pl.mtex("t", 2.0, 0.5, 0.5, "#frPLplot Example 3 - r(#gh)=sin 5#gh")

	# Close the plot at end

	pl.end()

main()
