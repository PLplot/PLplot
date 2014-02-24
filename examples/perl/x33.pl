#! /usr/bin/env perl
#
# Demonstrate most pllegend capability including unicode symbols.
#
# Copyright (C) 2010 Alan Irwin
# Perl/PDL translation Copyright (C) 2011 Doug Hunt
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
#

# This example designed just for devices (e.g., the cairo-related and
# qt-related devices) where the best choice of glyph is automatically
# selected by the related libraries (pango/cairo or Qt4) for each
# unicode character depending on what system fonts are installed.  Of
# course, you must have the appropriate TrueType fonts installed to
# have access to all the required glyphs.

use PDL;
use PDL::Graphics::PLplot;
use Getopt::Long qw [:config pass_through];

my @position_options = (
    PL_POSITION_LEFT   | PL_POSITION_TOP    | PL_POSITION_OUTSIDE,
    PL_POSITION_TOP    | PL_POSITION_OUTSIDE,
    PL_POSITION_RIGHT  | PL_POSITION_TOP    | PL_POSITION_OUTSIDE,
    PL_POSITION_RIGHT  | PL_POSITION_OUTSIDE,
    PL_POSITION_RIGHT  | PL_POSITION_BOTTOM | PL_POSITION_OUTSIDE,
    PL_POSITION_BOTTOM | PL_POSITION_OUTSIDE,
    PL_POSITION_LEFT   | PL_POSITION_BOTTOM | PL_POSITION_OUTSIDE,
    PL_POSITION_LEFT   | PL_POSITION_OUTSIDE,
    PL_POSITION_LEFT   | PL_POSITION_TOP    | PL_POSITION_INSIDE,
    PL_POSITION_TOP    | PL_POSITION_INSIDE,
    PL_POSITION_RIGHT  | PL_POSITION_TOP    | PL_POSITION_INSIDE,
    PL_POSITION_RIGHT  | PL_POSITION_INSIDE,
    PL_POSITION_RIGHT  | PL_POSITION_BOTTOM | PL_POSITION_INSIDE,
    PL_POSITION_BOTTOM | PL_POSITION_INSIDE,
    PL_POSITION_LEFT   | PL_POSITION_BOTTOM | PL_POSITION_INSIDE,
    PL_POSITION_LEFT   | PL_POSITION_INSIDE,
);

# Pick 5 arbitrary UTF-8 symbols useful for plotting points (✠✚✱✪✽✺✰✴✦).
my @special_symbols = (
    "✰",
    "✴",
    "✱",
    "✽",
    "✦",
);

# plcolorbar options

# Colorbar type options
my @colorbar_option_kinds = (
    PL_COLORBAR_SHADE,
    PL_COLORBAR_SHADE | PL_COLORBAR_SHADE_LABEL,
    PL_COLORBAR_IMAGE,
    PL_COLORBAR_GRADIENT,
);
my @colorbar_option_kind_labels = (
    "Shade colorbars",
    "Shade colorbars with custom labels",
    "Image colorbars",
    "Gradient colorbars",
);

# Which side of the page are we positioned relative to?
my @colorbar_position_options = (
    PL_POSITION_LEFT,
    PL_POSITION_RIGHT,
    PL_POSITION_TOP,
    PL_POSITION_BOTTOM,
);
my @colorbar_position_option_labels = (
    "Left",
    "Right",
    "Top",
    "Bottom",
);

# Colorbar label positioning options
my @colorbar_label_options = (
    PL_COLORBAR_LABEL_LEFT,
    PL_COLORBAR_LABEL_RIGHT,
    PL_COLORBAR_LABEL_TOP,
    PL_COLORBAR_LABEL_BOTTOM,
);
my @colorbar_label_option_labels = (
    "Label left",
    "Label right",
    "Label top",
    "Label bottom",
);

# Colorbar cap options
my @colorbar_cap_options = (
    PL_COLORBAR_CAP_NONE,
    PL_COLORBAR_CAP_LOW,
    PL_COLORBAR_CAP_HIGH,
    PL_COLORBAR_CAP_LOW | PL_COLORBAR_CAP_HIGH,
);
my @colorbar_cap_option_labels = (
    "No caps",
    "Low cap",
    "High cap",
    "Low and high caps",
);

