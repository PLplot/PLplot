-- $Id$

--	Font demo.


-- initialise Lua bindings to pl.pl.ot
if string.sub(_VERSION,1,7)=='Lua 5.0' then
	lib=loadlib('./plplotluac.so','luaopen_plplotluac') or loadlib('plplotluac.dll','luaopen_plplotluac')
	assert(lib)()
else
	require('plplotluac')
end


----------------------------------------------------------------------------
-- main
--
-- Displays the entire "plpoin" symbol (font) set.
----------------------------------------------------------------------------

-- Parse and process command line arguments 
pl.parseopts(arg, pl.PL_PARSE_FULL)

-- Initialize plplot 
pl.init()

pl.adv(0)

-- Set up viewport and window 
pl.col0(2)
pl.vpor(0.1, 1, 0.1, 0.9)
pl.wind(0, 1, 0, 1.3)

-- Draw the grid using plbox 
pl.box("bcg", 0.1, 0, "bcg", 0.1, 0)

-- Write the digits below the frame 
pl.col0(15)
for i=0, 9 do
  pl.mtex("b", 1.5, (0.1 * i + 0.05), 0.5, tostring(i))
end

k = 0
x = {}
y ={}
for i = 0, 12 do
  -- Write the digits to the left of the frame 
	pl.mtex("lv", 1, (1 - (2 * i + 1) / 26), 1, tostring(10*i))
	for j = 0, 9 do
    x[1] = 0.1 * j + 0.05
    y[1] = 1.25 - 0.1 * i

    -- Display the symbols
    if k < 128 then
      pl.poin(x, y, k)
    end
    k = k + 1
  end
end
  
pl.mtex("t", 1.5, 0.5, 0.5, "PLplot Example 6 - plpoin symbols")
pl.plend()
