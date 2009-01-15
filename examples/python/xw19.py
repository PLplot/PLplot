# $Id$

#  Copyright (C) Wesley Ebisuzaki
#  Copyright (C) 2009 Andrew Ross
#
#  Illustrates backdrop plotting of world, US maps.
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

# mapform19
#
# Defines specific coordinate transformation for example 19.
# Not to be confused with mapform in src/plmap.c.
# x[], y[] are the coordinates to be plotted.
def mapform19(n, x, y):
    for i in range(n):
        radius = 90.0 - y[i]
        xp = radius * cos(x[i] * pi / 180.0)
	yp = radius * sin(x[i] * pi / 180.0)
	x[i] = xp
	y[i] = yp
    return [x,y]

# Null coordinate transform (equivalent to passing NULL to C
# version of plmap / plmeridians.
def nullmapform(n,x,y):
    return [x,y]

# main
#
# Does a series of 3-d plots for a given data set, with different
# viewing options in each plot.

def main():

    # Longitude (x) and latitude (y)

    miny = -70
    maxy = 80

    # Cartesian plots 
    # Most of world

    minx = 190
    maxx = 190+360

    plcol0(1)
    plenv(minx, maxx, miny, maxy, 1, -1)
    plmap(nullmapform,"usaglobe", minx, maxx, miny, maxy)

    # The Americas

    minx = 190
    maxx = 340

    plcol0(1)
    plenv(minx, maxx, miny, maxy, 1, -1)
    plmap(nullmapform, "usaglobe", minx, maxx, miny, maxy)

    # Polar, Northern hemisphere

    minx = 0
    maxx = 360

    plenv(-75., 75., -75., 75., 1, -1)
    plmap(mapform19,"globe", minx, maxx, miny, maxy)

    pllsty(2)
    plmeridians(mapform19,10.0, 10.0, 0.0, 360.0, -10.0, 80.0)
    plend()

main()
