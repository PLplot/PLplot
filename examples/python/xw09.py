from Numeric import *
from pl import *

XPTS = 35
YPTS = 46

def main():

    mark = 1500
    space = 1500
    
    clevel = arange(-1., 1., 0.2)

    xx = (arrayrange(XPTS) - XPTS/2) / float((XPTS/2))
    yy = (arrayrange(YPTS) - YPTS/2) / float((YPTS/2)) - 1.
    xx.shape = (-1,1)
    z = (xx*xx)-(yy*yy)
    # 2.*outerproduct(xx,yy) for new versions of Numeric which have outerproduct.
    w = 2.*xx*yy

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

    #restore defaults
    plcol0(1)

main()
