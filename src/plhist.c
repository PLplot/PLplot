/* $Id$
   $Log$
   Revision 1.7  1993/07/01 22:13:38  mjl
   Changed all plplot source files to include plplotP.h (private) rather than
   plplot.h.  Rationalized namespace -- all externally-visible internal
   plplot functions now start with "plP_".

 * Revision 1.6  1993/02/23  05:14:32  mjl
 * Changed reference in error message from plstar to plinit.
 *
 * Revision 1.5  1993/01/23  05:55:21  mjl
 * Formatting changes only, I think.
 *
 * Revision 1.4  1992/10/12  17:08:04  mjl
 * Added PL_NEED_SIZE_T define to those files that need to know the value
 * of (size_t) for non-POSIX systems (in this case the Amiga) that require you
 * to include <stddef.h> to get it.
 *
 * Revision 1.3  1992/09/30  18:25:50  furnish
 * Massive cleanup to irradicate garbage code.  Almost everything is now
 * prototyped correctly.  Builds on HPUX, SUNOS (gcc), AIX, and UNICOS.
 *
 * Revision 1.2  1992/09/29  04:46:01  furnish
 * Massive clean up effort to remove support for garbage compilers (K&R).
 *
 * Revision 1.1  1992/05/20  21:34:32  furnish
 * Initial checkin of the whole PLPLOT project.
 *
*/

/*	plhist.c

	Histogram plotter.
*/

#define PL_NEED_MALLOC
#define PL_NEED_SIZE_T
#include "plplotP.h"

#include <stdlib.h>
#include <math.h>

/*----------------------------------------------------------------------*\
* void plhist()
*
* Draws a histogram of n values of a variable in array data[0..n-1] in
* the range datmin to datmax using nbin bins. If "oldwin" is 1, the
* histogram is plotted in the current window. If not, the routine calls
* "plenv" to set up the graphics environment.
\*----------------------------------------------------------------------*/

void
c_plhist(PLINT n, PLFLT *data, PLFLT datmin, PLFLT datmax,
	 PLINT nbin, PLINT oldwin)
{
    PLINT bin, level;
    PLFLT *x, *y, dx, ymax;
    short i;

    plP_glev(&level);
    if (level < 1)
	plexit("plhist: Please call plinit first.");

    if (level < 3 && oldwin)
	plexit("plhist: Please set up window first.");

    if (datmin >= datmax)
	plexit("plhist: Data range invalid.");

    if (!(x = (PLFLT *) malloc((size_t) nbin * sizeof(PLFLT))))
	 plexit("plhist: Out of memory");

    if (!(y = (PLFLT *) malloc((size_t) nbin * sizeof(PLFLT))))
	 plexit("plhist: Out of memory");

    dx = (datmax - datmin) / nbin;
    for (i = 0; i < nbin; i++) {
	x[i] = datmin + i * dx;
	y[i] = 0.0;
    }

    for (i = 0; i < n; i++) {
	bin = (data[i] - datmin) / dx;
	bin = bin > 0 ? bin : 0;
	bin = bin < nbin ? bin : nbin - 1;
	y[bin]++;
    }

    if (!oldwin) {
	ymax = 0.0;
	for (i = 0; i < nbin; i++)
	    ymax = MAX(ymax, y[i]);
	plenv(datmin, datmax, (PLFLT) 0.0, (PLFLT) (1.1 * ymax), 0, 0);
    }

    plbin(nbin, x, y, 0);
    free((void *) x);
    free((void *) y);
}

/*----------------------------------------------------------------------*\
* void plbin()
*
* Plot a histogram using the arrays x and y to represent data values
* and frequencies respectively. If center is false, x values denote the
* lower edge of the bin, and if center is true, they denote the center
* of the bin.
\*----------------------------------------------------------------------*/

void
c_plbin(PLINT nbin, PLFLT *x, PLFLT *y, PLINT center)
{
    PLINT i;
    PLFLT xmin, xmax, vpwxmi, vpwxma, vpwymi, vpwyma;
    PLINT level;

    plP_glev(&level);
    if (level < 3)
	plexit("plbin: Please set up window first.");

    /* Check x[i] are in ascending order */

    for (i = 0; i < nbin - 1; i++)
	if (x[i] >= x[i + 1])
	    plexit("plbin: Elements of x[] must be increasing.");

    plP_gvpw(&vpwxmi, &vpwxma, &vpwymi, &vpwyma);
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
