#! /usr/local/bin/python
#
#	x10c.c
#
#	Window positioning demo.

import pl
import sys

# main
#
# Demonstrates absolute positioning of graphs on a page.

def main():

	# Parse and process command line arguments

	pl.ParseOpts(sys.argv, pl.PARSE_FULL)

	# Initialize plplot

	pl.init()

	pl.adv(0)
	pl.vpor(0.0, 1.0, 0.0, 1.0)
	pl.wind(0.0, 1.0, 0.0, 1.0)
	pl.box("bc", 0.0, 0, "bc", 0.0, 0)

	pl.svpa(50.0, 150.0, 100.0, 150.0)
	pl.wind(0.0, 1.0, 0.0, 1.0)
	pl.box("bc", 0.0, 0, "bc", 0.0, 0)
	pl.ptex(0.5, 0.5, 1.0, 0.0, 0.5, "BOX at (50,150,100,150)")
	pl.end()

main()
