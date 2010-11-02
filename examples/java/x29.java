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

    x29( String[] args )
    {
        // Parse command line arguments
        pls.parseopts( args, PLStream.PL_PARSE_FULL | PLStream.PL_PARSE_NOPROGRAM );

        // Initialize plplot
        pls.init();

        pls.sesc( '@' );

        plot1();

        plot2();

        plot3();

        plot4();

        pls.end();
    }

    // Plot a model diurnal cycle of temperature
    void plot1()
    {
        int    i, npts;
        double xmin, xmax, ymin, ymax;
        double x[], y[], xerr1[], xerr2[], yerr1[], yerr2[];

        // Data points every 10 minutes for 1 day
        npts = 73;

        x     = new double[npts];
        y     = new double[npts];
        xerr1 = new double[npts];
        xerr2 = new double[npts];
        yerr1 = new double[npts];
        yerr2 = new double[npts];

        xmin = 0;
        xmax = 60.0 * 60.0 * 24.0;    // Number of seconds in a day
        ymin = 10.0;
        ymax = 20.0;

        for ( i = 0; i < npts; i++ )
        {
            x[i] = xmax * ( (double) i / (double) npts );
            y[i] = 15.0 - 5.0 * Math.cos( 2 * Math.PI * ( (double) i / (double) npts ) );
            // Set x error bars to +/- 5 minute
            xerr1[i] = x[i] - 60 * 5;
            xerr2[i] = x[i] + 60 * 5;
            // Set y error bars to +/- 0.1 deg C
            yerr1[i] = y[i] - 0.1;
            yerr2[i] = y[i] + 0.1;
        }

        pls.adv( 0 );

        // Rescale major ticks marks by 0.5
        pls.smaj( 0.0, 0.5 );
        // Rescale minor ticks and error bar marks by 0.5
        pls.smin( 0.0, 0.5 );

        pls.vsta();
        pls.wind( xmin, xmax, ymin, ymax );

        // Draw a box with ticks spaced every 3 hour in X and 1 degree C in Y.
        pls.col0( 1 );
        // Set time format to be hours:minutes
        pls.timefmt( "%H:%M" );
        pls.box( "bcnstd", 3.0 * 60 * 60, 3, "bcnstv", 1, 5 );

        pls.col0( 3 );
        pls.lab( "Time (hours:mins)", "Temperature (degC)", "@frPLplot Example 29 - Daily temperature" );

        pls.col0( 4 );

        pls.line( x, y );
        pls.col0( 2 );
        pls.errx( xerr1, xerr2, y );
        pls.col0( 3 );
        pls.erry( x, yerr1, yerr2 );

        // Rescale major / minor tick marks back to default
        pls.smin( 0.0, 1.0 );
        pls.smaj( 0.0, 1.0 );
    }

    // Plot the number of hours of daylight as a function of day for a year
    void plot2()
    {
        int    j, npts;
        double xmin, xmax, ymin, ymax;
        double lat, p, d;
        double x[], y[];

        // Latitude for London
        lat = 51.5;

        npts = 365;

        x = new double[npts];
        y = new double[npts];

        xmin = 0;
        xmax = npts * 60.0 * 60.0 * 24.0;
        ymin = 0;
        ymax = 24;

        // Formula for hours of daylight from
        // "A Model Comparison for Daylength as a Function of Latitude and
        // Day of the Year", 1995, Ecological Modelling, 80, pp 87-95.
        for ( j = 0; j < npts; j++ )
        {
            x[j] = j * 60.0 * 60.0 * 24.0;
            p    = Math.asin( 0.39795 * Math.cos( 0.2163108 + 2 * Math.atan( 0.9671396 * Math.tan( 0.00860 * ( j - 186 ) ) ) ) );
            d    = 24.0 - ( 24.0 / Math.PI ) *
                   Math.acos( ( Math.sin( 0.8333 * Math.PI / 180.0 ) + Math.sin( lat * Math.PI / 180.0 ) * Math.sin( p ) ) /
                ( Math.cos( lat * Math.PI / 180.0 ) * Math.cos( p ) ) );
            y[j] = d;
        }

        pls.col0( 1 );
        // Set time format to be abbreviated month name followed by day of month
        pls.timefmt( "%b %d" );
        pls.prec( 1, 1 );
        pls.env( xmin, xmax, ymin, ymax, 0, 40 );


        pls.col0( 3 );
        pls.lab( "Date", "Hours of daylight", "@frPLplot Example 29 - Hours of daylight at 51.5N" );

        pls.col0( 4 );

        pls.line( x, y );

        pls.prec( 0, 0 );
    }

    void plot3()
    {
        int      i, npts;
        double   xmin, xmax, ymin, ymax;
        long     tstart;
        double   x[], y[];

        TimeZone tz  = TimeZone.getTimeZone( "UTC" );
        Calendar cal = Calendar.getInstance( tz );

        cal.set( 2005, 11, 1, 0, 0, 0 );

        tstart = cal.getTimeInMillis() / 1000;

        npts = 62;

        x = new double[npts];
        y = new double[npts];

        xmin = (double) tstart;
        xmax = xmin + npts * 60.0 * 60.0 * 24.0;
        ymin = 0.0;
        ymax = 5.0;

        for ( i = 0; i < npts; i++ )
        {
            x[i] = xmin + i * 60.0 * 60.0 * 24.0;
            y[i] = 1.0 + Math.sin( 2 * Math.PI * ( (double) i ) / 7.0 ) +
                   Math.exp( ( (double) Math.min( i, npts - i ) ) / 31.0 );
        }
        pls.adv( 0 );

        pls.vsta();
        pls.wind( xmin, xmax, ymin, ymax );

        pls.col0( 1 );
        // Set time format to be ISO 8601 standard YYYY-MM-HH. Note that this is
        // equivalent to %f for C99 compliant implementations of strftime.
        pls.timefmt( "%Y-%m-%d" );
        // Draw a box with ticks spaced every 14 days in X and 1 hour in Y.
        pls.box( "bcnstd", 14 * 24.0 * 60.0 * 60.0, 14, "bcnstv", 1, 4 );

        pls.col0( 3 );
        pls.lab( "Date", "Hours of television watched", "@frPLplot Example 29 - Hours of television watched in Dec 2005 / Jan 2006" );

        pls.col0( 4 );

        pls.ssym( 0.0, 0.5 );
        pls.poin( x, y, 2 );
        pls.line( x, y );
    }

    void plot4()
    {
        // TAI-UTC (seconds) as a function of time.
        // Use Besselian epochs as the continuous time interval just to prove
        // this does not introduce any issues.

        double  scale, offset1, offset2;
        double  xmin[] = new double[1], xmax[] = new double[1];
        double  ymin   = 0.0, ymax = 0.0, xlabel_step = 0.0;
        int     kind, npts = 1001, i;
        boolean if_TAI_time_format = false;
        String  time_format        = "";
        String  title_suffix       = "";
        String  xtitle             = "";
        String  title = "";
        double  x[];
        double  y[];
        int     tai_year[] = new int[1], tai_month[] = new int[1],
                tai_day[]  = new int[1], tai_hour[] = new int[1],
                tai_min[]  = new int[1];
        double tai_sec[]   = new double[1], tai;
        int    utc_year[]  = new int[1], utc_month[] = new int[1],
               utc_day[]   = new int[1], utc_hour[] = new int[1],
               utc_min[]   = new int[1];
        double utc_sec[]   = new double[1], utc[] = new double[1];

        // Use the definition given in http://en.wikipedia.org/wiki/Besselian_epoch
        // B = 1900. + (JD -2415020.31352)/365.242198781
        // ==> (as calculated with aid of "bc -l" command)
        // B = (MJD + 678940.364163900)/365.242198781
        // ==>
        // MJD = B*365.24219878 - 678940.364163900
        scale   = 365.242198781;
        offset1 = -678940.;
        offset2 = -0.3641639;
        pls.configtime( scale, offset1, offset2, 0x0, false, 0, 0, 0, 0, 0, 0. );

        for ( kind = 0; kind < 7; kind++ )
        {
            if ( kind == 0 )
            {
                pls.ctime( 1950, 0, 2, 0, 0, 0., xmin );
                pls.ctime( 2020, 0, 2, 0, 0, 0., xmax );
                npts               = 70 * 12 + 1;
                ymin               = 0.0;
                ymax               = 36.0;
                time_format        = "%Y%";
                if_TAI_time_format = true;
                title_suffix       = "from 1950 to 2020";
                xtitle             = "Year";
                xlabel_step        = 10.;
            }
            else if ( kind == 1 || kind == 2 )
            {
                pls.ctime( 1961, 7, 1, 0, 0, 1.64757 - .20, xmin );
                pls.ctime( 1961, 7, 1, 0, 0, 1.64757 + .20, xmax );
                npts         = 1001;
                ymin         = 1.625;
                ymax         = 1.725;
                time_format  = "%S%2%";
                title_suffix = "near 1961-08-01 (TAI)";
                xlabel_step  = 0.05 / ( scale * 86400. );
                if ( kind == 1 )
                {
                    if_TAI_time_format = true;
                    xtitle             = "Seconds (TAI)";
                }
                else
                {
                    if_TAI_time_format = false;
                    xtitle             = "Seconds (TAI) labelled with corresponding UTC";
                }
            }
            else if ( kind == 3 || kind == 4 )
            {
                pls.ctime( 1963, 10, 1, 0, 0, 2.6972788 - .20, xmin );
                pls.ctime( 1963, 10, 1, 0, 0, 2.6972788 + .20, xmax );
                npts         = 1001;
                ymin         = 2.55;
                ymax         = 2.75;
                time_format  = "%S%2%";
                title_suffix = "near 1963-11-01 (TAI)";
                xlabel_step  = 0.05 / ( scale * 86400. );
                if ( kind == 3 )
                {
                    if_TAI_time_format = true;
                    xtitle             = "Seconds (TAI)";
                }
                else
                {
                    if_TAI_time_format = false;
                    xtitle             = "Seconds (TAI) labelled with corresponding UTC";
                }
            }
            else if ( kind == 5 || kind == 6 )
            {
                pls.ctime( 2009, 0, 1, 0, 0, 34. - 5., xmin );
                pls.ctime( 2009, 0, 1, 0, 0, 34. + 5., xmax );
                npts         = 1001;
                ymin         = 32.5;
                ymax         = 34.5;
                time_format  = "%S%2%";
                title_suffix = "near 2009-01-01 (TAI)";
                xlabel_step  = 1. / ( scale * 86400. );
                if ( kind == 5 )
                {
                    if_TAI_time_format = true;
                    xtitle             = "Seconds (TAI)";
                }
                else
                {
                    if_TAI_time_format = false;
                    xtitle             = "Seconds (TAI) labelled with corresponding UTC";
                }
            }

            x = new double[npts];
            y = new double[npts];
            for ( i = 0; i < npts; i++ )
            {
                x[i] = xmin[0] + i * ( xmax[0] - xmin[0] ) / ( (double) ( npts - 1 ) );
                pls.configtime( scale, offset1, offset2, 0x0, false, 0, 0, 0, 0, 0, 0. );
                tai = x[i];
                pls.btime( tai_year, tai_month, tai_day, tai_hour, tai_min, tai_sec, tai );
                pls.configtime( scale, offset1, offset2, 0x2, false, 0, 0, 0, 0, 0, 0. );
                pls.btime( utc_year, utc_month, utc_day, utc_hour, utc_min, utc_sec, tai );
                pls.configtime( scale, offset1, offset2, 0x0, false, 0, 0, 0, 0, 0, 0. );
                pls.ctime( utc_year[0], utc_month[0], utc_day[0], utc_hour[0], utc_min[0], utc_sec[0], utc );
                y[i] = ( tai - utc[0] ) * scale * 86400.;
            }

            pls.adv( 0 );
            pls.vsta();
            pls.wind( xmin[0], xmax[0], ymin, ymax );
            pls.col0( 1 );
            if ( if_TAI_time_format )
                pls.configtime( scale, offset1, offset2, 0x0, false, 0, 0, 0, 0, 0, 0. );
            else
                pls.configtime( scale, offset1, offset2, 0x2, false, 0, 0, 0, 0, 0, 0. );
            pls.timefmt( time_format );
            pls.box( "bcnstd", xlabel_step, 0, "bcnstv", 0., 0 );
            pls.col0( 3 );
            title = "@frPLplot Example 29 - TAI-UTC " + title_suffix;
            pls.lab( xtitle, "TAI-UTC (sec)", title );

            pls.col0( 4 );

            pls.line( x, y );
        }
    }


    public static void main( String[] args )
    {
        new x29( args );
    }
};

