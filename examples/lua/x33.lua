--[[ $Id: x04.lua 11667 2011-03-21 03:35:08Z airwin $

  Demonstrate most pllegend capability including unicode symbols.

  Copyright (C) 2010 Alan W. Irwin

  This file is part of PLplot.

  PLplot is free software you can redistribute it and/or modify
  it under the terms of the GNU General Library Public License as published
  by the Free Software Foundation either version 2 of the License, or
  (at your option) any later version.

  PLplot is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU Library General Public License for more details.

  You should have received a copy of the GNU Library General Public License
  along with PLplot if not, write to the Free Software
  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
--]]

-- initialise Lua bindings for PLplot examples.
dofile("plplot_examples.lua")

----------------------------------------------------------------------------
-- plot1
--
-- Log-linear plot.
----------------------------------------------------------------------------

-- return single bit (for OR)
function bit(x,b)
  return (math.mod(x, 2^b) - math.mod(x,2^(b-1)) > 0)
end

-- logic OR for number values

function lor(x,y)
  result = 0
  for p=1,8 do result = result + (((bit(x,p) or bit(y,p)) == true) and 2^(p-1) or 0) end
  return result
end

-- Intialize pllegend arrays taking advantage of Lua's default global scope.

function initialize_pllegend_arrays(nlegend)
  opt_array = {}
  text_colors = {}
  text = {}
  box_colors = {}
  box_patterns = {}
  box_scales = {}
  box_line_widths = {}
  line_colors = {}
  line_styles = {}
  line_widths = {}
  symbol_colors = {}
  symbol_scales = {}
  symbol_numbers = {}
  symbols = {}
  for i=1,nlegend do
    opt_array[i] = 0
    text_colors[i] = 0
    text[i] = ""
    box_colors[i] = 0
    box_patterns[i] = 0
    box_scales[i] = 0.0
    box_line_widths[i] = 0
    line_colors[i] = 0
    line_styles[i] = 0
    line_widths[i] = 0
    symbol_colors[i] = 0
    symbol_scales[i] = 0.0
    symbol_numbers[i] = 0
    symbols[i] = ""
  end
end

----------------------------------------------------------------------------
-- main
----------------------------------------------------------------------------

position_options = {
lor(pl.PL_POSITION_LEFT, lor(pl.PL_POSITION_TOP , pl.PL_POSITION_OUTSIDE)),
lor(pl.PL_POSITION_TOP, pl.PL_POSITION_OUTSIDE),
lor(pl.PL_POSITION_RIGHT, lor(pl.PL_POSITION_TOP, pl.PL_POSITION_OUTSIDE)),
lor(pl.PL_POSITION_RIGHT, pl.PL_POSITION_OUTSIDE),
lor(pl.PL_POSITION_RIGHT, lor(pl.PL_POSITION_BOTTOM, pl.PL_POSITION_OUTSIDE)),
lor(pl.PL_POSITION_BOTTOM, pl.PL_POSITION_OUTSIDE),
lor(pl.PL_POSITION_LEFT, lor(pl.PL_POSITION_BOTTOM, pl.PL_POSITION_OUTSIDE)),
lor(pl.PL_POSITION_LEFT, pl.PL_POSITION_OUTSIDE),
lor(pl.PL_POSITION_LEFT, lor(pl.PL_POSITION_TOP, pl.PL_POSITION_INSIDE)),
lor(pl.PL_POSITION_TOP, pl.PL_POSITION_INSIDE),
lor(pl.PL_POSITION_RIGHT, lor(pl.PL_POSITION_TOP, pl.PL_POSITION_INSIDE)),
lor(pl.PL_POSITION_RIGHT, pl.PL_POSITION_INSIDE),
lor(pl.PL_POSITION_RIGHT, lor(pl.PL_POSITION_BOTTOM, pl.PL_POSITION_INSIDE)),
lor(pl.PL_POSITION_BOTTOM, pl.PL_POSITION_INSIDE),
lor(pl.PL_POSITION_LEFT, lor(pl.PL_POSITION_BOTTOM, pl.PL_POSITION_INSIDE)),
lor(pl.PL_POSITION_LEFT, pl.PL_POSITION_INSIDE)
}

-- Pick 5 arbitrary UTF-8 symbols useful for plotting points (✠✚✱✪✽✺✰✴✦).
special_symbols = {
"✰",
"✴",
"✱",
"✽",
"✦"
}

-- Parse and process command line arguments 
pl.parseopts(arg, pl.PL_PARSE_FULL)

-- Initialize plplot 
pl.init()

-- First page illustrating the 16 standard positions.
pl.adv(0)
pl.vpor(0.25, 0.75, 0.25, 0.75)
pl.wind(0.0, 1.0, 0.0, 1.0)
pl.box("bc", 0.0, 0, "bc", 0.0, 0)
pl.sfont(pl.PL_FCI_SANS, -1, -1)
pl.mtex("t", 8.0, 0.5, 0.5, "The 16 standard legend positions with")
pl.mtex("t", 6.0, 0.5, 0.5, "the same (0.05) offset in x and y")

nlegend = 1
initialize_pllegend_arrays(nlegend)
-- Only specify legend data that are required according to the
-- value of opt_array for that entry.
opt_base = lor(pl.PL_LEGEND_BACKGROUND, pl.PL_LEGEND_BOUNDING_BOX)
opt_array[1] = lor(pl.PL_LEGEND_LINE, pl.PL_LEGEND_SYMBOL)
line_styles[1] = 1
line_widths[1] = 1
symbol_scales[1] = 1.
symbol_numbers[1] = 4
symbols[1] = "*"
-- Use monotype fonts so that all legends are the same size.
pl.sfont(pl.PL_FCI_MONO, -1, -1)
pl.scol0a( 15, 32, 32, 32, 0.70 )

for k=0,15 do
  position = position_options[k+1]
  opt = opt_base
  text[1] = string.format("%2.2d", k)
  text_colors[1] = 1 + (k % 8)
  line_colors[1] = 1 + (k % 8)
  symbol_colors[1] = 1 + (k % 8)
  legend_width, legend_height = pl.legend(
    position, opt, 0.05, 0.05,
    0.1, 15, 1, 1, 0, 0, opt_array, 1.0, 1.0, 2.0,
    1., text_colors, text, 
    box_colors, box_patterns, box_scales, box_line_widths, 
    line_colors, line_styles, line_widths,
    symbol_colors, symbol_scales, symbol_numbers, symbols )
end

pl.plend()
