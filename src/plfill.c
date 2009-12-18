/* $Id$
 *
 *      Polygon pattern fill.
 *
 * Copyright (C) 2004, 2005, 2006, 2007, 2008, 2009  Alan W. Irwin
 * Copyright (C) 2005, 2006, 2007, 2008, 2009  Arjen Markus
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

#define INSIDE( ix, iy )    ( BETW( ix, xmin, xmax ) && BETW( iy, ymin, ymax ))

#define DTOR    ( PI / 180. )
#define BINC    50

struct point
{
    PLINT x, y;
};
static PLINT bufferleng, buffersize, *buffer;

/* Static function prototypes */

static int
compar( const void *, const void * );

static void
addcoord( PLINT, PLINT );

static void
tran( PLINT *, PLINT *, PLFLT, PLFLT );

static void
buildlist( PLINT, PLINT, PLINT, PLINT, PLINT, PLINT, PLINT );

static int
notpointinpolygon( PLINT n, const PLINT *x, const PLINT *y, PLINT xp, PLINT yp );

static int
circulation( PLINT *x, PLINT *y, PLINT npts );

static void
fill_intersection_polygon( PLINT recursion_depth, PLINT ifextrapolygon,
                           void ( *fill )( short *, short *, PLINT ),
                           const PLINT *x1, const PLINT *y1,
                           PLINT i1start, PLINT n1,
                           const PLINT *x2, const PLINT *y2,
                           const PLINT *if2, PLINT n2 );

static int
notintersect( PLINT *xintersect, PLINT *yintersect,
              PLINT xA1, PLINT yA1, PLINT xA2, PLINT yA2,
              PLINT xB1, PLINT yB1, PLINT xB2, PLINT yB2 );

/*----------------------------------------------------------------------*\
 * void plfill()
 *
 * Pattern fills the polygon bounded by the input points.
 * If hardware fill is used, a maximum of PL_MAXPOLY-1 vertices is allowed.
 * The final point is explicitly added if it doesn't match up to the first,
 * to prevent clipping problems.
 \*----------------------------------------------------------------------*/

void
c_plfill( PLINT n, PLFLT *x, PLFLT *y )
{
    PLINT xpoly[PL_MAXPOLY], ypoly[PL_MAXPOLY];
    PLINT i;

    if ( plsc->level < 3 )
    {
        plabort( "plfill: Please set up window first" );
        return;
    }
    if ( n < 3 )
    {
        plabort( "plfill: Not enough points in object" );
        return;
    }
    if ( n > PL_MAXPOLY - 1 )
    {
        plwarn( "plfill: too many points in polygon" );
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
        plsc->clpymi, plsc->clpyma, plP_fill );
}

/*----------------------------------------------------------------------*\
 * void plfill3()
 *
 * Pattern fills the polygon in 3d bounded by the input points.
 * If hardware fill is used, a maximum of PL_MAXPOLY-1 vertices is allowed.
 * The final point is explicitly added if it doesn't match up to the first,
 * to prevent clipping problems.
 \*----------------------------------------------------------------------*/

void
c_plfill3( PLINT n, PLFLT *x, PLFLT *y, PLFLT *z )
{
    PLFLT tx[PL_MAXPOLY], ty[PL_MAXPOLY], tz[PL_MAXPOLY];
    PLFLT *V[3];
    PLINT xpoly[PL_MAXPOLY], ypoly[PL_MAXPOLY];
    PLINT i;
    PLFLT xmin, xmax, ymin, ymax, zmin, zmax, zscale;

    if ( plsc->level < 3 )
    {
        plabort( "plfill3: Please set up window first" );
        return;
    }
    if ( n < 3 )
    {
        plabort( "plfill3: Not enough points in object" );
        return;
    }
    if ( n > PL_MAXPOLY - 1 )
    {
        plwarn( "plfill3: too many points in polygon" );
        n = PL_MAXPOLY;
    }

    plP_gdom( &xmin, &xmax, &ymin, &ymax );
    plP_grange( &zscale, &zmin, &zmax );

    /* copy the vertices so we can clip without corrupting the input */
    for ( i = 0; i < n; i++ )
    {
        tx[i] = x[i]; ty[i] = y[i]; tz[i] = z[i];
    }
    if ( tx[0] != tx[n - 1] || ty[0] != ty[n - 1] || tz[0] != tz[n - 1] )
    {
        if ( n < PL_MAXPOLY ) n++;
        tx[n - 1] = tx[0]; ty[n - 1] = ty[0]; tz[n - 1] = tz[0];
    }
    V[0] = tx; V[1] = ty; V[2] = tz;
    n    = plP_clip_poly( n, V, 0, 1, -xmin );
    n    = plP_clip_poly( n, V, 0, -1, xmax );
    n    = plP_clip_poly( n, V, 1, 1, -ymin );
    n    = plP_clip_poly( n, V, 1, -1, ymax );
    n    = plP_clip_poly( n, V, 2, 1, -zmin );
    n    = plP_clip_poly( n, V, 2, -1, zmax );
    for ( i = 0; i < n; i++ )
    {
        xpoly[i] = plP_wcpcx( plP_w3wcx( tx[i], ty[i], tz[i] ));
        ypoly[i] = plP_wcpcy( plP_w3wcy( tx[i], ty[i], tz[i] ));
    }

/* AWI: in the past we have used
 *  plP_fill(xpoly, ypoly, n);
 * here, but our educated guess is this fill should be done via the clipping
 * interface instead as below.
 * No example tests this code so one of our users will end up inadvertently
 * testing this for us.
 *
 * jc: I have checked, and both versions does give the same result, i.e., clipping
 * to the window boundaries. The reason is that the above plP_clip_poly() does
 * the clipping. To check this, is enough to diminish the x/y/z min/max arguments in
 * plw3d() in x08c. But let's keep it, although 10% slower...
 */
    plP_plfclp( xpoly, ypoly, n, plsc->clpxmi, plsc->clpxma,
        plsc->clpymi, plsc->clpyma, plP_fill );
}

/*----------------------------------------------------------------------*\
 * void plfill_soft()
 *
 * Pattern fills in software the polygon bounded by the input points.
 \*----------------------------------------------------------------------*/

