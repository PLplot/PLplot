/* $Id$
   $Log$
   Revision 1.4  1992/10/12 17:08:04  mjl
   Added PL_NEED_SIZE_T define to those files that need to know the value
   of (size_t) for non-POSIX systems (in this case the Amiga) that require you
   to include <stddef.h> to get it.

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

#include "plplot.h"
#include <math.h>
#include <stdlib.h>

/*----------------------------------------------------------------------*\
* void plhist()
*
* Draws a histogram of n values of a variable in array data[0..n-1] in
* the range datmin to datmax using nbin bins. If "oldwin" is 1, the
* histogram is plotted in the current window. If not, the routine calls
* "plenv" to set up the graphics environment. 
\*----------------------------------------------------------------------*/

void
c_plhist (PLINT n, PLFLT * data, PLFLT datmin, PLFLT datmax,
	  PLINT nbin, PLINT oldwin)
{
    PLINT bin, level;
    PLFLT *x, *y, dx, ymax;
    short i;

    glev(&level);
    if (level < 1)
	plexit("Please call plstar before plhist.");
    if (level < 3 && oldwin)
	plexit("Please set up window before calling plhist.");
    if (datmin >= datmax)
	plexit("Data range invalid in plhist.");

    if (!(x = (PLFLT *) malloc((size_t) nbin * sizeof(PLFLT))))
	plexit("Out of memory in plhist");
    if (!(y = (PLFLT *) malloc((size_t) nbin * sizeof(PLFLT))))
	plexit("Out of memory in plhist");

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
    free((VOID *) x);
    free((VOID *) y);
}
