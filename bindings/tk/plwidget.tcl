# $Id$
# $Log$
# Revision 1.1  1993/07/02 06:58:35  mjl
# The new TCL/TK driver!  Yes it's finally here!  YAAAAAAAAYYYYYYY!!!
#

#----------------------------------------------------------------------------
# PLPLOT TK/TCL graphics renderer
# plplot window initialization procs
# 
# Maurice LeBrun
# IFS, University of Texas at Austin
# 29-May-1993
#
# Note: to keep namespace wholesome, all procs defined here begin with
# "plwindow_" for button- or menu-accessible commands, or "pl_" for
# utility commands.
#----------------------------------------------------------------------------

#------------------------------------------------------------------------------
# Proc to create the plplot widget container frame as well as the plplot
# widget and any desired siblings.  Note the client may modify widget
# parameters using configure.
#------------------------------------------------------------------------------

proc plwindow_create {w {pack {bottom expand fill}}} {
    
# Make container frame & map into the server window.

    frame $w
    pack append [winfo parent $w] $w $pack

# Make frame for top row widgets.

    plwindow_create_TopRow $w

# Make plplot widget.

    plframe $w.plwin -relief sunken -background #000000

# Map child widgets into container frame.

    pack append $w \
	$w.ftop {top fill} \
	$w.plwin {left expand fill}
}

#------------------------------------------------------------------------------
# Proc to create top row widgets.
#------------------------------------------------------------------------------

proc plwindow_create_TopRow {w} {

    frame $w.ftop

# Make label widget for status messages.

    pack append $w.ftop \
	[label $w.ftop.lstat -anchor w -relief raised -text $w] \
	{right expand fill} 

# Make widget selector (for global operations).
#
#    pack append $w.ftop \
#	[radiobutton $w.ftop.select \
#	    -relief raised -variable $w.active_widget -value $w.plwin] {left}
#
#    $w.ftop.select invoke

# End of page indicator

    pack append $w.ftop \
	[label $w.ftop.leop -relief raised] \
	{left fill padx 12}

# Forward page button

    pack append $w.ftop \
	[button $w.ftop.fp -text ">>" -relief raised] \
	{left fill padx 10}

# Make radio buttons for zoom/pan/reset.

    pack append $w.ftop \
	[button $w.ftop.save -text "S" -relief raised] \
	{left fill padx 10} \
	[button $w.ftop.saveas -text "A" -relief raised] \
	{left fill padx 10} 

# Make radio buttons for zoom/pan/reset.

    pack append $w.ftop \
	[radiobutton $w.ftop.bz -text "Z" \
	 -relief raised -command "plwindow_zoom $w"] {left} \
	[radiobutton $w.ftop.br -text "R" \
	 -relief raised -command "plwindow_reset $w"] {left}

    $w.ftop.br select
}

#------------------------------------------------------------------------------
# Proc to initialize the container frame and child widgets.
# All the bindings as well as communication links to the client are made here.
#------------------------------------------------------------------------------

proc plwindow_init {w client} {
    
# Configure forward page button

    $w.ftop.fp configure -command "send $client set advance 1"

# Initialize plplot widget

    plwindow_init_plplot $w $client
}

#------------------------------------------------------------------------------
# Proc to initialize plplot widget.
#------------------------------------------------------------------------------

proc plwindow_init_plplot {w client} {
    
# Give client name to plplot widget.

    $w.plwin attach $client

# Enable keyboard traversal when widget has the input focus.

    tk_bindForTraversal $w.plwin

# Bindings

    bind $w.plwin <KeyPress> "send $client {keypress %N %K %A}"
    bind $w.plwin <Any-Enter> "focus $w.plwin"

# Inform client of plplot widget name for widget commands.

    send $client "set plwidget $w.plwin"

# I want the focus

    focus $w.plwin
}

#------------------------------------------------------------------------------
# Proc to set end-of-page indicator to help user out.
#------------------------------------------------------------------------------

proc plwindow_flash {w} {

    set bg [lindex [$w.ftop.leop config -bg] 4]
    if {$bg == "gray"} {
	set default [lindex [$w.ftop.lstat config -bg] 4]
	$w.ftop.leop config -bg $default
    } else {
	$w.ftop.leop config -bg gray
    }
}

#------------------------------------------------------------------------------
# Executed as part of orderly shutdown procedure.
#------------------------------------------------------------------------------

