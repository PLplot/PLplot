# $Id$
# $Log$
# Revision 1.40  2000/07/19 21:11:49  furnish
# Jumbo patch by Joao Cardoso.  Adds XOR, a polygon-fill light-shading
# surface plotter, contour labelling, and demo updates to show off these
# new features.
#
# Revision 1.39  1998/12/01 20:49:22  furnish
# Various fixups contributed by Joao Cardoso <jcardoso@inescn.pt>.
#
# Revision 1.38  1996/02/23  16:49:29  furnish
# Some little hacks to make the scroll bars look better during zooms.
#
# Revision 1.37  1995/08/22  16:17:53  mjl
# Inserted a necessary "update".
#
# Revision 1.36  1995/06/23  02:54:19  mjl
# Added resize binding, just experimental but I may want to mess with it
# again later.
#
# Revision 1.35  1995/06/13  21:28:24  mjl
# Miscellaneous tk4 fixes.
#
# Revision 1.34  1995/06/02  20:31:54  mjl
# Set zoom starting point to default to center.  Better for zooming in on
# small plot features, although worse for large objects (e.g. entire plots).
#
# Revision 1.33  1995/06/01  21:28:24  mjl
# Changed to call getSaveFile to retrieve save file name.
#
# Revision 1.32  1995/05/26  20:18:43  mjl
# Split up the plot-menu building code into different procs and rewrote
# substantially.  Save menu changed: the device is now specified as an option,
# with all Save-As.. selections going to this device.  A file type option
# was added, to select between 1 plot/file (the default) and many plots/file
# (archive files).  Intended to reduce errors caused by forgetting to close
# the save file.  The Again and Close commands are now disabled unless an
# archive type save file is open.  Zoom menu changed: forward/back entries
# added, to go forward or back one zoom level.  Disabled if it's not possible
# to do so.  Zoom options added: preserve aspect ratio (check button), zoom
# starting from corner or center (radio button).  Default is to zoom from
# one corner, preserving aspect ratio.
#
# Revision 1.31  1995/04/12  08:15:46  mjl
# Changed the way the client variable is treated.  If specified, enables
# settings relevant to working with the plplot/TK driver.  To simplify the
# code, the client variable is now kept globally and not passed as an argument
# so some proc syntaxes were changed.  Also, this forces only 1 plplot stream
# per plserver process (I doubt anyone has tried to do otherwise anyway).
#
# Revision 1.30  1995/03/16  23:17:01  mjl
# Key filtering changed to new modifier magnification scheme.  Changed style
# of reports (of keypress or mouse events) back to client code in order to be
# more complete.
#
# Revision 1.29  1994/10/11  18:57:58  mjl
# Default zooming behavior changed -- now it zooms outward from the center
# (first point clicked on), preserving aspect ratio.  This works correctly
# even if the orientation or the aspect ratio is changed.  The old behavior
# can be recovered by setting the global variable "zoomopt" to 1.
#
# Revision 1.28  1994/09/27  22:06:11  mjl
# Several important additions to the zooming capability:
#  - A double click now zooms by 2X, centered at the mouse cursor.  If the
#    cursor is too close to the boundary to allow this, an appropriately
#    higher magnification is used.
#  - The zoomed window positions are retained and can be recalled.  The keys
#    "b" (backward) and "f" (forward) are used to traverse the window list.
#
# Revision 1.27  1994/09/23  07:42:01  mjl
# Added a status message while waiting for plot to print.
#
# Revision 1.26  1994/08/25  04:01:23  mjl
# Simplified and sped up eop handling.
#
# Revision 1.25  1994/07/13  21:40:35  mjl
# Put in status message while waiting for plot to be saved.  Reassures the
# user that the program hasn't crashed, when saving very complicated plots.
#
# Revision 1.24  1994/06/09  20:24:57  mjl
# Massive reorganization and cleaning up.  Main result is that the plplot
# "megawidget" acts much more like a normal Tk widget.  Still not
# configurable; this will require extending it using itcl.  But now,
# creation and mapping is much more straightforward, both directly (from
# plserver or an extended wish) and from the plplot/tk driver.  You can do
# simply:
#
#     plxframe .plw
#     pack append . .plw {bottom fill expand}
#
# and you get a plframe "megawidget", complete with plot menu (with dump,
# zoom, etc) and status label.  Support widgets relevant for the plplot/tk
# driver do not come up unless invoked from the tk driver (by specifying
# a client to connect to).  The main drawback at this point with this method
# is that direct plotting commands in Tcl must be specified using the syntax
# (for the above example) ".plw.plwin cmd <command> <args>", whereas in
# itcl is simply ".plw <command> <args>".
#

#----------------------------------------------------------------------------
# PLPLOT TK/TCL graphics renderer
# plplot window initialization procs
# 
# Maurice LeBrun
# IFS, University of Texas at Austin
# 29-May-1993
#
# Note: to keep namespace problems to a minimum, all procs defined here begin
# with "pl".  These are further subdivided into "plw_" for button- or
# menu-accessible commands, or "pl_" for utility commands.
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
# plw_create
#
# Front-end routine to create plplot megawidget for use from PLplot tk
# driver.  Right now does nothing special.
#----------------------------------------------------------------------------

proc plw_create {w {client_id {}}} {
    plxframe $w $client_id
}

#----------------------------------------------------------------------------
# plr_create
#
# A front-end to plw_create, used by plrender.
#----------------------------------------------------------------------------

proc plr_create {w {client_id {}}} {
    global is_plrender; set is_plrender 1
    plw_create $w $client_id
}

#----------------------------------------------------------------------------
# plxframe
#
# Creates the "extended" plframe widget.  Eventually may be replaced with
# a real megawidget capability, using itcl.  The actual plframe widget
# is named $w.plwin.  Example usage:
#
# plxframe .plw
# pack append . .plw {bottom fill expand}
#
# The PLplot/TK (or DP) driver works by fork/exec of a plserver (an
# extended wish), and subsequent communication of graphics instructions
# data from the driver via a FIFO or socket.  In this case the client_id
# variable must be specified in the call.  In direct widget instantiation
# the client_id variable should not be used.
#----------------------------------------------------------------------------

