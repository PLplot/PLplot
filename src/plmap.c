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

#ifdef HAVE_SHAPELIB
#include <shapefil.h>

SHPHandle
OpenShapeFile( PLCHAR_VECTOR fn );

#ifdef HAVE_SAHOOKS
static void
CustomErrors( PLCHAR_VECTOR message );
#endif //HAVE_SAHOOKS

#define OpenMap     OpenShapeFile
#define CloseMap    SHPClose

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
checkwrap( PLMAPFORM_callback mapform, PLFLT lon, PLFLT lat )
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
drawmapdata( PLMAPFORM_callback mapform, int shapetype, PLINT n, PLFLT *x, PLFLT *y, PLFLT dx, PLFLT dy, PLFLT just, PLCHAR_VECTOR text )
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
    else if ( shapetype == SHPT_ARCZ || shapetype == SHPT_ARCM )
        plline( n, x, y );
    else if ( shapetype == SHPT_POLYGON || shapetype == SHPT_POLYGONZ || shapetype == SHPT_POLYGONM )
        plfill( n, x, y );
    else if ( shapetype == SHPT_POINT || shapetype == SHPT_POINTM || shapetype == SHPT_POINTZ )
        for ( i = 0; i < n; ++i )
            plptex( x[i], y[i], dx, dy, just, text );
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
drawmap( PLMAPFORM_callback mapform, PLCHAR_VECTOR name,
         PLFLT dx, PLFLT dy, int shapetype, PLFLT just, PLCHAR_VECTOR text,
         PLFLT minx, PLFLT maxx, PLFLT miny, PLFLT maxy, PLINT_VECTOR plotentries, PLINT nplotentries )
{
    int    i, j;
    char   *filename = NULL;
    char   warning[1024];
    int    nVertices = 200;
    PLFLT  minsectlon, maxsectlon, minsectlat, maxsectlat;
    PLFLT  *bufx   = NULL, *bufy = NULL;
    int    bufsize = 0;
    size_t filenamelen;
    PLFLT  **splitx             = NULL;
    PLFLT  **splity             = NULL;
    int    *splitsectionlengths = NULL;
    int    nsplitsections;
    PLFLT  lastsplitpointx;
    PLFLT  lastsplitpointy;
    PLFLT  penultimatesplitpointx;
    PLFLT  penultimatesplitpointy;
    char   islatlon     = 1;
    int    appendresult = 0;


    SHPHandle in;
    int       nentries;
    int       entryindex = 0;
    // Unnecessarily set nparts to quiet -O3 -Wuninitialized warnings.
    //int              nparts      = 0;
    int       entrynumber = 0;
    int       partnumber  = 0;
    double    mins[4];
    double    maxs[4];
    SHPObject *object = NULL;
    double    *bufxraw;
    double    *bufyraw;
    char      *prjfilename = NULL;
    PDFstrm   *prjfile;
    char      prjtype[] = { 0, 0, 0, 0, 0, 0, 0 };

    //
    // read map outline
    //

    //strip the .shp extension if a shapefile has been provided
    if ( strstr( name, ".shp" ) )
        filenamelen = ( strlen( name ) - 4 );
    else
        filenamelen = strlen( name );
    filename = (char *) malloc( filenamelen + 1 );
    if ( !filename )
    {
        plabort( "Could not allocate memory for map filename root" );
        return;
    }
    strncpy( filename, name, filenamelen );
    filename[ filenamelen ] = '\0';

    //Open the shp and shx file using shapelib
    if ( ( in = OpenShapeFile( filename ) ) == NULL )
    {
        strcpy( warning, "Could not find " );
        strcat( warning, filename );
        strcat( warning, " file." );
        plabort( warning );
        free( filename );
        return;
    }
    SHPGetInfo( in, &nentries, &shapetype, mins, maxs );
    //also check for a prj file which will tell us if the data is lat/lon or projected
    //if it is projected then set ncopies to 1 - i.e. don't wrap round longitudes
    prjfilename = (char *) malloc( filenamelen + 5 );
    if ( !prjfilename )
    {
        free( filename );
        plabort( "Could not allocate memory for generating map projection filename" );
        return;
    }
    strncpy( prjfilename, name, filenamelen );
    prjfilename[ filenamelen ] = '\0';
    strcat( prjfilename, ".prj" );
    prjfile = plLibOpenPdfstrm( prjfilename );
    if ( prjfile && prjfile->file )
    {
        fread( prjtype, 1, 6, prjfile->file );
        if ( strcmp( prjtype, "PROJCS" ) == 0 )
            islatlon = 0;
        pdf_close( prjfile );
    }
    free( prjfilename );
    prjfilename = NULL;

    bufx = NULL;
    bufy = NULL;

    for (;; )
    {
        //each object in the shapefile is split into parts.
        //If we are need to plot the first part of an object then read in a new object
        //and check how many parts it has. Otherwise use the object->panPartStart vector
        //to check the offset of this part and the next part and allocate memory. Copy
        //the data to this memory converting it to PLFLT and draw it.
        //finally increment the part number or if we have finished with the object reset the
        //part numberand increment the object.

        //break condition if we've reached the end of the file
        if ( ( !plotentries && ( entrynumber == nentries ) ) || ( plotentries && ( entryindex == nplotentries ) ) )
            break;

        //if partnumber == 0 then we need to load the next object
        if ( partnumber == 0 )
        {
            if ( plotentries )
                object = SHPReadObject( in, plotentries[entryindex] );
            else
                object = SHPReadObject( in, entrynumber );
        }
        //if the object could not be read, increment the object index to read and
        //return to the top of the loop to try the next object.
        if ( object == NULL )
        {
            entrynumber++;
            entryindex++;
            partnumber = 0;
            continue;
        }

        //work out how many points are in the current part
        if ( object->nParts == 0 )
            nVertices = object->nVertices;                                                       //if object->nParts==0, we can still have 1 vertex. A bit odd but it's the way it goes
        else if ( partnumber == ( object->nParts - 1 ) )
            nVertices = object->nVertices - object->panPartStart[partnumber];                    //panPartStart holds the offset for each part
        else
            nVertices = object->panPartStart[partnumber + 1] - object->panPartStart[partnumber]; //panPartStart holds the offset for each part
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

        //point the plot buffer to the correct starting vertex
        //and copy it to the PLFLT arrays. If we had object->nParts == 0
        //then panPartStart will be NULL
        if ( object->nParts > 0 )
        {
            bufxraw = object->padfX + object->panPartStart[partnumber];
            bufyraw = object->padfY + object->panPartStart[partnumber];
        }
        else
        {
            bufxraw = object->padfX;
            bufyraw = object->padfY;
        }

        for ( i = 0; i < nVertices; i++ )
        {
            bufx[i] = (PLFLT) bufxraw[i];
            bufy[i] = (PLFLT) bufyraw[i];
        }

        //set the min x/y of the object
        minsectlon = object->dfXMin;
        maxsectlon = object->dfXMax;
        minsectlat = object->dfYMin;
        maxsectlat = object->dfYMax;

        //increment the partnumber or if we've reached the end of
        //an entry increment the entrynumber and set partnumber to 0
        if ( partnumber == object->nParts - 1 || object->nParts == 0 )
        {
            entrynumber++;
            entryindex++;
            partnumber = 0;
            SHPDestroyObject( object );
            object = NULL;
        }
        else
            partnumber++;

        if ( nVertices == 0 )
            continue;

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
    SHPClose( in );

    //free memory
    free( bufx );
    free( bufy );
    free( filename );
}
#endif //HAVE_SHAPELIB


//--------------------------------------------------------------------------
// void plmap(PLMAPFORM_callback mapform, PLCHAR_VECTOR name,
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
plmap( PLMAPFORM_callback mapform, PLCHAR_VECTOR name,
       PLFLT minx, PLFLT maxx, PLFLT miny, PLFLT maxy )
{
#ifdef HAVE_SHAPELIB
    drawmap( mapform, name, 0.0, 0.0, SHPT_ARC, 0.0, NULL, minx, maxx,
        miny, maxy, NULL, 0 );
#else
    plwarn( "plmap is a no-op because shapelib is not available." );
#endif
}

//--------------------------------------------------------------------------
// void plmapline(PLMAPFORM_callback mapform,
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
plmapline( PLMAPFORM_callback mapform, PLCHAR_VECTOR name,
           PLFLT minx, PLFLT maxx, PLFLT miny, PLFLT maxy,
           PLINT_VECTOR plotentries, PLINT nplotentries )
{
#ifdef HAVE_SHAPELIB
    drawmap( mapform, name, 0.0, 0.0, SHPT_ARC, 0.0, "", minx, maxx,
        miny, maxy, plotentries, nplotentries );
#else
    plwarn( "plmapline is a no-op because shapelib is not available." );
#endif
}

//--------------------------------------------------------------------------
// void plmapstring(PLMAPFORM_callback mapform,
//		PLCHAR_VECTOR name, PLFLT just, PLCHAR_VECTOR string,
//		PLFLT minx, PLFLT maxx, PLFLT miny, PLFLT maxy,
//		PLINT_VECTOR plotentries, PLINT nplotentries);
//
//As per plmapline but plots symbols. The map equivalent of plstring. string
//has the same meaning as in plstring.
//--------------------------------------------------------------------------
void
plmapstring( PLMAPFORM_callback mapform,
             PLCHAR_VECTOR name, PLCHAR_VECTOR string,
             PLFLT minx, PLFLT maxx, PLFLT miny, PLFLT maxy,
             PLINT_VECTOR plotentries, PLINT nplotentries )
{
#ifdef HAVE_SHAPELIB
    drawmap( mapform, name, 1.0, 0.0, SHPT_POINT, 0.5, string, minx, maxx,
        miny, maxy, plotentries, nplotentries );
#else
    plwarn( "plmapstring is a no-op because shapelib is not available." );
#endif
}

//--------------------------------------------------------------------------
// void plmaptex(PLMAPFORM_callback mapform,
//		PLCHAR_VECTOR name, PLFLT dx, PLFLT dy PLFLT just, PLCHAR_VECTOR text,
//		PLFLT minx, PLFLT maxx, PLFLT miny, PLFLT maxy,
//		PLINT plotentry);
//
//As per plmapline but plots text. The map equivalent of plptex. dx, dy,
//just and text have the same meaning as in plptex.
//--------------------------------------------------------------------------
void
plmaptex( PLMAPFORM_callback mapform,
          PLCHAR_VECTOR name, PLFLT dx, PLFLT dy, PLFLT just, PLCHAR_VECTOR text,
          PLFLT minx, PLFLT maxx, PLFLT miny, PLFLT maxy,
          PLINT plotentry )
{
#ifdef HAVE_SHAPELIB
    drawmap( mapform, name, dx, dy, SHPT_POINT, just, text, minx, maxx,
        miny, maxy, &plotentry, 1 );
#else
    plwarn( "plmaptex is a no-op because shapelib is not available." );
#endif
}

//--------------------------------------------------------------------------
// void plmapfill(PLMAPFORM_callback mapform,
//		PLCHAR_VECTOR name, PLFLT minx, PLFLT maxx, PLFLT miny,
//		PLFLT maxy, PLINT_VECTOR plotentries, PLINT nplotentries);
//
//As per plmapline but plots a filled polygon. The map equivalent to
//plfill. Uses the pattern defined by plsty or plpat.
//--------------------------------------------------------------------------
void
plmapfill( PLMAPFORM_callback mapform,
           PLCHAR_VECTOR name, PLFLT minx, PLFLT maxx, PLFLT miny,
           PLFLT maxy, PLINT_VECTOR plotentries, PLINT nplotentries )
{
#ifdef HAVE_SHAPELIB
    drawmap( mapform, name, 0.0, 0.0, SHPT_POLYGON, 0.0, NULL, minx, maxx,
        miny, maxy, plotentries, nplotentries );
#else
    plwarn( "plmapfill is a no-op because shapelib is not available." );
#endif
}

//--------------------------------------------------------------------------
// void plmeridians(PLMAPFORM_callback mapform,
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
plmeridians( PLMAPFORM_callback mapform,
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

//--------------------------------------------------------------------------
// SHPHandle OpenShapeFile(fn)
//
//! Returns a handle to a shapefile from the filename fn. Content based on
//! plLibOpenPdfstrm in plctrl.c
//! Locations checked:
//!	PLPLOT_LIB_ENV = $(PLPLOT_LIB)
//!	current directory
//!	PLPLOT_HOME_ENV/lib = $(PLPLOT_HOME)/lib
//!	DATA_DIR
//!	PLLIBDEV
//!
//! @param fn Name of the file.
//!
//! @Return handle to a shapefile on success or NULL if the file cannot be
//! found
//--------------------------------------------------------------------------
#ifdef HAVE_SHAPELIB
#ifdef HAVE_SAHOOKS
// Our thanks to Frank Warmerdam, the developer of shapelib for suggesting
// this approach for quieting shapelib "Unable to open" error messages.
static
void CustomErrors( PLCHAR_VECTOR message )
{
    if ( strstr( message, "Unable to open" ) == NULL )
        fprintf( stderr, "%s\n", message );
}
#endif

SHPHandle
OpenShapeFile( PLCHAR_VECTOR fn )
{
    SHPHandle file;
    char      *fs = NULL, *dn = NULL;
#ifdef HAVE_SAHOOKS
    SAHooks   sHooks;

    SASetupDefaultHooks( &sHooks );
    sHooks.Error = CustomErrors;
#else
    // Using ancient version of shapelib without SAHooks or SHPOpenLL.
    // For this case live with the misleading "Unable to open" error
    // messages.
    // int sHooks;
#define SHPOpenLL( a, b, c )    SHPOpen( a, b )
#endif //HAVE_SAHOOKS

//***   search build tree               ***

    if ( plInBuildTree() == 1 )
    {
        plGetName( SOURCE_DIR, "data", fn, &fs );

        if ( ( file = SHPOpenLL( fs, "rb", &sHooks ) ) != NULL )
            goto done;
    }

//***	search PLPLOT_LIB_ENV = $(PLPLOT_LIB)	***

#if defined ( PLPLOT_LIB_ENV )
    if ( ( dn = getenv( PLPLOT_LIB_ENV ) ) != NULL )
    {
        plGetName( dn, "", fn, &fs );

        if ( ( file = SHPOpenLL( fs, "rb", &sHooks ) ) != NULL )
            goto done;
        fprintf( stderr, PLPLOT_LIB_ENV "=\"%s\"\n", dn ); // what IS set?
    }
#endif  // PLPLOT_LIB_ENV

//***	search current directory	***

    if ( ( file = SHPOpenLL( fn, "rb", &sHooks ) ) != NULL )
    {
        pldebug( "OpenShapeFile", "Found file %s in current directory.\n", fn );
        free_mem( fs );
        return ( file );
    }

//***	search PLPLOT_HOME_ENV/lib = $(PLPLOT_HOME)/lib	***

#if defined ( PLPLOT_HOME_ENV )
    if ( ( dn = getenv( PLPLOT_HOME_ENV ) ) != NULL )
    {
        plGetName( dn, "lib", fn, &fs );

        if ( ( file = SHPOpenLL( fs, "rb", &sHooks ) ) != NULL )
            goto done;
        fprintf( stderr, PLPLOT_HOME_ENV "=\"%s\"\n", dn ); // what IS set?
    }
#endif  // PLPLOT_HOME_ENV/lib

//***   search installed location	***

#if defined ( DATA_DIR )
    plGetName( DATA_DIR, "", fn, &fs );

    if ( ( file = SHPOpenLL( fs, "rb", &sHooks ) ) != NULL )
        goto done;
#endif  // DATA_DIR

//***   search hardwired location	***

#ifdef PLLIBDEV
    plGetName( PLLIBDEV, "", fn, &fs );

    if ( ( file = SHPOpenLL( fs, "rb", &sHooks ) ) != NULL )
        goto done;
#endif  // PLLIBDEV

//***   not found, give up      ***
    pldebug( "OpenShapeFile", "File %s not found.\n", fn );
    free_mem( fs );
    return NULL;

done:
    pldebug( "OpenShapeFile", "SHPOpen successfully opened two files with basename %s\n", fs );
    free_mem( fs );
    return ( file );
}
#endif //ifdef HAVE_SHAPELIB
