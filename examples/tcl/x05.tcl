#----------------------------------------------------------------------------
# PLplot Tcl demo #5
#
# $Id$
# $Log$
# Revision 1.4  1995/04/24 21:28:15  mjl
# Made the widget name default to "loopback" for easier use from pltcl.
#
# Revision 1.3  1995/01/27  02:49:49  mjl
# New Tcl example files.  Can now be run from either pltcl -- using the
# "loopback" command as argument, or from plserver (or other plplot-aware
# extended wish) -- using the widget name as argument.
#
# Revision 1.2  1994/08/09  08:23:25  mjl
# Changed to new tclMatrix notation.
#
# Revision 1.1  1994/06/30  18:49:37  mjl
# Tcl demo programs, which fully reproduce their x??c counterpart.
#----------------------------------------------------------------------------

proc x05 {{w loopback}} {
    set npts 2047
    matrix data f $npts

# Fill up data points

    $w cmd plssub 1 1
    $w cmd plbop

    set delta [expr 2.0 * 3.141592654 / $npts]
    for {set i 0} {$i < $npts} {incr i} {
	data $i = [expr sin($i * $delta)]
    }

    $w cmd plcol 1
    $w cmd plhist $npts data -1.1 1.1 44 0

    $w cmd plcol 2
    $w cmd pllab "#frValue" "#frFrequency" \
	"#frPLplot Example 5 - Probability function of Oscillator"

    $w cmd pleop
}
