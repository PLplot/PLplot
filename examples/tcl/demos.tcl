# $Id$
# $Log$
# Revision 1.1  1994/06/23 22:39:30  mjl
# New demo file for "pltcl" interpreter.  Needs fleshing out.
#
#----------------------------------------------------------------------------
# PLPLOT TCL demos
# 
# Maurice LeBrun
# IFS, University of Texas at Austin
# 23-Jun-1994
#
# To plot these, start up pltcl and select the desired device type.  Type
# "source demos.tcl", then type "1" for the first demo, "2" for the
# second, and so on.
#----------------------------------------------------------------------------

proc 1 {} {
    1a
}

proc 1a {} {
    plcol 1
    plenv 0 1 0 1 0 0
    plcol 6
    pllab "(x)" "(y)" "#frPLplot Example 1 - y=1-2x+2x#u2"

    matrix x1 float 101
    matrix y1 float 101

    for {set i 0} {$i < 101} {incr i} {
	set x [expr $i * .01]
	$x1 $i = $x
	$y1 $i = [expr 1 - 2 * $x + 2 * $x * $x]
    }

    plline 101 $x1 $y1
}
