# $Id$
#
# $Log$
# Revision 1.7  1995/05/06 17:08:14  mjl
# Shave and a haircut.
#
# Revision 1.6  1995/01/13  23:19:30  mjl
# Implemented new cmap1 selector to handle additional control point
# attribute.  Improved widget layout some.
#
# Revision 1.5  1994/08/25  03:59:13  mjl
# Fixed limiting saturation value that was messing up grayscale cmap1's.
#
# Revision 1.4  1994/06/09  20:09:18  mjl
# Changed plplot direct widget commands to begin with a "pl", e.g. scol<?>
# to plscol<?>, etc.  To make going between the C and Tcl API's as natural
# as possible.
#
# Revision 1.3  1994/05/10  22:44:57  mjl
# Changed the way cmap1 position sliders work.  Still a monotonic
# relationship between control point number and position in cmap1 space is
# strictly enforced, but now when you try to move one past another, they
# both get moved.  Any number of control points can be shifted in this way.
# This is a much more useful way to manipulate the color map as it can be
# used to "tie" together two control points (as is appropriate for the
# center two points in the default map).
#
# Revision 1.2  1994/05/10  21:46:51  mjl
# Modified to dynamically alter colormaps.  Color editor now accepts a
# command argument and instance pointer to handle what should be done when
# a color is changed.  Hooks to new, more efficient way of setting colors
# in plframe added.  Buttons for incremental change of cmap1 position
# sliders added (except on boundary control points).
#
# Revision 1.1  1994/04/25  19:12:05  mjl
# The procs to handle palette modification (options menu).
#
#----------------------------------------------------------------------------
# Color palette editors in Tcl for PLPlot.
# Maurice LeBrun
# 13-Apr-1994
# IFS, University of Texas at Austin
#
# Based on the very nice pixmap editor by Sam Shen (sls@aero.org)
# Also some code taken from "coloredit.tk" (author?)
# Note: this should really be rewritten in itcl.
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
# plfile_open
#
# A simple proc to get a file for i/o, with some error checking.
#----------------------------------------------------------------------------

proc plfile_open {op} {
    set file [getItem "Enter file name"]
    if { [string index $file 0] == "~" } {
	set file [glob ~][string trimleft $file ~]
    }
    if { [string length $file] > 0 } {
	if {$op == "r"} {
	    if { ! [file readable $file] } {
		bogue_out "Can't read file $file"
		return {}
	    }
	    return $file
	}
	if {$op == "w"} {
	    if { [file exists $file] } {
		if {![confirm "File $file already exists.  Are you sure?"]} {
		    return {}
		}
	    }
	    if { [catch {open $file w} f] } {
		bogue_out "Can't write to file $file"
		return {}
	    } else {
		close $f
		return $file
	    }
	}
    } else {
	bogue_out "No file specified"
	return {}
    }
}

#----------------------------------------------------------------------------
#
# ColorEditor is ripped off from the Tk demo program tcolor.
# This version differs from the one by Sam Shen in that:
#
# 1. Only 8 bits is retained for each RGB component.  I find this to be
#    clearer and easier to deal with than the default 16 bits used by X.
#    In addition, plplot only wants 8 bits and most devices only display 8
#    bits.  The main disadvantage is that there is more roundoff when
#    switching between color spaces.
# 2. HSL color space is used instead of HSV (or HSB).  I'm just more used
#    to HSL.
# 3. Hue goes from 0 to 359 (degrees).
# 4. Saturation and Lightness go from 0 to 100.  I'd really prefer 0 to 1
#    but TK doesn't support floating point scales yet, so this is the next
#    best thing.
#
#----------------------------------------------------------------------------


set colorSpace hsl

