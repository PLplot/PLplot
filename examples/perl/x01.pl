#! /usr/bin/env perl

use PDL;
use PDL::Graphics::PLplot;
use Math::Trig;

$xscale = 6.;
$yscale = 1.;
$xoff = 0.;
$yoff = 0.;

sub plot1 {

  my $x = pdl ($xoff + $xscale * pdl ([1..60]) / 60.0);
  my $y = pdl ($yoff + $yscale * ($x ** 2.));

  $xmin = $x->index(0);
  $xmax = $x->index(59);
  $ymin = $y->index(0);
  $ymax = $y->index(59);

  my $idx = pdl ([0..5]) * 10 + 3;
  $xs = $x->index($idx);
  $ys = $y->index($idx);

  # Set up the viewport and window using PLENV. The range in X is
  # 0.0 to 6.0, and the range in Y is 0.0 to 30.0. The axes are
  # scaled separately (just = 0), and we just draw a labelled
  # box (axis = 0).

  plcol0 (1);
  plenv ($xmin, $xmax, $ymin, $ymax, 0, 0);
  plcol0 (2);
  pllab ("(x)", "(y)", "#frPLplot Example 1 - y=x#u2");

  # Plot the data points

  plcol0 (4);
  plpoin (6, $xs, $ys, 9);

  # Draw the line through the data

  plcol0 (3);
  plline ($x, $y);

}

sub plot2 {

  # Set up the viewport and window using PLENV. The range in X is -2.0 to
  # 10.0, and the range in Y is -0.4 to 2.0. The axes are scaled separately
  # (just = 0), and we draw a box with axes (axis = 1).

  plcol0 (1);
  plenv (-2.0, 10.0, -0.4, 1.2, 0, 1);
  plcol0 (2);
  pllab ("(x)", "sin(x)/x", "#frPLplot Example 1 - Sinc Function");

  # Fill up the arrays

  my $x = (pdl ([0..99]) - 19.0) / 6.0;
  my $y = sin ($x) / $x;
  $y->index(which ($x == 0)) .= 1.0;

  # Draw the line

  plcol0 (3);
  plwid (2);
  plline ($x, $y);
  plwid (1);
}

sub plot3 {
    my $space1 = 1500;
    my $mark1 = 1500;

    # For the final graph we wish to override the default tick intervals, and
    # so do not use plenv().

    pladv (0);

    # Use standard viewport, and define X range from 0 to 360 degrees, Y range
    # from -1.2 to 1.2.

    plvsta ();
    plwind (0.0, 360.0, -1.2, 1.2);

    # Draw a box with ticks spaced 60 degrees apart in X, and 0.2 in Y.

    plcol0 (1);
    plbox (60.0, 2, 0.2, 2, "bcnst", "bcnstv");

    # Superimpose a dashed line grid, with 1.5 mm marks and spaces.

    plstyl (pdl ([1]), pdl ([$mark1]), pdl ([$space1]));
    plcol0 (2);
    plbox (30.0, 0, 0.2, 0, "g", "g");
    my $z = pdl [0];
    plstyl ($z, $z, $z);

    plcol0 (3);
    pllab ("Angle (degrees)", "sine", "#frPLplot Example 1 - Sine function");

    my $x = 3.6 * pdl ([0..100]);
    my $y = sin ($x * pi / 180.0);

    plcol0 (4);
    plline ($x, $y);
}

# plplot initialization
# Divide page into 2x2 plots unless user overrides

plssub (2, 2);

# Initialize plplot

plinit ();

# Set up the data
# Original case

$xscale = 6.;
$yscale = 1.;
$xoff = 0.;
$yoff = 0.;

# Set up the data

$xscale = 1.;
$yscale = 0.0014;
$yoff = 0.0185;

# Do a plot

plot1;

$digmax = 5;
plsyax ($digmax, 0);

plot1;

plot2;

plot3;

plend;
