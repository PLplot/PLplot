# $Id$
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

# --------------------------------------------------------------------------
# main
#
# Demonstrate most pllegend capability including unicode symbols.
# --------------------------------------------------------------------------

set MAX_NLEGEND    7

proc x33 {{w loopback}} {

    set position_options [list \
        [expr {$::PLPLOT::PL_POSITION_LEFT | $::PLPLOT::PL_POSITION_TOP | $::PLPLOT::PL_POSITION_OUTSIDE}]     \
        [expr {$::PLPLOT::PL_POSITION_TOP | $::PLPLOT::PL_POSITION_OUTSIDE}]                                   \
        [expr {$::PLPLOT::PL_POSITION_RIGHT | $::PLPLOT::PL_POSITION_TOP | $::PLPLOT::PL_POSITION_OUTSIDE}]    \
        [expr {$::PLPLOT::PL_POSITION_RIGHT | $::PLPLOT::PL_POSITION_OUTSIDE}]                                 \
        [expr {$::PLPLOT::PL_POSITION_RIGHT | $::PLPLOT::PL_POSITION_BOTTOM | $::PLPLOT::PL_POSITION_OUTSIDE}] \
        [expr {$::PLPLOT::PL_POSITION_BOTTOM | $::PLPLOT::PL_POSITION_OUTSIDE}]                                \
        [expr {$::PLPLOT::PL_POSITION_LEFT | $::PLPLOT::PL_POSITION_BOTTOM | $::PLPLOT::PL_POSITION_OUTSIDE}]  \
        [expr {$::PLPLOT::PL_POSITION_LEFT | $::PLPLOT::PL_POSITION_OUTSIDE}]                                  \
        [expr {$::PLPLOT::PL_POSITION_LEFT | $::PLPLOT::PL_POSITION_TOP | $::PLPLOT::PL_POSITION_INSIDE}]      \
        [expr {$::PLPLOT::PL_POSITION_TOP | $::PLPLOT::PL_POSITION_INSIDE}]                                    \
        [expr {$::PLPLOT::PL_POSITION_RIGHT | $::PLPLOT::PL_POSITION_TOP | $::PLPLOT::PL_POSITION_INSIDE}]     \
        [expr {$::PLPLOT::PL_POSITION_RIGHT | $::PLPLOT::PL_POSITION_INSIDE}]                                  \
        [expr {$::PLPLOT::PL_POSITION_RIGHT | $::PLPLOT::PL_POSITION_BOTTOM | $::PLPLOT::PL_POSITION_INSIDE}]  \
        [expr {$::PLPLOT::PL_POSITION_BOTTOM | $::PLPLOT::PL_POSITION_INSIDE}]                                 \
        [expr {$::PLPLOT::PL_POSITION_LEFT | $::PLPLOT::PL_POSITION_BOTTOM | $::PLPLOT::PL_POSITION_INSIDE}]   \
        [expr {$::PLPLOT::PL_POSITION_LEFT | $::PLPLOT::PL_POSITION_INSIDE}]                                   ]

    # Pick 5 arbitrary UTF-8 symbols useful for plotting points (✠✚✱✪✽✺✰✴✦).
    set special_symbols [list \
        "✰" \
        "✴" \
        "✱" \
        "✽" \
        "✦" ]

    set colorbar 0 ;# By default do not plot plcolorbar pages
                    # for now while we are working out the API.

    set notes { "Make sure you get it right!" NULL }

    # Parse and process command line arguments
    # TODO
    # plMergeOpts options "x33c options" notes
    # void plparseopts &argc argv PL_PARSE_FULL

    # First page illustrating the 16 standard positions.
    $w cmd pladv 0
    $w cmd plvpor 0.25 0.75 0.25 0.75
    $w cmd plwind 0.0 1.0 0.0 1.0
    $w cmd plbox "bc" 0.0 0 "bc" 0.0 0
    $w cmd plsfont [expr {$::PLPLOT::PL_FCI_SANS}] -1 -1
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
        set position      [lindex $position_options $k]
        set opt           $opt_base
        set text          [list [format "%2.2d" $k]]
        set text_colors   [list [expr {1 +  $k % 8}]]
        set line_colors   [list [expr {1 +  $k % 8}]]
        set symbol_colors [list [expr {1 +  $k % 8}]]

        $w cmd pllegend $position $opt 0.05 0.05 \
            0.1 15 1 1 0 0 \
            $opt_array 1.0 1.0 2.0 \
            1. $text_colors $text \
            {} {} {} {} \
            $line_colors $line_styles $line_widths \
            $symbol_colors $symbol_scales $symbol_numbers $symbols
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
    set text           [list]
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
        lappend text_colors    [expr {1 +  $k % 8}]
        lappend line_colors    [expr {1 +  $k % 8}]
        lappend symbol_colors  [expr {1 +  $k % 8}]
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
    $w cmd pllegend $position $opt $x $y \
        0.05 15 1 1 $nrow $ncolumn \
        $opt_array 1.0 1.0 2.0 \
        1. $text_colors $text \
        {} {} {} {} \
        $line_colors $line_styles $line_widths \
        $symbol_colors $symbol_scales $symbol_numbers $symbols

    set position [expr {$::PLPLOT::PL_POSITION_BOTTOM | $::PLPLOT::PL_POSITION_OUTSIDE}]
    set opt      $opt_base
    set x        0.
    set y        0.1
    set nrow     1
    set ncolumn  $nlegend
    $w cmd pllegend $position $opt $x $y \
        0.05 15 1 1 $nrow $ncolumn \
        $opt_array 1.0 1.0 2.0 \
        1. $text_colors $text \
        {} {} {} {} \
        $line_colors $line_styles $line_widths \
        $symbol_colors $symbol_scales $symbol_numbers $symbols

    set position [expr {$::PLPLOT::PL_POSITION_LEFT | $::PLPLOT::PL_POSITION_OUTSIDE}]
    set opt      $opt_base
    set x        0.1
    set y        0.
    set nrow     $nlegend
    set ncolumn  1
    $w cmd pllegend $position $opt $x $y \
        0.05 15 1 1 $nrow $ncolumn \
        $opt_array 1.0 1.0 2.0 \
        1. $text_colors $text \
        {} {} {} {} \
        $line_colors $line_styles $line_widths \
        $symbol_colors $symbol_scales $symbol_numbers $symbols

    set position [expr {$::PLPLOT::PL_POSITION_RIGHT | $::PLPLOT::PL_POSITION_OUTSIDE}]
    set opt      $opt_base
    set x        0.1
    set y        0.
    set nrow     $nlegend
    set ncolumn  1
    $w cmd pllegend $position $opt $x $y \
        0.05 15 1 1 $nrow $ncolumn \
        $opt_array 1.0 1.0 2.0 \
        1. $text_colors $text \
        {} {} {} {} \
        $line_colors $line_styles $line_widths \
        $symbol_colors $symbol_scales $symbol_numbers $symbols

    set position [expr {$::PLPLOT::PL_POSITION_LEFT | $::PLPLOT::PL_POSITION_TOP | $::PLPLOT::PL_POSITION_INSIDE}]
    set opt      $opt_base
    set x        0.
    set y        0.
    set nrow     6
    set ncolumn  2
    $w cmd pllegend $position $opt $x $y \
        0.05 15 1 1 $nrow $ncolumn \
        $opt_array 1.0 1.0 2.0 \
        1. $text_colors $text \
        {} {} {} {} \
        $line_colors $line_styles $line_widths \
        $symbol_colors $symbol_scales $symbol_numbers $symbols

    set position [expr {$::PLPLOT::PL_POSITION_RIGHT | $::PLPLOT::PL_POSITION_TOP | $::PLPLOT::PL_POSITION_INSIDE}]
    set opt      [expr {$opt_base | $::PLPLOT::PL_LEGEND_ROW_MAJOR}]
    set x        0.
    set y        0.
    set nrow     6
    set ncolumn  2
    $w cmd pllegend $position $opt $x $y \
        0.05 15 1 1 $nrow $ncolumn \
        $opt_array 1.0 1.0 2.0 \
        1. $text_colors $text \
        {} {} {} {} \
        $line_colors $line_styles $line_widths \
        $symbol_colors $symbol_scales $symbol_numbers $symbols

    set position [expr {$::PLPLOT::PL_POSITION_BOTTOM | $::PLPLOT::PL_POSITION_INSIDE}]
    set opt      [expr {$opt_base | $::PLPLOT::PL_LEGEND_ROW_MAJOR}]
    set x        0.
    set y        0.
    set nrow     3
    set ncolumn  3
    $w cmd pllegend $position $opt $x $y \
        0.05 15 1 1 $nrow $ncolumn \
        $opt_array 1.0 1.0 2.0 \
        1. $text_colors $text \
        {} {} {} {} \
        $line_colors $line_styles $line_widths \
        $symbol_colors $symbol_scales $symbol_numbers $symbols

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
        # Set up legend arrays with the correct size, type.
        if { $i <= $nturn } {
            incr nlegend 1
        } else {
            incr nlegend -1
        }
        set nlegend [max 1 $nlegend]
        # nly specify legend data that are required according to the
        # value of opt_array for that entry.
        set opt_array      [list]
        set line_styles    [list]
        set line_widths    [list]
        set symbol_scales  [list]
        set symbol_numbers [list]
        set symbols        [list]
        set text           [list]
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
            lappend text_colors    [expr {1 + $k % 8}]
            lappend line_colors    [expr {1 + $k % 8}]
            lappend symbol_colors  [expr {1 + $k % 8}]
        }
        # Use monotype fonts so that all legends are the same size.
        $w cmd plsfont [expr {$::PLPLOT::PL_FCI_MONO}] -1 -1
        $w cmd plscol0a 15 32 32 32 0.70

        set nrow    [min 3 $nlegend]
        set ncolumn 0

        set legend_data [$w cmd pllegend $position $opt $x $y \
            0.025 15 1 1 $nrow $ncolumn \
            $opt_array 1.0 1.0 1.5 \
            1. $text_colors $text \
            {} {} {} {} \
            $line_colors $line_styles $line_widths \
            $symbol_colors $symbol_scales $symbol_numbers $symbols]
        foreach {legend_width legend_height} $legend_data {break}

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
    set position [expr {$::PLPLOT::PL_POSITION_LEFT | $::PLPLOT::PL_POSITION_TOP}]
    set opt_base [expr {$::PLPLOT::PL_LEGEND_BACKGROUND | $::PLPLOT::PL_LEGEND_BOUNDING_BOX | $::PLPLOT::PL_LEGEND_TEXT_LEFT}]

    # Set up None, Box, Line, Symbol, and Line & Symbol legend entries.

    set opt_array       [list [expr {$::PLPLOT::PL_LEGEND_NONE}]]
    set text            [list "None"]
    set text_colors     [list 1]
    set line_colors     [list {}]
    set line_styles     [list {}]
    set line_widths     [list {}]
    set box_colors      [list {}]
    set box_patterns    [list {}]
    set box_scales      [list {}]
    set box_line_widths [list {}]
    set symbol_scales   [list {}]
    set symbol_numbers  [list {}]
    set symbols         [list {}]
    set symbol_colors   [list {}]

    lappend opt_array       [expr {$::PLPLOT::PL_LEGEND_COLOR_BOX}]
    lappend text            "Box"
    lappend text_colors     2
    lappend line_colors     {}
    lappend line_styles     {}
    lappend line_widths     {}
    lappend box_colors      2
    lappend box_patterns    0
    lappend box_scales      0.8
    lappend box_line_widths 1
    lappend symbol_scales   {}
    lappend symbol_numbers  {}
    lappend symbols         {}
    lappend symbol_colors   {}

    lappend opt_array       [expr {$::PLPLOT::PL_LEGEND_LINE}]
    lappend text            "Line"
    lappend text_colors     3
    lappend line_colors     3
    lappend line_styles     1
    lappend line_widths     1
    lappend box_colors      {}
    lappend box_patterns    {}
    lappend box_scales      {}
    lappend box_line_widths {}
    lappend symbol_scales   {}
    lappend symbol_numbers  {}
    lappend symbols         {}
    lappend symbol_colors   {}

    lappend opt_array       [expr {$::PLPLOT::PL_LEGEND_SYMBOL}]
    lappend text            "Symbol"
    lappend text_colors     4
    lappend line_colors     {}
    lappend line_styles     {}
    lappend line_widths     {}
    lappend box_colors      {}
    lappend box_patterns    {}
    lappend box_scales      {}
    lappend box_line_widths {}
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
    lappend box_colors      {}
    lappend box_patterns    {}
    lappend box_scales      {}
    lappend box_line_widths {}
    lappend symbol_colors   5
    lappend symbol_scales   $text_scale
    lappend symbol_numbers  4
    lappend symbols         [lindex $special_symbols 2]

    set opt $opt_base
    $w cmd plscol0a 15 32 32 32 0.70

    set legend_data [$w cmd pllegend $position $opt $x $y \
        0.1 15 1 1 0 0 \
        $opt_array 1.0 $text_scale 2.0 \
        0. $text_colors $text \
        $box_colors $box_patterns $box_scales $box_line_widths \
        $line_colors $line_styles $line_widths \
        $symbol_colors $symbol_scales $symbol_numbers $symbols]

    foreach {legend_width legend_height} $legend_data {break}
    set max_height [max $max_height $legend_height]

    # Set up symbol legend entries with various symbols.
    set opt_array       [list]
    set line_styles     [list]
    set line_widths     [list]
    set symbol_scales   [list]
    set symbol_numbers  [list]
    set symbols         [list]
    set text            [list]
    set text_colors     [list]
    set line_colors     [list]
    set symbol_colors   [list]
    set box_colors      [list]
    set box_patterns    [list]
    set box_scales      [list]
    set box_line_widths [list]
    for { set i 0} {$i < $nlegend} {incr i} {
        lappend opt_array      [expr {$::PLPLOT::PL_LEGEND_SYMBOL}]
        lappend text           "Symbol [lindex $special_symbols $i]"
        lappend text_colors    [expr {$i + 1}]
        lappend symbol_colors  [expr {$i + 1}]
        lappend symbol_scales  $text_scale
        lappend symbol_numbers 4
        lappend symbols        [lindex $special_symbols $i]
    }

    set opt $opt_base
    set x   [expr {$x + $legend_width}]
    $w cmd plscol0a 15 32 32 32 0.70

    set legend_data [$w cmd pllegend $position $opt $x $y \
        0.1 15 1 1 0 0 \
        $opt_array 1.0 $text_scale 2.0 \
        0. $text_colors $text \
        {} {} {} {} \
        {} {} {} \
        $symbol_colors $symbol_scales $symbol_numbers $symbols]
    foreach {legend_width legend_height} $legend_data {break}
    set max_height [max $max_height $legend_height]

    # Set up symbol legend entries with various numbers of symbols.
    set opt_array       [list]
    set line_styles     [list]
    set line_widths     [list]
    set symbol_scales   [list]
    set symbol_numbers  [list]
    set symbols         [list]
    set text            [list]
    set text_colors     [list]
    set line_colors     [list]
    set symbol_colors   [list]
    set box_colors      [list]
    set box_patterns    [list]
    set box_scales      [list]
    set box_line_widths [list]
    for {set i 0} {$i < $nlegend} {incr i} {
        lappend opt_array      [expr {$::PLPLOT::PL_LEGEND_SYMBOL}]
        lappend text           [format "%s %d" "Symbol Number" [expr {$i + 2}]]
        lappend text_colors    [expr {$i + 1}]
        lappend symbol_colors  [expr {$i + 1}]
        lappend symbol_scales  $text_scale
        lappend symbol_numbers [expr {$i + 2}]
        lappend symbols        [lindex $special_symbols 2]
    }

    set opt $opt_base
    set x   [expr {$x + $legend_width}]
    $w cmd plscol0a 15 32 32 32 0.70

    set legend_data [$w cmd pllegend $position $opt $x $y \
        0.1 15 1 1 0 0 \
        $opt_array 1.0 $text_scale 2.0 \
        0. $text_colors $text \
        {} {} {} {} \
        {} {} {} \
        $symbol_colors $symbol_scales $symbol_numbers $symbols]
    foreach {legend_width legend_height} $legend_data {break}
    set max_height [max $max_height $legend_height]

    # Set up box legend entries with various colours.
    set opt_array       [list]
    set line_styles     [list]
    set line_widths     [list]
    set symbol_scales   [list]
    set symbol_numbers  [list]
    set symbols         [list]
    set text            [list]
    set text_colors     [list]
    set line_colors     [list]
    set symbol_colors   [list]
    set box_colors      [list]
    set box_patterns    [list]
    set box_scales      [list]
    set box_line_widths [list]
    for {set i 0} {$i < $nlegend} {incr i} {
        lappend opt_array       [expr {$::PLPLOT::PL_LEGEND_COLOR_BOX}]
        lappend text            [format "%s %d" "Box Color" [expr {$i + 1}]]
        lappend text_colors     [expr {$i + 1}]
        lappend box_colors      [expr {$i + 1}]
        lappend box_patterns    0
        lappend box_scales      0.8
        lappend box_line_widths 1
    }

    set opt $opt_base
    # Use new origin
    set x          $xstart
    set y          [expr {$y + $max_height}]
    set max_height 0.
    $w cmd plscol0a 15 32 32 32 0.70

    set legend_data [$w cmd pllegend $position $opt $x $y \
        0.1 15 1 1 0 0 \
        $opt_array 1.0 $text_scale 2.0 \
        0. $text_colors $text \
        $box_colors $box_patterns $box_scales $box_line_widths \
        {} {} {} \
        {} {} {} {}]
    foreach {legend_width legend_height} $legend_data {break}
    set max_height [max $max_height $legend_height]

    # Set up box legend entries with various patterns.
    set opt_array       [list]
    set line_styles     [list]
    set line_widths     [list]
    set symbol_scales   [list]
    set symbol_numbers  [list]
    set symbols         [list]
    set text            [list]
    set text_colors     [list]
    set line_colors     [list]
    set symbol_colors   [list]
    set box_colors      [list]
    set box_patterns    [list]
    set box_scales      [list]
    set box_line_widths [list]
    for {set i 0} {$i < $nlegend} {incr i} {
        lappend opt_array       [expr {$::PLPLOT::PL_LEGEND_COLOR_BOX}]
        lappend text            [format "%s %d" "Box Pattern" $i]
        lappend text_colors     2
        lappend box_colors      2
        lappend box_patterns    i
        lappend box_scales      0.8
        lappend box_line_widths 1
    }

    set opt $opt_base
    set x   [expr {$x + $legend_width}]
    $w cmd plscol0a 15 32 32 32 0.70

    set legend_data [$w cmd pllegend $position $opt $x $y \
        0.1 15 1 1 0 0 \
        $opt_array 1.0 $text_scale 2.0 \
        0. $text_colors $text \
        $box_colors $box_patterns $box_scales $box_line_widths \
        {} {} {} \
        {} {} {} {}]
    foreach {legend_width legend_height} $legend_data {break}
    set max_height [max $max_height $legend_height]

    # Set up box legend entries with various box pattern line widths.
    set opt_array       [list]
    set line_styles     [list]
    set line_widths     [list]
    set symbol_scales   [list]
    set symbol_numbers  [list]
    set symbols         [list]
    set text            [list]
    set text_colors     [list]
    set line_colors     [list]
    set symbol_colors   [list]
    set box_colors      [list]
    set box_patterns    [list]
    set box_scales      [list]
    set box_line_widths [list]
    for {set i 0} {$i < $nlegend} {incr i} {
        lappend opt_array       [expr {$::PLPLOT::PL_LEGEND_COLOR_BOX}]
        lappend text            [format "%s %d" "Box Line Width" [expr {$i + 1}]]
        lappend text_colors     2
        lappend box_colors      2
        lappend box_patterns    3
        lappend box_scales      0.8
        lappend box_line_widths [expr {$i + 1}]
    }

    set opt $opt_base
    set x   [expr {$x + $legend_width}]
    $w cmd plscol0a 15 32 32 32 0.70

    set legend_data [$w cmd pllegend $position $opt $x $y \
        0.1 15 1 1 0 0 \
        $opt_array 1.0 $text_scale 2.0 \
        0. $text_colors $text \
        $box_colors $box_patterns $box_scales $box_line_widths \
        {} {} {} \
        {} {} {} {}]
    foreach {legend_width legend_height} $legend_data {break}
    set max_height [max $max_height $legend_height]

    # Set up line legend entries with various colours.
    set opt_array       [list]
    set line_styles     [list]
    set line_widths     [list]
    set symbol_scales   [list]
    set symbol_numbers  [list]
    set symbols         [list]
    set text            [list]
    set text_colors     [list]
    set line_colors     [list]
    set symbol_colors   [list]
    set box_colors      [list]
    set box_patterns    [list]
    set box_scales      [list]
    set box_line_widths [list]
    for {set i 0} {$i < $nlegend} {incr i} {
        lappend opt_array   [expr {$::PLPLOT::PL_LEGEND_LINE}]
        lappend text        [format "%s %d" "Line Color" [expr {$i + 1}]]
        lappend text_colors [expr {$i + 1}]
        lappend line_colors [expr {$i + 1}]
        lappend line_styles 1
        lappend line_widths 1
    }

    set opt $opt_base
    # Use new origin
    set x          $xstart
    set y          [expr {$y + $max_height}]
    set max_height 0.
    $w cmd plscol0a 15 32 32 32 0.70

    set legend_data [$w cmd pllegend $position $opt $x $y \
        0.1 15 1 1 0 0 \
        $opt_array 1.0 $text_scale 2.0 \
        0. $text_colors $text \
        {} {} {} {} \
        $line_colors $line_styles $line_widths \
        {} {} {} {}]
    foreach {legend_width legend_height} $legend_data {break}
    set max_height [max $max_height $legend_height]

    # Set up line legend entries with various styles.
    set opt_array       [list]
    set line_styles     [list]
    set line_widths     [list]
    set symbol_scales   [list]
    set symbol_numbers  [list]
    set symbols         [list]
    set text            [list]
    set text_colors     [list]
    set line_colors     [list]
    set symbol_colors   [list]
    set box_colors      [list]
    set box_patterns    [list]
    set box_scales      [list]
    set box_line_widths [list]
    for {set i 0} {$i < $nlegend} {incr i} {
        lappend opt_array   [expr {$::PLPLOT::PL_LEGEND_LINE}]
        lappend text        [format "%s %d" "Line Style" [expr {$i + 1}]]
        lappend text_colors 2
        lappend line_colors 2
        lappend line_styles [expr {$i + 1}]
        lappend line_widths 1
    }

    set opt $opt_base
    set x   [expr {$x + $legend_width}]
    $w cmd plscol0a 15 32 32 32 0.70

    set legend_data [$w cmd pllegend $position $opt $x $y \
        0.1 15 1 1 0 0 \
        $opt_array 1.0 $text_scale 2.0 \
        0. $text_colors $text \
        {} {} {} {} \
        $line_colors $line_styles $line_widths \
        {} {} {} {}]
    foreach {legend_width legend_height} $legend_data {break}
    set max_height [max $max_height $legend_height]

    # Set up line legend entries with various widths.
    set opt_array       [list]
    set line_styles     [list]
    set line_widths     [list]
    set symbol_scales   [list]
    set symbol_numbers  [list]
    set symbols         [list]
    set text            [list]
    set text_colors     [list]
    set line_colors     [list]
    set symbol_colors   [list]
    set box_colors      [list]
    set box_patterns    [list]
    set box_scales      [list]
    set box_line_widths [list]
    for {set i 0} {$i < $nlegend} {incr i} {
        lappend opt_array   [expr {$::PLPLOT::PL_LEGEND_LINE}]
        lappend text        [format "%s %d" "Line Width" [expr {$i + 1}]]
        lappend text_colors 2
        lappend line_colors 2
        lappend line_styles 1
        lappend line_widths [expr {$i + 1}]
    }

    set opt $opt_base
    set x   [expr {$x + $legend_width}]
    $w cmd plscol0a 15 32 32 32 0.70

    set legend_data [$w cmd pllegend $position $opt $x $y \
        0.1 15 1 1 0 0 \
        $opt_array 1.0 $text_scale 2.0 \
        0. $text_colors $text \
        {} {} {} {} \
        $line_colors $line_styles $line_widths \
        {} {} {} {}]
    foreach {legend_width legend_height} $legend_data {break}
    set max_height [max $max_height $legend_height]
}

# Auxiliary routines
proc max {a b} {
    expr {$a > $b? $a : $b}
}

proc min {a b} {
    expr {$a < $b? $a : $b}
}
