# $Id$
# $Log$
# Revision 1.2  1993/07/16 22:04:02  mjl
# Added several utility procs, mostly for getting info or confirmation from
# the user.
#
# Revision 1.1  1993/07/02  06:58:34  mjl
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
    mkDialog .null $dialog_args {OK {}}
    dpos .null
    tkwait visibility .null
    grab .null
    tkwait window .null
}

#----------------------------------------------------------------------------
# bogon_alert
#
# Invokes a dialog explaining that the user bogued out.
#----------------------------------------------------------------------------

proc bogon_alert {msg} {
    set dialog_args "-text \"$msg\" -aspect 800 -justify left"
    mkDialog .bogus $dialog_args {OK {}}
    dpos .bogus
    tkwait visibility .bogus
    grab .bogus
    focus .bogus
    tkwait window .bogus
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
# Numeric utility procs:
#
#    min	returns minimum of two numeric arguments
#    max	returns maximum of two numeric arguments
#----------------------------------------------------------------------------

proc min {x y} {
    return [expr "$x < $y ? $x : $y"]
}

proc max {x y} {
    return [expr "$x > $y ? $x : $y"]
}

#----------------------------------------------------------------------------
# getItem
#
# Asks user to input something, returning the result.
# Selecting "Cancel" returns the empty string.
#----------------------------------------------------------------------------

proc getItem {item} {
    global itemval
    set w .entry
    set itemval ""

    catch {destroy $w}
    toplevel $w
    dpos $w
    wm title $w "Entry"
    wm iconname $w "Entry"
    message $w.msg -font -Adobe-times-medium-r-normal--*-180* -aspect 800 \
	    -text $item

    frame $w.frame -borderwidth 10
    pack append $w.frame \
	[entry $w.frame.e1 -relief sunken] {top pady 10 fillx} 

    button $w.ok -text OK -command \
	"set itemval \[$w.frame.e1 get\]; destroy $w"
    button $w.cancel -text Cancel -command "destroy $w"

    bind $w.frame.e1 <Return> \
	"set itemval \[$w.frame.e1 get\]; destroy $w"

    pack append $w $w.msg {top fill} $w.frame {top expand fill} \
	$w.ok {left expand fill} $w.cancel {left expand fill}

    tkwait visibility $w
    grab $w
    focus $w.frame.e1
    tkwait window $w
    return $itemval
}

#----------------------------------------------------------------------------
# confirm
#
# Sure about that, buddy?
#----------------------------------------------------------------------------

proc confirm {msg} {
    global confirm_flag
    set dialog_args "-text {$msg} \
		     -aspect 500 -justify left"
    mkDialog .confirm $dialog_args \
	"OK {set confirm_flag 1}" "Cancel {set confirm_flag 0}"
    tkwait visibility .confirm
    grab .confirm
    focus .confirm
    tkwait window .confirm
    return $confirm_flag
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
# EnterCoords
#
# Create a top-level window that displays a bunch of entries used for
# entering window coordinates.
#
# Arguments:
#    w		Name of top level window
#    desc	Short description of coordinates to be entered.
#
# Global variables referenced:
#    xmin	Relative min in x (0-1)
#    ymin	Relative min in y (0-1)
#    xmax	Relative max in x (0-1)
#    ymax	Relative max in y (0-1)
#
# The global variables are modified by the entry widgets and may be
# overwritten at any time so the caller must wait for the dialog to be
# destroyed and then use them immediately.  Note: there is no bounds
# checking done here on the global min/max variables.
#----------------------------------------------------------------------------

proc EnterCoords {w desc} {
    global xmin ymin xmax ymax
    global tabList

    catch {destroy $w}
    toplevel $w
    dpos $w

    wm title $w "Entry window"
    wm iconname $w "Entry"

    message $w.msg \
	-font -Adobe-times-medium-r-normal--*-180* \
	-aspect 700 \
	-text "Enter $desc.  Each coordinate should range from 0 to 1, with (0,0) corresponding to the lower left hand corner.  Click \"OK\" button when finished."

    pack append $w \
	$w.msg {top fill}

    set rows {min max}
    set cols {x y}
    set tabList ""

    foreach j $rows {
	frame $w.$j

	foreach i $cols {
            set var $i$j
	    frame $w.$j.$i -bd 1m

	    entry $w.$j.$i.entry -relief sunken -width 10
	    $w.$j.$i.entry insert 0 [set $var]
	    bind $w.$j.$i.entry <Tab> "EnterCoords_tab \$tabList"
	    bind $w.$j.$i.entry <Return> "EnterCoords_destroy $w"
            set tabList [concat $tabList $w.$j.$i.entry]

	    label $w.$j.$i.label -width 10
	    $w.$j.$i.label config -text "$var:"

	    pack append $w.$j.$i \
		$w.$j.$i.entry right \
		$w.$j.$i.label left

	    pack append $w.$j \
		$w.$j.$i {left fillx}
	}

	pack append $w \
	    $w.$j {top fillx} 
    }

    button $w.ok -text OK -command "EnterCoords_destroy $w"
    pack append $w \
	$w.ok {bottom fill}

    tkwait visibility $w
    grab $w
    focus $w.min.x.entry
}

# This procedure is invoked when the top level entry dialog is destroyed.
# It updates the global vars used to communicate the entry values then
# destroys the window.

proc EnterCoords_destroy {w} {
    global xmin ymin xmax ymax

    set xmin [$w.min.x.entry get]
    set ymin [$w.min.y.entry get]
    set xmax [$w.max.x.entry get]
    set ymax [$w.max.y.entry get]

    destroy $w
}

# The procedure below is invoked in response to tabs in the entry
# windows.  It moves the focus to the next window in the tab list.
# Arguments:
#
# list -	Ordered list of windows to receive focus

proc EnterCoords_tab {list} {
    set i [lsearch $list [focus]]
    if {$i < 0} {
	set i 0
    } else {
	incr i
	if {$i >= [llength $list]} {
	    set i 0
	}
    }
    focus [lindex $list $i]
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