proc plwindow_end {w} {
    $w.plwin detach
}

#----------------------------------------------------------------------------
# plwindow_zoom
#
# Zooms plot in response to user input.
#----------------------------------------------------------------------------

proc plwindow_zoom {w} {
    $w.ftop.lstat configure -text \
	"Zoom: Click on upper left hand corner of zoom region."
    bind $w.plwin <ButtonPress> "pl_zoom_start $w %x %y"
}

#----------------------------------------------------------------------------
# plwindow_pan
#
# Pans plot in response to user input.
#----------------------------------------------------------------------------

proc plwindow_pan {w} {
    $w.ftop.lstat configure -text \
	"Pan: Click in center of plot and drag."
    bind $w.plwin <ButtonPress> "pl_pan_start $w %x %y"
}

#----------------------------------------------------------------------------
# plwindow_reset
#
# Resets after zoom/pan.
#----------------------------------------------------------------------------

proc plwindow_reset {w} {
    $w.ftop.lstat configure -text $w
    bind $w.plwin <ButtonPress> {}
    $w.plwin view reset
}

#----------------------------------------------------------------------------
# pl_zoom_start
#
# Starts plot zoom.
#----------------------------------------------------------------------------

proc pl_zoom_start {w x y} {
    bind $w.plwin <ButtonPress> {}
    $w.ftop.lstat configure -text \
	"Select zoom region by dragging mouse, then release."

    $w.plwin view init
    bind $w.plwin <B1-ButtonRelease> "pl_zoom_select $w $x $y %x %y"
    bind $w.plwin <B1-Motion> "pl_zoom_draw $w $x $y %x %y"
}

#----------------------------------------------------------------------------
# pl_pan_start
#
# Starts plot pan.
#----------------------------------------------------------------------------

proc pl_pan_start {w x y} {
    bind $w.plwin <ButtonPress> {}
    $w.ftop.lstat configure -text \
	"Drag plot to new location, then release button."

    set lx [winfo width $w.plwin]
    set ly [winfo height $w.plwin]

    $w.plwin view init
    bind $w.plwin <B1-ButtonRelease> "pl_pan_select $w $x $y %x %y $lx $ly"
    bind $w.plwin <B1-Motion> "pl_pan_draw $w $x $y %x %y $lx $ly"
}

#----------------------------------------------------------------------------
# pl_zoom_draw
#
# Draws zoom box.
#----------------------------------------------------------------------------

proc pl_zoom_draw {w x0 y0 x1 y1} {
    $w.plwin view draw $x0 $y0 $x1 $y1
}

#----------------------------------------------------------------------------
# pl_zoom_select
#
# Selects region for zoom.
#----------------------------------------------------------------------------

proc pl_zoom_select {w x0 y0 x1 y1} {
    bind $w.plwin <B1-ButtonRelease> {}
    bind $w.plwin <B1-Motion> {}
    $w.ftop.lstat configure -text "$w"
    $w.plwin view select $x0 $y0 $x1 $y1
}

#----------------------------------------------------------------------------
# pl_pan_draw
#
# Draws pan box.
#----------------------------------------------------------------------------

proc pl_pan_draw {w x0 y0 x1 y1 lx ly} {
    $w.plwin view draw \
	[expr $x1-$x0] [expr $y1-$y0] [expr $lx+$x1-$x0] [expr $ly+$y1-$y0]
}

#----------------------------------------------------------------------------
# pl_pan_select
#
# Selects region for pan.
#----------------------------------------------------------------------------

proc pl_pan_select {w x0 y0 x1 y1 lx ly} {
    bind $w.plwin <B1-ButtonRelease> {}
    bind $w.plwin <B1-Motion> {}
    $w.ftop.lstat configure -text "$w"
    $w.plwin view select \
	[expr $x1-$x0] [expr $y1-$y0] [expr $lx+$x1-$x0] [expr $ly+$y1-$y0]
}

#.menu.file.m add command \
#    -label "Print" \
#    -command {null_command "Print"} \
#    -underline 0
#
#.menu.file.m add command \
#    -label "Save..." \
#    -command {null_command "Save..."} \
#    -underline 0
#
#.menu.file.m add command \
#    -label "Save As..." \
#    -command {null_command "Save As..."} \
#    -underline 5

