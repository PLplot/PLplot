//--------------------------------------------------------------------------
// $Id$
//--------------------------------------------------------------------------
//
//--------------------------------------------------------------------------
// Copyright (C) 2004  Andrew Ross
// Copyright (C) 2004  Alan W. Irwin
//
// This file is part of PLplot.
//
// PLplot is free software; you can redistribute it and/or modify
// it under the terms of the GNU Library General Public License as published by
// the Free Software Foundation; version 2 of the License.
//
// PLplot is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Library General Public License for more details.
//
// You should have received a copy of the GNU Library General Public License
// along with PLplot; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA
//--------------------------------------------------------------------------
//
//--------------------------------------------------------------------------
// Implementation of PLplot example 19 in C++.
//--------------------------------------------------------------------------

#include "plc++demos.h"

#ifdef PL_USE_NAMESPACE
using namespace std;
#endif

class x19 {
public:
    x19( int, const char ** );

private:
    // Class data
    plstream *pls;
};

void
map_transform( PLFLT x, PLFLT y, PLFLT *xt, PLFLT *yt, PLPointer /* data */ )
{
    double radius;

    radius = 90.0 - y;
    *xt    = radius * cos( x * M_PI / 180.0 );
    *yt    = radius * sin( x * M_PI / 180.0 );
}

//--------------------------------------------------------------------------
// mapform19
//
// Defines specific coordinate transformation for example 19.
// Not to be confused with mapform in src/plmap.c.
// x[], y[] are the coordinates to be plotted.
//--------------------------------------------------------------------------

void
mapform19( PLINT n, PLFLT *x, PLFLT *y )
{
    int    i;
    double xp, yp, radius;
    for ( i = 0; i < n; i++ )
    {
        radius = 90.0 - y[i];
        xp     = radius * cos( x[i] * M_PI / 180.0 );
        yp     = radius * sin( x[i] * M_PI / 180.0 );
        x[i]   = xp;
        y[i]   = yp;
    }
}

// "Normalize" longitude values so that they always fall between -180.0 and
// 180.0
PLFLT
normalize_longitude( PLFLT lon )
{
    PLFLT times;
    if ( lon >= -180.0 && lon <= 180.0 )
    {
        return ( lon );
    }
    else
    {
        times = floor( ( fabs( lon ) + 180.0 ) / 360.0 );
        if ( lon < 0.0 )
        {
            return ( lon + 360.0 * times );
        }
        else
        {
            return ( lon - 360.0 * times );
        }
    }
}

// A custom axis labeling function for longitudes and latitudes.
void
geolocation_labeler( PLINT axis, PLFLT value, char *label, PLINT length, PLPointer /* data */ )
{
    const char *direction_label = "";
    PLFLT      label_val        = 0.0;

    if ( axis == PL_Y_AXIS )
    {
        label_val = value;
        if ( label_val > 0.0 )
        {
            direction_label = " N";
        }
        else if ( label_val < 0.0 )
        {
            direction_label = " S";
        }
        else
        {
            direction_label = "Eq";
        }
    }
    else if ( axis == PL_X_AXIS )
    {
        label_val = normalize_longitude( value );
        if ( label_val > 0.0 )
        {
            direction_label = " E";
        }
        else if ( label_val < 0.0 )
        {
            direction_label = " W";
        }
        else
        {
            direction_label = "";
        }
    }
    if ( axis == PL_Y_AXIS && value == 0.0 )
    {
        // A special case for the equator
        snprintf( label, length, "%s", direction_label );
    }
    else
    {
        snprintf( label, length, "%.0f%s", fabs( label_val ), direction_label );
    }
}


x19::x19( int argc, const char ** argv )
{
    PLFLT minx, maxx, miny, maxy;

    // plplot initialization

    pls = new plstream();

    // Parse and process command line arguments.
    pls->parseopts( &argc, argv, PL_PARSE_FULL );

    // Longitude (x) and latitude (y)

    miny = -70;
    maxy = 80;

    // Initialize PLplot.
    pls->init();
    // Cartesian plots
    // Most of world

    minx = 190;
    maxx = 190 + 360;

    // Setup a custom latitude and longitude-based scaling function.
    pls->slabelfunc( geolocation_labeler, NULL );

    pls->col0( 1 );
    pls->env( minx, maxx, miny, maxy, 1, 70 );
    pls->map( NULL, "usaglobe", minx, maxx, miny, maxy );

    // The Americas

    minx = 190;
    maxx = 340;

    pls->col0( 1 );
    pls->env( minx, maxx, miny, maxy, 1, 70 );
    pls->map( NULL, "usaglobe", minx, maxx, miny, maxy );

    // Clear the labeling function
    pls->slabelfunc( NULL, NULL );

    // Polar, Northern hemisphere
    minx = 0;
    maxx = 360;

    pls->env( -75., 75., -75., 75., 1, -1 );
    pls->map( mapform19, "globe", minx, maxx, miny, maxy );

    pls->lsty( 2 );
    pls->meridians( mapform19, 10.0, 10.0, 0.0, 360.0, -10.0, 80.0 );

    // Polar, Northern hemisphere, this time with a PLplot-wide transform

    minx = 0;
    maxx = 360;

    pls->stransform( map_transform, NULL );

    pls->lsty( 1 );
    pls->env( -75., 75., -75., 75., 1, -1 );
    // No need to set the map transform here as the global transform will be
    // used.
    pls->map( NULL, "globe", minx, maxx, miny, maxy );

    pls->lsty( 2 );
    pls->meridians( NULL, 10.0, 10.0, 0.0, 360.0, -10.0, 80.0 );

    // Show Baltimore, MD on the map
    pls->col0( 2 );
    pls->ssym( 0.0, 2.0 );
    PLFLT x = -76.6125;
    PLFLT y = 39.2902778;
    pls->poin( 1, &x, &y, 18 );
    pls->ssym( 0.0, 1.0 );
    pls->ptex( -76.6125, 43.0, 0.0, 0.0, 0.0, "Baltimore, MD" );

    // For C, this is how the global transform is cleared
    pls->stransform( NULL, NULL );
    delete pls;
}

int main( int argc, const char ** argv )
{
    x19 *x = new x19( argc, argv );

    delete x;
}


//--------------------------------------------------------------------------
//                              End of x19.cc
//--------------------------------------------------------------------------