my $colorbar = 0; # By default do not plot plcolorbar pages
                  # for now while we are working out the API.

GetOptions ("colorbar" => \$colorbar,
            "help"   => \$help);

if ($help) {
  print (<<EOT);
$0 options:
    --colorbar            Plot the \"color bar\" pages.
EOT
  push (@ARGV, "-h");
}
unshift (@ARGV, $0);

#--------------------------------------------------------------------------
# main
#
# Demonstrate most pllegend capability including unicode symbols.
#--------------------------------------------------------------------------

use constant MAX_NLEGEND => 7;
use constant NULL        => 0;

plParseOpts(\@ARGV, PL_PARSE_FULL);

# Initialize plplot
plinit();

# First page illustrating the 16 standard positions.
pladv( 0 );
plvpor( 0.25, 0.75, 0.25, 0.75 );
plwind( 0.0, 1.0, 0.0, 1.0 );
plbox( 0.0, 0, 0.0, 0, "bc", "bc" );
plsfont( PL_FCI_SANS, -1, -1 );
plmtex( 8.0, 0.5, 0.5, "t", "The 16 standard legend positions with" );
plmtex( 6.0, 0.5, 0.5, "t", "the same (0.05) offset in x and y" );

my $nlegend = 1;
# Only specify legend data that are required according to the
# value of opt_array for that entry.
my $opt_base          = PL_LEGEND_BACKGROUND | PL_LEGEND_BOUNDING_BOX;

my (@opt_array, @line_styles, @line_widths, @symbol_scales, @symbol_numbers, @symbols,
    @text, @text_colors, @line_colors, @symbol_colors);
$opt_array[0]      = PL_LEGEND_LINE | PL_LEGEND_SYMBOL;
$line_styles[0]    = 1;
$line_widths[0]    = 1;
$symbol_scales[0]  = 1.;
$symbol_numbers[0] = 4;
$symbols[0]        = "*";

# Use monotype fonts so that all legends are the same size.
plsfont( PL_FCI_MONO, -1, -1 );
plscol0a( 15, 32, 32, 32, 0.70 );

for ( $k = 0; $k < 16; $k++ )
  {
    $position = $position_options[$k];
    $opt      = $opt_base;
    $text[0]  = sprintf( "%2.2d", $k );
    $text_colors[0]   = 1 + ( $k % 8 );
    $line_colors[0]   = 1 + ( $k % 8 );
    $symbol_colors[0] = 1 + ( $k % 8 );

    my ($legend_width, $legend_height) = pllegend($opt, $position, 0.05, 0.05,
                                                  0.1, 15, 1, 1, 0, 0,
                                                  $nlegend, \@opt_array, 1.0, 1.0, 2.0,
                                                  1., \@text_colors, \@text,
                                                  NULL, NULL, NULL, NULL,
                                                  \@line_colors, \@line_styles, \@line_widths,
                                                  \@symbol_colors, \@symbol_scales, \@symbol_numbers, \@symbols );
  }

# Second page illustrating effect of nrow, ncolumn for the same legend
# data.;
pladv( 0 );
plvpor( 0.25, 0.75, 0.25, 0.75 );
plwind( 0.0, 1.0, 0.0, 1.0 );
plbox( 0.0, 0, 0.0, 0,  "bc", "bc" );
plsfont( PL_FCI_SANS, -1, -1 );
plmtex( 8.0, 0.5, 0.5, "t", "The effect of nrow, ncolumn, PL_LEGEND_ROW_MAJOR," );
plmtex( 6.0, 0.5, 0.5, "t", "and position for the same legend data" );

$nlegend = 7;

# Only specify legend data that are required according to the
# value of opt_array for that entry.
my $opt_base = PL_LEGEND_BACKGROUND | PL_LEGEND_BOUNDING_BOX;
for ( my $k = 0; $k < $nlegend; $k++ )
  {
    $opt_array[$k]    = PL_LEGEND_LINE | PL_LEGEND_SYMBOL;
    $line_styles[$k]  = 1;
    $line_widths[$k]  = 1;
    $symbol_scales[$k] = 1.;
    $symbol_numbers[$k]= 2;
    $symbols[$k]       = "*";
    $text[$k] = sprintf("%2.2d", $k );
    $text_colors[$k]   = 1 + ( $k % 8 );
    $line_colors[$k]   = 1 + ( $k % 8 );
    $symbol_colors[$k] = 1 + ( $k % 8 );
  }

