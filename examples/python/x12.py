#! /usr/local/bin/python
#
#	x12c.c
#
#	Bar chart demo.

import pl
import sys

# main
#
# Does a simple bar chart, using color fill.  If color fill is
# unavailable, pattern fill is used instead (automatic).

def main():

	# Parse and process command line arguments

	pl.ParseOpts(sys.argv, pl.PARSE_FULL)

	# Initialize plplot

	pl.init()

	pl.adv(0)
	pl.vsta()
	pl.wind(1980.0, 1990.0, 0.0, 35.0)
	pl.box("bc", 1.0, 0, "bcnv", 10.0, 0)
	pl.col(2)
	pl.lab("Year", "Widget Sales (millions)", "#frPLplot Example 12")

	y0 = [5, 15, 12, 24, 28, 30, 20, 8, 12, 3]

	for i in range(10):
		pl.col(i + 1)
		pl.psty(0)
		fbox((1980. + i), y0[i])
		string = `y0[i]`
		pl.ptex((1980. + i + .5), (y0[i] + 1.), 1.0, 0.0, .5, string)
		string = `1980 + i`
		pl.mtex("b", 1.0, ((i + 1) * .1 - .05), 0.5, string)

	# Don't forget to call PLEND to finish off!

	pl.end()

def fbox(x0, y0):

    x = [x0, x0, x0 + 1., x0 + 1.]
    y = [0., y0, y0, 0.]
    pl.fill(x, y)
    pl.col(1)
    pl.lsty(1)
    pl.line(x, y)

main()
