#! /usr/bin/env perl

#  Sample plots using date / time formatting for axes
#
#  Copyright (C) 2007 Andrew Ross
#  Perl/PDL version 2008 Doug Hunt
#
#  This file is part of PLplot.
#
#    PLplot is free software; you can redistribute it and/or modify
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

#--------------------------------------------------------------------------
# main
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
#--------------------------------------------------------------------------

use PDL;
use PDL::Graphics::PLplot;
use Time::Local;
use List::Util;

use constant PI => 4*atan2(1,1);

# Parse command line arguments 
plParseOpts (\@ARGV, PL_PARSE_SKIP | PL_PARSE_NOPROGRAM);

# Initialize plplot 
plinit();

plot1();

plot2();

plot3();

# Don't forget to call plend() to finish off!
plend();

exit(0);

# Plot a model diurnal cycle of temperature 
sub plot1 {

  # Data points every 10 minutes for 1 day 
  my $npts = 145;

  my $xmin = 0;
  my $xmax = 60.0*60.0*24.0; # Number of seconds in a day
  my $ymin = 10.0;
  my $ymax = 20.0;

  my $x = $xmax * sequence($npts)/$npts;
  my $y = 15.0 - 5.0*cos(2*PI*sequence($npts)/$npts);
  
  pladv(0);

  plvsta();
  plwind($xmin, $xmax, $ymin, $ymax);

  # Draw a box with ticks spaced every 3 hour in X and 1 degree C in Y.
  plcol0(1);

  # Set time format to be hours:minutes
  pltimefmt("%H:%M");
  plbox(3.0*60*60, 3, 1, 5, "bcnstd", "bcnstv");

  plcol0(3);
  pllab("Time (hours:mins)", "Temperature (degC)", "#frPLplot Example 29 - Daily temperature");
  
  plcol0(4);

  plline($x, $y);
}

# Plot the number of hours of daylight as a function of day for a year 
sub plot2 {

  # Latitude for London
  my $lat = 51.5;

  my $npts = 365;

  my $xmin = 0;
  my $xmax = $npts*60.0*60.0*24.0;
  my $ymin = 0;
  my $ymax = 24;
  
  # Formula for hours of daylight from 
  # "A Model Comparison for Daylength as a Function of Latitude and 
  # Day of the Year", 1995, Ecological Modelling, 80, pp 87-95.
  my $j = sequence($npts);
  my $x = $j * 60.0*60.0*24.0;
  my $p = asin(0.39795*cos(0.2163108 + 2*atan(0.9671396*tan(0.00860*($j-186)))));
  my $y = 24.0 - (24.0/PI) * 
    acos( (sin(0.8333*PI/180.0) + sin($lat*PI/180.0)*sin($p) ) / 
	  (cos($lat*PI/180)*cos($p)) );

  plcol0(1);
  # Set time format to be abbreviated month name followed by day of month 
  pltimefmt("%b %d");
  plenv($xmin, $xmax, $ymin, $ymax, 0, 40);

  plcol0(3);
  pllab("Date", "Hours of daylight", "#frPLplot Example 29 - Hours of daylight at 51.5N");
  
  plcol0(4);

  plline($x, $y);
  
}

# Return a 1D PDL consisting of the minimum of each pairwise
# element of the two input 1D PDLs
sub minover { return cat($_[0], $_[1])->xchg(0,1)->minimum }

sub plot3 {


  # Calculate seconds since the Unix epoch for 2005-12-01 UTC.
  my $tstart = timegm(0,0,0,1,11,105); 
   
  my $npts = 62;

  my $xmin = $tstart;
  my $xmax = $xmin + $npts*60.0*60.0*24.0;
  my $ymin = 0.0;
  my $ymax = 5.0;
  
  my $i = sequence($npts);
  my $x = $xmin + $i*60.0*60.0*24.0;
  my $y = 1.0 + sin( 2*PI*$i / 7 ) + exp( (minover($i,$npts-$i)) / 31.0);

  pladv(0);

  plvsta();
  plwind($xmin, $xmax, $ymin, $ymax);

  plcol0(1);

  # Set time format to be ISO 8601 standard YYYY-MM-DD. Note that this is
  # equivalent to %f for C99 compliant implementations of strftime.
  pltimefmt("%Y-%m-%d");

  # Draw a box with ticks spaced every 14 days in X and 1 hour in Y. 
  plbox(14*24.0*60.0*60.0,14, 1, 4, "bcnstd", "bcnstv");

  plcol0(3);
  pllab("Date", "Hours of television watched", "#frPLplot Example 29 - Hours of television watched in Dec 2005 / Jan 2006");
  
  plcol0(4);

  plpoin($x, $y, 2);
  plline($x, $y);
 
}

