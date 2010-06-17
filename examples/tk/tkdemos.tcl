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

plstdwin .
plxframe .plw
pack append . .plw {left expand fill}

for {set i 1} {$i <= 31} {incr i} {
    set demo x[format "%02d" $i]
    source $demo.tcl
    proc $i {} "
        .plw.plwin cmd plspause 0
        $demo .plw.plwin
        .plw.plwin cmd plcol0 1
        .plw.plwin cmd plsori 0
        .plw.plwin cmd plspal0 cmap0_default.pal
        .plw.plwin cmd plspal1 cmap1_default.pal 1
        .plw.plwin cmd plstransform NULL
    "
}
