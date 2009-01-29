--[[ $Id$

	Bar chart demo.

  Copyright (C) 2008  Werner Smekal

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

function pl.fbox(x0, y0)
  x = {}
  y = {}

  x[1] = x0;
  y[1] = 0.;
  x[2] = x0;
  y[2] = y0;
  x[3] = x0 + 1.;
  y[3] = y0;
  x[4] = x0 + 1.;
  y[4] = 0.;
  pl.fill(x, y);
  pl.col0(1);
  pl.lsty(1);
  pl.line(x, y);
end

--------------------------------------------------------------------------
-- main
--
-- Does a simple bar chart, using color fill.  If color fill is
-- unavailable, pattern fill is used instead (automatic).
--------------------------------------------------------------------------

y0 = {}

-- Parse and process command line arguments 

pl.parseopts(arg, pl.PL_PARSE_FULL);

-- Initialize plplot 
pl.init()

pl.adv(0)
pl.vsta()
pl.wind(1980.0, 1990.0, 0.0, 35.0)
pl.box("bc", 1.0, 0, "bcnv", 10.0, 0)
pl.col0(2)
pl.lab("Year", "Widget Sales (millions)", "#frPLplot Example 12")

y0[1] = 5
y0[2] = 15
y0[3] = 12
y0[4] = 24
y0[5] = 28
y0[6] = 30
y0[7] = 20
y0[8] = 8
y0[9] = 12
y0[10] = 3

for i=1, 10 do
	pl.col0(i);
	pl.psty(0);
	pl.fbox((1980. + i - 1), y0[i]);
	pl.ptex((1980. + i - .5), (y0[i] + 1.), 1.0, 0.0, .5, tostring(y0[i]));
	pl.mtex("b", 1.0, (i * .1 - .05), 0.5, tostring(1980+i-1));
end

pl.plend();
