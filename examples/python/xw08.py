from Numeric import *
from pl import *

XPTS = 35		# Data points in x
YPTS = 46		# Data points in y

opt = [1, 2, 3, 3]

alt = [60.0, 20.0, 60.0, 60.0]

az = [30.0, 60.0, 120.0, 160.0]

title = ["#frPLplot Example 8 - Alt=60, Az=30, Opt=1",
	 "#frPLplot Example 8 - Alt=20, Az=60, Opt=2",
	 "#frPLplot Example 8 - Alt=60, Az=120, Opt=3",
	 "#frPLplot Example 8 - Alt=60, Az=160, Opt=3"]

# Routine for restoring colour map1 to default.
# See static void plcmap1_def(void) in plctrl.c for reference.
def restore_cmap1():
    # For center control points, pick black or white, whichever is closer to bg 
    # Be careful to pick just short of top or bottom else hue info is lost
    vertex = sum(array(plgcolbg()))/(3.*255.)
    if vertex < 0.5:
	vertex = 0.01
	midpt = 0.10
    else:
	vertex = 0.99
	midpt = 0.90
    # Independent variable of control points.
    i = array((0., 0.44, 0.50, 0.50, 0.56, 1.))
    # Hue for control points.  Blue-violet to red
    h = array((260., 260., 260., 0., 0., 0.))
    # Lightness ranging from medium to vertex to medium
    l = array((0.5, midpt, vertex, vertex, midpt, 0.5))
    # Saturation is complete for default
    s = array((1., 1., 1., 1., 1., 1.))
    # Integer flag array is zero (no interpolation along far-side of colour
    # figure.)  Not used in python plplot API, but soon will be changed.
    rev = array((0, 0, 0, 0, 0, 0))
    # Default number of cmap1 colours
    plscmap1n(128)
    # Interpolate between control points to set up default cmap1.
    plscmap1l(0, i, h, l, s)

# Routine for initializing color map 1 in HLS space.
# Basic grayscale variation from half-dark (which makes more interesting
# looking plot compared to dark) to light.
def cmap1_init():
    # Independent variable of control points.
    i = array((0., 1.))
    # Hue for control points.  Blue-violet to red
    h = array((0., 0.))
    # Lightness ranging from half-dark (for interest) to light.
    l = array((0.5, 1.))
    # Gray scale has zero saturation
    s = array((0., 0.))
    # Integer flag array is zero (no interpolation along far-side of colour
    # figure.)  Not used in python plplot API, but soon will be changed.
    rev = array((0, 0))
    # number of cmap1 colours is 256
    plscmap1n(256)
    # Interpolate between control points to set up cmap1.
    plscmap1l(0, i, h, l, s)

# main
#
# Does a series of 3-d plots for a given data set, with different
# viewing options in each plot.

def main():

    x = (arrayrange(XPTS) - (XPTS / 2)) / float(XPTS / 2)
    y = (arrayrange(YPTS) - (YPTS / 2)) / float(YPTS / 2)
    x.shape = (-1,1)
    r2 = (x*x) + (y*y)
    x.shape = (-1,)
    z = exp(-r2)*cos((2.0*pi)*sqrt(r2))

    pllightsource(1., 1., 1.)
    #set up modified gray scale.
    cmap1_init()
    for k in range(4):
	for ifshade in range(2):
	    pladv(0)
	    plvpor(0.0, 1.0, 0.0, 0.9)
	    plwind(-1.0, 1.0, -0.9, 1.1)
	    plcol0(1)
	    plw3d(1.0, 1.0, 1.0, -1.0, 1.0, -1.0, 1.0, -1.0, 1.0,
	    alt[k], az[k])
	    plbox3("bnstu", "x axis", 0.0, 0,
	    "bnstu", "y axis", 0.0, 0,
	    "bcdmnstuv", "z axis", 0.0, 0)
	    
	    plcol0(2)
	    if ifshade == 1:
		plotsh3d(x, y, z, 0)
	    else:
		plot3d(x, y, z, opt[k], 1)
	    plcol0(3)
	    plmtex("t", 1.0, 0.5, 0.5, title[k])

    # Restore defaults
    plcol0(1)
    restore_cmap1()
	
main()
