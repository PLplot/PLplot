/* $Id$
   $Log$
   Revision 1.2  1992/09/29 04:45:54  furnish
   Massive clean up effort to remove support for garbage compilers (K&R).

 * Revision 1.1  1992/05/20  21:34:23  furnish
 * Initial checkin of the whole PLPLOT project.
 *
*/

/*	plerr.c

	Plot error bars.
*/

#include "plplot.h"
#include <math.h>

/* Plots horizontal error bars (xmin(i),y(i)) to (xmax(i),y(i)) */

void 
c_plerrx( PLINT n, PLFLT *xmin, PLFLT *xmax, PLFLT *y )
{
    PLINT level;
    short i;

    glev(&level);
    if (level < 3)
	plexit("Please set up window before calling plerrx.");
    for (i = 0; i < n; i++)
	plerx1(xmin[i], xmax[i], y[i]);
}

/* Plots vertical error bars (x,ymin(i)) to (x(i),ymax(i)) */

void 
c_plerry( PLINT n, PLFLT *x, PLFLT *ymin, PLFLT *ymax )
{
    PLINT level;
    short i;

    glev(&level);
    if (level < 3)
	plexit("Please set up window before calling plerry.");
    for (i = 0; i < n; i++)
	plery1(x[i], ymin[i], ymax[i]);
}

/* Plots single horizontal error bar */

void plerx1 (PLFLT xmin, PLFLT xmax, PLFLT y)
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

/* Plots single vertical error bar */

void plery1 (PLFLT x, PLFLT ymin, PLFLT ymax)
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
