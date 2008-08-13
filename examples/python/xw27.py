#  $Id$
#
#  Copyright (C) 2007 Arjen Markus
#  Copyright (C) 2008 Andrew Ross

#  Drawing "spirograph" curves - epitrochoids, cycolids, roulettes

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
# Generates two kinds of plots:
#   - construction of a cycloid (animated)
#   - series of epitrochoids and hypotrochoids

def main():

    # R, r, p, N 
    params = [ [21.0,  7.0,  7.0,  3.0],  # Deltoid
               [21.0,  7.0, 10.0,  3.0],
               [21.0, -7.0, 10.0,  3.0],
               [20.0,  3.0,  7.0, 20.0],
               [20.0,  3.0, 10.0, 20.0],
               [20.0, -3.0, 10.0, 20.0],
               [20.0, 13.0,  7.0, 20.0],
               [20.0, 13.0, 20.0, 20.0],
               [20.0,-13.0, 20.0, 20.0] ]

    # Illustrate the construction of a cycloid

    # TODO
    #cycloid()

    # Loop over the various curves
    # First an overview, then all curves one by one
    plssub(3, 3)  # Three by three window

    for i in range(9) :
        pladv(0)
        plvpor( 0.0, 1.0, 0.0, 1.0 )
        spiro( params[i] )

    pladv(0)
    plssub(1, 1)  # One window per curve

    for i in range(9):
        pladv(0)
        plvpor( 0.0, 1.0, 0.0, 1.0 )
        spiro( params[i] )


def spiro(params):
    # Fill the coordinates
    NPNT = 20000

    windings = int(params[3])
    steps    = int(NPNT/windings)
    dphi     = 8.0*arccos(-1.0)/steps

    xmin = 0.0 # This initialisation is safe!
    xmax = 0.0
    ymin = 0.0
    ymax = 0.0

    xcoord = zeros(windings*steps+1)
    ycoord = zeros(windings*steps+1)

    for i in range(windings*steps+1) :
        phi       = i * dphi
        phiw      = (params[0]-params[1])/params[1]*phi
        xcoord[i] = (params[0]-params[1])*cos(phi) + params[2]*cos(phiw)
        ycoord[i] = (params[0]-params[1])*sin(phi) - params[2]*sin(phiw)
        
        if ( xmin > xcoord[i] ) :
            xmin = xcoord[i]
        if ( xmax < xcoord[i] ) :
            xmax = xcoord[i]
        if ( ymin > ycoord[i] ) :
            ymin = ycoord[i]
        if ( ymax < ycoord[i] ) :
            ymax = ycoord[i]
            
    if ( xmax-xmin > ymax-ymin ) :
        scale = xmax - xmin
    else :
        scale = ymax - ymin
    
    xmin = - 0.65 * scale
    xmax =   0.65 * scale
    ymin = - 0.65 * scale
    ymax =   0.65 * scale

    plwind( xmin, xmax, ymin, ymax )

    plcol0(1)
    plline( xcoord, ycoord )


main()
