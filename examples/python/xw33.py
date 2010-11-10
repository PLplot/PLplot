# -*- coding: utf-8; -*-
# $Id$
#  Copyright (C) 2006-2010 Alan W. Irwin

#  Demonstrate most pllegend capability including unicode symbols.

#  This file is part of PLplot.
#
#  PLplot is free software; you can redistribute it and/or modify
#  it under the terms of the GNU General Library Public License as published
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
PL_LEGEND_LEFT | PL_LEGEND_UPPER | PL_LEGEND_OUTSIDE,
PL_LEGEND_UPPER | PL_LEGEND_OUTSIDE,
PL_LEGEND_RIGHT | PL_LEGEND_UPPER | PL_LEGEND_OUTSIDE,
PL_LEGEND_RIGHT| PL_LEGEND_OUTSIDE,
PL_LEGEND_RIGHT | PL_LEGEND_LOWER | PL_LEGEND_OUTSIDE,
PL_LEGEND_LOWER | PL_LEGEND_OUTSIDE,
PL_LEGEND_LEFT | PL_LEGEND_LOWER | PL_LEGEND_OUTSIDE,
PL_LEGEND_LEFT | PL_LEGEND_OUTSIDE,
PL_LEGEND_LEFT | PL_LEGEND_UPPER | PL_LEGEND_INSIDE,
PL_LEGEND_UPPER | PL_LEGEND_INSIDE,
PL_LEGEND_RIGHT | PL_LEGEND_UPPER | PL_LEGEND_INSIDE,
PL_LEGEND_RIGHT| PL_LEGEND_INSIDE,
PL_LEGEND_RIGHT | PL_LEGEND_LOWER | PL_LEGEND_INSIDE,
PL_LEGEND_LOWER | PL_LEGEND_INSIDE,
PL_LEGEND_LEFT | PL_LEGEND_LOWER | PL_LEGEND_INSIDE,
PL_LEGEND_LEFT | PL_LEGEND_INSIDE,
]

def main():
    pladv(0)
    plvpor(0.25, 0.75, 0.25, 0.75)
    plwind(0.0, 1.0, 0.0, 1.0)
    plbox("bc", 0.0, 0, "bc", 0.0, 0)
    plmtex("t", 7.5, 0.5, 0.5, "The 16 standard legend positions with")
    plmtex("t", 6., 0.5, 0.5, "the same (0.05) offset in x and y") 

    # Set up legend arrays with the correct size, type.
    nlegend = 1
    opt_array = zeros(nlegend, "int")
    text_colors = zeros(nlegend, "int")
    text = zeros(nlegend, "S200")
    box_colors = zeros(nlegend, "int")
    box_patterns = zeros(nlegend, "int")
    box_scales = zeros(nlegend)
    line_colors = zeros(nlegend, "int")
    line_styles = zeros(nlegend, "int")
    line_widths = zeros(nlegend, "int")
    symbol_colors = zeros(nlegend, "int")
    symbol_scales = zeros(nlegend)
    symbol_numbers = zeros(nlegend, "int")
    symbols = zeros(nlegend, "S100")

    # Only specify legend data that are required according to the
    # value of opt_array for that entry.
    opt_base = PL_LEGEND_BACKGROUND | PL_LEGEND_BOUNDING_BOX
    opt_array[0] = PL_LEGEND_LINE | PL_LEGEND_SYMBOL
    line_styles[0] = 1
    line_widths[0] = 1
    symbol_scales[0] = 1.
    symbol_numbers[0] = 4
    symbols[0] = "*"

    # Use monotype fonts so that all legends are the same size.
    plsfont(PL_FCI_MONO, -1, -1)
    plscol0a( 15, 32, 32, 32, 0.70 ) 

    for k in range(16):
        opt = opt_base | position_options[k]
        text[0] = "%2.2d" % k
        text_colors[0] = 1 + (k % 8)
        line_colors[0] = 1 + (k % 8)
        symbol_colors[0] = 1 + (k % 8)

        pllegend( opt, 0.05, 0.05,
                  0.1, 15, 1, 1, 0, 0, opt_array, 1.0, 1.0, 2.0,
                  1., text_colors, text, box_colors, box_patterns,
                  box_scales, line_colors, line_styles, line_widths,
                  symbol_colors, symbol_scales, symbol_numbers, symbols )

main()
