#! /usr/local/bin/python
#
#	x04c.c
#
#	Log plot demo.

import math
import pl
import sys

# main
# 
# Illustration of logarithmic axes, and redefinition of window.

def main():

	# Parse and process command line arguments

    	pl.ParseOpts(sys.argv, pl.PARSE_FULL)

	# Initialize plplot

	pl.init()

	pl.adv(0)
	pl.font(2)

	f0 = 1000.0
	freql = []
	ampl = []
	phase = []
	for i in range(101):
		freql.append(1.0 + i / 20.0)
		freq = pow(10.0, freql[i])
		ampl.append(
		20.0 * math.log10(1.0 / math.sqrt(1.0 + pow((freq / f0), 2.)))
		)
		phase.append(-(180.0 / math.pi) * math.atan(freq / f0))

	pl.vpor(0.15, 0.85, 0.1, 0.9)
	pl.wind(1.0, 6.0, -80.0, 0.0)
	pl.col(1)
	pl.box("bclnst", 0.0, 0, "bnstv", 0.0, 0)
	pl.col(2)
	pl.line(freql, ampl)
	pl.col(1)
	pl.ptex(5.0, -30.0, 1.0, -20.0, 0.5, "-20 dB/decade")

	pl.wind(1.0, 6.0, -100.0, 0.0)
	pl.box("", 0.0, 0, "cmstv", 30.0, 3)
	pl.col(3)
	pl.line(freql, phase)

	pl.col(1)
	pl.mtex("b", 3.2, 0.5, 0.5, "Frequency")
	pl.mtex("t", 2.0, 0.5, 0.5, "Single Pole Low-Pass Filter")
	pl.col(2)
	pl.mtex("l", 5.0, 0.5, 0.5, "Amplitude (dB)")
	pl.col(3)
	pl.mtex("r", 5.0, 0.5, 0.5, "Phase shift (degrees)")

	pl.end()

main()
