//--------------------------------------------------------------------------
//
// File:           nnai.c
//
// Created:        15/11/2002
//
// Author:         Pavel Sakov
//                 CSIRO Marine Research
//
// Purpose:        Code for:
//                 -- Natural Neighbours Array Interpolator
//
// Description:    `nnai' is a tructure for conducting
//                 consequitive Natural Neighbours interpolations on a given
//                 spatial data set in a given array of points. It allows to
//                 modify Z coordinate of data in between interpolations.
//                 `nnai' is the fastest of the three Natural
//                 Neighbours interpolators in `nn' library.
//
// Revisions:      None
//
//--------------------------------------------------------------------------

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "nn.h"
#include "delaunay.h"
#include "nan.h"

typedef struct
{
    int   nvertices;
    int   * vertices;           // vertex indices [nvertices]
    double* weights;            // vertex weights [nvertices]
} nn_weights;

struct nnai
{
    delaunay  * d;
    double    wmin;
    double    n;                // number of output points
    double    * x;              // [n]
    double    * y;              // [n]
    nn_weights* weights;
};

void nn_quit( const char* format, ... );
void nnpi_calculate_weights( nnpi* nn );
int nnpi_get_nvertices( nnpi* nn );
int* nnpi_get_vertices( nnpi* nn );
double* nnpi_get_weights( nnpi* nn );
void nnpi_normalize_weights( nnpi* nn );
void nnpi_reset( nnpi* nn );
void nnpi_set_point( nnpi* nn, point* p );

// Builds Natural Neighbours array interpolator. This includes calculation of
// weights used in nnai_interpolate().
//
// @param d Delaunay triangulation
// @return Natural Neighbours interpolation
//
nnai* nnai_build( delaunay* d, int n, double* x, double* y )
{
    nnai  * nn  = malloc( sizeof ( nnai ) );
    nnpi  * nnp = nnpi_create( d );
    int   * vertices;
    double* weights;
    int   i;

    if ( n <= 0 )
        nn_quit( "nnai_create(): n = %d\n", n );

    nn->d = d;
    nn->n = n;
    nn->x = malloc( (size_t) n * sizeof ( double ) );
    memcpy( nn->x, x, (size_t) n * sizeof ( double ) );
    nn->y = malloc( (size_t) n * sizeof ( double ) );
    memcpy( nn->y, y, (size_t) n * sizeof ( double ) );
    nn->weights = malloc( (size_t) n * sizeof ( nn_weights ) );

    for ( i = 0; i < n; ++i )
    {
        nn_weights* w = &nn->weights[i];
        point     p;

        p.x = x[i];
        p.y = y[i];

        nnpi_reset( nnp );
        nnpi_set_point( nnp, &p );
        nnpi_calculate_weights( nnp );
        nnpi_normalize_weights( nnp );

        vertices = nnpi_get_vertices( nnp );
        weights  = nnpi_get_weights( nnp );

        w->nvertices = nnpi_get_nvertices( nnp );
        w->vertices  = malloc( (size_t) ( w->nvertices ) * sizeof ( int ) );
        memcpy( w->vertices, vertices, (size_t) ( w->nvertices ) * sizeof ( int ) );
        w->weights = malloc( (size_t) ( w->nvertices ) * sizeof ( double ) );
        memcpy( w->weights, weights, (size_t) ( w->nvertices ) * sizeof ( double ) );
    }

    nnpi_destroy( nnp );

    return nn;
}

// Destroys Natural Neighbours array interpolator.
//
// @param nn Structure to be destroyed
//
void nnai_destroy( nnai* nn )
{
    int i;

    for ( i = 0; i < nn->n; ++i )
    {
        nn_weights* w = &nn->weights[i];

        free( w->vertices );
        free( w->weights );
    }

    free( nn->x );
    free( nn->y );
    free( nn->weights );
    free( nn );
}

// Conducts NN interpolation in a fixed array of output points using
// data specified for a fixed array of input points. Uses pre-calculated
// weights.
//
// @param nn NN array interpolator
// @param zin input data [nn->d->npoints]
// @param zout output data [nn->n]. Must be pre-allocated!
//
void nnai_interpolate( nnai* nn, double* zin, double* zout )
{
    int i;

    for ( i = 0; i < nn->n; ++i )
    {
        nn_weights* w = &nn->weights[i];
        double    z   = 0.0;
        int       j;

        for ( j = 0; j < w->nvertices; ++j )
        {
            double weight = w->weights[j];

            if ( weight < nn->wmin )
            {
                z = NaN;
                break;
            }
            z += weight * zin[w->vertices[j]];
        }

        zout[i] = z;
    }
}

