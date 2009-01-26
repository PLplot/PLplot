--[[ $Id: $

 	 3-d plot demo.

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



-- initialise Lua bindings to PLplot
if string.sub(_VERSION,1,7)=='Lua 5.0' then
	lib=loadlib('./plplotluac.so','luaopen_plplotluac') or loadlib('plplotluac.dll','luaopen_plplotluac')
	assert(lib)()
else
	require('plplotluac')
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
}

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

title[] = {
    "#frPLplot Example 8 - Alt=60, Az=30",
    "#frPLplot Example 8 - Alt=20, Az=60",
}


PLFLT *x, *y, **z
PLFLT xx, yy, r
clevel = {}
nlevel = {}
rosen=1

-- Parse and process command line arguments 
pl.parseopts(arg, pl.PL_PARSE_FULL)
if sombrero ~= 0 then rosen=0 end

-- Initialize plplot 
pl.init()

-- Allocate data structures 

  x = (PLFLT *) calloc(XPTS, sizeof(PLFLT))
  y = (PLFLT *) calloc(YPTS, sizeof(PLFLT))

  plAlloc2dGrid(&z, XPTS, YPTS)

  for (i = 0 i < XPTS i++) {
    x[i] = ((double) (i - (XPTS / 2)) / (double) (XPTS / 2))
    if (rosen)
      x[i] *=  1.5
  }

  for (i = 0 i < YPTS i++) {
    y[i] = (double) (i - (YPTS / 2)) / (double) (YPTS / 2)
    if (rosen)
      y[i] += 0.5
  }

  for (i = 0 i < XPTS i++) {
    xx = x[i]
    for (j = 0 j < YPTS j++) {
      yy = y[j]
      if (rosen) {
	z[i][j] = pow(1. - xx, 2.) + 100. * pow(yy - pow(xx, 2.), 2.)
	-- The log argument may be zero for just the right grid.  
	if (z[i][j] > 0.)
	  z[i][j] = log(z[i][j])
	else
	  z[i][j] = -5. -- -MAXFLOAT would mess-up up the scale 
      }
      else {
	r = sqrt(xx * xx + yy * yy)
	z[i][j] = exp(-r * r) * cos(2.0 * M_PI * r)
      }
    }
  }

  plMinMax2dGrid(z, XPTS, YPTS, &zmax, &zmin)
  step = (zmax-zmin)/(nlevel+1)
  for (i=0 i<nlevel i++)
    clevel[i] = zmin + step + step*i

  pllightsource(1.,1.,1.)

  for (k = 0 k < 2 k++) {
    for (ifshade = 0 ifshade < 4 ifshade++) {
      pladv(0)
      plvpor(0.0, 1.0, 0.0, 0.9)
      plwind(-1.0, 1.0, -0.9, 1.1)
      plcol0(3)
      plmtex("t", 1.0, 0.5, 0.5, title[k])
      plcol0(1)
      if (rosen)
	plw3d(1.0, 1.0, 1.0, -1.5, 1.5, -0.5, 1.5, zmin, zmax, alt[k], az[k])
      else
	plw3d(1.0, 1.0, 1.0, -1.0, 1.0, -1.0, 1.0, zmin, zmax, alt[k], az[k])

      plbox3("bnstu", "x axis", 0.0, 0,
	     "bnstu", "y axis", 0.0, 0,
	     "bcdmnstuv", "z axis", 0.0, 0)
      plcol0(2)

      if (ifshade == 0) { -- diffuse light surface plot 
	cmap1_init(1)
	plsurf3d(x, y, z, XPTS, YPTS, 0, NULL, 0)
      } else if (ifshade == 1) { -- magnitude colored plot 
	cmap1_init(0)
	plsurf3d(x, y, z, XPTS, YPTS, MAG_COLOR, NULL, 0)
      } else if (ifshade == 2) { --  magnitude colored plot with faceted squares 
	cmap1_init(0)
	plsurf3d(x, y, z, XPTS, YPTS, MAG_COLOR | FACETED, NULL, 0)
      } else {                    -- magnitude colored plot with contours 
	cmap1_init(0)
	plsurf3d(x, y, z, XPTS, YPTS, MAG_COLOR | SURF_CONT | BASE_CONT, clevel, nlevel)
      }
    }
  }

-- Clean up 

  free((void *) x)
  free((void *) y)
  plFree2dGrid(z, XPTS, YPTS)

  plend()

  exit(0)
}
