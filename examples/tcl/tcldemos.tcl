#----------------------------------------------------------------------------
# PLplot TCL demos
# 
# Maurice LeBrun
# IFS, University of Texas at Austin
# 23-Jun-1994
#
# To plot these, start up pltcl and type plinit to get a selection of
# device types.  Choose one of the devices then type
# "source tcldemos.tcl", then type "1" for the first demo, "2" for the
# second, and so on.  Not all examples are fully implemented yet.
# Note if you use xwin for the device type, you must type CR in the
# xwin window to move from one plot to the next.
#
# Note: each demo proc is supplied a widget argument if run from a widget,
# or "loopback" if being run from pltcl.  In the latter case, the
# "loopback cmd" does nothing, but is required two make the two styles of
# Tcl scripts compatible.
#
# $Id$
# $Log$
# Revision 1.4  2000/12/19 01:51:51  airwin
# Improved self documentation of this script.
#
# Revision 1.3  1995/06/30 13:48:27  furnish
# Update loop limit to reflect the two new demo files.
#
# Revision 1.2  1995/04/24  21:27:24  mjl
# Removed the "loopback" parameter since it's no longer necessary.
#
# Revision 1.1  1995/01/27  02:48:03  mjl
# New front-end demo file for use from pltcl.  Uses new "loopback" command
# to run scripts also used with a plframe widget.
#
#----------------------------------------------------------------------------

for {set i 1} {$i <= 18} {incr i} {
    set demo x[format "%02d" $i]
    source $demo.tcl
    proc $i {} "$demo"
}
