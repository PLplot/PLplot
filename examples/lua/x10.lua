-- $Id$

--	Window positioning demo.

-- initialise Lua bindings to PLplot
if string.sub(_VERSION,1,7)=='Lua 5.0' then
	lib=loadlib('./plplotluac.so','luaopen_plplotluac') or loadlib('plplotluac.dll','luaopen_plplotluac')
	assert(lib)()
else
	require('plplotluac')
end

----------------------------------------------------------------------------
-- main
--
-- Demonstrates absolute positioning of graphs on a page.
----------------------------------------------------------------------------

-- Parse and process command line arguments
pl.parseopts(arg, pl.PL_PARSE_FULL);

-- Initialize plplot
pl.init()

pl.adv(0)
pl.vpor(0.0, 1.0, 0.0, 1.0)
pl.wind(0.0, 1.0, 0.0, 1.0)
pl.box("bc", 0.0, 0, "bc", 0.0, 0)

pl.svpa(50.0, 150.0, 50.0, 100.0)
pl.wind(0.0, 1.0, 0.0, 1.0)
pl.box("bc", 0.0, 0, "bc", 0.0, 0)
pl.ptex(0.5, 0.5, 1.0, 0.0, 0.5, "BOX at (50,150,50,100)")
pl.plend()
