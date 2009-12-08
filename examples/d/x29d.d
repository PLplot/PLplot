/* $Id$
 *
 *     Sample plots using date / time formatting for axes
 *
 * Copyright (C) 2009 Werner Smekal
 *
 * This file is part of PLplot.
 *
 *  PLplot is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Library Public License as published
 * by the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * PLplot is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with PLplot; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 *
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
    plparseopts( args, PL_PARSE_FULL );

    /* Initialize plplot */
    plinit();

    /* Change the escape character to a '@' instead of the default '#' */
    plsesc( '@' );

    plot myPlot = new plot;

    myPlot.page1();
    myPlot.page2();
    myPlot.page3();
    myPlot.page4();

    /* Don't forget to call plend() to finish off! */
    plend();
    return 0;
}


class plot {
    private PLFLT[] x, y;
    private PLFLT[] xerr1, xerr2, yerr1, yerr2;

    private void resizeArrays( int size )
    {
        x.length     = y.length = size;
        xerr1.length = xerr2.length = size;
        yerr1.length = yerr2.length = size;
    }

    /* Plot a model diurnal cycle of temperature */
    void page1()
    {
        /* resize arrays */
        const int npts = 73;
        resizeArrays( npts );

        /* Data points every 10 minutes for 1 day */
        PLFLT xmin = 0;
        PLFLT xmax = 60.0 * 60.0 * 24.0; /* Number of seconds in a day */
        PLFLT ymin = 10.0;
        PLFLT ymax = 20.0;

        for ( int i = 0; i < npts; i++ )
        {
            x[i] = xmax * ( i / cast(PLFLT) npts );
            y[i] = 15.0 - 5.0 * cos( 2 * PI * ( i / cast(PLFLT) npts ));

            /* Set x error bars to +/- 5 minute */
            xerr1[i] = x[i] - 60 * 5;
            xerr2[i] = x[i] + 60 * 5;

            /* Set y error bars to +/- 0.1 deg C */
            yerr1[i] = y[i] - 0.1;
            yerr2[i] = y[i] + 0.1;
        }

        pladv( 0 );

        /* Rescale major ticks marks by 0.5 */
        plsmaj( 0.0, 0.5 );

        /* Rescale minor ticks and error bar marks by 0.5 */
        plsmin( 0.0, 0.5 );

        plvsta();
        plwind( xmin, xmax, ymin, ymax );

        /* Draw a box with ticks spaced every 3 hour in X and 1 degree C in Y. */
        plcol0( 1 );

        /* Set time format to be hours:minutes */
        pltimefmt( "%H:%M" );
        plbox( "bcnstd", 3.0 * 60 * 60, 3, "bcnstv", 1, 5 );

        plcol0( 3 );
        pllab( "Time (hours:mins)", "Temperature (degC)", "@frPLplot Example 29 - Daily temperature" );

        plcol0( 4 );

        plline( x, y );
        plcol0( 2 );
        plerrx( xerr1, xerr2, y );
        plcol0( 3 );
        plerry( x, yerr1, yerr2 );

        /* Rescale major / minor tick marks back to default */
        plsmin( 0.0, 1.0 );
        plsmaj( 0.0, 1.0 );
    }

    /* Plot the number of hours of daylight as a function of day for a year */
    void page2()
    {
        /* resize arrays */
        const int npts = 365;
        resizeArrays( npts );

        /* Latitude for London */
        PLFLT lat = 51.5;

        PLFLT xmin = 0;
        PLFLT xmax = npts * 60.0 * 60.0 * 24.0;
        PLFLT ymin = 0;
        PLFLT ymax = 24;

        /* Formula for hours of daylight from
         * "A Model Comparison for Daylength as a Function of Latitude and
         * Day of the Year", 1995, Ecological Modelling, 80, pp 87-95. */
        PLFLT p, d;
        for ( int j = 0; j < npts; j++ )
        {
            x[j] = j * 60.0 * 60.0 * 24.0;
            p    = asin( 0.39795 * cos( 0.2163108 + 2 * atan( 0.9671396 * tan( 0.00860 * ( j - 186 )))));
            d    = 24.0 - ( 24.0 / PI ) *
                   acos(( sin( 0.8333 * PI / 180.0 ) + sin( lat * PI / 180.0 ) * sin( p )) /
                ( cos( lat * PI / 180.0 ) * cos( p )));
            y[j] = d;
        }

        plcol0( 1 );

        /* Set time format to be abbreviated month name followed by day of month */
        pltimefmt( "%b %d" );
        plprec( 1, 1 );
        plenv( xmin, xmax, ymin, ymax, 0, 40 );

        plcol0( 3 );
        pllab( "Date", "Hours of daylight", "@frPLplot Example 29 - Hours of daylight at 51.5N" );

        plcol0( 4 );

        plline( x, y );

        plprec( 0, 0 );
    }

