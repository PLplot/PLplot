# PLXWin.tcl
# Geoffrey Furnish
# 10 June 1994
#
# @> [incr Tcl] interface to PLplot
#
# $Id$
#
# $Log$
# Revision 1.7  1995/06/01 21:22:22  mjl
# Removed a lot of the miscellaneous init garbage, now handled by
# plw_setup_defaults.
#
# Revision 1.6  1995/05/06  17:08:13  mjl
# Shave and a haircut.
#
# Revision 1.5  1995/04/12  08:00:30  mjl
# Eliminated use of client variable and fixed plw_create_TopRow syntax.
#
# Revision 1.4  1995/03/23  21:07:21  mjl
# Fix to fix of key bindings.
#
# Revision 1.3  1995/03/23  20:46:09  mjl
# Fixed key bindings.
#
# Revision 1.2  1994/10/04  18:18:47  mjl
# Inserted initialization of zoom windows lists.  Gross, but it should
# work.
#
# Revision 1.1  1994/06/10  20:43:57  furnish
# New extended plframe itcl widget.
#
###############################################################################

itcl_class PLXWin {
    inherit PLWin

    constructor {config} {

	PLWin::constructor

    # Set up defaults

	plw_setup_defaults $this

    # Make frame for top row widgets.  plframe widget must already have been
    # created (the plframe is queried for a list of the valid output devices
    # for page dumps).

	plw_create_TopRow $this
	pack append $this $this.ftop {top fill}

    # Initialize plplot widget.  Enable keyboard traversal when widget has
    # the input focus.  Also grab the initial input focus.

	tk_bindForTraversal $this.plwin
	focus $this.plwin
    }
}
