#!/usr/bin/env python
# $Id$

#  Copyright (C) 2008 Alan W. Irwin

#  Set/get tester.
#
#  This file is part of PLplot.
#
#  PLplot is free software; you can redistribute it and/or modify
#  it under the terms of the GNU General Library Public License as published
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

# Test setting / getting page size across plinit.
xp1 = 200.
yp1 = 200.
xleng1 = 400
yleng1 = 200
xoff1 = 10
yoff1 = 20
plspage(xp1, yp1, xleng1, yleng1, xoff1, yoff1)

compression = plgcompression()
compression1 = compression + 100
plscompression(compression1)
compression2 = plgcompression()
if compression2 != compression1:
    sys.stderr.write("plgcompression test failed\n")
    plend()
    sys.exit(1)
plscompression(compression)

(fam, num, bmax) = plgfam()
fam1 = fam + 100
num1 = num + 100
bmax1 = bmax + 100
plsfam(fam1, num1, bmax1)
(fam2, num2, bmax2) = plgfam()
if fam2 != fam1  or num2 != num1 or bmax2 != bmax1:
    sys.stderr.write("plgfam test failed\n")
    plend()
    sys.exit(1)
plsfam(fam, num, bmax)
          
# Initialize plplot
plinit()

from plplot_py_demos import *

def main():

    # Test if device initialization screwed around with any of the
    # preset page values.
    (xp2, yp2, xleng2, yleng2, xoff2, yoff2) = plgpage()
    if xp2 != xp1 or yp2 != yp1 or xleng2 != xleng1 or yleng2 != yleng1 or xoff2 != xoff1 or yoff2 != yoff1:
        sys.stderr.write("plgpage test failed\n")
        plend()
        sys.exit(1)

    # Exercise plscolor, plscol0, plscmap1, and plscmap1a to make sure
    # they work without any obvious error messages.
    plscolor(1)
    plscol0(1, 255, 0, 0)
    r1 = [0, 255]
    g1 = [255, 0]
    b1 = [0, 0]
    a1 = [1.0, 1.0]
    plscmap1(r1,g1,b1)
    plscmap1a(r1,g1,b1,a1)

    level = plglevel()
    if level != 1:
        sys.stderr.write("plglevel test failed. Level is %d, but 1 expected.\n" % level)
        plend();
        sys.exit(1);

    pladv(0)
    plvpor(0.0, 1.0, 0.0, 1.0)
    plwind(0.2, 0.3, 0.4, 0.5)

    # Restore defaults
    #plcol0(1)
    
main()
# Terminate plplot
plend()