void
plfill_soft( short *x, short *y, PLINT n )
{
    PLINT  i, j;
    PLINT  xp1, yp1, xp2, yp2, xp3, yp3;
    PLINT  k, dinc;
    PLFLT  ci, si;
    double temp;

    buffersize = 2 * BINC;
    buffer     = (PLINT *) malloc((size_t) buffersize * sizeof ( PLINT ));
    if ( !buffer )
    {
        plabort( "plfill: Out of memory" );
        return;
    }

/* Loop over sets of lines in pattern */

    for ( k = 0; k < plsc->nps; k++ )
    {
        bufferleng = 0;

        temp = DTOR * plsc->inclin[k] * 0.1;
        si   = sin( temp ) * plsc->ypmm;
        ci   = cos( temp ) * plsc->xpmm;

        /* normalize: 1 = si*si + ci*ci */

        temp = sqrt((double) ( si * si + ci * ci ));
        si  /= temp;
        ci  /= temp;

        dinc = (PLINT) ( plsc->delta[k] * SSQR( plsc->ypmm * ABS( ci ),
                             plsc->xpmm * ABS( si )) / 1000. );

        if ( dinc < 0 ) dinc = -dinc;
        if ( dinc == 0 ) dinc = 1;

        xp1 = x[n - 2];
        yp1 = y[n - 2];
        tran( &xp1, &yp1, (PLFLT) ci, (PLFLT) si );

        xp2 = x[n - 1];
        yp2 = y[n - 1];
        tran( &xp2, &yp2, (PLFLT) ci, (PLFLT) si );

/* Loop over points in polygon */

        for ( i = 0; i < n; i++ )
        {
            xp3 = x[i];
            yp3 = y[i];
            tran( &xp3, &yp3, (PLFLT) ci, (PLFLT) si );
            buildlist( xp1, yp1, xp2, yp2, xp3, yp3, dinc );
            xp1 = xp2;
            yp1 = yp2;
            xp2 = xp3;
            yp2 = yp3;
        }

/* Sort list by y then x */

        qsort((void *) buffer, (size_t) bufferleng / 2,
            (size_t) sizeof ( struct point ), compar );

/* OK, now do the hatching */

        i = 0;

        while ( i < bufferleng )
        {
            xp1 = buffer[i];
            yp1 = buffer[i + 1];
            i  += 2;
            xp2 = xp1;
            yp2 = yp1;
            tran( &xp1, &yp1, (PLFLT) ci, (PLFLT) ( -si ));
            plP_movphy( xp1, yp1 );
            xp1 = buffer[i];
            yp1 = buffer[i + 1];
            i  += 2;
            if ( yp2 != yp1 )
            {
                fprintf( stderr, "plfill: oh oh we are lost\n" );
                for ( j = 0; j < bufferleng; j += 2 )
                {
                    fprintf( stderr, "plfill: %d %d\n",
                        (int) buffer[j], (int) buffer[j + 1] );
                }
                continue;       /* Uh oh we're lost */
            }
            tran( &xp1, &yp1, (PLFLT) ci, (PLFLT) ( -si ));
            plP_draphy( xp1, yp1 );
        }
    }
    free((void *) buffer );
}

/*----------------------------------------------------------------------*\
 * Utility functions
 \*----------------------------------------------------------------------*/

void
tran( PLINT *a, PLINT *b, PLFLT c, PLFLT d )
{
    PLINT ta, tb;

    ta = *a;
    tb = *b;

    *a = (PLINT) floor((double) ( ta * c + tb * d + 0.5 ));
    *b = (PLINT) floor((double) ( tb * c - ta * d + 0.5 ));
}

void
buildlist( PLINT xp1, PLINT yp1, PLINT xp2, PLINT yp2, PLINT xp3, PLINT yp3,
           PLINT dinc )
{
    PLINT min_y, max_y;
    PLINT dx, dy, cstep, nstep, ploty, plotx;

    dx = xp2 - xp1;
    dy = yp2 - yp1;

    if ( dy == 0 )
    {
        if ( yp2 > yp3 && (( yp2 % dinc ) == 0 ))
            addcoord( xp2, yp2 );
        return;
    }

    if ( dy > 0 )
    {
        cstep = 1;
        min_y = yp1;
        max_y = yp2;
    }
    else
    {
        cstep = -1;
        min_y = yp2;
        max_y = yp1;
    }

    nstep = ( yp3 > yp2 ? 1 : -1 );
    if ( yp3 == yp2 ) nstep = 0;

    /* Build coordinate list */

    ploty = ( min_y / dinc ) * dinc;
    if ( ploty < min_y ) ploty += dinc;

    for (; ploty <= max_y; ploty += dinc )
    {
        if ( ploty == yp1 ) continue;
        if ( ploty == yp2 )
        {
            if ( cstep == -nstep ) continue;
            if ( yp2 == yp3 && yp1 > yp2 ) continue;
        }
        plotx = xp1 + (PLINT) floor(((double) ( ploty - yp1 ) * dx ) / dy + 0.5 );
        addcoord( plotx, ploty );
    }
}

void
addcoord( PLINT xp1, PLINT yp1 )
{
    PLINT *temp;

    if ( bufferleng + 2 > buffersize )
    {
        buffersize += 2 * BINC;
        temp        = (PLINT *) realloc((void *) buffer,
            (size_t) buffersize * sizeof ( PLINT ));
        if ( !temp )
        {
            free((void *) buffer );
            plexit( "plfill: Out of memory!" );
        }
        buffer = temp;
    }

    buffer[bufferleng++] = xp1;
    buffer[bufferleng++] = yp1;
}

int
compar( const void *pnum1, const void *pnum2 )
{
    const struct point *pnt1, *pnt2;

    pnt1 = (const struct point *) pnum1;
    pnt2 = (const struct point *) pnum2;

    if ( pnt1->y < pnt2->y )
        return -1;
    else if ( pnt1->y > pnt2->y )
        return 1;

    /* Only reach here if y coords are equal, so sort by x */

    if ( pnt1->x < pnt2->x )
        return -1;
    else if ( pnt1->x > pnt2->x )
        return 1;

    return 0;
}

/*----------------------------------------------------------------------*\
 * void plP_plfclp()
 *
 * Fills a polygon within the clip limits.
 \*----------------------------------------------------------------------*/