    void page3()
    {
        /* resize arrays */
        const int npts = 62;
        resizeArrays( npts );

        /* Calculate continuous time corresponding to 2005-12-01 UTC. */
        PLFLT tstart;
        plctime( 2005, 11, 01, 0, 0, 0., &tstart );

        PLFLT xmin = tstart;
        PLFLT xmax = xmin + npts * 60.0 * 60.0 * 24.0;
        PLFLT ymin = 0.0;
        PLFLT ymax = 5.0;

        for ( int i = 0; i < npts; i++ )
        {
            x[i] = xmin + i * 60.0 * 60.0 * 24.0;
            y[i] = 1.0 + sin( 2 * PI * i / 7.0 ) + exp( fmin( i, npts - i ) / 31.0 );
        }
        pladv( 0 );

        plvsta();
        plwind( xmin, xmax, ymin, ymax );

        plcol0( 1 );
        /* Set time format to be ISO 8601 standard YYYY-MM-DD. */
        pltimefmt( "%F" );
        /* Draw a box with ticks spaced every 14 days in X and 1 hour in Y. */
        plbox( "bcnstd", 14 * 24.0 * 60.0 * 60.0, 14, "bcnstv", 1, 4 );

        plcol0( 3 );
        pllab( "Date", "Hours of television watched", "@frPLplot Example 29 - Hours of television watched in Dec 2005 / Jan 2006" );

        plcol0( 4 );

        /* Rescale symbol size (used by plpoin) by 0.5 */
        plssym( 0.0, 0.5 );
        plpoin( x, y, 2 );
        plline( x, y );
    }

