#!/usr/bin/env python

# plshade demo, using color fill.

import sys
import os

module_dir = "@MODULE_DIR@"

if module_dir[0] == '@':
	module_dir = os.getcwd ()

sys.path.insert (0, module_dir)

from Numeric import *
from pl import *

def main():
    ns = 20
    nx = 35
    ny = 46

    sh_cmap = 1
    min_color = 1; min_width = 0; max_color = 0; max_width = 0

    # Set up data array

    x = (arrayrange(nx) - .5*nx) / (nx/2.)
    x.shape = (-1,1)
    y = (arrayrange(ny) - .5*ny) / (ny/2.) - 1.
    zz = -sin(7.*x) * cos(7.*y) + x*x - y*y
    ww = -cos(7.*x) * sin(7.*y) + 2.*x*y
    
    zmin = min(zz.flat)
    zmax = max(zz.flat)

    #print "zmin=", zmin, " zmax=", zmax
    
    clevel = zmin + (zmax - zmin) * (arrayrange(ns)+.5)/ns

    # Build the 1-d coord arrays.

    distort = .4
    
    xx =  -1. + arrayrange(nx) *( 2./(nx-1))
    xg1 = xx + distort * cos( .5 * pi * xx )
    yy =  -1. + arrayrange(ny) * (2./(ny-1.))
    yg1 = yy - distort * cos( .5 * pi * yy )

    # Build the 2-d coord arrays.
    xx.shape = (-1,1)
    xg2 = xx + distort*cos((0.5*pi)*xx)*cos((0.5*pi)*yy)
    yg2 = yy - distort*cos((0.5*pi)*xx)*cos((0.5*pi)*yy)

    # Parse and process command line arguments

    plParseOpts(sys.argv, PARSE_FULL)

    # Initialize plplot

    plinit()
    # Plot using identity transform

    pladv(0)
    plvpor( .1, .9, .1, .9 )
    plwind( -1., 1., -1., 1. )

    for i in range(ns):
	shade_min = zmin + (zmax - zmin) * i / ns
	shade_max = zmin + (zmax - zmin) * (i+1) / ns
	sh_color = i/(ns-1.)
	sh_width = 2
	plpsty(0)
	plshade( zz, -1., 1., -1., 1.,
		   shade_min, shade_max, sh_cmap, sh_color, sh_width,
		   min_color, min_width, max_color, max_width, 1 )

    plcol0(1)
    plbox( "bcnst", 0., 0, "bcnstv", 0., 0 )
    plcol0(2)

    #    plcont(w, nx, ny, 1, nx, 1, ny, clevel, ns, mypltr, NULL);

    pllab( "distance", "altitude", "Bogon density" )

    pleop()

    # Plot using 1d coordinate transform

    pladv(0)
    plvpor( .1, .9, .1, .9 )
    plwind( -1., 1., -1., 1. )

    for i in range(ns):
	shade_min = zmin + (zmax - zmin) * i / ns
	shade_max = zmin + (zmax - zmin) * (i+1) / ns
	sh_color = i/ (ns-1.)
	sh_width=2
	plpsty(0)
	plshade( zz, -1., 1., -1., 1.,
		   shade_min, shade_max, sh_cmap, sh_color, sh_width,
		   min_color, min_width, max_color, max_width,
		   1, 'pltr1', xg1, yg1 )

    plcol0(1)
    plbox( "bcnst", 0.0, 0, "bcnstv", 0.0, 0 )
    plcol0(2)
	       
    #    plcont(w, nx, ny, 1, nx, 1, ny, clevel, ns, pltr1, (void *) &cgrid1);

    pllab( "distance", "altitude", "Bogon density" )

    pleop()

    # Plot using 2d coordinate transform

    pladv(0)
    plvpor( .1, .9, .1, .9 )
    plwind( -1., 1., -1., 1. )

    for i in range(ns):
	shade_min = zmin + (zmax - zmin) * i / ns
	shade_max = zmin + (zmax - zmin) * (i+1) / ns
	sh_color = i/ (ns-1.)
	sh_width=2
	plpsty(0)
	plshade( zz, -1., 1., -1., 1.,
		   shade_min, shade_max, sh_cmap, sh_color, sh_width,
		   min_color, min_width, max_color, max_width,
		   0, 'pltr2', xg2, yg2 )

    plcol0(1)
    plbox( "bcnst", 0.0, 0, "bcnstv", 0.0, 0 )
    plcol0(2)

#    plcont(w, nx, ny, 1, nx, 1, ny, clevel, ns, pltr2, (void *) &cgrid2);

    pllab( "distance", "altitude", "Bogon density, with streamlines" )
    pleop()

    # Do it again, but show wrapping support.

    pladv(0)
    plvpor( .1, .9, .1, .9 )
    plwind( -1., 1., -1., 1. )

    # Hold perimeter
    t = 2.*pi*arrayrange(100)/99.
    px = cos(t)
    py = sin(t)

    # We won't draw it now b/c it should be drawn /after/ the plshade stuff.

    # Now build the new coordinate matrices.
    
    r = arrayrange(nx)/(nx-1.)
    r.shape = (-1,1)
    t = (2.*pi/(ny-1.))*arrayrange(ny)
    xg = r*cos(t)
    yg = r*sin(t)
    z = exp(-r*r)*cos(5.*pi*r)*cos(5.*t)

    # Need a new clevel to go along with the new data set.

    zmin = min(z.flat)
    zmax = max(z.flat)
    clevel = zmin + ((zmax - zmin)/ns) * (arrayrange(ns)+.5)

    # Now we can shade the interior region.

    for i in range(ns):
	shade_min = zmin + (zmax - zmin) * i / ns
	shade_max = zmin + (zmax - zmin) * (i+1) / ns
	sh_color = i/ (ns-1.)
	sh_width=2
	plpsty(0)
	plshade( z, -1., 1., -1., 1.,
		   shade_min, shade_max, sh_cmap, sh_color, sh_width,
		   min_color, min_width, max_color, max_width,
		   0, 'pltr2', xg, yg, 2 )

    # Now we can draw the perimeter.

    plcol0(1)
    plline( px, py )

    # And label the plot.

    plcol0(2)
    pllab( "", "",  "Tokamak Bogon Instability" )
    plend()

main()