class ColorEditor {
    member r 255
    member g 0
    member b 0
    member color "#ff0000"
    member updating 0
    member name ""
    member w .ceditor
    member ok 0
    method run {{color gray} {cmd none} {instance {}} args} {
	global colorSpace
	set w [getmember w]
	catch {destroy $w}
	toplevel $w
	wm title $w "Color Editor"
	frame $w.buttons
	radiobutton $w.rgb -text "RGB" -variable colorSpace \
	    -value rgb  -relief flat \
	    -command "ColorEditor:changeColorSpace $this rgb"
	radiobutton $w.cmy -text "CMY" -variable colorSpace \
	    -value cmy  -relief flat \
	    -command "ColorEditor:changeColorSpace $this cmy"
	radiobutton $w.hsl -text "HSL" -variable colorSpace \
	    -value hsl  -relief flat \
	    -command "ColorEditor:changeColorSpace $this hsl"
	button $w.ok -text "Ok" -command "ColorEditor:ok $this"
	button $w.cancel -text "Cancel" -command "ColorEditor:cancel $this"
	pack append $w.buttons \
	    $w.rgb "left padx 4" \
	    $w.cmy "left padx 4" \
	    $w.hsl "left padx 4" \
	    $w.cancel "right padx 4 pady 2" \
	    $w.ok "right padx 4 pady 2"
	frame $w.left
	foreach i {1 2 3} {
	    frame $w.left$i
	    label $w.label$i
	    scale $w.scale$i -from 0 -to 255 -length 10c -orient horizontal \
		-command "ColorEditor:colChanged $this $cmd $instance $args"
	    button $w.up$i -width 2 -text + \
		-command "ColorEditor:inc $this $i 1"
	    button $w.down$i -width 2 -text - \
		-command "ColorEditor:inc $this $i -1"
	    pack append $w.left$i \
		$w.label$i {top frame w} \
		$w.down$i {left padx .5c} \
		$w.scale$i left \
		$w.up$i {left padx .5c}
	    pack append $w.left $w.left$i "top expand"
	}
	frame $w.right
	frame $w.swatch -width 2c -height 5c -background $color

	global gen_fixed_font
	label $w.value -text $color -width 13 -font $gen_fixed_font
	pack append $w.right \
	    $w.swatch {top expand fill} \
	    $w.value {bottom pady .5c}
	pack append $w \
	    $w.buttons "top fillx" \
	    $w.left "left expand filly" \
	    $w.right "right padx .5c pady .5c frame s"
	loadNamedColor $color
	changeColorSpace $colorSpace
	grab set $w
	tkwait window $w
	if [getmember ok] {
	    return [getmember color]
	} else {
	    return {}
	}
    }
    method cancel {} {
	setmember ok 0
	destroy [getmember w]
    }
    method ok {} {
	setmember ok 1
	destroy [getmember w]
    }
    method inc {i inc} {
	set w [getmember w]
	$w.scale$i set [expr [$w.scale$i get]+$inc]
    }
    method colChanged {cmd instance args} {
	if [getmember updating] {
	    return
	}
	global colorSpace
	set w [getmember w]
	if {$colorSpace == "rgb"} {
	    set r [format %.0f [$w.scale1 get]]
	    set g [format %.0f [$w.scale2 get]]
	    set b [format %.0f [$w.scale3 get]]
	} 
	if {$colorSpace == "cmy"} {
	    set r [format %.0f [expr {255 - [$w.scale1 get]}]]
	    set g [format %.0f [expr {255 - [$w.scale2 get]}]]
	    set b [format %.0f [expr {255 - [$w.scale3 get]}]]
	} 
	if {$colorSpace == "hsl"} {
	    set list [hslToRgb \
			  [expr {[$w.scale1 get]}] \
			  [expr {[$w.scale2 get]/100.0}] \
			  [expr {[$w.scale3 get]/100.0}]]

	    set r [format %.0f [expr [lindex $list 0]*255]]
	    set g [format %.0f [expr [lindex $list 1]*255]]
	    set b [format %.0f [expr [lindex $list 2]*255]]
	}
	set color [format "#%02x%02x%02x" $r $g $b]
	setmember color $color
	setmember r $r
	setmember g $g
	setmember b $b
	$w.swatch config -bg $color
	$w.value config -text $color
	if { $cmd != "none" } {
	    $cmd $instance $args $color
	}
	update idletasks
    }
    method setScales {} {
	set r [getmember r]
	set b [getmember b]
	set g [getmember g]
	set w [getmember w]
	setmember updating 1
	global colorSpace

	if {$colorSpace == "rgb"} {
	    $w.scale1 config -to 255
	    $w.scale2 config -to 255
	    $w.scale3 config -to 255

	    $w.scale1 set [format %.0f $r]
	    $w.scale2 set [format %.0f $g]
	    $w.scale3 set [format %.0f $b]
	}
	if {$colorSpace == "cmy"} {
	    $w.scale1 config -to 255
	    $w.scale2 config -to 255
	    $w.scale3 config -to 255

	    $w.scale1 set [format %.0f [expr (255-$r)]]
	    $w.scale2 set [format %.0f [expr (255-$g)]]
	    $w.scale3 set [format %.0f [expr (255-$b)]]
	}
	if {$colorSpace == "hsl"} {
	    $w.scale1 config -to 359
	    $w.scale2 config -to 100
	    $w.scale3 config -to 100

	    set list [rgbToHsl $r $g $b]

	    $w.scale1 set [format %.0f [expr {[lindex $list 0]}]]
	    $w.scale2 set [format %.0f [expr {[lindex $list 1] * 100.0}]]
	    $w.scale3 set [format %.0f [expr {[lindex $list 2] * 100.0}]]
	}
	setmember updating 0
    }
    method loadNamedColor name {
	set w [getmember w]
	if {[string index $name 0] != "#"} {
	    set list [winfo rgb $w.swatch $name]
	    set r [lindex $list 0]
	    set g [lindex $list 1]
	    set b [lindex $list 2]
	} else {
	    case [string length $name] {
		4 {set format "#%1x%1x%1x"; set shift 12}
		7 {set format "#%2x%2x%2x"; set shift 8}
		10 {set format "#%3x%3x%3x"; set shift 4}
		13 {set format "#%4x%4x%4x"; set shift 0}
		default {error "syntax error in color name \"$name\""}
	    }
	    if {[scan $name $format r g b] != 3} {
		error "syntax error in color name \"$name\""
	    }
	    set r [expr $r<<$shift]
	    set g [expr $g<<$shift]
	    set b [expr $b<<$shift]
	}

	set r [expr $r>>8]
	set g [expr $g>>8]
	set b [expr $b>>8]

	setmember r $r
	setmember g $g
	setmember b $b
	set color [format "#%02x%02x%02x" $r $g $b]

	setmember color $color
	setScales
	$w.swatch config -bg $color
	$w.value config -text $name
    }
    method setLabels {l1 l2 l3} {
	set w [getmember w]
	$w.label1 config -text $l1
	$w.label2 config -text $l2
	$w.label3 config -text $l3
    }
    method changeColorSpace space {
	global colorSpace
	set colorSpace $space
	if {$space == "rgb"} {
	    setLabels Red Green Blue
	    setScales
	    return
	}
	if {$space == "cmy"} {
	    setLabels Cyan Magenta Yellow
	    setScales
	    return
	}
	if {$space == "hsl"} {
	    setLabels Hue Saturation Lightness
	    setScales
	    return
	}
    }

