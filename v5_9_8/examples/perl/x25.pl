#! /usr/bin/env perl
#
# Demo x25 for the PLplot PDL binding
#
# Copyright (C) 2006 Andrew Ross
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

# SYNC: x25c.c 1.3

use PDL;
use PDL::Graphics::PLplot;
use Math::Trig qw [pi];

use Getopt::Long;

my ($i, $j);

# Parse and process command line arguments 

plParseOpts (\@ARGV, PL_PARSE_SKIP | PL_PARSE_NOPROGRAM);


# Initialize plplot

plssub(3,3);
plinit();

my $xextreme = pdl( [-120, -120, -120, -80, -220, -20, -20, -80, 20], [120, 120, 120, 80, -120, 20, 20, 80, 120]);
my $yextreme = pdl( [-120, 20, -20, -20, -120, -120, -20, -80, -120], [120, 120, 120, 120, 120, 120, 20, 80, 120]);
my ($x0, $y0);

for ( $k = 0; $k < 2; $k++ ) {
    for ( $j = 0; $j < 4; $j ++ ) {
	if ( $j == 0 ) {
# Polygon 1: a diamond 
	    $x0 = pdl( 0, -100, 0, 100);
	    $y0 = pdl( -100, 0, 100, 0);
	}
	if ( $j == 1 ) {
# Polygon 1: a diamond - reverse direction
	    $x0 = pdl( 100, 0, -100, 0);
	    $y0 = pdl( 0, 100, 0, -100);
	}
	if ( $j == 2 ) {
# Polygon 2: a square with punctures
	    $x0 = pdl( -100, -100, 80, -100, -100, -80, 0, 80, 100, 100);
	    $y0 = pdl( -100, -80, 0, 80, 100, 100, 80, 100, 100, -100);
	}
	if ( $j == 3 ) {
# Polygon 2: a square with punctures - reversed direction 
	    $x0 = pdl( 100, 100, 80, 0, -80, -100, -100, 80, -100, -100);
	    $y0 = pdl( -100, 100, 100, 80, 100, 100, 80, 0, -80, -100);
	}

	for ( $i = 0; $i < 9 ; $i ++ ) {
	    pladv(0);
	    plvsta();
	    plwind($xextreme->at($i,0), $xextreme->at($i,1), $yextreme->at($i,0), $yextreme->at($i,1));
	    
	    plcol0(2) ;
	    plbox(1.0, 0, 10.0, 0, "bc", "bcnv", );
	    plcol0(1) ;
	    plpsty(0) ;

	    if ( $k == 0 ) {
		plfill( $x0, $y0 );
	    } else {
		plgradient( $x0, $y0, 45. );
	    }

	    plcol0(2) ;
	    pllsty(1) ;
	    plline($x0,$y0) ;
	}
    }
}
# Don't forget to call plend() to finish off!

plend();