proc plxframe {w {client_id {}}} {

    global client plot_menu_on plwidget

# the client should'nt know the name of the plot window?
    set plwidget $w

# Save client name

    if {$client_id != ""} then {
	set client $client_id
    }

# Make container frame.  It is mapped later.

# jc: if the container already exists, don't exit (catch added)

    catch "frame $w"

# Create child plplot widget (plframe), and pack into parent.

    plframe $w.plwin -relief sunken
    pack append $w \
	$w.plwin {bottom expand fill}

# Set up defaults

    plw_setup_defaults $w

# Make frame for top row widgets.
# plframe widget must already have been created (the plframe is queried
# for a list of the valid output devices for page dumps).

if $plot_menu_on then {
    plw_create_TopRow $w
    pack append $w \
	$w.ftop {top fill}
    update }

# Enable keyboard traversal when widget has the input focus.
# Also grab the initial input focus.

    tk_bindForTraversal $w.plwin
    focus $w.plwin

# Set up bop/eop signal and inform client of plplot widget name for widget
# commands.

    if { [info exists client] } then {
		if $plot_menu_on then {
	set bop_col [option get $w.ftop.leop off Label]
	set eop_col [option get $w.ftop.leop on Label]

	$w.plwin configure -bopcmd "plw_flash $w $bop_col"
	$w.plwin configure -eopcmd "plw_flash $w $eop_col"
		} else {
		    $w.plwin configure -bopcmd {update}
		    $w.plwin configure -eopcmd {update}
		}
	}

    # Resize binding -- just experimental for now.
    #	bind $w.plwin <Configure> "client_cmd \"plfinfo %w %h\""

	client_cmd "set plwidget $w.plwin"

    return $w
}

#----------------------------------------------------------------------------
# plw_setup_defaults
#
# Set up default settings.
#----------------------------------------------------------------------------

proc plw_setup_defaults {w} {

# In the two cases below, the options can be specified in advance through
# the global variables zoomopt_0, etc, and saveopt_0, etc.  Not a great
# solution but will have to do for now.

# zoom options:
#  0:	0=don't preserve aspect ratio, 1=do
#  1:	0=stretch from corner, 1=stretch from center

    global zoomopts zoomopt_0 zoomopt_1

    set zoomopts($w,0) 1
    set zoomopts($w,1) 1
    if { [info exists zoomopt_0] } {set zoomopts($w,0) $zoomopt_0}
    if { [info exists zoomopt_1] } {set zoomopts($w,1) $zoomopt_1}

# save options:
#  0:   name of default save device
#  1:   0=save 1 plot/file, 1=save multi plots/file (must close!)

    global saveopts saveopt_0 saveopt_1

    set saveopts($w,0) "psc"
    set saveopts($w,1) 0
    if { [info exists saveopt_0] } {set saveopts($w,0) $saveopt_0}
    if { [info exists saveopt_1] } {set saveopts($w,1) $saveopt_1}

# Set up zoom windows list

    global zidx zidx_max zxl zyl zxr zyr

    set zidx($w) 0
    set zidx_max($w) 0
    set zxl($w,0) 0.0
    set zyl($w,0) 0.0
    set zxr($w,0) 1.0
    set zyr($w,0) 1.0

# Bindings

    bind $w.plwin <Any-KeyPress> \
	"plw_key_filter $w %N %s %x %y %K %A"

    bind $w.plwin <Any-ButtonPress> \
	"plw_user_mouse $w %b %s %x %y"

    bind $w.plwin <B1-Motion> \
	"plw_user_mouse $w %b %s %x %y"

    bind $w.plwin <B2-Motion> \
	"plw_user_mouse $w %b %s %x %y"

    bind $w.plwin <B3-Motion> \
	"plw_user_mouse $w %b %s %x %y"
	
    bind $w.plwin <Any-Enter> \
	"focus $w.plwin"
}

#----------------------------------------------------------------------------
# plw_create_TopRow
#
# Create top row widgets.  Page-oriented widgets only have a meaning in
# the context of the PLplot driver, so don't create them if there is no
# client (as occurs for direct widget instantiation).
#----------------------------------------------------------------------------

proc plw_create_TopRow {w} {
    global is_plrender client

    frame $w.ftop

# End of page indicator

    if { [info exists client] } then {
	pack append $w.ftop \
	    [label $w.ftop.leop -relief raised] \
	    {left fill padx 12}

	$w.ftop.leop config -bg [option get $w.ftop.leop on Label]
    }

# Plot menu

    pack append $w.ftop \
	[plw_create_pmenu $w $w.ftop.pmenu] \
	{left fill padx 12}

# Forward and backward (plrender only) page buttons.
# Just a hack until I get around to doing it right.

    if { [info exists client] } then {
	if { [info exists is_plrender] } {
	    pack append $w.ftop \
		[button $w.ftop.bp -text "<<" -relief raised] \
		{left fill padx 10}

	    $w.ftop.bp configure -command \
		"client_cmd {keypress 65288 0 0 0 0. 0. BackSpace}"
	}

	pack append $w.ftop \
	    [button $w.ftop.fp -text ">>" -relief raised] \
	    {left fill padx 10}

	$w.ftop.fp configure -command \
	    "client_cmd {keypress 65293 0 0 0 0. 0. Return}"
    }

# Label widget for status messages.

    label $w.ftop.lstat -anchor w -relief raised
    plw_label_reset $w
    pack append $w.ftop $w.ftop.lstat \
	{right expand fill} 
}

#----------------------------------------------------------------------------
# plw_create_pmenu
#
# Create plot menu.
#
# It is tempting to create buttons for some of these options, but buttons
# are difficult to effectively place and extend.  Menus have a clear
# placement mechanism and are easy to add to.  Further, TK menus can be
# torn off (select menu with middle mouse button and move to where you
# want it) which makes selecting top-level menu buttons easy.  Finally,
# certain menu options have keyboard equivalents: zoom-select (z),
# zoom-reset (r), print (P), and save-again (s).
#----------------------------------------------------------------------------

proc plw_create_pmenu {w pmbut} {

    global pmenu; set pmenu($w) $pmbut.m

    menubutton $pmbut -text "Plot" -menu $pmenu($w) -relief raised
    menu $pmenu($w)

    plw_create_pmenu_print   $w
    plw_create_pmenu_save    $w
    plw_create_pmenu_orient  $w
    plw_create_pmenu_zoom    $w
    plw_create_pmenu_page    $w
    plw_create_pmenu_options $w

    return $pmbut
}