    method rgbToHsl {r g b} {
	set r [expr $r/255.0]
	set g [expr $g/255.0]
	set b [expr $b/255.0]

	set MIN [min $r $g $b]
	set MAX [max $r $g $b]
	set l [expr ($MIN+$MAX)/2.0]

	if {$MIN == $MAX} {
	    set s 0
	    set h 0
	} else {
	    if {$l < 0.5} {
		set s [expr ($MAX-$MIN)/($MAX+$MIN)]
	    } else {
		set s [expr ($MAX-$MIN)/(2.0-$MAX-$MIN)]
	    }
	    set d [expr $MAX-$MIN]
	    set rc [expr ($MAX-$r)/$d]
	    set gc [expr ($MAX-$g)/$d]
	    set bc [expr ($MAX-$b)/$d]
	    if {$r == $MAX} {
		set h [expr $bc-$gc]
	    } else {
		if {$g == $MAX} {
		    set h [expr $rc-$bc+2]
		} else {
		    set h [expr $gc-$rc-2]
		}
	    }

	    set h [expr {$h*60}]
	    if {$h <  0}   {set h [expr $h+360]}
	    if {$h >= 360} {set h [expr $h-360]}
	}
	set h [format %.0f $h]
	return [list $h $s $l]
    }

    method value {n1 n2 h} {
	while {$h > 360} {set h [expr {$h-360}]}
	while {$h < 0}   {set h [expr {$h+360}]}

	if {$h < 60} {
	    set val [expr {$n1+($n2-$n1)*$h/60.}]

	} elseif {$h < 180} {
	    set val $n2

	} elseif {$h < 240} {
	    set val [expr {$n1+($n2-$n1)*(240-$h)/60.}]

	} else {
	    set val $n1
	}

	return $val
    }

