#----------------------------------------------------------------------------
# $Id$
#----------------------------------------------------------------------------

proc x02 {{w loopback}} {

# Divide screen into 16 regions

    # For starting from scratch this call to pladv increments cursub, but 
    # then the following plssub sets it to zero so the whole thing is 
    # essentially a nop.  However, for the case when other examples are run 
    # first, this call to pladv is absolutely essential to finish the 
    # preceding page.
    $w cmd pladv 0
    $w cmd plssub 4 4
    $w cmd plschr 0.0 3.5
    $w cmd plfont 4

    for {set i 0} {$i < 16} {incr i} {
	$w cmd plcol0 $i
	$w cmd pladv 0
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

# Restore defaults
    $w cmd plssub 1 1
    $w cmd plfont 1
    $w cmd plcol0 1
    $w cmd pleop
}
