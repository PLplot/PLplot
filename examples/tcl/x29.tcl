#       Sample plots using date / time formatting for axes
#
#  Copyright (C) 2007,2008 Andrew Ross
#
#  This file is part of PLplot.
#
#  PLplot is free software; you can redistribute it and/or modify
#  it under the terms of the GNU Library General Public License as published
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

    $w cmd plsesc "@"

    x29_plot1 $w

    x29_plot2 $w

    x29_plot3 $w

    x29_plot4 $w

    # Restore escape character back to default so we don't affect
    # later plots made after this one.
    $w cmd plsesc "#"
}

# Plot a model diurnal cycle of temperature
proc x29_plot1 {{w loopback}} {

    # Data points every 10 minutes for 1 day
    set npts 73

    matrix x f $npts
    matrix y f $npts
    matrix xerr1 f $npts
    matrix xerr2 f $npts
    matrix yerr1 f $npts
    matrix yerr2 f $npts

    set xmin 0
    # Number of seconds in a day
    set xmax [expr {60.0*60.0*24.0}]
    set ymin 10.0
    set ymax 20.0

    for {set i 0} {$i<$npts} {incr i} {
	set xx [expr {$xmax*double($i)/double($npts)}]
	set yy [expr {15.0 - 5.0*cos( 2*$::PLPLOT::PL_PI*double($i)/double($npts))}]
        x $i = $xx
        y $i = $yy
        xerr1 $i = [expr {$xx - 60.0*5.0}]
        xerr2 $i = [expr {$xx + 60.0*5.0}]
        yerr1 $i = [expr {$yy - 0.1}]
        yerr2 $i = [expr {$yy + 0.1}]
    }

    $w cmd pladv 0

    $w cmd plsmaj 0.0 0.5
    $w cmd plsmin 0.0 0.5

    $w cmd plvsta
    $w cmd plwind $xmin $xmax $ymin $ymax

    # Draw a box with ticks spaced every 3 hour in X and 1 degree C in Y.
    $w cmd plcol0 1
    # Set time format to be hours:minutes
    $w cmd pltimefmt "%H:%M"
    $w cmd plbox "bcnstd" [expr {3.0*60*60}] 3 "bcnstv" 1 5

    $w cmd plcol0 3
    $w cmd pllab "Time (hours:mins)" "Temperature (degC)" \
	"@frPLplot Example 29 - Daily temperature"

    $w cmd plcol0 4

    $w cmd plline $npts x y
    $w cmd plcol0 2
    $w cmd plerrx $npts xerr1 xerr2 y
    $w cmd plcol0 3
    $w cmd plerry $npts x yerr1 yerr2

    $w cmd plsmaj 0.0 1.0
    $w cmd plsmin 0.0 1.0
}

# Plot the number of hours of daylight as a function of day for a year
proc x29_plot2 {{w loopback}} {

    set pi $::PLPLOT::PL_PI

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
    $w cmd plprec 1 1
    $w cmd plenv $xmin $xmax $ymin $ymax 0 40


    $w cmd plcol0 3
    $w cmd pllab "Date" "Hours of daylight" \
	"@frPLplot Example 29 - Hours of daylight at 51.5N"

    $w cmd plcol0 4

    $w cmd plline $npts x y

    $w cmd plprec 0 0
}

