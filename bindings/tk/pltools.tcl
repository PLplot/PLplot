# $Id$
# $Log$
# Revision 1.1  1993/07/02 06:58:34  mjl
# The new TCL/TK driver!  Yes it's finally here!  YAAAAAAAAYYYYYYY!!!
#

#----------------------------------------------------------------------------
# PLPLOT TK/TCL graphics renderer support procs
# Maurice LeBrun
# 1-Jul-1993
# IFS, University of Texas at Austin
#
# Includes some code borrowed from the TCL/TK widget demo.
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
# null_command
#
# Invokes a dialog explaining that the real binding isn't written yet.
#----------------------------------------------------------------------------

proc null_command {cmd_name} {
    set dialog_args "-text {Command \"$cmd_name\" not yet implemented.} \
		     -aspect 500 -justify left"
    mkDialog .modal $dialog_args {OK {}}
    dpos .modal
    tkwait visibility .modal
    grab .modal
    tkwait window .modal
}

#----------------------------------------------------------------------------
# dpos w
#
# Position a dialog box at a reasonable place on the screen.
#----------------------------------------------------------------------------

proc dpos w {
    wm geometry $w +300+300
}

#----------------------------------------------------------------------------
# mkDialog w msgArgs list list ...
#
# Create a dialog box with a message and any number of buttons at
# the bottom.
#
# Arguments:
#    w -	Name to use for new top-level window.
#    msgArgs -	List of arguments to use when creating the message of the
#		dialog box (e.g. text, justifcation, etc.)
#    list -	A two-element list that describes one of the buttons that
#		will appear at the bottom of the dialog.  The first element
#		gives the text to be displayed in the button and the second
#		gives the command to be invoked when the button is invoked.
#----------------------------------------------------------------------------

proc mkDialog {w msgArgs args} {
    catch {destroy $w}
    toplevel $w -class Dialog
    wm title $w "Dialog box"
    wm iconname $w "Dialog"

    # Create two frames in the main window. The top frame will hold the
    # message and the bottom one will hold the buttons.  Arrange them
    # one above the other, with any extra vertical space split between
    # them.

    frame $w.top -relief raised -border 1
    frame $w.bot -relief raised -border 1
    pack append $w $w.top {top fill expand} $w.bot {top fill expand}
    
    # Create the message widget and arrange for it to be centered in the
    # top frame.
    
    eval message $w.top.msg -justify center $msgArgs
    pack append $w.top $w.top.msg {top expand padx 10 pady 10}
    
    # Create as many buttons as needed and arrange them from left to right
    # in the bottom frame.  Embed the left button in an additional sunken
    # frame to indicate that it is the default button, and arrange for that
    # button to be invoked as the default action for clicks and returns in
    # the dialog.

    if {[llength $args] > 0} {
	set arg [lindex $args 0]
	frame $w.bot.0 -relief sunken -border 1
	pack append $w.bot $w.bot.0 {left expand padx 20 pady 20}
	button $w.bot.0.button -text [lindex $arg 0] \
		-command "[lindex $arg 1]; destroy $w"
	pack append $w.bot.0 $w.bot.0.button {expand padx 12 pady 12}
	bind $w <Return> "[lindex $arg 1]; destroy $w"
	focus $w

	set i 1
	foreach arg [lrange $args 1 end] {
	    button $w.bot.$i -text [lindex $arg 0] \
		    -command "[lindex $arg 1]; destroy $w"
	    pack append $w.bot $w.bot.$i {left expand padx 20}
	    set i [expr $i+1]
	}
    }
    bind $w <Any-Enter> [list focus $w]
    focus $w
}

#----------------------------------------------------------------------------
# evalCmd w
#
# Create a top-level window containing a text widget that allows you
# to enter a TCL command and have it executed.
#
# Arguments:
#    w -	Name to use for new top-level window.
#----------------------------------------------------------------------------

proc evalCmd {{w .eval}} {
    catch {destroy $w}
    toplevel $w -geometry 400x300
    dpos $w
    wm title $w "Interpret command"
    wm iconname $w "Interpret"

    frame $w.cmd
    label $w.cmd.label -text "Command:" -width 13 -anchor w
    entry $w.cmd.entry -width 40 -relief sunken -bd 2 -textvariable command
    button $w.cmd.button -text "Execute" \
	    -command "eval \$command"
    pack append $w.cmd $w.cmd.label left $w.cmd.entry left \
	    $w.cmd.button {left pady 10 padx 20}
    bind $w.cmd.entry <Return> "eval \$command"

    text $w.t -relief raised -bd 2 -setgrid true
    $w.t insert 0.0 {\
Type TCL command to be executed in the window above, then type <Return>
or click on "Execute".  
}
    $w.t mark set insert 0.0
    bind $w <Any-Enter> "focus $w.cmd.entry"

    button $w.ok -text OK -command "destroy $w"

    pack append $w $w.cmd {top fill} \
	    $w.ok {bottom fillx} $w.t {expand fill}
}


#------------------------------------------------------------------------------
# Proc to set up debug bindings.
#------------------------------------------------------------------------------

proc dbug_bind {w} {

bind $w <Any-ButtonPress>	{puts stderr "Widget event: ButtonPress"}
bind $w <Any-ButtonRelease>	{puts stderr "Widget event: ButtonRelease"}
bind $w <Any-Circulate>		{puts stderr "Widget event: Circulate"}
bind $w <Any-CirculateRequest>	{puts stderr "Widget event: CirculateRequest"}
bind $w <Any-Colormap>		{puts stderr "Widget event: Colormap"}
bind $w <Any-Configure>		{puts stderr "Widget event: Configure"}
bind $w <Any-ConfigureRequest>	{puts stderr "Widget event: ConfigureRequest"}
bind $w <Any-Destroy>		{puts stderr "Widget event: Destroy"}
bind $w <Any-Enter>		{puts stderr "Widget event: Enter"}
bind $w <Any-Expose> 		{puts stderr "Widget event: Expose"}
bind $w <Any-FocusIn>		{puts stderr "Widget event: FocusIn"}
bind $w <Any-FocusOut>		{puts stderr "Widget event: FocusOut"}
bind $w <Any-Gravity>		{puts stderr "Widget event: Gravity"}
bind $w <Any-Keymap>		{puts stderr "Widget event: Keymap"}
bind $w <Any-KeyPress>		{puts stderr "Widget event: KeyPress"}
bind $w <Any-KeyRelease>	{puts stderr "Widget event: KeyRelease"}
bind $w <Any-Leave>		{puts stderr "Widget event: Leave"}
bind $w <Any-Map>		{puts stderr "Widget event: Map"}
bind $w <Any-MapRequest>	{puts stderr "Widget event: MapRequest"}
#bind $w <Any-Motion>		{puts stderr "Widget event: Motion"}
bind $w <Any-Property>		{puts stderr "Widget event: Property"}
bind $w <Any-Reparent>		{puts stderr "Widget event: Reparent"}
bind $w <Any-ResizeRequest>	{puts stderr "Widget event: ResizeRequest"}
bind $w <Any-Unmap>		{puts stderr "Widget event: Unmap"}
bind $w <Any-Visibility>	{puts stderr "Widget event: Visibility"}

}
