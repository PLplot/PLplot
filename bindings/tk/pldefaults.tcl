# $Id$
#----------------------------------------------------------------------------
#
# Sets default configuration options for plplot/TK driver.
# Maurice LeBrun
# IFS, University of Texas
#
# This is largely based on options.motif.tk from Ioi K Lam's Tix package.
# The TK options are set to be fairly Motif-like.
#
# It is very easy to customize plplot/TK settings for a particular site
# or user.  The steps are:
#
# 1. Copy the desired settings from here into the plconfig proc in
#    plconfig.tcl, modifying them to taste
#
# 2. Deposit the modified plconfig.tcl in the desired directory for
#    autoloading. The autoload path used by the TK driver and plserver is
#    as follows:
#
#	 user-specified directory(s) (set by -auto_load argument)
#	 Current directory
#	 $PL_LIBRARY 
#	 $HOME/tcl
#	 INSTALL_DIR/tcl
#
# 3. Create a tclIndex file for plconfig.tcl in the same directory.  I
#    use an alias that makes this easy:
#
#    alias mktclidx  "echo 'auto_mkindex . *.tcl; destroy .' | wish"
#
#    Then just "mktclidx" will do the trick.
#
#
# This scheme allows for a clear and easy way to selectively modify
# defaults on a site, user, or project dependent basis (more easily than
# can be done with resources alone).
#
#----------------------------------------------------------------------------

