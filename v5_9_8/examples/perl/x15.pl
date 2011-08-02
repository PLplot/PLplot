#! /usr/bin/env perl
#
# Demo x15 for the PLplot PDL binding
#
# Shade plot demo
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

# SYNC: x15c.c 1.16

use PDL;
use PDL::Graphics::PLplot;

use constant XPTS => 35;    # Data points in x
use constant YPTS => 46;    # Data points in y

$z = zeroes (XPTS, YPTS);
my $zmin, $zmax;

# Function prototypes

sub plot1 ();
sub plot2 ();
sub f2mnmx ($);
sub cmap1_init1	();
sub cmap1_init2	();

# main
#
# Does a variety of shade plots

sub main {

    # Parse and process command line arguments

    plParseOpts (\@ARGV, PL_PARSE_SKIP | PL_PARSE_NOPROGRAM);

    # Set up color map 0
    #
    #  plscmap0n(3);
    #


    # Set up color map 1

    cmap1_init2 ();

    # Initialize plplot

    plinit ();

    # Set up data array

    my $xx = (sequence (XPTS) - int(XPTS / 2)) / int(XPTS / 2);
    my $yy = (sequence (YPTS) - int(YPTS / 2)) / int(YPTS / 2) - 1.0;
    for (my $i = 0; $i < XPTS; $i++) {
	my $xi = $xx->index ($i);
	for (my $j = 0; $j < YPTS; $j++) {
	    my $yi = $yy->index ($j);
	    $z->slice ("$i,$j") .= $xi ** 2 - $yi ** 2
		+ ($xi - $yi) / ($xi * $xi + $yi ** 2 + 0.1);
	}
    }

    ($zmin, $zmax) = f2mnmx ($z);

    plot1 ();
    plot2 ();
    plot3 ();

    plend();

}

# cmap1_init1
#
# Initializes color map 1 in HLS space

sub cmap1_init1 () {
    my $i = pdl [0,       # left boundary
		 0.45,    # just before center
		 0.55,    # just after center
		 1];      # right boundary

    my $h = pdl [260,     # hue -- low: blue-violet
		 260,     # only change as we go over vertex
		 20,      # hue -- high: red
		 20];     # keep fixed

    my $l;
    if (1) {
	$l = pdl [0.5,      # lightness -- low
		  0.0,      # lightness -- center
		  0.0,      # lightness -- center
		  0.5];     # lightness -- high
    } else {
	plscolbg (255, 255, 255);
	$l = pdl [0.5,      # lightness -- low
		  1.0,      # lightness -- center
		  1.0,      # lightness -- center
		  0.5];     # lightness -- high
    }
    my $s = pdl [1,       # maximum saturation
		 1,       # maximum saturation
		 1,       # maximum saturation
		 1];      # maximum saturation

    plscmap1l (0, $i, $h, $l, $s, pdl ([]));
}

# cmap1_init2
#
# Initializes color map 1 in HLS space

sub cmap1_init2 () {
    my $i = pdl [0,       # left boundary
		 0.45,    # just before center
		 0.55,    # just after center
		 1];      # right boundary

    my $h = pdl [260,     # hue -- low: blue-violet
		 260,     # only change as we go over vertex
		 20,      # hue -- high: red
		 20];     # keep fixed

    my $l;
    if (1) {
	$l = pdl [0.6,      # lightness -- low
		  0.0,      # lightness -- center
		  0.0,      # lightness -- center
		  0.6];     # lightness -- high
    } else {
	plscolbg (255, 255, 255);
	$l = pdl [0.5,      # lightness -- low
		  1.0,      # lightness -- center
		  1.0,      # lightness -- center
		  0.5];     # lightness -- high
    }
    my $s = pdl [1,       # maximum saturation
		 0.5,     # maximum saturation
		 0.5,     # maximum saturation
		 1];      # maximum saturation

    plscmap1l (0, $i, $h, $l, $s, pdl ([]));
}

# plot1
#
# Illustrates a single shaded region

