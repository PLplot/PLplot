/* $Id$
   $Log$
   Revision 1.1  1992/05/20 21:34:32  furnish
   Initial checkin of the whole PLPLOT project.

*/

/*	plhist.c

	Histogram plotter.
*/

#include "plplot.h"
#include <math.h>

#ifdef PLSTDC
#include <stdlib.h>
#ifdef INCLUDE_STDDEF
#include <stddef.h>
#endif
#ifdef INCLUDE_MALLOC
#include <malloc.h>
#endif

#else
extern char *malloc();
extern void free();
#define size_t	int
#endif

/*----------------------------------------------------------------------*\
* void plhist()
*
* Draws a histogram of n values of a variable in array data[0..n-1] in
* the range datmin to datmax using nbin bins. If "oldwin" is 1, the
* histogram is plotted in the current window. If not, the routine calls
* "plenv" to set up the graphics environment. 
\*----------------------------------------------------------------------*/

#ifdef PLSTDC
void
c_plhist (PLINT n, PLFLT * data, PLFLT datmin, PLFLT datmax,
	  PLINT nbin, PLINT oldwin)
#else
void 
c_plhist(n, data, datmin, datmax, nbin, oldwin)
PLINT n, nbin, oldwin;
PLFLT *data, datmin, datmax;
#endif
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
