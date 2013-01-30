#! /usr/bin/env perl
#
# Demo x02 for the PLplot PDL binding
#
# Multiple window and color map 0 demo
# (inspired from t/x02.t of module Graphics::PLplot, by Tim Jenness)
#
# Copyright (C) 2004  Rafael Laboissiere
#               2008  Doug Hunt -- Added working, more PDL-ish demo2
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

use PDL;
use PDL::Graphics::PLplot;


sub main {
# Parse and process command line arguments

    plParseOpts (\@ARGV, PL_PARSE_SKIP | PL_PARSE_NOPROGRAM);

# Initialise plplot

    plinit ();

# Run demos
    demo1();
    demo2();
    
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

# Min & max lightness values
    my $lmin = 0.15;
    my $lmax = 0.85;

    plbop();

# Divide screen into 100 regions

    plssub(10, 10);

    my $basemap = sequence(16);
    my $custmap = sequence(100);

    my ($r, $g, $b) = plgcol0($basemap);

    my $h = (360/10) * ($custmap % 10);
    my $s = ones(100);
    my $l = $lmin + ($lmax - $lmin) * ($custmap / 10)->floor / 9;
    my ($r1, $g1, $b1) = plhlsrgb($h, $l, $s);

    $r = $r->append($r1*255.001);
    $g = $g->append($g1*255.001);
    $b = $b->append($b1*255.001);

    #for ($i = 0; $i < 116; $i++) {
    #  printf("%3.0f %3.0f %3.0f %3.0f \n", $i, $r->at($i), $g->at($i), $b->at($i)); 
    #}
    
# Now set cmap0 all at once (faster, since fewer driver calls)
    plscmap0($r, $g, $b);

    draw_windows( 100, 16 );

    pleop();
}

#--------------------------------------------------------------------------
# draw_windows
#
# Draws a set of numbered boxes with colors according to cmap0 entry.
#--------------------------------------------------------------------------

sub draw_windows
{
    my ($nw, $cmap_offset) = @_;

    plschr(0.0, 3.5);
    plfont(4);

    for (my $i = 0; $i < $nw; $i++) {
	plcol0($i+$cmap_offset);
	my $text = $i;
	pladv(0);
	my $vmin = 0.1;
	my $vmax = 0.9;
	for (my $j = 0; $j <= 2; $j++) {
	    plwidth($j + 1);
	    plvpor($vmin, $vmax, $vmin, $vmax);
	    plwind(0.0, 1.0, 0.0, 1.0);
	    plbox(0,0,0,0,"bc","bc");
	    $vmin += 0.1;
	    $vmax -= 0.1;
	}
	plwidth(1);
	plptex(0.5, 0.5, 1.0, 0.0, 0.5, $text);
    }
}

main();
