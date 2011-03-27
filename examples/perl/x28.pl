#! /usr/bin/env perl

#
#	plmtex3, plptex3 demo.
#
#   Copyright (C) 2007 Alan W. Irwin
#   Perl/PDL version 2008 Doug Hunt
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

# Choose these values to correspond to tick marks.

use PDL;
use PDL::Graphics::PLplot;

use constant XPTS => 2;
use constant YPTS => 2;
use constant NREVOLUTION => 16;
use constant NROTATION   => 8;
use constant NSHEAR      => 8;
use constant PI          => 4*atan2(1,1);

my $xmin = 0;
my $xmax = 1.0;
my $xmid = 0.5*($xmax + $xmin);
my $xrange = $xmax - $xmin;

my $ymin = 0;
my $ymax = 1.0;
my $ymid = 0.5*($ymax + $ymin);
my $yrange = $ymax - $ymin;

my $zmin = 0;
my $zmax = 1.0;
my $zmid = 0.5*($zmax + $zmin);
my $zrange = $zmax - $zmin;

my $ysmin    = $ymin + 0.1 * $yrange;
my $ysmax    = $ymax - 0.1 * $yrange;
my $ysrange  = $ysmax - $ysmin;
my $dysrot   = $ysrange / ( NROTATION - 1 );
my $dysshear = $ysrange / ( NSHEAR - 1 );
my $zsmin    = $zmin + 0.1 * $zrange;
my $zsmax    = $zmax - 0.1 * $zrange;
my $zsrange  = $zsmax - $zsmin;
my $dzsrot   = $zsrange / ( NROTATION - 1 );
my $dzsshear = $zsrange / ( NSHEAR - 1 );
my $ys;
my $zx;

my $pstring = "The future of our civilization depends on software freedom.";

my $x = $xmin + ($xmax-$xmin)*sequence(XPTS)/(XPTS-1);
my $y = $ymin + ($ymax-$ymin)*sequence(YPTS)/(YPTS-1);
my $z = zeroes(XPTS,YPTS);

# Parse and process command line arguments

plParseOpts (\@ARGV, PL_PARSE_SKIP | PL_PARSE_NOPROGRAM);

plinit();

# Page 1: Demonstrate inclination and shear capability pattern.

pladv(0);
plvpor(-0.15, 1.15, -0.05, 1.05);
plwind(-1.2, 1.2, -0.8, 1.5);
plw3d(1.0, 1.0, 1.0, $xmin, $xmax, $ymin, $ymax, $zmin, $zmax, 20., 45.);

plcol0(2);
plbox3($xmax-$xmin, 0, $ymax-$ymin, 0, $zmax-$zmin, 0, "b", "", "b", "", "bcd", ""); # note reordered arguments!

my $omega = 2*PI*(sequence(NREVOLUTION)/NREVOLUTION);

# z = zmin.
plschr(0., 1.0);
my $x_inclination = 0.5*$xrange*cos($omega);
my $y_inclination = 0.5*$yrange*sin($omega);
my $z_inclination = 0;
my $x_shear = -0.5*$xrange*sin($omega);
my $y_shear =  0.5*$yrange*cos($omega);
my $z_shear =  0;
plptex3(
    $xmid, $ymid, $zmin,
    $x_inclination, $y_inclination, $z_inclination,
    $x_shear, $y_shear, $z_shear,
    0.0, "  revolution");

# x = xmax.
$x_inclination =  0;
$y_inclination = -0.5*$yrange*cos($omega);
$z_inclination =  0.5*$zrange*sin($omega);
$x_shear = 0;
$y_shear = 0.5*$yrange*sin($omega);
$z_shear = 0.5*$zrange*cos($omega);
plptex3(
    $xmax, $ymid, $zmid,
    $x_inclination, $y_inclination, $z_inclination,
    $x_shear, $y_shear, $z_shear, 
    0.0, "  revolution");

$x_inclination = 0.5*$xrange*cos($omega);
$y_inclination = 0;
$z_inclination = 0.5*$zrange*sin($omega);
$x_shear = -0.5*$xrange*sin($omega);
$y_shear =  0;
$z_shear =  0.5*$zrange*cos($omega);
plptex3(
    $xmid, $ymax, $zmid,
    $x_inclination, $y_inclination, $z_inclination,
    $x_shear, $y_shear, $z_shear, 
    0.0, "  revolution");

