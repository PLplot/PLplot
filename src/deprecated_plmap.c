//      Continental Outline and Political Boundary Backgrounds
//
//      Some plots need a geographical background such as the global
//      surface temperatures or the population density.  The routine
//      plmap() will draw one of the following backgrounds: continental
//      outlines, political boundaries, the United States, and the United
//      States with the continental outlines.  The routine plmeridians()
//      will add the latitudes and longitudes to the background.  After
//      the background has been drawn, one can use a contour routine or a
//      symbol plotter to finish off the plot.
//
//      Copyright (C) 1991, 1993, 1994  Wesley Ebisuzaki
//      Copyright (C) 1994, 2000, 2001  Maurice LeBrun
//      Copyright (C) 1999  Geoffrey Furnish
//      Copyright (C) 2000-2016 Alan W. Irwin
//      Copyright (C) 2001  Andrew Roach
//      Copyright (C) 2001, 2004  Rafael Laboissiere
//      Copyright (C) 2002  Vincent Darley
//      Copyright (C) 2003  Joao Cardoso
//
//      This file is part of PLplot.
//
//      PLplot is free software; you can redistribute it and/or modify
//      it under the terms of the GNU Library General Public License
//      as published by the Free Software Foundation; version 2 of the
//      License.
//
//      PLplot is distributed in the hope that it will be useful, but
//      WITHOUT ANY WARRANTY; without even the implied warranty of
//      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//      GNU Library General Public License for more details.
//
//      You should have received a copy of the GNU Library General Public
//      License along with this library; if not, write to the Free Software
//      Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301
//      USA
//
//

#define DEBUG
#define NEED_PLDEBUG

#include "plplotP.h"


#define MAP_FILE        ".map"
#define OpenMap         plLibOpenPdfstrm
#define CloseMap        pdf_close
#define OFFSET          ( 180 * 100 )
#define SCALE           100.0
#define W_BUFSIZ        ( 32 * 1024 )
#define SHPT_ARC        1
#define SHPT_POINT      2
#define SHPT_POLYGON    3

//redistributes the lon value onto either 0-360 or -180-180 for wrapping
//purposes.
void
rebaselon( PLFLT *lon, PLFLT midlon )
{
    if ( *lon > midlon + 180.0 )
        *lon -= floor( ( *lon - midlon - 180.0 ) / 360.0 + 1.0 ) * 360.0;
    else if ( *lon < midlon - 180.0 )
        *lon += floor( ( midlon - 180.0 - *lon ) / 360.0 + 1.0 ) * 360.0;
}

//append a PLFLT to an array of PLFLTs. array is a pointer to the array,
//n is the current size of the array, val is the value to append.
//returns 0 for success, 1 for could not allocate new memory. If memory
//could not be allocated then the previous array remains intact
int appendflt( PLFLT **array, size_t n, PLFLT val )
{
    size_t i;
    PLFLT  *temp = (PLFLT *) malloc( ( n + 1 ) * sizeof ( PLFLT ) );
    if ( !temp )
        return 1;
    for ( i = 0; i < n; ++i )
        temp[i] = ( *array )[i];
    temp[n] = val;
    free( *array );
    *array = temp;
    return 0;
}

//As for appendflt, but for an array of ints
int appendint( int **array, size_t n, int val )
{
    size_t i;
    int    *temp = (int *) malloc( ( n + 1 ) * sizeof ( int ) );
    if ( !temp )
        return 1;
    for ( i = 0; i < n; ++i )
        temp[i] = ( *array )[i];
    temp[n] = val;
    free( *array );
    *array = temp;
    return 0;
}

//As for appendflt, but for an array of pointers to PLFLTs
int appendfltptr( PLFLT ***array, size_t n, PLFLT *val )
{
    size_t i;
    PLFLT  **temp = (PLFLT **) malloc( ( n + 1 ) * sizeof ( PLFLT * ) );
    if ( !temp )
        return 1;
    for ( i = 0; i < n; ++i )
        temp[i] = ( *array )[i];
    temp[n] = val;
    free( *array );
    *array = temp;
    return 0;
}

