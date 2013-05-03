--[[ $Id: x04.lua 11667 2011-03-21 03:35:08Z airwin $

  Demonstrate most pllegend capability including unicode symbols.

  Copyright (C) 2010 Alan W. Irwin

  This file is part of PLplot.

  PLplot is free software you can redistribute it and/or modify
  it under the terms of the GNU Library General Public License as published
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
  return ((x % 2^b) - (x % 2^(b-1)) > 0)
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
    box_line_widths[i] = 0.
    line_colors[i] = 0
    line_styles[i] = 0
    line_widths[i] = 0.
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

-- Set up legend arrays with the correct size, type.
initialize_pllegend_arrays(nlegend)

-- Only specify legend data that are required according to the
-- value of opt_array for that entry.
opt_base = lor(pl.PL_LEGEND_BACKGROUND, pl.PL_LEGEND_BOUNDING_BOX)
opt_array[1] = lor(pl.PL_LEGEND_LINE, pl.PL_LEGEND_SYMBOL)
line_styles[1] = 1
line_widths[1] = 1.
symbol_scales[1] = 1.0
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
    opt, position, 0.05, 0.05,
    0.1, 15, 1, 1, 0, 0, opt_array, 1.0, 1.0, 2.0,
    1.0, text_colors, text, 
    box_colors, box_patterns, box_scales, box_line_widths, 
    line_colors, line_styles, line_widths,
    symbol_colors, symbol_scales, symbol_numbers, symbols )
end

-- Second page illustrating effect of nrow, ncolumn for the same legend
-- data
pl.adv(0)
pl.vpor(0.25, 0.75, 0.25, 0.75)
pl.wind(0.0, 1.0, 0.0, 1.0)
pl.box("bc", 0.0, 0, "bc", 0.0, 0)
pl.sfont(pl.PL_FCI_SANS, -1, -1)
pl.mtex("t", 8.0, 0.5, 0.5, "The effect of nrow, ncolumn, PL_LEGEND_ROW_MAJOR,")
pl.mtex("t", 6.0, 0.5, 0.5, "and position for the same legend data")

-- Set up legend arrays with the correct size, type.
nlegend = 7
initialize_pllegend_arrays(nlegend)

-- Only specify legend data that are required according to the
-- value of opt_array for that entry.
opt_base = lor(pl.PL_LEGEND_BACKGROUND, pl.PL_LEGEND_BOUNDING_BOX)
for k = 0, nlegend-1 do
  opt_array[k+1] = lor(pl.PL_LEGEND_LINE, pl.PL_LEGEND_SYMBOL)
  line_styles[k+1] = 1
  line_widths[k+1] = 1.
  symbol_scales[k+1] = 1.0
  symbol_numbers[k+1] = 2
  symbols[k+1] = "*"
  text[k+1] = string.format("%2.2d", k)
  text_colors[k+1] = 1 + (k % 8)
  line_colors[k+1] = 1 + (k % 8)
  symbol_colors[k+1] = 1 + (k % 8)
end

-- Use monotype fonts so that all legends are the same size.
pl.sfont(pl.PL_FCI_MONO, -1, -1)
pl.scol0a( 15, 32, 32, 32, 0.70 )

position = lor(pl.PL_POSITION_TOP, pl.PL_POSITION_OUTSIDE)
opt = opt_base
x = 0.
y = 0.1
nrow = 1
ncolumn = nlegend
legend_width, legend_height = pl.legend(
  opt, position, x, y,
  0.05, 15, 1, 1, nrow, ncolumn, opt_array, 1.0, 1.0, 2.0,
  1.0, text_colors, text, 
  box_colors, box_patterns, box_scales, box_line_widths, 
  line_colors, line_styles, line_widths,
  symbol_colors, symbol_scales, symbol_numbers, symbols )

position = lor(pl.PL_POSITION_BOTTOM, pl.PL_POSITION_OUTSIDE)
opt = opt_base
x = 0.
y = 0.1
nrow = 1
ncolumn = nlegend
legend_width, legend_height = pl.legend(
  opt, position, x, y,
  0.05, 15, 1, 1, nrow, ncolumn, opt_array, 1.0, 1.0, 2.0,
  1.0, text_colors, text, 
  box_colors, box_patterns, box_scales, box_line_widths, 
  line_colors, line_styles, line_widths,
  symbol_colors, symbol_scales, symbol_numbers, symbols )

position = lor(pl.PL_POSITION_LEFT, pl.PL_POSITION_OUTSIDE)
opt = opt_base
x = 0.1
y = 0.
nrow = nlegend
ncolumn = 1
legend_width, legend_height = pl.legend(
  opt, position, x, y,
  0.05, 15, 1, 1, nrow, ncolumn, opt_array, 1.0, 1.0, 2.0,
  1.0, text_colors, text, 
  box_colors, box_patterns, box_scales, box_line_widths, 
  line_colors, line_styles, line_widths,
  symbol_colors, symbol_scales, symbol_numbers, symbols )

position = lor(pl.PL_POSITION_RIGHT, pl.PL_POSITION_OUTSIDE)
opt = opt_base
x = 0.1
y = 0.
nrow = nlegend
ncolumn = 1
legend_width, legend_height = pl.legend(
  opt, position, x, y,
  0.05, 15, 1, 1, nrow, ncolumn, opt_array, 1.0, 1.0, 2.0,
  1.0, text_colors, text, 
  box_colors, box_patterns, box_scales, box_line_widths, 
  line_colors, line_styles, line_widths,
  symbol_colors, symbol_scales, symbol_numbers, symbols )

position = lor(pl.PL_POSITION_LEFT, lor(pl.PL_POSITION_TOP, pl.PL_POSITION_INSIDE))
opt = opt_base
x = 0.
y = 0.
nrow = 6
ncolumn = 2
legend_width, legend_height = pl.legend(
  opt, position, x, y,
  0.05, 15, 1, 1, nrow, ncolumn, opt_array, 1.0, 1.0, 2.0,
  1.0, text_colors, text, 
  box_colors, box_patterns, box_scales, box_line_widths, 
  line_colors, line_styles, line_widths,
  symbol_colors, symbol_scales, symbol_numbers, symbols )

position = lor(pl.PL_POSITION_RIGHT, lor(pl.PL_POSITION_TOP, pl.PL_POSITION_INSIDE))
opt = lor(opt_base, pl.PL_LEGEND_ROW_MAJOR)
x = 0.
y = 0.
nrow = 6
ncolumn = 2
legend_width, legend_height = pl.legend(
  opt, position, x, y,
  0.05, 15, 1, 1, nrow, ncolumn, opt_array, 1.0, 1.0, 2.0,
  1.0, text_colors, text, 
  box_colors, box_patterns, box_scales, box_line_widths, 
  line_colors, line_styles, line_widths,
  symbol_colors, symbol_scales, symbol_numbers, symbols )

position = lor(pl.PL_POSITION_BOTTOM, pl.PL_POSITION_INSIDE)
opt = lor(opt_base, pl.PL_LEGEND_ROW_MAJOR)
x = 0.
y = 0.
nrow = 3
ncolumn = 3
legend_width, legend_height = pl.legend(
  opt, position, x, y,
  0.05, 15, 1, 1, nrow, ncolumn, opt_array, 1.0, 1.0, 2.0,
  1.0, text_colors, text, 
  box_colors, box_patterns, box_scales, box_line_widths, 
  line_colors, line_styles, line_widths,
  symbol_colors, symbol_scales, symbol_numbers, symbols )

-- Third page demonstrating legend alignment
pl.adv(0)
pl.vpor(0.0, 1.0, 0.0, 0.9)
pl.wind(0.0, 1.0, 0.0, 1.0)
pl.sfont(pl.PL_FCI_SANS, -1, -1)
pl.mtex("t", 2.0, 0.5, 0.5, "Demonstrate legend alignment")

x = 0.1
y = 0.1
nturn = 4
nlegend = 0
position = lor(pl.PL_POSITION_TOP, lor(pl.PL_POSITION_LEFT, pl.PL_POSITION_SUBPAGE))
opt_base = lor(pl.PL_LEGEND_BACKGROUND, pl.PL_LEGEND_BOUNDING_BOX)
opt = opt_base
for i=0,8 do
  -- Set up legend arrays with the correct size, type.
  if i <= nturn then
    nlegend = nlegend + 1
  else
    nlegend = nlegend - 1
  end
  nlegend = math.max(1, nlegend)
  initialize_pllegend_arrays(nlegend)
  -- Only specify legend data that are required according to the
  -- value of opt_array for that entry.
  for k = 0, nlegend-1 do
    opt_array[k+1] = lor(pl.PL_LEGEND_LINE, pl.PL_LEGEND_SYMBOL)
    line_styles[k+1] = 1
    line_widths[k+1] = 1.
    symbol_scales[k+1] = 1.0
    symbol_numbers[k+1] = 2
    symbols[k+1] = "*"
    text[k+1] = string.format("%2.2d", k)
    text_colors[k+1] = 1 + (k % 8)
    line_colors[k+1] = 1 + (k % 8)
    symbol_colors[k+1] = 1 + (k % 8)
  end
  -- Use monotype fonts so that all legends are the same size.
  pl.sfont(pl.PL_FCI_MONO, -1, -1)
  pl.scol0a( 15, 32, 32, 32, 0.70 )

  nrow = math.min(3, nlegend)
  ncolumn = 0
  legend_width, legend_height = pl.legend(
    opt, position, x, y,
    0.025, 15, 1, 1, nrow, ncolumn, opt_array, 1.0, 1.0, 1.5,
    1.0, text_colors, text, 
    box_colors, box_patterns, box_scales, box_line_widths, 
    line_colors, line_styles, line_widths,
    symbol_colors, symbol_scales, symbol_numbers, symbols )
  if i==nturn then
    position = lor(pl.PL_POSITION_TOP, lor(pl.PL_POSITION_RIGHT, pl.PL_POSITION_SUBPAGE))
    opt = opt_base
    x = 1.0 - x
    y = y + legend_height
  else
    x = x + legend_width
    y = y + legend_height
  end
end

-- Fourth page illustrating various kinds of legends
max_height = 0.
xstart = 0.0
ystart = 0.1
x = xstart
y = ystart
text_scale = 0.90
pl.adv(0)
pl.vpor(0.0, 1.0, 0.0, 0.90)
pl.wind(0.0, 1.0, 0.0, 1.0)
-- pl.box("bc", 0.0, 0, "bc", 0.0, 0)
pl.sfont(pl.PL_FCI_SANS, -1, -1)
pl.mtex("t", 2.0, 0.5, 0.5, "Demonstrate Various Kinds of Legends")

-- Set up legend arrays with the correct size, type.
nlegend = 5
initialize_pllegend_arrays(nlegend)

-- Only specify legend data that are required according to the
-- value of opt_array for that entry.
position = lor(pl.PL_POSITION_LEFT, pl.PL_POSITION_TOP)
opt_base = lor(pl.PL_LEGEND_BACKGROUND, lor(pl.PL_LEGEND_BOUNDING_BOX, pl.PL_LEGEND_TEXT_LEFT))

-- Set up None, Box, Line, Symbol, and Line & Symbol legend entries.
opt_array[1] = pl.PL_LEGEND_NONE
text[1] = "None"
text_colors[1] = 1

opt_array[2] = pl.PL_LEGEND_COLOR_BOX
text[2] = "Box"
text_colors[2] = 2
box_colors[2] = 2
box_patterns[2] = 0
box_scales[2] = 0.8
box_line_widths[2] = 1.

opt_array[3] = pl.PL_LEGEND_LINE
text[3] = "Line"
text_colors[3] = 3
line_colors[3] = 3
line_styles[3] = 1
line_widths[3] = 1.

opt_array[4] = pl.PL_LEGEND_SYMBOL
text[4] = "Symbol"
text_colors[4] = 4
symbol_colors[4] = 4
symbol_scales[4] = text_scale
symbol_numbers[4] = 4
symbols[4] = special_symbols[3]

opt_array[5] = lor(pl.PL_LEGEND_SYMBOL, pl.PL_LEGEND_LINE)
text[5] = "L & S"
text_colors[5] = 5
line_colors[5] = 5
line_styles[5] = 1
line_widths[5] = 1.
symbol_colors[5] = 5
symbol_scales[5] = text_scale
symbol_numbers[5] = 4
symbols[5] = special_symbols[3]

opt = opt_base
pl.scol0a( 15, 32, 32, 32, 0.70 )
legend_width, legend_height = pl.legend(
  opt, position, x, y,
  0.1, 15, 1, 1, 0, 0, opt_array, 1.0, text_scale, 2.0,
  0.0, text_colors, text, 
  box_colors, box_patterns, box_scales, box_line_widths, 
  line_colors, line_styles, line_widths,
  symbol_colors, symbol_scales, symbol_numbers, symbols )

-- Set up symbol legend entries with various symbols.
for i=0,nlegend-1 do
  opt_array[i+1] = pl.PL_LEGEND_SYMBOL
  text[i+1] = "Symbol " .. special_symbols[i+1]
  text_colors[i+1] = i+1
  symbol_colors[i+1] = i+1
  symbol_scales[i+1] = text_scale
  symbol_numbers[i+1] = 4
  symbols[i+1] = special_symbols[i+1]
end

opt = opt_base
x = x + legend_width
pl.scol0a( 15, 32, 32, 32, 0.70 )
legend_width, legend_height = pl.legend(
  opt, position, x, y,
  0.1, 15, 1, 1, 0, 0, opt_array, 1.0, text_scale, 2.0,
  0.0, text_colors, text, 
  box_colors, box_patterns, box_scales, box_line_widths, 
  line_colors, line_styles, line_widths,
  symbol_colors, symbol_scales, symbol_numbers, symbols )
max_height = math.max(max_height, legend_height)

-- Set up symbol legend entries with various numbers of symbols.
for i=0,nlegend-1 do
  opt_array[i+1] = pl.PL_LEGEND_SYMBOL
  text[i+1] = string.format("Symbol Number %d", i+2)
  text_colors[i+1] = i+1
  symbol_colors[i+1] = i+1
  symbol_scales[i+1] = text_scale
  symbol_numbers[i+1] = i+2
  symbols[i+1] = special_symbols[3]
end

opt = opt_base
x = x + legend_width
pl.scol0a( 15, 32, 32, 32, 0.70 )
legend_width, legend_height = pl.legend(
  opt, position, x, y,
  0.1, 15, 1, 1, 0, 0, opt_array, 1.0, text_scale, 2.0,
  0.0, text_colors, text, 
  box_colors, box_patterns, box_scales, box_line_widths, 
  line_colors, line_styles, line_widths,
  symbol_colors, symbol_scales, symbol_numbers, symbols )
max_height = math.max(max_height, legend_height)

-- Set up box legend entries with various colours.
for i=0,nlegend-1 do
  opt_array[i+1] = pl.PL_LEGEND_COLOR_BOX
  text[i+1] = string.format("%s %d", "Box Color", i+1)
  text_colors[i+1] = i+1
  box_colors[i+1] = i+1
  box_patterns[i+1] = 0
  box_scales[i+1] = 0.8
  box_line_widths[i+1] = 1.
end

opt = opt_base
-- Use new origin
x = xstart
y = y + max_height
max_height = 0
pl.scol0a( 15, 32, 32, 32, 0.70 )
legend_width, legend_height = pl.legend(
  opt, position, x, y,
  0.1, 15, 1, 1, 0, 0, opt_array, 1.0, text_scale, 2.0,
  0.0, text_colors, text, 
  box_colors, box_patterns, box_scales, box_line_widths, 
  line_colors, line_styles, line_widths,
  symbol_colors, symbol_scales, symbol_numbers, symbols )
max_height = math.max(max_height, legend_height)

-- Set up box legend entries with various patterns.
for i=0,nlegend-1 do
  opt_array[i+1] = pl.PL_LEGEND_COLOR_BOX
  text[i+1] = string.format("%s %d", "Box Pattern", i)
  text_colors[i+1] = 2
  box_colors[i+1] = 2
  box_patterns[i+1] = i
  box_scales[i+1] = 0.8
  box_line_widths[i+1] = 1.
end

opt = opt_base
x = x + legend_width
pl.scol0a( 15, 32, 32, 32, 0.70 )
legend_width, legend_height = pl.legend(
  opt, position, x, y,
  0.1, 15, 1, 1, 0, 0, opt_array, 1.0, text_scale, 2.0,
  0.0, text_colors, text, 
  box_colors, box_patterns, box_scales, box_line_widths, 
  line_colors, line_styles, line_widths,
  symbol_colors, symbol_scales, symbol_numbers, symbols )
max_height = math.max(max_height, legend_height)

-- Set up box legend entries with various pattern line widths.
for i=0,nlegend-1 do
  opt_array[i+1] = pl.PL_LEGEND_COLOR_BOX
  text[i+1] = string.format("%s %d", "Box Line Width", i+1)
  text_colors[i+1] = 2
  box_colors[i+1] = 2
  box_patterns[i+1] = 3
  box_scales[i+1] = 0.8
  box_line_widths[i+1] = i+1
end

opt = opt_base
x = x + legend_width
pl.scol0a( 15, 32, 32, 32, 0.70 )
legend_width, legend_height = pl.legend(
  opt, position, x, y,
  0.1, 15, 1, 1, 0, 0, opt_array, 1.0, text_scale, 2.0,
  0.0, text_colors, text, 
  box_colors, box_patterns, box_scales, box_line_widths, 
  line_colors, line_styles, line_widths,
  symbol_colors, symbol_scales, symbol_numbers, symbols )
max_height = math.max(max_height, legend_height)

-- Set up line legend entries with various colours.
for i=0,nlegend-1 do
  opt_array[i+1] = pl.PL_LEGEND_LINE
  text[i+1] = string.format("%s %d", "Line Color", i+1)
  text_colors[i+1] = i+1
  line_colors[i+1] = i+1
  line_styles[i+1] = 1
  line_widths[i+1] = 1.
end

opt = opt_base
-- Use new origin
x = xstart
y = y + max_height
max_height = 0
pl.scol0a( 15, 32, 32, 32, 0.70 )
legend_width, legend_height = pl.legend(
  opt, position, x, y,
  0.1, 15, 1, 1, 0, 0, opt_array, 1.0, text_scale, 2.0,
  0.0, text_colors, text, 
  box_colors, box_patterns, box_scales, box_line_widths, 
  line_colors, line_styles, line_widths,
  symbol_colors, symbol_scales, symbol_numbers, symbols )
max_height = math.max(max_height, legend_height)

-- Set up line legend entries with various styles.
for i=0,nlegend-1 do
  opt_array[i+1] = pl.PL_LEGEND_LINE
  text[i+1] = string.format("%s %d", "Line Style", i+1)
  text_colors[i+1] = 2
  line_colors[i+1] = 2
  line_styles[i+1] = i+1
  line_widths[i+1] = 1.
end

opt = opt_base
x = x + legend_width
pl.scol0a( 15, 32, 32, 32, 0.70 )
legend_width, legend_height = pl.legend(
  opt, position, x, y,
  0.1, 15, 1, 1, 0, 0, opt_array, 1.0, text_scale, 2.0,
  0.0, text_colors, text, 
  box_colors, box_patterns, box_scales, box_line_widths, 
  line_colors, line_styles, line_widths,
  symbol_colors, symbol_scales, symbol_numbers, symbols )
max_height = math.max(max_height, legend_height)

-- Set up line legend entries with various widths.
for i=0,nlegend-1 do
  opt_array[i+1] = pl.PL_LEGEND_LINE
  text[i+1] = string.format("%s %d", "Line Width", i+1)
  text_colors[i+1] = 2
  line_colors[i+1] = 2
  line_styles[i+1] = 1
  line_widths[i+1] = i+1
end

opt = opt_base
x = x + legend_width
pl.scol0a( 15, 32, 32, 32, 0.70 )
legend_width, legend_height = pl.legend(
  opt, position, x, y,
  0.1, 15, 1, 1, 0, 0, opt_array, 1.0, text_scale, 2.0,
  0.0, text_colors, text, 
  box_colors, box_patterns, box_scales, box_line_widths, 
  line_colors, line_styles, line_widths,
  symbol_colors, symbol_scales, symbol_numbers, symbols )
max_height = math.max(max_height, legend_height)

pl.plend()