    void page4()
    {
        /* TAI-UTC (seconds) as a function of time.
         *       Use Besselian epochs as the continuous time interval just to prove
         *       this does not introduce any issues. */

        /* Use the definition given in http://en.wikipedia.org/wiki/Besselian_epoch
         * B = 1900. + (JD -2415020.31352)/365.242198781
         * ==> (as calculated with aid of "bc -l" command)
         * B = (MJD + 678940.364163900)/365.242198781
         * ==>
         * MJD = B*365.24219878 - 678940.364163900 */
        PLFLT scale   = 365.242198781;
        PLFLT offset1 = -678940.;
        PLFLT offset2 = -0.3641639;
        plconfigtime( scale, offset1, offset2, 0x0, 0, 0, 0, 0, 0, 0, 0. );

        PLFLT  xmin, xmax, ymin, ymax, xlabel_step;
        int    npts, if_TAI_time_format;
        string time_format, title_suffix, xtitle;
        for ( int kind = 0; kind < 7; kind++ )
        {
            if ( kind == 0 )
            {
                plctime( 1950, 0, 2, 0, 0, 0.0, &xmin );
                plctime( 2020, 0, 2, 0, 0, 0.0, &xmax );
                npts               = 70 * 12 + 1;
                ymin               = 0.0;
                ymax               = 36.0;
                time_format        = "%Y%";
                if_TAI_time_format = 1;
                title_suffix       = "from 1950 to 2020";
                xtitle             = "Year";
                xlabel_step        = 10.0;
            }
            else if ( kind == 1 || kind == 2 )
            {
                plctime( 1961, 7, 1, 0, 0, 1.64757 - 0.20, &xmin );
                plctime( 1961, 7, 1, 0, 0, 1.64757 + 0.20, &xmax );
                npts         = 1001;
                ymin         = 1.625;
                ymax         = 1.725;
                time_format  = "%S%2%";
                title_suffix = "near 1961-08-01 (TAI)";
                xlabel_step  = 0.05 / ( scale * 86400. );
                if ( kind == 1 )
                {
                    if_TAI_time_format = 1;
                    xtitle             = "Seconds (TAI)";
                }
                else
                {
                    if_TAI_time_format = 0;
                    xtitle             = "Seconds (TAI) labelled with corresponding UTC";
                }
            }
            else if ( kind == 3 || kind == 4 )
            {
                plctime( 1963, 10, 1, 0, 0, 2.6972788 - 0.20, &xmin );
                plctime( 1963, 10, 1, 0, 0, 2.6972788 + 0.20, &xmax );
                npts         = 1001;
                ymin         = 2.55;
                ymax         = 2.75;
                time_format  = "%S%2%";
                title_suffix = "near 1963-11-01 (TAI)";
                xlabel_step  = 0.05 / ( scale * 86400. );
                if ( kind == 3 )
                {
                    if_TAI_time_format = 1;
                    xtitle             = "Seconds (TAI)";
                }
                else
                {
                    if_TAI_time_format = 0;
                    xtitle             = "Seconds (TAI) labelled with corresponding UTC";
                }
            }
            else if ( kind == 5 || kind == 6 )
            {
                plctime( 2009, 0, 1, 0, 0, 34.0 - 5.0, &xmin );
                plctime( 2009, 0, 1, 0, 0, 34.0 + 5.0, &xmax );
                npts         = 1001;
                ymin         = 32.5;
                ymax         = 34.5;
                time_format  = "%S%2%";
                title_suffix = "near 2009-01-01 (TAI)";
                xlabel_step  = 1. / ( scale * 86400. );
                if ( kind == 5 )
                {
                    if_TAI_time_format = 1;
                    xtitle             = "Seconds (TAI)";
                }
                else
                {
                    if_TAI_time_format = 0;
                    xtitle             = "Seconds (TAI) labelled with corresponding UTC";
                }
            }

            PLINT   tai_year, tai_month, tai_day, tai_hour, tai_min;
            PLFLT   tai_sec, tai;
            PLINT   utc_year, utc_month, utc_day, utc_hour, utc_min;
            PLFLT   utc_sec, utc;

            PLFLT[] x, y;
            x.length = y.length = npts;
            for ( int i = 0; i < npts; i++ )
            {
                x[i] = xmin + i * ( xmax - xmin ) / ( npts - 1 );
                plconfigtime( scale, offset1, offset2, 0x0, 0, 0, 0, 0, 0, 0, 0. );
                tai = x[i];
                plbtime( &tai_year, &tai_month, &tai_day, &tai_hour, &tai_min, &tai_sec, tai );
                plconfigtime( scale, offset1, offset2, 0x2, 0, 0, 0, 0, 0, 0, 0. );
                plbtime( &utc_year, &utc_month, &utc_day, &utc_hour, &utc_min, &utc_sec, tai );
                plconfigtime( scale, offset1, offset2, 0x0, 0, 0, 0, 0, 0, 0, 0. );
                plctime( utc_year, utc_month, utc_day, utc_hour, utc_min, utc_sec, &utc );
                y[i] = ( tai - utc ) * scale * 86400.;
            }

            pladv( 0 );
            plvsta();
            plwind( xmin, xmax, ymin, ymax );
            plcol0( 1 );
            if ( if_TAI_time_format )
                plconfigtime( scale, offset1, offset2, 0x0, 0, 0, 0, 0, 0, 0, 0. );
            else
                plconfigtime( scale, offset1, offset2, 0x2, 0, 0, 0, 0, 0, 0, 0. );
            pltimefmt( time_format );
            plbox( "bcnstd", xlabel_step, 0, "bcnstv", 0., 0 );
            plcol0( 3 );
            string title = "@frPLplot Example 29 - TAI-UTC ";
            title ~= title_suffix;
            pllab( xtitle, "TAI-UTC (sec)", title );

            plcol0( 4 );

            plline( x, y );
        }
    }
}
