# $Id$
# $Log$
# Revision 1.1  1994/06/30 18:49:37  mjl
# Tcl demo programs, which fully reproduce their x??c counterpart.
#
#----------------------------------------------------------------------------
# PLplot Tcl demo #5
#
# After sourcing, just type "5".
#----------------------------------------------------------------------------

proc 5 {} {
    set npts 2047
    matrix data f $npts

# Fill up data points

    plssub 1 1
    plbop

    set delta [expr 2.0 * 3.141592654 / $npts]
    for {set i 0} {$i < $npts} {incr i} {
	$data $i = [expr sin($i * $delta)]
    }

    plcol 1
    plhist $npts $data -1.1 1.1 44 0

    plcol 2
    pllab "#frValue" "#frFrequency" \
	"#frPLplot Example 5 - Probability function of Oscillator"

    pleop
}