//* Sets minimal allowed weight for Natural Neighbours interpolation.
// @param nn Natural Neighbours array interpolator
// @param wmin Minimal allowed weight
//
void nnai_setwmin( nnai* nn, double wmin )
{
    nn->wmin = wmin;
}

// The rest of this file contains a number of test programs.
//
#if defined ( NNAI_TEST )

#include <sys/time.h>

#define NPOINTSIN    10000
#define NMIN         10
#define NX           101
#define NXMIN        1

#define SQ( x )    ( ( x ) * ( x ) )

static double franke( double x, double y )
{
    x *= 9.0;
    y *= 9.0;
    return 0.75 * exp( ( -SQ( x - 2.0 ) - SQ( y - 2.0 ) ) / 4.0 )
           + 0.75 * exp( -SQ( x - 2.0 ) / 49.0 - ( y - 2.0 ) / 10.0 )
           + 0.5 * exp( ( -SQ( x - 7.0 ) - SQ( y - 3.0 ) ) / 4.0 )
           - 0.2 * exp( -SQ( x - 4.0 ) - SQ( y - 7.0 ) );
}

static void usage()
{
    printf(
        "Usage: nn_test [-v|-V] [-n <nin> <nxout>]\n"
        "Options:\n"
        "  -a              -- use non-Sibsonian interpolation rule\n"
        "  -n <nin> <nout>:\n"
        "            <nin> -- number of input points (default = 10000)\n"
        "           <nout> -- number of output points per side (default = 64)\n"
        "  -v              -- verbose\n"
        "  -V              -- very verbose\n"
        );
}

