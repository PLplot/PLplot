// $Id$
//
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
//      Copyright (C) 2000, 2001, 2002  Alan W. Irwin
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

#define NEED_PLDEBUG    1

#include "plplotP.h"

#ifdef HAVE_SHAPELIB
#include <shapefil.h>

SHPHandle
OpenShapeFile( const char *fn );

#endif

//--------------------------------------------------------------------------
// void plmap(void (*mapform)(PLINT, PLFLT *, PLFLT *), const char *type,
//            PLFLT minlong, PLFLT maxlong, PLFLT minlat, PLFLT maxlat);
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
// type is a character string. The value of this parameter determines the
// type of background. The possible values are,
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
// minlong, maxlong are the values of the longitude on the left and right
// side of the plot, respectively. The value of minlong must be less than
// the values of maxlong, and the values of maxlong-minlong must be less
// or equal to 360.
//
// minlat, maxlat are the minimum and maximum latitudes to be plotted on
// the background.  One can always use -90.0 and 90.0 as the boundary
// outside the plot window will be automatically eliminated.  However, the
// program will be faster if one can reduce the size of the background
// plotted.
//--------------------------------------------------------------------------

#ifdef HAVE_SHAPELIB
#define MAP_FILE    ""
#define OpenMap     OpenShapeFile
#define CloseMap    SHPClose
#else
#define MAP_FILE    ".map"
#define OpenMap     plLibOpenPdfstrm
#define CloseMap    pdf_close
#define OFFSET      ( 180 * 100 )
#define SCALE       100.0
#define W_BUFSIZ    ( 32 * 1024 )
#endif

