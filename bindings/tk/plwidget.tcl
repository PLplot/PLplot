# $Id$
# $Log$
# Revision 1.5  1993/08/09 22:22:18  mjl
# Added "Close" entry in Plot menu, for closing out save file.
#
# Revision 1.4  1993/07/31  08:04:31  mjl
# Documentation changes only.
#
# Revision 1.3  1993/07/28  05:43:41  mjl
# Changed << and >> buttons when running plrender to simulate keyboard
# input, by sending a <Backspace> and <CR>, respectively (works better
# this way).
#
# Revision 1.2  1993/07/16  22:06:40  mjl
# Changed top row of widgets.  Now most commands accessible through the "plot"
# menu.  Many additions, including save, save as, orient.., zoom.., page..,
# and so on.  Procs added to handle the new options as well as support for
# scrollbars on zooms.
#
# Revision 1.1  1993/07/02  06:58:35  mjl
# The new TCL/TK driver!  Yes it's finally here!  YAAAAAAAAYYYYYYY!!!

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
# Save
#-------

    $w.ftop.pmenu.m add command \
	-label "Save" \
	-command "plw_save $w"

#-------
# Close
#-------

    $w.ftop.pmenu.m add command \
	-label "Close" \
	-command "plw_close $w"

#----------
# Save As
#----------

    $w.ftop.pmenu.m add cascade \
	-label "Save As  =>" \
	-menu $w.ftop.pmenu.m.saveas

    menu $w.ftop.pmenu.m.saveas

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
	-label "Enter bounds.." \
	-command "plw_page_enter $w"

# Page - reset

    $w.ftop.pmenu.m.page add command \
	-label "Reset" \
	-command "plw_page_reset $w"

#---------
# Redraw 
#---------

    $w.ftop.pmenu.m add command \
	-label "Redraw" \
	-command "$w.plwin redraw"

# The end

    return $w.ftop.pmenu
}

#----------------------------------------------------------------------------
# plw_configure_TopRow
#
# Finish configuration of top row widgets.
# Right now it just generates the device list in the "Save As" widget menu.
#----------------------------------------------------------------------------

proc plw_configure_TopRow {w} {
    update
    set j 0
    foreach i [$w.plwin info devices] {
	$w.ftop.pmenu.m.saveas add command \
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

    $w.ftop.fp configure -command \
	"send [list $client] keypress Return"

# Configure back page button, plrender only

    if {[info exists is_plrender]} {
	$w.ftop.bp configure -command \
	    "send [list $client] keypress BackSpace"
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

    bind $w.plwin <KeyPress> "send [list $client] keypress %K %N %A"
    bind $w.plwin <Any-Enter> "focus $w.plwin"

# Inform client of plplot widget name for widget commands.

    send $client "set plwidget $w.plwin"

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
# plw_save
#
# Saves plot to an already open file.  If none open, issues an error dialog.
#----------------------------------------------------------------------------

proc plw_save {w} {
    if { [catch "$w.plwin save" foo]} {
	bogon_alert "$foo"
    }
}

#----------------------------------------------------------------------------
# plw_close
#
# Close save file.
#----------------------------------------------------------------------------

proc plw_close {w} {
    if { [catch "$w.plwin save close" foo]} {
	bogon_alert "$foo"
    }
}

#----------------------------------------------------------------------------
# plw_saveas
#
# Saves plot to specified device.
#----------------------------------------------------------------------------

proc plw_saveas {w dev} {
    set file [getItem "Enter file name"]
    if { [file exists $file] } {
	if { ! [confirm "File $file already exists.  Are you sure?"] } {
	    return
	}
    }
    $w.plwin save as $dev $file
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
    global xmin ymin xmax ymax

    set coords [$w.plwin view]
    set xmin [lindex "$coords" 0]
    set ymin [lindex "$coords" 1]
    set xmax [lindex "$coords" 2]
    set ymax [lindex "$coords" 3]

    EnterCoords .e "window coordinates for zoom"
    tkwait window .e

    pl_view $w select $xmin $ymin $xmax $ymax
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
    if {[winfo ismapped $w.hscroll]} then {
	pack unpack $w.hscroll
    }
    if {[winfo exists $w.vscroll]} then {
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
# Changes window into device space.
#----------------------------------------------------------------------------

proc plw_page_enter {w} {
    global xmin ymin xmax ymax

    set coords [$w.plwin page]
    set xmin [lindex "$coords" 0]
    set ymin [lindex "$coords" 1]
    set xmax [lindex "$coords" 2]
    set ymax [lindex "$coords" 3]

    EnterCoords .e "area of page to use for plotting"
    tkwait window .e

    $w.plwin page $xmin $ymin $xmax $ymax
}

#----------------------------------------------------------------------------
# plw_page_reset
#
# Resets window into device space.
#----------------------------------------------------------------------------

proc plw_page_reset {w} {
    $w.plwin page 0. 0. 1. 1.
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
    set xmin [expr "$wx0/$wlx."     ]
    set xmax [expr "$wx1/$wlx."     ]
    set ymin [expr "1 - $wy1/$wly." ]
    set ymax [expr "1 - $wy0/$wly." ]

    $w.plwin draw end

# Select new plot region

    pl_view $w zoom $xmin $ymin $xmax $ymax
}

#----------------------------------------------------------------------------
# pl_view
#
# Handles change of view into plot.
#----------------------------------------------------------------------------

proc pl_view {w op x0 y0 x1 y1} {
    
# Adjust arguments to be in bounds and properly ordered (xmin < xmax, etc)

    set xmin [min $x0 $x1]
    set ymin [min $y0 $y1]
    set xmax [max $x0 $x1]
    set ymax [max $y0 $y1]

    set xmin [max 0. [min 1. $xmin]]
    set ymin [max 0. [min 1. $ymin]]
    set xmax [max 0. [min 1. $xmax]]
    set ymax [max 0. [min 1. $ymax]]

# If bounds are (0,0) (1,1), just return.

    if {($xmin == 0) && ($xmax == 1)} \
    then {set hscroll 0} else {set hscroll 1}

    if {($ymin == 0) && ($ymax == 1)} \
    then {set vscroll 0} else {set vscroll 1}

    if { ! ($hscroll || $vscroll)} {return}

# Select plot region

    $w.plwin view $op $xmin $ymin $xmax $ymax

# Create scrollbars if they don't already exist.

    if {$hscroll && ! [winfo exists $w.hscroll]} then {
	scrollbar $w.hscroll -relief sunken -orient horiz \
	    -command "$w.plwin xscroll"
	$w.plwin config -xscroll "$w.hscroll set"
    }
    if {$vscroll && ! [winfo exists $w.vscroll]} then {
	scrollbar $w.vscroll -relief sunken \
	    -command "$w.plwin yscroll"
	$w.plwin config -yscroll "$w.vscroll set"
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