# Use monotype fonts so that all legends are the same size.
plsfont( PL_FCI_MONO, -1, -1 );
plscol0a( 15, 32, 32, 32, 0.70 );
$position = PL_POSITION_TOP | PL_POSITION_OUTSIDE;
$opt      = $opt_base;
$x        = 0.;
$y        = 0.1;
$nrow     = 1;
$ncolumn  = $nlegend;
my ($legend_width, $legend_height) = pllegend($opt, $position, $x, $y,
                                              0.05, 15, 1, 1, $nrow, $ncolumn,
                                              $nlegend, \@opt_array, 1.0, 1.0, 2.0,
                                              1., \@text_colors, \@text,
                                              NULL, NULL, NULL, NULL,
                                              \@line_colors, \@line_styles, \@line_widths,
                                              \@symbol_colors, \@symbol_scales, \@symbol_numbers, \@symbols );

$position = PL_POSITION_BOTTOM | PL_POSITION_OUTSIDE;
$opt      = $opt_base;
$x        = 0.;
$y        = 0.1;
$nrow     = 1;
$ncolumn  = $nlegend;
my ($legend_width, $legend_height) = pllegend($opt, $position, $x, $y,
                                              0.05, 15, 1, 1, $nrow, $ncolumn,
                                              $nlegend, \@opt_array, 1.0, 1.0, 2.0,
                                              1., \@text_colors, \@text,
                                              NULL, NULL, NULL, NULL,
                                              \@line_colors, \@line_styles, \@line_widths,
                                              \@symbol_colors, \@symbol_scales, \@symbol_numbers, \@symbols );

$position = PL_POSITION_LEFT | PL_POSITION_OUTSIDE;
$opt      = $opt_base;
$x        = 0.1;
$y        = 0.;
$nrow     = $nlegend;
$ncolumn  = 1;
my ($legend_width, $legend_height) = pllegend($opt, $position, $x, $y,
                                              0.05, 15, 1, 1, $nrow, $ncolumn,
                                              $nlegend, \@opt_array, 1.0, 1.0, 2.0,
                                              1., \@text_colors, \@text,
                                              NULL, NULL, NULL, NULL,
                                              \@line_colors, \@line_styles, \@line_widths,
                                              \@symbol_colors, \@symbol_scales, \@symbol_numbers, \@symbols );

$position = PL_POSITION_RIGHT | PL_POSITION_OUTSIDE;
$opt      = $opt_base;
$x        = 0.1;
$y        = 0.;
$nrow     = $nlegend;
$ncolumn  = 1;
my ($legend_width, $legend_height) = pllegend($opt, $position, $x, $y,
                                              0.05, 15, 1, 1, $nrow, $ncolumn,
                                              $nlegend, \@opt_array, 1.0, 1.0, 2.0,
                                              1., \@text_colors, \@text,
                                              NULL, NULL, NULL, NULL,
                                              \@line_colors, \@line_styles, \@line_widths,
                                              \@symbol_colors, \@symbol_scales, \@symbol_numbers, \@symbols );

$position = PL_POSITION_LEFT | PL_POSITION_TOP | PL_POSITION_INSIDE;
$opt      = $opt_base;
$x        = 0.;
$y        = 0.;
$nrow     = 6;
$ncolumn  = 2;
my ($legend_width, $legend_height) = pllegend($opt, $position, $x, $y,
                                              0.05, 15, 1, 1, $nrow, $ncolumn,
                                              $nlegend, \@opt_array, 1.0, 1.0, 2.0,
                                              1., \@text_colors, \@text,
                                              NULL, NULL, NULL, NULL,
                                              \@line_colors, \@line_styles, \@line_widths,
                                              \@symbol_colors, \@symbol_scales, \@symbol_numbers, \@symbols );