void
plmap( void ( *mapform )( PLINT, PLFLT *, PLFLT * ), const char *type,
       PLFLT minlong, PLFLT maxlong, PLFLT minlat, PLFLT maxlat )
{
    int       i, j;
    char      *filename;
    char      *warning;
    int       n = 200;
    PLFLT     minsectlon, maxsectlon, minsectlat, maxsectlat;
    const int ncopies = 5;          //must be odd - original plus copies shifted by multiples of +/- 360
    const int mid     = ncopies / 2 + 1;
    PLFLT     **bufx  = NULL, **bufy = NULL;
    int       bufsize = 0;

#ifdef HAVE_SHAPELIB
    SHPHandle        in;
    int              nentries;
    int              nparts;
    int              entrynumber = 0;
    int              partnumber  = 0;
    int              shapetype;
    double           mins[4];
    double           maxs[4];
    SHPObject        *object = NULL;
    double           *bufxraw;
    double           *bufyraw;
#else
    register PDFstrm *in;
    //PLFLT            bufx[ncopies][200], bufy[ncopies][200];
    unsigned char    n_buff[2], buff[800];
    long int         t;
    int              k;
#endif

    //
    // read map outline
    //
    filename = malloc( strlen( type ) + strlen( MAP_FILE ) + 1 );
    strcpy( filename, type );
    strcat( filename, MAP_FILE );

    warning = malloc( strlen( type ) + strlen( MAP_FILE ) + 50 );
    strcpy( warning, "Could not find " );
    strcat( warning, filename );
    strcat( warning, " file." );
#ifdef HAVE_SHAPELIB
    if ( ( in = OpenShapeFile( filename ) ) == NULL )
    {
        plwarn( warning );
        return;
    }
    SHPGetInfo( in, &nentries, &shapetype, mins, maxs );
#else
    if ( ( in = plLibOpenPdfstrm( filename ) ) == NULL )
    {
        plwarn( warning );
        return;
    }
#endif

    bufx = malloc( ncopies * sizeof ( PLFLT* ) );
    bufy = malloc( ncopies * sizeof ( PLFLT* ) );
    for ( i = 0; i < ncopies; i++ )
    {
        bufx[i] = NULL;
        bufy[i] = NULL;
    }

    for (;; )
    {
#ifdef HAVE_SHAPELIB
        //break condition if we've reached the end of the file
        if ( entrynumber == nentries )
            break;
        //if partnumber == 0 then we need to load the next object
        if ( partnumber == 0 )
        {
            object = SHPReadObject( in, entrynumber );
            nparts = object->nParts;
        }

        //work out how many points are in the current part
        if ( partnumber == ( nparts - 1 ) )
            n = object->nVertices - object->panPartStart[partnumber];
        else
            n = object->panPartStart[partnumber + 1] - object->panPartStart[partnumber];
#endif
        //allocate memory for the data
        if ( n > bufsize )
        {
            bufsize = n;
            for ( i = 0; i < ncopies; i++ )
            {
                if ( bufx[i] )
                    free( bufx[i] );
                if ( bufy[i] )
                    free( bufy[i] );
                bufx[i] = malloc( bufsize * sizeof ( double ) );
                bufy[i] = malloc( bufsize * sizeof ( double ) );
            }
        }

#ifdef HAVE_SHAPELIB
        //point the plot buffer to the correct starting vertex
        //and copy it to the PLFLT arrays
        bufxraw = object->padfX + object->panPartStart[partnumber];
        bufyraw = object->padfY + object->panPartStart[partnumber];
        for ( i = 0; i < n; i++ )
        {
            bufx[mid][i] = (PLFLT) bufxraw[i];
            for ( j = 0; j < ncopies; j++ )
                bufy[j][i] = (PLFLT) bufyraw[i];
        }

        //set the minlat/lon of the object
        minsectlon = object->dfXMin;
        maxsectlon = object->dfXMax;
        minsectlat = object->dfYMin;
        maxsectlat = object->dfYMax;

        //increment the partnumber or if we've reached the end of
        //an entry increment the entrynumber and set partnumber to 0
        if ( partnumber == nparts - 1 )
        {
            entrynumber++;
            partnumber = 0;
        }
        else
            partnumber++;
#else
        // read in # points in segment
        if ( pdf_rdx( n_buff, (long) sizeof ( unsigned char ) * 2, in ) == 0 )
            break;
        n = ( n_buff[0] << 8 ) + n_buff[1];
        if ( n == 0 )
            break;

        pdf_rdx( buff, (long) sizeof ( unsigned char ) * 4 * n, in );
        if ( n == 1 )
            continue;

        for ( j = i = 0; i < n; i++, j += 2 )
        {
            t            = ( buff[j] << 8 ) + buff[j + 1];
            bufx[mid][i] = ( (PLFLT) t - OFFSET ) / SCALE;
        }
        for ( i = 0; i < n; i++, j += 2 )
        {
            t          = ( buff[j] << 8 ) + buff[j + 1];
            bufy[0][i] = ( (PLFLT) t - OFFSET ) / SCALE;
            for ( k = 1; k < ncopies; k++ )
                bufy[k][i] = bufy[0][i];
        }
        //set the min/max section lat/lon with extreme values
        //to be overwritten later
        minsectlon = 1000.;
        maxsectlon = -1000.;
        minsectlat = 1000.;
        maxsectlat = -1000.;

#endif
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

        for ( i = 0; i < n - 1; i++ )
        {
            if ( bufx[mid][i] - bufx[mid][i + 1] > 180. )
                bufx[mid][i + 1] += 360.;
            else if ( bufx[mid][i] - bufx[mid][i + 1] < -180. )
                bufx[mid][i + 1] -= 360.;
        }
        for ( i = 0; i < n; i++ )
        {
            for ( j = 0; j < mid; j++ )
                bufx[j][i] = bufx[mid][i] + 360. * (PLFLT) ( j - mid );
            for ( j = mid + 1; j < ncopies; j++ )
                bufx[j][i] = bufx[mid][i] + 360. * (PLFLT) ( j - mid );
#ifndef HAVE_SHAPELIB
            minsectlon = MIN( minsectlon, bufx[mid][i] );
            maxsectlon = MAX( minsectlon, bufx[mid][i] );
            minsectlat = MIN( minsectlat, bufy[mid][i] );
            maxsectlat = MAX( minsectlat, bufy[mid][i] );
#endif
        }

        //check if the latitude range means we need to plot this section
        if ( ( maxsectlat > minlat ) && ( minsectlat < maxlat ) )
        {
            //check which of the translated maps fall within the
            //range and transform and plot them - note more than one
            //map may be needed due to wrapping
            for ( j = 0; j < ncopies; j++ )
            {
                if ( ( minsectlon + 360. * (PLFLT) ( j - mid ) < maxlong )
                     && ( maxsectlon + 360. * (PLFLT) ( j - mid ) > minlong ) )
                {
                    if ( mapform != NULL )
                        ( *mapform )( n, bufx[j], bufy[j] );
                    plline( n, bufx[j], bufy[j] );
                }
            }
        }



#ifdef HAVE_SHAPELIB
        if ( partnumber == 0 )
            SHPDestroyObject( object );
#endif
    }
    // Close map file
#ifdef HAVE_SHAPELIB
    SHPClose( in );
#else
    pdf_close( in );
#endif

    //free memory
    for ( i = 0; i < ncopies; i++ )
    {
        if ( bufx[i] )
            free( bufx[i] );
        if ( bufy[i] )
            free( bufy[i] );
    }
    free( bufx );
    free( bufy );
    free( filename );
    free( warning );
}

