#----------------------------------------------------------------------------
# $Id$
#----------------------------------------------------------------------------

# Does a series of 3-d plots for a given data set, with different
# viewing options in each plot.

proc x08 {{w loopback}} {

    matrix opt i 4 = {1, 2, 3, 3}
    matrix alt f 4 = {60.0, 20.0, 60.0, 60.0}
    matrix az  f 4 = {30.0, 60.0, 120.0, 160.0}

    set xpts 35
    set ypts 46
    set n_col 256
    set two_pi [expr 2.0 * 3.14159265358979323846 ]

    matrix x f $xpts
    matrix y f $ypts
    matrix z f $xpts $ypts
    matrix rr i $n_col
    matrix gg i $n_col
    matrix bb i $n_col

    for {set i 0} {$i < $xpts} {incr i} {
	x $i = [expr ($i - ($xpts/2)) / double($xpts/2) ]
    }

    for {set i 0} {$i < $ypts} {incr i} {
	y $i = [expr ($i - ($ypts/2)) / double($ypts/2) ]
    }

    for {set i 0} {$i < $xpts} {incr i} {
	set xx [x $i]
	for {set j 0} {$j < $ypts} {incr j} {
	    set yy [y $j]
	    set r [expr sqrt( $xx * $xx + $yy * $yy ) ]

	    z $i $j = [expr exp(-$r * $r) * cos( $two_pi * $r ) ]
	}
    }
    $w cmd pllightsource 1. 1. 1.
    for {set k 0} {$k < $n_col} {incr k} {
       rr $k = [expr $k]
       gg $k = [expr $k]
       bb $k = [expr $k]
    }
    $w cmd  plscmap1 rr gg bb $n_col
    for {set k 0} {$k < 4} {incr k} {
       for {set ifshade 0} {$ifshade < 2} {incr ifshade} {
  	  $w cmd pladv 0
  	  $w cmd plvpor 0.0 1.0 0.0 0.9
  	  $w cmd plwind -1.0 1.0 -0.9 1.1
  	  $w cmd plcol0 1
  	  $w cmd plw3d 1.0 1.0 1.0 -1.0 1.0 -1.0 1.0 -1.0 1.0 [alt $k] [az $k]
  	  $w cmd plbox3 "bnstu" "x axis" 0.0 0 \
	    "bnstu" "y axis" 0.0 0 \
	    "bcdmnstuv" "z axis" 0.0 0
  	  $w cmd plcol0 2
	  if {$ifshade == 1} {
	     $w cmd plotsh3d x y z 0
	  } else {
	     $w cmd plot3d x y z [opt $k] 1
	  }
  	  $w cmd plcol0 3
  	  set title [format "#frPLplot Example 8 - Alt=%.0f, Az=%.0f, Opt=%d" \
	    [alt $k] [az $k] [opt $k] ]
  	  $w cmd plmtex "t" 1.0 0.5 0.5 $title
       }
    }

# Restore defaults
    $w cmd plcol0 1

}
