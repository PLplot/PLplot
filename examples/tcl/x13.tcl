#----------------------------------------------------------------------------
# PLplot Tcl demo #13
#
# $Id$
# $Log$
# Revision 1.4  1995/07/01 21:43:44  furnish
# Initial implementation, converted C example program code to Tcl.
#
# Revision 1.3  1995/04/24  21:28:21  mjl
# Made the widget name default to "loopback" for easier use from pltcl.
#
# Revision 1.2  1995/01/27  02:49:56  mjl
# New Tcl example files.  Can now be run from either pltcl -- using the
# "loopback" command as argument, or from plserver (or other plplot-aware
# extended wish) -- using the widget name as argument.
#
# Revision 1.1  1994/06/30  18:50:19  mjl
# Tcl demo programs, just a template for now.
#----------------------------------------------------------------------------

# Does a simple pie chart.

proc x13 {{w loopback}} {

    set text0 "Maurice"
    set text1 "Randy"
    set text2 "Mark"
    set text3 "Steve"
    set text4 "Warner"

#    int i, j;
#    PLFLT dthet, theta0, theta1, theta, just, dx, dy;
#    static PLFLT x[500], y[500], per[5];

    matrix x f 500
    matrix y f 500
    matrix per f 5 = {10., 32., 12., 30., 16.}

    $w cmd plenv 0. 10. 0. 10. 1 -2
    $w cmd plcol 2

    set theta0 0.;
    set pi 3.1415927

    set dthet [expr 2. * $pi / 500.]
    for {set i 0} {$i <= 4} {incr i} {
	set j 0;
	x $j = 5.;
	y $j = 5.;
	incr j

	set theta1 [expr $theta0 + 2. * $pi * [per $i] / 100.]
	if {$i == 4} {
	    set theta1 [expr 2. * $pi]
	}

	for {set theta $theta0} {$theta <= $theta1} {
	    set theta [expr $theta + $dthet]} {
	    x $j = [expr 5. + 3. * cos($theta) ]
	    y $j = [expr 5. + 3. * sin($theta) ]
	    incr j
	}

	$w cmd plcol [expr $i + 1]
	$w cmd plpsty [expr (($i + 3) % 8 + 1)]
	$w cmd plfill $j x y
	$w cmd plcol 1
	$w cmd plline $j x y
	set just [expr ($theta0 + $theta1) / 2.]
	set dx [expr .25 * cos($just)]
	set dy [expr .25 * sin($just)]
	if {$just < $pi / 2. || $just > 3. * $pi / 2.} {
	    set just 0.
	} else {
	    set just 1.
	}

	$w cmd plptex [expr [x [expr $j / 2]] + $dx] \
	    [expr [y [expr $j / 2]] + $dy] 1.0 0.0 $just [set text$i]
	set theta0 [expr $theta - $dthet]
    }
    $w cmd plfont 2
    $w cmd plschr 0. 1.3
    $w cmd plptex 5.0 9.0 1.0 0.0 0.5 "Percentage of Sales"
}