    method hslToRgb {h s l}  {
	if {$l <= 0.5} {
	    set m2 [expr {$l*($s+1)}]

	} else {
	    set m2 [expr {$l+$s-$l*$s}]
	}
	set m1 [expr {2*$l-$m2}]

	set r [value $m1 $m2 [expr {$h+120}]]
	set g [value $m1 $m2 $h]
	set b [value $m1 $m2 [expr {$h-120}]]

	return "$r $g $b"
    }
}

#----------------------------------------------------------------------------
# 
# Color map 0 editor
#
# For now, the colors are only actually set when you punch the OK button.
# Eventually they will be dynamically set (best to use a custom color
# map in this case so only the color cell values need to be changed).
#----------------------------------------------------------------------------

#
# Buttons0 implements the buttons on the main window.
#

class Buttons0 {
    method create {w plot} {
	frame $w
	button $w.savep -text "Save Palette" \
	    -command "Buttons0:savePalette $this"
	button $w.loadp -text "Load Palette" \
	    -command "Buttons0:loadPalette $this $plot"
	button $w.ok -text "OK" \
	    -command "Buttons0:ok $this"

	pack append $w \
	    $w.savep "left fill expand" \
	    $w.loadp "left fill expand" \
	    $w.ok "left fill expand"

	return $w
    }
    method ok {} {
	destroy .edit
    }
    method savePalette {} {
	global palette
	ColorPalette0:savePalette $palette
    }
    method loadPalette {plot} {
	global palette
	ColorPalette0:loadPalette $palette $plot
    }
}

#
# ColorPalette0 implements the color palette.
#

class ColorPalette0 {
    member w {}
    member editor {}
    method create {w plot} {
	global ncol0 plcmap0_col
	setmember w $w
	frame $w -bd 2 -relief raised
	set cmap0 [$plot.plwin cmd plgcmap0]
	set ncol0 [lindex $cmap0 0]
	for {set i 0} {$i < $ncol0} {incr i} {
	    set plcmap0_col($i) [lindex $cmap0 [expr $i+1]]
	    frame $w.$i
	    label $w.$i.color -width 14 \
		-anchor w 
	    button $w.$i.patch -text Edit \
		-command "ColorPalette0:edit $this $i $plot"
	    pack append $w.$i \
		$w.$i.color "right frame e" \
		$w.$i.patch "left padx 4 pady 4 frame w"
	    pack append $w $w.$i "top fillx"
	}
	ColorPalette0:loadcmap $this
	setmember editor [ColorEditor]
	return $w
    }
    method savePalette {} {
	set file [plfile_open w]
	if {$file != {}} {
	    set f [open $file "w"]
	    global ncol0
	    puts $f "$ncol0"
	    for {set i 0} {$i < $ncol0} {incr i} {
		set color [ColorPalette0:getColor $this $i]
		puts $f "$color"
	    }
	    close $f
	}
    }
    method loadPalette {plot} {
	set file [plfile_open r]
	if {$file != {}} {
	    global ncol0 plcmap0_col
	    set f [open $file "r"]
	    set ncol0 [gets $f]
	    if {$ncol0 < 0 || $ncol0 > 255} {
		bogue_out "Illegal number of colors: $ncol0"
		close $f
		return
	    }
	    for {set i 0} {$i < $ncol0} {incr i} {
		set line [gets $f]
		set plcmap0_col($i) [lindex $line 0]
	    }
	    close $f
	    setcmap $plot
	    destroy .edit.palette
	    pack append .edit \
		[ColorPalette0:create $this .edit.palette $plot] "left filly"
	}
    }
    method loadcmap {} {
	set w [getmember w]
	global ncol0 plcmap0_col 
	for {set i 0} {$i < $ncol0} {incr i} {
	    $w.$i.color config -text $plcmap0_col($i) 
	    $w.$i.patch config -background $plcmap0_col($i)
	}
    }
    method edit {i plot} {
	set w [getmember w]
	global ncol0 plcmap0_col 
	set orig [ColorPalette0:getColor $this $i]
	set color [ColorEditor:run [getmember editor] \
		       [lindex [$w.$i.patch config -background] 4] \
		       ColorPalette0:colChanged $this $i $plot]

	if {$color != {}} {
	    set plcmap0_col($i) $color
	} else {
	    set color $orig
	}

	$w.$i.color config -text $color
	$w.$i.patch config -background $color
	$plot.plwin cmd plscol0 $i $color
    }
    method colChanged {data color} {
	set i    [lindex $data 0]
	set plot [lindex $data 1]
	set w    [getmember w]

	$w.$i.color config -text $color
	$w.$i.patch config -background $color
	$plot.plwin cmd plscol0 $i $color
    }
    method setcmap {plot} {
	global ncol0 plcmap0_col 
	set cmap0 ""
	for {set i 0} {$i < $ncol0} {incr i} {
	    set cmap0 "$cmap0 $plcmap0_col($i)"
	}
	$plot.plwin cmd plscmap0 $ncol0 $cmap0
    }
    method getColor {i} {
	set w [getmember w]
	return [lindex [$w.$i.patch config -background] 4]
    }
}

