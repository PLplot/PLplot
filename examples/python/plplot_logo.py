#!/usr/bin/env python

# Copyright (C) 2008  Alan W. Irwin
#
# This file is part of PLplot.
#
# PLplot is free software; you can redistribute it and/or modify
# it under the terms of the GNU Library General Public License as published
# by the Free Software Foundation; version 2 of the License.
#
# PLplot is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Library General Public License for more details.
#
# You should have received a copy of the GNU Library General Public License
# along with the file PLplot; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA


# Generate PLplot logo.

# Append to effective python path so that can find plplot modules.
from plplot_python_start import *

import sys
from plplot import *

# Parse and process command line arguments
plparseopts(sys.argv, PL_PARSE_FULL)

# Initialize plplot
plinit()

from plplot_py_demos import *

XPTS = 35		# Data points in x
YPTS = 46		# Data points in y

alt = 60.0
az = 30.0
# Routine for defining a specific color map 1 in HLS space.
# if gray is true, use basic grayscale variation from half-dark to light.
# otherwise use false color variation from blue (240 deg) to red (360 deg).
def cmap1_init(gray):
    # Independent variable of control points.
    i = array((0., 1.))
    if gray:
	# Hue for control points.  Doesn't matter since saturation is zero.
	h = array((0., 0.))
	# Lightness ranging from half-dark (for interest) to light.
	l = array((0.5, 1.))
	# Gray scale has zero saturation
	s = array((0., 0.))
    else:
	# Hue ranges from blue (240 deg) to red (0 or 360 deg)
	h = array((240., 0.))
	# Lightness and saturation are constant (values taken from C example).
	l = array((0.6, 0.6))
	s = array((0.8, 0.8))

    # number of cmap1 colours is 256 in this case.
    plscmap1n(256)
    # Interpolate between control points to set up cmap1.
    plscmap1l(0, i, h, l, s)

def main():

    rosen = 1
    x = (arange(XPTS) - (XPTS / 2)) / float(XPTS / 2)
    y = (arange(YPTS) - (YPTS / 2)) / float(YPTS / 2)
    if rosen == 1:
	x = 1.5*x
	y = 0.5 + y
    x.shape = (-1,1)
    r2 = (x*x) + (y*y)
    z = (1. - x)*(1. - x) + 100 * (x*x - y)*(x*x - y)
    # The log argument may be zero for just the right grid.  */
    z = log(choose(greater(z,0.), (exp(-5.), z)))

    x.shape = (-1,)
    zmin = min(z.flat)
    zmax = max(z.flat)
    nlevel = 10
    step = (zmax-zmin)/(nlevel+1)
    clevel = zmin + step + arange(nlevel)*step
    plschr(0., 1.8)
    plwid(1)
    pladv(0)
    plvpor(0.0, 1.0, 0.0, 1.0)
    plwind(-0.43, 0.840, 0.05, 0.48)
    plcol0(1)
    plw3d(1.0, 1.0, 1.0, -1.5, 1.5, -0.5, 1.5, zmin, zmax,
          alt, az)
    plbox3("bnstu", "", 0.0, 0,
           "bnstu", "", 0.0, 0,
           "bcdmnstuv", "", 0.0, 0)
    plmtex3("zs", 5.0, 1.07, 1.0, "z axis")

    plcol0(2)
    # magnitude colored plot with faceted squares
    cmap1_init(0)
    plsurf3d(x, y, z, MAG_COLOR | FACETED, ())
    
    # Transluscent gradient over left portion of surface.
    plvpor(0.0, 0.7, 0.0, 1.0)
    plwind(0.0, 1.0, 0.0, 1.0)
    # The overlaid box is drawn using plshades with a color map that is 
    # the same color but has a linearly varying transparency. 
    
    pos = array([0.0, 1.0])
    rcoord = array([0.0, 0.0])
    gcoord = array([0.0, 0.0])
    bcoord = array([0.0, 0.0])
    acoord = array([0.0, 1.0])
    rev = array([0, 0])

    # Create the color map with 128 colors and use plscmap1la to initialize 
    # the color values with a linear varying transparency (or alpha) 
    plscmap1n(128)
    plscmap1la(1, pos, rcoord, gcoord, bcoord, acoord, rev)
    
    # Create a 2 x 2 array that contains the z values (0.0 to 1.0) that will 
    # used for the shade plot. plshades will use linear interpolation to 
    # calculate the z values of all the intermediate points in this array.
    z = reshape(zeros(2*2),(2,2))
    z[0][0] = 1.0
    z[0][1] = 1.0
    
    # Set the color levels array. These levels are also between 0.0 and 1.0
    nlevel=601
    clevel = arange(nlevel)/float(nlevel-1)
    
    # Draw the shade plot with zmin = 0.0, zmax = 1.0 and x and y coordinate 
    # ranges such that it fills the entire plotting area. 
    plshades(z, 0.0, 1.0, 0.0, 1.0, clevel, 0, 1)
    
main()
# Terminate plplot
plend()