#----------------------------------------------------------------------------
# plw_create_pmenu_print
#
# Create plot-print menu
#----------------------------------------------------------------------------

proc plw_create_pmenu_print {w} {

    global pmenu

    $pmenu($w) add command -label "Print" \
	-command "plw_print $w"
}

#----------------------------------------------------------------------------
# plw_create_pmenu_save
#
# Create plot-save menu (cascade)
#----------------------------------------------------------------------------

proc plw_create_pmenu_save {w} {

    global pmenu; set m $pmenu($w).save

    $pmenu($w) add cascade -label "Save" -menu $m
    menu $m

# Save - As

    $m add command -label "As" \
	-command "plw_save_as $w"

# Save - Again

    $m add command -label "Again" \
	-command "plw_save_again $w" \
	-state disabled

# Save - Close

    $m add command -label "Close" \
	-command "plw_save_close $w" \
	-state disabled

    $m add separator

# Save - Set device.. (another cascade)

    $m add cascade -label "Set device" -menu $m.sdev
    menu $m.sdev

# Generate the device list in the "Save/Set device" widget menu, by querying
# the plframe widget for the available output devices (which are listed).

    global saveopts

    set devnames [$w.plwin info devnames]
    set devkeys [$w.plwin info devkeys]
    set ndevs [llength $devnames]
    for {set i 0} {$i < $ndevs} {incr i} {
	set devnam [lindex $devnames $i]
	set devkey [lindex $devkeys $i]

	$m.sdev add radio -label $devnam \
	    -variable saveopts($w,0) -value $devkey
    }

# Save - Set file type.. (another cascade)

    $m add cascade -label "Set file type" -menu $m.sfile
    menu $m.sfile

# Single file (one plot/file)

    $m.sfile add radio -label "Single file (one plot/file)" \
	-variable saveopts($w,1) -value 0

# Archive file (many plots/file)

    $m.sfile add radio -label "Archive file (many plots/file)" \
	-variable saveopts($w,1) -value 1
}

#----------------------------------------------------------------------------
# plw_create_pmenu_orient
#
# Create plot-orient menu (cascade)
#----------------------------------------------------------------------------

proc plw_create_pmenu_orient {w} {

    global pmenu; set m $pmenu($w).orient

    $pmenu($w) add cascade -label "Orient" -menu $m 
    menu $m

    $m configure -postcommand "plw_update_orient $w"

# Orient - 0 degrees

    $m add radio -label "0 degrees" \
	-command "plw_orient $w 0"

# Orient - 90 degrees

    $m add radio -label "90 degrees" \
	-command "plw_orient $w 1"

# Orient - 180 degrees

    $m add radio -label "180 degrees" \
	-command "plw_orient $w 2"

# Orient - 270 degrees

    $m add radio -label "270 degrees" \
	-command "plw_orient $w 3"
}

#----------------------------------------------------------------------------
# plw_create_pmenu_zoom
#
# Create plot-zoom menu (cascade)
#----------------------------------------------------------------------------

proc plw_create_pmenu_zoom {w} {

    global pmenu; set m $pmenu($w).zoom

    $pmenu($w) add cascade -label "Zoom" -menu $m
    menu $m

    $m configure -postcommand "plw_update_zoom $w"

# Zoom - select (by mouse)

    $m add command -label "Select" \
	-command "plw_zoom_select $w"

# Zoom - back (go back 1 zoom level)

    $m add command -label "Back" \
	-command "plw_zoom_back $w" \
	-state disabled

# Zoom - forward (go forward 1 zoom level)

    $m add command -label "Forward" \
	-command "plw_zoom_forward $w" \
	-state disabled

# Zoom - enter bounds

    $m add command -label "Enter bounds.." \
	-command "plw_zoom_enter $w"

# Zoom - reset

    $m add command -label "Reset" \
	-command "plw_zoom_reset $w"

# Zoom - options (another cascade)

    $m add cascade -label "Options" -menu $m.options
    menu $m.options

    global zoomopts
    $m.options add check -label "Preserve aspect ratio" \
	-variable zoomopts($w,0)

    $m.options add separator

    $m.options add radio -label "Start from corner" \
	-variable zoomopts($w,1) -value 0

    $m.options add radio -label "Start from center" \
	-variable zoomopts($w,1) -value 1

    $m.options invoke [plw_entry 1]
}

#----------------------------------------------------------------------------
# plw_create_pmenu_page
#
# Create plot-page menu (cascade)
#----------------------------------------------------------------------------

proc plw_create_pmenu_page {w} {

    global pmenu; set m $pmenu($w).page

    $pmenu($w) add cascade -label "Page" -menu $m
    menu $m

# Page - enter bounds

    $m add command -label "Setup.." \
	-command "plw_page_enter $w"

# Page - reset

    $m add command -label "Reset" \
	-command "plw_page_reset $w"
}

#----------------------------------------------------------------------------
# plw_create_pmenu_redraw
#
# Create plot-redraw menu
# I only use this for debugging in cases where the normal redraw capability
# isn't working right.
#----------------------------------------------------------------------------

proc plw_create_pmenu_redraw {w} {

    global pmenu

    $pmenu($w) add command -label "Redraw" \
	-command "$w.plwin redraw"
}

#----------------------------------------------------------------------------
# plw_create_pmenu_options
#
# Create plot-options menu (cascade)
#----------------------------------------------------------------------------

proc plw_create_pmenu_options {w} {

    global pmenu; set m $pmenu($w).options

    $pmenu($w) add cascade -label "Options" -menu $m
    menu $m

    $m add command -label "Palette 0" \
	-command "plcmap0_edit $w" 

    $m add command -label "Palette 1" \
	-command "plcmap1_edit $w" 
}

#----------------------------------------------------------------------------
# plw_entry
#
# Compensates for tk3.6 -> tk4.0 lunacy, where the menu entries are all
# incremented by 1!  This happens because of the tear-off entry.  You can
# create the menu without a tear-off, but that's even more intrusive.
#----------------------------------------------------------------------------

proc plw_entry {entry} {

    global tk_version
    if { $tk_version >= 4.0 } then {
	incr entry
    }
    return $entry
}

