#!/usr/bin/env perl

# $Id$
# Simple line plot and multiple windows demo.  In Perl

use PLplot qw(:all);

use Getopt::Long;

# plplot initialization
# Divide page into 2x2 plots

plssub (2,2);

Getopt::Long::Configure ("pass_through");
GetOptions qw(-locate! -fontset=i -xor! -h!);

if (defined $opt_h) {
  print "\n$0 options:\n" .
    "    -locate\t\t Turns on test of API locate function\n" .
    "    -fontset\t\t Selects stroke font set (0 or 1, def:1)\n" .
    "    -xor\t\t Turns on test of XOR\n";
  push @ARGV, "-h";
}

$locate_mode = (defined $opt_locate) ? 1 : 0;
$fontset = (defined $opt_fontset) ? $opt_fontset : 1;
$test_xor = (defined $opt_xor) ? $opt_xor : 0;

# Also possible: 
#    plParseOpts_p (\@ARGV, "PL_PARSE_FULL", "PL_PARSE_NOPROGRAM");

plParseOpts_p ([$0, @ARGV], "PL_PARSE_FULL");

# Get version number, just for kicks

plgver (\$ver);
print "Plplot library version: $ver\n";

# Initialize plplot

plinit ();

# Select font set as per input flag

plfontld ($fontset ? 1 : 0);

# Set up the data
# Original case 

$xscale = 6.;
$yscale = 1.;
$xoff = 0.;
$yoff = 0.;

# Do a plot

plot1 (0);

$xscale = 1.;
$yscale = 0.0014;
$yoff = 0.0185;

# Do a plot

$digmax = 5;
plsyax ($digmax, 0);

plot1 (1);

plot2 ();

plot3 ();

plend ();

# =============================================================== 

sub plot1
{
  my $do_test = shift;

  @x = map { $xoff + $xscale * ($_ + 1) / 60.0 } (0..59);
  @y = map { $yoff + $yscale * $x[$_] * $x[$_] } (0..59);
  
  my $xmin = $x[0];
  my $xmax = $x[59];
  my $ymin = $y[0];
  my $ymax = $y[59];
  
  my @xs = map { $x[$_ * 10 + 3] } (0..5);
  my @ys = map { $y[$_ * 10 + 3] } (0..5);
  
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

  plpoin (6, \@xs, \@ys, 9);

  # Draw the line through the data
  
  plcol0 (3);
  plline_p (\@x, \@y);

  if ($do_test and $test_xor) {
    print "Xor test not yet implemented\n";
  }

}

# =============================================================== 
sub plot2
{

  # Set up the viewport and window using PLENV. The range in X is -2.0 to
  # 10.0, and the range in Y is -0.4 to 2.0. The axes are scaled separately
  # (just = 0), and we draw a box with axes (axis = 1). 

  plcol0 (1);
  plenv (-2.0, 10.0, -0.4, 1.2, 0, 1);
  plcol0 (2);
  pllab ("(x)", "sin(x)/x", "#frPLplot Example 1 - Sinc Function");
  
  # Fill up the arrays
  
  foreach my $i (0..99) {
    $x[$i] = ($i - 19.0) / 6.0;
    $y[$i] = 1.0;
    if ($x[$i] != 0.0) {
      $y[$i] = sin($x[$i]) / $x[$i];
    }
  }

  # Draw the line 

  plcol0 (3);
  plline_p (\@x, \@y);
}

# ===============================================================

sub plot3
{
  $space1 = 1500;
  $mark1 = 1500;

  # For the final graph we wish to override the default tick intervals, and
  # so do not use plenv().
  
  pladv (0);
  
  # Use standard viewport, and define X range from 0 to 360 degrees, Y range
  # from -1.2 to 1.2. 
  
  plvsta ();
  plwind (0.0, 360.0, -1.2, 1.2);
  
  # Draw a box with ticks spaced 60 degrees apart in X, and 0.2 in Y.
  
  plcol0 (1);
  plbox ("bcnst", 60.0, 2, "bcnstv", 0.2, 2);

  # Superimpose a dashed line grid, with 1.5 mm marks and spaces. 
  # plstyl expects a pointer!
  
  plstyl_p ($mark1, $space1);
  plcol0 (2);
  plbox ("g", 30.0, 0, "g", 0.2, 0);
  plstyl_p ([], []);

  plcol0 (3);
  pllab ("Angle (degrees)", "sine", "#frPLplot Example 1 - Sine function");

  @x = map { 3.6 * $_ } (0..100);
  @y = map { sin($_ * 3.141592654 / 180.0) } @x;

  plcol0 (4);
  plline_p (\@x, \@y);
  plline_p (\@x, \@y);
}
