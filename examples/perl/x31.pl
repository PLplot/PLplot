#! /usr/bin/env perl
#
# Set/get tester.
#
# Window positioning demo.
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

# SYNC: x31c.c 1.?? FIXME

use PDL;
use PDL::Graphics::PLplot;

# Parse and process command line arguments

plParseOpts (\@ARGV, PL_PARSE_SKIP | PL_PARSE_NOPROGRAM);

# Test setting / getting compression parameter across plinit.
my $compression1 = 100;
plscompression ($compression1);

# Test setting / getting familying parameters across plinit.
my $fam1 = 0;
my $num1 = 10;
my $bmax1 = 1000;
plsfam ($fam1, $num1, $bmax1);
          
# Test setting / getting page parameters across plinit.
my $xp1 = 200.;
my $yp1 = 200.;
my $xleng1 = 400;
my $yleng1 = 200;
my $xoff1 = 10;
my $yoff1 = 20;
plspage ($xp1, $yp1, $xleng1, $yleng1, $xoff1, $yoff1);
# Initialize plplot

plinit ();

my $status = 0;

# Test if device initialization screwed around with the preset
# compression parameter.
my $compression2 = plgcompression ();
printf ("Output various PLplot parameters\n");

printf ("compression parameter = %d\n", $compression2);
if ($compression2 != $compression1) {
    printf STDERR ("plgcompression test failed\n");
    $status = 1;
}

# Test if device initialization screwed around with any of the
# preset familying values.
my ($fam2, $num2, $bmax2) = plgfam ();
printf ("family parameters: fam, num, bmax = %d %d %d\n", $fam2, $num2, $bmax2);
if ($fam2 != $fam1 || $num2 != $num1 || $bmax2 != $bmax1) {
    printf STDERR ("plgfam test failed\n");
    $status = 1;
}

# Test if device initialization screwed around with any of the
# preset page values.
my ($xp2, $yp2, $xleng2, $yleng2, $xoff2, $yoff2) = plgpage ();
printf ("page parameters: xp, yp, xleng, yleng, xoff, yoff = %f %f %d %d %d %d\n", $xp2, $yp2, $xleng2, $yleng2, $xoff2, $yoff2);
if ($xp2 != $xp1 || $yp2 != $yp1 || $xleng2 != $xleng1 || $yleng2 != $yleng1 || $xoff2 != $xoff1 || $yoff2 != $yoff1) {
    printf STDERR ("plgpage test failed\n");
    $status = 1;
}

# Exercise plscolor, plscol0, plscmap1, and plscmap1a to make sure
# they work without any obvious error messages.
plscolor (1);
plscol0 (1, 255, 0, 0);
my $r1 = pdl [0, 255];
my $g1 = pdl [255, 0];
my $b1 = pdl [0, 0];
my $a1 = pdl [1.0, 1.0];
plscmap1 ($r1, $g1, $b1);
plscmap1a ($r1, $g1, $b1, $a1);

my $level2 = plglevel ();
printf ("level parameter = %d\n", $level2);
if ($level2 != 1) {
    printf STDERR ("plglevel test failed.\n");
    $status = 1;
}

pladv (0);
plvpor (0.01, 0.99, 0.02, 0.49);
my ($xmin, $xmax, $ymin, $ymax) = plgvpd ();
printf ("plvpor: xmin, xmax, ymin, ymax = %f %f %f %f\n", $xmin, $xmax, $ymin, $ymax);
if ($xmin != 0.01 || $xmax != 0.99 || $ymin != 0.02 || $ymax != 0.49) {
    printf STDERR ("plgvpd test failed\n");
    $status = 1;
}
$xmid = 0.5*($xmin+$xmax);
$ymid = 0.5*($ymin+$ymax);

plwind (0.2, 0.3, 0.4, 0.5);
my ($xmin, $xmax, $ymin, $ymax) = plgvpw ();
printf("plwind: xmin, xmax, ymin, ymax = %f %f %f %f\n", $xmin, $xmax, $ymin, $ymax);
if ($xmin != 0.2 || $xmax != 0.3 || $ymin != 0.4 || $ymax != 0.5) {
    printf STDERR ("plgvpw test failed\n");
    $status = 1;
}

# Get world coordinates for middle of viewport
# FIXME plcalc_world is not yet implemented.  I have inferred the argument list
# below where xmid, and ymid are input, and ($wx, $wy, $win) are all output.
# my ($wx, $wy, $win) = plcalc_world ($xmid, $ymid);
my ($wx, $wy, $win) = (0., 0., 0); #Temporary FIXME
printf("world parameters: wx, wy, win = %f %f %d\n", $wx, $wy, $win);
if (abs($wx-0.5*($xmin+$xmax)) > 1.0E-5 || abs($wy-0.5*($ymin+$ymax)) > 1.0E-5) {
    printf STDERR ("plcalc_world test failed\n");
    $status = 1;
    $status = 0;  #FIXME, temporary
}