#----------------------------------------------------------------------------
# plw_start
#
# Responsible for plplot graphics package initialization on the widget.
# People driving the widget directly should just use pack themselves.
#
# Put here to reduce the possibility of a time out over a slow network --
# the client program waits until the variable widget_is_ready is set.
#----------------------------------------------------------------------------

proc plw_start {w} {
    global client

# Manage widget hierarchy

    pack append [winfo parent $w] $w \
	{bottom expand fill}

    update

# Inform client that we're done.

    if { [info exists client] } then {
	client_cmd "set widget_is_ready 1"
    }

# jc: call a user supplied routine to do any necessary post initialization
   catch after_plw_start
}

#----------------------------------------------------------------------------
# plw_key_filter
#
# Front-end to key handler.
# For supported operations it's best to modify the global key variables
# to get the desired action.  More advanced stuff can be done with the
# $user_key_filter proc.  Find anything particularly useful?  Let me know,
# so it can be added to the default behavior.
#----------------------------------------------------------------------------

proc plw_key_filter {w keycode state x y keyname ascii} {
    global user_key_filter

    global key_zoom_select
    global key_zoom_back
    global key_zoom_forward    
    global key_zoom_reset
    global key_print
    global key_save_again
    global key_scroll_right
    global key_scroll_left
    global key_scroll_up
    global key_scroll_down

#    puts "keypress: $keyname $keycode $ascii $state"

# Call user-defined key filter, if one exists

    if { [info exists user_key_filter] } then {
	$user_key_filter $w $keyname $keycode $ascii
    }

# Interpret keystroke

    switch $keyname \
	$key_zoom_select	"plw_zoom_select $w" \
	$key_zoom_back		"plw_zoom_back $w" \
	$key_zoom_forward	"plw_zoom_forward $w" \
	$key_zoom_reset		"plw_zoom_reset $w" \
	$key_print		"plw_print $w" \
	$key_save_again		"plw_save_again $w" \
	$key_scroll_right	"plw_view_scroll $w  1  0 $state" \
	$key_scroll_left	"plw_view_scroll $w -1  0 $state" \
	$key_scroll_up		"plw_view_scroll $w  0 -1 $state" \
	$key_scroll_down	"plw_view_scroll $w  0  1 $state" 

# Pass keypress event info back to client.

    plw_user_key $w $keycode $state $x $y $keyname $ascii
}

#----------------------------------------------------------------------------
# plw_user_key
#
# Passes keypress event information back to client.
# Based on plw_user_mouse.
#----------------------------------------------------------------------------

proc plw_user_key {w keycode state x y keyname ascii} {
    global client

    if { [info exists client] } then {

    # calculate relative window coordinates.

	set xw [expr "$x / [winfo width $w.plwin]."]
	set yw [expr "1.0 - $y / [winfo height $w.plwin]."]

    # calculate normalized device coordinates into original window.

	set view [$w.plwin view]
	set xrange [expr "[lindex $view 2] - [lindex $view 0]"]
	set xnd [expr "($xw * $xrange) + [lindex $view 0]"]
	set yrange [expr "[lindex $view 3] - [lindex $view 1]"]
	set ynd [expr "($yw * $yrange ) + [lindex $view 1]"]

    # send them back to the client.

#	puts "keypress $keycode $state $x $y $xnd $ynd $keyname $ascii"
	client_cmd \
	    [list keypress $keycode $state $x $y $xnd $ynd $keyname $ascii]
    }
}

#----------------------------------------------------------------------------
# plw_user_mouse
#
# Passes buttonpress event information back to client.
# Written by Radey Shouman
#----------------------------------------------------------------------------

proc plw_user_mouse {w button state x y} {
    global client

    if { [info exists client] } then {

    # calculate relative window coordinates.

	set xw [expr "$x / [winfo width $w.plwin]."]
	set yw [expr "1.0 - $y / [winfo height $w.plwin]."]

    # calculate normalized device coordinates into original window.

	set view [$w.plwin view]
	set xrange [expr "[lindex $view 2] - [lindex $view 0]"]
	set xnd [expr "($xw * $xrange) + [lindex $view 0]"]
	set yrange [expr "[lindex $view 3] - [lindex $view 1]"]
	set ynd [expr "($yw * $yrange ) + [lindex $view 1]"]

    # send them back to the client.

	client_cmd \
	    [list buttonpress $button $state $x $y $xnd $ynd]
    }
}

#----------------------------------------------------------------------------
# plw_flash
#
# Set eop button color to indicate page status.
#----------------------------------------------------------------------------

proc plw_flash {w col} {
    $w.ftop.leop config -bg $col
    update idletasks
}

#----------------------------------------------------------------------------
# plw_end
#
# Executed as part of orderly shutdown procedure.  Eventually will just
# destroy the plframe and surrounding widgets, and server will exit only
# if all plotting widgets have been destroyed and it is a child of the
# plplot/TK driver.  Maybe.
#
# The closelink command was added in the hopes of making the dp driver
# cleanup a bit more robust, but doesn't seem to have any effect except
# to slow things down quite a bit.  
#----------------------------------------------------------------------------

proc plw_end {w} {
    global dp
#    $w.plwin closelink
    if { $dp } then {
	global list_sock
	close $list_sock
    }
    exit
}

#----------------------------------------------------------------------------
# plw_print
#
# Prints plot.  Uses the "plpr" script, which must be set up for your site
# as appropriate.  There are better ways to do it but this way is safest
# for now.
#----------------------------------------------------------------------------

proc plw_print {w} {
    plw_label_set $w "Printing plot..."
    update
    if { [catch "$w.plwin print" foo] } {
	bogue_out "$foo"
    } else {
	status_msg $w "Plot printed."
    }
}

#----------------------------------------------------------------------------
# plw_save_as
#
# Saves plot to default device, prompting for file name.
#----------------------------------------------------------------------------

proc plw_save_as {w} {
    global pmenu saveopts
    set file [getSaveFile $saveopts($w,0)]
    if { [string length $file] > 0 } {
	if { [file exists $file] } {
	    if { ! [confirm "File $file already exists.  Are you sure?"] } {
		return
	    }
	}

	plw_label_set $w "Saving plot..."
	update
	if { [catch "$w.plwin save as $saveopts($w,0) $file" foo] } {
	    plw_label_reset $w
	    bogue_out "$foo"
	} else {
	    status_msg $w "Plot saved."
	}

	if { $saveopts($w,1) == 0 } then {
	    $w.plwin save close
	} else {
	    $pmenu($w).save entryconfigure 1 -state normal
	    $pmenu($w).save entryconfigure 2 -state normal
	    bogue_out "Warning: archive files must be closed before using"
	}
    } else {
	bogue_out "No file specified"
    }
}