//Check to see if the mapform wraps round longitudes. For example, a polar
// projection wraps round longitudes, but a cylindrical projection does not.
//Returns 1 if the mapform wraps or 0 if not.
char
checkwrap( void ( *mapform )( PLINT, PLFLT *, PLFLT * ), PLFLT lon, PLFLT lat )
{
    PLFLT x[] = { lon };
    PLFLT y[] = { lat };
    PLFLT resultx;
    PLFLT resulty;

    if ( !mapform )
        return 0;

    mapform( 1, x, y );
    resultx = x[0];
    resulty = y[0];
    x[0]    = lon + 360;
    y[0]    = lat;
    return ( ( ABS( x[0] - resultx ) < 1.0e-12 ) && ( ABS( y[0] - resulty ) < 1.0e-12 ) );
}

//--------------------------------------------------------------------------
//Actually draw the map lines points and text.
//--------------------------------------------------------------------------
void
drawmapdata( void ( *mapform )( PLINT, PLFLT *, PLFLT * ), int shapetype, PLINT n, PLFLT *x, PLFLT *y, PLFLT dx, PLFLT dy, PLFLT just, PLCHAR_VECTOR text )
{
    PLINT i;

    //do the transform if needed
    if ( mapform != NULL )
        ( *mapform )( n, x, y );

    if ( shapetype == SHPT_ARC )
        plline( n, x, y );
    else if ( shapetype == SHPT_POINT )
        for ( i = 0; i < n; ++i )
            plptex( x[i], y[i], dx, dy, just, text );
    else if ( shapetype == SHPT_POLYGON )
        plfill( n, x, y );
}


