#----------------------------------------------------------------------------
# $Id$
#----------------------------------------------------------------------------

# Does a series of mesh plots for a given data set, with different
# viewing options in each plot.

proc x11 {{w loopback}} {

    matrix opt i 4 = {1, 2, 3, 3}
    matrix alt f 4 = {60.0, 20.0, 60.0, 60.0}
    matrix az  f 4 = {30.0, 60.0, 120.0, 160.0}

    set xpts 35
    set ypts 46
    set two_pi [expr 2.0 * 3.1415926535897932384 ]

    matrix x f $xpts
    matrix y f $ypts
    matrix z f $xpts $ypts

    for {set i 0} {$i < $xpts} {incr i} {
	x $i = [expr ($i - ($xpts/2.)) / ($xpts/2.) ]
    }

    for {set i 0} {$i < $ypts} {incr i} {
	y $i = [expr ($i - ($ypts/2.)) / ($ypts/2.) ]
    }

    for {set i 0} {$i < $xpts} {incr i} {
	set xx [x $i]
	for {set j 0} {$j < $ypts} {incr j} {
	    set yy [y $j]
	    z $i $j = [expr cos($two_pi * $xx) * sin($two_pi * $yy) ]
	}
    }

    for {set k 0} {$k < 4} {incr k} {
	$w cmd pladv 0
	$w cmd plcol0 1
	$w cmd plvpor 0.0 1.0 0.0 0.8
	$w cmd plwind -1.0 1.0 -1.0 1.5

	$w cmd plw3d 1.0 1.0 1.2 -1.0 1.0 -1.0 1.0 -1.5 1.5 [alt $k] [az $k]
	$w cmd plbox3 "bnstu" "x axis" 0.0 0 \
	    "bnstu" "y axis" 0.0 0 \
	    "bcdmnstuv" "z axis" 0.0 4

	$w cmd plcol0 2

# You may use either of the next two lines.
#	$w cmd plmesh x y z $xpts $ypts [opt $k]
	$w cmd plmesh x y z [opt $k]

	$w cmd plcol0 3

	set title [format "#frPLplot Example 11.%d - Alt=%f, Az=%f, Opt=%d" \
		       $k [alt $k] [az $k] [opt $k] ]
	$w cmd plmtex "t" 1.0 0.5 0.5 $title
    }
}
