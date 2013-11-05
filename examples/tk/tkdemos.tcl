#----------------------------------------------------------------------------
# $Id$
#
# PLplot TK demos
#
# Maurice LeBrun
# IFS, University of Texas at Austin
# 26-Jan-1995
#
# To plot these, start up plserver.  Type "source tkdemos.tcl", at which
# point a plframe extended widget is created.  Then type "1" for the first
# demo, "2" for the second, and so on.
#
# Note: each demo proc is supplied a widget argument if run from a widget,
# or "loopback" if being run from pltcl.  In the latter case, the
# "loopback cmd" does nothing, but is required two make the two styles of
# Tcl scripts compatible.
#----------------------------------------------------------------------------

proc sourceUtf8 {sourceFile} {

    if { [encoding system] != "utf-8" } {
        if { [package vsatisfies [info patch] 8.5] } {

            source -encoding utf-8 $sourceFile

        } else {

            #
            # Pre-Tcl 8.5
            #
            set infile [open $sourceFile]
            set contents [read $infile]
            close $infile
            eval [encoding convertfrom utf-8 \
                [encoding convertto [encoding system] $contents]]
        }
    } else {
        source $sourceFile
    }
}

set  utf8_examples {4 18 24 26 33}

# This is hacked logic for distinguishing between the plserver
# and wish cases which requires an extra directory be lappended in
# the build tree for the Pltk case.  FIXME.
if {[catch {package require Plplotter}]} {
    # use 'plserver' method
    plstdwin .
    plxframe .plw
    set plwin .plw.plwin
} else {
    plframe .plw
    set plwin .plw
}
pack append . .plw {left expand fill}

for {set i 0} {$i <= 33} {incr i} {
    if {$i != 32} {
	set demo x[format "%02d" $i]

	#
	# If the source code contains UTF-8 characters (beyond the
	# ASCII-7 encoding), take special measures
	#
	
	if { [lsearch $utf8_examples $i] < 0 } {
	    source $demo.tcl
	} else {
	    sourceUtf8 $demo.tcl
	}
	
	proc $i {} "
            global plwin
            $plwin cmd plspause 0
            $demo $plwin
            $plwin cmd plcol0 1
            $plwin cmd plsori 0
            $plwin cmd plspal0 cmap0_default.pal
            $plwin cmd plspal1 cmap1_default.pal 1
            $plwin cmd plstransform NULL
        "
    }
}
