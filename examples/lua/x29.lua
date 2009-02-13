--[[ $Id$

       Sample plots using date / time formatting for axes

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



-- Plot a model diurnal cycle of temperature 
function plot1() 
  x = {}
  y = {}

  xerr1 = {}
  xerr2 = {}
  yerr1 = {}
  yerr2 = {}
  -- Data points every 10 minutes for 1 day 
  npts = 73

  xmin = 0
  xmax = 60*60*24    -- Number of seconds in a day 
  ymin = 10
  ymax = 20

  for i = 1, npts do
    x[i] = xmax*((i-1)/npts)
    y[i] = 15 - 5*math.cos(2*math.pi*((i-1)/npts))
    -- Set x error bars to +/- 5 minute 
    xerr1[i] = x[i]-60*5
    xerr2[i] = x[i]+60*5
    -- Set y error bars to +/- 0.1 deg C 
    yerr1[i] = y[i]-0.1
    yerr2[i] = y[i]+0.1
  end
  
  pl.adv(0)

  -- Rescale major ticks marks by 0.5 
  pl.smaj(0, 0.5)
  -- Rescale minor ticks and error bar marks by 0.5 
  pl.smin(0, 0.5)

  pl.vsta()
  pl.wind(xmin, xmax, ymin, ymax)

  -- Draw a box with ticks spaced every 3 hour in X and 1 degree C in Y. 
  pl.col0(1)
  -- Set time format to be hours:minutes 
  pl.timefmt("%H:%M")
  pl.box("bcnstd", 3*60*60, 3, "bcnstv", 1, 5)

  pl.col0(3)
  pl.lab("Time (hours:mins)", "Temperature (degC)", "@frPLplot Example 29 - Daily temperature")
  
  pl.col0(4)

  pl.line(x, y)
  pl.col0(2)
  pl.errx(xerr1, xerr2, y)
  pl.col0(3)
  pl.erry(x, yerr1, yerr2)

  -- Rescale major / minor tick marks back to default 
  pl.smin(0, 1) 
  pl.smaj(0, 1) 
end


-- Plot the number of hours of daylight as a function of day for a year 
function plot2() 
  x = {}
  y = {}

  -- Latitude for London 
  lat = 51.5

  npts = 365

  xmin = 0
  xmax = npts*60*60*24
  ymin = 0
  ymax = 24
  
  -- Formula for hours of daylight from 
  -- "A Model Comparison for Daylength as a Function of Latitude and 
  -- Day of the Year", 1995, Ecological Modelling, 80, pp 87-95. 
  for j = 1, npts do
    x[j] = (j-1)*60*60*24
    p = math.asin(0.39795*math.cos(0.2163108 + 2*math.atan(0.9671396*math.tan(0.00860*(j-1-186)))))
    d = 24 - (24/math.pi)*
      math.acos( (math.sin(0.8333*math.pi/180) + math.sin(lat*math.pi/180)*math.sin(p)) / 
	    (math.cos(lat*math.pi/180)*math.cos(p)) )
    y[j] = d
  end

  pl.col0(1)
  -- Set time format to be abbreviated month name followed by day of month 
  pl.timefmt("%b %d")
  pl.prec(1, 1)
  pl.env(xmin, xmax, ymin, ymax, 0, 40)


  pl.col0(3)
  pl.lab("Date", "Hours of daylight", "@frPLplot Example 29 - Hours of daylight at 51.5N")
  
  pl.col0(4)

  pl.line(x, y)
  
  pl.prec(0, 0)
end


function plot3()
  x = {}
  y = {}

  tstart = 1133395200
   
  npts = 62

  xmin = tstart
  xmax = xmin + npts*60*60*24
  ymin = 0
  ymax = 5
  
  for i = 1, npts do
    x[i] = xmin + (i-1)*60*60*24
    y[i] = 1 + math.sin(2*math.pi*(i-1)/7) + math.exp(math.min(i-1,npts-i+1)/31)
  end
  pl.adv(0)

  pl.vsta()
  pl.wind(xmin, xmax, ymin, ymax)

  pl.col0(1)
  -- Set time format to be ISO 8601 standard YYYY-MM-DD. Note that this is
  --equivalent to %f for C99 compliant implementations of strftime. 
  pl.timefmt("%Y-%m-%d")
  -- Draw a box with ticks spaced every 14 days in X and 1 hour in Y. 
  pl.box("bcnstd", 14*24*60*60,14, "bcnstv", 1, 4)

  pl.col0(3)
  pl.lab("Date", "Hours of television watched", "@frPLplot Example 29 - Hours of television watched in Dec 2005 / Jan 2006")
  
  pl.col0(4)

  -- Rescale symbol size (used by plpoin) by 0.5 
  pl.ssym(0, 0.5)
  pl.poin(x, y, 2)
  pl.line(x, y)
end


----------------------------------------------------------------------------
-- main
-- 
-- Draws several plots which demonstrate the use of date / time formats for
-- the axis labels.
-- Time formatting is done using the system strftime routine. See the 
-- documentation of this for full details of the available formats.
--
-- 1) Plotting temperature over a day (using hours / minutes)
-- 2) Plotting 
--
-- Note: Times are stored as seconds since the epoch (usually 1st Jan 1970). 
-- 
----------------------------------------------------------------------------

-- Parse command line arguments 
pl.parseopts(arg, pl.PL_PARSE_FULL)

-- Initialize plplot 
pl.init()

-- Change the escape character to a '@' instead of the default '#' 
pl.sesc('@')

plot1()

plot2()

plot3()

-- Don't forget to call plend() to finish off! 
pl.plend()
