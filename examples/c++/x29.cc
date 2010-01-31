// $Id$
//
//       Sample plots using date / time formatting for axes
//
// Copyright (C) 2007 Andrew Ross
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

#include "plc++demos.h"

#ifdef PL_USE_NAMESPACE
using namespace std;
#endif

#include <ctime>

class x29 {
public:
    x29( int, const char** );

private:
    plstream *pls;

    PLFLT    x[365], y[365];
    PLFLT    xerr1[365], xerr2[365], yerr1[365], yerr2[365];

    // Function prototypes
    void plot1();
    void plot2();
    void plot3();
    void plot4();

    PLFLT MIN( PLFLT x, PLFLT y ) { return ( x < y ? x : y ); };
    PLFLT MAX( PLFLT x, PLFLT y ) { return ( x > y ? x : y ); };
};

//--------------------------------------------------------------------------
// // main
//
// Draws several plots which demonstrate the use of date / time formats for
// the axis labels.
// Time formatting is done using the system strftime routine. See the
// documentation of this for full details of the available formats.
//
// 1) Plotting temperature over a day (using hours / minutes)
// 2) Plotting
//
// Note: Times are stored as seconds since the epoch (usually 1st Jan 1970).
//
//--------------------------------------------------------------------------

x29::x29( int argc, const char *argv[] )
{
    pls = new plstream();

    // Parse command line arguments
    pls->parseopts( &argc, argv, PL_PARSE_FULL );

    // Initialize plplot
    pls->init();

    pls->sesc( '@' );

    plot1();

    plot2();

    plot3();

    plot4();

    delete pls;
}

// Plot a model diurnal cycle of temperature
void
x29::plot1()
{
    int   i, npts;
    PLFLT xmin, xmax, ymin, ymax;

    // Data points every 10 minutes for 1 day
    npts = 73;

    xmin = 0;
    xmax = 60.0 * 60.0 * 24.0; // Number of seconds in a day
    ymin = 10.0;
    ymax = 20.0;

    for ( i = 0; i < npts; i++ )
    {
        x[i] = xmax * ( (PLFLT) i / (PLFLT) npts );
        y[i] = 15.0 - 5.0 * cos( 2 * M_PI * ( (PLFLT) i / (PLFLT) npts ) );
        // Set x error bars to +/- 5 minute
        xerr1[i] = x[i] - 60 * 5;
        xerr2[i] = x[i] + 60 * 5;
        // Set y error bars to +/- 0.1 deg C
        yerr1[i] = y[i] - 0.1;
        yerr2[i] = y[i] + 0.1;
    }

    pls->adv( 0 );

    // Rescale major ticks marks by 0.5
    pls->smaj( 0.0, 0.5 );
    // Rescale minor ticks and error bar marks by 0.5
    pls->smin( 0.0, 0.5 );

    pls->vsta();
    pls->wind( xmin, xmax, ymin, ymax );

    // Draw a box with ticks spaced every 3 hour in X and 1 degree C in Y.
    pls->col0( 1 );
    // Set time format to be hours:minutes
    pls->timefmt( "%H:%M" );
    pls->box( "bcnstd", 3.0 * 60 * 60, 3, "bcnstv", 1, 5 );

    pls->col0( 3 );
    pls->lab( "Time (hours:mins)", "Temperature (degC)", "@frPLplot Example 29 - Daily temperature" );

    pls->col0( 4 );

    pls->line( npts, x, y );
    pls->col0( 2 );
    pls->errx( npts, xerr1, xerr2, y );
    pls->col0( 3 );
    pls->erry( npts, x, yerr1, yerr2 );

    /* Rescale major / minor tick marks back to default */
    pls->smin( 0.0, 1.0 );
    pls->smaj( 0.0, 1.0 );
}

