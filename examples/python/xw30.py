# $Id:$

#  Copyright (C) 2008 Hazen Babcock
#  Copyright (C) 2008 Andrew Ross

#  Alpha color values demonstration.
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

red   = array([  0, 255,   0,   0])
green = array([  0,   0, 255,   0])
blue  = array([  0,   0,   0, 255])
alpha = array([1.0, 1.0, 1.0, 1.0])

pos = array([0.0, 1.0])
rcoord = array([1.0, 1.0])
gcoord = array([0.0, 0.0])
bcoord = array([0.0, 0.0])
acoord = array([0.0, 1.0])
rev = array([0, 0])

def main():

  px = array([0.1, 0.5, 0.5, 0.1])
  py = array([0.1, 0.1, 0.5, 0.5])


  plscmap0n (4)
  plscmap0a (red, green, blue, alpha)

  # 
  #   Page 1:
  #
  #   This is a series of red, green and blue rectangles overlaid
  #   on each other with gradually increasing transparency.
  

  # Set up the window 
  pladv (0)
  plvpor (0.0, 1.0, 0.0, 1.0)
  plwind (0.0, 1.0, 0.0, 1.0)
  plcol0 (0)
  plbox ("", 1.0, 0, "", 1.0, 0)

  # Draw the boxes
  for i in range(9):
    icol = i%3 + 1

    # Get a color, change its transparency and 
    # set it as the current color. 
    rgba = plgcol0a (icol)
    plscol0a (icol, rgba[0], rgba[1], rgba[2], 1.0 - float(i)/9.0)
    plcol0 (icol)

    # Draw the rectangle 
    plfill (px, py)

    # Shift the rectangles coordinates
    px += 0.5/9.0
    py += 0.5/9.0

  # 
  #   Page 2:
  #
  #   This is a bunch of boxes colored red, green or blue with a single 
  #   large (red) box of linearly varying transparency overlaid. The
  #   overlaid box is completely transparent at the bottom and completely
  #   opaque at the top.
  

  # Set up the window 
  pladv(0)
  plvpor(0.1, 0.9, 0.1, 0.9)
  plwind(0.0, 1.0, 0.0, 1.0)

  # Draw the boxes. There are 25 of them drawn on a 5 x 5 grid. 
  for i in range(5): 
    # Set box X position 
    px[0] = 0.05 + 0.2 * i
    px[1] = px[0] + 0.1
    px[2] = px[1]
    px[3] = px[0]

    # We don't want the boxes to be transparent, so since we changed   
    # the colors transparencies in the first example we have to change 
    # the transparencies back to completely opaque. 
    icol = i%3 + 1
    rgba = plgcol0a (icol)
    plscol0a (icol, rgba[0], rgba[1], rgba[2], 1.0)
    plcol0 (icol)
    for j in range(5):
      # Set box y position and draw the box. 
      py[0] = 0.05 + 0.2 * j
      py[1] = py[0]
      py[2] = py[0] + 0.1
      py[3] = py[2]
      plfill(px, py)

  # The overlaid box is drawn using plshades with a color map that is 
  # the same color but has a linearly varying transparency. 

  # Create the color map with 128 colors and use plscmap1la to initialize 
  # the color values with a linear varying transparency (or alpha) 
  plscmap1n(128)
  plscmap1la(1, pos, rcoord, gcoord, bcoord, acoord, rev)

  # Create a 2 x 2 array that contains the z values (0.0 to 1.0) that will 
  # used for the shade plot. plshades will use linear interpolation to 
  # calculate the z values of all the intermediate points in this array.
  z = reshape(zeros(2*2),(2,2))
  z[0][1] = 1.0
  z[1][1] = 1.0

  # Set the color levels array. These levels are also between 0.0 and 1.0
  clevel = 0.01*arange(101)

  # Draw the shade plot with zmin = 0.0, zmax = 1.0 and x and y coordinate 
  # ranges such that it fills the entire plotting area. 
  plshades(z, 0.0, 1.0, 0.0, 1.0, clevel, 0, 1)


main()
