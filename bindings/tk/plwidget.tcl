# $Id$
# $Log$
# Revision 1.22  1994/04/25 19:00:34  mjl
# Added "Options" cascade menu, along with "Palette 0" and "Palette 1"
# entries.  Added shift and control modifiers to the key bindings
# responsible for calling the key_filter proc.
#
# Revision 1.21  1994/04/08  11:58:50  mjl
# Changes to support new mouse handler, keyboard filter, scrolling using
# cursor keys.  eop label should now flash correctly.
#
# Revision 1.20  1994/02/07  22:58:11  mjl
# Eliminated plw_flash in favor of plw_bop and plw_eop.  Added widget
# configure commands to set bop/eop behavior.
#
# Revision 1.19  1994/01/17  21:30:24  mjl
# Improved security for Tcl-DP communication by disabling any further
# connections after the initial client/server handshaking is finished.
#
# Revision 1.18  1994/01/15  17:41:52  mjl
# Improved handling of status label widget.  Added proc to handle
# initialization of data link via socket.
#
# Revision 1.17  1993/12/22  23:09:51  mjl
# Changes so that TK driver no longer times out on slow network connections
# (it's just rreeaaalllyyyy ssllooowwww).  Where timeouts are a problem,
# the client must issue the command to the server without waiting for a
# reply, and then sit in TK wait loop until some specified condition is
# met (usually the server sets a client interpreter variable when done).
#
# Revision 1.16  1993/12/21  10:22:44  mjl
# Now route ALL communication to the client through the client_cmd proc.
# This part reworked to not suck when using Tcl-DP.
#
# Revision 1.15  1993/12/15  09:02:13  mjl
# Changes to support Tcl-DP style communication; eliminated plframe
# widget attach and detach commands (no longer supported).

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
# Create the plplot widget container frame as well as the plplot widget
# and any desired siblings.  Note the client may modify widget parameters
# using configure.
#----------------------------------------------------------------------------

proc plw_create {w {pack {bottom expand fill}}} {
    
# Make container frame & map into the server window.

    frame $w
    pack append [winfo parent $w] $w $pack

# Make frame for top row widgets.

    plw_create_TopRow $w
    pack append $w \
	$w.ftop {top fill}

# Make plplot widget.

    plframe $w.plwin -relief sunken
    pack append $w \
	$w.plwin {left expand fill}

# Do remaining configure of top row.

    plw_configure_TopRow $w
}

#----------------------------------------------------------------------------
# plr_create
#
# A front-end to plw_create, used by plrender.
#----------------------------------------------------------------------------

proc plr_create {w {pack {bottom expand fill}}} {
    global is_plrender

    set is_plrender 1
    plw_create $w $pack
}

#----------------------------------------------------------------------------
# plw_create_TopRow
#
# Create top row widgets.
#----------------------------------------------------------------------------