int main( int argc, char* argv[] )
{
    int             nin = NPOINTSIN;
    int             nx = NX;
    int             nout = 0;
    point           * pin = NULL;
    delaunay        * d = NULL;
    point           * pout = NULL;
    nnai            * nn = NULL;
    double          * zin = NULL;
    double          * xout = NULL;
    double          * yout = NULL;
    double          * zout = NULL;
    int             cpi = -1;    // control point index
    struct timeval  tv0, tv1, tv2;
    struct timezone tz;
    int             i;

    i = 1;
    while ( i < argc )
    {
        switch ( argv[i][1] )
        {
        case 'a':
            i++;
            nn_rule = NON_SIBSONIAN;
            break;
        case 'n':
            i++;
            if ( i >= argc )
                nn_quit( "no number of data points found after -i\n" );
            nin = atoi( argv[i] );
            i++;
            if ( i >= argc )
                nn_quit( "no number of ouput points per side found after -i\n" );
            nx = atoi( argv[i] );
            i++;
            break;
        case 'v':
            i++;
            nn_verbose = 1;
            break;
        case 'V':
            i++;
            nn_verbose = 2;
            break;
        default:
            usage();
            break;
        }
    }

    if ( nin < NMIN )
        nin = NMIN;
    if ( nx < NXMIN )
        nx = NXMIN;

    printf( "\nTest of Natural Neighbours array interpolator:\n\n" );
    printf( "  %d data points\n", nin );
    printf( "  %d output points\n", nx * nx );

    //
    // generate data
    //
    printf( "  generating data:\n" );
    fflush( stdout );
    pin = malloc( nin * sizeof ( point ) );
    zin = malloc( nin * sizeof ( double ) );
    for ( i = 0; i < nin; ++i )
    {
        point* p = &pin[i];

        p->x   = (double) random() / RAND_MAX;
        p->y   = (double) random() / RAND_MAX;
        p->z   = franke( p->x, p->y );
        zin[i] = p->z;
        if ( nn_verbose )
            printf( "    (%f, %f, %f)\n", p->x, p->y, p->z );
    }

    //
    // triangulate
    //
    printf( "  triangulating:\n" );
    fflush( stdout );
    d = delaunay_build( nin, pin, 0, NULL, 0, NULL );

    //
    // generate output points
    //
    points_generate2( -0.1, 1.1, -0.1, 1.1, nx, nx, &nout, &pout );
    xout = malloc( nout * sizeof ( double ) );
    yout = malloc( nout * sizeof ( double ) );
    zout = malloc( nout * sizeof ( double ) );
    for ( i = 0; i < nout; ++i )
    {
        point* p = &pout[i];

        xout[i] = p->x;
        yout[i] = p->y;
        zout[i] = NaN;
    }
    cpi = ( nx / 2 ) * ( nx + 1 );

    gettimeofday( &tv0, &tz );

    //
    // create interpolator
    //
    printf( "  creating interpolator:\n" );
    fflush( stdout );
    nn = nnai_build( d, nout, xout, yout );

    fflush( stdout );
    gettimeofday( &tv1, &tz );
    {
        long dt = 1000000 * ( tv1.tv_sec - tv0.tv_sec ) + tv1.tv_usec - tv0.tv_usec;

        printf( "    interpolator creation time = %ld us (%.2f us / point)\n", dt, (double) dt / nout );
    }

    //
    // interpolate
    //
    printf( "  interpolating:\n" );
    fflush( stdout );
    nnai_interpolate( nn, zin, zout );
    if ( nn_verbose )
        for ( i = 0; i < nout; ++i )
            printf( "    (%f, %f, %f)\n", xout[i], yout[i], zout[i] );

    fflush( stdout );
    gettimeofday( &tv2, &tz );
    {
        long dt = 1000000.0 * ( tv2.tv_sec - tv1.tv_sec ) + tv2.tv_usec - tv1.tv_usec;

        printf( "    interpolation time = %ld us (%.2f us / point)\n", dt, (double) dt / nout );
    }

    if ( !nn_verbose )
        printf( "    control point: (%f, %f, %f) (expected z = %f)\n", xout[cpi], yout[cpi], zout[cpi], franke( xout[cpi], yout[cpi] ) );

    printf( "  interpolating one more time:\n" );
    fflush( stdout );
    nnai_interpolate( nn, zin, zout );
    if ( nn_verbose )
        for ( i = 0; i < nout; ++i )
            printf( "    (%f, %f, %f)\n", xout[i], yout[i], zout[i] );

    fflush( stdout );
    gettimeofday( &tv0, &tz );
    {
        long dt = 1000000.0 * ( tv0.tv_sec - tv2.tv_sec ) + tv0.tv_usec - tv2.tv_usec;

        printf( "    interpolation time = %ld us (%.2f us / point)\n", dt, (double) dt / nout );
    }

    if ( !nn_verbose )
        printf( "    control point: (%f, %f, %f) (expected z = %f)\n", xout[cpi], yout[cpi], zout[cpi], franke( xout[cpi], yout[cpi] ) );

    printf( "  entering new data:\n" );
    fflush( stdout );
    for ( i = 0; i < nin; ++i )
    {
        point* p = &pin[i];

        p->z   = p->x * p->x - p->y * p->y;
        zin[i] = p->z;
        if ( nn_verbose )
            printf( "    (%f, %f, %f)\n", p->x, p->y, p->z );
    }

    printf( "  interpolating:\n" );
    fflush( stdout );
    nnai_interpolate( nn, zin, zout );
    if ( nn_verbose )
        for ( i = 0; i < nout; ++i )
            printf( "    (%f, %f, %f)\n", xout[i], yout[i], zout[i] );

    if ( !nn_verbose )
        printf( "    control point: (%f, %f, %f) (expected z = %f)\n", xout[cpi], yout[cpi], zout[cpi], xout[cpi] * xout[cpi] - yout[cpi] * yout[cpi] );

    printf( "  restoring data:\n" );
    fflush( stdout );
    for ( i = 0; i < nin; ++i )
    {
        point* p = &pin[i];

        p->z   = franke( p->x, p->y );
        zin[i] = p->z;
        if ( nn_verbose )
            printf( "    (%f, %f, %f)\n", p->x, p->y, p->z );
    }

    printf( "  interpolating:\n" );
    fflush( stdout );
    nnai_interpolate( nn, zin, zout );
    if ( nn_verbose )
        for ( i = 0; i < nout; ++i )
            printf( "    (%f, %f, %f)\n", xout[i], yout[i], zout[i] );

    if ( !nn_verbose )
        printf( "    control point: (%f, %f, %f) (expected z = %f)\n", xout[cpi], yout[cpi], zout[cpi], franke( xout[cpi], yout[cpi] ) );

    printf( "\n" );

    nnai_destroy( nn );
    free( zin );
    free( xout );
    free( yout );
    free( zout );
    free( pout );
    delaunay_destroy( d );
    free( pin );

    return 0;
}

#endif
