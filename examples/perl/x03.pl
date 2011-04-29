#! /usr/bin/env perl
#
# Demo x03 for the PLplot PDL binding
#
# Polar plot demo
# (inspired from t/x03.t of module Graphics::PLplot, by Tim Jenness)
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

# SYNC: x03c.c 1.18

use PDL;
use PDL::Graphics::PLplot;
use Math::Trig qw [pi];


# Parse and process command line arguments

plParseOpts (\@ARGV, PL_PARSE_SKIP | PL_PARSE_NOPROGRAM);

# Set orientation to portrait - note not all device drivers
# support this, in particular most interactive drivers do not
plsori(1);

# Initialize plplot

plinit ();

# Set up viewport and window but do not draw box

plenv (-1.3, 1.3, -1.3, 1.3, 1, -2);

my $dtr = pi / 180.0;
my $x0 = cos ($dtr * sequence (361));
my $y0 = sin ($dtr * sequence (361));

my ($x, $y);
for my $i (1 .. 10) {

  # Draw circles for polar grid

   # old way
   #  my $x = pdl (0.1 * $i * $x0);
   #  my $y = pdl (0.1 * $i * $y0);
   #  plline ($x, $y);

   # new way
   plarc(0, 0, 0.1 * $i, 0.1 * $i, 0, 360, 0, 0);
}

plcol0 (2);
for my $i (0 .. 11) {
  my $theta = 30 * $i;
  my $dx = cos ( $dtr * $theta );
  my $dy = sin ( $dtr * $theta );

  # Draw radial spokes for polar grid

  pljoin (0, 0, $dx, $dy);

  if ($theta < 9.99) {
      $offset = 0.45;
  } elsif ($theta < 99.9) {
      $offset = 0.30;
  } else {
      $offset = 0.15;
  }

  # Write labels for angle
  # Slightly off zero to avoid floating point logic flips at 90 and 270 deg

  if ($dx >= -0.00001) {
    plptex ($dx, $dy, $dx, $dy, -$offset, int ($theta));
  } else {
    plptex ($dx, $dy, -$dx, -$dy, (1.+$offset), int ($theta));
  }
}

# Draw the graph

my $r = sin ($dtr * 5 * sequence (361));
$x = $x0 * $r;
$y = $y0 * $r;

plcol0 (3);
plline ($x, $y);

plcol0 (4);
plmtex (2, 0.5, 0.5, "t", "#frPLplot Example 3 - r(#gh)=sin 5#gh");

# Close the plot at end

plend();