# Draw minimal 3D grid to finish defining the 3D box.
plmesh($x, $y, $z, DRAW_LINEXY);


# Page 2: Demonstrate rotation of string around its axis.
pladv(0);
plvpor(-0.15, 1.15, -0.05, 1.05);
plwind(-1.2, 1.2, -0.8, 1.5);
plw3d(1.0, 1.0, 1.0, $xmin, $xmax, $ymin, $ymax, $zmin, $zmax, 20, 45);

plcol0(2);
plbox3($xmax-$xmin, 0, $ymax-$ymin, 0, $zmax-$zmin, 0, "b", "", "b", "", "bcd", "");

# y = ymax.
$x_inclination = 1;
$y_inclination = 0;
$z_inclination = 0;
$x_shear = 0;
$omega = 2*PI*(sequence(NROTATION)/NROTATION);
$y_shear = 0.5*$yrange*sin($omega);
$z_shear = 0.5*$zrange*cos($omega);
$zs = $zsmax - $dzsrot * sequence(NROTATION);
plptex3(
    $xmid, $ymax, $zs,
    $x_inclination, $y_inclination, $z_inclination,
    $x_shear, $y_shear, $z_shear,
    0.5, "rotation for y = y#dmax#u");

# x = xmax.
$x_inclination =  0;
$y_inclination = -1;
$z_inclination =  0;
$y_shear = 0;
$x_shear = 0.5*$xrange*sin($omega);
$z_shear = 0.5*$zrange*cos($omega);
$zs = $zsmax - $dzsrot * sequence(NROTATION);
plptex3(
    $xmax, $ymid, $zs,
    $x_inclination, $y_inclination, $z_inclination,
    $x_shear, $y_shear, $z_shear,
    0.5, "rotation for x = x#dmax#u");

# z = zmin.
$x_inclination = 1;
$y_inclination = 0;
$z_inclination = 0;
$x_shear = 0.;
$y_shear = 0.5*$yrange*cos($omega);
$z_shear = 0.5*$zrange*sin($omega);
$ys = $ysmax - $dysrot * sequence(NROTATION);
plptex3(
    $xmid, $ys, $zmin,
    $x_inclination, $y_inclination, $z_inclination,
    $x_shear, $y_shear, $z_shear,
    0.5, "rotation for z = z#dmin#u");

# Draw minimal 3D grid to finish defining the 3D box.
plmesh($x, $y, $z, DRAW_LINEXY);

# Page 3: Demonstrate shear of string along its axis.
# Work around xcairo and pngcairo (but not pscairo) problems for 
# shear vector too close to axis of string. (N.B. no workaround
# would be domega = 0.)
my $domega = 0.05;
pladv(0);
plvpor(-0.15, 1.15, -0.05, 1.05);
plwind(-1.2, 1.2, -0.8, 1.5);
plw3d(1.0, 1.0, 1.0, $xmin, $xmax, $ymin, $ymax, $zmin, $zmax, 20, 45);

plcol0(2);
plbox3($xmax-$xmin, 0,
       $ymax-$ymin, 0,
       $zmax-$zmin, 0, "b", "", "b", "", "bcd", "");

# y = ymax.
plschr(0., 1.0);
$x_inclination = 1;
$y_inclination = 0;
$z_inclination = 0;
$y_shear = 0;

$omega = $domega + 2*PI*(sequence(NSHEAR)/NSHEAR);

$x_shear = 0.5*$xrange*sin($omega);
$z_shear = 0.5*$zrange*cos($omega);
$zs = $zsmax - $dzsshear * sequence(NSHEAR);
plptex3(
    $xmid, $ymax, $zs,
    $x_inclination, $y_inclination, $z_inclination,
    $x_shear, $y_shear, $z_shear,
    0.5, "shear for y = y#dmax#u");

# x = xmax.
$x_inclination =  0;
$y_inclination = -1;
$z_inclination =  0;
$x_shear =  0;
$y_shear = -0.5*$yrange*sin($omega);
$z_shear =  0.5*$zrange*cos($omega);
$zs = $zsmax - $dzsshear * sequence(NSHEAR);
plptex3(
    $xmax, $ymid, $zs,
    $x_inclination, $y_inclination, $z_inclination,
    $x_shear, $y_shear, $z_shear,
    0.5, "shear for x = x#dmax#u");

# z = zmin.
$x_inclination = 1;
$y_inclination = 0;
$z_inclination = 0;
$z_shear = 0;

