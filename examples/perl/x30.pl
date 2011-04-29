#! /usr/bin/env perl

#  Alpha color values demonstration.
#
#  Copyright (C) 2008 Hazen Babcock
#  perl/PDL version 2008 Doug Hunt
#
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
#  This example will only really be interesting when used with devices that 
#  support or alpha (or transparency) values, such as the cairo device family.

use PDL;
use PDL::NiceSlice;
use PDL::Graphics::PLplot;

my $red   = long (0, 255,   0,   0);
my $green = long (0,   0, 255,   0);
my $blue  = long (0,   0,   0, 255);
my $alpha = double (1.0, 1.0, 1.0, 1.0);

my $px    = double (0.1, 0.5, 0.5, 0.1);
my $py    = double (0.1, 0.1, 0.5, 0.5);

my $pos    = double (0.0, 1.0);
my $rcoord = double (1.0, 1.0);
my $gcoord = double (0.0, 0.0);
my $bcoord = double (0.0, 0.0);
my $acoord = double (0.0, 1.0);
my $rev    = long (0, 0);

# Parse command line arguments 
plParseOpts (\@ARGV, PL_PARSE_SKIP | PL_PARSE_NOPROGRAM);

plinit ();
plscmap0n (4);
plscmap0a ($red, $green, $blue, $alpha);

#     Page 1:
#
#     This is a series of red, green and blue rectangles overlaid
#     on each other with gradually increasing transparency.

# Set up the window 
pladv (0);
plvpor (0.0, 1.0, 0.0, 1.0);
plwind (0.0, 1.0, 0.0, 1.0);
plcol0 (0);
plbox (1.0, 0, 1.0, 0, "", "");

# Draw the boxes 
for (my $i = 0; $i < 9; $i++) {
  my $icol = $i%3 + 1;

  # Get a color, change its transparency and 
  # set it as the current color. 
  my ($r, $g, $b, $a) = plgcol0a($icol);
  plscol0a ($icol, $r, $g, $b, 1 - $i/9);
  plcol0 ($icol);

  # Draw the rectangle 
  plfill ($px, $py);

  # Shift the rectangles coordinates 
  $px += 0.5/9;
  $py += 0.5/9;
}

#     Page 2:
#
#     This is a bunch of boxes colored red, green or blue with a single 
#     large (red) box of linearly varying transparency overlaid. The
#     overlaid box is completely transparent at the bottom and completely
#     opaque at the top.

# Set up the window 
pladv(0);
plvpor(0.1, 0.9, 0.1, 0.9);
plwind(0.0, 1.0, 0.0, 1.0);

# Draw the boxes. There are 25 of them drawn on a 5 x 5 grid.
for(my $i = 0; $i < 5; $i++){

  # Set box X position 
  $px(0) .= 0.05 + 0.2 * $i;
  $px(1) .= $px(0) + 0.1;
  $px(2) .= $px(1);
  $px(3) .= $px(0);

  # We don't want the boxes to be transparent, so since we changed
  # the colors transparencies in the first example we have to change
  # the transparencies back to completely opaque.
  my $icol = $i%3 + 1;
  my ($r, $g, $b, $a) = plgcol0a ($icol);
  plscol0a ($icol, $r, $g, $b, 1.0);
  plcol0 ($icol);
  for(my $j = 0; $j < 5; $j++) {
    # Set box y position and draw the box.
    $py(0) .= 0.05 + 0.2 * $j;
    $py(1) .= $py(0);
    $py(2) .= $py(0) + 0.1;
    $py(3) .= $py(2);
    plfill($px, $py);
  }
}

# The overlaid box is drawn using plshades with a color map that is 
# the same color but has a linearly varying transparency.

# Create the color map with 128 colors and use plscmap1la to initialize 
# the color values with a linear varying transparency (or alpha) 
plscmap1n(128);
plscmap1la(1, $pos, $rcoord, $gcoord, $bcoord, $acoord, $rev);

# Use that cmap1 to create a transparent red gradient for the whole
# window.
my $px = pdl(0,1,1,0);
my $py = pdl(0,0,1,1);
plgradient($px, $py, 90);

plend();

