# PLXWin.tcl
# Geoffrey Furnish
# 10 June 1994
#
# @> [incr Tcl] interface to PLplot
#
# $Id$
#
# $Log$
# Revision 1.2  1994/10/04 18:18:47  mjl
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

	#puts "running PLXWin ctor"

	# The following code copied over from plxframe in
	# plwidget.tcl.  Now if I could just get Maurice to forget
	# aobut non itcl stuff altogether ...

	set client {}

	# Make frame for top row widgets.
	# plframe widget must already have been created (the plframe
	# is queried for a list of the valid output devices for page
	# dumps).

	plw_create_TopRow $this $client
	pack append $this $this.ftop {top fill}

	# Initialize plplot widget
	# Enable keyboard traversal when widget has the input focus.
	# Also grab the initial input focus.

	tk_bindForTraversal $this.plwin
	focus $this.plwin

	# Set up zoom windows list

	global zidx zidx_max zxl zyl zxr zyr

	set zidx($this) 0
	set zidx_max($this) 0
	set zxl($this,0) 0.0
	set zyl($this,0) 0.0
	set zxr($this,0) 1.0
	set zyr($this,0) 1.0

	# Bindings
	#
	# Note: it is necessary here to protect the $client variable
	# from becoming two tokens if it has embedded spaces, such as
	# occurs when you have multiple copies running.  The [list
	# $client] construct will enclose $client with a pair of
	# braces if necessary (can't do it directly since braces
	# prevent variable expansion).  The reason this is necessary
	# is because binding the command to a key causes it to be
	# parsed by the interpreter twice -- once during the bind and
	# once during its execution.

	bind $this.plwin <Any-KeyPress> \
	    "key_filter $this [list $client] %K %N %A 0 0"
	
	bind $this.plwin <Shift-KeyPress> \
	    "key_filter $this [list $client] %K %N %A 1 0"

	bind $this.plwin <Control-KeyPress> \
	    "key_filter $this [list $client] %K %N %A 0 1"

	bind $this.plwin <Shift-Control-KeyPress> \
	    "key_filter $this [list $client] %K %N %A 1 1"

	bind $this.plwin <Any-ButtonPress> \
	    "plw_user_mouse $this [list $client] %b %s %x %y"

	bind $this.plwin <Any-Enter> \
	    "focus $this.plwin"

    }
}
