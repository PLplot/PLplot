#----------------------------------------------------------------------------
# $Id$
#
# Demo polar grid plotter
#----------------------------------------------------------------------------

proc plgrid {{w loopback}} {
    set ni 10
    set nj 20
    set nj1 [expr $nj + 1]

    matrix xi f $ni
    matrix yi f $ni
    matrix xj f $nj1
    matrix yj f $nj1

    set dr     [expr 1. / $ni]
    set dtheta [expr 2. * 3.141592654 / $nj]

# Set up viewport and window, but do not draw box 

    $w cmd plssub 1 1
    $w cmd plcol 1
    $w cmd plenv -1.3 1.3 -1.3 1.3 1 -2

# Draw i-lines

    for {set i 0} {$i < $ni} {incr i} {
	for {set j 0} {$j < $nj1} {incr j} {
	    set r     [expr $i * $dr]
	    set theta [expr $j * $dtheta]
	    set psi   [expr $theta + 0.5 * $r * sin($theta)]

	    xj $j = [expr $r * cos($psi)]
	    yj $j = [expr $r * sin($psi)]
	}
	$w cmd plline $nj1 xj yj
    }

# Draw j-lines

    for {set j 0} {$j < $nj} {incr j} {
	for {set i 0} {$i < $ni} {incr i} {
	    set r     [expr $i * $dr]
	    set theta [expr $j * $dtheta]
	    set psi   [expr $theta + 0.5 * $r * sin($theta)]

	    xi $i = [expr $r * cos($psi)]
	    yi $i = [expr $r * sin($psi)]
	}
	$w cmd plline $ni xi yi
    }
}
