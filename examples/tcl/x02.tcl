#----------------------------------------------------------------------------
# PLplot Tcl demo #2
#
# $Id$
# $Log$
# Revision 1.3  1995/04/12 21:12:09  mjl
# Minor nit.
#
# Revision 1.2  1995/01/27  02:49:47  mjl
# New Tcl example files.  Can now be run from either pltcl -- using the
# "loopback" command as argument, or from plserver (or other plplot-aware
# extended wish) -- using the widget name as argument.
#
# Revision 1.1  1994/06/30  18:49:34  mjl
# Tcl demo programs, which fully reproduce their x??c counterpart.
#----------------------------------------------------------------------------

proc x02 {w} {
    $w cmd plbop

# Divide screen into 16 regions

    $w cmd plssub 4 4
    $w cmd plschr 0.0 3.5
    $w cmd plfont 4

    for {set i 0} {$i < 16} {incr i} {
	$w cmd plcol $i
	$w cmd pladv
	set vmin 0.1
	set vmax 0.9
	for {set j 0} {$j <= 2} {incr j} {
	    $w cmd plwid [expr $j+1]
	    $w cmd plvpor $vmin $vmax $vmin $vmax
	    $w cmd plwind 0.0 1.0 0.0 1.0
	    $w cmd plbox "bc" 0.0 0 "bc" 0.0 0
	    set vmin [expr $vmin+0.1]
	    set vmax [expr $vmax-0.1]
	}
	$w cmd plwid 1
	$w cmd plptex 0.5 0.5 1.0 0.0 0.5 $i
    }
    $w cmd pleop
}
