# $Id$
# $Log$
# Revision 1.1  1994/06/30 18:49:34  mjl
# Tcl demo programs, which fully reproduce their x??c counterpart.
#
#----------------------------------------------------------------------------
# PLplot Tcl demo #2
#
# After sourcing, just type "2".
#----------------------------------------------------------------------------

proc 2 {} {

    plbop

# Divide screen into 16 regions

    plssub 4 4
    plschr 0.0 3.5
    plfont 4

    for {set i 0} {$i < 15} {incr i} {
	plcol $i
	pladv
	set vmin 0.1
	set vmax 0.9
	for {set j 0} {$j <= 2} {incr j} {
	    plwid [expr $j+1]
	    plvpor $vmin $vmax $vmin $vmax
	    plwind 0.0 1.0 0.0 1.0
	    plbox "bc" 0.0 0 "bc" 0.0 0
	    set vmin [expr $vmin+0.1]
	    set vmax [expr $vmax-0.1]
	}
	plwid 1
	plptex 0.5 0.5 1.0 0.0 0.5 $i
    }
    pleop
}
