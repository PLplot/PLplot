# $Id$
# $Log$
# Revision 1.2  1994/07/19 22:36:49  mjl
# Sped up some by eliminating half of the points.
#
# Revision 1.1  1994/06/30  18:49:35  mjl
# Tcl demo programs, which fully reproduce their x??c counterpart.
#
#----------------------------------------------------------------------------
# PLplot Tcl demo #3
#
# After sourcing, just type "3".
#----------------------------------------------------------------------------

proc 3 {} {
    set npts 180
    set npts1 [expr $npts+1]

    matrix x0 f $npts1
    matrix y0 f $npts1
    matrix x f $npts1
    matrix y f $npts1

    set dtr [expr 3.141592654 / 180.]
    set dtheta [expr $dtr * 360. / $npts]

    for {set i 0} {$i <= $npts} {incr i} {
	$x0 $i = [expr cos($dtheta * $i)]
	$y0 $i = [expr sin($dtheta * $i)]
    }

# Set up viewport and window, but do not draw box 

    plssub 1 1
    plcol 1
    plenv -1.3 1.3 -1.3 1.3 1 -2
    for {set i 1} {$i <= 10} {incr i} {
	for {set j 0} {$j <= $npts} {incr j} {
	    $x $j = [expr 0.1 * $i * [$x0 $j]]
	    $y $j = [expr 0.1 * $i * [$y0 $j]]
	}

# Draw circles for polar grid

	plline $npts1 $x $y
    }

    plcol 2
    for {set i 0} {$i <= 11} {incr i} {
	set theta [expr 30.0 * $i]
	set xg [expr cos($dtr * $theta)]
	set yg [expr sin($dtr * $theta)]

# Draw radial spokes for polar grid

	pljoin 0.0 0.0 $xg $yg

# Write labels for angle

	if {$xg >= 0} {
	    set dx $xg
	    set dy $yg
	    set just -0.15
	} else {
	    set dx [expr -$xg]
	    set dy [expr -$yg]
	    set just 1.15
	}
	plptex $xg $yg $dx $dy $just [expr round($theta)]
    }

# Draw the graph 

    for {set i 0} {$i <= $npts} {incr i} {
	set r [expr sin($dtheta * (5 * $i))]
	$x $i = [expr [$x0 $i] * $r]
	$y $i = [expr [$y0 $i] * $r]
    }
    plcol 3
    plline $npts1 $x $y

    plcol 4
    plmtex "t" 2.0 0.5 0.5 "#frPLplot Example 3 - r(#gh)=sin 5#gh"

    pleop
}
