# $Id$
# $Log$
# Revision 1.10  1993/08/31 20:12:45  mjl
# Added file name globbing to save file name retrieval.  Can now save to
# ~/blob or whatever.  Using a wildcard will result in the first matching
# name to be used.
#
# Revision 1.9  1993/08/28  06:33:08  mjl
# Changed all send commands to go through the plw_send proc, which (a) puts
# send commands in background (to speed interpretation) and (b) catches any
# errors encountered (such as a lack of response by the remote interpreter).
# This prevents error messages from the plserver interpreter when attached
# to an unresponsive client interpreter (such as would happen with many
# number-crunching codes).
#
# Revision 1.8  1993/08/20  19:39:03  mjl
# Many minor adjustments on how the plot view is set up and scrolled.
# Should now work intuitively and accurately for any combination of zooming
# and page settings.  Added proc that adds scrollbars if necessary given
# the current window settings, to be used from TK driver.
#
# Revision 1.7  1993/08/18  20:26:34  mjl
# Added print menu.  Grouped Save operations under a single cascading menu.
# Added a status message (timed) to appear in status bar when successfully
# saving a plot or closing the plot file.  Changed Page setup to use
# new device variables (margin, aspect, jx, jy).  Switched over to new
# Form2d proc for direct entry of window or page setup variables.
#
# Revision 1.6  1993/08/13  06:42:36  mjl
# Added bulletproofing for when idiot user selects "Cancel" in GetItem.
#
# Revision 1.5  1993/08/09  22:22:18  mjl
# Added "Close" entry in Plot menu, for closing out save file.
#
# Revision 1.4  1993/07/31  08:04:31  mjl
# Documentation changes only.
#
# Revision 1.3  1993/07/28  05:43:41  mjl
# Changed << and >> buttons when running plrender to simulate keyboard
# input, by sending a <Backspace> and <CR>, respectively (works better
# this way).

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

    pack append $w.ftop \
	[label $w.ftop.lstat -anchor w -relief raised -text $w] \
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
	-label "Save  =>" \
	-menu $w.ftop.pmenu.m.save

    menu $w.ftop.pmenu.m.save

# Save - As.. (another cascade)
#
# This cascade menubutton doesn't have its entries filled in until the
# plframe is initialized, because we need to query the widget to find out
# the available output devices (which are listed).

    $w.ftop.pmenu.m.save add cascade \
	-label "As  =>" \
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
	-label "Orient  =>" \
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
	-label "Zoom  =>" \
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
	-label "Page   =>" \
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

    $w.ftop.fp configure -command "plw_send $client {keypress Return}"

# Configure back page button, plrender only

    if {[info exists is_plrender]} {
	$w.ftop.bp configure -command "plw_send $client {keypress BackSpace}"
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
    
# Give client name to plplot widget.

    $w.plwin attach $client

# Enable keyboard traversal when widget has the input focus.

    tk_bindForTraversal $w.plwin

# Bindings

    bind $w.plwin <KeyPress> "plw_send $client {keypress %K %N %A}"
    bind $w.plwin <Any-Enter> "focus $w.plwin"

# Inform client of plplot widget name for widget commands.

    plw_send $client "set plwidget $w.plwin"

# I want the focus

    focus $w.plwin
}

#----------------------------------------------------------------------------
# plw_flash
#
# Set end-of-page indicator to help user out.
#----------------------------------------------------------------------------

proc plw_flash {w} {

    set bg [lindex [$w.ftop.leop config -bg] 4]
    if {$bg == "gray"} {
	set default [lindex [$w.ftop.lstat config -bg] 4]
	$w.ftop.leop config -bg $default
    } else {
	$w.ftop.leop config -bg gray
    }
}

#----------------------------------------------------------------------------
# plw_end
#
# Executed as part of orderly shutdown procedure.
#----------------------------------------------------------------------------

proc plw_end {w} {
    $w.plwin detach
}

#----------------------------------------------------------------------------
# plw_send
#
# Send string to client.  Does it in the background and catches errors
# (if client is busy it won't respond).
#----------------------------------------------------------------------------

