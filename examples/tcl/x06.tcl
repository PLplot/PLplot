# $Id$
# $Log$
# Revision 1.2  1994/08/09 08:23:26  mjl
# Changed to new tclMatrix notation.
#
# Revision 1.1  1994/06/30  18:49:38  mjl
# Tcl demo programs, which fully reproduce their x??c counterpart.
#
#----------------------------------------------------------------------------
# PLplot Tcl demo #6
#
# After sourcing, just type "6".
#----------------------------------------------------------------------------

proc 6 {} {

    matrix x f 1
    matrix y f 1

    pladv
    plfont 1

# Set up viewport and window

    plcol 2
    plvpor 0.1 1.0 0.1 0.9
    plwind 0.0 1.0 0.0 1.3

# Draw the grid using plbox

    plbox "bcgt" 0.1 0 "bcgt" 0.1 0

# Write the digits below the frame

    plcol 15
    for {set i 0} {$i <= 9} {incr i} {
	plmtex "b" 1.5 [expr 0.1 * $i + 0.05] 0.5 $i
    }

    set k 0
    for {set i 0} {$i <= 12} {incr i} {

# Write the digits to the left of the frame

	plmtex "lv" 1.0 [expr 1.0 - (2 * $i + 1)/26.0] 1.0 [expr 10*$i]
	for {set j 0} {$j <= 9} {incr j} {
	    x 0 = [expr 0.1 * $j + 0.05]
	    y 0 = [expr 1.25 - 0.1 * $i]

# Display the symbols

	    if {$k < 128} {
		plpoin 1 x y $k
	    }
	    incr k
	}
    }

    plmtex "t" 1.5 0.5 0.5 "PLPLOT Example 6 - PLPOIN symbols"

    pleop
}
