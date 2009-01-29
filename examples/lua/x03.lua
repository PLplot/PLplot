--[[ $Id$

	Polar plot demo.

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

----------------------------------------------------------------------------
-- main
--
-- Generates polar plot, with 1-1 scaling.
----------------------------------------------------------------------------

x0 = {}
y0 = {}
x = {}
y = {}

dtr = math.pi/180
for i = 1, 361 do
  x0[i] = math.cos(dtr * (i-1))
  y0[i] = math.sin(dtr * (i-1))
end

-- Parse and process command line arguments 

pl.parseopts(arg, pl.PL_PARSE_FULL)

-- Set orientation to landscape - note not all device drivers 
-- support this, in particular most interactive drivers do not 
pl.sori(1)

-- Initialize plplot 
pl.init()

-- Set up viewport and window, but do not draw box 
pl.env(-1.3, 1.3, -1.3, 1.3, 1, -2)
for i = 1, 10 do
	for j = 1, 361 do
	    x[j] = 0.1 * i * x0[j]
	    y[j] = 0.1 * i * y0[j]
	end

  -- Draw circles for polar grid 
  pl.line(x, y)
end


pl.col0(2)
for i=0, 11 do
	theta = 30 * i
	dx = math.cos(dtr * theta)
	dy = math.sin(dtr * theta)

  -- Draw radial spokes for polar grid 

	pl.join(0, 0, dx, dy)

  -- Write labels for angle 
	if theta < 9.99 then
	  offset = 0.45
	else
    if theta < 99.9 then
      offset = 0.30
    else 
      offset = 0.15
    end
	end

  -- Slightly off zero to avoid floating point logic flips at 90 and 270 deg. 
	if dx >= -0.00001 then
	    pl.ptex(dx, dy, dx, dy, -offset, tostring(math.floor(theta)))
	else
	    pl.ptex(dx, dy, -dx, -dy, 1.+offset, tostring(math.floor(theta)))
  end
end

-- Draw the graph 
for i=1, 361 do
  r = math.sin(dtr * (5 * i-1))
  x[i] = x0[i] * r
  y[i] = y0[i] * r
end
pl.col0(3)
pl.line(x, y)

pl.col0(4)
pl.mtex("t", 2, 0.5, 0.5, "#frPLplot Example 3 - r(#gh)=sin 5#gh")

-- Close the plot at end 
pl.plend()
