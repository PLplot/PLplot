# $Id$
# $Log$
# Revision 1.13  1994/05/07 03:11:35  mjl
# Ripped out code dependent on tk3.2 since it finally started to get in the
# way.  Also improved some documentation.
#
# Revision 1.12  1994/04/25  18:57:07  mjl
# Removed the rename of open since now we need it for reading/writing
# palette files (I hope everyone's security is up to par by now).  Deleted
# options menu -- now these are associated with the plframe widget.  Added
# shift and control attributes to key_filter as well as speeded up
# scrolling.  Shift+cursor gives the fast setting and Shift+Control+cursor
# gives the faster setting.
#
# Revision 1.11  1994/04/08  11:57:12  mjl
# Now the default resources are first loaded, then the user's customizations,
# if present.  A keystroke filtering proc was added to allow binding useful
# GUI operations to keystrokes.  The defaults are pretty reasonable, set
# in the pldefaults proc (and should be modified in plconfig).  Supported
# operations are "z" to zoom, "r" to reset zoom, "p" to print, "s" to save
# again, <Right>, <Left>, <Up>, <Down>, to scroll by cursor key while in
# zoom mode (units scrolled is user alterable as well).
#
# Revision 1.10  1994/01/15  17:43:21  mjl
# Added procs to handle communication link.  plserver_link_init gets invoked
# automatically when plserver is started by the Tcl/TK/DP driver.
# plserver_start is used when running plserver in stand-alone mode.
#
# Revision 1.9  1993/12/21  10:22:43  mjl
# Now route ALL communication to the client through the client_cmd proc.
# This part reworked to not suck when using Tcl-DP.
#
# Revision 1.8  1993/12/15  09:17:57  mjl
# Changes to support Tcl-DP and an atexit tweak.
#
# Revision 1.7  1993/12/09  20:34:45  mjl
# Added code to tell client to abort when exit is selected.

#----------------------------------------------------------------------------
# PLPLOT TK/TCL graphics renderer menu procs
# Maurice LeBrun
# 26-Apr-1993
# IFS, University of Texas at Austin
#
# Includes some code borrowed from the TCL/TK widget demo.
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
# plserver_init
# Create the main window, menus, etc.
#----------------------------------------------------------------------------

proc plserver_init {} {

# Set up configuration options.
# The first is to hold default values of everything, the second is for
# user customization.  See pldefaults.tcl for more info.

    pldefaults
    plconfig

# I refuse to allow exec's, ever.
# Open's have to remain, however, to read/write palette info.

    rename exec {}

# Create the main window
# Use the default window title.

    set root_width  [winfo vrootwidth .] 
    set root_height [winfo vrootheight .]

    wm minsize . 300 240
    wm maxsize . [expr "$root_width/64*63"] [expr "$root_height/64*62"]

# Set window geometry if not already set.
# Depart from square slightly to account for menu bar.

    if { ! [ info exists geometry ] } {
	set width  [expr "$root_width / 16 * 10"]
	set height [expr "$root_height / 16 * 11"]
	set geometry ${width}x${height}
    }
    wm geometry . $geometry

# Create the window for the menu bar

    frame .menu -relief raised -borderwidth 1
    pack append . .menu {top fillx}

#------------
# File menu
#------------

    menubutton .menu.file -text "File" -menu .menu.file.m -underline 0
    menu .menu.file.m

#    .menu.file.m add command \
#	-label "Open" \
#	-command {null_command "Open"} \
#	-underline 0

    .menu.file.m add command \
	-label "About..." \
	-command about \
	-underline 0

#    if { ! [ info exists child ] } {
#	.menu.file.m add command \
#	    -label "Launch client..." \
#	    -command {null_command "Launch client..."} \
#	    -underline 0
#    }

    .menu.file.m add separator

    .menu.file.m add command \
	-label "Exit" \
	-command {exit_app} \
	-underline 0

    bind . <Control-x> {exit_app}

    pack append .menu .menu.file {left}

#--------------
# Debug menu
#--------------

    if {[info exists debug_on]} {
	menubutton .menu.debug -text "Debug" -menu .menu.debug.m -underline 0
	menu .menu.debug.m

	.menu.debug.m add command \
	    -label "Execute TCL command" \
	    -command evalCmd \
	    -underline 0

	pack append .menu .menu.debug {left}
    }

#--------------
# Help menu
#--------------

    menubutton .menu.help -text "Help" -menu .menu.help.m -underline 0
    menu .menu.help.m

    .menu.help.m add command \
	-label "On Tcl/TK..." \
	-command help_tcltk \
	-underline 3

    .menu.help.m add command \
	-label "On GUI..." \
	-command help_gui \
	-underline 3

    .menu.help.m add command \
	-label "On Keys..." \
	-command help_keys \
	-underline 3

    pack append .menu .menu.help {right}

# Set up for keyboard-based menu traversal

    tk_menuBar .menu .menu.file .menu.options .menu.help
    tk_bindForTraversal . .menu

    focus default .
    update
}

