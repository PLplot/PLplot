#----------------------------------------------------------------------------
# Demo polar grid plotter
#
# $Id$
# $Log$
# Revision 1.2  2000/12/29 05:59:51  airwin
# Improve the self-documentation.
#
# Revision 1.1  1995/05/06 21:32:03  mjl
# Demo proc for plotting a polar grid.

# To run this demo execute pltcl.  Then from within that environment
# execute "plinit" (which gives you a choice of drivers),
# "source plgrid.tcl" (this file), then finally "plgrid"
#
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
