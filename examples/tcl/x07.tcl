# $Id$
# $Log$
# Revision 1.1  1994/06/30 18:49:39  mjl
# Tcl demo programs, which fully reproduce their x??c counterpart.
#
#----------------------------------------------------------------------------
# PLplot Tcl demo #7
#
# After sourcing, just type "7".
#----------------------------------------------------------------------------

proc 7 {} {

    matrix base i 17 = \
	{0, 200, 500, 600, 700, 800, 900, \
	     2000, 2100, 2200, 2300, 2400, 2500, 2600, 2700, 2800, 2900}

    matrix x f 1
    matrix y f 1

    plfontld 1
    for {set l 0} {$l < 17} {incr l} {
	pladv

# Set up viewport and window

	plcol 2
	plvpor 0.15 0.95 0.1 0.9
	plwind 0.0 1.0 0.0 1.0

# Draw the grid using plbox

	plbox "bcgt" 0.1 0 "bcgt" 0.1 0

# Write the digits below the frame

	plcol 15
	for {set i 0} {$i <= 9} {incr i} {
	    plmtex "b" 1.5 [expr 0.1 * $i + 0.05] 0.5 $i
	}

	set k 0
	for {set i 0} {$i <= 9} {incr i} {

# Write the digits to the left of the frame

	    plmtex "lv" 1.0 [expr (0.95 - 0.1 * $i)] 1.0 \
		[expr [$base $l] + 10*$i]

	    for {set j 0} {$j <= 9} {incr j} {
		$x 0 = [expr 0.1 * $j + 0.05]
		$y 0 = [expr 0.95 - 0.1 * $i]

# Display the symbols

		plsym 1 $x $y [expr [$base $l] + $k]
		incr k
	    }
	}
	plmtex "t" 1.5 0.5 0.5 "PLPLOT Example 7 - PLSYM symbols"
    }
}
