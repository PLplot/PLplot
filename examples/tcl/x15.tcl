#----------------------------------------------------------------------------
# $Id$
#----------------------------------------------------------------------------

proc x15 {{w loopback}} {
  set xpts 35
  set ypts 46
  matrix z f $xpts $ypts

#  $w cmd plscmap0n 3
#  This changing of cmap2 actually works, but it is not actually used, and it 
#  unnecessarily changes other tcl and tk examples which *do* use cmap1.
#  Thus, comment it out.
#  cmap1_init2 $w

  for {set i 0} {$i < $xpts} {incr i} {
    set x [expr ($i - ($xpts/2.)) / ($xpts/2.)]
    for {set j 0} {$j < $ypts} {incr j} {
      set y [expr ($j - .5 * $ypts) / (.5 * $ypts) - 1.]

      z $i $j = [expr $x*$x - $y*$y + ($x - $y)/($x * $x + $y * $y + .1) ]
    }
  }
  set zmin [z 0 0]
  set zmax $zmin
  for {set i 0} {$i < $xpts} {incr i} {
    for {set j 0} {$j < $ypts} {incr j} {
      if {[z $i $j] < $zmin} { set zmin [z $i $j] }
      if {[z $i $j] > $zmax} { set zmax [z $i $j] }
    }
  }
  plot151 $w $z $zmin $zmax
  plot152 $w $z $zmin $zmax
  $w cmd pleop
  
}

proc  cmap1_init2 { w } {

  matrix i f 4
  matrix h f 4
  matrix l f 4
  matrix s f 4
  matrix rev f 4
  i 0 = 0.0
  i 1 = 0.45
  i 2 = 0.55
  i 3 = 1.0

  h 0 = 260.0
  h 1 = 260.0
  h 2 = 20.0
  h 3 = 20.0

  l 0 = 0.6
  l 1 = 0.0
  l 2 = 0.0
  l 3 = 0.6

  s 0 = 1.0
  s 1 = 0.5
  s 2 = 0.5
  s 3 = 1.0
  
  rev 0 = 0
  rev 1 = 0
  rev 2 = 0
  rev 3 = 0

  $w cmd plscmap1l 0 4 i h l s rev
}
proc plot151 { w z zmin zmax } {

  $w cmd pladv 0
  $w cmd plvpor .1 .9 .1 .9
  $w cmd plwind -1.0 1.0 -1.0 1.0

  set shade_min [expr $zmin + ($zmax - $zmin)*.4 ]
  set shade_max [expr $zmin + ($zmax - $zmin)*.6 ]
  set sh_color 7
  set sh_width 2
  set min_color 9
  set max_color 2
  set min_width 2
  set max_width 2
  set sh_cmap 0

  $w cmd plpsty 8
  $w cmd plshade z -1. 1. -1. 1. $shade_min $shade_max \
                  $sh_cmap $sh_color $sh_width \
                  $min_color $min_width $max_color $max_width 1
  $w cmd plcol0 1
  $w cmd plbox bcnst 0.0 0 bcnstv 0.0 0
  $w cmd plcol0 2
  $w cmd pllab "distance" "altitude" "Bogon flux"
}

proc plot152 { w z zmin zmax } {

  set sh_cmap 0
  set min_color 0
  set min_width 0
  set max_color 0
  set max_width 0

  $w cmd pladv 0
  $w cmd plvpor .1 .9 .1 .9
  $w cmd plwind -1.0 1.0 -1.0 1.0
  for {set i 0} {$i < 10} {incr i} {
    set shade_min [expr $zmin + ($zmax - $zmin)*$i/10.0 ]
    set shade_max [expr $zmin + ($zmax - $zmin)*($i + 1)/10.0 ]
    set sh_color [expr $i + 6]
    set sh_width 2
    $w cmd plpsty [expr ($i + 2) % 8 + 1]
    $w cmd plshade z -1. 1. -1. 1. $shade_min $shade_max \
                    $sh_cmap $sh_color $sh_width \
                    $min_color $min_width $max_color $max_width 1

    $w cmd plcol0 1
    $w cmd plbox bcnst 0.0 0 bcnstv 0.0 0
    $w cmd plcol0 2
    $w cmd pllab "distance" "altitude" "Bogon flux"
  }
}