proc plw_send {client msg} {
    after 1 catch [list "send [list $client] $msg"]
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
#----------------------------------------------------------------------------

proc plw_saveas {w dev} {
    set file [lindex [glob -nocomplain [getItem "Enter file name"]] 0]
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
    $w.ftop.lstat configure -text \
	"Zoom: Click on upper left hand corner of zoom region."
    bind $w.plwin <ButtonPress> "pl_zoom_start $w %x %y"
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

    Form2d .e "window coordinates for zoom"
    tkwait window .e

    pl_view_select $w $fv00 $fv01 $fv10 $fv11
}

#----------------------------------------------------------------------------
# plw_zoom_reset
#
# Resets after zoom.
# Note that an explicit redraw is not necessary since the packer issues a
# resize after the scrollbars are unmapped.
#----------------------------------------------------------------------------

proc plw_zoom_reset {w} {
    $w.ftop.lstat configure -text $w
    bind $w.plwin <ButtonPress> {}
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

    Form2d .e "area of page to use for plotting"
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
# pl_zoom_start
#
# Starts plot zoom.
#----------------------------------------------------------------------------

proc pl_zoom_start {w wx wy} {
    bind $w.plwin <ButtonPress> {}
    $w.ftop.lstat configure -text \
	"Select zoom region by dragging mouse, then release."

    $w.plwin draw init
    bind $w.plwin <B1-ButtonRelease> "pl_zoom_select $w $wx $wy %x %y"
    bind $w.plwin <B1-Motion> "pl_zoom_draw $w $wx $wy %x %y"
}

#----------------------------------------------------------------------------
# pl_zoom_draw
#
# Draws zoom box.
#----------------------------------------------------------------------------

proc pl_zoom_draw {w wx0 wy0 wx1 wy1} {
    $w.plwin draw rect $wx0 $wy0 $wx1 $wy1
}

#----------------------------------------------------------------------------
# pl_zoom_select
#
# Selects region for zoom.
#----------------------------------------------------------------------------

proc pl_zoom_select {w wx0 wy0 wx1 wy1} {
    
# Finish rubber band draw

    bind $w.plwin <B1-ButtonRelease> {}
    bind $w.plwin <B1-Motion> {}
    $w.ftop.lstat configure -text "$w"

    set wlx [winfo width $w.plwin]
    set wly [winfo height $w.plwin]
    set xl [expr "$wx0/$wlx."     ]
    set xr [expr "$wx1/$wlx."     ]
    set yl [expr "1 - $wy1/$wly." ]
    set yr [expr "1 - $wy0/$wly." ]

    $w.plwin draw end

# Select new plot region

    pl_view_zoom $w $xl $yl $xr $yr
}

#----------------------------------------------------------------------------
# pl_view_select
#
# Handles change of view into plot.
# Given in relative plot window coordinates.
#----------------------------------------------------------------------------

proc pl_view_select {w x0 y0 x1 y1} {
    
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

    pl_fixview $w $hscroll $vscroll
}

#----------------------------------------------------------------------------
# pl_view_zoom
#
# Handles zoom.
# Given in relative device coordinates.
#----------------------------------------------------------------------------

proc pl_view_zoom {w x0 y0 x1 y1} {
    
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

    pl_fixview $w $hscroll $vscroll
}

#----------------------------------------------------------------------------
# pl_fixview
#
# Handles updates of scrollbars & plot after view change.
#----------------------------------------------------------------------------

proc pl_fixview {w hscroll vscroll} {
    
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

    pl_view_select $w $xl $yl $xr $yr
}

#----------------------------------------------------------------------------
# status_msg
#
# Used for temporarily flashing a status message in the status bar.  Better
# than a dialog because it can be ignored and will go away on its own.
#----------------------------------------------------------------------------

proc status_msg {w msg} {

    $w.ftop.lstat configure -text "$msg"
    after 2500 "$w.ftop.lstat configure -text $w"
}