void
plP_plfclp( PLINT *x, PLINT *y, PLINT npts,
            PLINT xmin, PLINT xmax, PLINT ymin, PLINT ymax,
            void ( *draw )( short *, short *, PLINT ))
{
    /* Must have at least 3 points and draw() specified */
    if ( npts < 3 || !draw ) return;

#ifdef USE_FILL_INTERSECTION_POLYGON
    PLINT *x1, *y1, i1start = 0, i, im1, n1;
    PLINT x2[4]  = { xmin, xmax, xmax, xmin };
    PLINT y2[4]  = { ymin, ymin, ymax, ymax };
    PLINT if2[4] = { 0, 0, 0, 0 };
    PLINT n2     = 4;
    if (( x1 = (PLINT *) malloc( npts * sizeof ( PLINT ))) == NULL )
    {
        plexit( "plP_plfclp: Insufficient memory" );
    }
    if (( y1 = (PLINT *) malloc( npts * sizeof ( PLINT ))) == NULL )
    {
        plexit( "plP_plfclp: Insufficient memory" );
    }
    /* Polygon 2 obviously has no dups, but get rid of them in polygon
     * 1 if they exist.  ToDo: Deal with self-intersecting polygon 1
     * case as well. */

    im1 = npts - 1;
    n1  = 0;
    for ( i = 0; i < npts; i++ )
    {
        if ( !( x[i] == x[im1] && y[i] == y[im1] ))
        {
            x1[n1]   = x[i];
            y1[n1++] = y[i];
        }
        im1 = i;
    }

    fill_intersection_polygon( 0, 0, draw, x1, y1, i1start, n1, x2, y2, if2, n2 );
    free( x1 );
    free( y1 );
    return;
}
#else /* USE_FILL_INTERSECTION_POLYGON */

    PLINT i, x1, x2, y1, y2;
    int   iclp = 0, iout = 2;
    short _xclp[2 * PL_MAXPOLY + 2], _yclp[2 * PL_MAXPOLY + 2];
    short *xclp, *yclp;
    int   drawable;
    int   crossed_xmin1 = 0, crossed_xmax1 = 0;
    int   crossed_ymin1 = 0, crossed_ymax1 = 0;
    int   crossed_xmin2 = 0, crossed_xmax2 = 0;
    int   crossed_ymin2 = 0, crossed_ymax2 = 0;
    int   crossed_up    = 0, crossed_down = 0;
    int   crossed_left  = 0, crossed_right = 0;
    int   inside_lb;
    int   inside_lu;
    int   inside_rb;
    int   inside_ru;


    if ( npts < PL_MAXPOLY )
    {
        xclp = _xclp;
        yclp = _yclp;
    }
    else
    {
        if ((( xclp = (short *) malloc(( 2 * npts + 2 ) * sizeof ( short ))) == NULL ) ||
            (( yclp = (short *) malloc(( 2 * npts + 2 ) * sizeof ( short ))) == NULL ))
        {
            plexit( "plP_plfclp: Insufficient memory" );
        }
    }
    inside_lb = !notpointinpolygon( npts, x, y, xmin, ymin );
    inside_lu = !notpointinpolygon( npts, x, y, xmin, ymax );
    inside_rb = !notpointinpolygon( npts, x, y, xmax, ymin );
    inside_ru = !notpointinpolygon( npts, x, y, xmax, ymax );

    for ( i = 0; i < npts - 1; i++ )
    {
        x1 = x[i]; x2 = x[i + 1];
        y1 = y[i]; y2 = y[i + 1];

        drawable = ( INSIDE( x1, y1 ) && INSIDE( x2, y2 ));
        if ( !drawable )
            drawable = !plP_clipline( &x1, &y1, &x2, &y2,
                xmin, xmax, ymin, ymax );

        if ( drawable )
        {
            /* Boundary crossing condition -- coming in. */
            crossed_xmin2 = ( x1 == xmin ); crossed_xmax2 = ( x1 == xmax );
            crossed_ymin2 = ( y1 == ymin ); crossed_ymax2 = ( y1 == ymax );

            crossed_left  = ( crossed_left || crossed_xmin2 );
            crossed_right = ( crossed_right || crossed_xmax2 );
            crossed_down  = ( crossed_down || crossed_ymin2 );
            crossed_up    = ( crossed_up || crossed_ymax2 );
            iout          = iclp + 2;
            /* If the first segment, just add it. */

            if ( iclp == 0 )
            {
                xclp[iclp] = x1; yclp[iclp] = y1; iclp++;
                xclp[iclp] = x2; yclp[iclp] = y2; iclp++;
            }

            /* Not first point.  If first point of this segment matches up to the
             * previous point, just add it.  */

            else if ( x1 == xclp[iclp - 1] && y1 == yclp[iclp - 1] )
            {
                xclp[iclp] = x2; yclp[iclp] = y2; iclp++;
            }

            /* Otherwise, we need to add both points, to connect the points in the
             * polygon along the clip boundary.  If we encircled a corner, we have
             * to add that first.
             */

            else
            {
                /* Treat the case where we encircled two corners:
                 * Construct a polygon out of the subset of vertices
                 * Note that the direction is important too when adding
                 * the extra points */
                xclp[iclp + 1] = x2; yclp[iclp + 1] = y2;
                xclp[iclp + 2] = x1; yclp[iclp + 2] = y1;
                iout           = iout - iclp + 1;
                /* Upper two */
                if ((( crossed_xmin1 && crossed_xmax2 ) ||
                     ( crossed_xmin2 && crossed_xmax1 )) &&
                    inside_lu )
                {
                    if ( crossed_xmin1 )
                    {
                        xclp[iclp] = xmin; yclp[iclp] = ymax; iclp++;
                        xclp[iclp] = xmax; yclp[iclp] = ymax; iclp++;
                    }
                    else
                    {
                        xclp[iclp] = xmax; yclp[iclp] = ymax; iclp++;
                        xclp[iclp] = xmin; yclp[iclp] = ymax; iclp++;
                    }
                }
                /* Lower two */
                else if ((( crossed_xmin1 && crossed_xmax2 ) ||
                          ( crossed_xmin2 && crossed_xmax1 )) &&
                         inside_lb )
                {
                    if ( crossed_xmin1 )
                    {
                        xclp[iclp] = xmin; yclp[iclp] = ymin; iclp++;
                        xclp[iclp] = xmax; yclp[iclp] = ymin; iclp++;
                    }
                    else
                    {
                        xclp[iclp] = xmax; yclp[iclp] = ymin; iclp++;
                        xclp[iclp] = xmin; yclp[iclp] = ymin; iclp++;
                    }
                }
                /* Left two */
                else if ((( crossed_ymin1 && crossed_ymax2 ) ||
                          ( crossed_ymin2 && crossed_ymax1 )) &&
                         inside_lb )
                {
                    if ( crossed_ymin1 )
                    {
                        xclp[iclp] = xmin; yclp[iclp] = ymin; iclp++;
                        xclp[iclp] = xmin; yclp[iclp] = ymax; iclp++;
                    }
                    else
                    {
                        xclp[iclp] = xmin; yclp[iclp] = ymax; iclp++;
                        xclp[iclp] = xmin; yclp[iclp] = ymin; iclp++;
                    }
                }
                /* Right two */
                else if ((( crossed_ymin1 && crossed_ymax2 ) ||
                          ( crossed_ymin2 && crossed_ymax1 )) &&
                         inside_rb )
                {
                    if ( crossed_ymin1 )
                    {
                        xclp[iclp] = xmax; yclp[iclp] = ymin; iclp++;
                        xclp[iclp] = xmax; yclp[iclp] = ymax; iclp++;
                    }
                    else
                    {
                        xclp[iclp] = xmax; yclp[iclp] = ymax; iclp++;
                        xclp[iclp] = xmax; yclp[iclp] = ymin; iclp++;
                    }
                }
                /* Now the case where we encircled one corner */
                /* Lower left */
                else if (( crossed_xmin1 && crossed_ymin2 ) ||
                         ( crossed_ymin1 && crossed_xmin2 ))
                {
                    xclp[iclp] = xmin; yclp[iclp] = ymin; iclp++;
                }
                /* Lower right */
                else if (( crossed_xmax1 && crossed_ymin2 ) ||
                         ( crossed_ymin1 && crossed_xmax2 ))
                {
                    xclp[iclp] = xmax; yclp[iclp] = ymin; iclp++;
                }
                /* Upper left */
                else if (( crossed_xmin1 && crossed_ymax2 ) ||
                         ( crossed_ymax1 && crossed_xmin2 ))
                {
                    xclp[iclp] = xmin; yclp[iclp] = ymax; iclp++;
                }
                /* Upper right */
                else if (( crossed_xmax1 && crossed_ymax2 ) ||
                         ( crossed_ymax1 && crossed_xmax2 ))
                {
                    xclp[iclp] = xmax; yclp[iclp] = ymax; iclp++;
                }

                /* Now add current segment. */
                xclp[iclp] = x1; yclp[iclp] = y1; iclp++;
                xclp[iclp] = x2; yclp[iclp] = y2; iclp++;
            }

            /* Boundary crossing condition -- going out. */
            crossed_xmin1 = ( x2 == xmin ); crossed_xmax1 = ( x2 == xmax );
            crossed_ymin1 = ( y2 == ymin ); crossed_ymax1 = ( y2 == ymax );
        }
    }

    /* Limit case - all vertices are outside of bounding box.  So just fill entire
     * box, *if* the bounding box is completely encircled.
     */
    if ( iclp == 0 )
    {
        if ( inside_lb )
        {
            xclp[0] = xmin; yclp[0] = ymin;
            xclp[1] = xmax; yclp[1] = ymin;
            xclp[2] = xmax; yclp[2] = ymax;
            xclp[3] = xmin; yclp[3] = ymax;
            xclp[4] = xmin; yclp[4] = ymin;
            ( *draw )( xclp, yclp, 5 );

            if ( xclp != _xclp )
            {
                free( xclp );
                free( yclp );
            }

            return;
        }
    }

    /* Now handle cases where fill polygon intersects two sides of the box */

    if ( iclp >= 2 )
    {
        int debug = 0;
        int dir   = circulation( x, y, npts );
        if ( debug )
        {
            if (( xclp[0] == xmin && xclp[iclp - 1] == xmax ) ||
                ( xclp[0] == xmax && xclp[iclp - 1] == xmin ) ||
                ( yclp[0] == ymin && yclp[iclp - 1] == ymax ) ||
                ( yclp[0] == ymax && yclp[iclp - 1] == ymin ) ||
                ( xclp[0] == xmin && yclp[iclp - 1] == ymin ) ||
                ( yclp[0] == ymin && xclp[iclp - 1] == xmin ) ||
                ( xclp[0] == xmax && yclp[iclp - 1] == ymin ) ||
                ( yclp[0] == ymin && xclp[iclp - 1] == xmax ) ||
                ( xclp[0] == xmax && yclp[iclp - 1] == ymax ) ||
                ( yclp[0] == ymax && xclp[iclp - 1] == xmax ) ||
                ( xclp[0] == xmin && yclp[iclp - 1] == ymax ) ||
                ( yclp[0] == ymax && xclp[iclp - 1] == xmin ))
            {
                printf( "dir=%d, clipped points:\n", dir );
                for ( i = 0; i < iclp; i++ )
                    printf( " x[%d]=%d y[%d]=%d", i, xclp[i], i, yclp[i] );
                printf( "\n" );
                printf( "pre-clipped points:\n" );
                for ( i = 0; i < npts; i++ )
                    printf( " x[%d]=%d y[%d]=%d", i, x[i], i, y[i] );
                printf( "\n" );
            }
        }

        /* The cases where the fill region is divided 2/2 */
        /* Divided horizontally */
        if ( xclp[0] == xmin && xclp[iclp - 1] == xmax )
        {
            if ( dir > 0 )
            {
                xclp[iclp] = xmax; yclp[iclp] = ymax; iclp++;
                xclp[iclp] = xmin; yclp[iclp] = ymax; iclp++;
            }
            else
            {
                xclp[iclp] = xmax; yclp[iclp] = ymin; iclp++;
                xclp[iclp] = xmin; yclp[iclp] = ymin; iclp++;
            }
        }
        else if ( xclp[0] == xmax && xclp[iclp - 1] == xmin )
        {
            if ( dir > 0 )
            {
                xclp[iclp] = xmin; yclp[iclp] = ymin; iclp++;
                xclp[iclp] = xmax; yclp[iclp] = ymin; iclp++;
            }
            else
            {
                xclp[iclp] = xmin; yclp[iclp] = ymax; iclp++;
                xclp[iclp] = xmax; yclp[iclp] = ymax; iclp++;
            }
        }

        /* Divided vertically */
        else if ( yclp[0] == ymin && yclp[iclp - 1] == ymax )
        {
            if ( dir > 0 )
            {
                xclp[iclp] = xmin; yclp[iclp] = ymax; iclp++;
                xclp[iclp] = xmin; yclp[iclp] = ymin; iclp++;
            }
            else
            {
                xclp[iclp] = xmax; yclp[iclp] = ymax; iclp++;
                xclp[iclp] = xmax; yclp[iclp] = ymin; iclp++;
            }
        }
        else if ( yclp[0] == ymax && yclp[iclp - 1] == ymin )
        {
            if ( dir > 0 )
            {
                xclp[iclp] = xmax; yclp[iclp] = ymin; iclp++;
                xclp[iclp] = xmax; yclp[iclp] = ymax; iclp++;
            }
            else
            {
                xclp[iclp] = xmin; yclp[iclp] = ymin; iclp++;
                xclp[iclp] = xmin; yclp[iclp] = ymax; iclp++;
            }
        }

        /* The cases where the fill region is divided 3/1 --
         *    LL           LR           UR           UL
         +-----+      +-----+      +-----+      +-----+
         |     |      |     |      |    \|      |/    |
         |     |      |     |      |     |      |     |
         |\    |      |    /|      |     |      |     |
         +-----+      +-----+      +-----+      +-----+
         *
         * Note when we go the long way around, if the direction is reversed the
         * three vertices must be visited in the opposite order.
         */
        /* LL, short way around */
        else if (( xclp[0] == xmin && yclp[iclp - 1] == ymin && dir < 0 ) ||
                 ( yclp[0] == ymin && xclp[iclp - 1] == xmin && dir > 0 ))
        {
            xclp[iclp] = xmin; yclp[iclp] = ymin; iclp++;
        }
        /* LL, long way around, counterclockwise */
        else if (( xclp[0] == xmin && yclp[iclp - 1] == ymin && dir > 0 ))
        {
            xclp[iclp] = xmax; yclp[iclp] = ymin; iclp++;
            xclp[iclp] = xmax; yclp[iclp] = ymax; iclp++;
            xclp[iclp] = xmin; yclp[iclp] = ymax; iclp++;
        }
        /* LL, long way around, clockwise */
        else if (( yclp[0] == ymin && xclp[iclp - 1] == xmin && dir < 0 ))
        {
            xclp[iclp] = xmin; yclp[iclp] = ymax; iclp++;
            xclp[iclp] = xmax; yclp[iclp] = ymax; iclp++;
            xclp[iclp] = xmax; yclp[iclp] = ymin; iclp++;
        }
        /* LR, short way around */
        else if (( xclp[0] == xmax && yclp[iclp - 1] == ymin && dir > 0 ) ||
                 ( yclp[0] == ymin && xclp[iclp - 1] == xmax && dir < 0 ))
        {
            xclp[iclp] = xmax; yclp[iclp] = ymin; iclp++;
        }
        /* LR, long way around, counterclockwise */
        else if ( yclp[0] == ymin && xclp[iclp - 1] == xmax && dir > 0 )
        {
            xclp[iclp] = xmax; yclp[iclp] = ymax; iclp++;
            xclp[iclp] = xmin; yclp[iclp] = ymax; iclp++;
            xclp[iclp] = xmin; yclp[iclp] = ymin; iclp++;
        }
        /* LR, long way around, clockwise */
        else if ( xclp[0] == xmax && yclp[iclp - 1] == ymin && dir < 0 )
        {
            xclp[iclp] = xmin; yclp[iclp] = ymin; iclp++;
            xclp[iclp] = xmin; yclp[iclp] = ymax; iclp++;
            xclp[iclp] = xmax; yclp[iclp] = ymax; iclp++;
        }
        /* UR, short way around */
        else if (( xclp[0] == xmax && yclp[iclp - 1] == ymax && dir < 0 ) ||
                 ( yclp[0] == ymax && xclp[iclp - 1] == xmax && dir > 0 ))
        {
            xclp[iclp] = xmax; yclp[iclp] = ymax; iclp++;
        }
        /* UR, long way around, counterclockwise */
        else if ( xclp[0] == xmax && yclp[iclp - 1] == ymax && dir > 0 )
        {
            xclp[iclp] = xmin; yclp[iclp] = ymax; iclp++;
            xclp[iclp] = xmin; yclp[iclp] = ymin; iclp++;
            xclp[iclp] = xmax; yclp[iclp] = ymin; iclp++;
        }
        /* UR, long way around, clockwise */
        else if ( yclp[0] == ymax && xclp[iclp - 1] == xmax && dir < 0 )
        {
            xclp[iclp] = xmax; yclp[iclp] = ymin; iclp++;
            xclp[iclp] = xmin; yclp[iclp] = ymin; iclp++;
            xclp[iclp] = xmin; yclp[iclp] = ymax; iclp++;
        }
        /* UL, short way around */
        else if (( xclp[0] == xmin && yclp[iclp - 1] == ymax && dir > 0 ) ||
                 ( yclp[0] == ymax && xclp[iclp - 1] == xmin && dir < 0 ))
        {
            xclp[iclp] = xmin; yclp[iclp] = ymax; iclp++;
        }
        /* UL, long way around, counterclockwise */
        else if ( yclp[0] == ymax && xclp[iclp - 1] == xmin && dir > 0 )
        {
            xclp[iclp] = xmin; yclp[iclp] = ymin; iclp++;
            xclp[iclp] = xmax; yclp[iclp] = ymin; iclp++;
            xclp[iclp] = xmax; yclp[iclp] = ymax; iclp++;
        }
        /* UL, long way around, clockwise */
        else if ( xclp[0] == xmin && yclp[iclp - 1] == ymax && dir < 0 )
        {
            xclp[iclp] = xmax; yclp[iclp] = ymax; iclp++;
            xclp[iclp] = xmax; yclp[iclp] = ymin; iclp++;
            xclp[iclp] = xmin; yclp[iclp] = ymin; iclp++;
        }
    }

    /* Check for the case that only one side has been crossed
     * (AM) Just checking a single point turns out not to be
     * enough, apparently the crossed_*1 and crossed_*2 variables
     * are not quite what I expected.
     */
    if ( inside_lb + inside_rb + inside_lu + inside_ru == 4 )
    {
        int   dir = circulation( x, y, npts );
        PLINT xlim[4], ylim[4];
        int   insert;
        int   incr;

        xlim[0] = xmin; ylim[0] = ymin;
        xlim[1] = xmax; ylim[1] = ymin;
        xlim[2] = xmax; ylim[2] = ymax;
        xlim[3] = xmin; ylim[3] = ymax;

        if ( crossed_left + crossed_right + crossed_down + crossed_up == 1 )
        {
            if ( dir > 0 )
            {
                incr   = 1;
                insert = 0 * crossed_left + 1 * crossed_down + 2 * crossed_right +
                         3 * crossed_up;
            }
            else
            {
                incr   = -1;
                insert = 3 * crossed_left + 2 * crossed_up + 1 * crossed_right +
                         0 * crossed_down;
            }
        }

        if ( crossed_left + crossed_right == 2 && crossed_down + crossed_up == 0 )
        {
            if ( xclp[iclp - 1] == xmin )
            {
                if ( dir == 1 )
                {
                    incr   = 1;
                    insert = 0;
                }
                else
                {
                    incr   = -1;
                    insert = 3;
                }
            }
            else
            {
                if ( dir == 1 )
                {
                    incr   = 1;
                    insert = 1;
                }
                else
                {
                    incr   = -1;
                    insert = 2;
                }
            }
        }

        if ( crossed_left + crossed_right == 0 && crossed_down + crossed_up == 2 )
        {
            if ( yclp[iclp - 1] == ymin )
            {
                if ( dir == 1 )
                {
                    incr   = 1;
                    insert = 1;
                }
                else
                {
                    incr   = -1;
                    insert = 0;
                }
            }
            else
            {
                if ( dir == 1 )
                {
                    incr   = 1;
                    insert = 3;
                }
                else
                {
                    incr   = -1;
                    insert = 2;
                }
            }
        }

        for ( i = 0; i < 4; i++ )
        {
            xclp[iclp] = xlim[insert];
            yclp[iclp] = ylim[insert];
            iclp++;
            insert += incr;
            if ( insert > 3 ) insert = 0;
            if ( insert < 0 ) insert = 3;
        }
    }

    /* Draw the sucker */
    if ( iclp >= 3 )
        ( *draw )( xclp, yclp, iclp );

    if ( xclp != _xclp )
    {
        free( xclp );
        free( yclp );
    }
}
#endif /* USE_FILL_INTERSECTION_POLYGON */

