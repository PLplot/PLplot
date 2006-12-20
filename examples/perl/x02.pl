#! /usr/bin/env perl
#
# Demo x02 for the PLplot PDL binding
#
# Multiple window and color map 0 demo
# (inspired from t/x02.t of module Graphics::PLplot, by Tim Jenness)
#
# Copyright (C) 2004  Rafael Laboissiere
#
# This file is part of PLplot.
#
# PLplot is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Library Public License as published
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

# SYNC: x02c.c 1.14

use PDL;
use PDL::Graphics::PLplot;


sub main {
# Parse and process command line arguments

    plParseOpts (\@ARGV, PL_PARSE_SKIP | PL_PARSE_NOPROGRAM);

# Initialise plplot

    plinit ();

# Run demos
    demo1();

    # plhlsrgb is currently not implemented in PDL
    # so demo 2 does not work
    #demo2();
    
    plend();
}

#--------------------------------------------------------------------------
# demo1
#
# Demonstrates multiple windows and default color map 0 palette.
#--------------------------------------------------------------------------
sub demo1 {

    plbop();

# Divide screen into 16 regions
    plssub(4, 4);

    draw_windows( 16, 0 );

    pleop();

}

#--------------------------------------------------------------------------
# demo2
#
# Demonstrates multiple windows, user-modified color map 0 palette, and
# HLS -> RGB translation.
#--------------------------------------------------------------------------

sub demo2 {
# Set up cmap0
# Use 100 custom colors in addition to base 16
    my $r = zeroes(116);
    my $g = zeroes(116);
    my $b = zeroes(116);

# Min & max lightness values
    my $lmin = 0.15;
    my $lmax = 0.85;

    plbop();

# Divide screen into 100 regions

    plssub(10, 10);

    my $h, $l, $s, $r1, $g1, $g1;

    for (my $i = 0; $i <= 99; $i++) {

    # Bounds on HLS, from plhlsrgb() commentary --
    #	hue		[0., 360.]	degrees
    #	lightness	[0., 1.]	magnitude
    #	saturation	[0., 1.]	magnitude
    #

    # Vary hue uniformly from left to right
        $h = (360. / 10. ) * ( $i % 10 );
    # Vary lightness uniformly from top to bottom, between min & max
        $l = $lmin + ($lmax - $lmin) * ($i / 10) / 9.;
    # Use max saturation
        $s = 1.0;

        ($r1, $g1, $b1) = plhlsrgb($h, $l, $s);

        set $r, i+16, r1 * 255;
        set $g, i+16, g1 * 255;
        set $b, i+16, b1 * 255;
    }

# Load default cmap0 colors into our custom set
    for (my $i = 0; $i <= 15; $i++) {
        ($r1, $g1, $b1) = plgcol0($i);
        set $r, i, r1;
        set $g, i, g1;
        set $b, i, b1;
    }
    

# Now set cmap0 all at once (faster, since fewer driver calls)
    plscmap0(r, g, b, 116);

    draw_windows( 100, 16 );

    pleop();
}

#--------------------------------------------------------------------------
# draw_windows
#
# Draws a set of numbered boxes with colors according to cmap0 entry.
#--------------------------------------------------------------------------

sub draw_windows(int nw, int cmap0_offset)
{
    my ($nw, $cmap_offset) = @_;

    plschr(0.0, 3.5);
    plfont(4);

    for (my $i = 0; $i < $nw; $i++) {
	plcol0($i+$cmap0_offset);
	my $text = $i;
	pladv(0);
	my $vmin = 0.1;
	my $vmax = 0.9;
	for (my $j = 0; $j <= 2; $j++) {
	    plwid($j + 1);
	    plvpor($vmin, $vmax, $vmin, $vmax);
	    plwind(0.0, 1.0, 0.0, 1.0);
	    plbox(0,0,0,0,"bc","bc");
	    $vmin += 0.1;
	    $vmax -= 0.1;
	}
	plwid(1);
	plptex(0.5, 0.5, 1.0, 0.0, 0.5, $text);
    }
}

main();
