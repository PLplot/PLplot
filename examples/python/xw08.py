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

    for k in range(4):
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
	plot3d(x, y, z, opt[k], 1)
	plcol0(3)
	plmtex("t", 1.0, 0.5, 0.5, title[k])

    pllightsource(1., 1., 1.)
    #set up gray scale.
    rr = arrayrange(256)
    gg = rr
    bb = rr
    plscmap1(rr, gg, bb)
    
    for k in range(4):
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
	plotsh3d(x, y, z, 0)
	plcol0(3)
	plmtex("t", 1.0, 0.5, 0.5, title[k])


    # Restore defaults
    plcol0(1)
	
main()
