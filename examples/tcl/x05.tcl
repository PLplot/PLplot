#----------------------------------------------------------------------------
# $Id$
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

    $w cmd plcol0 1
    $w cmd plhist $npts data -1.1 1.1 44 0

    $w cmd plcol0 2
    $w cmd pllab "#frValue" "#frFrequency" \
	"#frPLplot Example 5 - Probability function of Oscillator"

    $w cmd pleop
}