proc pldefaults {} {
    global tk_version
    global gen_font
    global gen_bold_font
    global gen_menu_font
    global gen_italic_font
    global gen_font_small
    global gen_bold_font_small
    global gen_fixed_font

    global dialog_font
    global dialog_bold_font

# Font-related resources.

    set gen_font		-*-helvetica-medium-r-normal-*-*-180-*
    set gen_bold_font		-*-helvetica-bold-r-normal-*-*-180-*
    set gen_menu_font		-*-helvetica-medium-o-normal-*-*-180-*
    set gen_italic_font		-*-helvetica-bold-o-normal-*-*-180-*
    set gen_font_small		-*-helvetica-medium-r-normal-*-*-120-*
    set gen_bold_font_small	-*-helvetica-bold-r-normal-*-*-120-*
    set gen_fixed_font		-*-courier-medium-r-normal-*-*-180-*

    set dialog_font		-*-times-medium-r-normal-*-*-180-*
    set dialog_bold_font	-*-times-bold-r-normal-*-*-180-*

#----------------------------------------------------------------------------
# Color-related resources. 
# Sort of Motif-y.
# It doesn't actually hurt to do this if not on a color system.

    set gen_bg		lightgray
    set gen_fg		black
    set gen_darker_bg	gray
    set gen_darker_fg	black
    set gen_active_bg	$gen_bg
    set gen_active_fg	$gen_fg

    global tcl_platform
    if {$tcl_platform(platform) == "unix"} {

	option add *font		$gen_font
	option add *Entry.font	$gen_font
	option add *Menu*font	$gen_menu_font
	option add *Menubutton*font	$gen_menu_font
	option add *Scale.font	$gen_bold_font_small
	option add *color.font	$gen_fixed_font

	option add *background			$gen_bg
	option add *foreground			$gen_fg
	option add *activeBackground		$gen_active_bg
	option add *activeForeground		$gen_active_fg
	option add *disabledForeground		gray45
	option add *Checkbutton.selector		yellow
	option add *Radiobutton.selector		yellow
	#option add *Entry.background		#c07070
	option add *Entry.foreground		black
	option add *Entry.insertBackground		black
	option add *Listbox.background		$gen_darker_bg
	option add *Scale.foreground		$gen_fg
	option add *Scale.activeForeground		$gen_bg
	option add *Scale.background		$gen_bg
	option add *Scale.sliderForeground		$gen_bg
	option add *Scale.sliderBackground		$gen_darker_bg
	option add *Scrollbar.background		$gen_bg
	option add *Scrollbar.troughColor		$gen_darker_bg

	# End of page indicator
	
	option add *leop.off			$gen_bg
	option add *leop.on				gray45
	
	# This specifies the default plplot widget background color.
	# A white background looks better on grayscale or mono.
	
	if {[winfo depth .] == 1} {
	    option add *Plframe.background		white
	} else {
	    option add *Plframe.background		black
	}

	#----------------------------------------------------------------------------
	# Miscellaneous 
	
	option add *anchor				w
	option add *Button.borderWidth		2
	option add *Button.anchor			c
	option add *Checkbutton.borderWidth		2
	option add *Radiobutton.borderWidth		2
	option add *Label.anchor			w
	option add *Labelframe.borderWidth		2
	option add *Entry.relief			sunken
	option add *Scrollbar.relief		sunken
	
	# I have this in here so that applications written before Tk 4.0 still
	# look the same.  More selectivity might be better.
	
	option add *highlightThickness		0
	
	# Have focus follow mouse, only available in Tk 4.0+
	# This is needed if you want to control scales using keystrokes.
	
	tk_focusFollowsMouse

    }
    
# Various options -- use global variables for simplicity.
# Not a great solution but will have to do for now.

# zoom options:
#  0:	0=don't preserve aspect ratio, 1=do
#  1:	0=stretch from corner, 1=stretch from center

    global zoomopt_0;		set zoomopt_0 1
    global zoomopt_1;		set zoomopt_1 1

# save options:
#  0:   name of default save device
#  1:   0=save 1 plot/file, 1=save multi plots/file (must close!)

    global saveopt_0;		set saveopt_0 psc
    global saveopt_1;		set saveopt_1 0

# Scale widget bindings

    bind Scale <Button-1>  {%W set [%W get %x %y] }
    bind Scale <B1-Motion> {%W set [%W get %x %y] }

# Key shortcut definitions -- change them if you want!
# Turn them into an empty string to disable.

    global key_resume;		set key_resume		"Return"
    global key_zoom_select;	set key_zoom_select	"z"
    global key_zoom_back;	set key_zoom_back	"b"
    global key_zoom_forward;	set key_zoom_forward	"f"    
    global key_zoom_reset;	set key_zoom_reset	"r"
    global key_print;		set key_print		"P"
    global key_save_again;	set key_save_again	"s"
    global key_scroll_right;	set key_scroll_right	"Right"
    global key_scroll_left;	set key_scroll_left	"Left"
    global key_scroll_up;	set key_scroll_up	"Up"
    global key_scroll_down;	set key_scroll_down	"Down"
    global key_scroll_mag;	set key_scroll_mag	"5"
    global key_scroll_speed;	set key_scroll_speed	"1"

# enable/disable top plot and file menu

    global file_menu_on;	set file_menu_on "1"
    global plot_menu_on;	set plot_menu_on "1"
}

proc plw_moveCursor {w x y xd yd} {
    # If not an activeplot, then return.
    if {![$w cget -activeplot]} {return}
    incr x $xd
    incr y $yd
    # Move the cursor as directed.
    event generate $w <Motion> -warp 1 -x $x -y $y
}

# Set up cursor-keybindings so that they can be used to
# manipulate the pointer on top of active Plframes.
foreach combo {
    {} 
    Shift Option Control 
    {Shift Option} {Shift Control} {Option Control}
    {Shift Option Control}
} {
    set multiply 1
    for {set i 0} {$i < [llength $combo]} {incr i} {
	set multiply [expr {$multiply * 5}]
    }
    if {[llength $combo]} {
	set prefix "[join $combo -]-"
    } else {
	set prefix ""
    }
    foreach dir {Left Right Up Down} x {-1 1 0 0} y {0 0 -1 1} {
	bind Plframe <${prefix}$dir> "plw_moveCursor %W %x %y\
	  [expr {$x * $multiply}] [expr {$y * $multiply}]"
    }
}

