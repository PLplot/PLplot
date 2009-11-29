/* $Id$
 *
 *      Implement linear gradients for PLplot.
 *
 * Copyright (C) 2009  Alan W. Irwin
 *
 * This file is part of PLplot.
 *
 * PLplot is free software; you can redistribute it and/or modify
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

#include "plplotP.h"

/* software fallback for gradient. */
static void
plgradient_soft( PLINT n, PLFLT *x, PLFLT *y, PLFLT angle );

/* define where plshades plots gradient for software fallback for
 * gradient.  */

static int
gradient_defined( PLFLT x, PLFLT y );

/*----------------------------------------------------------------------*\
 * void plgradient()
 *
 * Draws a linear gradient at an angle relative to the increasing x
 * direction for the polygon bounded by the x and y vertices.  x, and
 * y are expressed in world coordinates, and angle (in the world
 * coordinate system) is expressed in degrees.  The gradient is
 * expressed using colour and transparency information from cmap1.  The
 * geometrical gradient direction is specified by the angle argument.
 * The 0. to 1. range of the independent variable of cmap1 corresponds
 * to the range of the polygon in the direction specified by angle.
 \*----------------------------------------------------------------------*/

void
c_plgradient( PLINT n, PLFLT *x, PLFLT *y, PLFLT angle )
{
    if ( plsc->level < 3 )
    {
        plabort( "plgradient: Please set up window first" );
        return;
    }
    if ( n < 3 )
    {
        plabort( "plgradient: Not enough vertices in polygon" );
        return;
    }

    if ( !plsc->dev_gradient )
    {
        plgradient_soft( n, x, y, angle );
    }
    else
    {
        plsc->gradient_angle = PI * angle / 180.;
        PLINT i, xpoly[PL_MAXPOLY], ypoly[PL_MAXPOLY];
        if ( n > PL_MAXPOLY - 1 )
        {
            plwarn( "plgradient: too many points in polygon" );
            n = PL_MAXPOLY;
        }
        for ( i = 0; i < n; i++ )
        {
            xpoly[i] = plP_wcpcx( x[i] );
            ypoly[i] = plP_wcpcy( y[i] );
        }
        if ( x[0] != x[n - 1] || y[0] != y[n - 1] )
        {
            if ( n < PL_MAXPOLY ) n++;
            xpoly[n - 1] = plP_wcpcx( x[0] );
            ypoly[n - 1] = plP_wcpcy( y[0] );
        }
        plP_plfclp( xpoly, ypoly, n, plsc->clpxmi, plsc->clpxma,
            plsc->clpymi, plsc->clpyma, plP_gradient );
    }
}

/*----------------------------------------------------------------------*\
 * void plgradient_soft()
 *
 * Software fallback for gradient.  See c_plgradient for an explanation
 * of the arguments.
 \*----------------------------------------------------------------------*/

void
plgradient_soft( PLINT n, PLFLT *x, PLFLT *y, PLFLT angle )
{
    PLFLT xrot, xrot_min, xrot_max, cosangle, sinangle;
    PLFLT xmin, xmax, ymin, ymax;
    PLFLT **z, *edge, xcoord, ycoord;
    PLINT i, j;

    if ( n < 3 )
    {
        plabort( "plgradient_soft: Not enough vertices in polygon" );
        return;
    }


    /* Define polygon boundary so it is accessible from gradient_defined. */
    plsc->n_polygon = n;
    plsc->x_polygon = x;
    plsc->y_polygon = y;

    /* Find x and y range of polygon. */
    xmin = x[0];
    xmax = xmin;
    ymin = y[0];
    ymax = ymin;
    /* Also find x range in rotated coordinate system where
     * xrot = x*cosangle + y*sinangle. */
    cosangle = cos( PI / 180. * angle );
    sinangle = sin( PI / 180. * angle );
    xrot     = x[0] * cosangle + y[0] * sinangle;
    xrot_min = xrot;
    xrot_max = xrot;
    for ( i = 1; i < n; i++ )
    {
        if ( x[i] < xmin )
            xmin = x[i];
        else if ( x[i] > xmax )
            xmax = x[i];

        if ( y[i] < ymin )
            ymin = y[i];
        else if ( y[i] > ymax )
            ymax = y[i];

        xrot = x[i] * cosangle + y[i] * sinangle;
        if ( xrot < xrot_min )
            xrot_min = xrot;
        else if ( xrot > xrot_max )
            xrot_max = xrot;
    }

    /* 2 x 2 array more than sufficient to define plane. */
    /* Temporarily use more to overcome irregular edge issue on defined
     * region. */
    #define NX    20
    #define NY    20
    plAlloc2dGrid( &z, NX, NY );
    for ( i = 0; i < NX; i++ )
    {
        xcoord = xmin + ((PLFLT) i ) * ( xmax - xmin ) / (PLFLT) ( NX - 1 );
        for ( j = 0; j < NY; j++ )
        {
            ycoord  = ymin + ((PLFLT) j ) * ( ymax - ymin ) / (PLFLT) ( NY - 1 );
            xrot    = xcoord * cosangle + ycoord * sinangle;
            z[i][j] = ( xrot - xrot_min ) / ( xrot_max - xrot_min );
        }
    }
    /* 101 edges gives reasonably smooth results for example 30. */
    #define NEDGE    101
    /* Define NEDGE shade edges (or NEDGE-1 shade levels)
     * from 0. to 1. */
    if (( edge = (PLFLT *) malloc( NEDGE * sizeof ( PLFLT ))) == NULL )
        plexit( "plgradient_soft: Insufficient memory" );
    for ( i = 0; i < NEDGE; i++ )
        edge[i] = (PLFLT) i / (PLFLT) ( NEDGE - 1 );

    plshades( z, NX, NY, gradient_defined, xmin, xmax, ymin, ymax,
        edge, NEDGE, 0, 0, 0, plfill, 1, NULL, NULL );
    free((void *) edge );
    plFree2dGrid( z, NX, NY );
}

PLINT
gradient_defined( PLFLT x, PLFLT y )
{
    return plP_pointinpolygon( plsc->n_polygon, plsc->x_polygon, plsc->y_polygon,
        x, y );
}
