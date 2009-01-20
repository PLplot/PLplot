-- $Id$

--	Histogram demo.

-- initialise Lua bindings to PLplot
if string.sub(_VERSION,1,7)=='Lua 5.0' then
	lib=loadlib('./plplotluac.so','luaopen_plplotluac') or loadlib('plplotluac.dll','luaopen_plplotluac')
	assert(lib)()
else
	require('plplotluac')
end

--------------------------------------------------------------------------
-- main
--
-- Draws a histogram from sample data.
--------------------------------------------------------------------------

NPTS=2047
data = {}

-- Parse and process command line arguments
pl.parseopts(arg, pl.PL_PARSE_FULL);

-- Initialize plplot
pl.init()

-- Fill up data points
delta = 2.0*math.pi/NPTS
for i=1, NPTS do
  data[i] = math.sin((i-1)*delta)
end

pl.col0(1)
pl.hist(data, -1.1, 1.1, 44, 0)
pl.col0(2)
pl.lab("#frValue", "#frFrequency", 
         "#frPLplot Example 5 - Probability function of Oscillator")

pl.plend()