$position = PL_POSITION_RIGHT | PL_POSITION_TOP | PL_POSITION_INSIDE;
$opt      = $opt_base | PL_LEGEND_ROW_MAJOR;
$x        = 0.;
$y        = 0.;
$nrow     = 6;
$ncolumn  = 2;
my ($legend_width, $legend_height) = pllegend($opt, $position, $x, $y,
                                              0.05, 15, 1, 1, $nrow, $ncolumn,
                                              $nlegend, \@opt_array, 1.0, 1.0, 2.0,
                                              1., \@text_colors, \@text,
                                              NULL, NULL, NULL, NULL,
                                              \@line_colors, \@line_styles, \@line_widths,
                                              \@symbol_colors, \@symbol_scales, \@symbol_numbers, \@symbols );

$position = PL_POSITION_BOTTOM | PL_POSITION_INSIDE;
$opt      = $opt_base | PL_LEGEND_ROW_MAJOR;
$x        = 0.;
$y        = 0.;
$nrow     = 3;
$ncolumn  = 3;
my ($legend_width, $legend_height) = pllegend($opt, $position, $x, $y,
                                              0.05, 15, 1, 1, $nrow, $ncolumn,
                                              $nlegend, \@opt_array, 1.0, 1.0, 2.0,
                                              1., \@text_colors, \@text,
                                              NULL, NULL, NULL, NULL,
                                              \@line_colors, \@line_styles, \@line_widths,
                                              \@symbol_colors, \@symbol_scales, \@symbol_numbers, \@symbols );


# Third page demonstrating legend alignment
pladv( 0 );
plvpor( 0.0, 1.0, 0.0, 0.9 );
plwind( 0.0, 1.0, 0.0, 1.0 );
plsfont( PL_FCI_SANS, -1, -1 );
plmtex( 2.0, 0.5, 0.5, "t", "Demonstrate legend alignment" );
$x        = 0.1;
$y        = 0.1;
$nturn    = 4;
$nlegend  = 0;
$position = PL_POSITION_TOP | PL_POSITION_LEFT | PL_POSITION_SUBPAGE;
$opt_base = PL_LEGEND_BACKGROUND | PL_LEGEND_BOUNDING_BOX;
$opt      = $opt_base;
for ( $i = 0; $i < 9; $i++ )
  {
    # Set up legend arrays with the correct size, type.
    if ( $i <= $nturn ) {
      $nlegend += 1;
    } else {
      $nlegend -= 1;
    }
    $nlegend = pdl( 1, $nlegend )->max;
    # nly specify legend data that are required according to the
    #  value of opt_array for that entry.
    for ( $k = 0; $k < $nlegend; $k++ )
      {
        $opt_array[$k]      = PL_LEGEND_LINE | PL_LEGEND_SYMBOL;
        $line_styles[$k]    = 1;
        $line_widths[$k]    = 1;
        $symbol_scales[$k]  = 1.;
        $symbol_numbers[$k] = 2;
        $symbols[$k]        = "*";
        $text[$k] = sprintf ("%2.2d", $k );
        $text_colors[$k]   = 1 + ( $k % 8 );
        $line_colors[$k]   = 1 + ( $k % 8 );
        $symbol_colors[$k] = 1 + ( $k % 8 );
      }
    # Use monotype fonts so that all legends are the same size.
    plsfont( PL_FCI_MONO, -1, -1 );
    plscol0a( 15, 32, 32, 32, 0.70 );

    $nrow    = pdl( 3, $nlegend )->min;
    $ncolumn = 0;

    my ($legend_width, $legend_height) = pllegend($opt, $position, $x, $y,
                                                  0.025, 15, 1, 1, $nrow, $ncolumn,
                                                  $nlegend, \@opt_array, 1.0, 1.0, 1.5,
                                                  1., \@text_colors, \@text,
                                                  NULL, NULL, NULL, NULL,
                                                  \@line_colors, \@line_styles, \@line_widths,
                                                  \@symbol_colors, \@symbol_scales, \@symbol_numbers, \@symbols );

    if ( $i == $nturn )
      {
        $position = PL_POSITION_TOP | PL_POSITION_RIGHT | PL_POSITION_SUBPAGE;
        $opt      = $opt_base;
        $x        = 1. - $x;
        $y       += $legend_height;
      }
    else
      {
        $x += $legend_width;
        $y += $legend_height;
      }
  }

