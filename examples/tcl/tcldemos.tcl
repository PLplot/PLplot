#----------------------------------------------------------------------------
# PLplot TCL demos
# 
# Maurice LeBrun
# IFS, University of Texas at Austin
# 23-Jun-1994
#
# To plot these, start up pltcl and select the desired device type.  Type
# "source tcldemos.tcl", then type "1" for the first demo, "2" for the
# second, and so on.  Not all examples are fully implemented yet.
#
# Note: each demo proc is supplied a widget argument if run from a widget,
# or "loopback" if being run from pltcl.  In the latter case, the
# "loopback cmd" does nothing, but is required two make the two styles of
# Tcl scripts compatible.
#
# $Id$
# $Log$
# Revision 1.2  1995/04/24 21:27:24  mjl
# Removed the "loopback" parameter since it's no longer necessary.
#
# Revision 1.1  1995/01/27  02:48:03  mjl
# New front-end demo file for use from pltcl.  Uses new "loopback" command
# to run scripts also used with a plframe widget.
#
#----------------------------------------------------------------------------

for {set i 1} {$i <= 16} {incr i} {
    set demo x[format "%02d" $i]
    source $demo.tcl
    proc $i {} "$demo"
}
