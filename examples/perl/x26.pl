#! /usr/bin/env perl
#
# Demo x26 for the PLplot PDL binding
#
# Multi-lingual version of the first page of example 4.
#
# Copyright (C) 2006 Alan Irwin
# Copyright (C) 2006 Andrew Ross
#
# Thanks to the following for providing translated strings for this example:
#   Valery Pipin (Russian)
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

#  This example designed just for devices (e.g., psttfc and the
#  cairo-related devices) that use the pango and fontconfig libraries. The
#  best choice of glyph is selected by fontconfig and automatically rendered
#  by pango in way that is sensitive to complex text layout (CTL) language
#  issues for each unicode character in this example. Of course, you must
#  have the appropriate TrueType fonts installed to have access to all the
#  required glyphs.

#   Translation instructions: The strings to be translated are given by
#   x_label, y_label, alty_label, title_label, and line_label below.  The
#   encoding used must be UTF-8.

#   The following strings to be translated involve some scientific/mathematical
#   jargon which is now discussed further to help translators.

#   (1) dB is a decibel unit, see http://en.wikipedia.org/wiki/Decibel .
#   (2) degrees is an angular measure, see
#       http://en.wikipedia.org/wiki/Degree_(angle) .
#   (3) low-pass filter is one that transmits (passes) low frequencies.
#   (4) pole is in the mathematical sense, see
#       http://en.wikipedia.org/wiki/Pole_(complex_analysis) .  "Single Pole"
#       means a particular mathematical transformation of the filter function has
#       a single pole, see
#       http://ccrma.stanford.edu/~jos/filters/Pole_Zero_Analysis_I.html .
#       Furthermore, a single-pole filter must have an inverse square decline
#       (or -20 db/decade). Since the filter plotted here does have that
#       characteristic, it must by definition be a single-pole filter, see also
#       http://www-k.ext.ti.com/SRVS/Data/ti/KnowledgeBases/analog/document/faqs/1p.htm
#   (5) decade represents a factor of 10, see
#       http://en.wikipedia.org/wiki/Decade_(log_scale) .


# SYNC: x26c.c 1.1

use PDL;
use PDL::Graphics::PLplot;
use Math::Trig qw [pi];

use Getopt::Long;

@x_label = (
  "Frequency",
  "Частота"
);

@y_label = (
  "Amplitude (dB)",
  "Амплитуда (dB)"
);

@alty_label = (
  "Phase shift (degrees)",
  "Фазовый сдвиг (градусы)"
);

@legend_text = (
    [ "Amplitude", "Phase shift"   ],
    [ "Амплитуда", "Фазовый сдвиг" ],
);

@title_label = (
  "Single Pole Low-Pass Filter",
  "Однополюсный Низко-Частотный Фильтр"
);

@line_label = (
  "-20 dB/decade",
  "-20 dB/десяток"
);


sub main {

# Parse and process command line arguments

    plParseOpts (\@ARGV, PL_PARSE_SKIP | PL_PARSE_NOPROGRAM);

# Initialize plplot

    plinit();
    plfont(2);

# Make log plots using two different styles.

    for (my $i=0; $i<scalar(@x_label); $i++) {
      plot1(0, $x_label[$i], $y_label[$i], $alty_label[$i],
	    $legend_text[$i], $title_label[$i], $line_label[$i]);
    }

    plend();

}

sub plot1 {
    my ($type, $x_label, $y_label, $alty_label, $legend_text, $title_label, $line_label) = @_;

    pladv(0);

# Set up data for log plot

    my $f0 = 1.0;
    my $freql = -2.0 + sequence(101)/20.0;
    my $freq = 10**$freql;
    my $ampl = 20.0 * log10(1.0 / sqrt(1.0 + ($freq / $f0)**2));
    my $phase = -(180.0 / pi) * atan($freq / $f0);

    plvpor(0.15, 0.85, 0.1, 0.9);
    plwind(-2.0, 3.0, -80.0, 0.0);

# Try different axis and labelling styles.

    plcol0(1);
    if ($type == 0) {
	plbox(0.0, 0,0.0, 0, "bclnst", "bnstv");
    }
    if ($type == 1) {
	plbox(0.0, 0, 0.0, 0, "bcfghlnst", "bcghnstv");
    }

# Plot ampl vs freq

    plcol0(2);
    plline($freql, $ampl);
    plcol0(2);
    plptex(1.6, -30.0, 1.0, -20.0, 0.5, $line_label);

# Put labels on

    plcol0(1);
    plmtex(3.2, 0.5, 0.5, "b", $x_label);
    plmtex(2.0, 0.5, 0.5, "t", $title_label);
    plcol0(2);
    plmtex(5.0, 0.5, 0.5, "l", $y_label);

# For the gridless case, put phase vs freq on same plot

    if ($type == 0) {
	plcol0(1);
	plwind(-2.0, 3.0, -100.0, 0.0);
	plbox(0.0, 0, 30.0, 3, "", "cmstv");
	plcol0(3);
	plline($freql, $phase);
        plstring($freql, $phase, "*");
	plcol0(3);
	plmtex(5.0, 0.5, 0.5, "r", $alty_label);
    }

  # Draw a legend
  my @opt_array   = (PL_LEGEND_LINE, PL_LEGEND_LINE | PL_LEGEND_SYMBOL);
  my @text_colors = (2, 3);
  my @text        = ("Amplitude", "Phase shift");
  my @line_colors = (2, 3);
  my @line_styles = (1, 1);
  my @line_widths = (1, 1);
  my @symbol_colors = (3, 3); # ???
  my @symbol_scales = (1, 1); # ???
  my @symbol_numbers = (4, 4);# ???
  my @symbols        = ('*', '*'); # ???

  plscol0a(15, 32, 32, 32, 0.70);
  my ($legend_width, $legend_height) = pllegend(
      PL_LEGEND_BACKGROUND | PL_LEGEND_BOUNDING_BOX, 0,
      0.0, 0.0, 0.1, 15,
      1, 1, 0, 0,
      $nlegend = 2, \@opt_array,
      1.0, 1.0, 2.0,
      1., \@text_colors, $legend_text,
      0, 0, 0, 0,
      \@line_colors, \@line_styles, \@line_widths,
      \@symbol_colors, \@symbol_scales, \@symbol_numbers, \@symbols);

}

main();
