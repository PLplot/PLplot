#----------------------------------------------------------------------------
# PLplot Tcl demo #1
#
# $Id$
# $Log$
# Revision 1.5  1995/04/24 21:28:13  mjl
# Made the widget name default to "loopback" for easier use from pltcl.
#
# Revision 1.4  1995/01/27  02:49:45  mjl
# New Tcl example files.  Can now be run from either pltcl -- using the
# "loopback" command as argument, or from plserver (or other plplot-aware
# extended wish) -- using the widget name as argument.
#
# Revision 1.3  1994/08/09  08:23:22  mjl
# Changed to new tclMatrix notation.
#
# Revision 1.2  1994/06/30  18:47:52  mjl
# Minor label and commenting changes.
#
# Revision 1.1  1994/06/25  20:34:24  mjl
# Demo program 1.  Now fully reproduces x01c plots.
#----------------------------------------------------------------------------

proc x01 {{w loopback}} {
    global xscale yscale xoff yoff

    $w cmd plssub 2 2

# First plot

    set xscale 6.0
    set yscale 1.0
    set xoff 0.0
    set yoff 0.0

    plot1 $w

# Second

    set xscale 1.0
    set yscale 0.0014
    set yoff   0.0185

    $w cmd plsyax 5
    plot1 $w

# Third

    plot2 $w

# Fourth

    plot3 $w

# An eop is nice here.

    $w cmd pleop
}

# This is supposed to work just like the plot1() in x01c.c

proc plot1 {w} {
    global xscale yscale xoff yoff

    set npts 60
    matrix x f $npts
    matrix y f $npts

    for {set i 0} {$i < $npts} {incr i} {
	x $i = [expr $xoff + $xscale * ($i + 1) / $npts]
	y $i = [expr $yoff + $yscale * pow([x $i],2)]
    }

    set xmax [x [expr $npts-1]]
    set ymax [y [expr $npts-1]]

    matrix x1 f 6
    matrix y1 f 6

    for {set i 0} {$i < 6} {incr i} {
	set j [expr $i*10+3]
	x1 $i = [x $j]
	y1 $i = [y $j]
    }

    $w cmd plcol 1
    $w cmd plenv $xoff $xmax $yoff $ymax 0 0
    $w cmd plcol 6
    $w cmd pllab "(x)" "(y)" "#frPLplot Example 1 - y=x#u2"

    # plot the data points

    $w cmd plcol 9
    $w cmd plpoin 6 x1 y1 9

    # draw the line through the data

    $w cmd plcol 4
    $w cmd plline $npts x y
}

# This is supposed to work just like the plot2() in x01c.c

proc plot2 {w} {
    $w cmd plcol 1
    $w cmd plenv -2 10 -.4 1.2 0 1
    $w cmd plcol 2
    $w cmd pllab "(x)" "sin(x)/x" "#frPLplot Example 1 - Sinc Function"

    # Fill up the array

    matrix x1 f 101
    matrix y1 f 101

    for {set i 0} {$i < 101} {incr i} {
	set x [expr ($i - 19.)/6.]
	x1 $i = $x
	y1 $i = 1
	if {$x != 0} { y1 $i = [expr sin($x)/$x] }
    }

    $w cmd plcol 3
    $w cmd plline 101 x1 y1
}

# This is supposed to work just like the plot3() in x01c.c

proc plot3 {w} {

    $w cmd pladv
    $w cmd plvsta
    $w cmd plwind 0.0 360.0 -1.2 1.2

# Draw a box with ticks spaced 60 degrees apart in X, and 0.2 in Y.

    $w cmd plcol 1
    $w cmd plbox "bcnst" 60.0 2 "bcnstv" 0.2 2

# Superimpose a dashed line grid, with 1.5 mm marks and spaces. 
# plstyl expects two integer matrices for mark and space!

    matrix mark i 1
    matrix space i 1

    mark 0 = 1500
    space 0 = 1500
    $w cmd plstyl 1 mark space

    $w cmd plcol 2
    $w cmd plbox "g" 30.0 0 "g" 0.2 0

    mark 0 = 0
    space 0 = 0
    $w cmd plstyl 0 mark space

    $w cmd plcol 3
    $w cmd pllab "Angle (degrees)" "sine" "#frPLplot Example 1 - Sine function"

    matrix x f 101
    matrix y f 101

    for {set i 0} {$i < 101} {incr i} {
	x $i = [expr 3.6 * $i]
	y $i = [expr sin([x $i] * 3.141592654 / 180.0)]
    }

    $w cmd plcol 4
    $w cmd plline 101 x y
}
