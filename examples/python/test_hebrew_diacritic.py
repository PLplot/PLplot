#!/usr/bin/env python
# -*- coding: utf-8; -*-
#  Copyright (C) 2010 Alan W. Irwin

#  Displays Genesis Chapter 1 verse 3 "And God said: 'Let there be
#  light.' And there was light" in Hebrew as a test of diacritical
#  mark positioning.
#
#  This file is part of PLplot.
#
#  PLplot is free software; you can redistribute it and/or modify
#  it under the terms of the GNU Library General Public License as published
#  by the Free Software Foundation; either version 2 of the License, or
#  (at your option) any later version.
#
#  PLplot is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU Library General Public License for more details.
#
#  You should have received a copy of the GNU Library General Public License
#  along with PLplot; if not, write to the Free Software
#  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
#
# Append to effective python path so that can find plplot modules.
from plplot_python_start import *

import sys
from plplot import *

# Parse and process command line arguments
plparseopts(sys.argv, PL_PARSE_FULL)

# Initialize plplot
plinit()

from plplot_py_demos import *

def main():
    pladv(0)
    plcol0(2)
    plvpor(0.02, 0.98, 0.02, 0.90)
    plwind(0.0, 1.0, 0.0, 1.0)
    plsfci(0)
    plschr(0., 2.5)
    # The text is Genesis, Chapter 1 verse 3 in Hebrew.
    plptex (0.5, 0.5, 1., 0., 0.5, "וַיֹּאמֶר אֱלֹהִים יְהִי אוֹר וַיְהִי אוֹר וַיַּרְא")
    plschr(0., 1.0)
    # Restore defaults
    plcol0(1)
main()
plend()

