# $Id$
# $Log$
# Revision 1.7  1993/12/09 20:34:45  mjl
# Added code to tell client to abort when exit is selected.
#
# Revision 1.6  1993/09/08  18:39:14  mjl
# Added global cascade_arrow variable to fake cascade menu arrows under
# Tk 3.2 (Tk 3.3 has default ones that are much nicer).
#
# Revision 1.5  1993/09/01  14:51:05  mjl
# Removed window specification for top level TK driver window.  This might
# have had something to do with the window coming up on the wrong screen
# under tvtwm, and there appears to be no problem under VUE this way
# (although at one time I did have problems, dunno why).
#
# Revision 1.4  1993/08/03  20:29:27  mjl
# Put security hole plug in a more useful spot.
#
# Revision 1.3  1993/07/31  08:05:49  mjl
# Enabled help menu entries; split off resource initialization into another
# file for more flexibility.
#
# Revision 1.2  1993/07/16  22:03:15  mjl
# Inserted hack to partially plug TK's security problem.  Also hard-coded
# font selection temporarily (see explanation within).  Made debug menu
# normally invisible.
#
# Revision 1.1  1993/07/02  06:58:33  mjl
# The new TCL/TK driver!  Yes it's finally here!  YAAAAAAAAYYYYYYY!!!
#

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

    global cascade_arrow tkVersion

# Set up configuration options.

    plconfig

# Hack to plug TK's gaping security hole.  This is a minimal set.  
#
# Note: if you want to do some fancy autoload stuff, the open command will
# be necessary.  It is a real security risk to keep it in, however.  You
# should probably autoload or source all the files you want procs from
# before this point is reached.

    rename exec {}
    rename open {}

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

# Set a variable to fake menu cascade arrows for TK 3.2 and earlier

    if { $tkVersion < 3.3 } { 
	set cascade_arrow "=>" 
    } else {
	set cascade_arrow "" 
    }
    
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
# Options menu
#--------------

    menubutton .menu.options -text "Options" -menu .menu.options.m -underline 0
    menu .menu.options.m

    .menu.options.m add command \
	-label "Palette 0" \
	-command {null_command "Palette 0"} \
	-underline 8

    .menu.options.m add command \
	-label "Palette 1" \
	-command {null_command "Palette 1"} \
	-underline 8

    .menu.options.m add separator

    .menu.options.m add command \
	-label "Load Configuration" \
	-command {null_command "Load Configuration"} \
	-underline 0

    .menu.options.m add command \
	-label "Save Configuration" \
	-command {null_command "Save Configuration"} \
	-underline 0

    .menu.options.m add command \
	-label "Save Configuration As..." \
	-command {null_command "Save Configuration As..."} \
	-underline 19

    pack append .menu .menu.options {left}

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
#----------------------------------------------------------------------------

proc exit_app {} {
    global client
    if { [ info exists client ] } {
	send $client "after 1 abort"
	unset client
    }
    destroy .
}
