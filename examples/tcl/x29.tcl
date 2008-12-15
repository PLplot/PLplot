# $Id$
#
#       Sample plots using date / time formatting for axes
#
#  Copyright (C) 2007,2008 Andrew Ross
#
#  This file is part of PLplot.
#
#  PLplot is free software; you can redistribute it and/or modify
#  it under the terms of the GNU General Library Public License as published
#  by the Free Software Foundation; either version 2 of the License, or
#  (at your option) any later version.
#
#  PLplot is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU Library General Public License for more details.
#
#  You should have received a copy of the GNU Library General Public License
#  along with PLplot; if not, write to the Free Software
#  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
#
#

# Draws several plots which demonstrate the use of date / time formats for
# the axis labels.
# Time formatting is done using the system strftime routine. See the 
# documentation of this for full details of the available formats.
#
# 1) Plotting temperature over a day (using hours / minutes)
# 2) Plotting 
#
# Note: Times are stored as seconds since the epoch (usually 1st Jan 1970). 
# 

proc x29 {{w loopback}} {

    x29_plot1 $w

    x29_plot2 $w

    x29_plot3 $w

}

# Plot a model diurnal cycle of temperature
proc x29_plot1 {{w loopback}} {

    set pi 3.14159265358979323846    

    # Data points every 10 minutes for 1 day
    set npts 145
    
    matrix x f $npts
    matrix y f $npts
    
    set xmin 0
    # Number of seconds in a day
    set xmax [expr {60.0*60.0*24.0}]    
    set ymin 10.0
    set ymax 20.0
    
    for {set i 0} {$i<$npts} {incr i} {
	x $i = [expr {$xmax*double($i)/double($npts)}]
	y $i = [expr {15.0 - 5.0*cos( 2*$pi*double($i)/double($npts))}]
    }
  
    $w cmd pladv 0

    $w cmd plvsta
    $w cmd plwind $xmin $xmax $ymin $ymax

    # Draw a box with ticks spaced every 3 hour in X and 1 degree C in Y.
    $w cmd plcol0 1
    # Set time format to be hours:minutes
    $w cmd pltimefmt "%H:%M"
    $w cmd plbox "bcnstd" [expr {3.0*60*60}] 3 "bcnstv" 1 5

    $w cmd plcol0 3
    $w cmd pllab "Time (hours:mins)" "Temperature (degC)" \
	"#frPLplot Example 29 - Daily temperature"
  
    $w cmd plcol0 4

    $w cmd plline $npts x y
}

# Plot the number of hours of daylight as a function of day for a year
proc x29_plot2 {{w loopback}} {    
    
    set pi 3.14159265358979323846

    # Latitude for London
    set lat 51.5
    
    set npts 365
    
    matrix x f $npts
    matrix y f $npts
    
    set xmin 0
    set xmax [expr {$npts*60.0*60.0*24.0}]
    set ymin 0
    set ymax 24
    
    # Formula for hours of daylight from 
    # "A Model Comparison for Daylength as a Function of Latitude and 
    # Day of the Year", 1995, Ecological Modelling, 80, pp 87-95.
    for {set j 0} {$j < $npts} {incr j} {
	x $j = [expr {$j*60.0*60.0*24.0}]
	set p [expr {asin(0.39795*cos(0.2163108 + 2*atan(0.9671396*tan(0.00860*($j-186))))) }]
	set d [expr {24.0 - (24.0/$pi)* \
			 acos( (sin(0.8333*$pi/180.0) + \
				    sin($lat*$pi/180.0)*sin($p)) / \
				   (cos($lat*$pi/180.0)*cos($p)) )}]
	y $j = $d
    }
    
    $w cmd plcol0 1
    # Set time format to be abbreviated month name followed by day of month
    $w cmd pltimefmt "%b %d"
    $w cmd plenv $xmin $xmax $ymin $ymax 0 40


    $w cmd plcol0 3
    $w cmd pllab "Date" "Hours of daylight" \
	"#frPLplot Example 29 - Hours of daylight at 51.5N"
  
    $w cmd plcol0 4

    $w cmd plline $npts x y
  
}

proc x29_plot3 {{w loopback}} {

    set pi 3.14159265358979323846

    # Calculate seconds since the Unix epoch for 2005-12-01 UTC.
    # On newer versions of tcl should use the -format "%Y-%m-%d" 
    # option rather than free-form scanning, but this doesn't 
    # seem to work on tcl8.4
    # Also -timezone :UTC should be used instead of -gmt
    set tstart [clock scan "2005-12-01" \
		    -gmt true ]
   
    set npts 62

    matrix x f $npts
    matrix y f $npts

    set xmin $tstart
    set xmax [expr {$xmin + $npts*60.0*60.0*24.0}]
    set ymin 0.0
    set ymax 5.0
  
    for {set i 0} {$i<$npts} {incr i} {
	x $i = [expr {$xmin + $i*60.0*60.0*24.0}]
	set imin [expr {$i < $npts-$i ? $i : $npts-$i}]
	y $i = [expr {1.0 + sin( 2*$pi*double($i)/7.0 ) +  \
			      exp( double($imin) / 31.0) }]
    }
    $w cmd pladv 0

    $w cmd plvsta
    $w cmd plwind $xmin $xmax $ymin $ymax

    $w cmd plcol0 1
    # Set time format to be ISO 8601 standard YYYY-MM-DD. Note that this is
    # equivalent to %f for C99 compliant implementations of strftime.
    $w cmd pltimefmt "%Y-%m-%d"
    # Draw a box with ticks spaced every 14 days in X and 1 hour in Y.
    $w cmd plbox "bcnstd" [expr {14*24.0*60.0*60.0}] 14 "bcnstv" 1 4
    
    $w cmd plcol0 3
    $w cmd pllab "Date" "Hours of television watched" \
	"#frPLplot Example 29 - Hours of television watched in Dec 2005 / Jan 2006"
  
    $w cmd plcol0 4

    $w cmd plpoin $npts x y 2
    $w cmd plline $npts x y
 
}