// Plot the number of hours of daylight as a function of day for a year
void
x29::plot2()
{
    int   j, npts;
    PLFLT xmin, xmax, ymin, ymax;
    PLFLT lat, p, d;

    // Latitude for London
    lat = 51.5;

    npts = 365;

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
        p    = asin( 0.39795 * cos( 0.2163108 + 2 * atan( 0.9671396 * tan( 0.00860 * ( j - 186 ) ) ) ) );
        d    = 24.0 - ( 24.0 / M_PI ) *
               acos( ( sin( 0.8333 * M_PI / 180.0 ) + sin( lat * M_PI / 180.0 ) * sin( p ) ) /
            ( cos( lat * M_PI / 180.0 ) * cos( p ) ) );
        y[j] = d;
    }

    pls->col0( 1 );
    // Set time format to be abbreviated month name followed by day of month
    pls->timefmt( "%b %d" );
    pls->prec( 1, 1 );
    pls->env( xmin, xmax, ymin, ymax, 0, 40 );


    pls->col0( 3 );
    pls->lab( "Date", "Hours of daylight", "@frPLplot Example 29 - Hours of daylight at 51.5N" );

    pls->col0( 4 );

    pls->line( npts, x, y );

    pls->prec( 0, 0 );
}

void
x29::plot3()
{
    int   i, npts;
    PLFLT xmin, xmax, ymin, ymax;
    PLFLT tstart;

    /* Calculate continuous time corresponding to 2005-12-01 UTC. */
    pls->ctime( 2005, 11, 01, 0, 0, 0., tstart );

    npts = 62;

    xmin = tstart;
    xmax = xmin + npts * 60.0 * 60.0 * 24.0;
    ymin = 0.0;
    ymax = 5.0;

    for ( i = 0; i < npts; i++ )
    {
        x[i] = xmin + i * 60.0 * 60.0 * 24.0;
        y[i] = 1.0 + sin( 2 * M_PI * ( (PLFLT) i ) / 7.0 ) +
               exp( ( (PLFLT) MIN( i, npts - i ) ) / 31.0 );
    }
    pls->adv( 0 );

    pls->vsta();
    pls->wind( xmin, xmax, ymin, ymax );

    pls->col0( 1 );
    // Set time format to be ISO 8601 standard YYYY-MM-HH. Note that this is
    // equivalent to %f for C99 compliant implementations of strftime.
    pls->timefmt( "%Y-%m-%d" );
    // Draw a box with ticks spaced every 14 days in X and 1 hour in Y.
    pls->box( "bcnstd", 14 * 24.0 * 60.0 * 60.0, 14, "bcnstv", 1, 4 );

    pls->col0( 3 );
    pls->lab( "Date", "Hours of television watched", "@frPLplot Example 29 - Hours of television watched in Dec 2005 / Jan 2006" );

    pls->col0( 4 );

    // Rescale symbol size (used by plpoin) by 0.5
    pls->ssym( 0.0, 0.5 );
    pls->poin( npts, x, y, 2 );
    pls->line( npts, x, y );
}

