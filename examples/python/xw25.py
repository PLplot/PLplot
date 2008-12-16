# $Id: xw30.py 8224 2008-02-07 16:18:26Z andrewross $

#  Copyright (C) 2007, 2008 Andrew Ross

#  Filling and clipping polygons.
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
from plplot_py_demos import *

# main
#
# Test program for filling polygons and proper clipping


def main():

    plssub(3,3)

    xextreme = [ [ -120.0,  120.0 ],
                 [ -120.0,  120.0 ],
                 [ -120.0,  120.0 ],
                 [  -80.0,   80.0 ],
                 [ -220.0, -120.0 ],
                 [  -20.0,   20.0 ],
                 [  -20.0,   20.0 ],
                 [  -80.0,   80.0 ],
                 [   20.0,  120.0 ] ]

    yextreme = [ [ -120.0,  120.0 ],
                 [   20.0,  120.0 ],
                 [  -20.0,  120.0 ],
                 [  -20.0,  120.0 ],
                 [ -120.0,  120.0 ],
                 [ -120.0,  120.0 ],
                 [  -20.0,   20.0 ],
                 [  -80.0,   80.0 ],
                 [ -120.0,  120.0 ] ]
                 
    for j in range(4):

        # Polygon 1: a diamond
        if (j == 0):
            x0 = [0, -100, 0, 100]
            y0 = [-100, 0, 100, 0]
        # Polygon 2: a diamond - reverse direction
        elif (j == 1):
            x0 = [100, 0, -100, 0]
            y0 = [0, 100, 0, -100]
        # Polygon 3: a square with punctures
        elif (j == 2):
            x0 = [-100, -100, 80, -100, -100, -80, 0, 80, 100, 100]
            y0 = [-100, -80, 0, 80, 100, 100, 80, 100, 100, -100]
        # Polygon 4: a square with punctures - reverse direction
        elif (j == 3):
            x0 = [100, 100, 80, 0, -80, -100, -100, 80, -100, -100]
            y0 = [-100, 100, 100, 80, 100, 100, 80, 0, -80, -100]

        for i in range(9):
            pladv(0)
            plvsta()
            plwind(xextreme[i][0], xextreme[i][1], yextreme[i][0], yextreme[i][1])

            plcol0(2)
            plbox("bc", 1.0, 0, "bcnv", 10.0, 0)
            plcol0(1)
            plpsty(0)
            plfill(x0,y0)
            plcol0(2)
            pllsty(1)
            plline(x0,y0)

    # Don't forget to call plend() to finish off!

    plend()

main()
