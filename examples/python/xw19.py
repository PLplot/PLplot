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

## "Normalize" longitude values so that they always fall between -180.0 and
## 180.0
def normalize_longitude(lon):
    if ((lon >= -180.0) and (lon <= 180.0)):
        return lon
    else :
        times = floor ((fabs(lon) + 180.0) / 360.0)
        if (lon < 0.0) :
            return(lon + 360.0 * times)
        else :
            return(lon - 360.0 * times)


## A custom axis labeling function for longitudes and latitudes.
def geolocation_labeler(axis, value, data):
    if (axis == 2) :
        label_val = value
        if (label_val > 0.0) :
            direction_label = " N"
        elif (label_val < 0.0) :
            direction_label = " S"
        
        else :
            direction_label = "Eq"
    elif (axis == 1) :
        label_val = normalize_longitude(value)
        if (label_val > 0.0) :
            direction_label = " E"
        elif (label_val < 0.0) :
            direction_label = " W"
        else :
            direction_label = ""

    if (axis == 2 and value == 0.0) :
        # A special case for the equator
        label = direction_label
    else :
        label = ""+`abs(label_val)` + direction_label
    return label


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

    # Setup a custom latitude and longitude-based scaling function.
    plslabelfunc(geolocation_labeler, None)

    plcol0(1)
    plenv(minx, maxx, miny, maxy, 1, 70)
    plmap(None,"usaglobe", minx, maxx, miny, maxy)

    # The Americas

    minx = 190
    maxx = 340

    plcol0(1)
    plenv(minx, maxx, miny, maxy, 1, 70)
    plmap(None, "usaglobe", minx, maxx, miny, maxy)

    # Clear the labeling function
    plslabelfunc(None, None)

    # Polar, Northern hemisphere

    minx = 0
    maxx = 360

    plenv(-75., 75., -75., 75., 1, -1)
    plmap(mapform19,"globe", minx, maxx, miny, maxy)

    pllsty(2)
    plmeridians(mapform19,10.0, 10.0, 0.0, 360.0, -10.0, 80.0)

main()
