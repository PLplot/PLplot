#!/usr/bin/env python

#	contour plot demo.

import math
import sys
from Numeric import *
from pl import *

xpts = 35
ypts = 46

clevel = arange(-1., 1., 0.2)

def main():

    mark = 1500
    space = 1500

    plinit()

    z = zeros( (xpts, ypts), 'd' )
    w = zeros( (xpts, ypts), 'd' )

    for i in range(xpts):
	xx = (i - xpts/2) / (1.*(xpts/2))
	for j in range(ypts):
	    yy = (j - ypts/2) / (1.*(ypts/2)) - 1.
	    z[i,j] = xx * xx - yy * yy
	    w[i,j] = 2. * xx * yy

    plenv( 0., 1.*(xpts-1), 0., 1.*(ypts-1), 0, 0 )
    plcol(2)
#    plcont2( z, 1, xpts, 1, ypts, clevel )
# Note this works, but the x and y range is just the index range.
# I am sure that by using a longer argument list and a transformation
# function you can rescale to -1 to 1 for both x and y, but AWI
# hasn't bothered to investigate this yet.
    plcont( z, clevel )
# The following block of commented out code displays the equivalent
# shaded plot of z.  Note the first 4 arguments of plenv are
# identical to the 2-5th arguments of plshade to control the x and
# y scaling.
#    plenv( -1., 1., -1., 1., 0, 0 )
#    zmin = min(z.flat)
#    zmax = max(z.flat)
#
#    ns = 25
#    sh_cmap = 1
#    min_color = 1; min_width = 0; max_color = 0; max_width = 0
#    for i in range(ns):
#	shade_min = zmin + (zmax - zmin) * i / ns
#	shade_max = zmin + (zmax - zmin) * (i+1) / ns
#	sh_color = i/(ns-1.)
#	sh_width = 2
#	plpsty(0)
#	plshade( z, -1., 1., -1., 1.,
#		   shade_min, shade_max, sh_cmap, sh_color, sh_width,
#		   min_color, min_width, max_color, max_width, 1 )


    plstyl( 1, mark, space )
    plcol(3)
   # plcont2( w, 1, xpts, 1, ypts, clevel )
    plcont( w, clevel )
    plstyl( 0, mark, space )

    # other stuff

    plcol(1)
    pllab("X Coordinate", "Y Coordinate", "Streamlines of flow");

    pleop()
    plend()

main()