sub plot1 () {
    my $sh_cmap = 0;
    my $min_color = 0;
    my $min_width = 0;
    my $max_color = 0;
    my $max_width = 0;

    pladv (0);
    plvpor (0.1, 0.9, 0.1, 0.9);
    plwind (-1.0, 1.0, -1.0, 1.0);

    # Plot using identity transform

    my $shade_min = $zmin + ($zmax - $zmin) * 0.4;
    my $shade_max = $zmin + ($zmax - $zmin) * 0.6;
    my $sh_color = 7;
    my $sh_width = 2;
    my $min_color = 9;
    my $max_color = 2;
    my $min_width = 2;
    my $max_width = 2;

    plpsty (8);
    plshade1 ($z, -1, 1, -1, 1,
	      $shade_min, $shade_max, $sh_cmap, $sh_color, $sh_width,
	      $min_color, $min_width, $max_color, $max_width,
	      1, "", "", 0);

    plcol0 (1);
    plbox (0.0, 0, 0.0, 0, "bcnst", "bcnstv");
    plcol0 (2);
    pllab ("distance", "altitude", "Bogon flux");
}

# plot2
#
# Illustrates multiple adjacent shaded regions, using different fill
# patterns for each region

sub plot2 () {

    my $nlin = pdl [1, 1, 1, 1, 1, 2, 2, 2, 2, 2];
    my $inc = pdl [ [450, 0], [-450, 0], [0, 0], [900, 0], 
		    [300, 0], [450,-450], [0, 900], [0, 450], 
		    [450, -450], [0, 900] ];
    my $spa = pdl [ [2000, 2000], [2000, 2000], [2000, 2000], 
		    [2000, 2000], [2000, 2000], [2000, 2000], 
		    [2000, 2000], [2000, 2000], [4000, 4000], 
		    [4000, 2000] ];

    my $sh_cmap = 0;
    my $min_color = 0;
    my $min_width = 0;
    my $max_color = 0;
    my $max_width = 0;
    my $sh_width = 2;

    pladv (0);
    plvpor (0.1, 0.9, 0.1, 0.9);
    plwind (-1.0, 1.0, -1.0, 1.0);

    # Plot using identity transform

    for (my $i = 0; $i < 10; $i++) {
	my $shade_min = $zmin + ($zmax - $zmin) * $i / 10.0;
	my $shade_max = $zmin + ($zmax - $zmin) * ($i + 1) / 10.0;
	my $sh_color = $i + 6;
	my $n = $nlin->slice("($i)");
	my $nm1 = $n - 1;
	plpat ($n, $inc->slice("0:$nm1,($i)"), $spa->slice("0:$nm1,($i)"));

	plshade1 ($z, -1, 1, -1, 1,
		  $shade_min, $shade_max, $sh_cmap, $sh_color, $sh_width,
		  $min_color, $min_width, $max_color, $max_width,
		  1, "", "", 0);
    }

    plcol0 (1);
    plbox (0.0, 0, 0.0, 0, "bcnst", "bcnstv");
    plcol0 (2);
    pllab ("distance", "altitude", "Bogon flux");
}

# plot3
#
# Illustrates shaded regions in 3d, using a different fill pattern for 
# each region.

sub plot3 () {

    my $xx = pdl [ [-1.0, 1.0, 1.0, -1.0, -1.0],
		    [-1.0, 1.0, 1.0, -1.0, -1.0] ];
    my $yy = pdl [ [1.0, 1.0, 0.0, 0.0, 1.0], 
		   [-1.0, -1.0, 0.0, 0.0, -1.0] ];
    my $zz = pdl [ [0.0, 0.0, 1.0, 1.0, 0.0], 
		   [0.0, 0.0, 1.0, 1.0, 0.0] ];

    pladv (0);
    plvpor (0.1, 0.9, 0.1, 0.9);
    plwind (-1.0, 1.0, -1.0, 1.0);
    plw3d (1., 1., 1., -1.0, 1.0, -1.0, 1.0, 0.0, 1.5, 30., -40,);
    
    # Plot using identity transform
    
    plcol0 (1);
    plbox3 (0.0, 0, 0.0, 0, 0.5, 0, "bntu", "X", "bntu", "Y", "bcdfntu", "Z");
    plcol0 (2);
    pllab ("","","3-d polygon filling");
    
    plcol0 (3);
    plpsty (1);
    plline3 ($xx->slice(":,0"), $yy->slice(":,0"), $zz->slice(":,0"));
    plfill3 (4, $xx->slice("0:4,0"), $yy->slice("0:4,0"), $zz->slice("0:4,0"));
    plpsty (2);
    plline3 ($xx->slice(":,1"), $yy->slice(":,1"), $zz->slice(":,1"));
    plfill3 (4, $xx->slice("0:4,1"), $yy->slice("0:4,1"), $zz->slice("0:4,1"));
    
}

# f2mnmx
#
# Returns min & max of input 2d array

sub f2mnmx ($) {
    my $f = shift;
    return (min ($f), max ($f));
}

main ();