//--------------------------------------------------------------------------
//This is a function called by the front end map functions to do the map drawing. Its
//parameters are:
//mapform: The transform used to convert the data in raw coordinates to x, y positions
//on the plot
//name: either one of the plplot provided lat/lon maps or the path/file name of a
//shapefile
//dx/dy: the gradient of text/symbols drawn if text is non-null
//shapetype: one of ARC, SHPT_ARCZ, SHPT_ARCM, SHPT_POLYGON, SHPT_POLYGONZ,
//SHPT_POLYGONM, SHPT_POINT, SHPT_POINTM, SHPT_POINTZ. See drawmapdata() for the
//how each type is rendered. But Basically the ARC options are lines, the POLYGON
//options are filled polygons, the POINT options are points/text. Options beginning
//SHPT will only be defined if HAVE_SHAPELIB is true
//text: The text (which can be actual text or a unicode symbol) to be drawn at
//each point
//minx/maxx: The min/max longitude when using a plplot provided map or x value if
//using a shapefile
//miny/maxy: The min/max latitude when using a plplot provided map or y value if
//using a shapefile
//plotentries: used only for shapefiles, as one shapefile contains multiple vectors
//each representing a different item (e.g. multiple boundaries, multiple height
//contours etc. plotentries is an array containing the indices of the
//entries within the shapefile that you wish to plot. if plotentries is null all
//entries are plotted
//nplotentries: the number of elements in plotentries. Ignored if plplot was not built
//with shapefile support or if plotentries is null
//--------------------------------------------------------------------------
void
drawmap( void ( *mapform )( PLINT, PLFLT *, PLFLT * ), PLCHAR_VECTOR name,
         PLFLT dx, PLFLT dy, int shapetype, PLFLT just, PLCHAR_VECTOR text,
         PLFLT minx, PLFLT maxx, PLFLT miny, PLFLT maxy, PLINT_VECTOR plotentries, PLINT nplotentries )
{
    int           i, j;
    char          *filename = NULL;
    char          truncatedfilename[900];
    char          warning[1024];
    int           nVertices = 200;
    PLFLT         minsectlon, maxsectlon, minsectlat, maxsectlat;
    PLFLT         *bufx   = NULL, *bufy = NULL;
    int           bufsize = 0;
    int           filenamelen;
    PLFLT         **splitx             = NULL;
    PLFLT         **splity             = NULL;
    int           *splitsectionlengths = NULL;
    int           nsplitsections;
    PLFLT         lastsplitpointx;
    PLFLT         lastsplitpointy;
    PLFLT         penultimatesplitpointx;
    PLFLT         penultimatesplitpointy;
    char          islatlon     = 1;
    int           appendresult = 0;

    PDFstrm       *in;
    //PLFLT            bufx[ncopies][200], bufy[ncopies][200];
    unsigned char n_buff[2], buff[800];
    long int      t;

    //
    // read map outline
    //

    //strip the .shp extension if a shapefile has been provided and add
    //the needed map file extension if we are not using shapefile
    if ( strstr( name, ".shp" ) )
        filenamelen = (int) ( name - strstr( name, ".shp" ) );
    else
        filenamelen = (int) strlen( name );
    filename = (char *) malloc( filenamelen + strlen( MAP_FILE ) + 1 );
    if ( !filename )
    {
        plabort( "Could not allocate memory for concatenating map filename" );
        return;
    }
    strncpy( filename, name, filenamelen );
    filename[ filenamelen ] = '\0';
    strcat( filename, MAP_FILE );

    //copy the filename to a fixed length array in case it is needed for warning messages
    if ( strlen( filename ) < 899 )
        strcpy( truncatedfilename, filename );
    else
    {
        memcpy( truncatedfilename, filename, 896 );
        truncatedfilename[896] = '.';
        truncatedfilename[897] = '.';
        truncatedfilename[898] = '.';
        truncatedfilename[899] = '\0';
    }

    strcpy( warning, "Could not find " );
    strcat( warning, filename );
    strcat( warning, " file." );
    if ( ( in = plLibOpenPdfstrm( filename ) ) == NULL )
    {
        plwarn( warning );
        return;
    }

    bufx = NULL;
    bufy = NULL;

    for (;; )
    {
        //allocate memory for the data
        if ( nVertices > bufsize )
        {
            bufsize = nVertices;
            free( bufx );
            free( bufy );
            bufx = (PLFLT *) malloc( (size_t) bufsize * sizeof ( PLFLT ) );
            bufy = (PLFLT *) malloc( (size_t) bufsize * sizeof ( PLFLT ) );
            if ( !bufx || !bufy )
            {
                plabort( "Could not allocate memory for map data" );
                free( filename );
                free( bufx );
                free( bufy );
                return;
            }
        }

        // read in # points in segment
        if ( pdf_rdx( n_buff, (long) sizeof ( unsigned char ) * 2, in ) == 0 )
            break;
        nVertices = ( n_buff[0] << 8 ) + n_buff[1];
        if ( nVertices == 0 )
            break;

        pdf_rdx( buff, (long) sizeof ( unsigned char ) * 4 * nVertices, in );
        if ( nVertices == 1 )
            continue;

        for ( j = i = 0; i < nVertices; i++, j += 2 )
        {
            t       = ( buff[j] << 8 ) + buff[j + 1];
            bufx[i] = ( (PLFLT) t - OFFSET ) / SCALE;
        }
        for ( i = 0; i < nVertices; i++, j += 2 )
        {
            t       = ( buff[j] << 8 ) + buff[j + 1];
            bufy[i] = ( (PLFLT) t - OFFSET ) / SCALE;
        }
        //set the min/max section lat/lon with extreme values
        //to be overwritten later
        minsectlon = 1000.;
        maxsectlon = -1000.;
        minsectlat = 1000.;
        maxsectlat = -1000.;

        if ( islatlon )
        {
            //two obvious issues exist here with plotting longitudes:
            //
            //1) wraparound causing lines which go the wrong way round
            //   the globe
            //2) some people plot lon from 0-360 deg, others from -180 - +180
            //
            //we can cure these problems by conditionally adding/subtracting
            //360 degrees to each data point in order to ensure that the
            //distance between adgacent points is always less than 180
            //degrees, then plotting up to 2 out of 5 copies of the data
            //each separated by 360 degrees.

            //arrays of pointers to the starts of each section of data that
            //has been split due to longitude wrapping, and an array of ints
            //to hold their lengths. Start with splitx and splity having one
            //element pointing to the beginning of bufx and bufy
            splitx = (PLFLT **) malloc( sizeof ( PLFLT* ) );
            splity = (PLFLT **) malloc( sizeof ( PLFLT* ) );
            //lengths of the split sections
            splitsectionlengths = (int *) malloc( sizeof ( size_t ) );
            if ( !splitx || !splity || !splitsectionlengths )
            {
                plabort( "Could not allocate memory for longitudinally split map data" );
                free( filename );
                free( bufx );
                free( bufy );
                free( splitx );
                free( splity );
                free( splitsectionlengths );
                return;
            }
            splitsectionlengths[0] = nVertices;
            nsplitsections         = 1;
            splitx[0] = bufx;
            splity[0] = bufy;

            //set the min/max lats/lons
            minsectlon = MIN( minsectlon, bufx[0] );
            maxsectlon = MAX( minsectlon, bufx[0] );
            minsectlat = MIN( minsectlat, bufy[0] );
            maxsectlat = MAX( maxsectlat, bufy[0] );
            //ensure our lat and lon are on 0-360 grid and split the
            //data where it wraps.
            rebaselon( &bufx[0], ( minx + maxx ) / 2.0 );
            for ( i = 1; i < nVertices; i++ )
            {
                //put lon into 0-360 degree range
                rebaselon( &bufx[i], ( minx + maxx ) / 2.0 );

                //check if the previous point is more than 180 degrees away
                if ( bufx[i - 1] - bufx[i] > 180. || bufx[i - 1] - bufx[i] < -180. )
                {
                    //check if the map transform deals with wrapping itself, e.g. in a polar projection
                    //in this case give one point overlap to the sections so that lines are contiguous
                    if ( checkwrap( mapform, bufx[i], bufy[i] ) )
                    {
                        appendresult += appendfltptr( &splitx, nsplitsections, bufx + i );
                        appendresult += appendfltptr( &splity, nsplitsections, bufy + i );
                        appendresult += appendint( &splitsectionlengths, nsplitsections, nVertices - i );
                        splitsectionlengths[nsplitsections - 1] -= splitsectionlengths[nsplitsections] - 1;
                        nsplitsections++;
                    }
                    //if the transform doesn't deal with wrapping then allow 2 points overlap to fill in the
                    //edges
                    else
                    {
                        appendresult += appendfltptr( &splitx, nsplitsections, bufx + i - 1 );
                        appendresult += appendfltptr( &splity, nsplitsections, bufy + i - 1 );
                        appendresult += appendint( &splitsectionlengths, nsplitsections, nVertices - i + 1 );
                        splitsectionlengths[nsplitsections - 1] -= splitsectionlengths[nsplitsections] - 2;
                        nsplitsections++;
                    }
                    if ( appendresult > 0 )
                    {
                        plabort( "Could not allocate memory for appending to longitudinally split map data" );
                        free( filename );
                        free( bufx );
                        free( bufy );
                        free( splitx );
                        free( splity );
                        free( splitsectionlengths );
                        return;
                    }
                }

                //update the mins and maxs
                minsectlon = MIN( minsectlon, bufx[i] );
                maxsectlon = MAX( minsectlon, bufx[i] );
                minsectlat = MIN( minsectlat, bufy[i] );
                maxsectlat = MAX( maxsectlat, bufy[i] );
            }

            //check if the latitude and longitude range means we need to plot this section
            if ( ( maxsectlat > miny ) && ( minsectlat < maxy )
                 && ( maxsectlon > minx ) && ( minsectlon < maxx ) )
            {
                //plot each split in turn, now is where we deal with the end points to
                //ensure we draw to the edge of the map
                for ( i = 0; i < nsplitsections; ++i )
                {
                    //check if the first 2 or last 1 points of the split section need
                    //wrapping and add or subtract 360 from them. Note that when the next
                    //section is drawn the code below will undo this if needed
                    if ( splitsectionlengths[i] > 2 )
                    {
                        if ( splitx[i][1] - splitx[i][2] > 180. )
                            splitx[i][1] -= 360.0;
                        else if ( splitx[i][1] - splitx[i][2] < -180. )
                            splitx[i][1] += 360.0;
                    }

                    if ( splitx[i][0] - splitx[i][1] > 180. )
                        splitx[i][0] -= 360.0;
                    else if ( splitx[i][0] - splitx[i][1] < -180. )
                        splitx[i][0] += 360.0;

                    if ( splitx[i][splitsectionlengths[i] - 2] - splitx[i][splitsectionlengths[i] - 1] > 180. )
                        splitx[i][splitsectionlengths[i] - 1] += 360.0;
                    else if ( splitx[i][splitsectionlengths[i] - 2] - splitx[i][splitsectionlengths[i] - 1] < -180. )
                        splitx[i][splitsectionlengths[i] - 1] -= 360.0;

                    //save the last 2 points - they will be needed by the next
                    //split section and will be overwritten by the mapform
                    lastsplitpointx        = splitx[i][splitsectionlengths[i] - 1];
                    lastsplitpointy        = splity[i][splitsectionlengths[i] - 1];
                    penultimatesplitpointx = splitx[i][splitsectionlengths[i] - 2];
                    penultimatesplitpointy = splity[i][splitsectionlengths[i] - 2];

                    //draw the split section
                    drawmapdata( mapform, shapetype, splitsectionlengths[i], splitx[i], splity[i], dx, dy, just, text );

                    for ( j = 1; j < splitsectionlengths[i]; ++j )
                    {
                        if ( ( splitx[i][j] < 200.0 && splitx[i][j - 1] > 260.0 ) || ( splitx[i][j - 1] < 200.0 && splitx[i][j] > 260.0 ) )
                            plwarn( "wrapping error" );
                    }

                    //restore the last 2 points
                    splitx[i][splitsectionlengths[i] - 1] = lastsplitpointx;
                    splity[i][splitsectionlengths[i] - 1] = lastsplitpointy;
                    splitx[i][splitsectionlengths[i] - 2] = penultimatesplitpointx;
                    splity[i][splitsectionlengths[i] - 2] = penultimatesplitpointy;
                }
            }
        }
        else
        {
            drawmapdata( mapform, shapetype, nVertices, bufx, bufy, dx, dy, just, text );
        }

        free( splitx );
        free( splity );
        free( splitsectionlengths );
    }
    // Close map file
    pdf_close( in );

    //free memory
    free( bufx );
    free( bufy );
    free( filename );
}



