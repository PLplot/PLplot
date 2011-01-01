## $Id$
##
## Sample plots using date / time formatting for axes
##
## Copyright (C) 2007, 2008 Andrew Ross 
##
##
## This file is part of PLplot.
##
## PLplot is free software; you can redistribute it and/or modify
## it under the terms of the GNU General Library Public License as published
## by the Free Software Foundation; either version 2 of the License, or
## (at your option) any later version.
##
## PLplot is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
## GNU Library General Public License for more details.
##
## You should have received a copy of the GNU Library General Public License
## along with PLplot; if not, write to the Free Software
## Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
##

1;

## 

function ix29c

  ## Parse command line arguments */
  ## plparseopts(&argc, argv, PL_PARSE_FULL);

  ## Initialize plplot
  plinit();
 
  plsesc('@');

  plot1();

  plot2();

  plot3();
  
  plot4();

  ## Clean up
  plend1();

endfunction



## Plot a model diurnal cycle of temperature
function plot1

  ## Data points every 10 minutes for 1 day
  npts = 73;

  xmin = 0;
  xmax = 60.0*60.0*24.0;    ## Number of seconds in a day
  ymin = 10.0;
  ymax = 20.0;

  i = 0:npts-1;
  x = i*xmax/npts;
  y = 15.0 - 5.0*cos( 2*pi*i/npts);
  xerr1 = x-60.0*5.0;
  xerr2 = x+60.0*5.0;
  yerr1 = y-0.1;
  yerr2 = y+0.1;
  
  pladv(0);

  ## Rescale major ticks marks by 0.5
  plsmaj(0.0,0.5);
  ## Rescale minor ticks and error bar marks by 0.5
  plsmin(0.0,0.5);

  plvsta();
  plwind(xmin, xmax, ymin, ymax);

  ## Draw a box with ticks spaced every 3 hour in X and 1 degree C in Y.
  plcol0(1);
  ## Set time format to be hours:minutes
  pltimefmt("%H:%M");
  plbox("bcnstd", 3.0*60*60, 3, "bcnstv", 1, 5);

  plcol0(3);
  pllab("Time (hours:mins)", "Temperature (degC)", "@frPLplot Example 29 - Daily temperature");
  
  plcol0(4);

  plline(x', y');
  plcol0(2);
  plerrx(xerr1', xerr2', y');
  plcol0(3);
  plerry(x', yerr1', yerr2');

  ## Rescale major / minor tick marks back to default
  plsmin(0.0,1.0);
  plsmaj(0.0,1.0);

endfunction

## Plot the number of hours of daylight as a function of day for a year
function plot2

  ## Latitude for London
  lat = 51.5;

  npts = 365;

  xmin = 0;
  xmax = npts*60.0*60.0*24.0;
  ymin = 0;
  ymax = 24;
  
  ## Formula for hours of daylight from 
  ## "A Model Comparison for Daylength as a Function of Latitude and 
  ## Day of the Year", 1995, Ecological Modelling, 80, pp 87-95.
  i = 0:npts-1;
  x = i*60.0*60.0*24.0;
  p = asin(0.39795*cos(0.2163108 + 2*atan(0.9671396*tan(0.00860*(i-186)))));
  d = 24.0 - (24.0/pi)*acos( (sin(0.8333*pi/180.0) + \
			      sin(lat*pi/180.0)*sin(p)) \
			    ./(cos(lat*pi/180.0)*cos(p)) ); 
  y = d;
  
  plcol0(1);
  ## Set time format to be abbreviated month name followed by day of month
  pltimefmt("%b %d");
  plprec(1,1);
  plenv(xmin, xmax, ymin, ymax, 0, 40);


  plcol0(3);
  pllab("Date", "Hours of daylight", "@frPLplot Example 29 - Hours of daylight at 51.5N");
  
  plcol0(4);

  plline(x', y');

  plprec(0,0);  

endfunction

function plot3

  ## Calculate seconds since the Unix epoch for 2005-12-01 UTC.
  tm.year = 105; ## Years since 1900
  tm.mon = 11;   ## 0 == January, 11 = December
  tm.mday = 1;   ## 1 = 1st of month
  tm.hour = 0;
  tm.min = 0;
  tm.sec = 0;

  ## NB - no need to call tzset in octave - it doesn't exist.
  tz = getenv("TZ");
  putenv("TZ","");
  
  ## tstart is a time_t value (cast to PLFLT) which represents the number
  ## of seconds elapsed since 00:00:00 on January 1, 1970, Coordinated 
  ## Universal Time (UTC).  
  tstart = mktime(tm);

  ## Note currently octave appears to have no way to unset a env 
  ## variable. 
  putenv("TZ",tz);

  npts = 62;

  xmin = tstart;
  xmax = xmin + npts*60.0*60.0*24.0;
  ymin = 0.0;
  ymax = 5.0;

  i = 0:npts-1;
  x = xmin + i*60.0*60.0*24.0;
  y = 1.0 + sin( 2*pi*i / 7.0 ) + exp( min(i,npts-i) / 31.0);

  pladv(0);
  
  plvsta();
  plwind(xmin, xmax, ymin, ymax);
  plcol0(1);
  ## Set time format to be ISO 8601 standard YYYY-MM-DD. Note that this is
  ## equivalent to %f for C99 compliant implementations of strftime.
  pltimefmt("%Y-%m-%d");
  ## Draw a box with ticks spaced every 14 days in X and 1 hour in Y.
  plbox("bcnstd", 14*24.0*60.0*60.0,14, "bcnstv", 1, 4);

  plcol0(3);
  pllab("Date", "Hours of television watched", "@frPLplot Example 29 - Hours of television watched in Dec 2005 / Jan 2006");
  
  plcol0(4);

  ## Rescale symbol size (used by plpoin) by 0.5
  plssym(0.0,0.5);
  plpoin(x', y', 2);
  plline(x', y');
   
endfunction

function plot4

  ## TAI-UTC (seconds) as a function of time.
  ## Use Besselian epochs as the continuous time interval just to prove
  ## this does not introduce any issues.
  
  ## Use the definition given in http://en.wikipedia.org/wiki/Besselian_epoch
  ## B = 1900. + (JD -2415020.31352)/365.242198781 
  ## ==> (as calculated with aid of "bc -l" command)
  ## B = (MJD + 678940.364163900)/365.242198781
  ## ==>
  ## MJD = B*365.24219878 - 678940.364163900
  scale = 365.242198781;
  offset1 = -678940.;
  offset2 = -0.3641639;
  plconfigtime(scale, offset1, offset2, 0x0, 0, 0, 0, 0, 0, 0, 0.);

  for kind=0:6
    if (kind == 0)
      xmin = plctime(1950,0,2,0,0,0.);
      xmax = plctime(2020,0,2,0,0,0.);
      npts = 70*12 + 1;
      ymin = 0.0;
      ymax = 36.0;
      time_format = "%Y%";
      if_TAI_time_format = 1;
      title_suffix = "from 1950 to 2020";
      xtitle =  "Year";
      xlabel_step = 10.;    
    elseif (kind == 1 || kind ==2)
      xmin = plctime(1961,7,1,0,0,1.64757-.20);
      xmax = plctime(1961,7,1,0,0,1.64757+.20);
      npts = 1001;
      ymin = 1.625;
      ymax = 1.725;
      time_format = "%S%2%";
      title_suffix = "near 1961-08-01 (TAI)";
      xlabel_step = 0.05/(scale*86400.);
      if (kind == 1)
        if_TAI_time_format = 1;
        xtitle = "Seconds (TAI)";
      else
        if_TAI_time_format = 0;
        xtitle = "Seconds (TAI) labelled with corresponding UTC";
      endif
    elseif (kind == 3 || kind ==4)
      xmin = plctime(1963,10,1,0,0,2.6972788-.20);
      xmax = plctime(1963,10,1,0,0,2.6972788+.20);
      npts = 1001;
      ymin = 2.55;
      ymax = 2.75;
      time_format = "%S%2%";
      title_suffix = "near 1963-11-01 (TAI)";
      xlabel_step = 0.05/(scale*86400.);
      if (kind == 3)
	if_TAI_time_format = 1;
	xtitle = "Seconds (TAI)";
      else 
	if_TAI_time_format = 0;
	xtitle = "Seconds (TAI) labelled with corresponding UTC";
      endif
    elseif (kind == 5 || kind == 6)
      xmin = plctime(2009,0,1,0,0,34.-5.);
      xmax = plctime(2009,0,1,0,0,34.+5.);
      npts = 1001;
      ymin = 32.5;
      ymax = 34.5;
      time_format = "%S%2%";
      title_suffix = "near 2009-01-01 (TAI)";
      xlabel_step = 1./(scale*86400.);
      if (kind == 5)
        if_TAI_time_format = 1;
        xtitle = "Seconds (TAI)";
      else
        if_TAI_time_format = 0;
        xtitle = "Seconds (TAI) labelled with corresponding UTC";
      endif
    endif
    
    x = xmin + (0:npts-1)*(xmax-xmin)/(npts-1);
    plconfigtime(scale, offset1, offset2, 0x0, 0, 0, 0, 0, 0, 0, 0.);
    tai = x;
    for i=1:npts
      [tai_year(i) , tai_month(i), tai_day(i), tai_hour(i), tai_min(i), tai_sec(i)] = plbtime(tai(i));
    endfor
    plconfigtime(scale, offset1, offset2, 0x2, 0, 0, 0, 0, 0, 0, 0.);
    for i=1:npts
      [utc_year(i), utc_month(i), utc_day(i), utc_hour(i), utc_min(i), utc_sec(i)] = plbtime(tai(i));
    endfor
    plconfigtime(scale, offset1, offset2, 0x0, 0, 0, 0, 0, 0, 0, 0.);
    for i=1:npts
      utc(i) = plctime(utc_year(i), utc_month(i), utc_day(i), utc_hour(i), utc_min(i), utc_sec(i));
    endfor
    y = (tai-utc)*scale*86400.0;
    
    pladv(0);
    plvsta();
    plwind(xmin, xmax, ymin, ymax);
    plcol0(1);
    if (if_TAI_time_format) 
      plconfigtime(scale, offset1, offset2, 0x0, 0, 0, 0, 0, 0, 0, 0.);
    else
      plconfigtime(scale, offset1, offset2, 0x2, 0, 0, 0, 0, 0, 0, 0.);
    endif
    pltimefmt(time_format);
    plbox("bcnstd", xlabel_step, 0, "bcnstv", 0., 0);
    plcol0(3);
    title = ["@frPLplot Example 29 - TAI-UTC ", title_suffix];
    pllab(xtitle, "TAI-UTC (sec)", title);
    
    plcol0(4);
    
    plline(x', y');
  endfor
endfunction

  
ix29c
 
