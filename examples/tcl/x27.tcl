# $Id$
#
#  Drawing "spirograph" curves - epitrochoids, cycolids, roulettes
#
#  Copyright (C) 2007  Arjen Markus
#  Copyright (C) 2008  Andrew Ross
#
# This file is part of PLplot.
#
# PLplot is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Library Public License as published
# by the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# PLplot is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Library General Public License for more details.
#
# You should have received a copy of the GNU Library General Public License
# along with PLplot; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
#
#

# --------------------------------------------------------------------------
# main
#
# Generates two kinds of plots:
#   - construction of a cycloid (animated)
#   - series of epitrochoids and hypotrochoids
# --------------------------------------------------------------------------

proc x27 {{w loopback}} {

  #     R, r, p, N
  set params {
    { 21.0   7.0   7.0   3.0  }
    { 21.0   7.0  10.0   3.0  }
    { 21.0  -7.0  10.0   3.0  }
    { 20.0   3.0   7.0  20.0  }
    { 20.0   3.0  10.0  20.0  }
    { 20.0  -3.0  10.0  20.0  }
    { 20.0  13.0   7.0  20.0  }
    { 20.0  13.0  20.0  20.0  }
    { 20.0 -13.0  20.0  20.0  }}

  #  Illustrate the construction of a cycloid

  cycloid $w

  #  Loop over the various curves
  #  First an overview, then all curves one by one
  $w cmd pladv 0
  $w cmd plssub 3 3

  for { set i 0 } { $i < 9 } { incr i } {
     $w cmd pladv 0
     $w cmd plvpor 0.0  1.0  0.0  1.0
     spiro $w [lindex $params $i]
  }
  $w cmd pladv 0
  $w cmd plssub 1 1

  for { set i 0 } { $i < 9 } { incr i } {
     $w cmd pladv 0
     $w cmd plvpor 0.0  1.0  0.0  1.0
     spiro $w [lindex $params $i]
  }

}

#  ===============================================================

proc cycloid {w} {

  #     TODO

}

#  ===============================================================

proc spiro {w params} {

  foreach {param1 param2 param3 param4} $params {break}

  set NPNT 20000

  matrix xcoord f [expr {$NPNT+1}]
  matrix ycoord f [expr {$NPNT+1}]

  #     Fill the coordinates

  set windings [expr {int($param4)}]
  set steps    [expr {$NPNT/$windings}]
  set dphi     [expr {8.0*acos(-1.0)/double($steps)}]

  #     This initialisation is safe!
  set xmin 0.0
  set xmax 0.0
  set ymin 0.0
  set ymax 0.0

  set n [expr {int($windings*$steps+1)}]

  for { set i 0 } { $i < $n } { incr i } {
     set phi  [expr {double($i) * $dphi}]
     set phiw [expr {($param1-$param2)/$param2*$phi}]
     xcoord $i = [expr {($param1-$param2)*cos($phi)+$param3*cos($phiw)}]
     ycoord $i = [expr {($param1-$param2)*sin($phi)-$param3*sin($phiw)}]

     if { $xmin > [xcoord $i] } { set xmin [xcoord $i] }
     if { $xmax < [xcoord $i] } { set xmax [xcoord $i] }
     if { $ymin > [ycoord $i] } { set ymin [ycoord $i] }
     if { $ymax < [ycoord $i] } { set ymax [ycoord $i] }
  }

  if { $xmax-$xmin > $ymax-$ymin } {
     set scale [expr {$xmax - $xmin}]
  } else {
     set scale [expr {$ymax - $ymin}]
  }
  set xmin [expr {- 0.65 * $scale}]
  set xmax [expr {  0.65 * $scale}]
  set ymin [expr {- 0.65 * $scale}]
  set ymax [expr {  0.65 * $scale}]

  $w cmd plwind $xmin $xmax $ymin $ymax

  $w cmd plcol0 1
  $w cmd plline $n xcoord ycoord

}
