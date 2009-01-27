-- $Id$

-- 3-d line and point plot demo.  Adapted from x08c.c.


-- initialise Lua bindings to PLplot
if string.sub(_VERSION,1,7)=='Lua 5.0' then
	lib=loadlib('./plplotluac.so','luaopen_plplotluac') or loadlib('plplotluac.dll','luaopen_plplotluac')
	assert(lib)()
else
	require('plplotluac')
end


function test_poly(k)
  draw= { { 1, 1, 1, 1 },
          { 1, 0, 1, 0 },
          { 0, 1, 0, 1 },
          { 1, 1, 0, 0 } }

  x = {}
  y = {}
  z = {}

  pl.adv(0)
  pl.vpor(0, 1, 0, 0.9)
  pl.wind(-1, 1, -0.9, 1.1)
  pl.col0(1)
  pl.w3d(1, 1, 1, -1, 1, -1, 1, -1, 1, alt[k], az[k])
  pl.box3("bnstu", "x axis", 0.0, 0,
          "bnstu", "y axis", 0.0, 0,
          "bcdmnstuv", "z axis", 0.0, 0)

  pl.col0(2)

  -- x = r math.sin(phi) math.cos(theta)
  -- y = r math.sin(phi) math.sin(theta)
  -- z = r math.cos(phi)
  -- r = 1 :=)

  for i=0, 19 do
    for j=0, 19 do
	    x[0] = math.sin( math.pi*j/20.1 ) * math.cos( 2*math.pi*i/20 )
	    y[0] = math.sin( math.pi*j/20.1 ) * math.sin( 2*math.pi*i/20 )
	    z[0] = math.cos( math.pi*j/20.1 )
	    
	    x[1] = math.sin( math.pi*(j+1)/20.1 ) * math.cos( 2*math.pi*i/20 )
	    y[1] = math.sin( math.pi*(j+1)/20.1 ) * math.sin( 2*math.pi*i/20 )
	    z[1] = math.cos( math.pi*(j+1)/20.1 )
	    
	    x[2] = math.sin( math.pi*(j+1)/20.1 ) * math.cos( 2*math.pi*(i+1)/20 )
	    y[2] = math.sin( math.pi*(j+1)/20.1 ) * math.sin( 2*math.pi*(i+1)/20 )
	    z[2] = math.cos( math.pi*(j+1)/20.1 )
	    
	    x[3] = math.sin( math.pi*j/20.1 ) * math.cos( 2*math.pi*(i+1)/20 )
	    y[3] = math.sin( math.pi*j/20.1 ) * math.sin( 2*math.pi*(i+1)/20 )
	    z[3] = math.cos( math.pi*j/20.1 )
	    
	    x[4] = math.sin( math.pi*j/20.1 ) * math.cos( 2*math.pi*i/20 )
	    y[4] = math.sin( math.pi*j/20.1 ) * math.sin( 2*math.pi*i/20 )
	    z[4] = math.cos( math.pi*j/20.1 )

	    pl.poly3( x, y, z, draw[k], 1 )
    end
  end

  pl.col0(3)
  pl.mtex("t", 1, 0.5, 0.5, "unit radius sphere" )
end


----------------------------------------------------------------------------
-- main
--
-- Does a series of 3-d plots for a given data set, with different
-- viewing options in each plot.
----------------------------------------------------------------------------

NPTS = 1000
opt = { 1, 0, 1, 0 }
alt = { 20, 35, 50, 65 }
az  = { 30, 40, 50, 60 }


-- Parse and process command line arguments 
pl.parseopts(arg, pl.PL_PARSE_FULL)

-- Initialize plplot 
pl.init()

for k=1, 4 do
	test_poly(k)
end

x = {}
y = {}
z = {}

-- From the mind of a sick and twisted physicist... 
for i=1, NPTS do
  z[i] = -1 + 2 * (i-1) / NPTS

  -- Pick one ... 
  --	r  = 1 - (i-1) / NPTS
  r = z[i]

  x[i] = r * math.cos( 2 * math.pi * 6 * (i-1) / NPTS )
  y[i] = r * math.sin( 2 * math.pi * 6 * (i-1) / NPTS )
end

for k=1, 4 do
	pl.adv(0)
	pl.vpor(0, 1, 0, 0.9)
	pl.wind(-1, 1, -0.9, 1.1)
	pl.col0(1)
	pl.w3d(1, 1, 1, -1, 1, -1, 1, -1, 1, alt[k], az[k])
	pl.box3("bnstu", "x axis", 0, 0,
	       "bnstu", "y axis", 0, 0,
	       "bcdmnstuv", "z axis", 0, 0)

	pl.col0(2)

	if opt[k]~=0 then 
    pl.line3( x, y, z )
	else
    pl.poin3( x, y, z, 1 )
  end

	pl.col0(3)
	pl.mtex("t", 1.0, 0.5, 0.5, "#frPLplot Example 18 - Alt=" .. alt[k] .. ", Az=" .. az[k])
end

pl.plend()
