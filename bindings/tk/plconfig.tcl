#----------------------------------------------------------------------------
# plconfig.tcl
#
# Sets configuration options for plplot/TK driver.
# Maurice LeBrun
# IFS, University of Texas
#
# This is largely based on options.motif.tk from Ioi K Lam's Tix package.
# The TK options are set to be fairly Motif-like.
#
# Aside: one way to be friendly to user preferences is to first check the
# options database to see if an appropriate resource specification already
# exists.  The problem with this is that if you have multiple plservers
# running, you need to specify either (a) a resource for each one (for some
# reason, under HP/UX at least, plserver*font is not matched if the main
# window name is "plserver #2"), or (b) an unqualified resource i.e. *font.
# Neither is a particularly attractive solution.
#
# The alternative is to just provide a way for the user to substitute
# a different plconfig proc, and I have chosen this approach.  Just copy
# this file, modify it to your tastes, and put it in plserver's search
# path for TCL procs:
#
#	 user-specified directory(s) (set by -auto_load argument)
#	 Current directory
#	 $PL_LIBRARY 
#	 $HOME/bin 
#	 /usr/local/plplot/tcl 
#----------------------------------------------------------------------------

proc plconfig {} {
    
# Font-related resources.

    set gen_font	-*-helvetica-medium-r-normal-*-18-*-*-*-*-*-*-*
    set gen_bold_font	-*-helvetica-bold-r-normal-*-18-*-*-*-*-*-*-*
    set gen_menu_font	-*-helvetica-medium-o-normal-*-18-*-*-*-*-*-*-*
    set gen_italic_font	-*-helvetica-bold-o-normal-*-18-*-*-*-*-*-*-*

    option add *font		$gen_font
    option add *Entry.font	$gen_font
    option add *Menu*font	$gen_menu_font
    option add *Menubutton*font	$gen_menu_font
    option add *Scale.font	$gen_italic_font

#----------------------------------------------------------------------------
# Color-related resources. 
# Sets colors in a Motif-like way, but only if on a color system.

    if {[tk colormode .] == "color"} {

	set gen_bg		lightgray
	set gen_fg		black
	set gen_darker_bg	gray
	set gen_darker_fg	black
	set gen_active_bg	$gen_bg
	set gen_active_fg	$gen_fg

	option add *background			$gen_bg
	option add *foreground			$gen_fg
	option add *activeBackground      	$gen_active_bg
	option add *activeForeground      	$gen_active_fg
	option add *disabledForeground		gray45
	option add *Checkbutton.selector	yellow
	option add *Radiobutton.selector	yellow
	option add *Entry.background		#c07070
	option add *Entry.foreground		black
	option add *Entry.insertBackground	black
	option add *Listbox.background		$gen_darker_bg
	option add *Scale.foreground		$gen_fg
	option add *Scale.activeForeground	$gen_bg
	option add *Scale.background		$gen_bg
	option add *Scale.sliderForeground	$gen_bg
	option add *Scale.sliderBackground	$gen_darker_bg
	option add *Scrollbar.foreground	$gen_bg
	option add *Scrollbar.activeForeground	$gen_bg
	option add *Scrollbar.background	$gen_darker_bg
    }

#----------------------------------------------------------------------------
# Miscellaneous 

    option add *anchor				w
    option add *Button.borderWidth    		2
    option add *Button.anchor			c
    option add *Checkbutton.borderWidth    	2
    option add *Radiobutton.borderWidth    	2
    option add *Label.anchor			w
    option add *Labelframe.borderWidth		2
    option add *Entry.relief			sunken
    option add *Scrollbar.relief		sunken
}
