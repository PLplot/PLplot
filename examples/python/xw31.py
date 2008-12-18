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

# Test setting / getting compression parameter across plinit.
compression1 = 100
plscompression(compression1)

# Test setting / getting familying parameters across plinit.
fam1 = False
num1 = 10
bmax1 = 1000
plsfam(fam1, num1, bmax1)
          
# Test setting / getting page parameters across plinit.
xp1 = 200.
yp1 = 200.
xleng1 = 400
yleng1 = 200
xoff1 = 10
yoff1 = 20
plspage(xp1, yp1, xleng1, yleng1, xoff1, yoff1)

# Initialize plplot
plinit()

from plplot_py_demos import *

def main():

    status = 0

    # Test if device initialization screwed around with the preset
    # compression parameter.
    compression2 = plgcompression()
    sys.stdout.write("Output various PLplot parameters\n")
    sys.stdout.write("compression parameter = %d\n" % compression2)
    if compression2 != compression1:
        sys.stderr.write("plgcompression test failed\n")
        plend()
        status = 1

    # Test if device initialization screwed around with any of the
    # preset familying values.
    (fam2, num2, bmax2) = plgfam()
    sys.stdout.write("family parameters: fam, num, bmax = %d %d %d\n" % (fam2, num2, bmax2))
    if fam2 != fam1  or num2 != num1 or bmax2 != bmax1:
        sys.stderr.write("plgfam test failed\n")
        plend()
        status = 1

    # Test if device initialization screwed around with any of the
    # preset page values.
    (xp2, yp2, xleng2, yleng2, xoff2, yoff2) = plgpage()
    sys.stdout.write("page parameters: xp, yp, xleng, yleng, xoff, yoff = %f %f %d %d %d %d\n" % (xp2, yp2, xleng2, yleng2, xoff2, yoff2))
    if xp2 != xp1 or yp2 != yp1 or xleng2 != xleng1 or yleng2 != yleng1 or xoff2 != xoff1 or yoff2 != yoff1:
        sys.stderr.write("plgpage test failed\n")
        plend()
        status = 1

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

    level2 = plglevel()
    sys.stdout.write("level parameter = %d\n" % level2)
    if level2 != 1:
        sys.stderr.write("plglevel test failed.\n")
        plend()
        status = 1

    pladv(0)
    plvpor(0.01, 0.99, 0.02, 0.49)
    (xmin, xmax, ymin, ymax) = plgvpd()
    sys.stdout.write("plvpor: xmin, xmax, ymin, ymax = %f %f %f %f \n" % (xmin, xmax, ymin, ymax))
    if xmin != 0.01 or xmax != 0.99 or ymin != 0.02 or ymax != 0.49:
        sys.stderr.write("plgvpd test failed\n")
        plend()
        status = 1
    xmid = 0.5*(xmin+xmax)
    ymid = 0.5*(ymin+ymax)

    plwind(0.2, 0.3, 0.4, 0.5)
    (xmin, xmax, ymin, ymax) = plgvpw()
    sys.stdout.write("plwind: xmin, xmax, ymin, ymax = %f %f %f %f \n" % (xmin, xmax, ymin, ymax))
    if xmin != 0.2 or xmax != 0.3 or ymin != 0.4 or ymax != 0.5:
        sys.stderr.write("plgvpw test failed\n")
        plend()
        status = 1

    # Get world coordinates for middle of viewport
    (wx, wy, win) = plcalc_world(xmid,ymid)
    sys.stdout.write("world parameters: wx, wy, win = %f %f %d \n" % (wx, wy, win))
    if abs(wx-0.5*(xmin+xmax)) > 1.0E-5 or abs(wy-0.5*(ymin+ymax)) > 1.0E-5:
        sys.stderr.write("plcalc_world test failed\n")
        plend()
        status = 1

    # Retrieve and print the name of the output file (if any)
    fnam = plgfnam()
    if fnam == '' :
        sys.stdout.write("No output file name is set\n")
    else :
        sys.stdout.write("Output file name read\n")
    sys.stderr.write("Output file name is %s\n" % fnam)

    # Set and get the number of digits used to display axis labels
    # Note digits is currently ignored in pls[xyz]ax and 
    # therefore it does not make sense to test the returned value.
    plsxax(3,0)
    (digmax, digits) = plgxax()
    sys.stdout.write("x axis parameters: digmax, digits = %d %d \n" % (digmax, digits))
    if digmax != 3:
        sys.stderr.write("plgxax test failed\n")
        plend()
        status = 1

    plsyax(4,0)
    (digmax, digits) = plgyax()
    sys.stdout.write("y axis parameters: digmax, digits = %d %d \n" % (digmax, digits))
    if digmax != 4:
        sys.stderr.write("plgyax test failed\n")
        plend()
        status = 1

    plszax(5,0)
    (digmax, digits) = plgzax()
    sys.stdout.write("z axis parameters: digmax, digits = %d %d \n" % (digmax, digits))
    if digmax != 5:
        sys.stderr.write("plgzax test failed\n")
        plend()
        status = 1

    plsdidev(0.05, PL_NOTSET, 0.1, 0.2)
    (mar, aspect, jx, jy) = plgdidev()
    sys.stdout.write("device-space window parameters: mar, aspect, jx, jy = %f %f %f %f \n" % (mar, aspect, jx, jy))
    if mar != 0.05 or jx != 0.1 or jy != 0.2:
        sys.stderr.write("plgdidev test failed\n")
        plend()
        status = 1

    plsdiori(1.0)
    ori = plgdiori()
    sys.stdout.write("ori parameter = %f \n" % ori)
    if ori != 1.0 :
        sys.stderr.write("plgdiori test failed\n")
        plend()
        status = 1

    plsdiplt(0.1, 0.2, 0.9, 0.8)
    (xmin, ymin, xmax, ymax) = plgdiplt()
    sys.stdout.write("plot-space window parameters: xmin, ymin, xmax, ymax = %f %f %f %f \n" % (xmin, ymin, xmax, ymax))
    if xmin != 0.1 or xmax != 0.9 or ymin != 0.2 or ymax != 0.8:
        sys.stderr.write("plgdiplt test failed\n")
        plend()
        status = 1

    plsdiplz(0.1, 0.1, 0.9, 0.9)
    (zxmin, zymin, zxmax, zymax) = plgdiplt()
    sys.stdout.write("zoomed plot-space window parameters: xmin, ymin, xmax, ymax = %f %f %f %f \n" % (zxmin, zymin, zxmax, zymax))
    if abs(zxmin -(xmin + (xmax-xmin)*0.1)) > 1.0E-5 or abs(zxmax -(xmin+(xmax-xmin)*0.9)) > 1.0E-5 or abs(zymin -(ymin+(ymax-ymin)*0.1)) > 1.0E-5 or abs(zymax -(ymin+(ymax-ymin)*0.9)) > 1.0E-5 :
        sys.stderr.write("plsdiplz test failed\n")
        plend()
        status = 1

    plscolbg(10,20,30)
    (r, g, b) = plgcolbg()
    sys.stdout.write("background colour parameters: r, g, b = %d %d %d \n" % (r, g, b))
    if r != 10 or g != 20 or b != 30:
        sys.stderr.write("plgcolbg test failed\n")
        plend()
        status = 1

    plscolbga(20, 30, 40, 0.5)
    (r, g, b, a) = plgcolbga()
    sys.stdout.write("background/transparency colour parameters: r, g, b, a = %d %d %d %f \n" % (r, g, b, a))
    if r != 20 or g != 30 or b != 40 or a != 0.5:
        sys.stderr.write("plgcolbga test failed\n")
        plend()
        status = 1


    # Restore defaults
    #plcol0(1)

    if status != 0 :
        sys.exit(status)

main()
# Terminate plplot
plend()