proc plw_create_TopRow {w} {
    global is_plrender

    frame $w.ftop

# End of page indicator

    pack append $w.ftop \
	[label $w.ftop.leop -relief raised] \
	{left fill padx 12}

# Plot menu

    pack append $w.ftop \
	[plw_create_pmenu $w] \
	{left fill padx 12}

# Backward page button, plrender only.

    if {[info exists is_plrender]} {
	pack append $w.ftop \
	    [button $w.ftop.bp -text "<<" -relief raised] \
	    {left fill padx 10}
    }

# Forward page button

    pack append $w.ftop \
	[button $w.ftop.fp -text ">>" -relief raised] \
	{left fill padx 10}

# Make label widget for status messages.

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
# It is tempting to create buttons for some of these options, but buttons are
# difficult to effectively place and extend.  Menus have a clear placement
# mechanism and are easy to add to.  Further, TK menus can be torn off
# (select menu with middle mouse button and move to where you want it) which
# makes selecting top-level menu buttons easy.
#----------------------------------------------------------------------------

proc plw_create_pmenu {w} {

    global cascade_arrow

    menubutton $w.ftop.pmenu -menu $w.ftop.pmenu.m \
	-text "Plot" \
	-relief raised

    menu $w.ftop.pmenu.m

#-------
# Print
#-------

    $w.ftop.pmenu.m add command \
	-label "Print" \
	-command "plw_print $w"

#-----------------
# Save (cascade)
#-----------------

    $w.ftop.pmenu.m add cascade \
	-label "Save  $cascade_arrow" \
	-menu $w.ftop.pmenu.m.save

    menu $w.ftop.pmenu.m.save

# Save - As.. (another cascade)
#
# This cascade menubutton doesn't have its entries filled in until the
# plframe is initialized, because we need to query the widget to find out
# the available output devices (which are listed).

    $w.ftop.pmenu.m.save add cascade \
	-label "As  $cascade_arrow" \
	-menu $w.ftop.pmenu.m.save.as

    menu $w.ftop.pmenu.m.save.as

# Save - Again

    $w.ftop.pmenu.m.save add command \
	-label "Again" \
	-command "plw_save $w"

# Save - Close

    $w.ftop.pmenu.m.save add command \
	-label "Close" \
	-command "plw_close $w"

#-----------------
# Orient (cascade)
#-----------------

    $w.ftop.pmenu.m add cascade \
	-label "Orient  $cascade_arrow" \
	-menu $w.ftop.pmenu.m.orient

    menu $w.ftop.pmenu.m.orient

# Orient - 0 degrees

    $w.ftop.pmenu.m.orient add radio \
	-label "0 degrees" \
	-command "plw_orient $w 0"

# Orient - 90 degrees

    $w.ftop.pmenu.m.orient add radio \
	-label "90 degrees" \
	-command "plw_orient $w 1"

# Orient - 180 degrees

    $w.ftop.pmenu.m.orient add radio \
	-label "180 degrees" \
	-command "plw_orient $w 2"

# Orient - 270 degrees

    $w.ftop.pmenu.m.orient add radio \
	-label "270 degrees" \
	-command "plw_orient $w 3"

#-----------------
# Zoom (cascade)
#-----------------

    $w.ftop.pmenu.m add cascade \
	-label "Zoom  $cascade_arrow" \
	-menu $w.ftop.pmenu.m.zoom

    menu $w.ftop.pmenu.m.zoom

# Zoom - select (by mouse)

    $w.ftop.pmenu.m.zoom add command \
	-label "Select" \
	-command "plw_zoom_select $w"

# Zoom - enter bounds

    $w.ftop.pmenu.m.zoom add command \
	-label "Enter bounds.." \
	-command "plw_zoom_enter $w"

# Zoom - reset

    $w.ftop.pmenu.m.zoom add command \
	-label "Reset" \
	-command "plw_zoom_reset $w"

#------------------------
# Set up page (cascade)
#------------------------

    $w.ftop.pmenu.m add cascade \
	-label "Page   $cascade_arrow" \
	-menu $w.ftop.pmenu.m.page

    menu $w.ftop.pmenu.m.page

# Page - enter bounds

    $w.ftop.pmenu.m.page add command \
	-label "Setup.." \
	-command "plw_page_enter $w"

# Page - reset

    $w.ftop.pmenu.m.page add command \
	-label "Reset" \
	-command "plw_page_reset $w"

#---------
# Redraw (only for debugging)
#---------
#
#    $w.ftop.pmenu.m add command \
#	-label "Redraw" \
#	-command "$w.plwin redraw"

#------------------
# Options (cascade)
#------------------

    $w.ftop.pmenu.m add cascade \
	-label "Options  $cascade_arrow" \
	-menu $w.ftop.pmenu.m.options

    menu $w.ftop.pmenu.m.options

    $w.ftop.pmenu.m.options add command \
	-label "Palette 0" \
	-command "plcmap0_edit $w" 

    $w.ftop.pmenu.m.options add command \
	-label "Palette 1" \
	-command "plcmap1_edit $w" 

#    $w.ftop.pmenu.m.options add separator

#    $w.ftop.pmenu.m.options add command \
#	-label "Load Configuration" \
#	-command {null_command "Load Configuration"} 

#    $w.ftop.pmenu.m.options add command \
#	-label "Save Configuration" \
#	-command {null_command "Save Configuration"} 

#    $w.ftop.pmenu.m.options add command \
#	-label "Save Configuration As..." \
#	-command {null_command "Save Configuration As..."} 

# Los endos

    return $w.ftop.pmenu
}

#----------------------------------------------------------------------------
# plw_configure_TopRow
#
# Finish configuration of top row widgets.
# Right now it just generates the device list in the "Save/As" widget menu.
#----------------------------------------------------------------------------

proc plw_configure_TopRow {w} {
    update
    set j 0
    foreach i [$w.plwin info devices] {
	$w.ftop.pmenu.m.save.as add command \
	    -label $i \
	    -command "plw_saveas $w $j"
	set j [expr "$j + 1"]
    }
}

#----------------------------------------------------------------------------
# plw_init
#
# Initialize the container frame and child widgets.
# All the bindings as well as communication links to the client are made here.
#
# Note: when passing a TCL command as a string argument, it is necessary to
# protect the $client variable from becoming two tokens if it has embedded
# spaces (such as occurs when you have multiple copies running).  The [list
# $client] construct will enclose $client with a pair of braces if necessary
# (can't do it directly since braces prevent variable expansion).
#----------------------------------------------------------------------------

proc plw_init {w client} {
    global is_plrender

# Configure forward page button

    $w.ftop.fp configure \
	-command "client_cmd [list $client] {keypress Return}"

# Configure back page button, plrender only

    if {[info exists is_plrender]} {
	$w.ftop.bp configure \
	    -command "client_cmd [list $client] {keypress BackSpace}"
    }

# Initialize plplot widget

    plw_init_plplot $w $client
}

#----------------------------------------------------------------------------
# plw_init_plplot
#
# Initialize plplot widget.
#----------------------------------------------------------------------------

proc plw_init_plplot {w client} {
    
# Enable keyboard traversal when widget has the input focus.

    tk_bindForTraversal $w.plwin

# Bindings

    bind $w.plwin <Any-KeyPress> \
	"key_filter $w [list $client] %K %N %A 0 0"

    bind $w.plwin <Shift-KeyPress> \
	"key_filter $w [list $client] %K %N %A 1 0"

    bind $w.plwin <Control-KeyPress> \
	"key_filter $w [list $client] %K %N %A 0 1"

    bind $w.plwin <Shift-Control-KeyPress> \
	"key_filter $w [list $client] %K %N %A 1 1"

    bind $w.plwin <Any-ButtonPress> \
	"plw_user_mouse $w [list $client] %b %s %x %y"

    bind $w.plwin <Any-Enter> \
	"focus $w.plwin"

# Inform client of plplot widget name for widget commands.

    client_cmd $client "set plwidget $w.plwin"

# Set up bop/eop signal.

    $w.plwin configure -bopcmd "plw_bop $w"
    $w.plwin configure -eopcmd "plw_eop $w"

# I want the focus

    focus $w.plwin
}

#----------------------------------------------------------------------------
# plw_start
#
# Responsible for plplot graphics package initialization on the widget.
#
# Put here to reduce the possibility of a time out over a slow network --
# the client program waits until the variable widget_is_ready is set.
#----------------------------------------------------------------------------

proc plw_start {w client} {

# Crank up graphics package

    $w.plwin cmd init

# Inform client that we're done.

    client_cmd $client "set widget_is_ready 1"
}

#----------------------------------------------------------------------------
# plw_bop
#
# Set button color to indicate we have started a new page.
#----------------------------------------------------------------------------

proc plw_bop {w} {
    $w.ftop.leop config -bg [option get $w.ftop.leop off Label]
    update idletasks
}

#----------------------------------------------------------------------------
# plw_eop
#
# Set button color to indicate we are at the end of a page.
#----------------------------------------------------------------------------

proc plw_eop {w} {
    $w.ftop.leop config -bg [option get $w.ftop.leop on Label]
    update idletasks
}

#----------------------------------------------------------------------------
# plw_end
#
# Executed as part of orderly shutdown procedure.  Eventually will just
# destroy the plframe and surrounding widgets, and server will exit only
# if all plotting widgets have been destroyed and it is a child of the
# plplot/TK driver.
#----------------------------------------------------------------------------

proc plw_end {w} {
    exit
}

#----------------------------------------------------------------------------
# plw_print
#
# Prints plot.  Uses the "plpr" script, which must be set up for your site
# as appropriate.  There are better ways to do this but all are too
# dangerous IMO until Tcl/TK gets some adequate security safeguards in
# place.
#----------------------------------------------------------------------------

proc plw_print {w} {
    if { [catch "$w.plwin print" foo]} {
	bogue_out "$foo"
    } else {
	status_msg $w "Plot printed."
    }
}

#----------------------------------------------------------------------------
# plw_saveas
#
# Saves plot to specified device.
# I have to go through a bit of trickery to get "~" expanded, since the
# Tcl 7.0 glob no longer expands names if the file doesn't already exist.
#----------------------------------------------------------------------------

proc plw_saveas {w dev} {
    set file [getItem "Enter file name"]
    if { [string index $file 0] == "~" } {
	set file [glob ~][string trimleft $file ~]
    }
    if { [string length $file] > 0 } {
	if { [file exists $file] } {
	    if { ! [confirm "File $file already exists.  Are you sure?"] } {
		return
	    }
	}
	if { [catch "$w.plwin save as $dev $file" foo]} {
	    bogue_out "$foo"
	} else {
	    status_msg $w "Plot saved."
	}
    } else {
	bogue_out "No file specified"
    }
}

#----------------------------------------------------------------------------
# plw_save
#
# Saves plot to an already open file.  If none open, issues an error dialog.
#----------------------------------------------------------------------------

proc plw_save {w} {
    if { [catch "$w.plwin save" foo]} {
	bogue_out "$foo"
    } else {
	status_msg $w "Plot saved."
    }
}

#----------------------------------------------------------------------------
# plw_close
#
# Close save file.
#----------------------------------------------------------------------------

proc plw_close {w} {
    if { [catch "$w.plwin save close" foo]} {
	bogue_out "$foo"
    } else {
	status_msg $w "Plot file closed."
    }
}

#----------------------------------------------------------------------------
# plw_zoom_select
#
# Zooms plot in response to mouse selection.
#----------------------------------------------------------------------------

proc plw_zoom_select {w} {
    global def_button_cmd

    set def_button_cmd [bind $w.plwin <ButtonPress>]
    plw_label_set $w "Zoom: Click on upper left hand corner of zoom region."
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
    if {[winfo exists $w.hscroll] && [winfo ismapped $w.hscroll]} then {
	pack unpack $w.hscroll
    }
    if {[winfo exists $w.vscroll] && [winfo exists $w.vscroll]} then {
	pack unpack $w.vscroll
    }
}

#----------------------------------------------------------------------------
# plw_orient
#
# Changes plot orientation.
#----------------------------------------------------------------------------

proc plw_orient {w rot} {
    $w.plwin orient $rot
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
# plw_zoom_mouse_draw
#
# Draws zoom box in response to mouse motion (with button held down).
#----------------------------------------------------------------------------

proc plw_zoom_mouse_draw {w wx0 wy0 wx1 wy1} {
    $w.plwin draw rect $wx0 $wy0 $wx1 $wy1
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

    set wlx [winfo width $w.plwin]
    set wly [winfo height $w.plwin]
    set xl [expr "$wx0/$wlx."     ]
    set xr [expr "$wx1/$wlx."     ]
    set yl [expr "1 - $wy1/$wly." ]
    set yr [expr "1 - $wy0/$wly." ]

    $w.plwin draw end

# Select new plot region

    plw_view_zoom $w $xl $yl $xr $yr
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
    
# Adjust arguments to be in bounds and properly ordered (xl < xr, etc)

    set xl [min $x0 $x1]
    set yl [min $y0 $y1]
    set xr [max $x0 $x1]
    set yr [max $y0 $y1]

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

    if {($xl == $xmin) && ($xr == $xmax)} \
    then {set hscroll 0} else {set hscroll 1}

    if {($yl == $xmin) && ($yr == $xmax)} \
    then {set vscroll 0} else {set vscroll 1}

    if { ! ($hscroll || $vscroll)} then {
	$w.plwin redraw
	return
    }

# Select plot region

    $w.plwin view zoom $xl $yl $xr $yr

# Fix up view

    plw_fixview $w $hscroll $vscroll
}

#----------------------------------------------------------------------------
# plw_view_scroll
#
# Scrolls view incrementally.
# Similar to clicking on arrow at end of scrollbar (but speed is user
# controllable).
#----------------------------------------------------------------------------

proc plw_view_scroll {w dx dy} {
    
    if {($dx != 0) && \
	    [winfo exists $w.hscroll] && [winfo ismapped $w.hscroll]} then {

	set first  [lindex [$w.hscroll get] 2]
	$w.plwin xscroll [expr $first+$dx]
    }
    if {($dy != 0) && \
	    [winfo exists $w.vscroll] && [winfo ismapped $w.vscroll]} then {

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
    if {$hscroll && ! [winfo exists $w.hscroll]} then {
	set created_sb 1
	scrollbar $w.hscroll -relief sunken -orient horiz \
	    -command "$w.plwin xscroll"
	$w.plwin config -xscroll "$w.hscroll set"
    }
    if {$vscroll && ! [winfo exists $w.vscroll]} then {
	set created_sb 1
	scrollbar $w.vscroll -relief sunken \
	    -command "$w.plwin yscroll"
	$w.plwin config -yscroll "$w.vscroll set"
    }

# When scrollbars are first created, it may be necessary to unmap then map
# the plframe widget so that it has a chance to initialize the scrollbars
# before they are mapped.

    if {$created_sb} then {
	pack unpack $w.plwin
	pack append $w $w.plwin {left expand fill}
    }

# Map scrollbars if not already mapped.
# To get packing right, need to unmap then remap plot widget.
# Otherwise need to do explicit redraw.

    if {($hscroll && ! [winfo ismapped $w.hscroll]) || \
        ($vscroll && ! [winfo ismapped $w.vscroll]) } then {

	update
	pack unpack $w.plwin
	if {$hscroll} {
	    pack append $w $w.hscroll {bottom fillx}
	}
	if {$vscroll} {
	    pack append $w $w.vscroll {right filly}
	}
	pack append $w $w.plwin {expand fill}

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

    $w.ftop.lstat configure -text " [string range $w 1 end]"
}

#----------------------------------------------------------------------------
# plw_label_set
#
# Sets message in status bar.
#----------------------------------------------------------------------------

proc plw_label_set {w msg} {

    $w.ftop.lstat configure -text " $msg"
}

#----------------------------------------------------------------------------
# plw_user_mouse
#
# Passes buttonpress event information back to client.
# Written by Radey Shouman
#----------------------------------------------------------------------------

proc plw_user_mouse {w client button state x y} {

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

    client_cmd [list $client] [list mouse $button $state $xnd $ynd]
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
