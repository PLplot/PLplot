#!/usr/bin/python

#	contour plot demo.

import math
import sys
from Numeric import *
from pl import *

xpts = 35
ypts = 46

clevel = array( [-1., -.8, -.6, -.4, -.2, 0, .2, .4, .6, .8, 1.] )

def main():

    mark = 1500
    space = 1500

    plinit()

    z = zeros( (xpts, ypts) )
    w = zeros( (xpts, ypts) )

    for i in range(xpts):
	xx = (i - xpts/2) / (1.*(xpts/2))
	for j in range(ypts):
	    yy = (j - ypts/2) / (1.*(ypts/2)) - 1.
	    z[i,j] = xx * xx - yy * yy
	    w[i,j] = 2. * xx * yy

    plenv( 0., 1.*xpts, 0., 1.*ypts, 0, 0 )
    plcol(2)
    plcont2( z, 1, xpts, 1, ypts, clevel )

    plstyl( 1, mark, space )
    plcol(3)
    plcont2( w, 1, 1.*xpts, 1, 1.*ypts, clevel )
    plstyl( 0, mark, space )

    # other stuff

    plcol(1)
    pllab("X Coordinate", "Y Coordinate", "Streamlines of flow");

    pleop()
    plend()

main()