proc x29_plot3 {{w loopback}} {

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
	y $i = [expr {1.0 + sin( 2*$::PLPLOT::PL_PI*double($i)/7.0 ) +  \
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
	"@frPLplot Example 29 - Hours of television watched in Dec 2005 / Jan 2006"

    $w cmd plcol0 4

    $w cmd plssym 0.0 0.5
    $w cmd plpoin $npts x y 2
    $w cmd plline $npts x y
}

proc x29_plot4 {{w loopback}} {
    # TAI-UTC (seconds) as a function of time.
    # Use Besselian epochs as the continuous time interval just to prove
    # this does not introduce any issues.

    # Use the definition given in http://en.wikipedia.org/wiki/Besselian_epoch
    # B = 1900. + (JD -2415020.31352)/365.242198781
    # ==> (as calculated with aid of "bc -l" command)
    # B = (MJD + 678940.364163900)/365.242198781
    # ==>
    # MJD = B*365.24219878 - 678940.364163900

    set ::tcl_precision 17

    set scale 365.242198781
    set offset1 -678940.0
    set offset2 -0.3641639

    matrix x f 1001
    matrix y f 1001	

    $w cmd plconfigtime $scale $offset1 $offset2 0 0 0 0 0 0 0 0.

    for {set kind 0} {$kind < 7} {incr kind} {
	if {$kind == 0} {
	    $w cmd plctime 1950 0 2 0 0 0. xmin
	    $w cmd plctime 2020 0 2 0 0 0. xmax
	    set npts [expr {70*12 + 1}]
	    set ymin 0.0
	    set ymax 36.0
	    set time_format "%Y%"
	    set if_TAI_time_format 1
	    set title_suffix "from 1950 to 2020"
	    set xtitle "Year"
	    set xlabel_step 10.
	} elseif {$kind == 1 || $kind == 2} {
	    $w cmd plctime 1961 7 1 0 0 [expr {1.64757-0.20}] xmin
	    $w cmd plctime 1961 7 1 0 0 [expr {1.64757+0.20}] xmax
	    set npts 1001
	    set ymin 1.625
	    set ymax 1.725
	    set time_format "%S%2%"
	    set title_suffix "near 1961-08-01 (TAI)"
	    set xlabel_step [expr {0.05/($scale*86400.)}]
	    if {$kind == 1} {
		set if_TAI_time_format 1
		set xtitle "Seconds (TAI)"
	    } else {
		set if_TAI_time_format 0
		set xtitle "Seconds (TAI) labelled with corresponding UTC"
	    }
	} elseif {$kind == 3 || $kind == 4} {
	    $w cmd plctime 1963 10 1 0 0 [expr {2.6972788-.20}] xmin
	    $w cmd plctime 1963 10 1 0 0 [expr {2.6972788+.20}] xmax
	    set npts 1001
	    set ymin 2.55
	    set ymax 2.75
	    set time_format "%S%2%"
	    set title_suffix "near 1963-11-01 (TAI)"
	    set xlabel_step [expr {0.05/($scale*86400.)}]
	    if {$kind == 3} {
		set if_TAI_time_format 1
		set xtitle "Seconds (TAI)"
	    } else {
		set if_TAI_time_format 0
		set xtitle "Seconds (TAI) labelled with corresponding UTC"
	    }
	} elseif {$kind == 5 || $kind == 6} {
	    $w cmd plctime 2009 0 1 0 0 [expr {34.-5.}] xmin
	    $w cmd plctime 2009 0 1 0 0 [expr {34.+5.}] xmax
	    set npts 1001
	    set ymin 32.5
	    set ymax 34.5
	    set time_format "%S%2%"
	    set title_suffix "near 2009-01-01 (TAI)"
	    set xlabel_step [expr {1./($scale*86400.)}]
	    if {$kind == 5} {
		set if_TAI_time_format 1
		set xtitle "Seconds (TAI)"
	    } else {
		set if_TAI_time_format 0
		set xtitle "Seconds (TAI) labelled with corresponding UTC"
	    }
	}

	for {set i 0} {$i<$npts} {incr i} {
	    set tai [expr {$xmin + $i*($xmax-$xmin)/double($npts-1)}]
	    x $i = $tai
	    $w cmd plconfigtime $scale $offset1 $offset2 0 0 0 0 0 0 0 0.
	    $w cmd plbtime tai_year tai_month tai_day tai_hour tai_min tai_sec $tai
	    $w cmd plconfigtime $scale $offset1 $offset2 2 0 0 0 0 0 0 0.
	    $w cmd plbtime utc_year utc_month utc_day utc_hour utc_min utc_sec $tai
	    $w cmd plconfigtime $scale $offset1 $offset2 0 0 0 0 0 0 0 0.
	    $w cmd plctime $utc_year $utc_month $utc_day $utc_hour $utc_min $utc_sec utc
	    set yy [expr {($tai-$utc)*$scale*86400.}]
	    y $i = $yy
	}

	$w cmd pladv 0
	$w cmd plvsta
	$w cmd plwind $xmin $xmax $ymin $ymax
	$w cmd plcol0 1
	if {$if_TAI_time_format == 1} {
	    $w cmd plconfigtime $scale $offset1 $offset2 0 0 0 0 0 0 0 0.
	} else {
	    $w cmd plconfigtime $scale $offset1 $offset2 2 0 0 0 0 0 0 0.
	}
	$w cmd pltimefmt $time_format
	$w cmd plbox "bcnstd" $xlabel_step 0 "bcnstv" 0. 0
	$w cmd plcol0 3
	set title "@frPLplot Example 29 - TAI-UTC "
	append title $title_suffix
	$w cmd pllab $xtitle "TAI-UTC (sec)" $title
	
	$w cmd plcol0 4
	
	$w cmd plline $npts x y
    }

}

