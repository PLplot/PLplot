#----------------------------------------------------------------------------
# PLplot Tcl demo #7
#
# $Id$
# $Log$
# Revision 1.5  1995/05/06 21:34:06  mjl
# Changed comment syntax to "the one true way".
#
# Revision 1.4  1995/04/24  21:28:17  mjl
# Made the widget name default to "loopback" for easier use from pltcl.
#
# Revision 1.3  1995/01/27  02:49:51  mjl
# New Tcl example files.  Can now be run from either pltcl -- using the
# "loopback" command as argument, or from plserver (or other plplot-aware
# extended wish) -- using the widget name as argument.
#
# Revision 1.2  1994/08/09  08:23:27  mjl
# Changed to new tclMatrix notation.
#
# Revision 1.1  1994/06/30  18:49:39  mjl
# Tcl demo programs, which fully reproduce their x??c counterpart.
#----------------------------------------------------------------------------

proc x07 {{w loopback}} {

    matrix base i 17 = \
	{0, 200, 500, 600, 700, 800, 900, \
	     2000, 2100, 2200, 2300, 2400, 2500, 2600, 2700, 2800, 2900}

    matrix x f 1
    matrix y f 1

    $w cmd plfontld 1
    for {set l 0} {$l < 17} {incr l} {
	$w cmd pladv

    # Set up viewport and window

	$w cmd plcol 2
	$w cmd plvpor 0.15 0.95 0.1 0.9
	$w cmd plwind 0.0 1.0 0.0 1.0

    # Draw the grid using plbox

	$w cmd plbox "bcgt" 0.1 0 "bcgt" 0.1 0

    # Write the digits below the frame

	$w cmd plcol 15
	for {set i 0} {$i <= 9} {incr i} {
	    $w cmd plmtex "b" 1.5 [expr 0.1 * $i + 0.05] 0.5 $i
	}

	set k 0
	for {set i 0} {$i <= 9} {incr i} {

	# Write the digits to the left of the frame

	    $w cmd plmtex "lv" 1.0 [expr (0.95 - 0.1 * $i)] 1.0 \
		[expr [base $l] + 10*$i]

	    for {set j 0} {$j <= 9} {incr j} {
		x 0 = [expr 0.1 * $j + 0.05]
		y 0 = [expr 0.95 - 0.1 * $i]

	    # Display the symbols

		$w cmd plsym 1 x y [expr [base $l] + $k]
		incr k
	    }
	}
	$w cmd plmtex "t" 1.5 0.5 0.5 "PLplot Example 7 - PLSYM symbols"
    }
}