# Fourth page illustrating various kinds of legends
$max_height = 0.;
$xstart     = 0.0;
$ystart     = 0.1;
$x          = $xstart;
$y          = $ystart;
$text_scale = 0.90;
pladv( 0 );
plvpor( 0.0, 1., 0.0, 0.90 );
plwind( 0.0, 1.0, 0.0, 1.0 );
plsfont( PL_FCI_SANS, -1, -1 );
plmtex( 2.0, 0.5, 0.5, "t", "Demonstrate Various Kinds of Legends" );

$nlegend = 5;

# Only specify legend data that are required according to the
# value of opt_array for that entry.
$position = PL_POSITION_LEFT | PL_POSITION_TOP;
$opt_base = PL_LEGEND_BACKGROUND | PL_LEGEND_BOUNDING_BOX | PL_LEGEND_TEXT_LEFT;

# Set up None, Box, Line, Symbol, and Line & Symbol legend entries.
$opt_array[0] = PL_LEGEND_NONE;
$text[0] = sprintf( "%s", "None" );
$text_colors[0] = 1;

$opt_array[1] = PL_LEGEND_COLOR_BOX;
$text[1] = sprintf( "%s", "Box" );
$text_colors[1]     = 2;
$box_colors[1]      = 2;
$box_patterns[1]    = 0;
$box_scales[1]      = 0.8;
$box_line_widths[1] = 1;

$opt_array[2] = PL_LEGEND_LINE;
$text[2] = sprintf(  "%s", "Line" );
$text_colors[2] = 3;
$line_colors[2] = 3;
$line_styles[2] = 1;
$line_widths[2] = 1;

$opt_array[3] = PL_LEGEND_SYMBOL;
$text[3] = sprintf(  "%s", "Symbol" );
$text_colors[3]    = 4;
$symbol_colors[3]  = 4;
$symbol_scales[3]  = $text_scale;
$symbol_numbers[3] = 4;
$symbols[3]        = $special_symbols[2];

$opt_array[4] = PL_LEGEND_SYMBOL | PL_LEGEND_LINE;
$text[4] = sprintf(  "%s", "L & S" );
$text_colors[4]    = 5;
$line_colors[4]    = 5;
$line_styles[4]    = 1;
$line_widths[4]    = 1;
$symbol_colors[4]  = 5;
$symbol_scales[4]  = $text_scale;
$symbol_numbers[4] = 4;
$symbols[4]        = $special_symbols[2];

$opt = $opt_base;
plscol0a( 15, 32, 32, 32, 0.70 );

my ($legend_width, $legend_height) = pllegend ( $opt, $position, $x, $y,
    0.1, 15, 1, 1, 0, 0,
    $nlegend, \@opt_array, 1.0, $text_scale, 2.0,
    0., \@text_colors, \@text,
    \@box_colors, \@box_patterns, \@box_scales, \@box_line_widths,
    \@line_colors, \@line_styles, \@line_widths,
    \@symbol_colors, \@symbol_scales, \@symbol_numbers, \@symbols );
    $max_height = pdl( $max_height, $legend_height )->max;

# Set up symbol legend entries with various symbols.
for ( $i = 0; $i < $nlegend; $i++ )
{
  $opt_array[$i] = PL_LEGEND_SYMBOL;
  $text[$i]     = sprintf(  "%s%s", "Symbol ", $special_symbols[$i] );
  $text_colors[$i]    = $i + 1;
  $symbol_colors[$i]  = $i + 1;
  $symbol_scales[$i]  = $text_scale;
  $symbol_numbers[$i] = 4;
  $symbols[$i]        = $special_symbols[$i];
}

$opt = $opt_base;
$x  += $legend_width;
plscol0a( 15, 32, 32, 32, 0.70 );

my ($legend_width, $legend_height) = pllegend ( $opt, $position, $x, $y,
                                                0.1, 15, 1, 1, 0, 0,
                                                $nlegend, \@opt_array, 1.0, $text_scale, 2.0,
                                                0., \@text_colors, \@text,
                                                NULL, NULL, NULL, NULL,
                                                NULL, NULL, NULL,
                                                \@symbol_colors, \@symbol_scales, \@symbol_numbers, \@symbols );
$max_height = pdl( $max_height, $legend_height )->max;

