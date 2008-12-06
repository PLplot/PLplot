#! /usr/bin/env perl

#  $Id$
#
#  Copyright (C) 2008 Doug Hunt

#  Drawing "spirograph" curves - epitrochoids, cycolids, roulettes

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

use PDL;
use PDL::Graphics::PLplot;

plParseOpts (\@ARGV, PL_PARSE_SKIP | PL_PARSE_NOPROGRAM);

plinit ();

# 
# Generates two kinds of plots:
#   - construction of a cycloid (animated) (TBD)
#   - series of epitrochoids and hypotrochoids

# R, r, p, N 
my $params = [ [21.0,  7.0,  7.0,  3.0],  # Deltoid
               [21.0,  7.0, 10.0,  3.0],
               [21.0, -7.0, 10.0,  3.0],
               [20.0,  3.0,  7.0, 20.0],
               [20.0,  3.0, 10.0, 20.0],
               [20.0, -3.0, 10.0, 20.0],
               [20.0, 13.0,  7.0, 20.0],
               [20.0, 13.0, 20.0, 20.0],
               [20.0,-13.0, 20.0, 20.0] ];

# Illustrate the construction of a cycloid

# TODO
#cycloid()

# Loop over the various curves
# First an overview, then all curves one by one
plssub(3, 3);  # Three by three window

foreach my $parm (@$params) {
  pladv(0);
  plvpor(0, 1, 0, 1);
  spiro($parm);
}

pladv(0);
plssub(1, 1);  # One window per curve

foreach my $parm (@$params) {
  pladv(0);
  plvpor(0, 1, 0, 1);
  spiro($parm);
}

sub spiro {
  my $params = shift;

  # Fill the coordinates
  my $NPNT = 20000;

  my $windings = int($params->[3]);
  my $steps    = int($NPNT/$windings);
  my $dphi     = 8.0*acos(-1.0)/$steps;

  my $xcoord = zeroes($windings*$steps+1);
  my $ycoord = zeroes($windings*$steps+1);

  my $phi  = sequence($windings*$steps+1) * $dphi;
  my $phiw = ($params->[0]-$params->[1])/$params->[1]*$phi;

  my $xcoord = ($params->[0]-$params->[1])*cos($phi) + $params->[2]*cos($phiw);
  my $ycoord = ($params->[0]-$params->[1])*sin($phi) - $params->[2]*sin($phiw);

  my ($xmin, $xmax) = $xcoord->minmax;
  my ($ymin, $ymax) = $ycoord->minmax;

  my $scale = ($xmax-$xmin > $ymax-$ymin) ? $xmax - $xmin : $ymax - $ymin;

  $xmin  = -0.65 * $scale;
  $xmax  =  0.65 * $scale;
  $ymin  = -0.65 * $scale;
  $ymax  =  0.65 * $scale;

  plwind($xmin, $xmax, $ymin, $ymax);

  plcol0(1);
  plline($xcoord, $ycoord);

}
