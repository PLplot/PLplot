#----------------------------------------------------------------------------
# $Id$
#----------------------------------------------------------------------------

proc x03 {{w loopback}} {
    set twopi  [expr 2. * 3.141592654]

# Set up viewport and window, but do not draw box 

    $w cmd plssub 1 1
    $w cmd plcol0 1
    $w cmd plenv -1.3 1.3 -1.3 1.3 1 -2

# Draw circles for polar grid

    set ni 10
    set nj 45
    set nj1 [expr $nj + 1]

    set dr     [expr 1. / $ni]
    set dtheta [expr $twopi / $nj]

    matrix xj f $nj1
    matrix yj f $nj1

    for {set i 1} {$i <= $ni} {incr i} {
	for {set j 0} {$j < $nj1} {incr j} {
	    set r     [expr $i * $dr]
	    set theta [expr $j * $dtheta]
	    xj $j = [expr $r * cos($theta)]
	    yj $j = [expr $r * sin($theta)]
	}
	$w cmd plline $nj1 xj yj
    }

# Draw radial spokes for polar grid and write labels for angle

    $w cmd plcol0 2
    for {set j 0} {$j <= 11} {incr j} {
	set theta [expr $j * $twopi / 12.]
	set xg [expr cos($theta)]
	set yg [expr sin($theta)]
	$w cmd pljoin 0.0 0.0 $xg $yg

	if {$xg >= 0} {
	    set dx $xg
	    set dy $yg
	    set just -0.15
	} else {
	    set dx [expr -$xg]
	    set dy [expr -$yg]
	    set just 1.15
	}
	set label [expr round($theta*360./$twopi)]

	$w cmd plptex $xg $yg $dx $dy $just $label
    }

# Draw the graph 

    set npts 180
    set npts1 [expr $npts+1]

    set dtheta [expr $twopi / $npts]

    matrix x f $npts1
    matrix y f $npts1

    for {set j 0} {$j <= $npts} {incr j} {
	set theta [expr $j * $dtheta]
	set r     [expr sin(5 * $theta)]
	x $j = [expr $r * cos($theta)]
	y $j = [expr $r * sin($theta)]
    }
    $w cmd plcol0 3
    $w cmd plline $npts1 x y

    $w cmd plcol0 4
    $w cmd plmtex "t" 2.0 0.5 0.5 "#frPLplot Example 3 - r(#gh)=sin 5#gh"

    $w cmd pleop
}