#----------------------------------------------------------------------------
# plw_save_again
#
# Saves plot to an already open file.
#----------------------------------------------------------------------------

proc plw_save_again {w} {
    if { [catch "$w.plwin save" foo] } {
	bogue_out "$foo"
    } else {
	status_msg $w "Plot saved."
    }
}

#----------------------------------------------------------------------------
# plw_save_close
#
# Close archive save file.
#----------------------------------------------------------------------------

proc plw_save_close {w} {
    global pmenu
    if { [catch "$w.plwin save close" foo] } {
	bogue_out "$foo"
    } else {
	status_msg $w "Archive file closed."
	$pmenu($w).save entryconfigure 1 -state disabled
	$pmenu($w).save entryconfigure 2 -state disabled
    }
}

#----------------------------------------------------------------------------
# plw_update_zoom
#
# Responsible for making sure zoom menu entries are normal or disabled as
# appropriate.  In particular, that "Back" or "Forward" are only displayed
# if it is possible to traverse the zoom windows list in that direction.
#----------------------------------------------------------------------------

proc plw_update_zoom {w} {
    global zidx zidx_max zxl zyl zxr zyr
    global pmenu

# Back

    if { $zidx($w) == 0 } then {
	$pmenu($w).zoom entryconfigure 1 -state disabled
    } else {
	$pmenu($w).zoom entryconfigure 1 -state normal
    }

# Forward

    if { $zidx_max($w) == 0 || $zidx($w) == $zidx_max($w) } then {
	$pmenu($w).zoom entryconfigure 2 -state disabled
    } else {
	$pmenu($w).zoom entryconfigure 2 -state normal
    }
}

#----------------------------------------------------------------------------
# plw_zoom_select
#
# Zooms plot in response to mouse selection.
#----------------------------------------------------------------------------

proc plw_zoom_select {w} {
    global def_button_cmd zoomopts

    set def_button_cmd [bind $w.plwin <ButtonPress>]

    if { $zoomopts($w,1) == 0 } then {
	plw_label_set $w "Click on one corner of zoom region."
    } else {
	plw_label_set $w "Click on center of zoom region."
    }

    bind $w.plwin <ButtonPress> "plw_zoom_start $w %x %y"
}

#----------------------------------------------------------------------------
# plw_zoom_enter
#
# Zooms plot in response to text entry.
#----------------------------------------------------------------------------

proc plw_zoom_enter {w} {
    global fv00 fv01 fv10 fv11
    global fn00 fn01 fn10 fn11

    set coords [$w.plwin view]

    set fv00 [lindex "$coords" 0]
    set fv01 [lindex "$coords" 1]
    set fv10 [lindex "$coords" 2]
    set fv11 [lindex "$coords" 3]

    set fn00 xmin
    set fn01 ymin
    set fn10 xmax
    set fn11 ymax

    Form2d .e "Enter window coordinates for zoom.  Each coordinate should range from 0 to 1, with (0,0) corresponding to the lower left hand corner."
    tkwait window .e

    plw_view_select $w $fv00 $fv01 $fv10 $fv11
}

#----------------------------------------------------------------------------
# plw_zoom_reset
#
# Resets after zoom.
# Note that an explicit redraw is not necessary since the packer issues a
# resize after the scrollbars are unmapped.
#----------------------------------------------------------------------------

proc plw_zoom_reset {w} {
    global def_button_cmd

    plw_label_reset $w
    bind $w.plwin <ButtonPress> $def_button_cmd
    $w.plwin view reset
    if { [winfo exists $w.hbar] && [winfo ismapped $w.hbar] } then {
	pack unpack $w.hbar
    }
    if { [winfo exists $w.vscroll] && [winfo exists $w.vscroll] } then {
	pack unpack $w.vscroll
    }

# Reset zoom windows list

    global zidx zidx_max zxl zyl zxr zyr

    set zidx($w) 0
    set zidx_max($w) 0
    set zxl($w,0) 0.0
    set zyl($w,0) 0.0
    set zxr($w,0) 1.0
    set zyr($w,0) 1.0
}

#----------------------------------------------------------------------------
# plw_update_orient
#
# Responsible for making sure orientation radio buttons are up to date.
#----------------------------------------------------------------------------

proc plw_update_orient {w} {
    global pmenu
    set rot [$w.plwin orient]
    set entry [expr [format "%.0f" $rot] % 4]
    $pmenu($w).orient invoke [plw_entry $entry]
}

#----------------------------------------------------------------------------
# plw_orient
#
# Changes plot orientation.
#----------------------------------------------------------------------------

proc plw_orient {w rot} {
    if { [$w.plwin orient] != $rot} then {
	$w.plwin orient $rot
    }
}

#----------------------------------------------------------------------------
# plw_page_enter
#
# Changes output page parameters (margins, aspect ratio, justification).
#----------------------------------------------------------------------------

proc plw_page_enter {w} {
    global fv00 fv01 fv10 fv11
    global fn00 fn01 fn10 fn11

    set coords [$w.plwin page]

    set fv00 [lindex "$coords" 0]
    set fv01 [lindex "$coords" 1]
    set fv10 [lindex "$coords" 2]
    set fv11 [lindex "$coords" 3]

    set fn00 mar
    set fn01 aspect
    set fn10 jx
    set fn11 jy

    Form2d .e "Enter page setup parameters.  mar denotes the fractional page area on each side to use as a margin (0 to 0.5).  jx and jy are the fractional justification relative to the center (-0.5 to 0.5).  aspect is the page aspect ratio (0 preserves original aspect ratio)."
    tkwait window .e

    $w.plwin page $fv00 $fv01 $fv10 $fv11
}

#----------------------------------------------------------------------------
# plw_page_reset
#
# Resets page parameters.
#----------------------------------------------------------------------------

proc plw_page_reset {w} {
    $w.plwin page 0. 0. 0. 0.
}

#----------------------------------------------------------------------------
# plw_zoom_start
#
# Starts plot zoom.
#----------------------------------------------------------------------------

