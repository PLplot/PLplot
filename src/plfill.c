/* $Id$
 *
 *      Polygon pattern fill.
 *
 * Copyright (C) 2004  Alan W. Irwin
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

#define DTOR    0.0174533
#define BINC    50

struct point
{
    PLINT x, y;
};
static PLINT bufferleng, buffersize, *buffer;

/* Static function prototypes */
/* INDENT OFF */

static int
compar( const void *, const void * );

static void
addcoord( PLINT, PLINT );

static void
tran( PLINT *, PLINT *, PLFLT, PLFLT );

static void
buildlist( PLINT, PLINT, PLINT, PLINT, PLINT, PLINT, PLINT );

static int
pointinpolygon( PLINT n, PLINT *x, PLINT *y, PLINT xp, PLINT yp );

static int
circulation( PLINT *x, PLINT *y, PLINT npts );


/* INDENT ON */

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

static void
tran( PLINT *a, PLINT *b, PLFLT c, PLFLT d )
{
    PLINT ta, tb;

    ta = *a;
    tb = *b;

    *a = (PLINT) floor((double) ( ta * c + tb * d + 0.5 ));
    *b = (PLINT) floor((double) ( tb * c - ta * d + 0.5 ));
}

static void
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

static void
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

static int
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

/* Must have at least 3 points and draw() specified */
    if ( npts < 3 || !draw ) return;

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

    inside_lb = pointinpolygon( npts, x, y, xmin, ymin );
    inside_lu = pointinpolygon( npts, x, y, xmin, ymax );
    inside_rb = pointinpolygon( npts, x, y, xmax, ymin );
    inside_ru = pointinpolygon( npts, x, y, xmax, ymax );

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

static int
circulation( PLINT *x, PLINT *y, PLINT npts )
{
    PLFLT xproduct;
    int   direction = 0;
    PLFLT x1, y1, x2, y2, x3, y3;
    int   i;

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

/*----------------------------------------------------------------------*\
 * int pointinpolygon()
 *
 * PLINT wrapper for plP_pointinpolygon.
 \*----------------------------------------------------------------------*/

static int
pointinpolygon( PLINT n, PLINT *x, PLINT *y, PLINT xp, PLINT yp )
{
    int   i, return_value;
    PLFLT *xflt, *yflt;
    if (( xflt = (PLFLT *) malloc( n * sizeof ( PLFLT ))) == NULL )
    {
        plexit( "pointinpolygon: Insufficient memory" );
    }
    if (( yflt = (PLFLT *) malloc( n * sizeof ( PLFLT ))) == NULL )
    {
        plexit( "pointinpolygon: Insufficient memory" );
    }
    for ( i = 0; i < n; i++ )
    {
        xflt[i] = (PLFLT) x[i];
        yflt[i] = (PLFLT) y[i];
    }
    return_value = plP_pointinpolygon( n, xflt, yflt, (PLFLT) xp, (PLFLT) yp );
    free( xflt );
    free( yflt );
    return return_value;
}
/*----------------------------------------------------------------------*\
 * int plP_pointinpolygon()
 *
 * Returns 1 if the point is inside the polygon, 0 otherwise
 * Notes:
 * Points on the polygon are considered to be outside.
 * This "Ray casting algorithm" has been described in
 * http://en.wikipedia.org/wiki/Point_in_polygon.
 * Logic still needs to be inserted to take care of the "ray passes
 * through vertex" problem in a numerically robust way.
 \*----------------------------------------------------------------------*/

int
plP_pointinpolygon( PLINT n, PLFLT *x, PLFLT *y, PLFLT xp, PLFLT yp )
{
    int   i;
    int   count_crossings;
    PLFLT x1, y1, x2, y2, xout, yout, xmax;
    PLFLT xvp, yvp, xvv, yvv, xv1, yv1, xv2, yv2;
    PLFLT inprod1, inprod2;

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

    xvp = xp - xout;
    yvp = yp - yout;

    for ( i = 0; i < n; i++ )
    {
        x1 = x[i];
        y1 = y[i];
        if ( i < n - 1 )
        {
            x2 = x[i + 1];
            y2 = y[i + 1];
        }
        else
        {
            x2 = x[0];
            y2 = y[0];
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
         * Are xout and xp on either side? */
        xvv     = x2 - x1;
        yvv     = y2 - y1;
        xv1     = xp - x1;
        yv1     = yp - y1;
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

    return ( count_crossings % 2 );
}
