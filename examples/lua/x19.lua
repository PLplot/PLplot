-- $Id$

--	Illustrates backdrop plotting of world, US maps.
--	Contributed by Wesley Ebisuzaki.


-- initialise Lua bindings to PLplot
if string.sub(_VERSION,1,7)=='Lua 5.0' then
	lib=loadlib('./plplotluac.so','luaopen_plplotluac') or loadlib('plplotluac.dll','luaopen_plplotluac')
	assert(lib)()
else
	require('plplotluac')
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
