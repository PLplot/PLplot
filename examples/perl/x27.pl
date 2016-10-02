#! /usr/bin/env perl

#  Copyright (C) 2008 Doug Hunt

#  Drawing "spirograph" curves - epitrochoids, cycolids, roulettes

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

use PDL;
use PDL::Graphics::PLplot;
use constant PI => 4*atan2(1,1);
plParseOpts (\@ARGV, PL_PARSE_SKIP | PL_PARSE_NOPROGRAM);

plinit ();

#
# Generates two kinds of plots:
#   - construction of a cycloid (animated) (TBD)
#   - series of epitrochoids and hypotrochoids

# R, r, p, N
# R and r should be integers to give correct termination of the
# angle loop using gcd.
# N.B. N is just a place holder since it is no longer used
# (because we now have proper termination of the angle loop).
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

my $fill = 0;
foreach my $parm (@$params) {
  pladv(0);
  plvpor(0, 1, 0, 1);
  spiro($parm, $fill);
}

pladv(0);
plssub(1, 1);  # One window per curve

foreach my $parm (@$params) {
  pladv(0);
  plvpor(0, 1, 0, 1);
  spiro($parm, $fill);
}

# Fill the curves
$fill = 1;

pladv( 0 );
plssub( 1, 1 ); # One window per curve

foreach my $parm (@$params) {
  pladv( 0 );
  plvpor(0, 1, 0, 1);
  spiro( $parm, $fill);
}

# Don't forget to call plend() to finish off!
plend();


#--------------------------------------------------------------------------
# Calculate greatest common divisor following pseudo-code for the
# Euclidian algorithm at http://en.wikipedia.org/wiki/Euclidean_algorithm
sub gcd  {
  my ($a, $b) = @_;
  $a = abs( $a );
  $b = abs( $b );
  while ( $b != 0 ) {
    my $t = $b;
    $b = $a % $b;
    $a = $t;
  }
  return $a;
}

sub spiro {
  my $params = shift;
  my $fill   = shift;

  # Fill the coordinates
  my $NPNT = 2000;

  my $windings = abs($params->[1]) / gcd($params->[0], $params->[1]);
  my $steps    = int($NPNT/$windings);
  my $dphi     = 2.0*PI/$steps;

  my $phi  = sequence($windings*$steps+1) * $dphi;
  my $phiw = ($params->[0]-$params->[1])/$params->[1]*$phi;

  my $xcoord = ($params->[0]-$params->[1])*cos($phi) + $params->[2]*cos($phiw);
  my $ycoord = ($params->[0]-$params->[1])*sin($phi) - $params->[2]*sin($phiw);

  my ($xmin, $xmax) = $xcoord->minmax;
  my ($ymin, $ymax) = $ycoord->minmax;

  my $xrange_adjust = 0.15 * ($xmax - $xmin);
  $xmin -= $xrange_adjust;
  $xmax += $xrange_adjust;
  my $yrange_adjust = 0.15 * ($ymax - $ymin);
  $ymin -= $yrange_adjust;
  $ymax += $yrange_adjust;

  plwind($xmin, $xmax, $ymin, $ymax);

  plcol0(1);
  if ($fill) {
    plfill ($xcoord, $ycoord);
  } else {
    plline ($xcoord, $ycoord);
  }

}
