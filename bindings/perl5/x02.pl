#!/usr/bin/env perl

# $Id$

#	Multiple window and color map 0 demo.

use PLplot qw(:all);

# --------------------------------------------------------------------------
#  main
#
#  Demonstrates multiple windows and default color map 0 palette.
# --------------------------------------------------------------------------

# Divide screen into 16 regions

plssub (4, 4);

# Parse and process command line arguments */

$argc = scalar @ARGV;
plParseOpts_p (\@ARGV, ("PL_PARSE_FULL"));

# Initialize plplot

plinit ();

plschr (0.0, 3.5);
plfont (4);

foreach $i (0..15) {
  plcol0 ($i);
  $text = "$i";
  pladv (0);
  $vmin = 0.1;
  $vmax = 0.9;
  foreach $j (0..2) {
    plwid ($j + 1);
    plvpor ($vmin, $vmax, $vmin, $vmax);
    plwind (0.0, 1.0, 0.0, 1.0);
    plbox ("bc", 0.0, 0, "bc", 0.0, 0);
    $vmin = $vmin + 0.1;
    $vmax = $vmax - 0.1;
  }
  plwid (1);
  plptex (0.5, 0.5, 1.0, 0.0, 0.5, $text);
}

plend ();