/*----------------------------------------------------------------------*\
 * int circulation()
 *
 * Returns the circulation direction for a given polyline: positive is
 * counterclockwise, negative is clockwise (right hand rule).
 *
 * Used to get the circulation of the fill polygon around the bounding box,
 * when the fill polygon is larger than the bounding box.  Counts left
 * (positive) vs right (negative) hand turns using a cross product, instead of
 * performing all the expensive trig calculations needed to get this 100%
 * correct.  For the fill cases encountered in plplot, this treatment should
 * give the correct answer most of the time, by far.  When used with plshades,
 * the typical return value is 3 or -3, since 3 turns are necessary in order
 * to complete the fill region.  Only for really oddly shaped fill regions
 * will it give the wrong answer.
 *
 * AM:
 * Changed the computation: use the outer product to compute the surface
 * area, the sign determines if the polygon is followed clockwise or
 * counterclockwise. This is more reliable. Floating-point numbers
 * are used to avoid overflow.
 \*----------------------------------------------------------------------*/

int
circulation( PLINT *x, PLINT *y, PLINT npts )
{
    PLFLT xproduct;
    int direction = 0;
    PLFLT x1, y1, x2, y2, x3, y3;
    int i;

    xproduct = 0.0;
    x1       = x[0];
    y1       = y[0];
    for ( i = 1; i < npts - 2; i++ )
    {
        x2       = x[i + 1];
        y2       = y[i + 1];
        x3       = x[i + 2];
        y3       = y[i + 2];
        xproduct = xproduct + ( x2 - x1 ) * ( y3 - y2 ) - ( y2 - y1 ) * ( x3 - x2 );
    }

    if ( xproduct > 0.0 ) direction = 1;
    if ( xproduct < 0.0 ) direction = -1;
    return direction;
}


