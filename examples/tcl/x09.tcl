#----------------------------------------------------------------------------
# PLplot Tcl demo #9
#
# $Id$
# $Log$
# Revision 1.6  1995/07/04 21:51:18  furnish
# Tcl demos need to set the color before doing any drawing since they
# may not be the first thing run, and so the color on entry to a Tcl
# demo might be something other than 1.
#
# Revision 1.5  1995/07/04  19:40:48  furnish
# Added plot to demonstrate the Tcl support for contouring of data
# defined on wrapped coords.
#
# Revision 1.4  1995/07/04  18:58:29  furnish
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

    $w cmd plcol 1
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

    x09_polar $w
}

# Demonstrate plotting of wrapped data.  What is significant to
# understand about this example is that for the common case of
# plotting polar data (or other forms of coordinates that wrap on
# themselves) you can do it from Tcl /without/ having to go to the
# trouble to construct a special data plotting matrix with an extra
# row or column and then copy the data into it, replicating the first
# row/col into the extra row/col.

proc x09_polar {{w loopback}} {

    $w cmd plenv -1 1 -1 1 0 -2
    $w cmd plcol 1

# Hold perimeter
    matrix px f 100; matrix py f 100

    for {set i 0} {$i < 100} {incr i} {
	set t [expr 2. * 3.1415927 * $i / 99.]
	px $i = [expr cos($t)]
	py $i = [expr sin($t)]
    }

    $w cmd plline 100 px py

    set xpts 40; set ypts 40
    matrix xg f $xpts $ypts
    matrix yg f $xpts $ypts
    matrix z f $xpts $ypts

    for {set i 0} {$i < $xpts} {incr i} {
	set r [expr $i / ($xpts - 1.)]
	for {set j 0} {$j < $ypts} {incr j} {
	    set t [expr 2. * 3.1415927 * $j / ($ypts - 1.)]

	    xg $i $j = [expr $r * cos($t)]
	    yg $i $j = [expr $r * sin($t)]

	    z $i $j = $r
	}
    }

    matrix lev f 10 = { .05, .15, .25, .35, .45, .55, .65, .75, .85, .95 }

    $w cmd plcol 2
    $w cmd plcont z lev pltr2 xg yg 2
#                                   ^-- :-).  Means: "2nd coord is wrapped."

    $w cmd plcol 1
    $w cmd pllab "" "" "Polar Contour Plot"
}