#
# External entry point
#

proc plcmap0_edit {plot} {

    global ncol0 plcmap0_col palette

    catch {destroy .edit}
    toplevel .edit
    dpos .edit
    wm title .edit "Color Map 0 Editor"
    wm iconname .edit "Cmap0 Editor"
    wm minsize .edit 100 100

    pack append .edit \
	[Buttons0:create [Buttons0] .edit.buttons $plot] "top fillx"

    pack append .edit \
	[ColorPalette0:create \
	     [set palette [ColorPalette0]] .edit.palette $plot] "left filly"
}

#----------------------------------------------------------------------------
# 
# Color map 1 editor
#
# The default colors are set the first time this file is sourced.
#
#----------------------------------------------------------------------------

#
# Buttons1 implements the buttons on the main window.
#

class Buttons1 {
    method create {w plot} {
	frame $w
	button $w.savep -text "Save Palette" \
	    -command "Buttons1:savePalette $this"
	button $w.loadp -text "Load Palette" \
	    -command "Buttons1:loadPalette $this $plot"
	button $w.ok -text "OK" \
	    -command "Buttons0:ok $this"

	pack append $w \
	    $w.savep "left fill expand" \
	    $w.loadp "left fill expand" \
	    $w.ok "left fill expand"

	return $w
    }
    method ok {} {
	destroy .edit
    }
    method savePalette {} {
	global palette
	ColorPalette1:savePalette $palette
    }
    method loadPalette {plot} {
	global palette
	ColorPalette1:loadPalette $palette $plot
    }
}

#
# ColorPalette1 implements the color palette.
#
# Anchor the position of each control point by its neighboring control points.
# The ones at the end must stay fixed.
#