/* PLFLT wrapper for !notpointinpolygon. */
int
plP_pointinpolygon( PLINT n, const PLFLT *x, const PLFLT *y, PLFLT xp, PLFLT yp )
{
    int i, return_value;
    PLINT *xint, *yint;
    PLFLT xmaximum = fabs( xp ), ymaximum = fabs( yp ), xscale, yscale;
    if (( xint = (PLINT *) malloc( n * sizeof ( PLINT ))) == NULL )
    {
        plexit( "PlP_pointinpolygon: Insufficient memory" );
    }
    if (( yint = (PLINT *) malloc( n * sizeof ( PLINT ))) == NULL )
    {
        plexit( "PlP_pointinpolygon: Insufficient memory" );
    }
    for ( i = 0; i < n; i++ )
    {
        xmaximum = MAX( xmaximum, fabs( x[i] ));
        ymaximum = MAX( ymaximum, fabs( y[i] ));
    }
    xscale = 1.e8 / xmaximum;
    yscale = 1.e8 / ymaximum;
    for ( i = 0; i < n; i++ )
    {
        xint[i] = (PLINT) ( xscale * x[i] );
        yint[i] = (PLINT) ( yscale * y[i] );
    }
    return_value = !notpointinpolygon( n, xint, yint,
        (PLINT) ( xscale * xp ), (PLINT) ( yscale * yp ));
    free( xint );
    free( yint );
    return return_value;
}
/*----------------------------------------------------------------------*\
 * int notpointinpolygon()
 *
 * Returns 0, 1, or 2 depending on whether the test point is definitely
 * inside, near the border, or definitely outside the polygon.
 * Notes:
 * This "Ray casting algorithm" has been described in
 * http://en.wikipedia.org/wiki/Point_in_polygon.
 * Logic still needs to be inserted to take care of the "ray passes
 * through vertex" problem in a numerically robust way.
 \*----------------------------------------------------------------------*/

