/* $Id$
 * $Log$
 * Revision 1.9  1994/06/30 18:22:10  mjl
 * All core source files: made another pass to eliminate warnings when using
 * gcc -Wall.  Lots of cleaning up: got rid of includes of math.h or string.h
 * (now included by plplot.h), and other minor changes.  Now each file has
 * global access to the plstream pointer via extern; many accessor functions
 * eliminated as a result.
 *
 * Revision 1.8  1994/03/23  08:15:17  mjl
 * Some cruft elimination.
 *
 * All external API source files: replaced call to plexit() on simple
 * (recoverable) errors with simply printing the error message (via
 * plabort()) and returning.  Should help avoid loss of computer time in some
 * critical circumstances (during a long batch run, for example).
*/

/*	plhist.c

	Histogram plotter.
*/

#include "plplotP.h"

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
    PLINT i, bin;
    PLFLT *x, *y, dx, ymax;

    if (plsc->level < 1) {
	plabort("plhist: Please call plinit first");
	return;
    }
    if (plsc->level < 3 && oldwin) {
	plabort("plhist: Please set up window first");
	return;
    }
    if (datmin >= datmax) {
	plabort("plhist: Data range invalid");
	return;
    }
    if ( ! (x = (PLFLT *) malloc((size_t) nbin * sizeof(PLFLT)))) {
	plabort("plhist: Out of memory");
	return;
    }
    if ( ! (y = (PLFLT *) malloc((size_t) nbin * sizeof(PLFLT)))) {
	free((void *) x);
	plabort("plhist: Out of memory");
	return;
    }

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

    if ( ! oldwin) {
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

    if (plsc->level < 3) {
	plabort("plbin: Please set up window first");
	return;
    }

    /* Check x[i] are in ascending order */

    for (i = 0; i < nbin - 1; i++) {
	if (x[i] >= x[i + 1]) {
	    plabort("plbin: Elements of x array must be increasing");
	    return;
	}
    }

    plP_gvpw(&vpwxmi, &vpwxma, &vpwymi, &vpwyma);
    if ( ! center) {
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
