# -*- coding: utf-8; -*-
# $Id$
#  Copyright (C) 2010 Alan W. Irwin

#  Demonstrate most pllegend capability including unicode symbols.

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
#  This example designed just for devices (e.g., the cairo-related and
#  qt-related devices) where the best choice of glyph is automatically
#  selected by the related libraries (pango/cairo or Qt4) for each
#  unicode character depending on what system fonts are installed.  Of
#  course, you must have the appropriate TrueType fonts installed to
#  have access to all the required glyphs.

from plplot_py_demos import *

position_options = [
PL_POSITION_LEFT | PL_POSITION_TOP | PL_POSITION_OUTSIDE,
PL_POSITION_TOP | PL_POSITION_OUTSIDE,
PL_POSITION_RIGHT | PL_POSITION_TOP | PL_POSITION_OUTSIDE,
PL_POSITION_RIGHT| PL_POSITION_OUTSIDE,
PL_POSITION_RIGHT | PL_POSITION_BOTTOM | PL_POSITION_OUTSIDE,
PL_POSITION_BOTTOM | PL_POSITION_OUTSIDE,
PL_POSITION_LEFT | PL_POSITION_BOTTOM | PL_POSITION_OUTSIDE,
PL_POSITION_LEFT | PL_POSITION_OUTSIDE,
PL_POSITION_LEFT | PL_POSITION_TOP | PL_POSITION_INSIDE,
PL_POSITION_TOP | PL_POSITION_INSIDE,
PL_POSITION_RIGHT | PL_POSITION_TOP | PL_POSITION_INSIDE,
PL_POSITION_RIGHT| PL_POSITION_INSIDE,
PL_POSITION_RIGHT | PL_POSITION_BOTTOM | PL_POSITION_INSIDE,
PL_POSITION_BOTTOM | PL_POSITION_INSIDE,
PL_POSITION_LEFT | PL_POSITION_BOTTOM | PL_POSITION_INSIDE,
PL_POSITION_LEFT | PL_POSITION_INSIDE,
]

# Pick 5 arbitrary UTF-8 symbols useful for plotting points (✠✚✱✪✽✺✰✴✦).
special_symbols = [
"✰",
"✴",
"✱",
"✽",
"✦",
]

def plcolorbar_example_1( bar_type, ticks, sub_ticks, values, title ):
    pladv( 0 )
    # Setup color palette 1
    plspal1( "cmap1_blue_red.pal", 1 )

    n = len(values)
    color_step = 1.0 / float(n - 1)
    colors = color_step*arange(n)

    opt = PL_POSITION_LEFT | bar_type | PL_COLORBAR_LABEL_LEFT | PL_COLORBAR_CAP_HIGH

    if bar_type & PL_COLORBAR_SHADE_LABEL:
        axis_opts_1 = "iv"
        axis_opts_2 = "i"
    else:
        if sub_ticks != 0:
            axis_opts_1 = "stv"
            axis_opts_2 = "st"
        else:
            axis_opts_1 = "tv"
            axis_opts_2 = "t"

    plcolorbar( opt, 0.1, 0.1, 0.5, 0.1,
                ticks, sub_ticks,
                axis_opts_1, "Test label - Left, High Cap",
                colors, values )

    opt = PL_POSITION_RIGHT | bar_type | PL_COLORBAR_LABEL_RIGHT | PL_COLORBAR_CAP_LOW

    plcolorbar( opt, 0.1, 0.4, 0.5, 0.1,
                ticks, sub_ticks,
                axis_opts_1, "Test label - Right, Low Cap",
                colors, values )

    opt = PL_POSITION_TOP | bar_type | PL_COLORBAR_LABEL_TOP | PL_COLORBAR_CAP_HIGH

    plcolorbar( opt, 0.1, 0.1, 0.5, 0.1,
                ticks, sub_ticks,
                axis_opts_2, "Test label - Upper, High Cap",
                colors, values )

    opt = PL_POSITION_BOTTOM | bar_type | PL_COLORBAR_LABEL_BOTTOM | PL_COLORBAR_CAP_LOW

    plcolorbar( opt, 0.4, 0.1, 0.5, 0.1,
                ticks, sub_ticks,
                axis_opts_2, "Test label - Lower, Low Cap",
                colors, values )

    plvpor( 0.0, 1.0, 0.0, 1.0 )
    plwind( 0.0, 1.0, 0.0, 1.0 )
    plptex( 0.5, 0.5, 0.0, 0.0, 0.5, title )