class ColorPalette1 {
    member w {}
    member editor {}
    method create {w plot} {
	global ncol1 plcmap1_col plcmap1_pos plcmap1_rev
	setmember w $w

	frame $w -bd 2 -relief flat
	frame $w.l -bd 2 -relief flat
	frame $w.r -bd 2 -relief flat

	frame $w.lab

	global gen_font
	label $w.lab.color -font $gen_font -relief flat -text "Color"
	label $w.lab.pos -relief flat -text "Position in cmap1 space"
	label $w.lab.rev -relief flat -text "Reverse"

	pack append $w.lab \
	    $w.lab.color \
	    {left padx 50}

	pack append $w.lab \
	    $w.lab.rev \
	    {right fillx}

	pack append $w.lab \
	    $w.lab.pos \
	    {right padx 200}

	set cmap1 [$plot.plwin cmd plgcmap1]
	set ncol1 [lindex $cmap1 0]
	for {set i 0} {$i < $ncol1} {incr i} {
	    set plcmap1_col($i) [lindex $cmap1 [expr 3*$i+1]]
	    set plcmap1_pos($i) [lindex $cmap1 [expr 3*$i+2]]
	    set plcmap1_rev($i) [lindex $cmap1 [expr 3*$i+3]]
	    frame $w.l.$i
	    label $w.l.$i.color -width 14 -anchor w
	    button $w.l.$i.patch -text Edit \
		-command "ColorPalette1:edit $this $i $plot"

	    scale $w.l.$i.scale -from 0 -to 100 -length 8c \
		-orient horizontal \
		-command "ColorPalette1:posChanged $this $i $plot"

	# I only decorate the movable sliders (i.e. not endpoints) by +/-
	# buttons.  But the sliders themselves are a good visual cue as to
	# what's going on so they get to stay.

	    if {$i == 0 || $i == $ncol1-1} {
		pack append $w.l.$i \
		    $w.l.$i.scale "right expand padx 8 pady 4" 
	    } else {
		button $w.l.$i.up -width 2 -text + \
		    -command "ColorPalette1:inc $this $i 1"
		button $w.l.$i.down -width 2 -text - \
		    -command "ColorPalette1:inc $this $i -1"

		pack append $w.l.$i \
		    $w.l.$i.up "right padx .5c" \
		    $w.l.$i.scale "right expand padx 8 pady 4" \
		    $w.l.$i.down "right padx .5c" 
	    }
	    pack append $w.l.$i \
		$w.l.$i.color "right frame e" \
		$w.l.$i.patch "left padx 4 pady 4 frame w" 
	    pack append $w.l $w.l.$i "top fillx"
	}

    # Reverse button -- indicates hue interpolation to be done around back
    # side.

	for {set i 0} {$i < $ncol1} {incr i} {

	    frame $w.r.$i
	    if {$i < $ncol1-1} {
		checkbutton $w.r.$i.rev \
		    -variable plcmap1_rev($i) -relief flat \
		    -command "ColorPalette1:setcmap $this $plot"
		pack append $w.r.$i \
		    $w.r.$i.rev "right padx .5c" 
	    }
	    pack append $w.r $w.r.$i "top filly expand"
	}

	ColorPalette1:loadcmap $this
	setmember editor [ColorEditor]

	pack append $w \
	    $w.lab "top fillx" \
	    $w.l "left" \
	    $w.r "right filly expand"

	return $w
    }
    method savePalette {} {
	set file [plfile_open w]
	if {$file != {}} {
	    set f [open $file "w"]
	    global ncol1 plcmap1_pos plcmap1_rev

	    puts $f "$ncol1"
	    for {set i 0} {$i < $ncol1} {incr i} {
		set color [ColorPalette1:getColor $this $i]
		puts $f "$color $plcmap1_pos($i) $plcmap1_rev($i)"
	    }
	    close $f
	}
    }
    method loadPalette {plot} {
	set file [plfile_open r]
	if {$file != {}} {
	    set f [open $file "r"]
	    global ncol1 plcmap1_col plcmap1_pos plcmap1_rev

	    set ncol1 [gets $f]
	    if {$ncol1 < 0 || $ncol1 > 255} {
		bogue_out "Illegal number of colors: $ncol1"
		close $f
		return
	    }
	    for {set i 0} {$i < $ncol1} {incr i} {
		set line [gets $f]
		set plcmap1_col($i) [lindex $line 0]
		set plcmap1_pos($i) [lindex $line 1]
		set plcmap1_rev($i) [lindex $line 2]
		if {$plcmap1_rev($i) == {}} {
		    set plcmap1_rev($i) 0
		}
	    }
	    close $f
	    setcmap $plot
	    destroy .edit.palette
	    pack append .edit \
		[ColorPalette1:create $this .edit.palette $plot] "left filly"
	}
    }
    method inc {i inc} {
	set w [getmember w]
	$w.l.$i.scale set [expr [$w.l.$i.scale get]+$inc]
    }
    method posChanged {i plot args} {
	global plcmap1_pos
	set w [getmember w]
	set curr [getpos $i]

	$w.l.$i.scale set $curr
	set plcmap1_pos($i) $curr
	setcmap $plot
    }
    method getpos {i} {
	global ncol1
	set w [getmember w]
	if {$i == 0} {
	    return 0
	}
	if {$i == $ncol1-1} {
	    return 100
	}
	set l [expr $i-1]
	set r [expr $i+1]
	
	set prev [$w.l.$l.scale get]
	set curr [$w.l.$i.scale get]
	set next [$w.l.$r.scale get]
	
	while { $curr < $prev } {
	    if { $l == 0 } break
	    $w.l.$l.scale set $curr
	    set plcmap1_pos($l) $curr
	    incr l -1
	    set prev [$w.l.$l.scale get]
	}
	while { $curr > $next } {
	    if { $r == $ncol1-1 } break
	    $w.l.$r.scale set $curr
	    set plcmap1_pos($r) $curr
	    incr r
	    set next [$w.l.$r.scale get]
	}
	return $curr
    }
    method loadcmap {} {
	set w [getmember w]
	global ncol1 plcmap1_col plcmap1_pos
	for {set i 0} {$i < $ncol1} {incr i} {
	    $w.l.$i.color config -text $plcmap1_col($i) 
	    $w.l.$i.patch config -background $plcmap1_col($i)
	    $w.l.$i.scale set $plcmap1_pos($i)
	}
    }
    method edit {i plot} {
	set w [getmember w]
	global ncol1 plcmap1_col plcmap1_pos plcmap1_rev
	set orig [ColorPalette1:getColor $this $i]
	set color [ColorEditor:run [getmember editor] \
		       [lindex [$w.l.$i.patch config -background] 4] \
		       ColorPalette1:colChanged $this $i $plot]

	if {$color != {}} {
	    set plcmap1_col($i) $color
	} else {
	    set color $orig
	}

	$w.l.$i.color config -text $color
	$w.l.$i.patch config -background $color
#	puts stdout "$plot.plwin cmd plscol1 $i $color $plcmap1_pos($i) $plcmap1_rev($i)"
	$plot.plwin cmd plscol1 $i $color $plcmap1_pos($i) $plcmap1_rev($i)
    }
    method colChanged {data color} {
	global plcmap1_pos plcmap1_rev
	set i    [lindex $data 0]
	set plot [lindex $data 1]
	set w    [getmember w]

	$w.l.$i.color config -text $color
	$w.l.$i.patch config -background $color
#	puts stdout "$plot.plwin cmd plscol1 $i $color $plcmap1_pos($i) $plcmap1_rev($i)"
	$plot.plwin cmd plscol1 $i $color $plcmap1_pos($i) $plcmap1_rev($i)
    }
    method setcmap {plot} {
	global ncol1 plcmap1_col plcmap1_pos plcmap1_rev
	set cmap1 ""
	for {set i 0} {$i < $ncol1} {incr i} {
	    set cmap1 \
		"$cmap1 $plcmap1_col($i) $plcmap1_pos($i) $plcmap1_rev($i)"
	}
#	puts stdout "$plot.plwin cmd plscmap1 $ncol1 $cmap1"
	$plot.plwin cmd plscmap1 $ncol1 $cmap1
    }
    method getColor {i} {
	set w [getmember w]
	return [lindex [$w.l.$i.patch config -background] 4]
    }
}

#
# External entry point
#

proc plcmap1_edit {plot} {

    global ncol1 plcmap1_col plcmap1_pos palette

    catch {destroy .edit}
    toplevel .edit
    dpos .edit
    wm title .edit "Color Map 1 Editor"
    wm iconname .edit "Cmap1 Editor"
    wm minsize .edit 100 100

    pack append .edit \
	[Buttons1:create [Buttons1] .edit.buttons $plot] "top fillx"

    pack append .edit \
	[ColorPalette1:create \
	     [set palette [ColorPalette1]] .edit.palette $plot] "left filly"
}

