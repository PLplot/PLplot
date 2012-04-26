#!/usr/bin/env python
# -*- coding: utf-8; -*-
# Test whether line breaks work for utf8 "Peace" strings.
# Append to effective python path so that can find plplot modules.
from plplot_python_start import *

import sys
from plplot import *
from numpy import *

# Taken from http://www.columbia.edu/~fdc/pace/

peace = [
    "和平 (Mandarin)",
    "शांति (Hindi)",
    "Peace (English)",
    "שלום (Hebrew)",
    "Мир (Russian)",
    "Friede (German)",
    "평화 (Korean)",
    "Paix (French)",
    "Paz (Spanish)",
    "ﺳﻼم (Arabic)",
    "Barış (Turkish)",
    "Hasîtî (Kurdish)",
    ]

# Parse and process command line arguments
plparseopts(sys.argv, PL_PARSE_FULL)

# Initialize plplot
plinit()
plcol0(2)
pladv(0)
plvpor(0.1, 0.9, 0.1, 0.9)
plwind(0., 1., 0., 1.)
# Just to show edges of viewport
plbox("bc", 0., 0, "bc", 0., 0)
plcol0(1)
plschr(0., 1.4)
for i in arange(6):
    plptex(0.01, 0.90 - 0.16*i, 1., 0., 0., peace[2*i] + "\n" + peace[2*i+1])
plend()
