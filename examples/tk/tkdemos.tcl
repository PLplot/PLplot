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

# Exclude 31 because it does not work well in this context and because
# the required x31.tcl is normally not copied to the tk directory in any case.
for {set i 1} {$i <= 30} {incr i} {
    set demo x[format "%02d" $i]
    source $demo.tcl
    proc $i {} "$demo .plw.plwin"
}
