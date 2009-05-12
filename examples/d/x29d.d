/* $Id$

       Sample plots using date / time formatting for axes

  Copyright (C) 2009 Werner Smekal

  This file is part of PLplot.

    PLplot is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Library Public License as published
  by the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  PLplot is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU Library General Public License for more details.

  You should have received a copy of the GNU Library General Public License
  along with PLplot; if not, write to the Free Software
  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA

*/

import std.string;
import std.math;

import plplot;

/*--------------------------------------------------------------------------*\
 * main
 * 
 * Draws several plots which demonstrate the use of date / time formats for
 * the axis labels.
 * Time formatting is done using the strfqsas routine from the qsastime
 * library.  This is similar to strftime, but works for a broad
 * date range even on 32-bit systems.  See the 
 * documentation of strfqsas for full details of the available formats.
 *
 * 1) Plotting temperature over a day (using hours / minutes)
 * 2) Plotting 
 *
 * Note: We currently use the default call for plconfigtime (done in
 * plinit) which means continuous times are interpreted as seconds since
 * 1970-01-01, but that may change in future, more extended versions of 
 * this example.
 * 
\*--------------------------------------------------------------------------*/

int main( char[][] args )
{
  /* Parse and process command line arguments */
  plparseopts(args, PL_PARSE_FULL);

  /* Initialize plplot */
  plinit();

  /* Change the escape character to a '@' instead of the default '#' */
  plsesc('@');

  plot myPlot = new plot;
  
  myPlot.page1();
  myPlot.page2();
  myPlot.page3();

  /* Don't forget to call plend() to finish off! */
  plend();
  return 0;
}


class plot {
  private PLFLT[] x, y;
  private PLFLT[] xerr1, xerr2, yerr1, yerr2;

  private void resizeArrays(int size)
  {
    x.length = y.length = size;
    xerr1.length = xerr2.length = size;
    yerr1.length = yerr2.length = size;
  }
  
  /* Plot a model diurnal cycle of temperature */
  void page1() 
  {
    /* resize arrays */
    const int npts = 73;
    resizeArrays(npts);
    
    /* Data points every 10 minutes for 1 day */
    PLFLT xmin = 0;
    PLFLT xmax = 60.0*60.0*24.0;    /* Number of seconds in a day */
    PLFLT ymin = 10.0;
    PLFLT ymax = 20.0;

    for(int i=0; i<npts; i++) {
      x[i] = xmax*(i/cast(PLFLT)npts);
      y[i] = 15.0 - 5.0*cos(2*PI*(i/cast(PLFLT)npts));
      
      /* Set x error bars to +/- 5 minute */
      xerr1[i] = x[i]-60*5;
      xerr2[i] = x[i]+60*5;
      
      /* Set y error bars to +/- 0.1 deg C */
      yerr1[i] = y[i]-0.1;
      yerr2[i] = y[i]+0.1;
    }
    
    pladv(0);

    /* Rescale major ticks marks by 0.5 */
    plsmaj(0.0, 0.5);
    
    /* Rescale minor ticks and error bar marks by 0.5 */
    plsmin(0.0, 0.5);

    plvsta();
    plwind(xmin, xmax, ymin, ymax);

    /* Draw a box with ticks spaced every 3 hour in X and 1 degree C in Y. */
    plcol0(1);
    
    /* Set time format to be hours:minutes */
    pltimefmt("%H:%M");
    plbox("bcnstd", 3.0*60*60, 3, "bcnstv", 1, 5);

    plcol0(3);
    pllab("Time (hours:mins)", "Temperature (degC)", "@frPLplot Example 29 - Daily temperature");
    
    plcol0(4);

    plline(x, y);
    plcol0(2);
    plerrx(xerr1, xerr2, y);
    plcol0(3);
    plerry(x, yerr1, yerr2);

    /* Rescale major / minor tick marks back to default */
    plsmin(0.0, 1.0); 
    plsmaj(0.0, 1.0); 
  }

  /* Plot the number of hours of daylight as a function of day for a year */
  void page2() 
  {
    /* resize arrays */
    const int npts = 365;
    resizeArrays(npts);
    
    /* Latitude for London */
    PLFLT lat = 51.5;

    PLFLT xmin = 0;
    PLFLT xmax = npts*60.0*60.0*24.0;
    PLFLT ymin = 0;
    PLFLT ymax = 24;
    
    /* Formula for hours of daylight from 
     * "A Model Comparison for Daylength as a Function of Latitude and 
     * Day of the Year", 1995, Ecological Modelling, 80, pp 87-95. */
    PLFLT p, d;
    for (int j=0; j<npts; j++) {
      x[j] = j*60.0*60.0*24.0;
      p = asin(0.39795*cos(0.2163108 + 2*atan(0.9671396*tan(0.00860*(j-186)))));
      d = 24.0 - (24.0/PI)*
        acos( (sin(0.8333*PI/180.0) + sin(lat*PI/180.0)*sin(p)) / 
        (cos(lat*PI/180.0)*cos(p)) );
      y[j] = d;
    }

    plcol0(1);
    
    /* Set time format to be abbreviated month name followed by day of month */
    pltimefmt("%b %d");
    plprec(1,1);
    plenv(xmin, xmax, ymin, ymax, 0, 40);

    plcol0(3);
    pllab("Date", "Hours of daylight", "@frPLplot Example 29 - Hours of daylight at 51.5N");
    
    plcol0(4);

    plline(x, y);
    
    plprec(0,0);
  }
  
  void page3()
  {
    /* resize arrays */
    const int npts = 62;
    resizeArrays(npts);
    
    /* Calculate continuous time corresponding to 2005-12-01 UTC. */
    PLFLT tstart;
    plctime(2005, 11, 01, 0, 0, 0., &tstart);

    PLFLT xmin = tstart;
    PLFLT xmax = xmin + npts*60.0*60.0*24.0;
    PLFLT ymin = 0.0;
    PLFLT ymax = 5.0;
    
    for(int i=0; i<npts; i++) {
      x[i] = xmin + i*60.0*60.0*24.0;
      y[i] = 1.0 + sin(2*PI*i/7.0) +  exp(fmin(i,npts-i)/31.0);
    }
    pladv(0);

    plvsta();
    plwind(xmin, xmax, ymin, ymax);

    plcol0(1);
    /* Set time format to be ISO 8601 standard YYYY-MM-DD. */
    pltimefmt("%F");
    /* Draw a box with ticks spaced every 14 days in X and 1 hour in Y. */
    plbox("bcnstd", 14*24.0*60.0*60.0,14, "bcnstv", 1, 4);

    plcol0(3);
    pllab("Date", "Hours of television watched", "@frPLplot Example 29 - Hours of television watched in Dec 2005 / Jan 2006");
    
    plcol0(4);

    /* Rescale symbol size (used by plpoin) by 0.5 */
    plssym(0.0,0.5);
    plpoin(x, y, 2);
    plline(x, y);
  }  
}