proc plw_zoom_start {w wx wy} {
    global def_button_cmd

    bind $w.plwin <ButtonPress> $def_button_cmd
    plw_label_set $w "Select zoom region by dragging mouse, then release."

    $w.plwin draw init
    bind $w.plwin <B1-Motion>        "plw_zoom_mouse_draw $w $wx $wy %x %y"
    bind $w.plwin <B1-ButtonRelease> "plw_zoom_mouse_end $w $wx $wy %x %y"
}

#----------------------------------------------------------------------------
# plw_zoom_coords
#
# Transforms the initial and final mouse coordinates to either:
#
# opt = 0	device coordinates
# opt = 1	normalized device coordinates
#
# The global variable "zoomopts" is used to determine zoom behavior:
#
# zoomopts($w,0):
#   0	box follows mouse movements exactly
#   1	box follows mouse movements so that aspect ratio is preserved (default)
#
# zoomopts($w,1):
#   0	first and last points specified determine opposite corners
#	of zoom box.
#   1	box is centered about the first point clicked on, 
#	perimeter follows mouse	(default)
#
#----------------------------------------------------------------------------

proc plw_zoom_coords {w x0 y0 x1 y1 opt} {
    global zoomopts

    set Lx [winfo width  $w.plwin]
    set Ly [winfo height $w.plwin]

# Enforce boundaries in device coordinate space

    set bounds [$w.plwin view bounds]
    set xmin [expr [lindex "$bounds" 0] * $Lx]
    set ymin [expr [lindex "$bounds" 1] * $Ly]
    set xmax [expr [lindex "$bounds" 2] * $Lx]
    set ymax [expr [lindex "$bounds" 3] * $Ly]

    set x1 [max $xmin [min $xmax $x1]]
    set y1 [max $ymin [min $ymax $y1]]

# Two-corners zoom.

    if { $zoomopts($w,1) == 0 } then {

    # Get box lengths

	set dx [expr $x1 - $x0]
	set dy [expr $y1 - $y0]

	set sign_dx [expr ($dx > 0) ? 1 : -1]
	set sign_dy [expr ($dy > 0) ? 1 : -1]

	set xl $x0
	set yl $y0

    # Constant aspect ratio

	if { $zoomopts($w,0) == 1 } then {

	# Scale factors used to maintain plot aspect ratio

	    set xscale [expr $xmax - $xmin]
	    set yscale [expr $ymax - $ymin]

	# Adjust box size for proper aspect ratio

	    set rx [expr double(abs($dx)) / $xscale]
	    set ry [expr double(abs($dy)) / $yscale]

	    if { $rx > $ry } then {
		set dy [expr $yscale * $rx * $sign_dy]
	    } else {
		set dx [expr $xscale * $ry * $sign_dx]
	    }

	    set xr [expr $xl + $dx]
	    set yr [expr $yl + $dy]

	# Now check again to see if in bounds, and adjust if not

	    if { $xr < $xmin || $xr > $xmax } then {
		if { $xr < $xmin } then {
		    set dx [expr $xmin - $x0]
		} else {
		    set dx [expr $xmax - $x0]
		}
		set rx [expr double(abs($dx)) / $xscale]
		set dy [expr $yscale * $rx * $sign_dy]
	    }

	    if { $yr < $ymin || $yr > $ymax } then {
		if { $yr < $ymin } then {
		    set dy [expr $ymin - $y0]
		} else {
		    set dy [expr $ymax - $y0]
		}
		set ry [expr double(abs($dy)) / $yscale]
		set dx [expr $xscale * $ry * $sign_dx]
	    }
	}

    # Final box coordinates

	set xr [expr $xl + $dx]
	set yr [expr $yl + $dy]

# zoom from center out, preserving aspect ratio

    } else {

    # Get box lengths, adjusting downward if necessary to keep in bounds

	set dx [expr abs($x1 - $x0)]
	set dy [expr abs($y1 - $y0)]

	set xr [expr $x0 + $dx]
	set xl [expr $x0 - $dx]
	set yr [expr $y0 + $dy]
	set yl [expr $y0 - $dy]

	if { $xl < $xmin } then {
	    set dx [expr $x0 - $xmin]
	}
	if { $xr > $xmax } then {
	    set dx [expr $xmax - $x0]
	}
	if { $yl < $ymin } then {
	    set dy [expr $y0 - $ymin]
	}
	if { $yr > $ymax } then {
	    set dy [expr $ymax - $y0]
	}

    # Constant aspect ratio

	if { $zoomopts($w,0) == 1 } then {

	# Scale factors used to maintain plot aspect ratio

	    set xscale [expr $xmax - $xmin]
	    set yscale [expr $ymax - $ymin]

	# Adjust box size for proper aspect ratio

	    set rx [expr double($dx) / $xscale]
	    set ry [expr double($dy) / $yscale]

	    if { $rx > $ry } then {
		set dy [expr $yscale * $rx]
	    } else {
		set dx [expr $xscale * $ry]
	    }

	    set xr [expr $x0 + $dx]
	    set xl [expr $x0 - $dx]
	    set yr [expr $y0 + $dy]
	    set yl [expr $y0 - $dy]

	# Now check again to see if in bounds, and adjust downward if not

	    if { $xl < $xmin } then {
		set dx [expr $x0 - $xmin]
		set rx [expr double($dx) / $xscale]
		set dy [expr $yscale * $rx]
	    }
	    if { $xr > $xmax } then {
		set dx [expr $xmax - $x0]
		set rx [expr double($dx) / $xscale]
		set dy [expr $yscale * $rx]
	    }
	    if { $yl < $ymin } then {
		set dy [expr $y0 - $ymin]
		set ry [expr double($dy) / $yscale]
		set dx [expr $xscale * $ry]
	    }
	    if { $yr > $ymax } then {
		set dy [expr $ymax - $y0]
		set ry [expr double($dy) / $yscale]
		set dx [expr $xscale * $ry]
	    }
	}

    # Final box coordinates

	set xr [expr $x0 + $dx]
	set xl [expr $x0 - $dx]
	set yr [expr $y0 + $dy]
	set yl [expr $y0 - $dy]
    }

# Optional translation to relative device coordinates.

    if { $opt == 1 } then {
	set wxl [expr "$xl / double($Lx)" ]
	set wxr [expr "$xr / double($Lx)" ]
	set wyl [expr "1.0 - $yr / double($Ly)" ]
	set wyr [expr "1.0 - $yl / double($Ly)" ]

    } else {
	set wxr $xl
	set wxl $xr
	set wyr $yl
	set wyl $yr
    }

    return "$wxl $wyl $wxr $wyr"
}

