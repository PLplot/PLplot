// $Id$
//
//       Sample plots using date / time formatting for axes
//
// Copyright (C) 2008 Andrew Ross
//
// This file is part of PLplot.
//
// PLplot is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Library Public License as published
// by the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// PLplot is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Library General Public License for more details.
//
// You should have received a copy of the GNU Library General Public License
// along with PLplot; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
//

package plplot.examples;

import plplot.core.*;

import java.util.*;


class x29 {

    PLStream pls = new PLStream();
    
    //------------------------------------------------------------------------
    // main
    // 
    // Draws several plots which demonstrate the use of date / time formats
    // for the axis labels.
    // Time formatting is done using the system strftime routine. See the 
    // documentation of this for full details of the available formats.
    //
    // 1) Plotting temperature over a day (using hours / minutes)
    // 2) Plotting 
    //
    // Note: Times are stored as seconds since the epoch (usually 
    // 1st Jan 1970). 
    // 
    //------------------------------------------------------------------------

    x29(String[] args)
    {

	// Parse command line arguments
	pls.parseopts(args, PLStream.PL_PARSE_FULL | PLStream.PL_PARSE_NOPROGRAM);

	// Initialize plplot 
	pls.init();

	plot1();
	
	plot2();
	
	plot3();

	pls.end();
	
    }

    // Plot a model diurnal cycle of temperature 
    void plot1() 
    {
	int i, npts;
	double xmin, xmax, ymin, ymax;
	double x[], y[];

	// Data points every 10 minutes for 1 day
	npts = 145;

	x = new double[npts];
	y = new double[npts];
	
	xmin = 0;
	xmax = 60.0*60.0*24.0;    // Number of seconds in a day
	ymin = 10.0;
	ymax = 20.0;

	for (i=0;i<npts;i++) {
	    x[i] = xmax*((double) i/(double)npts);
	    y[i] = 15.0 - 5.0*Math.cos( 2*Math.PI*((double) i / (double) npts));
	}
	
	pls.adv(0);
	
	pls.vsta();
	pls.wind(xmin, xmax, ymin, ymax);
	
	// Draw a box with ticks spaced every 3 hour in X and 1 degree C in Y.
	pls.col0(1);
	// Set time format to be hours:minutes
	pls.timefmt("%H:%M");
	pls.box("bcnstd", 3.0*60*60, 3, "bcnstv", 1, 5);
	
	pls.col0(3);
	pls.lab("Time (hours:mins)", "Temperature (degC)", "#frPLplot Example 29 - Daily temperature");
	
	pls.col0(4);

	pls.line(x, y);
    }

    // Plot the number of hours of daylight as a function of day for a year 
    void plot2() 
    {
	int j, npts;
	double xmin, xmax, ymin, ymax;
	double lat, p, d;
	double x[], y[];
	
	// Latitude for London 
	lat = 51.5;
	
	npts = 365;
	
	x = new double[npts];
	y = new double[npts];
	
	xmin = 0;
	xmax = npts*60.0*60.0*24.0;
	ymin = 0;
	ymax = 24;
	
	// Formula for hours of daylight from 
	// "A Model Comparison for Daylength as a Function of Latitude and 
	// Day of the Year", 1995, Ecological Modelling, 80, pp 87-95. 
	for (j = 0; j < npts; j++) {
	    x[j] = j*60.0*60.0*24.0;
	    p = Math.asin(0.39795*Math.cos(0.2163108 + 2*Math.atan(0.9671396*Math.tan(0.00860*(j-186)))));
	    d = 24.0 - (24.0/Math.PI)*
		Math.acos( (Math.sin(0.8333*Math.PI/180.0) + Math.sin(lat*Math.PI/180.0)*Math.sin(p)) / 
		      (Math.cos(lat*Math.PI/180.0)*Math.cos(p)) );
	    y[j] = d;
	}
	
	pls.col0(1);
	// Set time format to be abbreviated month name followed by day of month 
	pls.timefmt("%b %d");
	pls.env(xmin, xmax, ymin, ymax, 0, 40);
	

	pls.col0(3);
	pls.lab("Date", "Hours of daylight", "#frPLplot Example 29 - Hours of daylight at 51.5N");
	
	pls.col0(4);
	
	pls.line(x, y);
	
    }

    void plot3()
    {
	int i, npts;
	double xmin, xmax, ymin, ymax;
	long tstart;
	double x[], y[];

	TimeZone tz = TimeZone.getTimeZone("UTC");
	Calendar cal = Calendar.getInstance(tz);
	
	cal.set(2005, 11, 1, 0, 0, 0);
	
	tstart = cal.getTimeInMillis()/1000;

	npts = 62;
	
	x = new double[npts];
	y = new double[npts];
	
	xmin = (double) tstart;
	xmax = xmin + npts*60.0*60.0*24.0;
	ymin = 0.0;
	ymax = 5.0;
	
	for (i = 0; i<npts; i++) {
	    x[i] = xmin + i*60.0*60.0*24.0;
	    y[i] = 1.0 + Math.sin( 2*Math.PI*( (double) i ) / 7.0 ) + 
		Math.exp( ((double) Math.min(i,npts-i)) / 31.0);
	}
	pls.adv(0);
	
	pls.vsta();
	pls.wind(xmin, xmax, ymin, ymax);
	
	pls.col0(1);
	// Set time format to be ISO 8601 standard YYYY-MM-HH. Note that this is
	// equivalent to %f for C99 compliant implementations of strftime.
	pls.timefmt("%Y-%m-%d");
	// Draw a box with ticks spaced every 14 days in X and 1 hour in Y.
	pls.box("bcnstd", 14*24.0*60.0*60.0,14, "bcnstv", 1, 4);
	
	pls.col0(3);
	pls.lab("Date", "Hours of television watched", "#frPLplot Example 29 - Hours of television watched in Dec 2005 / Jan 2006");
	
	pls.col0(4);
	
	pls.poin(x, y, 2);
	pls.line(x, y);
 
    }


    public static void main( String[] args )
    {
	new x29( args );
    }

};

