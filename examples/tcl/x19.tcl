# $Id$
#
#      Copyright (C) 2002  Alan W. Irwin
#      Copyright (C) 2009  Arjen Markus
#
#
#      This file is part of PLplot.
#
#      PLplot is free software; you can redistribute it and/or modify
#      it under the terms of the GNU General Library Public License as
#      published by the Free Software Foundation; either version 2 of the
#      License, or (at your option) any later version.
#
#      PLplot is distributed in the hope that it will be useful,
#      but WITHOUT ANY WARRANTY; without even the implied warranty of
#      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#      GNU Library General Public License for more details.
#
#      You should have received a copy of the GNU Library General Public
#      License along with PLplot; if not, write to the Free Software
#      Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
#
#
#      Illustrates backdrop plotting of world, US maps.
#      Contributed by Wesley Ebisuzaki.
#
#

#
#   The coordinate transformation works directly on the two matrices
#   containing the x and y coordinates
#
proc mapform19 {n matx maty} {

    set deg_to_rad [expr {acos(-1.0)/180.0}]

    for {set i 0} {$i < $n} {incr i} {
        set x      [$matx $i]
        set y      [$maty $i]
        set radius [expr {90.0 - $y}]
        set xp     [expr {$radius * cos($x * $deg_to_rad)}]
        set yp     [expr {$radius * sin($x * $deg_to_rad)}]

        $matx $i = $xp
        $maty $i = $yp
    }
}

proc x19 {{w loopback}} {
    set miny -70
    set maxy 80

#   Cartesian plots
#   Most of the world

    set minx 190
    set maxx 550

    $w cmd plcol0 1
    $w cmd plenv $minx $maxx $miny $maxy 1 -1
    $w cmd plmap usaglobe $minx $maxx $miny $maxy

#   The Americas

    set minx 190
    set maxx 340

    $w cmd plcol0 1
    $w cmd plenv $minx $maxx $miny $maxy 1 -1
    $w cmd plmap usaglobe $minx $maxx $miny $maxy

#   Polar, Northern hemisphere
#   Note: the first argument now is the name of the procedure
#   that transforms the coordinates (plmap and plmeridians)

    set minx 0
    set maxx 360

    $w cmd plenv -75 75 -75 75 1 -1
    $w cmd plmap mapform19 globe $minx $maxx $miny $maxy

    $w cmd pllsty 2
    $w cmd plmeridians mapform19 10.0 10.0 0.0 360.0 -10.0 80.0
    $w cmd pllsty 1
# No defaults to restore
}