//--------------------------------------------------------------------------
// void plmap(void (*mapform)(PLINT, PLFLT *, PLFLT *), PLCHAR_VECTOR name,
//            PLFLT minx, PLFLT maxx, PLFLT miny, PLFLT maxy);
//
// plot continental outline in world coordinates
//
// v1.4: machine independant version
// v1.3: replaced plcontinent by plmap, added plmeridians
// v1.2: 2 arguments:  mapform, type of plot
//
// mapform(PLINT n, PLFLT *x, PLFLT *y) is a routine to transform the
// coordinate longitudes and latitudes to a plot coordinate system.  By
// using this transform, we can change from a longitude, latitude
// coordinate to a polar stereographic project, for example.  Initially,
// x[0]..[n-1] are the longitudes and y[0]..y[n-1] are the corresponding
// latitudes.  After the call to mapform(), x[] and y[] should be replaced
// by the corresponding plot coordinates.  If no transform is desired,
// mapform can be replaced by NULL.
//
// name is a character string. The value of this parameter determines the
// name of background. The possible values are,
//
//      "globe"		continental outlines
//      "usa"		USA and state boundaries
//      "cglobe"	continental outlines and countries
//      "usaglobe"	USA, state boundaries and continental outlines
// alternatively the filename of a shapefile can be passed if PLplot has
// been compiled with shapelib. In this case either the base name of the
// file can be passed or the filename including the .shp or .shx suffix.
// Only the .shp and .shx files are used.
//
// minx, maxx are the minimum and maximum untransformed x values to be
// plotted. For the built in plots these are longitudes. For other
//shapefiles these are in the units of the shapefile. The value of minx
//must be less than the values of maxx.
//
// miny, maxy are as per minx and maxx except for the built in plots
//the units are latitude.
//--------------------------------------------------------------------------

