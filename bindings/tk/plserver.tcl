# $Id$
# $Log$
# Revision 1.1  1993/07/02 06:58:33  mjl
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
    
# First I change the default font.  To get a different font just modify the
# resource.  For example, if you want helvetica 240 instead, type:
#
# echo "plserver*font: -adobe-helvetica-medium-r-normal--*-240*" | xrdb -merge

    if { ! [ string compare "[option get . font Font]" "" ] } {
	option add *font -adobe-helvetica-medium-r-normal--*-180*
    }

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
	set geometry ${width}x${height}+200+0
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
	-command {null_command "About..."} \
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

    menubutton .menu.debug -text "Debug" -menu .menu.debug.m -underline 0
    menu .menu.debug.m

    .menu.debug.m add command \
	-label "Execute TCL command" \
	-command {evalCmd} \
	-underline 0

    pack append .menu .menu.debug {left}

#--------------
# Help menu
#--------------

    menubutton .menu.help -text "Help" -menu .menu.help.m -underline 0
    menu .menu.help.m

    .menu.help.m add command \
	-label "On Keys..." \
	-command {null_command "On Keys..."} \
	-underline 3

    .menu.help.m add command \
	-label "Tutorial..." \
	-command {null_command "Tutorial..."} \
	-underline 0

    pack append .menu .menu.help {right}

# Set up for keyboard-based menu traversal

    tk_menuBar .menu .menu.file .menu.options .menu.debug .menu.help
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
    destroy .
}