def plcolorbar_example_2( bar_type, ticks, sub_ticks, values, title ):
    pladv( 0 )
    # Setup color palette 1
    plspal1( "cmap1_blue_yellow.pal", 1 )

    n = len(values)
    color_step = 1.0 / float(n - 1)
    colors = color_step*arange(n)
    opt = PL_POSITION_LEFT | bar_type | PL_COLORBAR_LABEL_LEFT | PL_COLORBAR_CAP_LOW

    if bar_type == PL_COLORBAR_SHADE_LABEL:
        axis_opts_1 = ""
        axis_opts_2 = ""
    else:
        if sub_ticks != 0:
            axis_opts_1 = "stv"
            axis_opts_2 = "st"
        else:
            axis_opts_1 = "tv"
            axis_opts_2 = "t"

    plcolorbar( opt, 0.1, 0.1, 0.5, 0.1,
                ticks, sub_ticks,
                axis_opts_1, "Test label - Left, Low Cap",
                colors, values )

    opt = PL_POSITION_RIGHT | bar_type | PL_COLORBAR_LABEL_RIGHT | PL_COLORBAR_CAP_HIGH

    plcolorbar( opt, 0.1, 0.4, 0.5, 0.1,
                ticks, sub_ticks,
                axis_opts_1, "Test label - Right, High Cap",
                colors, values )

    opt = PL_POSITION_TOP | bar_type | PL_COLORBAR_LABEL_TOP | PL_COLORBAR_CAP_LOW

    plcolorbar( opt, 0.1, 0.1, 0.5, 0.1,
                ticks, sub_ticks,
                axis_opts_2, "Test label - Upper, Low Cap",
                colors, values )

    opt = PL_POSITION_BOTTOM | bar_type | PL_COLORBAR_LABEL_BOTTOM | PL_COLORBAR_CAP_HIGH

    plcolorbar( opt, 0.4, 0.1, 0.5, 0.1,
                ticks, sub_ticks,
                axis_opts_2, "Test label - Lower, High Cap",
                colors, values )

    plvpor( 0.0, 1.0, 0.0, 1.0 )
    plwind( 0.0, 1.0, 0.0, 1.0 )
    plptex( 0.5, 0.5, 0.0, 0.0, 0.5, title )