#define NEW_NOTPOINTINPOLYGON_CODE
int
notpointinpolygon( int n, const PLINT *x, const PLINT *y, PLINT xp, PLINT yp )
{
#ifdef NEW_NOTPOINTINPOLYGON_CODE
    int i, im1;
    int count_crossings = 0;
    PLINT xmin, xout, yout, xintersect, yintersect;


    /* Determine a point outside the polygon  */

    xmin = x[0];
    xout = x[0];
    yout = y[0];
    for ( i = 1; i < n; i++ )
    {
        xout = MAX( xout, x[i] );
        xmin = MIN( xmin, x[i] );
    }
    /* + 10 to make sure completely outside. */
    xout = xout + ( xout - xmin ) + 10;

    /* Determine whether the line between (xout, yout) and (xp, yp) intersects
     * one of the polygon segments. */

    im1 = n - 1;
    for ( i = 0; i < n; i++ )
    {
        if ( !( x[im1] == x[i] && y[im1] == y[i] ) &&
             !notintersect( &xintersect, &yintersect,
                 x[im1], y[im1], x[i], y[i],
                 xp, yp, xout, yout ))
            count_crossings++;
        im1 = i;
    }

    /* Return the result: an even number of crossings means the
     * point is outside the polygon */

    return !( count_crossings % 2 );
}
#else /* NEW_NOTPOINTINPOLYGON_CODE */
    int i;
    int count_crossings;
    PLFLT x1, y1, x2, y2, xpp, ypp, xout, yout, xmax;
    PLFLT xvp, yvp, xvv, yvv, xv1, yv1, xv2, yv2;
    PLFLT inprod1, inprod2;

    xpp = (PLFLT) xp;
    ypp = (PLFLT) yp;

    count_crossings = 0;


    /* Determine a point outside the polygon  */

    xmax = x[0];
    xout = x[0];
    yout = y[0];
    for ( i = 0; i < n; i++ )
    {
        if ( xout > x[i] )
        {
            xout = x[i];
        }
        if ( xmax < x[i] )
        {
            xmax = x[i];
        }
    }
    xout = xout - ( xmax - xout );

    /* Determine for each side whether the line segment between
     * our two points crosses the vertex */

    xpp = (PLFLT) xp;
    ypp = (PLFLT) yp;

    xvp = xpp - xout;
    yvp = ypp - yout;

    for ( i = 0; i < n; i++ )
    {
        x1 = (PLFLT) x[i];
        y1 = (PLFLT) y[i];
        if ( i < n - 1 )
        {
            x2 = (PLFLT) x[i + 1];
            y2 = (PLFLT) y[i + 1];
        }
        else
        {
            x2 = (PLFLT) x[0];
            y2 = (PLFLT) y[0];
        }

        /* Skip zero-length segments */
        if ( x1 == x2 && y1 == y2 )
        {
            continue;
        }

        /* Line through the two fixed points:
         * Are x1 and x2 on either side? */
        xv1     = x1 - xout;
        yv1     = y1 - yout;
        xv2     = x2 - xout;
        yv2     = y2 - yout;
        inprod1 = xv1 * yvp - yv1 * xvp; /* Well, with the normal vector */
        inprod2 = xv2 * yvp - yv2 * xvp;
        if ( inprod1 * inprod2 >= 0.0 )
        {
            /* No crossing possible! */
            continue;
        }

        /* Line through the two vertices:
         * Are xout and xpp on either side? */
        xvv     = x2 - x1;
        yvv     = y2 - y1;
        xv1     = xpp - x1;
        yv1     = ypp - y1;
        xv2     = xout - x1;
        yv2     = yout - y1;
        inprod1 = xv1 * yvv - yv1 * xvv;
        inprod2 = xv2 * yvv - yv2 * xvv;
        if ( inprod1 * inprod2 >= 0.0 )
        {
            /* No crossing possible! */
            continue;
        }

        /* We do have a crossing */
        count_crossings++;
    }

    /* Return the result: an even number of crossings means the
     * point is outside the polygon */

    return !( count_crossings % 2 );
}
#endif /* NEW_NOTPOINTINPOLYGON_CODE */
/* Fill intersection of two simple (not self-intersecting) polygons.
 * There must be an even number of edge intersections between the two
 * polygons (ignoring vertex intersections which touch, but do not cross).
 * Eliminate those intersection pairs by recursion (calling the same
 * routine twice again with the second polygon split at a boundary defined
 * by the first intersection point, all polygon 1 vertices between
 * the intersections, and the second intersection point).
 * Once the recursion has eliminated all intersecting edges, fill or
 * not using the appropriate polygon depending on whether the first
 * and second polygons are identical or whether one of them is
 * entirely inside the other of them.  If ifextrapolygon is true, the fill
 * step will consist of another recursive call to the routine with
 * ifextrapolygon false, and the second polygon set to an additional
 * polygon defined by the stream (not yet implemented).
 * N.B. it is the calling routine's responsibility to insure the two
 * polygons are not self-intersecting and do not have duplicate points.  */


