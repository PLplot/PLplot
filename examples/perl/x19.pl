#! /usr/bin/env perl
#
# Demo x19 for the PLplot PDL binding
#
# Illustrates backdrop plotting of world, US maps.
#
# Copyright (C) 2004  Rafael Laboissiere
#
# This file is part of PLplot.
#
# PLplot is free software; you can redistribute it and/or modify
# it under the terms of the GNU Library General Public License as published
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

# SYNC: x19c.c 1.10

use PDL;
use PDL::Graphics::PLplot;
use Math::Trig qw [pi];

#--------------------------------------------------------------------------
# main
#
# Shows two views of the world map.
#--------------------------------------------------------------------------

# Parse and process command line arguments

plParseOpts (\@ARGV, PL_PARSE_SKIP | PL_PARSE_NOPROGRAM);

# Longitude (x) and latitude (y)

my $miny = -70;
my $maxy = 80;

plinit ();

# Cartesian plots
# Most of world

my $minx = -170;
my $maxx = $minx + 360;

# Setup a custom latitude and longitude-based scaling function.
plslabelfunc(\&geolocation_labeler);

plcol0 (1);
plenv ($minx, $maxx, $miny, $maxy, 1, 70);
plmap ($minx, $maxx, $miny, $maxy, 0, "usaglobe");

# The Americas

$minx = 190;
$maxx = 340;

plcol0 (1);
plenv ($minx, $maxx, $miny, $maxy, 1, 70);
plmap ($minx, $maxx, $miny, $maxy, 0, "usaglobe");

# Clear the labeling function
plslabelfunc(0);

# Polar, Northern hemisphere

$minx = 0;
$maxx = 360;

plenv (-75, 75, -75, 75, 1, -1);
plmap ($minx, $maxx, $miny, $maxy, \&mapform19, "globe");

pllsty (2);
plmeridians (10.0, 10.0, 0.0, 360.0, -10.0, 80.0, \&mapform19);

# Polar, Northern hemisphere, this time with a PLplot-wide transform

plstransform(\&mapform19);

pllsty(1);
plenv(-75, 75, -75, 75, 1, -1);

# No need to set the map transform here as the global transform will be
# used.
plmap($minx, $maxx, $miny, $maxy, 0, "globe");

pllsty(2);
plmeridians(10.0, 10.0, 0.0, 360.0, -10.0, 80.0, 0);

# Show Baltimore, MD on the map
plcol0(2);
plssym(0.0, 2.0);
my $x = pdl (-76.6125);
my $y = pdl (39.2902778);
plpoin($x, $y, 18);
plssym(0.0, 1.0);
plptex(-76.6125, 43.0, 0.0, 0.0, 0.0, "Baltimore, MD");

# For perl, this is how the global transform is cleared
plstransform(0);

plend ();

#--------------------------------------------------------------------------
# Subroutines
#--------------------------------------------------------------------------

#--------------------------------------------------------------------------
# mapform19
#
# Defines specific coordinate transformation for example 19.
# Not to be confused with mapform in src/plmap.c.
# $x and $y are the coordinates to be plotted.  They can be either
# perl scalars (when used as an argument to plstransform) or PDLs
# (when used as an argument to plmeridians or plmap)
#--------------------------------------------------------------------------
sub mapform19 {
  my ($x, $y) = @_;

  my $radius = 90.0 - $y;
  my $xp = $radius * cos ($x * pi / 180);
  my $yp = $radius * sin ($x * pi / 180);

  return ($xp, $yp);
}

#--------------------------------------------------------------------------
# geolocation_labeler
#
# A custom axis labeling function for longitudes and latitudes.
#--------------------------------------------------------------------------
sub geolocation_labeler {
  my ($axis, $value, $length) = @_;

  my ($direction_label, $label_val);
  if (($axis == PL_Y_AXIS) && $value == 0) {
      return "Eq";
  } elsif ($axis == PL_Y_AXIS) {
    $label_val = $value;
    $direction_label = ($label_val > 0) ? " N" : " S";
  } elsif ($axis == PL_X_AXIS) {
    my $times  = floor((abs($value) + 180.0 ) / 360.0);
    $label_val = ($value < 0) ? $value + 360.0 * $times : $value - 360.0 * $times;
    $direction_label = ($label_val > 0) ? " E"
                     : ($label_val < 0) ? " W"
                     :                    "";
  }
  return substr (sprintf ("%.0f%s", abs($label_val), $direction_label), 0, $length);
}

