#  Copyright (C) 2007 Arjen Markus
#  Copyright (C) 2008 Andrew Ross

#  Drawing "spirograph" curves - epitrochoids, cycolids, roulettes

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

from plplot_py_demos import *
import types

# main
# 
# Generates two kinds of plots:
#   - construction of a cycloid (animated)
#   - series of epitrochoids and hypotrochoids

def main():

    # R, r, p, N
    # R and r should be integers to give correct termination of the
    # angle loop using gcd.
    # N.B. N is just a place holder since it is no longer used
    # (because we now have proper termination of the angle loop).
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
        spiro( params[i], 0 )

    pladv(0)
    plssub(1, 1)  # One window per curve

    for i in range(9):
        pladv(0)
        plvpor( 0.0, 1.0, 0.0, 1.0 )
        spiro( params[i], 0 )

    # Fill the curves.
    pladv(0)
    plssub(1, 1)  # One window per curve

    for i in range(9):
        pladv(0)
        plvpor( 0.0, 1.0, 0.0, 1.0 )
        spiro( params[i], 1 )

    arcs()

def gcd(a, b):
    if not (type(a) is  types.IntType and type(b) is types.IntType):
        raise RuntimeError, "gcd arguments must be integers"
    a = abs(a);
    b = abs(b);
    while(b != 0):
        t = b
        b = a % b
        a = t
    return a

def spiro(params, fill):
    # Fill the coordinates
    NPNT = 2000

    # Proper termination of the angle loop very near the beginning
    # point, see
    # http://mathforum.org/mathimages/index.php/Hypotrochoid.
    windings = int(abs(params[1])/gcd(int(params[0]), int(params[1])))
    steps    = int(NPNT/windings)
    dphi     = 2.0*pi/float(steps)

    phi = arange(windings*steps+1)*dphi
    phiw      = (params[0]-params[1])/params[1]*phi
    xcoord = (params[0]-params[1])*cos(phi) + params[2]*cos(phiw)
    ycoord = (params[0]-params[1])*sin(phi) - params[2]*sin(phiw)
    xmin = min(xcoord)
    xmax = max(xcoord)
    ymin = min(ycoord)
    ymax = max(ycoord)
    
    xrange_adjust = 0.15 * (xmax - xmin)
    xmin -= xrange_adjust
    xmax += xrange_adjust
    yrange_adjust = 0.15 * (ymax - ymin)
    ymin -= yrange_adjust
    ymax += yrange_adjust

    plwind( xmin, xmax, ymin, ymax )

    plcol0(1)
    if fill:
        plfill( xcoord, ycoord )
    else:
        plline( xcoord, ycoord )

def arcs() :
    NSEG = 8

    theta = 0.0
    dtheta = 360.0 / NSEG
    plenv( -10.0, 10.0, -10.0, 10.0, 1, 0 )

    # Plot segments of circle in different colors
    for i in range (NSEG) :
        plcol0( i%2 + 1 )
        plarc(0.0, 0.0, 8.0, 8.0, theta, theta + dtheta, 0.0, 0)
        theta = theta + dtheta
    

    # Draw several filled ellipses inside the circle at different
    # angles.
    a = 3.0
    b = a * tan( (dtheta/180.0*pi)/2.0 )
    theta = dtheta/2.0
    for i in range(NSEG):
        plcol0( 2 - i%2 )
        plarc( a*cos(theta/180.0*pi), a*sin(theta/180.0*pi), a, b, 0.0, 360.0, theta, 1)
        theta = theta + dtheta
    


main()