void
plmap( void ( *mapform )( PLINT, PLFLT *, PLFLT * ), PLCHAR_VECTOR name,
       PLFLT minx, PLFLT maxx, PLFLT miny, PLFLT maxy )
{
    drawmap( mapform, name, 0.0, 0.0, SHPT_ARC, 0.0, NULL, minx, maxx,
        miny, maxy, NULL, 0 );
}

//--------------------------------------------------------------------------
// void plmapline( void ( *mapform )( PLINT, PLFLT *, PLFLT * ),
//		PLCHAR_VECTOR name, PLFLT minx, PLFLT maxx, PLFLT miny,
//		PLFLT maxy, PLINT_VECTOR plotentries, PLINT nplotentries);

//New version of plmap which allows us to specify which items in a shapefile
//we want to use. parameters are as above but with the plotentries being an
//array containing the indices of the elements we wish to draw and
//nplotentries being the number of items in plotentries.
//If shapefile access was not built into plplot then plotentries and
//nplotentries are ignored. If plotentries is null than all entries are
//drawn and nplotentries is ignored.
//The name distiguishes it from other functions which plot points/text and
//polygons, but note that the type of element in the shapefile need not
//match the type of element drawn - i.e. arc elements from a shapefile could
//be drawn as points using the plmaptex function.
//--------------------------------------------------------------------------
void
plmapline( void ( *mapform )( PLINT, PLFLT *, PLFLT * ), PLCHAR_VECTOR name,
           PLFLT minx, PLFLT maxx, PLFLT miny, PLFLT maxy,
           PLINT_VECTOR plotentries, PLINT nplotentries )
{
//    drawmap( mapform, name, 0.0, 0.0, SHPT_ARC, 0.0, "", minx, maxx,
//      miny, maxy, plotentries, nplotentries );
    plwarn( "plmapline not available when -DPL_DEPRECATED=ON cmake option is used." );
}

