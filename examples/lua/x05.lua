-- $Id: $

--	Histogram demo.

-- initialise Lua bindings to PLplot
if string.sub(_VERSION,1,7)=='Lua 5.0' then
	lib=loadlib('./plplotluac.so','luaopen_plplotluac') or loadlib('plplotluac.dll','luaopen_plplotluac')
	assert(lib)()
else
	require('plplotluac')
end
pl=plplotluac

--------------------------------------------------------------------------
-- main
--
-- Draws a histogram from sample data.
--------------------------------------------------------------------------

NPTS=2047
data = {}

-- Parse and process command line arguments
--    (void) plparseopts(&argc, argv, PL_PARSE_FULL);

-- Initialize plplot
pl.plinit()

-- Fill up data points
delta = 2.0*math.pi/NPTS
for i=1, NPTS do
  data[i] = math.sin((i-1)*delta)
end

pl.plcol0(1)
pl.plhist(data, -1.1, 1.1, 44, 0)
pl.plcol0(2)
pl.pllab("#frValue", "#frFrequency", 
         "#frPLplot Example 5 - Probability function of Oscillator")

pl.plend()
