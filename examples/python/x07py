#! /usr/local/bin/python
#
#	x07c.c
#
#	Font demo.

import pl
import sys

# main
#
# Displays the entire "plsym" symbol (font) set.

def main():

	base = [0, 200, 500, 600, 700, 800, 900, 2000, 2100,
		2200, 2300, 2400, 2500, 2600, 2700, 2800, 2900]

	# Parse and process command line arguments

	pl.ParseOpts(sys.argv, pl.PARSE_FULL)

	# Initialize plplot

	pl.init()

	pl.fontld(1)

	for l in range(17):
	    pl.adv(0)

	    # Set up viewport and window

	    pl.col(2)
	    pl.vpor(0.15, 0.95, 0.1, 0.9)
	    pl.wind(0.0, 1.0, 0.0, 1.0)

	    # Draw the grid using plbox

	    pl.box("bcgt", 0.1, 0, "bcgt", 0.1, 0)

	    # Write the digits below the frame

	    pl.col(15)
	    for i in range(10):
		    pl.mtex("b", 1.5, (0.1 * i + 0.05), 0.5, `i`)

	    k = 0
	    for i in range(10):

		    # Write the digits to the left of the frame

		    text = `base[l] + 10 * i`
		    pl.mtex("lv", 1.0, (0.95 - 0.1 * i), 1.0, text)

		    for j in range(10):
			    x = [ 0.1 * j + 0.05 ]
			    y = [ 0.95 - 0.1 * i ]

			    # Display the symbols

			    pl.sym(x, y, base[l] + k)
			    k = k + 1

	    pl.mtex("t", 1.5, 0.5, 0.5, "PLplot Example 7 - PLSYM symbols")

	pl.end()

main()
