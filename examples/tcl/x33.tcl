# $Id: x33c.c 11565 2011-02-13 21:06:58Z airwin $
#
# Demonstrate most pllegend capability including unicode symbols.
#
# Copyright (C) 2010 Alan Irwin
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
#
#
# This example designed just for devices (e.g., the cairo-related and
# qt-related devices) where the best choice of glyph is automatically
# selected by the related libraries (pango/cairo or Qt4) for each
# unicode character depending on what system fonts are installed.  Of
# course, you must have the appropriate TrueType fonts installed to
# have access to all the required glyphs.

set position_options [list
    [expr {$::PLPLOT::PL_POSITION_LEFT | $::PLPLOT::PL_POSITION_TOP | $::PLPLOT::PL_POSITION_OUTSIDE}]     \
    [expr {$::PLPLOT::PL_POSITION_TOP | $::PLPLOT::PL_POSITION_OUTSIDE}]                                   \
    [expr {$::PLPLOT::PL_POSITION_RIGHT | $::PLPLOT::PL_POSITION_TOP | $::PLPLOT::PL_POSITION_OUTSIDE}]    \
    [expr {$::PLPLOT::PL_POSITION_RIGHT | $::PLPLOT::PL_POSITION_OUTSIDE}]                                 \
    [expr {$::PLPLOT::PL_POSITION_RIGHT | $::PLPLOT::PL_POSITION_BOTTOM | $::PLPLOT::PL_POSITION_OUTSIDE}] \
    [expr {$::PLPLOT::PL_POSITION_BOTTOM | $::PLPLOT::PL_POSITION_OUTSIDE}]                                \
    [expr {$::PLPLOT::PL_POSITION_LEFT | $::PLPLOT::PL_POSITION_BOTTOM | $::PLPLOT::PL_POSITION_OUTSIDE}]  \
    [expr {$::PLPLOT::PL_POSITION_LEFT | $::PLPLOT::PL_POSITION_OUTSIDE}]                                  \
    [expr {$::PLPLOT::PL_POSITION_LEFT | $::PLPLOT::PL_POSITION_TOP | $::PLPLOT::PL_POSITION_INSIDE}]      \
    [expr {$::PLPLOT::PL_POSITION_TOP | $::PLPLOT::PL_POSITION_INSIDE]]                                    \
    [expr {$::PLPLOT::PL_POSITION_RIGHT | $::PLPLOT::PL_POSITION_TOP | $::PLPLOT::PL_POSITION_INSIDE}]     \
    [expr {$::PLPLOT::PL_POSITION_RIGHT | $::PLPLOT::PL_POSITION_INSIDE}]                                  \
    [expr {$::PLPLOT::PL_POSITION_RIGHT | $::PLPLOT::PL_POSITION_BOTTOM | $::PLPLOT::PL_POSITION_INSIDE}]  \
    [expr {$::PLPLOT::PL_POSITION_BOTTOM | $::PLPLOT::PL_POSITION_INSIDE}]                                 \
    [expr {$::PLPLOT::PL_POSITION_LEFT | $::PLPLOT::PL_POSITION_BOTTOM | $::PLPLOT::PL_POSITION_INSIDE}]   \
    [expr {$::PLPLOT::PL_POSITION_LEFT | $::PLPLOT::PL_POSITION_INSIDE}]                                   ]

// Pick 5 arbitrary UTF-8 symbols useful for plotting points (✠✚✱✪✽✺✰✴✦).
set special_symbols [list
    "✰" \
    "✴" \
    "✱" \
    "✽" \
    "✦" ]

set colorbar 0 ;# By default do not plot plcolorbar pages
                # for now while we are working out the API.