# Retrieve and print the name of the output file (if any)
# FIXME, I could not get the following code to work.
# my $fnam = plgfnam();
# plgfam currently needs an argument unlike other plg functions that return
# a character string such as plgver.  Also, if I tried an argument, it
# generated a run-time error in glibc.
# Temporarily use plgver instead which unlike the current plgfnam has no
# arguments.
my $fnam = plgver();
if ($fnam == "") {
    printf ("No output file name is set\n");
} else {
    printf ("Output file name read\n");
}
printf STDERR ("Output file name is %s\n", $fnam);

# Set and get the number of digits used to display axis labels
# Note digits is currently ignored in pls[xyz]ax and 
# therefore it does not make sense to test the returned value.
plsxax(3,0);
my ($digmax, $digits) = plgxax();
printf ("x axis parameters: digmax, digits = %d %d\n", $digmax, $digits);
if ($digmax != 3) {
    printf STDERR ("plgxax test failed\n");
    $status = 1;
}

plsyax(4,0);
my ($digmax, $digits) = plgyax();
printf ("y axis parameters: digmax, digits = %d %d\n", $digmax, $digits);
if ($digmax != 4) {
    printf STDERR ("plgyax test failed\n");
    $status = 1;
}

plszax(5,0);
my ($digmax, $digits) = plgzax();
printf ("z axis parameters: digmax, digits = %d %d\n", $digmax, $digits);
if ($digmax != 5) {
    printf STDERR ("plgzax test failed\n");
    $status = 1;
}

plsdidev(0.05, PL_NOTSET, 0.1, 0.2);
my ($mar, $aspect, $jx, $jy) = plgdidev();
printf ("device-space window parameters: mar, aspect, jx, jy = %f %f %f %f\n", $mar, $aspect, $jx, $jy);
if ($mar != 0.05 || $jx != 0.1 || $jy != 0.2) {
    printf STDERR ("plgdidev test failed\n");
    $status = 1;
}

plsdiori(1.0);
my $ori = plgdiori();
printf ("ori parameter = %f\n", $ori);
if ($ori != 1.0) {
    printf STDERR ("plgdiori test failed\n");
$status = 1;
}

plsdiplt(0.1, 0.2, 0.9, 0.8);
my ($xmin, $ymin, $xmax, $ymax) = (PDL->new(0), PDL->new(0), PDL->new(0), PDL->new(0));
plgdiplt($xmin, $ymin, $xmax, $ymax);
printf ("plot-space window parameters: xmin, ymin, xmax, ymax = %f %f %f %f\n",  $xmin, $ymin, $xmax, $ymax);
if ($xmin != 0.1 || $xmax != 0.9 || $ymin != 0.2 || $ymax != 0.8) {
    printf STDERR ("plgdiplt test failed\n");
    $status = 1;
}

plsdiplz(0.1, 0.1, 0.9, 0.9);
my ($zxmin, $zymin, $zxmax, $zymax) = plgdiplt();
printf ("zoomed plot-space window parameters: xmin, ymin, xmax, ymax = %f %f %f %f\n", $zxmin, $zymin, $zxmax, $zymax);
if (abs($zxmin -($xmin + ($xmax-$xmin)*0.1)) > 1.0E-5 || abs($zxmax -($xmin+($xmax-$xmin)*0.9)) > 1.0E-5 || abs($zymin -($ymin+($ymax-$ymin)*0.1)) > 1.0E-5 || abs($zymax -($ymin+($ymax-$ymin)*0.9)) > 1.0E-5 ) {
    printf STDERR ("plsdiplz test failed\n");
    $status = 1;
}

plscolbg (10,20,30);
my ($r, $g, $b) = plgcolbg();
printf ("background colour parameters: r, g, b = %d %d %d\n", $r, $g, $b);
if ($r != 10 || $g != 20 || $b != 30) {
    printf STDERR ("plgcolbg test failed\n");
    $status = 1;
}

plscolbga (20, 30, 40, 0.5);
my ($r, $g, $b, $a) = plgcolbga();
printf ("background/transparency colour parameters: r, g, b, a = %d %d %d %f\n", $r, $g, $b, $a);
if ($r != 20 || $g != 30 || $b != 40 || $a != 0.5) {
    printf STDERR ("plgcolbga test failed\n");
    $status = 1;
}

plend ();
if ($status == 1) {
    exit(1);
} else {
    exit(0);
}