//--------------------------------------------------------------------------
// void plmapstring( void ( *mapform )( PLINT, PLFLT *, PLFLT * ),
//		PLCHAR_VECTOR name, PLFLT just, PLCHAR_VECTOR string,
//		PLFLT minx, PLFLT maxx, PLFLT miny, PLFLT maxy,
//		PLINT_VECTOR plotentries, PLINT nplotentries);
//
//As per plmapline but plots symbols. The map equivalent of plstring. string
//has the same meaning as in plstring.
//--------------------------------------------------------------------------
void
plmapstring( void ( *mapform )( PLINT, PLFLT *, PLFLT * ),
             PLCHAR_VECTOR name, PLCHAR_VECTOR string,
             PLFLT minx, PLFLT maxx, PLFLT miny, PLFLT maxy,
             PLINT_VECTOR plotentries, PLINT nplotentries )
{
//    drawmap( mapform, name, 1.0, 0.0, SHPT_POINT, 0.5, string, minx, maxx,
//        miny, maxy, plotentries, nplotentries );
    plwarn( "plmapstring not available when -DPL_DEPRECATED=ON cmake option is used." );
}

//--------------------------------------------------------------------------
// void plmaptex( void ( *mapform )( PLINT, PLFLT *, PLFLT * ),
//		PLCHAR_VECTOR name, PLFLT dx, PLFLT dy PLFLT just, PLCHAR_VECTOR text,
//		PLFLT minx, PLFLT maxx, PLFLT miny, PLFLT maxy,
//		PLINT plotentry);
//
//As per plmapline but plots text. The map equivalent of plptex. dx, dy,
//just and text have the same meaning as in plptex.
//--------------------------------------------------------------------------
void
plmaptex( void ( *mapform )( PLINT, PLFLT *, PLFLT * ),
          PLCHAR_VECTOR name, PLFLT dx, PLFLT dy, PLFLT just, PLCHAR_VECTOR text,
          PLFLT minx, PLFLT maxx, PLFLT miny, PLFLT maxy,
          PLINT plotentry )
{
//    drawmap( mapform, name, dx, dy, SHPT_POINT, just, text, minx, maxx,
//        miny, maxy, &plotentry, 1 );
    plwarn( "plmaptext not available when -DPL_DEPRECATED=ON cmake option is used." );
}

