# $Id$
# $Log$
# Revision 1.7  1993/08/13 06:41:58  mjl
# Fixed broken font setting in GetItem proc.
#
# Revision 1.6  1993/08/13  04:36:00  mjl
# Previous scheme for scrolling backwards in a text widget help entry didn't
# work right under SunOS for some reason; now fixed.
#
# Revision 1.5  1993/08/09  22:21:49  mjl
# Removed all absolute references to fonts.  Now only accessed through
# global variables set in plconfig.tcl, for easier per-user customization.

#----------------------------------------------------------------------------
# PLPLOT TK/TCL graphics renderer support procs
# Maurice LeBrun
# 1-Jul-1993
# IFS, University of Texas at Austin
#
# Includes code borrowed from the TCL/TK widget demo.
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
# normal_text_setup
#
# Sets up text widgets the way I like them.
#----------------------------------------------------------------------------

proc normal_text_setup {w {width 60} {height 30}} {
    global dialog_font dialog_bold_font

    button $w.ok -text OK -command "destroy $w"
    text $w.t -relief raised -bd 2 -yscrollcommand "$w.s set" -setgrid true \
	    -width $width -height $height
    scrollbar $w.s -relief flat -command "text_scroll $w.t"
    pack append $w $w.ok {bottom fillx} $w.s {right filly} $w.t {expand fill}
    focus $w.t

# Set up display styles

    $w.t tag configure normal -font $dialog_font
    $w.t tag configure bold -font $dialog_bold_font

    if {[tk colormodel $w] == "color"} {
	$w.t tag configure color1 -background #eed5b7
	$w.t tag configure color2 -foreground red
	$w.t tag configure raised -background #eed5b7 -relief raised \
		-borderwidth 1
	$w.t tag configure sunken -background #eed5b7 -relief sunken \
		-borderwidth 1
    } else {
	$w.t tag configure color1 -background black -foreground white
	$w.t tag configure color2 -background black -foreground white
	$w.t tag configure raised -background white -relief raised \
		-borderwidth 1
	$w.t tag configure sunken -background white -relief sunken \
		-borderwidth 1
    }
    $w.t tag configure bgstipple -background black -borderwidth 0 \
	    -bgstipple gray25
    $w.t tag configure fgstipple -fgstipple gray50
    $w.t tag configure underline -underline on

# Set up bindings to be as useful as possible.

    bind $w <Any-Enter> "focus $w.t"

    bind $w.t <Return>	  "destroy $w"

    bind $w.t <Down>	  "text_scroll_by_line $w.t + 1"
    bind $w.t <Up>	  "text_scroll_by_line $w.t - 1"

    bind $w.t <Next>	  "text_scroll_by_page $w.t + 1"
    bind $w.t <space>	  "text_scroll_by_page $w.t + 1"

    bind $w.t <Prior>	  "text_scroll_by_page $w.t - 1"
    bind $w.t <BackSpace> "text_scroll_by_page $w.t - 1"
    bind $w.t <Delete>	  "text_scroll_by_page $w.t - 1"
}

#----------------------------------------------------------------------------
# text_scroll
#
# Scrolls text widget vertically, updating various things
#----------------------------------------------------------------------------

proc text_scroll {w line} {
    $w yview $line
    $w mark set insert [$w index @0,0]
}

#----------------------------------------------------------------------------
# text_scroll_by_line
#
# Scrolls text widget vertically by the given number of lines.
#----------------------------------------------------------------------------

proc text_scroll_by_line {w sign delta} {
    text_scroll $w [$w index "@0,0 $sign $delta lines"]
}

#----------------------------------------------------------------------------
# text_scroll_by_page
#
# Scrolls text widget vertically by the given number of pages (almost).
#----------------------------------------------------------------------------

proc text_scroll_by_page {w sign delta} {
    set height [lindex [$w config -height] 4]
    set delta [expr $delta*($height-2)]
    text_scroll $w [$w index "@0,0 $sign $delta lines"]
}

#----------------------------------------------------------------------------
# The procedure below inserts text into a given text widget and
# applies one or more tags to that text.  The arguments are:
#
# w		Window in which to insert
# text		Text to insert (it's inserted at the "insert" mark)
# args		One or more tags to apply to text.  If this is empty
#		then all tags are removed from the text.
#----------------------------------------------------------------------------

proc insertWithTags {w text args} {
    set start [$w index insert]
    $w insert insert $text
    foreach tag [$w tag names $start] {
	$w tag remove $tag $start insert
    }
    foreach i $args {
	$w tag add $i $start insert
    }
}

#----------------------------------------------------------------------------
# Numeric utility procs:
#
#    min	returns minimum argument
#    max	returns maximum argument
#
# Taken from utils.tcl by Tom Phelps (phelps@cs.Berkeley.EDU)
#----------------------------------------------------------------------------

proc min {args} {
   set x [lindex $args 0]
   foreach i $args {
      if {$i<$x} {set x $i}
   }
   return $x
}

proc max {args} {
   set x [lindex $args 0]
   foreach i $args {
      if {$i>$x} {set x $i}
   }
   return $x
}

#----------------------------------------------------------------------------
# getItem
#
# Asks user to input something, returning the result.
# Selecting "Cancel" returns the empty string.
#----------------------------------------------------------------------------

proc getItem {item} {
    global dialog_font dialog_bold_font
    global itemval

    set w .entry
    set itemval ""

    catch {destroy $w}
    toplevel $w
    dpos $w
    wm title $w "Entry"
    wm iconname $w "Entry"
    message $w.msg -font $dialog_font -aspect 800 -text $item

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
    global dialog_font dialog_bold_font
    global tabList
    global xmin ymin xmax ymax

    catch {destroy $w}
    toplevel $w
    dpos $w

    wm title $w "Entry window"
    wm iconname $w "Entry"

    message $w.msg \
	-font $dialog_font \
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
