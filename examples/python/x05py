#! /usr/local/bin/python
#
#	x05c.c
#
#	Histogram demo.

import math
import pl
import sys

NPTS = 2047

# main
#
# Draws a histogram from sample data.

def main():

	# Parse and process command line arguments

	pl.ParseOpts(sys.argv, pl.PARSE_FULL)

	# Initialize plplot

	pl.init()

	# Fill up data points

	delta = 2.0 * math.pi / NPTS
	data = []
	for i in range(NPTS):
		data.append(math.sin(i * delta))

	pl.col(1)
	pl.hist(data, -1.1, 1.1, 44, 0)
	pl.col(2)
	pl.lab("#frValue", "#frFrequency",
	       "#frPLplot Example 5 - Probability function of Oscillator")

	pl.end()

main()
