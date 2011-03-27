#! /usr/bin/env perl
#
# Demo x12 for the PLplot PDL binding
#
# Bar chart demo
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

# SYNC: x12c.c 1.14

use PDL;
use PDL::Graphics::PLplot;

my $pos = double (0.0, 0.25, 0.5, 0.75, 1.0);
my $red = double (0.0, 0.25, 0.5, 1.0, 1.0);
my $green = double (1.0, 0.5, 0.5, 0.5, 1.0);
my $blue = double (1.0, 1.0, 0.5, 0.25, 0.0);

# Parse and process command line arguments

plParseOpts (\@ARGV, PL_PARSE_SKIP | PL_PARSE_NOPROGRAM);

# Initialize plplot

plinit ();

pladv (0);

plvsta ();
plwind (1980.0, 1990.0, 0.0, 35.0);
plbox (1.0, 0, 10.0, 0, "bc", "bcnv");
plcol0 (2);
pllab ("Year", "Widget Sales (millions)", "#frPLplot Example 12");

my $y0 = pdl [5, 15, 12, 24, 28, 30, 20, 8, 12, 3];

plscmap1l(1, $pos, $red, $green, $blue, pdl []);

for (my $i = 0; $i < 10; $i++) {
  # plcol0 ($i + 1);
  plcol1 ($i/9.0);
  plpsty (0);
  myplfbox ((1980. + $i), $y0->index ($i));
  my $string = sprintf ("%.0f", $y0->index ($i));
  plptex ((1980. + $i + .5), ($y0->index ($i) + 1.), 1.0, 0.0, .5, $string);
  $string = sprintf ("%d", 1980 + $i);
  plmtex (1.0, (($i + 1) * .1 - .05), 0.5, "b", $string);
}

# Don't forget to call plend() to finish off!

plend ();

# The subroutine below is called plfbox in the x12c.c examples, but
# the PDL binding define already a plfbos that conflicts with this
# one.  Hence, it was recalled myplfbox here.

sub myplfbox {
  my $x0 = shift;
  my $y0 = shift;

  my $x = $x0 + pdl [0, 0, 1, 1];
  my $y = pdl [0, $y0->sclr, $y0->sclr, 0];

  plfill ($x, $y);
  plcol0 (1);
  pllsty (1);
  plline ($x, $y);
}