# Set up symbol legend entries with various numbers of symbols.
for ( $i = 0; $i < $nlegend; $i++ )
  {
    $opt_array[$i] = PL_LEGEND_SYMBOL;
    $text[$i]     = sprintf(  "%s %d", "Symbol Number", $i + 2 );
    $text_colors[$i]    = $i + 1;
    $symbol_colors[$i]  = $i + 1;
    $symbol_scales[$i]  = $text_scale;
    $symbol_numbers[$i] = $i + 2;
    $symbols[$i]        = $special_symbols[2];
}

$opt = $opt_base;
$x  += $legend_width;
plscol0a( 15, 32, 32, 32, 0.70 );

my ($legend_width, $legend_height) = pllegend ( $opt, $position, $x, $y,
                                                0.1, 15, 1, 1, 0, 0,
                                                $nlegend, \@opt_array, 1.0, $text_scale, 2.0,
                                                0., \@text_colors, \@text,
                                                NULL, NULL, NULL, NULL,
                                                NULL, NULL, NULL,
                                                \@symbol_colors, \@symbol_scales, \@symbol_numbers, \@symbols );
$max_height = pdl( $max_height, $legend_height )->max;

# Set up box legend entries with various colours.
for ( $i = 0; $i < $nlegend; $i++ )
  {
    $opt_array[$i] = PL_LEGEND_COLOR_BOX;
    $text[$i]     = sprintf(  "%s %d", "Box Color", $i + 1 );
    $text_colors[$i]     = $i + 1;
    $box_colors[$i]      = $i + 1;
    $box_patterns[$i]    = 0;
    $box_scales[$i]      = 0.8;
    $box_line_widths[$i] = 1;
  }

$opt = $opt_base;
# Use new origin
$x          = $xstart;
$y         += $max_height;
$max_height = 0.;
plscol0a( 15, 32, 32, 32, 0.70 );

my ($legend_width, $legend_height) = pllegend ( $opt, $position, $x, $y,
                                                0.1, 15, 1, 1, 0, 0,
                                                $nlegend, \@opt_array, 1.0, $text_scale, 2.0,
                                                0., \@text_colors, \@text,
                                                \@box_colors, \@box_patterns, \@box_scales, \@box_line_widths,
                                                NULL, NULL, NULL,
                                                NULL, NULL, NULL, NULL );
$max_height = pdl( $max_height, $legend_height )->max;

# Set up box legend entries with various patterns.
for ( $i = 0; $i < $nlegend; $i++ )
  {
    $opt_array[$i] = PL_LEGEND_COLOR_BOX;
    $text[$i]     = sprintf(  "%s %d", "Box Pattern", $i );
    $text_colors[$i]     = 2;
    $box_colors[$i]      = 2;
    $box_patterns[$i]    = $i;
    $box_scales[$i]      = 0.8;
    $box_line_widths[$i] = 1;
  }

$opt = $opt_base;
$x  += $legend_width;
plscol0a( 15, 32, 32, 32, 0.70 );

my ($legend_width, $legend_height) = pllegend ( $opt, $position, $x, $y,
                                                0.1, 15, 1, 1, 0, 0,
                                                $nlegend, \@opt_array, 1.0, $text_scale, 2.0,
                                                0., \@text_colors, \@text,
                                                \@box_colors, \@box_patterns, \@box_scales, \@box_line_widths,
                                                NULL, NULL, NULL,
                                                NULL, NULL, NULL, NULL );
$max_height = pdl( $max_height, $legend_height )->max;

# Set up box legend entries with various box pattern line widths.
for ( $i = 0; $i < $nlegend; $i++ )
  {
    $opt_array[$i] = PL_LEGEND_COLOR_BOX;
    $text[$i]     = sprintf(  "%s %d", "Box Line Width", $i + 1 );
    $text_colors[$i]     = 2;
    $box_colors[$i]      = 2;
    $box_patterns[$i]    = 3;
    $box_scales[$i]      = 0.8;
    $box_line_widths[$i] = $i + 1;
  }

$opt = $opt_base;
$x  += $legend_width;
plscol0a( 15, 32, 32, 32, 0.70 );

my ($legend_width, $legend_height) = pllegend ( $opt, $position, $x, $y,
                                                0.1, 15, 1, 1, 0, 0,
                                                $nlegend, \@opt_array, 1.0, $text_scale, 2.0,
                                                0., \@text_colors, \@text,
                                                \@box_colors, \@box_patterns, \@box_scales, \@box_line_widths,
                                                NULL, NULL, NULL,
                                                NULL, NULL, NULL, NULL );
