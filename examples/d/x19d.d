//      Illustrates backdrop plotting of world, US maps.
//      Contributed by Wesley Ebisuzaki.
//

import std.math;
import std.string;
import std.c.string;

import plplot;

extern ( C ) {
void
map_transform( PLFLT x, PLFLT y, PLFLT *xt, PLFLT *yt, PLPointer data )
{
    double radius;

    radius = 90.0 - y;
    *xt    = radius * cos( x * PI / 180.0 );
    *yt    = radius * sin( x * PI / 180.0 );
}

//--------------------------------------------------------------------------
// mapform19
//
// Defines specific coordinate transformation for example 19.
// Not to be confused with mapform in src/plmap.c.
// x[], y[] are the coordinates to be plotted.
//--------------------------------------------------------------------------
void mapform19( PLINT n, PLFLT* x, PLFLT* y )
{
    double xp, yp, radius;

    for ( int i = 0; i < n; i++ )
    {
        radius = 90.0 - y[i];
        xp     = radius * cos( x[i] * PI / 180.0 );
        yp     = radius * sin( x[i] * PI / 180.0 );
        x[i]   = xp;
        y[i]   = yp;
    }
}

// A custom axis labeling function for longitudes and latitudes.
void geolocation_labeler( PLINT axis, PLFLT value, char* label, PLINT length, PLPointer data )
{
    string direction_label;
    PLFLT  label_val;
    char   * mlabel;

    if ( axis == PL_Y_AXIS )
    {
        label_val = value;
        if ( label_val > 0.0 )
            direction_label = " N";
        else if ( label_val < 0.0 )
            direction_label = " S";
        else
            direction_label = "Eq";
    }
    else if ( axis == PL_X_AXIS )
    {
        label_val = normalize_longitude( value );
        if ( label_val > 0.0 )
            direction_label = " E";
        else if ( label_val < 0.0 )
            direction_label = " W";
        else
            direction_label = "";
    }

    if ( axis == PL_Y_AXIS && value == 0.0 )
        // A special case for the equator
        strcpy( label, toStringz( direction_label ) );
    else
        strcpy( label, toStringz( format( "%.0f%s", fabs( label_val ), direction_label ) ) );
}
}


// "Normalize" longitude values so that they always fall between -180.0 and
// 180.0
PLFLT normalize_longitude( PLFLT lon )
{
    if ( lon >= -180.0 && lon <= 180.0 )
        return ( lon );
    else
    {
        PLFLT times = floor( ( fabs( lon ) + 180.0 ) / 360.0 );
        if ( lon < 0.0 )
            return ( lon + 360.0 * times );
        else
            return ( lon - 360.0 * times );
    }
}


//--------------------------------------------------------------------------
// main
//
// Shows two views of the world map.
//--------------------------------------------------------------------------
int main( char[][] args )
{
    // Parse and process command line arguments
    plparseopts( args, PL_PARSE_FULL );

    // Longitude (x) and latitude (y)
    PLFLT miny = -70;
    PLFLT maxy = 80;

    plinit();

    // Cartesian plots
    // Most of world
    PLFLT minx = -170;
    PLFLT maxx = minx + 360;

    // Setup a custom latitude and longitude-based scaling function.
    plslabelfunc( &geolocation_labeler, null );

    plcol0( 1 );
    plenv( minx, maxx, miny, maxy, 1, 70 );
    plmap( null, "usaglobe", minx, maxx, miny, maxy );

    // The Americas
    minx = 190;
    maxx = 340;

    plcol0( 1 );
    plenv( minx, maxx, miny, maxy, 1, 70 );
    plmap( null, "usaglobe", minx, maxx, miny, maxy );

    // Clear the labeling function
    plslabelfunc( null, null );

    // Polar, Northern hemisphere
    minx = 0;
    maxx = 360;

    plenv( -75., 75., -75., 75., 1, -1 );
    plmap( &mapform19, "globe", minx, maxx, miny, maxy );

    pllsty( 2 );
    plmeridians( &mapform19, 10.0, 10.0, 0.0, 360.0, -10.0, 80.0 );

    // Polar, Northern hemisphere, this time with a PLplot-wide transform

    minx = 0;
    maxx = 360;

    plstransform( &map_transform, null );

    pllsty( 1 );
    plenv( -75., 75., -75., 75., 1, -1 );
    // No need to set the map transform here as the global transform will be
    // used.
    plmap( null, "globe", minx, maxx, miny, maxy );

    pllsty( 2 );
    plmeridians( null, 10.0, 10.0, 0.0, 360.0, -10.0, 80.0 );

    // Show Baltimore, MD on the map
    plcol0( 2 );
    plssym( 0.0, 2.0 );
    PLFLT x[1] = -76.6125;
    PLFLT y[1] = 39.2902778;
    plpoin( x, y, 18 );
    plssym( 0.0, 1.0 );
    plptex( -76.6125, 43.0, 0.0, 0.0, 0.0, "Baltimore, MD" );

    // For C, this is how the global transform is cleared
    plstransform( null, null );


    plend();
    return 0;
}
