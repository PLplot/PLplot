-- $Id: $

--	Window positioning demo.

-- initialise Lua bindings to PLplot
if string.sub(_VERSION,1,7)=='Lua 5.0' then
	lib=loadlib('./plplotluac.so','luaopen_plplotluac') or loadlib('plplotluac.dll','luaopen_plplotluac')
	assert(lib)()
else
	require('plplotluac')
end
pl=plplotluac

----------------------------------------------------------------------------
-- main
--
-- Demonstrates absolute positioning of graphs on a page.
----------------------------------------------------------------------------

-- Parse and process command line arguments
--    (void) plparseopts(&argc, argv, PL_PARSE_FULL);

-- Initialize plplot
pl.plinit()

pl.pladv(0)
pl.plvpor(0.0, 1.0, 0.0, 1.0)
pl.plwind(0.0, 1.0, 0.0, 1.0)
pl.plbox("bc", 0.0, 0, "bc", 0.0, 0)

pl.plsvpa(50.0, 150.0, 50.0, 100.0)
pl.plwind(0.0, 1.0, 0.0, 1.0)
pl.plbox("bc", 0.0, 0, "bc", 0.0, 0)
pl.plptex(0.5, 0.5, 1.0, 0.0, 0.5, "BOX at (50,150,50,100)")
pl.plend()