//--------------------------------------------------------------------------
// void plmapfill( void ( *mapform )( PLINT, PLFLT *, PLFLT * ),
//		PLCHAR_VECTOR name, PLFLT minx, PLFLT maxx, PLFLT miny,
//		PLFLT maxy, PLINT_VECTOR plotentries, PLINT nplotentries);
//
//As per plmapline but plots a filled polygon. The map equivalent to
//plfill. Uses the pattern defined by plsty or plpat.
//--------------------------------------------------------------------------
void
plmapfill( void ( *mapform )( PLINT, PLFLT *, PLFLT * ),
           PLCHAR_VECTOR name, PLFLT minx, PLFLT maxx, PLFLT miny,
           PLFLT maxy, PLINT_VECTOR plotentries, PLINT nplotentries )
{
//    drawmap( mapform, name, 0.0, 0.0, SHPT_POLYGON, 0.0, NULL, minx, maxx,
//        miny, maxy, plotentries, nplotentries );
    plwarn( "plmapfill not available when -DPL_DEPRECATED=ON cmake option is used." );
}

//--------------------------------------------------------------------------
// void plmeridians(void (*mapform)(PLINT, PLFLT *, PLFLT *),
//		    PLFLT dlong, PLFLT dlat, PLFLT minx, PLFLT maxx,
//		    PLFLT miny, PLFLT maxy);
//
// Plot the latitudes and longitudes on the background.  The lines
// are plotted in the current color and line style.
//
// mapform(PLINT n, PLFLT *x, PLFLT *y) is a routine to transform the
// coordinate longitudes and latitudes to a plot coordinate system.  By
// using this transform, we can change from a longitude, latitude
// coordinate to a polar stereographic project, for example.  Initially,
// x[0]..x[n-1] are the longitudes and y[0]..y[n-1] are the corresponding
// latitudes.  After the call to mapform(), x[] and y[] should be replaced
// by the corresponding plot coordinates.  If no transform is desired,
// mapform can be replaced by NULL.
//
// dlat, dlong are the interval in degrees that the latitude and longitude
// lines are to be plotted.
//
// minx, maxx are the values of the longitude on the left and right
// side of the plot, respectively. The value of minx must be less than
// the values of maxx, and the values of maxx-minx must be less
// or equal to 360.
//
// miny, maxy are the minimum and maximum latitudes to be plotted on
// the background.  One can always use -90.0 and 90.0 as the boundary
// outside the plot window will be automatically eliminated.  However, the
// program will be faster if one can reduce the size of the background
// plotted.
//--------------------------------------------------------------------------

#define NSEG    100

void
plmeridians( void ( *mapform )( PLINT, PLFLT *, PLFLT * ),
             PLFLT dlong, PLFLT dlat,
             PLFLT minlong, PLFLT maxlong, PLFLT minlat, PLFLT maxlat )
{
    PLFLT yy, xx, temp, x[2], y[2], dx, dy;

    if ( minlong > maxlong )
    {
        temp    = minlong;
        minlong = maxlong;
        maxlong = temp;
    }
    if ( minlat > maxlat )
    {
        temp   = minlat;
        minlat = maxlat;
        maxlat = temp;
    }
    dx = ( maxlong - minlong ) / NSEG;
    dy = ( maxlat - minlat ) / NSEG;

    // latitudes

    for ( yy = dlat * ceil( minlat / dlat ); yy <= maxlat; yy += dlat )
    {
        if ( mapform == NULL )
        {
            plpath( NSEG, minlong, yy, maxlong, yy );
        }
        else
        {
            for ( xx = minlong; xx < maxlong; xx += dx )
            {
                y[0] = y[1] = yy;
                x[0] = xx;
                x[1] = xx + dx;
                ( *mapform )( 2, x, y );
                plline( 2, x, y );
            }
        }
    }

    // longitudes

    for ( xx = dlong * ceil( minlong / dlong ); xx <= maxlong; xx += dlong )
    {
        if ( mapform == NULL )
        {
            plpath( NSEG, xx, minlat, xx, maxlat );
        }
        else
        {
            for ( yy = minlat; yy < maxlat; yy += dy )
            {
                x[0] = x[1] = xx;
                y[0] = yy;
                y[1] = yy + dy;
                ( *mapform )( 2, x, y );
                plline( 2, x, y );
            }
        }
    }
}
