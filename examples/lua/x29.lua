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


function plot4() 
  -- TAI-UTC (seconds) as a function of time.
  -- Use Besselian epochs as the continuous time interval just to prove
  -- this does not introduce any issues.
  
  x = {}
  y = {}

  -- Use the definition given in http://en.wikipedia.org/wiki/Besselian_epoch
  -- B = 1900. + (JD -2415020.31352)/365.242198781 
  -- ==> (as calculated with aid of "bc -l" command)
  -- B = (MJD + 678940.364163900)/365.242198781
  -- ==>
  -- MJD = B*365.24219878 - 678940.364163900
  scale = 365.242198781
  offset1 = -678940
  offset2 = -0.3641639
  pl.configtime(scale, offset1, offset2, 0, 0, 0, 0, 0, 0, 0, 0.)

  for kind = 0, 6 do
    if kind == 0 then
      xmin = pl.ctime(1950, 0, 2, 0, 0, 0)
      xmax = pl.ctime(2020, 0, 2, 0, 0, 0)
      npts = 70*12 + 1
      ymin = 0
      ymax = 36
      time_format = "%Y%"
      if_TAI_time_format = 1
      title_suffix = "from 1950 to 2020"
      xtitle = "Year"
      xlabel_step = 10
    end
    if kind==1 or kind==2 then
      xmin = pl.ctime(1961, 7, 1, 0, 0, 1.64757-0.20)
      xmax = pl.ctime(1961, 7, 1, 0, 0, 1.64757+0.20)
      npts = 1001
      ymin = 1.625
      ymax = 1.725
      time_format = "%S%2%"
      title_suffix = "near 1961-08-01 (TAI)"
      xlabel_step = 0.05/(scale*86400)
      if kind==1 then
        if_TAI_time_format = 1
        xtitle = "Seconds (TAI)"
      else 
        if_TAI_time_format = 0
        xtitle = "Seconds (TAI) labelled with corresponding UTC"
      end
    end
    if kind==3 or kind==4 then
      xmin = pl.ctime(1963, 10, 1, 0, 0, 2.6972788-0.20)
      xmax = pl.ctime(1963, 10, 1, 0, 0, 2.6972788+0.20)
      npts = 1001
      ymin = 2.55
      ymax = 2.75
      time_format = "%S%2%"
      title_suffix = "near 1963-11-01 (TAI)"
      xlabel_step = 0.05/(scale*86400)
      if kind==3 then
				if_TAI_time_format = 1
				xtitle = "Seconds (TAI)"
      else
				if_TAI_time_format = 0
				xtitle = "Seconds (TAI) labelled with corresponding UTC"
      end
    end
    if kind==5 or kind==6 then
      xmin = pl.ctime(2009, 0, 1, 0, 0, 34-5)
      xmax = pl.ctime(2009, 0, 1, 0, 0, 34+5)
      npts = 1001
      ymin = 32.5
      ymax = 34.5
      time_format = "%S%2%"
      title_suffix = "near 2009-01-01 (TAI)"
      xlabel_step = 1/(scale*86400)
      if kind==5 then
        if_TAI_time_format = 1
        xtitle = "Seconds (TAI)"
      else
        if_TAI_time_format = 0
        xtitle = "Seconds (TAI) labelled with corresponding UTC"
      end
    end

    for i = 1, npts do 
      x[i] = xmin + (i-1)*(xmax-xmin)/(npts-1)
      pl.configtime(scale, offset1, offset2, 0, 0, 0, 0, 0, 0, 0, 0)
      tai = x[i]
      tai_year, tai_month, tai_day, tai_hour, tai_min, tai_sec = pl.btime(tai)
      pl.configtime(scale, offset1, offset2, 2, 0, 0, 0, 0, 0, 0, 0)
      utc_year, utc_month, utc_day, utc_hour, utc_min, utc_sec = pl.btime(tai)
      pl.configtime(scale, offset1, offset2, 0, 0, 0, 0, 0, 0, 0, 0.)
      utc = pl.ctime(utc_year, utc_month, utc_day, utc_hour, utc_min, utc_sec)
      y[i]=(tai-utc)*scale*86400.
    end

    pl.adv(0)
    pl.vsta()
    pl.wind(xmin, xmax, ymin, ymax)
    pl.col0(1)
    if if_TAI_time_format ~= 0 then
      pl.configtime(scale, offset1, offset2, 0, 0, 0, 0, 0, 0, 0, 0)
    else
      pl.configtime(scale, offset1, offset2, 2, 0, 0, 0, 0, 0, 0, 0)
    end
    pl.timefmt(time_format)
    pl.box("bcnstd", xlabel_step, 0, "bcnstv", 0., 0)
    pl.col0(3)
    title = "@frPLplot Example 29 - TAI-UTC " .. title_suffix
    pl.lab(xtitle, "TAI-UTC (sec)", title)
    
    pl.col0(4)
    
    pl.line(x, y)
  end
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

plot4()

-- Don't forget to call plend() to finish off! 
pl.plend()
