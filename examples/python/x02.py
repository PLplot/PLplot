#! /usr/local/bin/python
#
#	x02c.c
#
#	Multiple window and color map 0 demo.

import pl
import sys

# Demonstrates multiple windows and default color map 0 palette.

def main():

	# Divide screen into 16 regions

	pl.ssub(4, 4)

	# Parse and process command line arguments

	pl.ParseOpts(sys.argv, pl.PARSE_FULL)

	# Initialize plplot

	pl.init()

	pl.schr(0.0, 3.5)
	pl.font(4)

	for i in range(16):
		pl.col(i)
		pl.adv(0)
		vmin = 0.1
		vmax = 0.9
		for j in range(3):
			pl.wid(j + 3)
			pl.vpor(vmin, vmax, vmin, vmax)
			pl.wind(0.0, 1.0, 0.0, 1.0)
			pl.box("bc", 0.0, 0, "bc", 0.0, 0)
			vmin = vmin + 0.1
			vmax = vmax - 0.1
		pl.wid(1)
		pl.ptex(0.5, 0.5, 1.0, 0.0, 0.5, `i`)

	pl.end()

main()