def main():
    # First page illustrating the 16 standard positions.
    pladv(0)
    plvpor(0.25, 0.75, 0.25, 0.75)
    plwind(0.0, 1.0, 0.0, 1.0)
    plbox("bc", 0.0, 0, "bc", 0.0, 0)
    plsfont(PL_FCI_SANS, -1, -1)
    plmtex("t", 8.0, 0.5, 0.5, "The 16 standard legend positions with")
    plmtex("t", 6.0, 0.5, 0.5, "the same (0.05) offset in x and y")

    # Set up legend arrays with the correct size, type.
    nlegend = 1
    opt_array = zeros(nlegend, "int")
    text_colors = zeros(nlegend, "int")
    text = zeros(nlegend, "S200")
    box_colors = zeros(nlegend, "int")
    box_patterns = zeros(nlegend, "int")
    box_scales = zeros(nlegend)
    box_line_widths = zeros(nlegend)
    line_colors = zeros(nlegend, "int")
    line_styles = zeros(nlegend, "int")
    line_widths = zeros(nlegend)
    symbol_colors = zeros(nlegend, "int")
    symbol_scales = zeros(nlegend)
    symbol_numbers = zeros(nlegend, "int")
    symbols = zeros(nlegend, "S100")

    # Only specify legend data that are required according to the
    # value of opt_array for that entry.
    opt_base = PL_LEGEND_BACKGROUND | PL_LEGEND_BOUNDING_BOX
    opt_array[0] = PL_LEGEND_LINE | PL_LEGEND_SYMBOL
    line_styles[0] = 1
    line_widths[0] = 1.
    symbol_scales[0] = 1.
    symbol_numbers[0] = 4
    symbols[0] = "*"

    # Use monotype fonts so that all legends are the same size.
    plsfont(PL_FCI_MONO, -1, -1)
    plscol0a( 15, 32, 32, 32, 0.70 )

    for k in range(16):
        position = position_options[k]
        opt = opt_base
        text[0] = "%2.2d" % k
        text_colors[0] = 1 + (k % 8)
        line_colors[0] = 1 + (k % 8)
        symbol_colors[0] = 1 + (k % 8)

        (legend_width, legend_height) = \
        pllegend( opt, position, 0.05, 0.05,
                  0.1, 15, 1, 1, 0, 0, opt_array, 1.0, 1.0, 2.0,
                  1., text_colors, text,
                  box_colors, box_patterns, box_scales, box_line_widths,
                  line_colors, line_styles, line_widths,
                  symbol_colors, symbol_scales, symbol_numbers, symbols )

    # Second page illustrating effect of nrow, ncolumn for the same legend
    # data.
    pladv(0)
    plvpor(0.25, 0.75, 0.25, 0.75)
    plwind(0.0, 1.0, 0.0, 1.0)
    plbox("bc", 0.0, 0, "bc", 0.0, 0)
    plsfont(PL_FCI_SANS, -1, -1)
    plmtex("t", 8.0, 0.5, 0.5, "The effect of nrow, ncolumn, PL_LEGEND_ROW_MAJOR,")
    plmtex("t", 6.0, 0.5, 0.5, "and position for the same legend data")

    # Set up legend arrays with the correct size, type.
    nlegend = 7
    opt_array = zeros(nlegend, "int")
    text_colors = zeros(nlegend, "int")
    text = zeros(nlegend, "S200")
    box_colors = zeros(nlegend, "int")
    box_patterns = zeros(nlegend, "int")
    box_scales = zeros(nlegend)
    box_line_widths = zeros(nlegend)
    line_colors = zeros(nlegend, "int")
    line_styles = zeros(nlegend, "int")
    line_widths = zeros(nlegend)
    symbol_colors = zeros(nlegend, "int")
    symbol_scales = zeros(nlegend)
    symbol_numbers = zeros(nlegend, "int")
    symbols = zeros(nlegend, "S100")

    # Only specify legend data that are required according to the
    # value of opt_array for that entry.
    opt_base = PL_LEGEND_BACKGROUND | PL_LEGEND_BOUNDING_BOX
    for k in range(nlegend):
        opt_array[k] = PL_LEGEND_LINE | PL_LEGEND_SYMBOL
        line_styles[k] = 1
        line_widths[k] = 1.
        symbol_scales[k] = 1.
        symbol_numbers[k] = 2
        symbols[k] = "*"
        text[k] = "%2.2d" % k
        text_colors[k] = 1 + (k % 8)
        line_colors[k] = 1 + (k % 8)
        symbol_colors[k] = 1 + (k % 8)

    # Use monotype fonts so that all legends are the same size.
    plsfont(PL_FCI_MONO, -1, -1)
    plscol0a( 15, 32, 32, 32, 0.70 )

    position = PL_POSITION_TOP | PL_POSITION_OUTSIDE
    opt = opt_base
    x = 0.
    y = 0.1
    nrow = 1
    ncolumn = nlegend
    (legend_width, legend_height) = \
     pllegend( opt, position, x, y,
              0.05, 15, 1, 1, nrow, ncolumn, opt_array, 1.0, 1.0, 2.0,
              1., text_colors, text,
              box_colors, box_patterns, box_scales, box_line_widths,
              line_colors, line_styles, line_widths,
              symbol_colors, symbol_scales, symbol_numbers, symbols )

    position = PL_POSITION_BOTTOM | PL_POSITION_OUTSIDE
    opt = opt_base
    x = 0.
    y = 0.1
    nrow = 1
    ncolumn = nlegend
    (legend_width, legend_height) = \
    pllegend( opt, position, x, y,
              0.05, 15, 1, 1, nrow, ncolumn, opt_array, 1.0, 1.0, 2.0,
              1., text_colors, text,
              box_colors, box_patterns, box_scales, box_line_widths,
              line_colors, line_styles, line_widths,
              symbol_colors, symbol_scales, symbol_numbers, symbols )

    position = PL_POSITION_LEFT | PL_POSITION_OUTSIDE
    opt = opt_base
    x = 0.1
    y = 0.
    nrow = nlegend
    ncolumn = 1
    (legend_width, legend_height) = \
    pllegend( opt, position, x, y,
              0.05, 15, 1, 1, nrow, ncolumn, opt_array, 1.0, 1.0, 2.0,
              1., text_colors, text,
              box_colors, box_patterns, box_scales, box_line_widths,
              line_colors, line_styles, line_widths,
              symbol_colors, symbol_scales, symbol_numbers, symbols )

    position = PL_POSITION_RIGHT | PL_POSITION_OUTSIDE
    opt = opt_base
    x = 0.1
    y = 0.
    nrow = nlegend
    ncolumn = 1
    (legend_width, legend_height) = \
    pllegend( opt, position, x, y,
              0.05, 15, 1, 1, nrow, ncolumn, opt_array, 1.0, 1.0, 2.0,
              1., text_colors, text,
              box_colors, box_patterns, box_scales, box_line_widths,
              line_colors, line_styles, line_widths,
              symbol_colors, symbol_scales, symbol_numbers, symbols )

    position = PL_POSITION_LEFT | PL_POSITION_TOP | PL_POSITION_INSIDE
    opt = opt_base
    x = 0.
    y = 0.
    nrow = 6
    ncolumn = 2
    (legend_width, legend_height) = \
    pllegend( opt, position, x, y,
              0.05, 15, 1, 1, nrow, ncolumn, opt_array, 1.0, 1.0, 2.0,
              1., text_colors, text,
              box_colors, box_patterns, box_scales, box_line_widths,
              line_colors, line_styles, line_widths,
              symbol_colors, symbol_scales, symbol_numbers, symbols )

    position = PL_POSITION_RIGHT | PL_POSITION_TOP | PL_POSITION_INSIDE
    opt = opt_base | PL_LEGEND_ROW_MAJOR
    x = 0.
    y = 0.
    nrow = 6
    ncolumn = 2
    (legend_width, legend_height) = \
    pllegend( opt, position, x, y,
              0.05, 15, 1, 1, nrow, ncolumn, opt_array, 1.0, 1.0, 2.0,
              1., text_colors, text,
              box_colors, box_patterns, box_scales, box_line_widths,
              line_colors, line_styles, line_widths,
              symbol_colors, symbol_scales, symbol_numbers, symbols )

    position = PL_POSITION_BOTTOM | PL_POSITION_INSIDE
    opt = opt_base | PL_LEGEND_ROW_MAJOR
    x = 0.
    y = 0.
    nrow = 3
    ncolumn = 3
    (legend_width, legend_height) = \
    pllegend( opt, position, x, y,
              0.05, 15, 1, 1, nrow, ncolumn, opt_array, 1.0, 1.0, 2.0,
              1., text_colors, text,
              box_colors, box_patterns, box_scales, box_line_widths,
              line_colors, line_styles, line_widths,
              symbol_colors, symbol_scales, symbol_numbers, symbols )

    # Third page demonstrating legend alignment
    pladv(0)
    plvpor(0.0, 1.0, 0.0, 0.9)
    plwind(0.0, 1.0, 0.0, 1.0)
    plsfont(PL_FCI_SANS, -1, -1)
    plmtex("t", 2.0, 0.5, 0.5, "Demonstrate legend alignment")

    x = 0.1
    y = 0.1
    nturn = 4
    nlegend = 0
    position = PL_POSITION_TOP |  PL_POSITION_LEFT | PL_POSITION_SUBPAGE
    opt_base = PL_LEGEND_BACKGROUND | PL_LEGEND_BOUNDING_BOX
    opt = opt_base
    for i in range(9):
        # Set up legend arrays with the correct size, type.
        if i <= nturn:
            nlegend += 1
        else:
            nlegend -= 1
        nlegend = max(1, nlegend)
        opt_array = zeros(nlegend, "int")
        text_colors = zeros(nlegend, "int")
        text = zeros(nlegend, "S200")
        box_colors = zeros(nlegend, "int")
        box_patterns = zeros(nlegend, "int")
        box_scales = zeros(nlegend)
        box_line_widths = zeros(nlegend)
        line_colors = zeros(nlegend, "int")
        line_styles = zeros(nlegend, "int")
        line_widths = zeros(nlegend)
        symbol_colors = zeros(nlegend, "int")
        symbol_scales = zeros(nlegend)
        symbol_numbers = zeros(nlegend, "int")
        symbols = zeros(nlegend, "S100")

        # Only specify legend data that are required according to the
        # value of opt_array for that entry.
        for k in range(nlegend):
            opt_array[k] = PL_LEGEND_LINE | PL_LEGEND_SYMBOL
            line_styles[k] = 1
            line_widths[k] = 1.
            symbol_scales[k] = 1.
            symbol_numbers[k] = 2
            symbols[k] = "*"
            text[k] = "%2.2d" % k
            text_colors[k] = 1 + (k % 8)
            line_colors[k] = 1 + (k % 8)
            symbol_colors[k] = 1 + (k % 8)

        # Use monotype fonts so that all legends are the same size.
        plsfont(PL_FCI_MONO, -1, -1)
        plscol0a( 15, 32, 32, 32, 0.70 )

        nrow = min(3, nlegend)
        ncolumn = 0

        (legend_width, legend_height) = \
        pllegend( opt, position, x, y,
                  0.025, 15, 1, 1, nrow, ncolumn, opt_array, 1.0, 1.0, 1.5,
                  1., text_colors, text,
                  box_colors, box_patterns, box_scales, box_line_widths,
                  line_colors, line_styles, line_widths,
                  symbol_colors, symbol_scales, symbol_numbers, symbols )
        if i == nturn:
            position = PL_POSITION_TOP |  PL_POSITION_RIGHT | PL_POSITION_SUBPAGE
            opt = opt_base
            x = 1. - x
            y += legend_height
        else:
            x += legend_width
            y += legend_height

    # Fourth page illustrating various kinds of legends
    max_height = 0.
    xstart = 0.0
    ystart = 0.1
    x = xstart
    y = ystart
    text_scale = 0.90
    pladv(0)
    plvpor(0.0, 1., 0.0, 0.90)
    plwind(0.0, 1.0, 0.0, 1.0)
    # plbox("bc", 0.0, 0, "bc", 0.0, 0)
    plsfont(PL_FCI_SANS, -1, -1)
    plmtex("t", 2.0, 0.5, 0.5, "Demonstrate Various Kinds of Legends")

    # Set up legend arrays with the correct size, type.
    nlegend = 5
    opt_array = zeros(nlegend, "int")
    text_colors = zeros(nlegend, "int")
    text = zeros(nlegend, "S200")
    box_colors = zeros(nlegend, "int")
    box_patterns = zeros(nlegend, "int")
    box_scales = zeros(nlegend)
    box_line_widths = zeros(nlegend)
    line_colors = zeros(nlegend, "int")
    line_styles = zeros(nlegend, "int")
    line_widths = zeros(nlegend)
    symbol_colors = zeros(nlegend, "int")
    symbol_scales = zeros(nlegend)
    symbol_numbers = zeros(nlegend, "int")
    symbols = zeros(nlegend, "S100")

    # Only specify legend data that are required according to the
    # value of opt_array for that entry.
    position = PL_POSITION_LEFT | PL_POSITION_TOP
    opt_base = PL_LEGEND_BACKGROUND | PL_LEGEND_BOUNDING_BOX | PL_LEGEND_TEXT_LEFT

    # Set up None, Box, Line, Symbol, and Line & Symbol legend entries.
    opt_array[0] = PL_LEGEND_NONE
    text[0] = "None"
    text_colors[0] = 1

    opt_array[1] = PL_LEGEND_COLOR_BOX
    text[1] = "Box"
    text_colors[1] = 2
    box_colors[1] = 2
    box_patterns[1] = 0
    box_scales[1] = 0.8
    box_line_widths[1] = 1.

    opt_array[2] = PL_LEGEND_LINE
    text[2] = "Line"
    text_colors[2] = 3
    line_colors[2] = 3
    line_styles[2] = 1
    line_widths[2] = 1.

    opt_array[3] = PL_LEGEND_SYMBOL
    text[3] = "Symbol"
    text_colors[3] = 4
    symbol_colors[3] = 4
    symbol_scales[3] = text_scale
    symbol_numbers[3] = 4
    symbols[3] = special_symbols[2]

    opt_array[4] = PL_LEGEND_SYMBOL | PL_LEGEND_LINE
    text[4] = "L & S"
    text_colors[4] = 5
    line_colors[4] = 5
    line_styles[4] = 1
    line_widths[4] = 1.
    symbol_colors[4] = 5
    symbol_scales[4] = text_scale
    symbol_numbers[4] = 4
    symbols[4] = special_symbols[2]

    opt = opt_base
    plscol0a( 15, 32, 32, 32, 0.70 )
    (legend_width, legend_height) = \
    pllegend( opt, position, x, y,
              0.1, 15, 1, 1, 0, 0, opt_array, 1.0, text_scale, 2.0,
              0., text_colors, text,
              box_colors, box_patterns, box_scales, box_line_widths,
              line_colors, line_styles, line_widths,
              symbol_colors, symbol_scales, symbol_numbers, symbols )
    max_height = max(max_height, legend_height)

    # Set up symbol legend entries with various symbols.
    for i in range(nlegend):
        opt_array[i] = PL_LEGEND_SYMBOL
        text[i] = "Symbol " + special_symbols[i]
        text_colors[i] = i+1
        symbol_colors[i] = i+1
        symbol_scales[i] = text_scale
        symbol_numbers[i] = 4
        symbols[i] = special_symbols[i]

    opt = opt_base
    x += legend_width
    plscol0a( 15, 32, 32, 32, 0.70 )
    (legend_width, legend_height) = \
    pllegend( opt, position, x, y,
              0.1, 15, 1, 1, 0, 0, opt_array, 1.0, text_scale, 2.0,
              0., text_colors, text,
              box_colors, box_patterns, box_scales, box_line_widths,
              line_colors, line_styles, line_widths,
              symbol_colors, symbol_scales, symbol_numbers, symbols )
    max_height = max(max_height, legend_height)

    # Set up symbol legend entries with various numbers of symbols.
    for i in range(nlegend):
        opt_array[i] = PL_LEGEND_SYMBOL
        text[i] = "Symbol Number %d" % (i+2)
        text_colors[i] = i+1
        symbol_colors[i] = i+1
        symbol_scales[i] = text_scale
        symbol_numbers[i] = i+2
        symbols[i] = special_symbols[2]

    opt = opt_base
    x += legend_width
    plscol0a( 15, 32, 32, 32, 0.70 )
    (legend_width, legend_height) = \
    pllegend( opt, position, x, y,
              0.1, 15, 1, 1, 0, 0, opt_array, 1.0, text_scale, 2.0,
              0., text_colors, text,
              box_colors, box_patterns, box_scales, box_line_widths,
              line_colors, line_styles, line_widths,
              symbol_colors, symbol_scales, symbol_numbers, symbols )
    max_height = max(max_height, legend_height)

    # Set up box legend entries with various colours.
    for i in range(nlegend):
        opt_array[i] = PL_LEGEND_COLOR_BOX
        text[i] = "%s %d" % ("Box Color",i+1)
        text_colors[i] = i+1
        box_colors[i] = i+1
        box_patterns[i] = 0
        box_scales[i] = 0.8
        box_line_widths[i] = 1.

    opt = opt_base
    # Use new origin
    x = xstart
    y += max_height
    max_height = 0.
    plscol0a( 15, 32, 32, 32, 0.70 )
    (legend_width, legend_height) = \
    pllegend( opt, position, x, y,
              0.1, 15, 1, 1, 0, 0, opt_array, 1.0, text_scale, 2.0,
              0., text_colors, text,
              box_colors, box_patterns, box_scales, box_line_widths,
              line_colors, line_styles, line_widths,
              symbol_colors, symbol_scales, symbol_numbers, symbols )
    max_height = max(max_height, legend_height)

    # Set up box legend entries with various patterns.
    for i in range(nlegend):
        opt_array[i] = PL_LEGEND_COLOR_BOX
        text[i] = "%s %d" % ("Box Pattern",i)
        text_colors[i] = 2
        box_colors[i] = 2
        box_patterns[i] = i
        box_scales[i] = 0.8
        box_line_widths[i] = 1.

    opt = opt_base
    x += legend_width
    plscol0a( 15, 32, 32, 32, 0.70 )
    (legend_width, legend_height) = \
    pllegend( opt, position, x, y,
              0.1, 15, 1, 1, 0, 0, opt_array, 1.0, text_scale, 2.0,
              0., text_colors, text,
              box_colors, box_patterns, box_scales, box_line_widths,
              line_colors, line_styles, line_widths,
              symbol_colors, symbol_scales, symbol_numbers, symbols )
    max_height = max(max_height, legend_height)

    # Set up box legend entries with various box pattern line widths.
    for i in range(nlegend):
        opt_array[i] = PL_LEGEND_COLOR_BOX
        text[i] = "%s %d" % ("Box Line Width",i+1)
        text_colors[i] = 2
        box_colors[i] = 2
        box_patterns[i] = 3
        box_scales[i] = 0.8
        box_line_widths[i] = float(i+1)

    opt = opt_base
    x += legend_width
    plscol0a( 15, 32, 32, 32, 0.70 )
    (legend_width, legend_height) = \
    pllegend( opt, position, x, y,
              0.1, 15, 1, 1, 0, 0, opt_array, 1.0, text_scale, 2.0,
              0., text_colors, text,
              box_colors, box_patterns, box_scales, box_line_widths,
              line_colors, line_styles, line_widths,
              symbol_colors, symbol_scales, symbol_numbers, symbols )
    max_height = max(max_height, legend_height)

    # Set up line legend entries with various colours.
    for i in range(nlegend):
        opt_array[i] = PL_LEGEND_LINE
        text[i] = "%s %d" % ("Line Color",i+1)
        text_colors[i] = i+1
        line_colors[i] = i+1
        line_styles[i] = 1
        line_widths[i] = 1.

    opt = opt_base
    # Use new origin
    x = xstart
    y += max_height
    max_height = 0.
    plscol0a( 15, 32, 32, 32, 0.70 )
    (legend_width, legend_height) = \
    pllegend( opt, position, x, y,
              0.1, 15, 1, 1, 0, 0, opt_array, 1.0, text_scale, 2.0,
              0., text_colors, text,
              box_colors, box_patterns, box_scales, box_line_widths,
              line_colors, line_styles, line_widths,
              symbol_colors, symbol_scales, symbol_numbers, symbols )
    max_height = max(max_height, legend_height)

    # Set up line legend entries with various styles
    for i in range(nlegend):
        opt_array[i] = PL_LEGEND_LINE
        text[i] = "%s %d" % ("Line Style",i+1)
        text_colors[i] = 2
        line_colors[i] = 2
        line_styles[i] = i+1
        line_widths[i] = 1.

    opt = opt_base
    x += legend_width
    plscol0a( 15, 32, 32, 32, 0.70 )
    (legend_width, legend_height) = \
    pllegend( opt, position, x, y,
              0.1, 15, 1, 1, 0, 0, opt_array, 1.0, text_scale, 2.0,
              0., text_colors, text,
              box_colors, box_patterns, box_scales, box_line_widths,
              line_colors, line_styles, line_widths,
              symbol_colors, symbol_scales, symbol_numbers, symbols )
    max_height = max(max_height, legend_height)

    # Set up line legend entries with various widths.
    for i in range(nlegend):
        opt_array[i] = PL_LEGEND_LINE
        text[i] = "%s %d" % ("Line Width",i+1)
        text_colors[i] = 2
        line_colors[i] = 2
        line_styles[i] = 1
        line_widths[i] = float(i+1)

    opt = opt_base
    x += legend_width
    plscol0a( 15, 32, 32, 32, 0.70 )
    (legend_width, legend_height) = \
    pllegend( opt, position, x, y,
              0.1, 15, 1, 1, 0, 0, opt_array, 1.0, text_scale, 2.0,
              0., text_colors, text,
              box_colors, box_patterns, box_scales, box_line_widths,
              line_colors, line_styles, line_widths,
              symbol_colors, symbol_scales, symbol_numbers, symbols )
    max_height = max(max_height, legend_height)

    if False:
        # Color bar examples
        values_small = [ 0.0, 1.0 ]
        values_uneven = [ 0.0, 2.0, 2.6, 3.4, 6.0, 7.0, 8.0, 9.0, 10.0 ]
        values_even = [ 0.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0 ]
        plcolorbar_example_1( PL_COLORBAR_IMAGE, 0.0, 0, values_small, "Image Color Bars" )
        plcolorbar_example_2( PL_COLORBAR_IMAGE, 0.0, 0, values_small, "Image Color Bars" )
        plcolorbar_example_1( PL_COLORBAR_SHADE | PL_COLORBAR_SHADE_LABEL, 0.0, 0, values_uneven, "Shade Color Bars - Uneven Steps" )
        plcolorbar_example_2( PL_COLORBAR_SHADE, 3.0, 3, values_even, "Shade Color Bars - Even Steps" )
        plcolorbar_example_1( PL_COLORBAR_GRADIENT, 0.5, 5, values_small, "Gradient Color Bars" )
        plcolorbar_example_2( PL_COLORBAR_GRADIENT, 0.5, 5, values_small, "Gradient Color Bars" )
main()
