#!/usr/bin/env python

#	Pie chart demo.

import sys
import os

module_dir = "@MODULE_DIR@"

if module_dir[0] == '@':
	module_dir = os.getcwd ()

sys.path.insert (0, module_dir)

from Numeric import *
from pl import *

text = ["Maurice",
	"Geoffrey",
	"Alan",
	"Rafael",
	"Vince"]

# main
#
# Does a simple pie chart.

def main():

    # Parse and process command line arguments

    plParseOpts(sys.argv, PARSE_FULL)

    # Initialize plplot

    plinit()

    plenv(0., 10., 0., 10., 1, -2)
    plcol0(2)
    #if these don't add up to 100, the logic adds to the last to make
    #up the balance.
    per = [10., 32., 12., 30., 16.]
    theta0 = 0

    for i in range(5):
	#theta quantities scaled as integers to make logic independent of
	#floating point considerations.
	#n.b. theta multiplied by 2 pi/500 afterward so per is in fact 
	#interpreted as a percentage.
	theta1 = theta0 + int(5.*per[i])
	if i == 4:
	    theta1 = 500

	theta = arrayrange(theta0,theta1+1)
	x = concatenate((array([5.]), (5. + 3.*cos((2.*pi/500.)*theta))))
	y = concatenate((array([5.]), (5. + 3.*sin((2.*pi/500.)*theta))))

	plcol0(i + 1)
	plpsty((i + 3) % 8 + 1)
	plfill(x, y)
	plcol0(1)
	plline(x, y)
	just = (2.*pi/500.)*(theta0 + theta1) / 2.
	dx = .25 * cos(just)
	dy = .25 * sin(just)
	if (theta0 + theta1) < 250 or (theta0 + theta1) > 750:
	    just = 0.
	else:
	    just = 1.

	mid = len(x) / 2
	plptex((x[mid] + dx), (y[mid] + dy), 1.0, 0.0, just, text[i])
	theta0 = theta[-1]

    plfont(2)
    plschr(0., 1.3)
    plptex(5.0, 9.0, 1.0, 0.0, 0.5, "Percentage of Sales")

    pleop()
    plend()

main()