#----------------------------------------------------------------------------
# plw_zoom_mouse_draw
#
# Draws zoom box in response to mouse motion (with button held down).
#----------------------------------------------------------------------------

proc plw_zoom_mouse_draw {w wx0 wy0 wx1 wy1} {

    set coords [plw_zoom_coords $w $wx0 $wy0 $wx1 $wy1 0]

    $w.plwin draw rect \
	[lindex "$coords" 0] [lindex "$coords" 1] \
	[lindex "$coords" 2] [lindex "$coords" 3] 
}

#----------------------------------------------------------------------------
# plw_zoom_mouse_end
#
# Performs actual zoom, invoked when user releases mouse button.
#----------------------------------------------------------------------------

proc plw_zoom_mouse_end {w wx0 wy0 wx1 wy1} {
    
# Finish rubber band draw

    bind $w.plwin <B1-ButtonRelease> {}
    bind $w.plwin <B1-Motion> {}
    plw_label_reset $w
    $w.plwin draw end

# Select new plot region

    set coords [plw_zoom_coords $w $wx0 $wy0 $wx1 $wy1 1]

    plw_view_zoom $w \
	[lindex "$coords" 0] [lindex "$coords" 1] \
	[lindex "$coords" 2] [lindex "$coords" 3] 
}

#----------------------------------------------------------------------------
# plw_view_select
#
# Handles change of view into plot.
# Given in relative plot window coordinates.
#----------------------------------------------------------------------------

proc plw_view_select {w x0 y0 x1 y1} {
    
# Adjust arguments to be in bounds and properly ordered (xl < xr, etc)

    set xl [min $x0 $x1]
    set yl [min $y0 $y1]
    set xr [max $x0 $x1]
    set yr [max $y0 $y1]

    set xmin 0.
    set ymin 0.
    set xmax 1.
    set ymax 1.

    set xl [max $xmin [min $xmax $xl]]
    set yl [max $ymin [min $ymax $yl]]
    set xr [max $xmin [min $xmax $xr]]
    set yr [max $ymin [min $ymax $yr]]

# Only create scrollbars if really needed.

    if {($xl == $xmin) && ($xr == $xmax)} \
    then {set hscroll 0} else {set hscroll 1}

    if {($yl == $xmin) && ($yr == $xmax)} \
    then {set vscroll 0} else {set vscroll 1}

    if { ! ($hscroll || $vscroll)} {return}

# Select plot region

    $w.plwin view select $xl $yl $xr $yr

# Fix up view

    plw_fixview $w $hscroll $vscroll
}

#----------------------------------------------------------------------------
# plw_view_zoom
#
# Handles zoom.
# Given in relative device coordinates.
#----------------------------------------------------------------------------

proc plw_view_zoom {w x0 y0 x1 y1} {
    
    global xl xr yl yr

# Adjust arguments to be properly ordered (xl < xr, etc)

    set xl [min $x0 $x1]
    set yl [min $y0 $y1]
    set xr [max $x0 $x1]
    set yr [max $y0 $y1]

# Check for double-click (specified zoom region less than a few pixels
# wide).  In this case, magnification is 2X in each direction, centered at
# the mouse location.  At the boundary, the magnification is determined
# by the distance to the boundary.

    set stdzoom 0.5
    if { ($xr - $xl < 0.02) && ($yr - $yl < 0.02) } then {
	set nxl [expr $xl - 0.5 * $stdzoom]
	set nxr [expr $xl + 0.5 * $stdzoom]
	if { $nxl < 0.0 } then {
	    set nxl 0.0
	    set nxr [expr 2.0 * $xl]
	} 
	if { $nxr > 1.0 } then {
	    set nxr 1.0
	    set nxl [expr 2.0 * $xl - 1.0]
	}
	set xl $nxl
	set xr $nxr

	set nyl [expr $yl - 0.5 * $stdzoom]
	set nyr [expr $yl + 0.5 * $stdzoom]
	if { $nyl < 0.0 } then {
	    set nyl 0.0
	    set nyr [expr 2.0 * $yl]
	}
	if { $nyr > 1.0 } then {
	    set nyr 1.0
	    set nyl [expr 2.0 * $yl - 1.0]
	}
	set yl $nyl
	set yr $nyr
    }

# Adjust arguments to be in bounds (in case margins are in effect).

    set bounds [$w.plwin view bounds]
    set xmin [lindex "$bounds" 0]
    set ymin [lindex "$bounds" 1]
    set xmax [lindex "$bounds" 2]
    set ymax [lindex "$bounds" 3]

    set xl [max $xmin [min $xmax $xl]]
    set yl [max $ymin [min $ymax $yl]]
    set xr [max $xmin [min $xmax $xr]]
    set yr [max $ymin [min $ymax $yr]]

# Only create scrollbars if really needed.

    set hscroll [expr ($xl != $xmin) || ($xr != $xmax)]
    set vscroll [expr ($yl != $ymin) || ($yr != $ymax)]

    if { ! ($hscroll || $vscroll)} then {
	$w.plwin redraw
	return
    }

# Select plot region

    $w.plwin view zoom $xl $yl $xr $yr

# Fix up view

    plw_fixview $w $hscroll $vscroll

# Add window to zoom windows list

    global zidx zidx_max zxl zyl zxr zyr

    incr zidx($w)
    set zidx_max($w) $zidx($w)

    set coords [$w.plwin view]
    set zxl($w,$zidx($w)) [lindex "$coords" 0]
    set zyl($w,$zidx($w)) [lindex "$coords" 1]
    set zxr($w,$zidx($w)) [lindex "$coords" 2]
    set zyr($w,$zidx($w)) [lindex "$coords" 3]
}

#----------------------------------------------------------------------------
# plw_zoom_back
#
# Traverses the zoom windows list backward.
#----------------------------------------------------------------------------