$max_height = pdl( $max_height, $legend_height )->max;

# Set up line legend entries with various colours.
for ( $i = 0; $i < $nlegend; $i++ )
  {
    $opt_array[$i] = PL_LEGEND_LINE;
    $text[$i]     = sprintf(  "%s %d", "Line Color", $i + 1 );
    $text_colors[$i] = $i + 1;
    $line_colors[$i] = $i + 1;
    $line_styles[$i] = 1;
    $line_widths[$i] = 1;
  }

$opt = $opt_base;
# Use new origin
$x          = $xstart;
$y         += $max_height;
$max_height = 0.;
plscol0a( 15, 32, 32, 32, 0.70 );

my ($legend_width, $legend_height) = pllegend ( $opt, $position, $x, $y,
                                                0.1, 15, 1, 1, 0, 0,
                                                $nlegend, \@opt_array, 1.0, $text_scale, 2.0,
                                                0., \@text_colors, \@text,
                                                NULL, NULL, NULL, NULL,
                                                \@line_colors, \@line_styles, \@line_widths,
                                                NULL, NULL, NULL, NULL );
$max_height = pdl( $max_height, $legend_height )->max;

# Set up line legend entries with various styles.
for ( $i = 0; $i < $nlegend; $i++ )
  {
    $opt_array[$i] = PL_LEGEND_LINE;
    $text[$i]     = sprintf(  "%s %d", "Line Style", $i + 1 );
    $text_colors[$i] = 2;
    $line_colors[$i] = 2;
    $line_styles[$i] = $i + 1;
    $line_widths[$i] = 1;
  }

$opt = $opt_base;
$x  += $legend_width;
plscol0a( 15, 32, 32, 32, 0.70 );

my ($legend_width, $legend_height) = pllegend ( $opt, $position, $x, $y,
                                                0.1, 15, 1, 1, 0, 0,
                                                $nlegend, \@opt_array, 1.0, $text_scale, 2.0,
                                                0., \@text_colors, \@text,
                                                NULL, NULL, NULL, NULL,
                                                \@line_colors, \@line_styles, \@line_widths,
                                                NULL, NULL, NULL, NULL );
$max_height = pdl( $max_height, $legend_height )->max;

# Set up line legend entries with various widths.
for ( $i = 0; $i < $nlegend; $i++ )
  {
    $opt_array[$i] = PL_LEGEND_LINE;
    $text[$i]     = sprintf(  "%s %d", "Line Width", $i + 1 );
    $text_colors[$i] = 2;
    $line_colors[$i] = 2;
    $line_styles[$i] = 1;
    $line_widths[$i] = $i + 1;
  }

$opt = $opt_base;
$x  += $legend_width;
plscol0a( 15, 32, 32, 32, 0.70 );

my ($legend_width, $legend_height) = pllegend ( $opt, $position, $x, $y,
                                                0.1, 15, 1, 1, 0, 0,
                                                $nlegend, \@opt_array, 1.0, $text_scale, 2.0,
                                                0., \@text_colors, \@text,
                                                NULL, NULL, NULL, NULL,
                                                \@line_colors, \@line_styles, \@line_widths,
                                                NULL, NULL, NULL, NULL );
$max_height = pdl( $max_height, $legend_height )->max;

plend();
exit;

