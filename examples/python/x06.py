#! /usr/local/bin/python
#
#	x06c.c
#
#	Font demo.

import pl
import sys

# main
#
# Displays the entire "plpoin" symbol (font) set.

def main():

	# Parse and process command line arguments

    	pl.ParseOpts(sys.argv, pl.PARSE_FULL)

	# Initialize plplot

	pl.init()

	pl.adv(0)

	# Set up viewport and window

	pl.col(2)
	pl.vpor(0.1, 1.0, 0.1, 0.9)
	pl.wind(0.0, 1.0, 0.0, 1.3)

	# Draw the grid using plbox

	pl.box("bcgt", 0.1, 0, "bcgt", 0.1, 0)

	# Write the digits below the frame

	pl.col(15)
	for i in range(10):
		pl.mtex("b", 1.5, (0.1 * i + 0.05), 0.5, `i`)

	k = 0
	for i in range(13):

		# Write the digits to the left of the frame

		pl.mtex("lv", 1.0, (1.0 - (2 * i + 1) / 26.0), 1.0, `10 * i`)

		for j in range(10):
			x = 0.1 * j + 0.05
			y = 1.25 - 0.1 * i

			# Display the symbol (plpoin expects that x
			# and y are arrays so pass lists)

			if k < 128:
				pl.poin([x], [y], k)
			k = k + 1

	pl.mtex("t", 1.5, 0.5, 0.5, "PLplot Example 6 - plpoin symbols")
	pl.end()

main()