proc plw_zoom_back {w} {
    
    global zidx zxl zyl zxr zyr

    if { $zidx($w) == 0 } then return

    incr zidx($w) -1

    set xl $zxl($w,$zidx($w))
    set yl $zyl($w,$zidx($w))
    set xr $zxr($w,$zidx($w))
    set yr $zyr($w,$zidx($w))

# Select plot region

    $w.plwin view select $xl $yl $xr $yr
}

#----------------------------------------------------------------------------
# plw_zoom_forward
#
# Traverses the zoom windows list forward.
#----------------------------------------------------------------------------

proc plw_zoom_forward {w} {
    
    global zidx zidx_max zxl zyl zxr zyr

    if { $zidx_max($w) == 0 || $zidx($w) == $zidx_max($w) } then return

    incr zidx($w)

    set xl $zxl($w,$zidx($w))
    set yl $zyl($w,$zidx($w))
    set xr $zxr($w,$zidx($w))
    set yr $zyr($w,$zidx($w))

# Select plot region

    $w.plwin view select $xl $yl $xr $yr
}

#----------------------------------------------------------------------------
# plw_view_scroll
#
# Scrolls view incrementally.
# Similar to clicking on arrow at end of scrollbar (but speed is user
# controllable).
#----------------------------------------------------------------------------

proc plw_view_scroll {w dx dy s} {
    global key_scroll_mag
    global key_scroll_speed

# Set up multiplication factor

    set mult $key_scroll_speed
    if { $s & 0x01 } then {
	set mult [expr $mult * $key_scroll_mag]
    }
    if { $s & 0x02 } then {
	set mult [expr $mult * $key_scroll_mag]
    }
    if { $s & 0x04 } then {
	set mult [expr $mult * $key_scroll_mag]
    }
    if { $s & 0x08 } then {
	set mult [expr $mult * $key_scroll_mag]
    }

# Now scroll

    if {($dx != 0) && \
	    [winfo exists $w.hbar] && [winfo ismapped $w.hbar] } then {

	set dx [expr $dx * $mult]
	set first  [lindex [$w.hbar.hscroll get] 2]
	$w.plwin xscroll [expr $first+$dx]
    }
    if {($dy != 0) && \
	    [winfo exists $w.vscroll] && [winfo ismapped $w.vscroll] } then {

	set dy [expr $dy * $mult]
	set first  [lindex [$w.vscroll get] 2]
	$w.plwin yscroll [expr $first+$dy]
    }
}

#----------------------------------------------------------------------------
# plw_fixview
#
# Handles updates of scrollbars & plot after view change.
#----------------------------------------------------------------------------

proc plw_fixview {w hscroll vscroll} {
    
# Create scrollbars if they don't already exist.

    set created_sb 0
    if { $hscroll && ! [winfo exists $w.hbar] } then {
	set created_sb 1
	frame $w.hbar
	scrollbar $w.hbar.hscroll -relief sunken -orient horiz \
	    -command "$w.plwin xscroll"
	$w.plwin config -xscroll "$w.hbar.hscroll set"
	frame $w.hbar.pad
	pack $w.hbar.hscroll -side left -fill x -expand 1
	pack $w.hbar.pad -side right
    }
    if { $vscroll && ! [winfo exists $w.vscroll] } then {
	set created_sb 1
	scrollbar $w.vscroll -relief sunken \
	    -command "$w.plwin yscroll"
	$w.plwin config -yscroll "$w.vscroll set"
    }

# When scrollbars are first created, it may be necessary to unmap then map
# the plframe widget so that it has a chance to initialize the scrollbars
# before they are mapped.

    if { $created_sb } then {
	pack unpack $w.plwin
	pack append $w $w.plwin {left expand fill}
    }

# Map scrollbars if not already mapped.
# To get packing right, need to unmap then remap plot widget.
# Otherwise need to do explicit redraw.

    if { ($hscroll && ! [winfo ismapped $w.hbar]) || \
         ($vscroll && ! [winfo ismapped $w.vscroll]) } then {

	update
	pack unpack $w.plwin
	if { $hscroll } then {
	    pack append $w $w.hbar {bottom fillx}
	}
	if { $vscroll } then {
	    pack append $w $w.vscroll {right filly}
	}
	pack append $w $w.plwin {expand fill}

	if {$hscroll && $vscroll} {
	    $w.hbar.pad configure -width [winfo reqwidth $w.vscroll]
	}

    } else {
	$w.plwin redraw
    }
}

#----------------------------------------------------------------------------
# plw_update_view
#
# Updates view.  Results in scrollbars being added if they are appropriate.
# Does nothing if the plot window is unchanged from the default.
#----------------------------------------------------------------------------

proc plw_update_view {w} {
    set coords [$w.plwin view]

    set xl [lindex "$coords" 0]
    set yl [lindex "$coords" 1]
    set xr [lindex "$coords" 2]
    set yr [lindex "$coords" 3]

    plw_view_select $w $xl $yl $xr $yr
}

#----------------------------------------------------------------------------
# status_msg
#
# Used for temporarily flashing a status message in the status bar.  Better
# than a dialog because it can be ignored and will go away on its own.
#----------------------------------------------------------------------------

proc status_msg {w msg} {

    plw_label_set $w $msg
    after 2500 plw_label_reset $w
}

#----------------------------------------------------------------------------
# plw_label_reset
#
# Resets message in status bar to the default.
#----------------------------------------------------------------------------

proc plw_label_reset {w} {
global plot_menu_on
if $plot_menu_on then {	# jc:
    $w.ftop.lstat configure -text " [string range $w 1 end]" }
}

#----------------------------------------------------------------------------
# plw_label_set
#
# Sets message in status bar.
#----------------------------------------------------------------------------

proc plw_label_set {w msg} {
global plot_menu_on
if $plot_menu_on then {	# jc:
    $w.ftop.lstat configure -text " $msg" }
}

#----------------------------------------------------------------------------
# plw_dplink
#
# Initializes socket data link between widget and client code.
# In addition, as this is the last client/server connection needed, I
# disable further connections.
#----------------------------------------------------------------------------

proc plw_dplink {w client} {

    global list_sock data_sock

    dp_Host +
    set rv [dp_connect -server 0]
    set list_sock [lindex $rv 0]
    set data_port [lindex $rv 1]

    dp_RDO $client set data_port $data_port
    set data_sock [lindex [dp_accept $list_sock] 0]
    $w.plwin openlink socket $data_sock
    dp_Host -
}