#----------------------------------------------------------------------------
# exit_app
#
# Destroys main window and does any other cleanup necessary.
# Eventually this will have to be completely redone to handle multiple
# client widgets.
#----------------------------------------------------------------------------

proc exit_app {} {
    global client

    if { [ info exists client ] } {
	client_cmd $client "unset server"
	client_cmd $client "abort"
	update
    }
    after 1 exit
}

#----------------------------------------------------------------------------
# key_filter
#
# Front-end to key handler.
# For supported operations it's best to modify the global key variables
# to get the desired action.  More advanced stuff can be done with the
# $user_key_filter proc.  Find anything particularly useful?  Let me know,
# so it can be added to the default behavior.
#----------------------------------------------------------------------------

proc key_filter {w client k n a shift control} {
    global user_key_filter

    global key_zoom_select
    global key_zoom_reset
    global key_print
    global key_save_again
    global key_scroll_right
    global key_scroll_left
    global key_scroll_up
    global key_scroll_down
    global key_scroll_slow
    global key_scroll_fast
    global key_scroll_faster

#    puts "keypress: $k $n $a"

    if {[info exists user_key_filter]} then {
	$user_key_filter $w $client $k $n $a
    }

    if {$shift} then {
	if {$control} then {
	    set s $key_scroll_faster
	} else {
	    set s $key_scroll_fast
	}
    } else {
	set s $key_scroll_slow
    }

    switch $k \
	$key_zoom_select	"plw_zoom_select $w" \
	$key_zoom_reset		"plw_zoom_reset $w" \
	$key_print		"plw_print $w" \
	$key_save_again		"plw_save $w" \
	$key_scroll_right	"plw_view_scroll $w  $s  0" \
	$key_scroll_left	"plw_view_scroll $w -$s  0" \
	$key_scroll_up		"plw_view_scroll $w  0 -$s" \
	$key_scroll_down	"plw_view_scroll $w  0  $s" 

    client_cmd [list $client] "keypress $k $n $a"
}

#----------------------------------------------------------------------------
# client_cmd
#
# Send string to client.  Does it in the background and catches errors
# (if client is busy it won't respond).
#
# The first "after 1" ensures the command is issued in the server's
# background, so that we always continue processing events.  This is
# important for handshaking and for good performance.
#
# The second "after 1" (or the dp_RDO if Tcl-DP rpc is used) ensures that
# the client process doesn't try to send a reply.  Also good for
# performance but more importantly in case the server has already terminated.
#----------------------------------------------------------------------------

proc client_cmd {client msg} {
    global dp

    if { $dp } {
	after 1 catch [list "dp_RDO [list $client] $msg"]
    } else {
	after 1 catch [list "send [list $client] after 1 $msg"]
    }
}

#----------------------------------------------------------------------------
# plserver_link_init
#
# Set up initial link to client.
#----------------------------------------------------------------------------

proc plserver_link_init {} {
    global dp client

    if { $dp } {
	global client_host client_port server_host server_port

	if { ! [ info exists client_host ] } {
	    set client_host localhost
	}

	set server_host [host_id]
	set server_port [dp_MakeRPCServer]
	set client [dp_MakeRPCClient $client_host $client_port]

	dp_RDO [list $client] set server_host $server_host
	dp_RDO [list $client] set server_port $server_port
	dp_RDO [list $client] set client [list $client]

    } else {
	global client_name server_name

	set server_name [winfo name .]
	set client $client_name

	send $client "set server_name [list $server_name]"
	send $client "set client [list $client]"
    }

}

#----------------------------------------------------------------------------
# plserver_start
#
# Startup proc when client is run independently.
#----------------------------------------------------------------------------

proc plserver_start {{use_dp 0}} {
    global dp client

    plserver_init

    set dp $use_dp

    if { $dp } {
	global client_host client_port server_host server_port

	set server_host [host_id]
	set server_port [dp_MakeRPCServer]

	puts stderr "Please start client with flags: "
	puts stderr "  -server_host $server_host -server_port $server_port"

	wait_until {[info exists client_port]}
	set client "[dp_MakeRPCClient $client_host $client_port]"
	dp_RDO [list $client] set client [list $client]

    } else {
	global client_name server_name

	puts stderr "Please start client with flags: "
	puts stderr "  -server_name $server_name"
	tkwait variable client_name

	set client $client_name
	send $client "set client [list $client]"
    }
}
