#!/usr/bin/env python

#	contour plot demo.

import sys
import os

module_dir = "@MODULE_DIR@"

if module_dir[0] == '@':
	module_dir = os.getcwd ()

sys.path.insert (0, module_dir)

from Numeric import *
from pl import *

XPTS = 35
YPTS = 46

def main():

    mark = 1500
    space = 1500
    
    clevel = arange(-1., 1., 0.2)

    # Parse and process command line arguments

    plParseOpts(sys.argv, PARSE_FULL)

    # Initialize plplot

    plinit()

    xx = (arrayrange(XPTS) - XPTS/2) / float((XPTS/2))
    yy = (arrayrange(YPTS) - YPTS/2) / float((YPTS/2)) - 1.
    z = transpose(resize(xx*xx,(len(yy),len(xx))))-(yy*yy)
    # 2.*outerproduct(xx,yy) for new versions of Numeric which have outerproduct.
    w = 2.*transpose(resize(xx,(len(yy),len(xx))))*yy

    plenv( 0., 1.*(XPTS-1), 0., 1.*(YPTS-1), 0, 0 )
    plcol0(2)
    plcont( z, clevel )
    plstyl( 1, mark, space )
    plcol0(3)
    plcont( w, clevel )
    plstyl( 0, mark, space )

    # other stuff

    plcol0(1)
    pllab("X Coordinate", "Y Coordinate", "Streamlines of flow");

    pleop()
    plend()

main()