#define MAX_RECURSION_DEPTH    10
void
fill_intersection_polygon( PLINT recursion_depth, PLINT ifextrapolygon,
                           void ( *fill )( short *, short *, PLINT ),
                           const PLINT *x1, const PLINT *y1,
                           PLINT i1start, PLINT n1,
                           const PLINT *x2, const PLINT *y2,
                           const PLINT *if2, PLINT n2 )
{
    PLINT i1, i1m1, i1wrap, i1wraplast,
          i2, i2m1, i2wrap, i2wraplast,
          kk, kkstart1, kkstart21, kkstart22,
          k, kstart, range1, range21, range22, nintersect,
          nsplit1, nsplit2;
    PLINT xintersect[2], yintersect[2], ifintersect;
    PLINT *xsplit1, *ysplit1, *ifsplit1,
    *xsplit2, *ysplit2, *ifsplit2;
    PLINT ifill, nfill = 0;
    const PLINT *xfiller, *yfiller;
    short *xfill, *yfill;

    if ( recursion_depth > MAX_RECURSION_DEPTH )
    {
        plwarn( "fill_intersection_polygon: Recursion_depth too large.  "
            "Probably an internal error figuring out intersections. " );
        return;
    }

    if ( n1 < 3 )
    {
        plwarn( "fill_intersection_polygon: Internal error; n1 < 3." );
        return;
    }

    if ( n2 < 3 )
    {
        plwarn( "fill_intersection_polygon: Internal error; n2 < 3." );
        return;
    }

    if ( i1start < 0 || i1start >= n1 )
    {
        plwarn( "fill_intersection_polygon: invalid i1start." );
        return;
    }

    i1m1 = i1start - 1;
    if ( i1m1 <= 0 )
        i1m1 = n1 - 1;

    for ( i1 = i1start; i1 < n1; i1++ )
    {
        if ( x1[i1] == x1[i1m1] && y1[i1] == y1[i1m1] )
            break;
        i1m1 = i1;
    }

    if ( i1 < n1 )
    {
        plwarn( "fill_intersection_polygon: Internal error; i1 < n1." );
        return;
    }

    i2m1 = n2 - 1;
    for ( i2 = 0; i2 < n2; i2++ )
    {
        if ( x2[i2] == x2[i2m1] && y2[i2] == y2[i2m1] )
            break;
        i2m1 = i2;
    }

    if ( i2 < n2 )
    {
        plwarn( "fill_intersection_polygon: Internal error; i < n2." );
        return;
    }

    /*
     *
     * Follow polygon 1 (checking intersections with polygon 2 for each
     * segment of polygon 1) until you have accumulated two
     * intersections with polygon 2.  Here is an ascii-art illustration
     * of the situation.
     *
     *
     *                  2???2
     *
     *                2       2
     *
     * --- 1    1
     *            1            2         1      1 ...
     *             X
     *                               1
     *                             X
     *           2
     *                1         1
     *                   1
     *                                 2
     *            2
     *                     2???2
     *
     *
     * "1" marks polygon 1 vertices, "2" marks polygon 2 vertices, "X"
     * marks the intersections, "---" stands for part of polygon 1
     * that has been previously searched for all possible intersections
     * from index 0, and "..." means polygon 1 continues
     * with more potential intersections above and/or below this diagram
     * before it finally hooks back to connect with the index 0 vertex.
     * "2???2" stands for parts of polygon 2 that must connect with each other
     * (since the polygon 1 path between the two intersections is
     * known to be free of intersections.)
     *
     * Polygon 2 is split at the boundary defined by the two
     * intersections and all (in this case three) polygon 1 vertices
     * between the two intersections for the next recursion level.  We
     * absolutely know for that boundary that no more intersections can
     * occur (both polygon 1 and polygon 2 are guaranteed not to
     * self-intersect) so we mark the status of those vertices with that
     * information so those polygon 2 split vertices will not be used to
     * search for further intersections at deeper recursion levels.
     * Note, we know nothing about whether the remaining "2???2" parts of the
     * split polygon 2 intersect with polygon 1 or not so those will
     * continued to be searched at deeper recursion levels. At the same
     * time, we absolutely know that the part of polygon 1 to the left of
     * rightmost x down to and including index 0 cannot yield more
     * intersections with any split of polygon 2 so we adjust the lower
     * limit of polygon 1 to be used for intersection searches at deeper
     * recursion levels.  The result should be that at sufficiently deep
     * recursion depth we always end up with the case that there are no
     * intersections to be found between polygon 1 and some polygon 2
     * split, and in that case we move on to the end phase below.
     */
    nintersect = 0;
    i1m1       = n1 - 1;
    i1wrap     = -1;
    for ( i1 = i1start; i1 < n1; i1++ )
    {
        i2m1   = n2 - 1;
        i2wrap = -1;
        for ( i2 = 0; i2 < n2; i2++ )
        {
            if ( !if2[i2] )
            {
                /* intersect is acted upon only if a series of conditions are met. */
                ifintersect = !notintersect(
                    &xintersect[nintersect], &yintersect[nintersect],
                    x1[i1m1], y1[i1m1], x1[i1], y1[i1],
                    x2[i2m1], y2[i2m1], x2[i2], y2[i2] );
                ifintersect = ifintersect && !(
                    xintersect[nintersect] == x1[i1m1] &&
                    yintersect[nintersect] == y1[i1m1] );
                ifintersect = ifintersect && !(
                    xintersect[nintersect] == x1[i1] &&
                    yintersect[nintersect] == y1[i1] );
                ifintersect = ifintersect && !(
                    xintersect[nintersect] == x2[i2m1] &&
                    yintersect[nintersect] == y2[i2m1] );
                ifintersect = ifintersect && !(
                    xintersect[nintersect] == x2[i2] &&
                    yintersect[nintersect] == y2[i2] );
                if ( ifintersect )
                {
                    /* The above test must be more elaborate if the intersection
                     * point is a vertex of the first polygon, the second polygon
                     * or both. ToDo! */
                    if ( nintersect == 0 )
                    {
                        i1wraplast = i1wrap;
                        i2wraplast = i2wrap;

                        nintersect++;
                    }
                    else
                    {
                        /* Have discovered the first two intersections for
                         * polygon 1 at i1 = i1start or above. */
                        /* New i1start is the largest non-negative polygon 1
                         * index below the last detected intersect. */
                        i1start = MAX( i1wrap, 0 );

                        /* Split polygon 2 at the boundary consisting of
                         * first intersection, intervening (if any) range1
                         * polygon 1 points and second intersection. */
                        /* range1 must always be non-negative because i1
                         * range only traversed once. */
                        range1 = i1wrap - i1wraplast;
                        /* Polygon 2 intersects could be anywhere (since
                         * i2 range repeated until get an intersect).
                         * Divide polygon 2 into two polygons with a
                         * common boundary consisting of the first intersect,
                         * range1 points from polygon1 starting at index
                         * kkstart1 of polygon 1, and the second intersect. */
                        kkstart1 = i1wraplast + 1;

                        /* Split 1 of polygon2 consists of the
                         * boundary + range21 points between kkstart21
                         * (= i2) and i2wraplast in ascending order of
                         * polygon 2 indices.  N.B.  if range21 is zero
                         * below we change that to n2, i.e., use the
                         * whole range of polygon 2 in ascending
                         * order.  For this case, range22 (below) will
                         * be zero and range1 (above) must be nonzero
                         * (in order to have two intersections). */
                        range21 = i2wraplast - i2wrap;
                        if ( range21 <= 0 )
                            range21 += n2;
                        kkstart21 = i2;

                        /* Split 2 of polygon 2 consists of the
                         * boundary + range22 (= n2 - range21) points
                         * between kkstart22 (= i2wrap) and i2wraplast + 1 in
                         * descending order of polygon 2 indices. */
                        range22   = n2 - range21;
                        kkstart22 = i2wrap;
                        if ( kkstart22 == -1 )
                            kkstart22 = i2m1;
                        nsplit1 = 2 + range1 + range21;
                        nsplit2 = 2 + range1 + range22;

                        if (( xsplit1 = (PLINT *) malloc( nsplit1 * sizeof ( PLINT ))) == NULL )
                        {
                            plexit( "fill_intersection_polygon: Insufficient memory" );
                        }
                        if (( ysplit1 = (PLINT *) malloc( nsplit1 * sizeof ( PLINT ))) == NULL )
                        {
                            plexit( "fill_intersection_polygon: Insufficient memory" );
                        }
                        if (( ifsplit1 = (PLINT *) malloc( nsplit1 * sizeof ( PLINT ))) == NULL )
                        {
                            plexit( "fill_intersection_polygon: Insufficient memory" );
                        }

                        if (( xsplit2 = (PLINT *) malloc( nsplit2 * sizeof ( PLINT ))) == NULL )
                        {
                            plexit( "fill_intersection_polygon: Insufficient memory" );
                        }
                        if (( ysplit2 = (PLINT *) malloc( nsplit2 * sizeof ( PLINT ))) == NULL )
                        {
                            plexit( "fill_intersection_polygon: Insufficient memory" );
                        }
                        if (( ifsplit2 = (PLINT *) malloc( nsplit2 * sizeof ( PLINT ))) == NULL )
                        {
                            plexit( "fill_intersection_polygon: Insufficient memory" );
                        }
                        /* Common boundary between split1 and split2. */
                        k           = 0;
                        xsplit1[k]  = xintersect[0];
                        ysplit1[k]  = yintersect[0];
                        ifsplit1[k] = 1;
                        xsplit2[k]  = xintersect[0];
                        ysplit2[k]  = yintersect[0];
                        ifsplit2[k] = 1;
                        kstart      = k + 1;
                        kk          = kkstart1;
                        /* No wrap checks on kk index below because
                         * it must always be in valid range (since
                         * polygon 1 traversed only once). */
                        for ( k = kstart; k < range1 + 1; k++ )
                        {
                            xsplit1[k]  = x1[kk];
                            ysplit1[k]  = y1[kk];
                            ifsplit1[k] = 2;
                            xsplit2[k]  = x1[kk];
                            ysplit2[k]  = y1[kk++];
                            ifsplit2[k] = 2;
                        }
                        xsplit1[k]  = xintersect[1];
                        ysplit1[k]  = yintersect[1];
                        ifsplit1[k] = 1;
                        xsplit2[k]  = xintersect[1];
                        ysplit2[k]  = yintersect[1];
                        ifsplit2[k] = 1;

                        /* Finish off collecting split1 using ascending kk
                         * values. */
                        kstart = k + 1;
                        kk     = kkstart21;
                        for ( k = kstart; k < nsplit1; k++ )
                        {
                            xsplit1[k]  = x2[kk];
                            ysplit1[k]  = y2[kk];
                            ifsplit1[k] = if2[kk++];
                            if ( kk >= n2 )
                                kk -= n2;
                        }
                        fill_intersection_polygon( recursion_depth + 1, ifextrapolygon, fill,
                            x1, y1, i1start, n1,
                            xsplit1, ysplit1, ifsplit1, nsplit1 );
                        free( xsplit1 );
                        free( ysplit1 );
                        free( ifsplit1 );

                        /* Finish off collecting split2 using descending kk
                         * values. */
                        kk = kkstart22;
                        for ( k = kstart; k < nsplit2; k++ )
                        {
                            xsplit2[k]  = x2[kk];
                            ysplit2[k]  = y2[kk];
                            ifsplit2[k] = if2[kk--];
                            if ( kk < 0 )
                                kk += n2;
                        }
                        fill_intersection_polygon( recursion_depth + 1, ifextrapolygon, fill,
                            x1, y1, i1start, n1,
                            xsplit2, ysplit2, ifsplit2, nsplit2 );
                        free( xsplit2 );
                        free( ysplit2 );
                        free( ifsplit2 );
                        return;
                    }
                }
            }
            i2m1   = i2;
            i2wrap = i2m1;
        }
        i1m1   = i1;
        i1wrap = i1m1;
    }

    if ( nintersect != 0 )
    {
        plwarn( "fill_intersection_polygon: Internal error; nintersect != 0." );
        return;
    }

    /* This end phase is reached only if no intersections are found.
     * For this case, the intersection of polygon 2 and 1, must be
     * either of them (in which case fill with the inner one), or neither
     * of them (in which case don't fill at all). */

    /* Look for first vertex in polygon 2 that is inside polygon 1. */
    for ( i2 = 0; i2 < n2; i2++ )
    {
        /* Do not bother checking vertices already known to be on the
         * boundary with polygon 1. */
        if ( !if2[i2] && !notpointinpolygon( n1, x1, y1, x2[i2], y2[i2] ))
            break;
    }

    if ( i2 < n2 )
    {
        /* All of polygon 2 inside polygon 1. */
        nfill   = n2;
        xfiller = x2;
        yfiller = y2;
    }
    else
    {
        /* Look for first vertex in polygon 1 that is inside polygon 2. */
        for ( i1 = 0; i1 < n1; i1++ )
        {
            if ( !notpointinpolygon( n2, x2, y2, x1[i1], y1[i1] ))
                break;
        }

        if ( i1 < n1 )
        {
            /* All of polygon 1 inside polygon 2 */
            nfill   = n1;
            xfiller = x1;
            yfiller = y1;
        }
        else
        {
        }
    }
    if ( nfill > 0 )
    {
        if (( xfill = (short *) malloc( nfill * sizeof ( short ))) == NULL )
        {
            plexit( "fill_intersection_polygon: Insufficient memory" );
        }
        if (( yfill = (short *) malloc( nfill * sizeof ( short ))) == NULL )
        {
            plexit( "fill_intersection_polygon: Insufficient memory" );
        }
        for ( ifill = 0; ifill < nfill; ifill++ )
        {
            xfill[ifill] = xfiller[ifill];
            yfill[ifill] = yfiller[ifill];
        }
        ( *fill )( xfill, yfill, nfill );
        free( xfill );
        free( yfill );
    }

    return;
}

