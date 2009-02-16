--[[ $Id$

	3-d plot demo.

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


-- bitwise or operator from http://lua-users.org/wiki/BaseSixtyFour
-- (c) 2006-2008 by Alex Kloss
-- licensed under the terms of the LGPL2

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


----------------------------------------------------------------------------
-- cmap1_init1
--
-- Initializes color map 1 in HLS space.
-- Basic grayscale variation from half-dark (which makes more interesting
-- looking plot compared to dark) to light.
-- An interesting variation on this:
--	s[1] = 1.0
----------------------------------------------------------------------------

function cmap1_init(gray)
  i = { 0, 1 } -- left and right boundary 

  if gray ~= 0 then
    h = { 0, 0 } -- hue -- low: red (arbitrary if s=0), high: red (arbitrary if s=0) 
    l = { 0.5, 1 }		-- lightness -- low: half-dark, high: light 
    s = { 0, 0 }		-- minimum saturation 
  else
    h = { 240, 0 } -- blue -> green -> yellow -> red 
    l = { 0.6, 0.6 }
    s = { 0.8, 0.8 }
  end

  pl.scmap1n(256)
  pl.scmap1l(0, i, h, l, s)
end


----------------------------------------------------------------------------
-- main
--
-- Does a series of 3-d plots for a given data set, with different
-- viewing options in each plot.
----------------------------------------------------------------------------

XPTS = 35		-- Data points in x 
YPTS = 46		-- Data points in y 
LEVELS = 10

alt = { 60, 20 }
az  = { 30, 60 }

title = {
    "#frPLplot Example 8 - Alt=60, Az=30",
    "#frPLplot Example 8 - Alt=20, Az=60"
}

clevel = {}
nlevel = LEVELS
rosen = 1
sombrero = 0

-- Parse and process command line arguments 
pl.parseopts(arg, pl.PL_PARSE_FULL)
if sombrero ~= 0 then rosen=0 end

-- Initialize plplot 
pl.init()

-- Allocate data structures 
x = {}
y = {}
z = {}

for i=1, XPTS do
  x[i] = (i-1-math.floor(XPTS/2)) / math.floor(XPTS/2)
  if rosen~=0 then x[i]=x[i]*1.5 end 
end

for i=1, YPTS do
  y[i] = (i-1-math.floor(YPTS/2)) / math.floor(YPTS/2)
  if rosen~=0 then y[i]=y[i]+0.5 end
end

for i=1, XPTS do
  xx = x[i]
  z[i]= {}
  for j=1, YPTS do
    yy = y[j]
    if rosen~=0 then
      z[i][j] = (1-xx)^2 + 100*(yy-xx^2)^2
      -- The log argument may be zero for just the right grid.  
      if z[i][j] > 0 then
        z[i][j] = math.log(z[i][j])
      else
        z[i][j] = -5   -- MAXFLOAT would mess-up up the scale 
      end
    else
      r = math.sqrt(xx^2 + yy^2)
      z[i][j] = math.exp(-r^2) * math.cos(2*math.pi*r)
    end
  end
end

zmax, zmin = pl.MinMax2dGrid(z)
step = (zmax-zmin)/(nlevel+1)
for i=1, nlevel do
  clevel[i] = zmin + step + step*(i-1)
end

pl.lightsource(1, 1, 1)

for k=1, 2 do
  for ifshade = 1, 4 do
    pl.adv(0)
    pl.vpor(0, 1, 0, 0.9)
    pl.wind(-1, 1, -0.9, 1.1)
    pl.col0(3)
    pl.mtex("t", 1, 0.5, 0.5, title[k])
    pl.col0(1)
    if rosen~=0 then
      pl.w3d(1, 1, 1, -1.5, 1.5, -0.5, 1.5, zmin, zmax, alt[k], az[k])
    else
      pl.w3d(1, 1, 1, -1, 1, -1, 1, zmin, zmax, alt[k], az[k])
    end

    pl.box3("bnstu", "x axis", 0, 0,
           "bnstu", "y axis", 0, 0,
           "bcdmnstuv", "z axis", 0, 0)
    pl.col0(2)

    if ifshade==1 then -- diffuse light surface plot 
      cmap1_init(1)
      pl.surf3d(x, y, z, 0, clevel)
    end
     
    if ifshade==2 then -- magnitude colored plot 
      cmap1_init(0)
      pl.surf3d(x, y, z, pl.MAG_COLOR, {})
    end
    
    if ifshade==3 then --  magnitude colored plot with faceted squares 
      cmap1_init(0)
      pl.surf3d(x, y, z, lor(pl.MAG_COLOR, pl.FACETED), {})
    end
    
    if ifshade==4 then  -- magnitude colored plot with contours 
      cmap1_init(0)
      pl.surf3d(x, y, z, lor(lor(pl.MAG_COLOR, pl.SURF_CONT), pl.BASE_CONT), clevel)
    end
  end
end

-- Clean up 
pl.plend()