# TODO
#set options [list {
#    {
#        "colorbar",              // Turns on pages showing colorbars
#        NULL,
#        NULL,
#        &colorbar,
#        PL_OPT_BOOL,
#        "-colorbar",
#        "Plot the \"color bar\" pages."
#    },
#    {
#        NULL,                   // option
#        NULL,                   // handler
#        NULL,                   // client data
#        NULL,                   // address of variable to set
#        0,                      // mode flag
#        NULL,                   // short syntax
#        NULL
#    }                           // long syntax
#};

set notes { "Make sure you get it right!" NULL }

# --------------------------------------------------------------------------
# main
#
# Demonstrate most pllegend capability including unicode symbols.
# --------------------------------------------------------------------------

set MAX_NLEGEND    7

proc x33 {{w loopback}} {

    # Parse and process command line arguments
    # TODO
    # plMergeOpts options "x33c options" notes
    # void plparseopts &argc argv PL_PARSE_FULL

    # Initialize plplot
    $w cmd plinit

    # First page illustrating the 16 standard positions.
    $w cmd pladv 0
    $w cmd plvpor 0.25 0.75 0.25 0.75
    $w cmd plwind 0.0 1.0 0.0 1.0
    $w cmd plbox "bc" 0.0 0 "bc" 0.0 0
    $w cmd plsfont $::PLPLOT::PL_FCI_SANS}] -1 -1
    $w cmd plmtex "t" 8.0 0.5 0.5 "The 16 standard legend positions with"
    $w cmd plmtex "t" 6.0 0.5 0.5 "the same 0.05 offset in x and y"

    set nlegend 1
    # Only specify legend data that are required according to the
    # value of opt_array for that entry.
    set opt_base       [expr {$::PLPLOT::PL_LEGEND_BACKGROUND | $::PLPLOT::PL_LEGEND_BOUNDING_BOX}]
    set opt_array      [list [expr {$::PLPLOT::PL_LEGEND_LINE | $::PLPLOT::PL_LEGEND_SYMBOL}]]
    set line_styles    [list 1]
    set line_widths    [list 1]
    set symbol_scales  [list 1.]
    set symbol_numbers [list 4]
    set symbols        [list "*"]

    # Use monotype fonts so that all legends are the same size.
    $w cmd plsfont [expr {$::PLPLOT::PL_FCI_MONO}] -1 -1
    $w cmd plscol0a 15 32 32 32 0.70

    for {set k 0} {$k < 16} {incr k} {
        set position  [lindex $position_options $k]
        set opt       $opt_base
        sprintf text[0] "%2.2d" k
        set text_colors   [list [expr {1 +  k % 8}]]
        set line_colors   [list [expr {1 +  k % 8}]]
        set symbol_colors [list [expr {1 +  k % 8}]]

        $w cmd pllegend( &legend_width, &legend_height, position, opt, 0.05, 0.05,
            0.1, 15, 1, 1, 0, 0,
            nlegend, opt_array, 1.0, 1.0, 2.0,
            1., text_colors, (const char **) text,
            NULL, NULL, NULL, NULL,
            line_colors, line_styles, line_widths,
            symbol_colors symbol_scales symbol_numbers const char ** symbols
    }

    # Second page illustrating effect of nrow, ncolumn for the same legend
    # data.
    $w cmd pladv 0
    $w cmd plvpor 0.25 0.75 0.25 0.75
    $w cmd plwind 0.0 1.0 0.0 1.0
    $w cmd plbox "bc" 0.0 0 "bc" 0.0 0
    $w cmd plsfont [expr {$::PLPLOT::PL_FCI_SANS}] -1 -1
    $w cmd plmtex "t" 8.0 0.5 0.5 "The effect of nrow ncolumn PL_LEGEND_ROW_MAJOR"
    $w cmd plmtex "t" 6.0 0.5 0.5 "and position for the same legend data"

    set nlegend 7

    # Only specify legend data that are required according to the
    # value of opt_array for that entry.
    set opt_base       [expr {$::PLPLOT::PL_LEGEND_BACKGROUND | $::PLPLOT::PL_LEGEND_BOUNDING_BOX}]
    set opt_array      [list]
    set line_styles    [list]
    set line_widths    [list]
    set symbol_scales  [list]
    set symbol_numbers [list]
    set symbols        [list]
    set text_colors    [list]
    set line_colors    [list]
    set symbol_colors  [list]
    for {set k 0} {$k < $nlegend} {incr k} {
        lappend opt_array      [expr {$::PLPLOT::PL_LEGEND_LINE | $::PLPLOT::PL_LEGEND_SYMBOL}]
        lappend line_styles    1
        lappend line_widths    1
        lappend symbol_scales  1.
        lappend symbol_numbers 2
        lappend symbols        "*"
        lappend text           [format "%2.2d" $k]
        lappend text_colors    [expr {1 +  k % 8}]
        lappend line_colors    [expr {1 +  k % 8}]
        lappend symbol_colors  [expr {1 +  k % 8}]
    }

    # Use monotype fonts so that all legends are the same size.
    $w cmd plsfont [expr {$::PLPLOT::PL_FCI_MONO}] -1 -1
    $w cmd plscol0a 15 32 32 32 0.70

    set position [expr {$::PLPLOT::PL_POSITION_TOP | $::PLPLOT::PL_POSITION_OUTSIDE}]
    set opt      $opt_base
    set x        0.
    set y        0.1
    set nrow     1
    set ncolumn  $nlegend
    $w cmd pllegend( &legend_width, &legend_height, position, opt, x, y,
        0.05, 15, 1, 1, nrow, ncolumn,
        nlegend, opt_array, 1.0, 1.0, 2.0,
        1., text_colors, (const char **) text,
        NULL, NULL, NULL, NULL,
        line_colors, line_styles, line_widths,
        symbol_colors symbol_scales symbol_numbers const char ** symbols

    set position [expr {$::PLPLOT::PL_POSITION_BOTTOM | $::PLPLOT::PL_POSITION_OUTSIDE}]
    set opt      $opt_base
    set x        0.
    set y        0.1
    set nrow     1
    set ncolumn  $nlegend
    $w cmd pllegend( &legend_width, &legend_height, position, opt, x, y,
        0.05, 15, 1, 1, nrow, ncolumn,
        nlegend, opt_array, 1.0, 1.0, 2.0,
        1., text_colors, (const char **) text,
        NULL, NULL, NULL, NULL,
        line_colors, line_styles, line_widths,
        symbol_colors symbol_scales symbol_numbers const char ** symbols

    set position [expr {$::PLPLOT::PL_POSITION_LEFT | $::PLPLOT::PL_POSITION_OUTSIDE}]
    set opt      $opt_base
    set x        0.1
    set y        0.
    set nrow     $nlegend
    set ncolumn  1
    $w cmd pllegend( &legend_width, &legend_height, position, opt, x, y,
        0.05, 15, 1, 1, nrow, ncolumn,
        nlegend, opt_array, 1.0, 1.0, 2.0,
        1., text_colors, (const char **) text,
        NULL, NULL, NULL, NULL,
        line_colors, line_styles, line_widths,
        symbol_colors symbol_scales symbol_numbers const char ** symbols

    set position [expr {$::PLPLOT::PL_POSITION_RIGHT | $::PLPLOT::PL_POSITION_OUTSIDE}]
    set opt      $opt_base
    set x        0.1
    set y        0.
    set nrow     $nlegend
    set ncolumn  1
    $w cmd pllegend( &legend_width, &legend_height, position, opt, x, y,
        0.05, 15, 1, 1, nrow, ncolumn,
        nlegend, opt_array, 1.0, 1.0, 2.0,
        1., text_colors, (const char **) text,
        NULL, NULL, NULL, NULL,
        line_colors, line_styles, line_widths,
        symbol_colors symbol_scales symbol_numbers const char ** symbols

    set position [expr {$::PLPLOT::PL_POSITION_LEFT | $::PLPLOT::PL_POSITION_TOP | $::PLPLOT::PL_POSITION_INSIDE}]
    set opt      $opt_base
    set x        0.
    set y        0.
    set nrow     6
    set ncolumn  2
    $w cmd pllegend( &legend_width, &legend_height, position, opt, x, y,
        0.05, 15, 1, 1, nrow, ncolumn,
        nlegend, opt_array, 1.0, 1.0, 2.0,
        1., text_colors, (const char **) text,
        NULL, NULL, NULL, NULL,
        line_colors, line_styles, line_widths,
        symbol_colors symbol_scales symbol_numbers const char ** symbols

    set position [expr {$::PLPLOT::PL_POSITION_RIGHT | $::PLPLOT::PL_POSITION_TOP | $::PLPLOT::PL_POSITION_INSIDE}]
    set opt      [expr {$opt_base | $::PLPLOT::PL_LEGEND_ROW_MAJOR}]
    set x        0.
    set y        0.
    set nrow     6
    set ncolumn  2
    $w cmd pllegend( &legend_width, &legend_height, position, opt, x, y,
        0.05, 15, 1, 1, nrow, ncolumn,
        nlegend, opt_array, 1.0, 1.0, 2.0,
        1., text_colors, (const char **) text,
        NULL, NULL, NULL, NULL,
        line_colors, line_styles, line_widths,
        symbol_colors symbol_scales symbol_numbers const char ** symbols

    set position [expr {$::PLPLOT::PL_POSITION_BOTTOM | $::PLPLOT::PL_POSITION_INSIDE}]
    set opt      [expr {$opt_base | $::PLPLOT::PL_LEGEND_ROW_MAJOR}]
    set x        0.
    set y        0.
    set nrow     3
    set ncolumn  3
    $w cmd pllegend( &legend_width, &legend_height, position, opt, x, y,
        0.05, 15, 1, 1, nrow, ncolumn,
        nlegend, opt_array, 1.0, 1.0, 2.0,
        1., text_colors, (const char **) text,
        NULL, NULL, NULL, NULL,
        line_colors, line_styles, line_widths,
        symbol_colors symbol_scales symbol_numbers const char ** symbols

    # Third page demonstrating legend alignment
    $w cmd pladv 0
    $w cmd plvpor 0.0 1.0 0.0 0.9
    $w cmd plwind 0.0 1.0 0.0 1.0
    $w cmd plsfont [expr {$::PLPLOT::PL_FCI_SANS}] -1 -1
    $w cmd plmtex "t" 2.0 0.5 0.5 "Demonstrate legend alignment"

    set x        0.1
    set y        0.1
    set nturn    4
    set nlegend  0
    set position [expr {$::PLPLOT::PL_POSITION_TOP | $::PLPLOT::PL_POSITION_LEFT | $::PLPLOT::PL_POSITION_SUBPAGE}]
    set opt_base [expr {$::PLPLOT::PL_LEGEND_BACKGROUND | $::PLPLOT::PL_LEGEND_BOUNDING_BOX}]
    set opt      $opt_base
    for {set i 0} {$i < 9} {incr i} {
    {
        # Set up legend arrays with the correct size, type.
        if { $i <= $nturn } {
            incr nlegend 1
        } else {
            incr nlegend -1
        }
        set nlegend [max 1 nlegend]
        # nly specify legend data that are required according to the
        # value of opt_array for that entry.
        set opt_array      [list]
        set line_styles    [list]
        set line_widths    [list]
        set symbol_scales  [list]
        set symbol_numbers [list]
        set symbols        [list]
        set text_colors    [list]
        set line_colors    [list]
        set symbol_colors  [list]
        for {set k 0} {$k < $nlegend} {incr k} {
            lappend opt_array      [expr {$::PLPLOT::PL_LEGEND_LINE | $::PLPLOT::PL_LEGEND_SYMBOL}]
            lappend line_styles    1
            lappend line_widths    1
            lappend symbol_scales  1.
            lappend symbol_numbers 2
            lappend symbols        "*"
            lappend text           [format "%2.2d" $k]
            lappend text_colors    [expr {1 +  k % 8}]
            lappend line_colors    [expr {1 +  k % 8}]
            lappend symbol_colors  [expr {1 +  k % 8}]
        }
        # Use monotype fonts so that all legends are the same size.
        $w cmd plsfont [expr {$::PLPLOT::PL_FCI_MONO}] -1 -1
        $w cmd plscol0a 15 32 32 32 0.70

        set nrow    [min 3 nlegend]
        set ncolumn 0

        $w cmd pllegend( &legend_width, &legend_height, position, opt, x, y,
            0.025, 15, 1, 1, nrow, ncolumn,
            nlegend, opt_array, 1.0, 1.0, 1.5,
            1., text_colors, (const char **) text,
            NULL, NULL, NULL, NULL,
            line_colors, line_styles, line_widths,
            symbol_colors symbol_scales symbol_numbers const char ** symbols

        if { $i == $nturn } {
            set position [expr {$::PLPLOT::PL_POSITION_TOP | $::PLPLOT::PL_POSITION_RIGHT | $::PLPLOT::PL_POSITION_SUBPAGE}]
            set opt $opt_base
            set x   [expr {1. - $x}]
            set y   [expr {$y + $legend_height}]
        } else {
            set x   [expr {$x + $legend_width}]
            set y   [expr {$y + $legend_height}]
        }
    }

    # Fourth page illustrating various kinds of legends
    set max_height 0.
    set xstart     0.0
    set ystart     0.1
    set x          $xstart
    set y          $ystart
    set text_scale 0.90
    $w cmd pladv 0
    $w cmd plvpor 0.0 1. 0.0 0.90
    $w cmd plwind 0.0 1.0 0.0 1.0
    # $w cmd plbox"bc" 0.0 0 "bc" 0.0 0
    $w cmd plsfont [expr {$::PLPLOT::PL_FCI_SANS}] -1 -1
    $w cmd plmtex "t" 2.0 0.5 0.5 "Demonstrate Various Kinds of Legends"

    set nlegend 5
    # Only specify legend data that are required according to the
    # value of opt_array for that entry.
    set position = [expr {$::PLPLOT::PL_POSITION_LEFT | $::PLPLOT::PL_POSITION_TOP}]
    set opt_base = [expr {$::PLPLOT::PL_LEGEND_BACKGROUND | $::PLPLOT::PL_LEGEND_BOUNDING_BOX | $::PLPLOT::PL_LEGEND_TEXT_LEFT}]

    # Set up None, Box, Line, Symbol, and Line & Symbol legend entries.
    set opt_array      [list]
    set line_styles    [list]
    set line_widths    [list]
    set symbol_scales  [list]
    set symbol_numbers [list]
    set symbols        [list]
    set text_colors    [list]
    set line_colors    [list]
    set symbol_colors  [list]

    set opt_array   [list [expr {$::PLPLOT::PL_LEGEND_NONE}]]
    set text        [list "None"]
    set text_colors [list 1]

    lappend opt_array       [expr {$::PLPLOT::PL_LEGEND_COLOR_BOX}]
    lappend text            "Box"
    lappend text_colors     2
    lappend box_colors      2
    lappend box_patterns    0
    lappend box_scales      0.8
    lappend box_line_widths 1

    lappend opt_array       [expr {$::PLPLOT::PL_LEGEND_LINE}]
    lappend text            "Line"
    lappend text_colors     3
    lappend line_colors     3
    lappend line_styles     1
    lappend line_widths     1

    lappend opt_array       [expr {$::PLPLOT::PL_LEGEND_SYMBOL}]
    lappend text            "Symbol"
    lappend text_colors     4
    lappend symbol_colors   4
    lappend symbol_scales   $text_scale
    lappend symbol_numbers  4
    lappend symbols         [lindex $special_symbols 2]

    lappend opt_array       [expr {$::PLPLOT::PL_LEGEND_SYMBOL | $::PLPLOT::PL_LEGEND_LINE}]
    lappend text            "L & S"
    lappend text_colors     5
    lappend line_colors     5
    lappend line_styles     1
    lappend line_widths     1
    lappend symbol_colors   5
    lappend symbol_scales   $text_scale
    lappend symbol_numbers  4
    lappend symbols         [lindex $special_symbols 2]

    set opt $opt_base
    $w cmd plscol0a 15 32 32 32 0.70

    $w cmd pllegend( &legend_width, &legend_height, position, opt, x, y,
        0.1, 15, 1, 1, 0, 0,
        nlegend, opt_array, 1.0, text_scale, 2.0,
        0., text_colors, (const char **) text,
        box_colors, box_patterns, box_scales, box_line_widths,
        line_colors, line_styles, line_widths,
        symbol_colors symbol_scales symbol_numbers const char ** symbols
    set max_height = MAX max_height legend_height

    # Set up symbol legend entries with various symbols.
    set opt_array      [list]
    set line_styles    [list]
    set line_widths    [list]
    set symbol_scales  [list]
    set symbol_numbers [list]
    set symbols        [list]
    set text_colors    [list]
    set line_colors    [list]
    set symbol_colors  [list]
    for { set i 0} {$i < $nlegend} {incr i} {
        lappend opt_array      [expr {$::PLPLOT::PL_LEGEND_SYMBOL}]
        lappend text           "Symbol [lindex $special_symbols $i]
        lappend text_colors    [expr {$i + 1}]
        lappend symbol_colors  [expr {$i + 1}]
        lappend symbol_scales  $text_scale
        lappend symbol_numbers 4
        lappend symbols        [lindex $special_symbols $i]
    }

    set opt $opt_base
    set x   [expr $x + $legend_width}]
    $w cmd plscol0a 15 32 32 32 0.70

    $w cmd pllegend( &legend_width, &legend_height, position, opt, x, y,
        0.1, 15, 1, 1, 0, 0,
        nlegend, opt_array, 1.0, text_scale, 2.0,
        0., text_colors, (const char **) text,
        NULL, NULL, NULL, NULL,
        NULL, NULL, NULL,
        symbol_colors symbol_scales symbol_numbers const char ** symbols
    set max_height [max $max_height $legend_height]

    # Set up symbol legend entries with various numbers of symbols.
    set opt_array      [list]
    set line_styles    [list]
    set line_widths    [list]
    set symbol_scales  [list]
    set symbol_numbers [list]
    set symbols        [list]
    set text_colors    [list]
    set line_colors    [list]
    set symbol_colors  [list]
    for {set i 0} {$i < $nlegend} {incr i} {
        lappend set opt_array[i] = [expr {$::PLPLOT::PL_LEGEND_SYMBOL}]
        lappend sprintf text[i] "%s %d" "Symbol Number" i + 2
        lappend set text_colors[i]    = i + 1
        lappend set symbol_colors[i]  = i + 1
        lappend set symbol_scales[i]  = text_scale
        lappend set symbol_numbers[i] = i + 2
        lappend set symbols[i]        = special_symbols[2]
    }

    set opt $opt_base
    set x   [expr {$x + $legend_width}]
    $w cmd plscol0a 15 32 32 32 0.70

    $w cmd pllegend( &legend_width, &legend_height, position, opt, x, y,
        0.1, 15, 1, 1, 0, 0,
        nlegend, opt_array, 1.0, text_scale, 2.0,
        0., text_colors, (const char **) text,
        NULL, NULL, NULL, NULL,
        NULL, NULL, NULL,
        symbol_colors symbol_scales symbol_numbers const char ** symbols
    set max_height [max $max_height $legend_height]

    # Set up box legend entries with various colours.
    set opt_array      [list]
    set line_styles    [list]
    set line_widths    [list]
    set symbol_scales  [list]
    set symbol_numbers [list]
    set symbols        [list]
    set text_colors    [list]
    set line_colors    [list]
    set symbol_colors  [list]
    for  i = 0 i < nlegend; i++
    {
        lappend set opt_array[i] = [expr {$::PLPLOT::PL_LEGEND_COLOR_BOX}]
        lappend sprintf text[i] "%s %d" "Box Color" i + 1
        lappend set text_colors[i]     = i + 1
        lappend set box_colors[i]      = i + 1
        lappend set box_patterns[i]    = 0
        lappend set box_scales[i]      = 0.8
        lappend set box_line_widths[i] = 1
    }

    set opt $opt_base
    # Use new origin
    set x          $xstart
    set y          [expr {$y + $max_height}]
    set max_height 0.
    $w cmd plscol0a 15 32 32 32 0.70

    $w cmd pllegend( &legend_width, &legend_height, position, opt, x, y,
        0.1, 15, 1, 1, 0, 0,
        nlegend, opt_array, 1.0, text_scale, 2.0,
        0., text_colors, (const char **) text,
        box_colors, box_patterns, box_scales, box_line_widths,
        NULL, NULL, NULL,
        NULL NULL NULL NULL
    set max_height [max $max_height $legend_height]

    # Set up box legend entries with various patterns.
    set opt_array      [list]
    set line_styles    [list]
    set line_widths    [list]
    set symbol_scales  [list]
    set symbol_numbers [list]
    set symbols        [list]
    set text_colors    [list]
    set line_colors    [list]
    set symbol_colors  [list]
    for  i = 0 i < nlegend; i++
    {
        lappend set opt_array[i] = [expr {$::PLPLOT::PL_LEGEND_COLOR_BOX}]
        lappend sprintf text[i] "%s %d" "Box Pattern" i
        lappend set text_colors[i]     = 2
        lappend set box_colors[i]      = 2
        lappend set box_patterns[i]    = i
        lappend set box_scales[i]      = 0.8
        lappend set box_line_widths[i] = 1
    }

    set opt $opt_base
    set x   [expr {$x + $legend_width}]
    $w cmd plscol0a 15 32 32 32 0.70

    $w cmd pllegend( &legend_width, &legend_height, position, opt, x, y,
        0.1, 15, 1, 1, 0, 0,
        nlegend, opt_array, 1.0, text_scale, 2.0,
        0., text_colors, (const char **) text,
        box_colors, box_patterns, box_scales, box_line_widths,
        NULL, NULL, NULL,
        NULL NULL NULL NULL
    set max_height = MAX max_height legend_height

    # Set up box legend entries with various box pattern line widths.
    set opt_array      [list]
    set line_styles    [list]
    set line_widths    [list]
    set symbol_scales  [list]
    set symbol_numbers [list]
    set symbols        [list]
    set text_colors    [list]
    set line_colors    [list]
    set symbol_colors  [list]
    for  i = 0 i < nlegend; i++
    {
        lappend set opt_array[i] = [expr {$::PLPLOT::PL_LEGEND_COLOR_BOX}]
        lappend sprintf text[i] "%s %d" "Box Line Width" i + 1
        lappend set text_colors[i]     = 2
        lappend set box_colors[i]      = 2
        lappend set box_patterns[i]    = 3
        lappend set box_scales[i]      = 0.8
        lappend set box_line_widths[i] = i + 1
    }

    set opt $opt_base
    set x   [expr {$x + $legend_width}]
    $w cmd plscol0a 15 32 32 32 0.70

    $w cmd pllegend( &legend_width, &legend_height, position, opt, x, y,
        0.1, 15, 1, 1, 0, 0,
        nlegend, opt_array, 1.0, text_scale, 2.0,
        0., text_colors, (const char **) text,
        box_colors, box_patterns, box_scales, box_line_widths,
        NULL, NULL, NULL,
        NULL NULL NULL NULL
    set max_height [max max_height legend_height]

    # Set up line legend entries with various colours.
    set opt_array      [list]
    set line_styles    [list]
    set line_widths    [list]
    set symbol_scales  [list]
    set symbol_numbers [list]
    set symbols        [list]
    set text_colors    [list]
    set line_colors    [list]
    set symbol_colors  [list]
    for  i = 0 i < nlegend; i++
    {
        lappend set opt_array[i] = [expr {$::PLPLOT::PL_LEGEND_LINE}]
        lappend sprintf text[i] "%s %d" "Line Color" i + 1
        lappend set text_colors[i] = i + 1
        lappend set line_colors[i] = i + 1
        lappend set line_styles[i] = 1
        lappend set line_widths[i] = 1
    }

    set opt $opt_base
    # Use new origin
    set x          $xstart
    set y          [expr {$x + $max_height}]
    set max_height 0.
    $w cmd plscol0a 15 32 32 32 0.70

    $w cmd pllegend( &legend_width, &legend_height, position, opt, x, y,
        0.1, 15, 1, 1, 0, 0,
        nlegend, opt_array, 1.0, text_scale, 2.0,
        0., text_colors, (const char **) text,
        NULL, NULL, NULL, NULL,
        line_colors, line_styles, line_widths,
        NULL NULL NULL NULL
    set max_height = MAX max_height legend_height

    # Set up line legend entries with various styles.
    set opt_array      [list]
    set line_styles    [list]
    set line_widths    [list]
    set symbol_scales  [list]
    set symbol_numbers [list]
    set symbols        [list]
    set text_colors    [list]
    set line_colors    [list]
    set symbol_colors  [list]
    for  i = 0 i < nlegend; i++
    {
        lappend set opt_array[i] = [expr {$::PLPLOT::PL_LEGEND_LINE}]
        lappend sprintf text[i] "%s %d" "Line Style" i + 1
        lappend set text_colors[i] = 2
        lappend set line_colors[i] = 2
        lappend set line_styles[i] = i + 1
        lappend set line_widths[i] = 1
    }

    set opt $opt_base
    set x   [expr {$x + $legend_width}]
    $w cmd plscol0a 15 32 32 32 0.70

    $w cmd pllegend( &legend_width, &legend_height, position, opt, x, y,
        0.1, 15, 1, 1, 0, 0,
        nlegend, opt_array, 1.0, text_scale, 2.0,
        0., text_colors, (const char **) text,
        NULL, NULL, NULL, NULL,
        line_colors, line_styles, line_widths,
        NULL NULL NULL NULL
    set max_height [max $max_height $legend_height]

    # Set up line legend entries with various widths.
    set opt_array      [list]
    set line_styles    [list]
    set line_widths    [list]
    set symbol_scales  [list]
    set symbol_numbers [list]
    set symbols        [list]
    set text_colors    [list]
    set line_colors    [list]
    set symbol_colors  [list]
    for  i = 0 i < nlegend; i++
    {
        lappend set opt_array[i] = [expr {$::PLPLOT::PL_LEGEND_LINE}]
        lappend sprintf text[i] "%s %d" "Line Width" i + 1
        lappend set text_colors[i] = 2
        lappend set line_colors[i] = 2
        lappend set line_styles[i] = 1
        lappend set line_widths[i] = i + 1
    }

    set opt $opt_base
    set x   [expr {$x + $legend_width}]
    $w cmd plscol0a 15 32 32 32 0.70

    $w cmd pllegend( &legend_width, &legend_height, position, opt, x, y,
        0.1, 15, 1, 1, 0, 0,
        nlegend, opt_array, 1.0, text_scale, 2.0,
        0., text_colors, (const char **) text,
        NULL, NULL, NULL, NULL,
        line_colors, line_styles, line_widths,
        NULL NULL NULL NULL
    set max_height [max $max_height $legend_height]
}
