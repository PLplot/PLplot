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

proc map_transform { x y } {

    set scale [expr acos(-1.) / 180.]

    set radius [expr {90. - $y}]

    return [list \
                [expr {$radius * cos( $x * $scale )}] \
                [expr {$radius * sin( $x * $scale )}]]
}

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

# "Normalize" longitude values so that they always fall between -180.0 and
# 180.0
proc normalize_longitude {lon} {
    if {$lon >= -180.0 && $lon <= 180.0} {
        return $lon
    } else {
        set times [expr {floor ((abs($lon) + 180.0) / 360.0)}]
        if {$lon < 0.0} {
            return [expr {($lon + 360.0 * $times)}]
        } else {
            return [expr {($lon - 360.0 * $times)}]
        }
    }

}

# A custom axis labeling function for longitudes and latitudes.
proc geolocation_labeler {axis value} {

    if {$axis == 2} {
        set label_val $value
        if {$label_val > 0.0} {
            set direction_label " N"
        } elseif {$label_val < 0.0} {
            set direction_label " S"
        } else {
            set direction_label "Eq"
        }
    } elseif {$axis == 1} {
        set label_val [normalize_longitude $value]
        if {$label_val > 0.0} {
            set direction_label " E"
        } elseif {$label_val < 0.0} {
            set direction_label " W"
        } else {
            set direction_label ""
        }
    }
    if {$axis == 2 && $value == 0.0} {
        # A special case for the equator
        set label $direction_label
    } else {
        set label [ format "%.0f%s" [expr {abs($label_val)}] $direction_label]
    }

    return $label
}

proc x19 {{w loopback}} {
    set miny -70
    set maxy 80

#   Cartesian plots
#   Most of the world

    set minx 190
    set maxx 550

    # Setup a custom latitude and longitude-based scaling function.
    $w cmd plslabelfunc "geolocation_labeler"

    $w cmd plcol0 1
    $w cmd plenv $minx $maxx $miny $maxy 1 70
    $w cmd plmap usaglobe $minx $maxx $miny $maxy

#   The Americas

    set minx 190
    set maxx 340

    $w cmd plcol0 1
    $w cmd plenv $minx $maxx $miny $maxy 1 70
    $w cmd plmap usaglobe $minx $maxx $miny $maxy

    # Clear the labeling function
    $w cmd plslabelfunc ""

#   Polar, Northern hemisphere
#   Note: the first argument now is the name of the procedure
#   that transforms the coordinates (plmap and plmeridians)

    set minx 0
    set maxx 360

    $w cmd plenv -75 75 -75 75 1 -1
    $w cmd plmap mapform19 globe $minx $maxx $miny $maxy

    $w cmd pllsty 2
    $w cmd plmeridians mapform19 10.0 10.0 0.0 360.0 -10.0 80.0

# Polar, Northern hemisphere
# This time we use a global coordinate transformation, so no coordinate
# transform function is required on the plmap/plmeridians calls.

    set minx 0
    set maxx 360

    $w cmd plstransform map_transform

    $w cmd pllsty 1
    $w cmd plenv -75 75 -75 75 1 -1

    $w cmd plmap globe $minx $maxx $miny $maxy

    $w cmd pllsty 2
    $w cmd plmeridians 10.0 10.0 0.0 360.0 -10.0 80.0

# Show Baltimore, MD on the map.
    $w cmd plcol0 2
    $w cmd plssym 0. 2.

# This is kind of a messy way to use plpoin for plotting a single symbol.
# But it's what we have for now.
    matrix x f 1
    matrix y f 1
    x 0 = -76.6125
    y 0 = 39.2902778

    $w cmd plpoin 1 x y 18

    $w cmd plssym 0. 1.
    $w cmd plptex -76.6125 43. 0.0 0.0 0.0 "Baltimore, MD"
  
    $w cmd plstransform NULL

    $w cmd pllsty 1
# No defaults to restore
}
