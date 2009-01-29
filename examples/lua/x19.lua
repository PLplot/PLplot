--[[ $Id$

	Illustrates backdrop plotting of world, US maps.
	Contributed by Wesley Ebisuzaki.

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

--------------------------------------------------------------------------
-- mapform19
--
-- Defines specific coordinate transformation for example 19.
-- Not to be confused with mapform in src/plmap.c.
-- x[], y[] are the coordinates to be plotted.
--------------------------------------------------------------------------

function mapform19(n, x, y) 
  for i = 1, n do
    radius = 90 - y[i]
    xp = radius * math.cos(x[i] * math.pi / 180)
    yp = radius * math.sin(x[i] * math.pi / 180)
    x[i] = xp
    y[i] = yp
  end
end

--------------------------------------------------------------------------
-- main
--
-- Shows two views of the world map.
--------------------------------------------------------------------------

-- Parse and process command line arguments 

pl.parseopts(arg, pl.PL_PARSE_FULL)

-- Longitude (x) and latitude (y) 

miny = -70
maxy = 80

pl.init()

-- Cartesian plots 
-- Most of world 

minx = 190
maxx = 190+360

pl.col0(1)
pl.env(minx, maxx, miny, maxy, 1, -1)
pl.map(NULL, "usaglobe", minx, maxx, miny, maxy)

-- The Americas 

minx = 190
maxx = 340

pl.col0(1)
pl.env(minx, maxx, miny, maxy, 1, -1)
pl.map(NULL, "usaglobe", minx, maxx, miny, maxy)

-- Polar, Northern hemisphere 

minx = 0
maxx = 360

pl.env(-75., 75., -75., 75., 1, -1)
pl.map(mapform19,"globe", minx, maxx, miny, maxy)

pl.lsty(2)
pl.meridians(mapform19, 10, 10, 0, 360, -10, 80)
pl.plend()
