#----------------------------------------------------------------------------
# PLplot Tcl demo #6
#
# $Id$
# $Log$
# Revision 1.3  1995/01/27 02:49:50  mjl
# New Tcl example files.  Can now be run from either pltcl -- using the
# "loopback" command as argument, or from plserver (or other plplot-aware
# extended wish) -- using the widget name as argument.
#
# Revision 1.2  1994/08/09  08:23:26  mjl
# Changed to new tclMatrix notation.
#
# Revision 1.1  1994/06/30  18:49:38  mjl
# Tcl demo programs, which fully reproduce their x??c counterpart.
#----------------------------------------------------------------------------

proc x06 {w} {

    matrix x f 1
    matrix y f 1

    $w cmd pladv
    $w cmd plfont 1

# Set up viewport and window

    $w cmd plcol 2
    $w cmd plvpor 0.1 1.0 0.1 0.9
    $w cmd plwind 0.0 1.0 0.0 1.3

# Draw the grid using plbox

    $w cmd plbox "bcgt" 0.1 0 "bcgt" 0.1 0

# Write the digits below the frame

    $w cmd plcol 15
    for {set i 0} {$i <= 9} {incr i} {
	$w cmd plmtex "b" 1.5 [expr 0.1 * $i + 0.05] 0.5 $i
    }

    set k 0
    for {set i 0} {$i <= 12} {incr i} {

# Write the digits to the left of the frame

	$w cmd plmtex "lv" 1.0 [expr 1.0 - (2 * $i + 1)/26.0] 1.0 [expr 10*$i]
	for {set j 0} {$j <= 9} {incr j} {
	    x 0 = [expr 0.1 * $j + 0.05]
	    y 0 = [expr 1.25 - 0.1 * $i]

# Display the symbols

	    if {$k < 128} {
		$w cmd plpoin 1 x y $k
	    }
	    incr k
	}
    }

    $w cmd plmtex "t" 1.5 0.5 0.5 "PLplot Example 6 - PLPOIN symbols"

    $w cmd pleop
}