//--------------------------------------------------------------------------
// void plmeridians(void (*mapform)(PLINT, PLFLT *, PLFLT *),
//		    PLFLT dlong, PLFLT dlat, PLFLT minlong, PLFLT maxlong,
//		    PLFLT minlat, PLFLT maxlat);
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
// minlong, maxlong are the values of the longitude on the left and right
// side of the plot, respectively. The value of minlong must be less than
// the values of maxlong, and the values of maxlong-minlong must be less
// or equal to 360.
//
// minlat, maxlat are the minimum and maximum latitudes to be plotted on
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
SHPHandle
OpenShapeFile( const char *fn )
{
    SHPHandle file;
    char      *fs = NULL, *dn = NULL;

//***   search build tree               ***

    if ( plInBuildTree() == 1 )
    {
        plGetName( SOURCE_DIR, "data", fn, &fs );

        if ( ( file = SHPOpen( fs, "rb" ) ) != NULL )
            goto done;
    }

//***	search PLPLOT_LIB_ENV = $(PLPLOT_LIB)	***

#if defined ( PLPLOT_LIB_ENV )
    if ( ( dn = getenv( PLPLOT_LIB_ENV ) ) != NULL )
    {
        plGetName( dn, "", fn, &fs );

        if ( ( file = SHPOpen( fs, "rb" ) ) != NULL )
            goto done;
        fprintf( stderr, PLPLOT_LIB_ENV "=\"%s\"\n", dn ); // what IS set?
    }
#endif  // PLPLOT_LIB_ENV

//***	search current directory	***

    if ( ( file = SHPOpen( fn, "rb" ) ) != NULL )
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

        if ( ( file = SHPOpen( fs, "rb" ) ) != NULL )
            goto done;
        fprintf( stderr, PLPLOT_HOME_ENV "=\"%s\"\n", dn ); // what IS set?
    }
#endif  // PLPLOT_HOME_ENV/lib

//***   search installed location	***

#if defined ( DATA_DIR )
    plGetName( DATA_DIR, "", fn, &fs );

    if ( ( file = SHPOpen( fs, "rb" ) ) != NULL )
        goto done;
#endif  // DATA_DIR

//***   search hardwired location	***

#ifdef PLLIBDEV
    plGetName( PLLIBDEV, "", fn, &fs );

    if ( ( file = SHPOpen( fs, "rb" ) ) != NULL )
        goto done;
#endif  // PLLIBDEV

//***   not found, give up      ***
    pldebug( "OpenShapeFile", "File %s not found.\n", fn );
    free_mem( fs );
    return NULL;

done:
    pldebug( "OpenShapeFile", "Found file %s\n", fs );
    free_mem( fs );
    return ( file );
}
#endif