void
x29::plot4()
{
    /* TAI-UTC (seconds) as a function of time.
     * Use Besselian epochs as the continuous time interval just to prove
     * this does not introduce any issues. */

    PLFLT scale, offset1, offset2;
    PLFLT xmin, xmax, ymin, ymax, xlabel_step;
    int   kind, npts, if_TAI_time_format, i;
    char  time_format[10];
    char  title_suffix[100];
    char  xtitle[100];
    char  title[100];
    PLFLT x[1001], y[1001];
    PLINT tai_year, tai_month, tai_day, tai_hour, tai_min;
    PLFLT tai_sec, tai;
    PLINT utc_year, utc_month, utc_day, utc_hour, utc_min;
    PLFLT utc_sec, utc;

    /* Use the definition given in http://en.wikipedia.org/wiki/Besselian_epoch
     * B = 1900. + (JD -2415020.31352)/365.242198781
     * ==> (as calculated with aid of "bc -l" command)
     * B = (MJD + 678940.364163900)/365.242198781
     * ==>
     * MJD = B*365.24219878 - 678940.364163900 */
    scale   = 365.242198781;
    offset1 = -678940.;
    offset2 = -0.3641639;
    pls->configtime( scale, offset1, offset2, 0x0, 0, 0, 0, 0, 0, 0, 0. );

    for ( kind = 0; kind < 7; kind++ )
    {
        if ( kind == 0 )
        {
            pls->ctime( 1950, 0, 2, 0, 0, 0., xmin );
            pls->ctime( 2020, 0, 2, 0, 0, 0., xmax );
            npts = 70 * 12 + 1;
            ymin = 0.0;
            ymax = 36.0;
            strncpy( time_format, "%Y%", 10 );
            if_TAI_time_format = 1;
            strncpy( title_suffix, "from 1950 to 2020", 100 );
            strncpy( xtitle, "Year", 100 );
            xlabel_step = 10.;
        }
        else if ( kind == 1 || kind == 2 )
        {
            pls->ctime( 1961, 7, 1, 0, 0, 1.64757 - .20, xmin );
            pls->ctime( 1961, 7, 1, 0, 0, 1.64757 + .20, xmax );
            npts = 1001;
            ymin = 1.625;
            ymax = 1.725;
            strncpy( time_format, "%S%2%", 10 );
            strncpy( title_suffix, "near 1961-08-01 (TAI)", 100 );
            xlabel_step = 0.05 / ( scale * 86400. );
            if ( kind == 1 )
            {
                if_TAI_time_format = 1;
                strncpy( xtitle, "Seconds (TAI)", 100 );
            }
            else
            {
                if_TAI_time_format = 0;
                strncpy( xtitle, "Seconds (TAI) labelled with corresponding UTC", 100 );
            }
        }
        else if ( kind == 3 || kind == 4 )
        {
            pls->ctime( 1963, 10, 1, 0, 0, 2.6972788 - .20, xmin );
            pls->ctime( 1963, 10, 1, 0, 0, 2.6972788 + .20, xmax );
            npts = 1001;
            ymin = 2.55;
            ymax = 2.75;
            strncpy( time_format, "%S%2%", 10 );
            strncpy( title_suffix, "near 1963-11-01 (TAI)", 100 );
            xlabel_step = 0.05 / ( scale * 86400. );
            if ( kind == 3 )
            {
                if_TAI_time_format = 1;
                strncpy( xtitle, "Seconds (TAI)", 100 );
            }
            else
            {
                if_TAI_time_format = 0;
                strncpy( xtitle, "Seconds (TAI) labelled with corresponding UTC", 100 );
            }
        }
        else if ( kind == 5 || kind == 6 )
        {
            pls->ctime( 2009, 0, 1, 0, 0, 34. - 5., xmin );
            pls->ctime( 2009, 0, 1, 0, 0, 34. + 5., xmax );
            npts = 1001;
            ymin = 32.5;
            ymax = 34.5;
            strncpy( time_format, "%S%2%", 10 );
            strncpy( title_suffix, "near 2009-01-01 (TAI)", 100 );
            xlabel_step = 1. / ( scale * 86400. );
            if ( kind == 5 )
            {
                if_TAI_time_format = 1;
                strncpy( xtitle, "Seconds (TAI)", 100 );
            }
            else
            {
                if_TAI_time_format = 0;
                strncpy( xtitle, "Seconds (TAI) labelled with corresponding UTC", 100 );
            }
        }

        for ( i = 0; i < npts; i++ )
        {
            x[i] = xmin + i * ( xmax - xmin ) / ( (double) ( npts - 1 ) );
            pls->configtime( scale, offset1, offset2, 0x0, 0, 0, 0, 0, 0, 0, 0. );
            tai = x[i];
            pls->btime( tai_year, tai_month, tai_day, tai_hour, tai_min, tai_sec, tai );
            pls->configtime( scale, offset1, offset2, 0x2, 0, 0, 0, 0, 0, 0, 0. );
            pls->btime( utc_year, utc_month, utc_day, utc_hour, utc_min, utc_sec, tai );
            pls->configtime( scale, offset1, offset2, 0x0, 0, 0, 0, 0, 0, 0, 0. );
            pls->ctime( utc_year, utc_month, utc_day, utc_hour, utc_min, utc_sec, utc );
            y[i] = ( tai - utc ) * scale * 86400.;
        }

        pls->adv( 0 );
        pls->vsta();
        pls->wind( xmin, xmax, ymin, ymax );
        pls->col0( 1 );
        if ( if_TAI_time_format )
            pls->configtime( scale, offset1, offset2, 0x0, 0, 0, 0, 0, 0, 0, 0. );
        else
            pls->configtime( scale, offset1, offset2, 0x2, 0, 0, 0, 0, 0, 0, 0. );
        pls->timefmt( time_format );
        pls->box( "bcnstd", xlabel_step, 0, "bcnstv", 0., 0 );
        pls->col0( 3 );
        strncpy( title, "@frPLplot Example 29 - TAI-UTC ", 100 );
        strncat( title, title_suffix, 100 );
        pls->lab( xtitle, "TAI-UTC (sec)", title );

        pls->col0( 4 );

        pls->line( npts, x, y );
    }
}

int main( int argc, const char **argv )
{
    x29 *x = new x29( argc, argv );

    delete x;
}
