#!/usr/bin/env python

# Append to effective python path so that can find plplot modules.
from plplot_python_start import *

import sys
from plplot import *
from numpy import *

# Parse and process command line arguments
plparseopts(sys.argv, PL_PARSE_FULL)

# Initialize plplot
plinit()
# Like yellow lines better.
plcol0(2)
pladv(0)
plvpor(0.2, 0.8, 0.2, 0.8)
plwind(0., 1., 0., 1.)
plbox("bc", 0., 0, "bc", 0., 0)

plptex(0.5, 0.8, 1., 0., 0.5, "X#d0#d#<italic/>1#d2#u3#u4#u5#u6#u7#u8#d9#d0#d1")
plptex(0.5, 0.8, 1., 0., 0.5, "X#u0#u#<italic/>1#u2#d3#d4#d5#d6#d7#d8#u9#u0#u1")
plptex(0.5, 0.6, 1., 0., 0.5, "#ga#uaa#gad#gdp#gr#gf#d, #ga#u#ga#gaad#gdp#gr#gf#d, B#ua#<italic/>a#gad#gdp#gr#gf#d, B#u#ga#gaad#gdp#gr#gf#d")
plptex(0.5, 0.4, 1., 0., 0.5, "#ga#daa#gad#gdp#gr#gf#u, #ga#d#ga#gaad#gdp#gr#gf#u, B#da#<italic/>a#gad#gdp#gr#gf#u, B#d#ga#gaad#gdp#gr#gf#u")
plptex(0.5, 0.2, 1., 0., 1.1, "B#uf#d, B#ur#d")
plptex(0.5, 0.2, 1., 0., -0.1, "B#df#u, B#dr#u")

pladv(0)
plvpor(0.2, 0.8, 0.2, 0.8)
plwind(0., 1.e-200, 0., 1.e-200)
plbox("bcmnt", 0., 0, "bcmntv", 0., 0)

# Some fiducial marks that show yoffset problems with the pdf device.
plmtex("t", -5.125, 0.1, 0.5, "X")
plmtex("t", -5.125, 0.2, 0.5, "X#d")
plmtex("t", -5.125, 0.3, 0.5, "X#u")
#plmtex("t", -10.250, 0.1, 0.5, "X")
#plmtex("t", -15.375, 0.1, 0.5, "X")
#plmtex("t", -20.500, 0.1, 0.5, "X")

# Roughly duplicate plptex tests of superscript/subscript.
plmtex("t", -5.125, 0.5, 0.5, "X#d0#d1#d2#u3#u4#u5#u6#u7#u8#d9#d0#d1")
plmtex("t", -5.125, 0.5, 0.5, "X#u0#u1#u2#d3#d4#d5#d6#d7#d8#u9#u0#u1")
plmtex("t", -10.250, 0.5, 0.5, "#ga#uaa#gad#gdp#gr#gf#d, #ga#u#ga#gaad#gdp#gr#gf#d, B#uaa#gad#gdp#gr#gf#d, B#u#ga#gaad#gdp#gr#gf#d")
plmtex("t", -15.375, 0.5, 0.5, "#ga#daa#gad#gdp#gr#gf#u, #ga#d#ga#gaad#gdp#gr#gf#u, B#daa#gad#gdp#gr#gf#u, B#d#ga#gaad#gdp#gr#gf#u")
plmtex("t", -20.500, 0.5, 1.1, "B#uf#d, B#ur#d")
plmtex("t", -20.500, 0.5, -0.1, "B#df#u, B#dr#u")

# These four tests mimic what occurs for badly scaled plbox axes.
plmtex("t", 3.2, 1.0, 0.5, "(x10#u-100#d)")
plmtex("b", 3.2, 1.0, 0.5, "(x10#u-100#d)")
plmtex("t", 2.0, 0.0-0.02, 1.0, "(x10#u-100#d)")
plmtex("t", 2.0, 1.0+0.02, 0.0, "(x10#u-100#d)")

# Terminate plplot
plend()

