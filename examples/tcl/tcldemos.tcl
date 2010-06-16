#----------------------------------------------------------------------------
# $Id$
#
# PLplot TCL demos
#
# Maurice LeBrun
# IFS, University of Texas at Austin
# 23-Jun-1994
#
# To plot these, start up pltcl with the argument "standard_examples".
# Choose one of the devices to start the demo. Press the space bar
# to move to the next window or to the next example
#
# Note: each demo proc is supplied a widget argument if run from a widget,
# or "loopback" if being run from pltcl.  In the latter case, the
# "loopback cmd" does nothing but is required to make the two styles of
# Tcl scripts compatible.
#----------------------------------------------------------------------------

for {set i 1} {$i <= 31} {incr i} {
    set demo x[format "%02d" $i]
    source $demo.tcl
    # restore defaults
    proc $i {} "
        $demo
        loopback cmd pleop
        loopback cmd plcol0 1
        loopback cmd plsori 0
        loopback cmd plspal0 cmap0_default.pal
        loopback cmd plspal1 cmap1_default.pal 1
        loopback cmd plstransform NULL
    "
}
