/* $Id$
   $Log$
   Revision 1.1  1992/05/20 21:34:23  furnish
   Initial checkin of the whole PLPLOT project.

*/

/*	plerr.c

	Plot error bars.
*/

#include "plplot.h"
#include <math.h>

/* Plots horizontal error bars (xmin(i),y(i)) to (xmax(i),y(i)) */

void 
c_plerrx(n, xmin, xmax, y)
PLINT n;
PLFLT *xmin, *xmax, *y;
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
c_plerry(n, x, ymin, ymax)
PLINT n;
PLFLT *x, *ymin, *ymax;
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

#ifdef PLSTDC
void plerx1 (PLFLT xmin, PLFLT xmax, PLFLT y)
#else
void 
plerx1(xmin, xmax, y)
PLFLT xmin, xmax, y;
#endif
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

#ifdef PLSTDC
void plery1 (PLFLT x, PLFLT ymin, PLFLT ymax)
#else
void 
plery1(x, ymin, ymax)
PLFLT x, ymin, ymax;
#endif
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
