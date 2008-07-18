/* $Id$

       Sample plots using date / time formatting for axes

  Copyright (C) 2007 Andrew Ross

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

#include "plcdemos.h"

#include <time.h>

static PLFLT x[365], y[365];

/* Function prototypes */

void plot1();
void plot2();
void plot3();

/*--------------------------------------------------------------------------*\
 * main
 * 
 * Draws several plots which demonstrate the use of date / time formats for
 * the axis labels.
 * Time formatting is done using the system strftime routine. See the 
 * documentation of this for full details of the available formats.
 *
 * 1) Plotting temperature over a day (using hours / minutes)
 * 2) Plotting 
 *
 * Note: Times are stored as seconds since the epoch (usually 1st Jan 1970). 
 * 
\*--------------------------------------------------------------------------*/

int
main(int argc, const char *argv[])
{

  /* Parse command line arguments */
  plparseopts(&argc, argv, PL_PARSE_FULL);

  /* Initialize plplot */
  plinit();

  plot1();

  plot2();

  plot3();

  /* Don't forget to call plend() to finish off! */
  plend();
  exit(0);
}

/* Plot a model diurnal cycle of temperature */
void
plot1() 
{
  int i, npts;
  PLFLT xmin, xmax, ymin, ymax;

  /* Data points every 10 minutes for 1 day */
  npts = 145;

  xmin = 0;
  xmax = 60.0*60.0*24.0;    /* Number of seconds in a day */
  ymin = 10.0;
  ymax = 20.0;

  for (i=0;i<npts;i++) {
    x[i] = xmax*((PLFLT) i/(PLFLT)npts);
    y[i] = 15.0 - 5.0*cos( 2*M_PI*((PLFLT) i / (PLFLT) npts));
  }
  
  pladv(0);

  plvsta();
  plwind(xmin, xmax, ymin, ymax);

  /* Draw a box with ticks spaced every 3 hour in X and 1 degree C in Y. */
  plcol0(1);
  /* Set time format to be hours:minutes */
  pltimefmt("%H:%M");
  plbox("bcnstd", 3.0*60*60, 3, "bcnstv", 1, 5);

  plcol0(3);
  pllab("Time (hours:mins)", "Temperature (degC)", "#frPLplot Example 29 - Daily temperature");
  
  plcol0(4);

  plline(npts, x, y);
}

/* Plot the number of hours of daylight as a function of day for a year */
void
plot2() 
{
  int j, npts;
  PLFLT xmin, xmax, ymin, ymax;
  PLFLT lat, p, d;

  /* Latitude for London */
  lat = 51.5;

  npts = 365;

  xmin = 0;
  xmax = npts*60.0*60.0*24.0;
  ymin = 0;
  ymax = 24;
  
  /* Formula for hours of daylight from 
   * "A Model Comparison for Daylength as a Function of Latitude and 
   * Day of the Year", 1995, Ecological Modelling, 80, pp 87-95. */
  for (j = 0; j < npts; j++) {
    x[j] = j*60.0*60.0*24.0;
    p = asin(0.39795*cos(0.2163108 + 2*atan(0.9671396*tan(0.00860*(j-186)))));
    d = 24.0 - (24.0/M_PI)*
      acos( (sin(0.8333*M_PI/180.0) + sin(lat*M_PI/180.0)*sin(p)) / 
	    (cos(lat*M_PI/180.0)*cos(p)) );
    y[j] = d;
  }

  plcol0(1);
  /* Set time format to be abbreviated month name followed by day of month */
  pltimefmt("%b %d");
  plenv(xmin, xmax, ymin, ymax, 0, 40);


  plcol0(3);
  pllab("Date", "Hours of daylight", "#frPLplot Example 29 - Hours of daylight at 51.5N");
  
  plcol0(4);

  plline(npts, x, y);
  
}

void
plot3()
{
  int i, npts;
  PLFLT xmin, xmax, ymin, ymax;
  PLFLT tstart;
  char *tz;

  struct tm tm;

  /* Calculate seconds since the Unix epoch for 2005-12-01 UTC. */
#if defined(WIN32)
  /* Adopt the known result for POSIX-compliant systems. */
  tstart = (PLFLT) 1133395200;
#else
  tm.tm_year = 105; /* Years since 1900 = 2005 */
  tm.tm_mon = 11;   /* 0 == January, 11 = December */
  tm.tm_mday = 1;   /* 1 = 1st of month */
  tm.tm_hour = 0;
  tm.tm_min = 0;
  tm.tm_sec = 0;

  /* Use POSIX-compliant equivalent of timegm GNU extension. */
  tz = getenv("TZ");
  setenv("TZ", "", 1);
  tzset();
  /* tstart is a time_t value (cast to PLFLT) which represents the number
   * of seconds elapsed since 00:00:00 on January 1, 1970, Coordinated 
   * Universal Time (UTC) on  2005-12-01 (UTC). */
  tstart = (PLFLT) mktime(&tm);
  if (tz)
    setenv("TZ", tz, 1);
  else
    unsetenv("TZ");
  tzset();
#endif
   
  npts = 62;

  xmin = tstart;
  xmax = xmin + npts*60.0*60.0*24.0;
  ymin = 0.0;
  ymax = 5.0;
  
  for (i = 0; i<npts; i++) {
    x[i] = xmin + i*60.0*60.0*24.0;
    y[i] = 1.0 + sin( 2*M_PI*( (PLFLT) i ) / 7.0 ) + 
      exp( ((PLFLT) MIN(i,npts-i)) / 31.0);
  }
  pladv(0);

  plvsta();
  plwind(xmin, xmax, ymin, ymax);

  plcol0(1);
  /* Set time format to be ISO 8601 standard YYYY-MM-DD. Note that this is
   * equivalent to %f for C99 compliant implementations of strftime. */
  pltimefmt("%Y-%m-%d");
  /* Draw a box with ticks spaced every 14 days in X and 1 hour in Y. */
  plbox("bcnstd", 14*24.0*60.0*60.0,14, "bcnstv", 1, 4);

  plcol0(3);
  pllab("Date", "Hours of television watched", "#frPLplot Example 29 - Hours of television watched in Dec 2005 / Jan 2006");
  
  plcol0(4);

  plpoin(npts, x, y, 2);
  plline(npts, x, y);
 
}
