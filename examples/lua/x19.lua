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

function map_transform(x,y)
  radius = 90 - y
  xt = radius * math.cos(x * math.pi / 180)
  yt = radius * math.sin(x * math.pi / 180)
  
  return xt, yt
end

--------------------------------------------------------------------------
-- mapform19
--
-- Defines specific coordinate transformation for example 19.
-- Not to be confused with mapform in src/plmap.c.
-- x[], y[] are the coordinates to be plotted.
--------------------------------------------------------------------------

function mapform19(n, x, y) 
  for i = 1, n do
    x[i], y[i] = map_transform(x[i], y[i])
  end
  
  return x, y
end

-- "Normalize" longitude values so that they always fall between
-- -180.0 and 180.0
function normalize_longitude(lon)
	if lon>=-180 and lon<=180 then
		return lon;
	else
			times = math.floor((math.abs(lon)+180)/360)
			if lon<0 then
				return lon+360*times
			else 
				return lon-360*times
			end
	end
end

-- A custom axis labeling function for longitudes and latitudes.
function geolocation_labeler(axis, value) 
	if axis==pl.PL_Y_AXIS then
		label_val = value
		if label_val>0 then
			direction_label = " N"
		else
			if label_val<0 then
				direction_label = " S"
			else 
				direction_label = "Eq"
			end
		end
	else
		if axis==pl.PL_X_AXIS then
			label_val = normalize_longitude(value);
			if label_val>0 then
				direction_label = " E"
			else
				if label_val<0 then
					direction_label = " W"
				else
					direction_label = ""
				end
			end
		end
	end
	
	if axis==pl.PL_Y_AXIS and value==0 then
		-- A special case for the equator
		label = direction_label
	else
		label = math.abs(label_val) .. direction_label
	end
	
	return label
end

--------------------------------------------------------------------------
-- main
--
-- Shows two views of the world map.
--------------------------------------------------------------------------

-- Parse and process command line arguments 

x = {}
y = {}

pl.parseopts(arg, pl.PL_PARSE_FULL)

-- Longitude (x) and latitude (y) 

miny = -70
maxy = 80

pl.init()

-- Cartesian plots 
-- Most of world 

minx = 190
maxx = 190+360

-- Setup a custom latitude and longitude-based scaling function.
pl.slabelfunc("geolocation_labeler");

pl.col0(1)
pl.env(minx, maxx, miny, maxy, 1, 70)
pl.map(nil, "usaglobe", minx, maxx, miny, maxy)

-- The Americas 

minx = 190
maxx = 340

pl.col0(1)
pl.env(minx, maxx, miny, maxy, 1, 70)
pl.map(nil, "usaglobe", minx, maxx, miny, maxy)

-- Clear the labeling function
pl.slabelfunc(nil);

-- Polar, Northern hemisphere 

minx = 0
maxx = 360

pl.env(-75., 75., -75., 75., 1, -1)
pl.map("mapform19", "globe", minx, maxx, miny, maxy)

pl.lsty(2)
pl.meridians("mapform19", 10, 10, 0, 360, -10, 80)

-- Polar, Northern hemisphere, this time with a PLplot-wide transform

minx = 0
maxx = 360

pl.stransform( "map_transform" )

pl.lsty( 1 )
pl.env( -75., 75., -75., 75., 1, -1 )
-- No need to set the map transform here as the global transform will be
-- used.
pl.map( nil, "globe", minx, maxx, miny, maxy )

pl.lsty( 2 );
pl.meridians( nil, 10.0, 10.0, 0.0, 360.0, -10.0, 80.0 )

-- Show Baltimore, MD on the map
pl.col0( 2 )
pl.ssym( 0.0, 2.0 )
x[1] = -76.6125
y[1] = 39.2902778
pl.poin( x, y, 18 )
pl.ssym( 0.0, 1.0 )
pl.ptex( -76.6125, 43.0, 0.0, 0.0, 0.0, "Baltimore, MD" )

-- For C, this is how the global transform is cleared
pl.stransform( );

pl.plend()
