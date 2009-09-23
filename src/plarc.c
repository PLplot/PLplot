/* plarc()
 *
 * Copyright (C) 2009  Hezekiah M. Carty
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
 */

#include "plplotP.h"

#define CIRCLE_SEGMENTS    PL_MAXPOLY
#define DEG_TO_RAD( x )                    (( x ) * M_PI / 180.0 )

#define PLARC_POINT_X( x, a, b, theta )    (( x ) + (( a ) * cos( theta )))
#define PLARC_POINT_Y( y, a, b, theta )    (( y ) + (( b ) * sin( theta )))

/*-------------------------------------------------------------------------
 * plarc_approx : Plot an approximated arc with a series of lines
 *
 * Takes the same arguments, with the same units, as c_plarc below.
 *
 *-------------------------------------------------------------------------*/
void
plarc_approx( PLFLT x,
    PLFLT y,
    PLFLT a,
    PLFLT b,
    PLFLT angle1,
    PLFLT angle2,
    PLBOOL fill )
{
    PLINT i;
    PLFLT theta0, theta_step, theta, d_angle;
    PLINT segments;
    PLFLT xs[CIRCLE_SEGMENTS], ys[CIRCLE_SEGMENTS];

    /* The difference between the start and end angles */
    d_angle = DEG_TO_RAD( angle2 - angle1 );
    if ( fabs( d_angle ) > M_PI * 2.0 )
        d_angle = M_PI * 2.0;

    /* The number of line segments used to approximate the arc */
    segments = d_angle / ( 2.0 * M_PI ) * CIRCLE_SEGMENTS;
    /* Always use at least 2 arc points, otherwise fills will break. */
    if ( segments < 2 )
        segments = 2;
    /* The start angle in radians and number of radians in each approximating
     * segment. */
    theta0 = DEG_TO_RAD( angle1 );

    theta_step = d_angle / ( segments - 1 );

    /* The coordinates for the circle outline */
    for ( i = 0; i < segments; i++ )
    {
        theta = theta0 + theta_step * (PLFLT) i;
        xs[i] = PLARC_POINT_X( x, a, b, theta );
        ys[i] = PLARC_POINT_Y( y, a, b, theta );
    }

    if ( fill )
    {
        /* Add the center point if we aren't drawing a circle */
        if ( fabs( d_angle ) < M_PI * 2.0 )
        {
            xs[segments - 1] = x;
            ys[segments - 1] = y;
        }
        /* Draw a filled arc */
        plfill( segments, xs, ys );
    }
    else
    {
        /* Draw the arc outline */
        plline( segments, xs, ys );
    }
}

/*-------------------------------------------------------------------------
 * plarc : Plot an arc
 *
 * Takes the following arguments:
 *
 *   x, y:
 *      x and y coordinates for the center of the arc
 *
 *   a, b:
 *      Radius of the arc's major and minor axes
 *
 *   angle1:
 *      Start angle (degrees)
 *
 *   angle2:
 *      End angle (degrees)
 *
 *   fill:
 *      Should the arc be filled?
 *
 *-------------------------------------------------------------------------*/
void
c_plarc( PLFLT x,
    PLFLT y,
    PLFLT a,
    PLFLT b,
    PLFLT angle1,
    PLFLT angle2,
    PLBOOL fill )
{
    PLINT      xscl[2], yscl[2];
    PLINT      clpxmi, clpxma, clpymi, clpyma;
    arc_struct *arc_info;

    /* TODO: For now, only unrotated plots use the driver-accelerated path. */
    if ( plsc->dev_arc && plsc->diorot == 0 )
    {
        arc_info = (arc_struct *) malloc((size_t) sizeof ( arc_struct ));

        xscl[0] = plP_wcpcx( x - a );
        xscl[1] = plP_wcpcx( x + a );
        yscl[0] = plP_wcpcy( y - b );
        yscl[1] = plP_wcpcy( y + b );
        difilt( xscl, yscl, 2, &clpxmi, &clpxma, &clpymi, &clpyma );

        arc_info->x = 0.5 * ( xscl[1] + xscl[0] );
        arc_info->y = 0.5 * ( yscl[1] + yscl[0] );
        arc_info->a = 0.5 * ( xscl[1] - xscl[0] );
        arc_info->b = 0.5 * ( yscl[1] - yscl[0] );

        arc_info->angle1 = angle1;
        arc_info->angle2 = angle2;
        arc_info->fill   = fill;

        plP_esc( PLESC_ARC, arc_info );

        free( arc_info );
    }
    else
    {
        plarc_approx( x, y, a, b, angle1, angle2, fill );
    }
}