###    if ( colorbar )
###    {
###        # Color bar examples
###        PLFLT values_small[2]  = { 0.0, 1.0 };
###        PLFLT values_uneven[9] = { 0.0, 2.0, 2.6, 3.4, 6.0, 7.0, 8.0, 9.0, 10.0 };
###        PLFLT values_even[9]   = { 0.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0 };
###
###        # Cut out the greatest and smallest bits of the color spectrum to
###        # leave colors for the end caps.
###        plscmap1_range( 0.01, 0.99 );
###
###        # We can only test image and gradient colorbars with two element arrays
###        for ( i = 2; i < COLORBAR_KINDS; i ++ )
###        {
###            plcolorbar_example( "cmap1_blue_yellow.pal", i, 0, 0, 2, values_small );
###        }
###        # Test shade colorbars with larger arrays
###        for ( i = 0; i < 2; i++ )
###        {
###            plcolorbar_example( "cmap1_blue_yellow.pal", i, 4, 2, 9, values_even );
###        }
###        for ( i = 0; i < 2; i++ )
###        {
###            plcolorbar_example( "cmap1_blue_yellow.pal", i, 0, 0, 9, values_uneven );
###        }
###    }
###    plend();
###    exit( 0 );
###}
###
###
###  void
###plcolorbar_example_page( int kind_i, int label_i, int cap_i, PLINT cont_color, PLINT cont_width, PLINT n_values, PLFLT *values ###)
###{
###    # Parameters for the colorbars on this page
###    PLINT position_i, position, opt;
###    PLFLT x, y, length, width;
###    PLFLT ticks;
###    PLINT sub_ticks;
###    PLFLT low_cap_color, high_cap_color;
###    PLINT vertical;
###    const char *axis_opts;
###    char label[200];
###    char title[200];
###
###    length = 0.5;
###    width = 0.05;
###
###    ticks = 0.0;
###    sub_ticks = 0;
###
###    low_cap_color = 0.0;
###    high_cap_color = 1.0;
###
###    # Start a new page
###    pladv( 0 );
###
###    # Draw one colorbar relative to each side of the page
###    for ( position_i = 0; position_i < COLORBAR_POSITIONS; position_i ++ )
###    {
###        position = colorbar_position_options[position_i];
###        opt =
###            colorbar_option_kinds[kind_i] |
###            colorbar_label_options[label_i] |
###            colorbar_cap_options[cap_i];
###
###        vertical = position & PL_POSITION_LEFT || position & PL_POSITION_RIGHT;
###
###        # Set the offset position on the page
###        if ( vertical )
###        {
###            x = 0.1;
###            y = 0.25;
###        }
###        else
###        {
###            x = 0.25;
###            y = 0.1;
###        }
###
###        # Set appropriate labeling options
###        if ( opt & PL_COLORBAR_SHADE_LABEL )
###        {
###            if ( vertical )
###                axis_opts = "iv";
###            else
###                axis_opts = "i";
###        }
###        else
###        {
###            if ( sub_ticks != 0 )
###            {
###                if ( vertical )
###                    axis_opts = "stv";
###                else
###                    axis_opts = "st";
###            }
###            else
###            {
###                if ( vertical )
###                    axis_opts = "tv";
###                else
###                    axis_opts = "t";
###            }
###        }
###
###        sprintf( label, "%s, %s",
###                 colorbar_position_option_labels[position_i],
###                 colorbar_label_option_labels[label_i] );
###
###        # Smaller text
###        pl = schr( 0.0###        plschr( 0.0, 0.75 );
###        # Small ticks on the vertical axis
###        plsmaj( 0.0, 0.5 );
###        plsmin( 0.0, 0.5 );
###
###        plcolorbar( opt, position,
###                    x, y, length, width,
###                    low_cap_color, high_cap_color,
###                    cont_color, cont_width,
###                    ticks, sub_ticks,
###                    axis_opts, label,
###                    n_values, values );
###
###        # Reset  1.0 );
###        plsmaj( 0.0, 1.0 );
###        plsmin( 0.0, 1.0 );
###    }
###
###    # Draw a page title
###    sprintf( title, "%s - %s",
###             colorbar_option_kind_labels[kind_i],
###             colorbar_cap_option_labels[cap_i] );
###    plvpor( 0.0, 1.0, 0.0, 1.0 );
###    plwind( 0.0, 1.0, 0.0, 1.0 );
###    plptex( 0.5, 0.5, 0.0, 0.0, 0.5, title );
###}
###
###void
###plcolorbar_example( const char *palette, int kind_i, PLINT cont_color, PLINT cont_width, PLINT n_values, PLFLT *values )
###{
###    int label_i, cap_i;
###
###    # Load the color palette
###    plspal1( palette, 1 );
###
###    for ( label_i = 0; label_i < COLORBAR_LABELS; label_i++ )
###    {
###        for ( cap_i = 0; cap_i < COLORBAR_CAPS; cap_i++ )
###        {
###            plcolorbar_example_page( kind_i, label_i, cap_i,
###                                     cont_color, cont_width,
###                                     n_values, values );
###        }
###    }
###}
###
