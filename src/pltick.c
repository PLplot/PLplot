/* $Id$
   $Log$
   Revision 1.3  1993/01/23 06:02:04  mjl
   Now holds all functions dealing with tick generation, including error
   bars.

 * Revision 1.2  1992/09/29  04:46:23  furnish
 * Massive clean up effort to remove support for garbage compilers (K&R).
 *
 * Revision 1.1  1992/05/20  21:34:53  furnish
 * Initial checkin of the whole PLPLOT project.
 *
*/

/*	pltick.c

	Routines for drawing error bars and tick marks.
*/

#include "plplot.h"

/*----------------------------------------------------------------------*\
* void plxtik()
*
* Draws a tick parallel to x.
\*----------------------------------------------------------------------*/

void
plxtik(PLINT x, PLINT y, PLINT below, PLINT above)
{
    draphy(x, y);
    if (below != 0)
	draphy(x, y - below);
    if (above != 0)
	draphy(x, y + above);
    draphy(x, y);
}

/*----------------------------------------------------------------------*\
* void plytik()
*
* Draws a tick parallel to y.
\*----------------------------------------------------------------------*/

void
plytik(PLINT x, PLINT y, PLINT left, PLINT right)
{
    draphy(x, y);
    if (left != 0)
	draphy(x - left, y);
    if (right != 0)
	draphy(x + right, y);
    draphy(x, y);
}

/*----------------------------------------------------------------------*\
* void plstik()
*
* Draws a slanting tick at position (mx,my) (measured in mm) of
* vector length (dx,dy).
\*----------------------------------------------------------------------*/

void 
plstik(PLFLT mx, PLFLT my, PLFLT dx, PLFLT dy)
{
    draphy(mmpcx(mx), mmpcy(my));
    draphy(mmpcx((PLFLT) (mx + dx)), mmpcy((PLFLT) (my + dy)));
    draphy(mmpcx(mx), mmpcy(my));
}

/*----------------------------------------------------------------------*\
* void plerx1()
*
* Plot single horizontal error bar.
\*----------------------------------------------------------------------*/

static void
plerx1(PLFLT xmin, PLFLT xmax, PLFLT y)
{
    PLFLT mindef, minht, xpmm, ypmm;
    PLINT yminor;

    gmin(&mindef, &minht);
    gpixmm(&xpmm, &ypmm);
    yminor = MAX(1.0, minht * ypmm);
    movwor(xmin, y);
    plxtik(wcpcx(xmin), wcpcy(y), yminor, yminor);
    drawor(xmax, y);
    plxtik(wcpcx(xmax), wcpcy(y), yminor, yminor);
}

/*----------------------------------------------------------------------*\
* void plery1()
*
* Plot single vertical error bar.
\*----------------------------------------------------------------------*/

static void
plery1(PLFLT x, PLFLT ymin, PLFLT ymax)
{
    PLFLT mindef, minht, xpmm, ypmm;
    PLINT xminor;

    gmin(&mindef, &minht);
    gpixmm(&xpmm, &ypmm);
    xminor = MAX(1.0, minht * xpmm);
    movwor(x, ymin);
    plytik(wcpcx(x), wcpcy(ymin), xminor, xminor);
    drawor(x, ymax);
    plytik(wcpcx(x), wcpcy(ymax), xminor, xminor);
}

/*----------------------------------------------------------------------*\
* void plerrx()
*
* Plot horizontal error bars (xmin(i),y(i)) to (xmax(i),y(i)).
\*----------------------------------------------------------------------*/

void
c_plerrx(PLINT n, PLFLT *xmin, PLFLT *xmax, PLFLT *y)
{
    PLINT level;
    short i;

    glev(&level);
    if (level < 3)
	plexit("plerrx: Please set up window first.");

    for (i = 0; i < n; i++)
	plerx1(xmin[i], xmax[i], y[i]);
}

/*----------------------------------------------------------------------*\
* void plerry()
*
* Plot vertical error bars (x,ymin(i)) to (x(i),ymax(i)).
\*----------------------------------------------------------------------*/

void
c_plerry(PLINT n, PLFLT *x, PLFLT *ymin, PLFLT *ymax)
{
    PLINT level;
    short i;

    glev(&level);
    if (level < 3)
	plexit("plerry: Please set up window first.");

    for (i = 0; i < n; i++)
	plery1(x[i], ymin[i], ymax[i]);
}