/* Find if an intersection (N.B. including end points) exists between
 * straight line segments A and B defined by their endpoints xA1, yA1,
 * xA2, yA2, xB1, yB1, xB2, yB2.  If not return true (-1 if parallel,
 * 1 if intersecting away from segments).  If true, return false.
 * Except for the parallel line case always return the intersection,
 * (xintersect, yintersect) via the argument list. */

int
notintersect( PLINT * xintersect, PLINT * yintersect,
              PLINT xA1, PLINT yA1, PLINT xA2, PLINT yA2,
              PLINT xB1, PLINT yB1, PLINT xB2, PLINT yB2 )
{
    PLFLT factor, fxintersect, fyintersect;
    /* These variables are PLFLT not for precision, but to
     * avoid integer overflows if they were typed as PLINT.  */
    PLFLT xA2A1, yA2A1, xB2B1, yB2B1;
    PLFLT xB1A1, yB1A1, xB2A1, yB2A1;
    /*
     * Two linear equations to be solved for x and y.
     * y = ((x - xA1)*yA2 + (xA2 - x)*yA1)/(xA2 - xA1)
     * y = ((x - xB1)*yB2 + (xB2 - x)*yB1)/(xB2 - xB1)
     *
     * Transform those two equations to coordinate system with origin
     * at (xA1, yA1).
     * y' = x'*yA2A1/xA2A1
     * y' = ((x' - xB1A1)*yB2A1 + (xB2A1 - x')*yB1A1)/xB2B1
     * ==>
     * x' = -(
     * (-xB1A1*yB2A1 + xB2A1*yB1A1)/xB2B1)/
     * (yB2B1/xB2B1 - yA2A1/xA2A1)
     * = (xB1A1*yB2A1 - xB2A1*yB1A1)*xA2A1/
     * (xA2A1*yB2B1 - yA2A1*xB2B1)
     *
     */

    xA2A1 = xA2 - xA1;
    yA2A1 = yA2 - yA1;
    xB2B1 = xB2 - xB1;
    yB2B1 = yB2 - yB1;

    factor = xA2A1 * yB2B1 - yA2A1 * xB2B1;
    if ( fabs( factor ) == 0. )
    {
        /* Two line segments are parallel */
        /* For this case check for an intersect for each of the end
         * points of A segment against the B segment. */
        fxintersect = xA1;
        fyintersect = yA1;
        if ( BETW( fxintersect, xB1, xB2 ) && BETW( fyintersect, yB1, yB2 ))
        {
            *xintersect = fxintersect;
            *yintersect = fyintersect;
            return 0;
        }
        else
        {
            fxintersect = xA2;
            fyintersect = yA2;
            if ( BETW( fxintersect, xB1, xB2 ) && BETW( fyintersect, yB1, yB2 ))
            {
                *xintersect = fxintersect;
                *yintersect = fyintersect;
                return 0;
            }
            else
            {
                /* parallel and no intersect. */
                return -1;
            }
        }
    }
    xB1A1 = xB1 - xA1;
    yB1A1 = yB1 - yA1;
    xB2A1 = xB2 - xA1;
    yB2A1 = yB2 - yA1;

    factor      = ( xB1A1 * yB2A1 - yB1A1 * xB2A1 ) / factor;
    fxintersect = factor * xA2A1 + xA1;
    *xintersect = fxintersect;
    fyintersect = factor * yA2A1 + yA1;
    *yintersect = fyintersect;

    /* The "redundant" x and y segment range checks (which include end points)
     * are needed for the vertical and the horizontal cases. */
    if (( BETW( fxintersect, xA1, xA2 ) && BETW( fyintersect, yA1, yA2 )) &&
        ( BETW( fxintersect, xB1, xB2 ) && BETW( fyintersect, yB1, yB2 )))
        return 0;
    else
        return 1;
}
