#----------------------------------------------------------------------------
# PLplot Tcl demo #9
#
# $Id$
# $Log$
# Revision 1.4  1995/07/04 18:58:29  furnish
# First real implementation of this Tcl demo.  No way to exactly
# reproduce the first plot from x09c.c, since no Tcl support for user
# defined coordinate transformation functions.  However the second and
# third come out exactly right.  Need to embellish this demo to show off
# the Tcl support for contouring of data defined on wrapped grids.
#
# Revision 1.3  1995/04/24  21:28:18  mjl
# Made the widget name default to "loopback" for easier use from pltcl.
#
# Revision 1.2  1995/01/27  02:49:53  mjl
# New Tcl example files.  Can now be run from either pltcl -- using the
# "loopback" command as argument, or from plserver (or other plplot-aware
# extended wish) -- using the widget name as argument.
#
# Revision 1.1  1994/06/30  18:50:15  mjl
# Tcl demo programs, just a template for now.
#----------------------------------------------------------------------------

# Contour plot demo.

proc x09 {{w loopback}} {

    set xpts 35
    set ypts 46

    matrix clevel f 11 = {-1., -.8, -.6, -.4, -.2, 0, .2, .4, .6, .8, 1.}

    matrix mark  i 1 = { 1500 }
    matrix space i 1 = { 1500 }

    matrix zz f $xpts $ypts
    matrix ww f $xpts $ypts

# Calculate the data matricies.

    for {set i 0} {$i < $xpts} {incr i} {
	set xx [expr ($i - ($xpts / 2.)) / ($xpts / 2.) ]
	for {set j 0} {$j < $ypts} {incr j} {
	    set yy [expr ($j - ($ypts / 2.)) / ($ypts / 2.) - 1.0 ]
	    zz $i $j = [expr $xx * $xx - $yy * $yy ]
	    ww $i $j = [expr 2. * $xx * $yy ]
	}
    }

    matrix xg1 f $xpts
    matrix yg1 f $ypts

    matrix xg2 f $xpts $ypts
    matrix yg2 f $xpts $ypts

    set pi 3.1415927
    set distort .4

# Build the 1-d coord arrays.

    for {set i 0} {$i < $xpts} {incr i} {
	set xx [expr -1. + $i * ( 2. / ($xpts-1.) )]
	xg1 $i = [expr $xx + $distort * cos( .5 * $pi * $xx ) ]
    }

    for {set j 0} {$j < $ypts} {incr j} {
	set yy [expr -1. + $j * ( 2. / ($ypts-1.) )]
	yg1 $j = [expr $yy - $distort * cos( .5 * $pi * $yy ) ]
    }

# Build the 2-d coord arrays.

    for {set i 0} {$i < $xpts} {incr i} {
	set xx [expr -1. + $i * ( 2. / ($xpts-1.) )]
	for {set j 0} {$j < $ypts} {incr j} {
	    set yy [expr -1. + $j * ( 2. / ($ypts-1.) )]

	    set argx [expr .5 * $pi * $xx]
	    set argy [expr .5 * $pi * $yy]

	    xg2 $i $j = [expr $xx + $distort * cos($argx) * cos($argy) ]
	    yg2 $i $j = [expr $yy - $distort * cos($argx) * cos($argy) ]
	}
    }

# No way to make the first plot in x09c.c, b/c can't do the mypltr
# thing from Tcl.  So here we just do a pltr0 thing, which will be
# similar except for different bounds in world coordinates.

    $w cmd plenv 0 [expr $xpts-1] 0 [expr $ypts-1] 0 0
    $w cmd plcol 2
    $w cmd plcont zz clevel

    $w cmd plstyl 1 mark space
    $w cmd plcol 3
    $w cmd plcont ww clevel

    $w cmd plstyl 0 mark space
    $w cmd plcol 1
    $w cmd pllab "X Coordinate" "Y Coordinate" "Streamlines of flow"

# Plot using 1d coordinate transform

    $w cmd plenv -1.0 1.0 -1.0 1.0 0 0
    $w cmd plcol 2
    $w cmd plcont zz clevel pltr1 xg1 yg1

    $w cmd plstyl 1 mark space
    $w cmd plcol 3
    $w cmd plcont ww clevel pltr1 xg1 yg1

    $w cmd plstyl 0 mark space
    $w cmd plcol 1
    $w cmd pllab "X Coordinate" "Y Coordinate" "Streamlines of flow"

# Plot using 2d coordinate transform

    $w cmd plenv -1.0 1.0 -1.0 1.0 0 0
    $w cmd plcol 2
    $w cmd plcont zz clevel pltr2 xg2 yg2

    $w cmd plstyl 1 mark space
    $w cmd plcol 3
    $w cmd plcont ww clevel pltr2 xg2 yg2

    $w cmd plstyl 0 mark space
    $w cmd plcol 1
    $w cmd pllab "X Coordinate" "Y Coordinate" "Streamlines of flow"
}
