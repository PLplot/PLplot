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
plgradient_soft( PLINT n, PLFLT *x, PLFLT *y,
                 PLFLT fmin, PLFLT fmax, PLFLT angle );

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
 * expressed using colour and transparency information from cmap1.
 * fmin >= 0. and fmax <= 1. give the range of independent variable of
 * cmap1 that is used for the gradient corresponding to the range of
 * the polygon in the direction specified by angle.
 \*----------------------------------------------------------------------*/

void
c_plgradient( PLINT n, PLFLT *x, PLFLT *y,
              PLFLT fmin, PLFLT fmax, PLFLT angle )
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

    /* For now this is the only method, but once at least one device driver
     * implements a native linear gradient for a device, this will become
     * the software fallback method for devices that don't have
     * native linear gradient capability. */
    plgradient_soft( n, x, y, fmin, fmax, angle );
}

/*----------------------------------------------------------------------*\
 * void plgradient_soft()
 *
 * Software fallback for gradient.  See c_plgradient for an explanation
 * of the arguments.
 \*----------------------------------------------------------------------*/

void
plgradient_soft( PLINT n, PLFLT *x, PLFLT *y,
                 PLFLT fmin, PLFLT fmax, PLFLT angle )
{
    PLFLT xrot, xrot_min, xrot_max, cosangle, sinangle;
    PLFLT xmin, xmax, ymin, ymax;
    PLFLT **z, *edge;
    PLINT i;

    if ( n < 3 )
    {
        plabort( "plgradient_soft: Not enough vertices in polygon" );
        return;
    }

    if ( fmin < 0. )
    {
        plwarn( "plgradient_soft: fmin < 0. set to 0." );
        fmin = 0.;
    }
    if ( fmax > 1. )
    {
        plwarn( "plgradient_soft: fmax > 1. set to 1." );
        fmax = 1.;
    }
    if ( fmin > fmax )
    {
        plabort( "plgradient_soft: fmin > fmax" );
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
    plAlloc2dGrid( &z, 2, 2 );
    xrot    = xmin * cosangle + ymin * sinangle;
    z[0][0] = (( xrot - xrot_min ) * fmin + ( xrot_max - xrot ) * fmax ) /
              ( xrot_max - xrot_min );
    xrot    = xmax * cosangle + ymin * sinangle;
    z[1][0] = (( xrot - xrot_min ) * fmin + ( xrot_max - xrot ) * fmax ) /
              ( xrot_max - xrot_min );
    xrot    = xmin * cosangle + ymax * sinangle;
    z[0][1] = (( xrot - xrot_min ) * fmin + ( xrot_max - xrot ) * fmax ) /
              ( xrot_max - xrot_min );
    xrot    = xmax * cosangle + ymax * sinangle;
    z[1][1] = (( xrot - xrot_min ) * fmin + ( xrot_max - xrot ) * fmax ) /
              ( xrot_max - xrot_min );

    /* 101 edges gives reasonably smooth results for example 30. */
    #define NEDGE    101
    /* Define NEDGE shade edges (or NEDGE-1 shade levels)
     * from fmin to fmax. */
    if (( edge = (PLFLT *) malloc( NEDGE * sizeof ( PLFLT ))) == NULL )
        plexit( "plgradient_soft: Insufficient memory" );
    for ( i = 0; i < NEDGE; i++ )
        edge[i] = fmin + ( fmax - fmin ) * (PLFLT) i / (PLFLT) ( NEDGE - 1 );

    /* For some reason, gradient_defined doesn't work correctly yet so use NULL
     * until this issue is sorted out.
     * plshades( z, 2, 2, gradient_defined, xmin, xmax, ymin, ymax,
     * edge, NEDGE, 0, -1, 2, plfill, 0, NULL, NULL ); */
    plshades( z, 2, 2, NULL, xmin, xmax, ymin, ymax,
        edge, NEDGE, 0, -1, 2, plfill, 0, NULL, NULL );
    free((void *) edge );
    plFree2dGrid( z, 2, 2 );
}

PLINT
gradient_defined( PLFLT x, PLFLT y )
{
    return PLP_pointinpolygon( plsc->n_polygon, plsc->x_polygon, plsc->y_polygon,
        x, y );
}