$y_shear = 0.5*$yrange*cos($omega);
$x_shear = 0.5*$xrange*sin($omega);
$ys = $ysmax - $dysshear * sequence(NSHEAR);
plptex3(
    $xmid, $ys, $zmin,
    $x_inclination, $y_inclination, $z_inclination,
    $x_shear, $y_shear, $z_shear,
    0.5, "shear for z = z#dmin#u");

# Draw minimal 3D grid to finish defining the 3D box.
plmesh($x, $y, $z, DRAW_LINEXY);

# Page 4: Demonstrate drawing a string on a 3D path.
pladv(0);
plvpor(-0.15, 1.15, -0.05, 1.05);
plwind(-1.2, 1.2, -0.8, 1.5);
plw3d(1.0, 1.0, 1.0, $xmin, $xmax, $ymin, $ymax, $zmin, $zmax, 40, -30);

plcol0(2);
plbox3($xmax-$xmin, 0,
       $ymax-$ymin, 0,
       $zmax-$zmin, 0, "b", "", "b", "", "bcd", "");

plschr(0, 1.2);
# domega controls the spacing between the various characters of the
# string and also the maximum value of omega for the given number
# of characters in *pstring.
my $domega = 2*PI/length($pstring);
my $omega  = 0;

# 3D function is a helix of the given radius and pitch 

my $radius = 0.5;
my $pitch  = 1/(2*PI);

# The shear vector should be perpendicular to the 3D line with Z
# component maximized, but for low pitch a good approximation is
# a constant vector that is parallel to the Z axis.
my $x_shear = 0;
my $y_shear = 0;
my $z_shear = 1;

foreach my $c (split '', $pstring) {
    my $sin_omega = sin($omega);
    my $cos_omega = cos($omega);
    my $xpos = $xmid + $radius*$sin_omega;
    my $ypos = $ymid - $radius*$cos_omega;
    my $zpos = $zmin + $pitch*$omega;

    # In general, the inclination is proportional to the derivative of 
    # the position wrt theta.
    my $x_inclination = $radius*$cos_omega;;
    my $y_inclination = $radius*$sin_omega;
    my $z_inclination = $pitch;

    plptex3(
	$xpos, $ypos, $zpos,
	$x_inclination, $y_inclination, $z_inclination,
	$x_shear, $y_shear, $z_shear,
	0.5, $c);
    $omega += $domega;
}

# Draw minimal 3D grid to finish defining the 3D box.
plmesh($x, $y, $z, DRAW_LINEXY);

# Page 5: Demonstrate plmtex3 axis labelling capability
pladv(0);
plvpor(-0.15, 1.15, -0.05, 1.05);
plwind(-1.2, 1.2, -0.8, 1.5);
plw3d(1.0, 1.0, 1.0, $xmin, $xmax, $ymin, $ymax, $zmin, $zmax, 20, 45);

plcol0(2);
plbox3($xmax-$xmin, 0,
       $ymax-$ymin, 0,
       $zmax-$zmin, 0, "b", "", "b", "", "bcd", "");

plschr(0, 1.0);
plmtex3( 3.0, 0.5, 0.5, "xp", "Arbitrarily displaced");
plmtex3( 4.5, 0.5, 0.5, "xp", "primary X-axis label");
plmtex3(-2.5, 0.5, 0.5, "xs", "Arbitrarily displaced");
plmtex3(-1.0, 0.5, 0.5, "xs", "secondary X-axis label");
plmtex3( 3.0, 0.5, 0.5, "yp", "Arbitrarily displaced");
plmtex3( 4.5, 0.5, 0.5, "yp", "primary Y-axis label");
plmtex3(-2.5, 0.5, 0.5, "ys", "Arbitrarily displaced");
plmtex3(-1.0, 0.5, 0.5, "ys", "secondary Y-axis label");
plmtex3( 4.5, 0.5, 0.5, "zp", "Arbitrarily displaced");
plmtex3( 3.0, 0.5, 0.5, "zp", "primary Z-axis label");
plmtex3(-2.5, 0.5, 0.5, "zs", "Arbitrarily displaced");
plmtex3(-1.0, 0.5, 0.5, "zs", "secondary Z-axis label");

# Draw minimal 3D grid to finish defining the 3D box.
plmesh($x, $y, $z, DRAW_LINEXY);

plend();
