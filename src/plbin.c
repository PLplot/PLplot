/* $Id$
   $Log$
   Revision 1.2  1992/09/29 04:45:44  furnish
   Massive clean up effort to remove support for garbage compilers (K&R).

 * Revision 1.1  1992/05/20  21:34:13  furnish
 * Initial checkin of the whole PLPLOT project.
 *
*/

/*	plbin.c

	Histogram plotter.
*/

#include "plplot.h"
#include <math.h>

/*----------------------------------------------------------------------*\
* void plbin()
*
* Plot a histogram using the arrays x and y to represent data values
* and frequencies respectively. If center is false, x values denote the
* lower edge of the bin, and if center is true, they denote the center
* of the bin.
\*----------------------------------------------------------------------*/

void 
c_plbin( PLINT nbin, PLFLT *x, PLFLT *y, PLINT center )
{
    PLINT i;
    PLFLT xmin, xmax, vpwxmi, vpwxma, vpwymi, vpwyma;
    PLINT level;

    glev(&level);
    if (level < 3)
	plexit("Please set up window before calling plbin.");

    /* Check x[i] are in ascending order */
    for (i = 0; i < nbin - 1; i++)
	if (x[i] >= x[i + 1])
	    plexit("Elements of x[] must be increasing in plbin.");

    gvpw(&vpwxmi, &vpwxma, &vpwymi, &vpwyma);
    if (!center) {
	for (i = 0; i < nbin - 1; i++) {
	    pljoin(x[i], vpwymi, x[i], y[i]);
	    pljoin(x[i], y[i], x[i + 1], y[i]);
	    pljoin(x[i + 1], y[i], x[i + 1], vpwymi);
	}
	if (x[nbin - 1] < vpwxma) {
	    pljoin(x[nbin - 1], vpwymi, x[nbin - 1], y[nbin - 1]);
	    pljoin(x[nbin - 1], y[nbin - 1], vpwxma, y[nbin - 1]);
	    pljoin(vpwxma, y[nbin - 1], vpwxma, vpwymi);
	}
    }
    else {
	if (nbin < 2)
	    return;
	xmin = vpwxmi;
	xmax = MAX(0.5 * (x[0] + x[2]), vpwxmi);
	if (xmin < xmax) {
	    pljoin(xmin, vpwymi, xmin, y[0]);
	    pljoin(xmin, y[0], xmax, y[0]);
	    pljoin(xmax, y[0], xmax, vpwymi);
	}
	for (i = 1; i < nbin - 1; i++) {
	    xmin = xmax;
	    xmax = MIN(0.5 * (x[i] + x[i + 1]), vpwxma);
	    pljoin(xmin, vpwymi, xmin, y[i]);
	    pljoin(xmin, y[i], xmax, y[i]);
	    pljoin(xmax, y[i], xmax, vpwymi);
	}
	xmin = xmax;
	xmax = vpwxma;
	if (xmin < xmax) {
	    pljoin(xmin, vpwymi, xmin, y[nbin - 1]);
	    pljoin(xmin, y[nbin - 1], xmax, y[nbin - 1]);
	    pljoin(xmax, y[nbin - 1], xmax, vpwymi);
	}
    }
}
