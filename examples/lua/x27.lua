--[[ $Id$

	Drawing "spirograph" curves - epitrochoids, cycolids, roulettes

   Copyright (C) 2009  Werner Smekal

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


function cycloid()
  -- TODO 
end


function spiro( params )
  NPNT = 20000
  xcoord = {}
  ycoord = {}

  -- Fill the coordinates 
  windings = params[4] 
  steps    = NPNT/windings 
  dphi     = 8*math.acos(-1)/steps 

  xmin = 0  -- This initialisation is safe! 
  xmax = 0 
  ymin = 0 
  ymax = 0 

  for i = 1, windings*steps+1 do
    phi       = (i-1) * dphi 
    phiw      = (params[1]-params[2])/params[2]*phi 
    xcoord[i] = (params[1]-params[2])*math.cos(phi) + params[3]*math.cos(phiw) 
    ycoord[i] = (params[1]-params[2])*math.sin(phi) - params[3]*math.sin(phiw) 

    if xmin>xcoord[i] then xmin = xcoord[i] end 
    if xmax<xcoord[i] then xmax = xcoord[i] end
    if ymin>ycoord[i] then ymin = ycoord[i] end
    if ymax<ycoord[i] then ymax = ycoord[i] end
  end

  if (xmax-xmin)>(ymax-ymin) then
    scale = xmax - xmin 
  else
    scale = ymax - ymin 
  end
  xmin = -0.65*scale 
  xmax =  0.65*scale 
  ymin = -0.65*scale 
  ymax =  0.65*scale 

  pl.wind(xmin, xmax, ymin, ymax)

  pl.col0(1)
  pl.line(xcoord, ycoord ) 
end


----------------------------------------------------------------------------
-- main
--
-- Generates two kinds of plots:
--   - construction of a cycloid (animated)
--   - series of epitrochoids and hypotrochoids
----------------------------------------------------------------------------

-- R, r, p, N 
params = {
  { 21.0,  7.0,  7.0,  3.0 },  -- Deltoid 
  { 21.0,  7.0, 10.0,  3.0 },
  { 21.0, -7.0, 10.0,  3.0 },
  { 20.0,  3.0,  7.0, 20.0 },
  { 20.0,  3.0, 10.0, 20.0 },
  { 20.0, -3.0, 10.0, 20.0 },
  { 20.0, 13.0,  7.0, 20.0 },
  { 20.0, 13.0, 20.0, 20.0 },
  { 20.0,-13.0, 20.0, 20.0 } } 

-- plplot initialization 

-- Parse and process command line arguments 
pl.parseopts(arg, pl.PL_PARSE_FULL)

-- Initialize plplot 
pl.init()

-- Illustrate the construction of a cycloid 
cycloid()

-- Loop over the various curves
-- First an overview, then all curves one by one

pl.ssub(3, 3)  -- Three by three window 

for i = 1, 9 do
  pl.adv(0) 
  pl.vpor(0, 1, 0, 1) 
  spiro(params[i]) 
end

pl.adv(0) 
pl.ssub(1, 1)  -- One window per curve 

for i = 1, 9 do
  pl.adv(0) 
  pl.vpor(0, 1, 0, 1) 
  spiro(params[i]) 
end

-- Don't forget to call plend() to finish off! 
pl.plend()
