#!/usr/local/bin/wish -f
#
# Selects a color by twiddling RGB and/or HSL values.
#
# $Id$
#
# $Log$
# Revision 1.1.2.1  2001/04/19 12:31:45  rlaboiss
# First merge against MAIN
#
# Revision 1.1  1994/04/08 12:06:29  mjl
# Prototype cmap1 selector.
#
#

proc min args {
    set x [lindex $args 0]
    foreach y $args {
	if {$y < $x} {set x $y}
    }
    return $x
}

proc max args {
    set x [lindex $args 0]
    foreach y $args {
	if {$y > $x} {set x $y}
    }
    return $x
}

proc floor {x} {
    if {$x < 0} {set t [expr {0-$x}]} {set t $x}
    set s [format %.0f $t]
    if {$t != $x} {return "-$s"} {return $s}
}

# Position a dialog box at a reasonable place on the screen.

proc dpos w {
    wm geometry $w +300+300
}

proc value {n1 n2 hue} {
    while {$hue > 360} {set hue [expr {$hue-360}]}
    while {$hue < 0}   {set hue [expr {$hue+360}]}

    if {$hue < 60} {
	set val [expr {$n1+($n2-$n1)*$hue/60.}]

    } elseif {$hue < 180} {
	set val $n2

    } elseif {$hue < 240} {
	set val [expr {$n1+($n2-$n1)*(240-$hue)/60.}]

    } else {
	set val $n1
    }

    set val [expr {$val*255}]
    set val [format %.0f [floor $val]]
    return $val
}

proc calcRGB {h l s} {
    global color

    set lf [expr {$l/100.}]
    set sf [expr {$s/100.}]

    if {$lf <= 0.5} {
	set m2 [expr {$lf*($sf+1)}]

    } else {
	set m2 [expr {$lf+$sf-$lf*$sf}]
    }
    set m1 [expr {2*$lf-$m2}]

    set r [value $m1 $m2 [expr {$h+120}]]
    set g [value $m1 $m2 $h]
    set b [value $m1 $m2 [expr {$h-120}]]
    set color [format "#%02x%02x%02x" $r $g $b]
}

proc setVar {w variable value} {
    global cp
    global h$cp l$cp s$cp p$cp

    set $variable$cp $value
    updateHLS $w
}

proc updateHLS {w} {
    global cp color
    global h$cp l$cp s$cp p$cp

    calcRGB [set h$cp] [set l$cp] [set s$cp]
    $w.frame2.cb configure -bg $color

    $w.frame.right.h set [set h$cp]
    $w.frame.right.l set [set l$cp]
    $w.frame.right.s set [set s$cp]
    $w.frame.right.p set [set p$cp]
}

# cmap1_select w
#
# Create a top-level window for cmap 1 palette selection.
#
# Arguments:
#    w -	Name to use for new top-level window.

proc cmap1_select {{w .cmap1}} {
    global cp color
    global h$cp l$cp s$cp p$cp

    catch {destroy $w}
    toplevel $w
    dpos $w
    wm title $w "Color map 1 selector"
    wm iconname $w "Cmap1"
    message $w.msg -font -Adobe-times-medium-r-normal--*-180* -aspect 300 \
	    -text "Parameters for color map 1 control points are specified below.  Click the \"OK\" button when done."
    frame $w.frame -borderwidth 10
    pack append $w.frame \
	[frame $w.frame.left] {left expand} \
	[frame $w.frame.right] {right expand}

    pack append $w.frame.left \
	[radiobutton $w.frame.left.b1 -text "Control Point 1" -variable cp \
	     -command "updateHLS $w" -relief flat -value 1] \
	{top pady 4 frame w} \
	[radiobutton $w.frame.left.b2 -text "Control Point 2" -variable cp \
	     -command "updateHLS $w" -relief flat -value 2] \
	{top pady 4 frame w} \
	[radiobutton $w.frame.left.b3 -text "Control Point 3" -variable cp \
	     -command "updateHLS $w" -relief flat -value 3] \
	{top pady 4 frame w} \
	[radiobutton $w.frame.left.b4 -text "Control Point 4" -variable cp \
	     -command "updateHLS $w" -relief flat -value 4] \
	{top pady 4 frame w}

    pack append $w.frame.right \
	[scale $w.frame.right.h -orient horizontal -length 200 \
	     -label "Hue" -from 0 -to 360 \
	     -command "setVar $w h" ] {top padx 20 frame e}

    pack append $w.frame.right \
	[scale $w.frame.right.l -orient horizontal -length 200 \
	     -label "Lightness" -from 0 -to 100 \
	     -command "setVar $w l" ] {top padx 20 frame e}

    pack append $w.frame.right \
	[scale $w.frame.right.s -orient horizontal -length 200 \
	     -label "Saturation" -from 0 -to 100 \
	     -command "setVar $w s" ] {top padx 20 frame e}

    pack append $w.frame.right \
	[scale $w.frame.right.p -orient horizontal -length 200 \
	     -label "Position" -from 0 -to 100 \
	     -command "setVar $w p" ] {top padx 20 frame e}

    $w.frame.left.b1 select

    $w.frame.right.h set [set h$cp]
    $w.frame.right.l set [set l$cp]
    $w.frame.right.s set [set s$cp]
    $w.frame.right.p set [set p$cp]

    frame $w.frame2
    pack append $w.frame2 \
	[frame $w.frame2.cb -height 40 -relief raised -borderwidth 2 \
	     -bg $color] {top fill}

    frame $w.frame3
    pack append $w.frame3 \
	[button $w.frame3.ok -text OK -command "destroy $w"] \
	{left expand fill} 
    button $w.ok -text OK -command "destroy $w"

    pack append $w $w.msg {top fill} $w.frame {top expand fill} \
	$w.frame2 {top fill pady 20} $w.frame3 {bottom fill}
}

set cp 1

# CP 1

set h1 0
set l1 50
set s1 100
set p1 0

# CP 2

set h2 90
set l2 50
set s2 100
set p2 45

# CP 3

set h3 180
set l3 50
set s3 100
set p3 55

# CP 4

set h4 270
set l4 50
set s4 100
set p4 100

calcRGB [set h$cp] [set l$cp] [set s$cp]

wm withdraw .
cmap1_select .cmap1
tkwait visibility .cmap1
grab .cmap1
tkwait window .cmap1
destroy .
