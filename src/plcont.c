/* $Id$
   $Log$
   Revision 1.4  1992/09/30 18:25:44  furnish
   Massive cleanup to irradicate garbage code.  Almost everything is now
   prototyped correctly.  Builds on HPUX, SUNOS (gcc), AIX, and UNICOS.

 * Revision 1.3  1992/09/29  04:45:51  furnish
 * Massive clean up effort to remove support for garbage compilers (K&R).
 *
 * Revision 1.2  1992/07/31  06:03:13  mjl
 * Minor bug fixes.
 *
 * Revision 1.1  1992/05/20  21:34:20  furnish
 * Initial checkin of the whole PLPLOT project.
 *
*/

/*	plcont.c

	Contour plotter.
*/

#include <stdio.h>
#include <stdlib.h>

#define PL_NEED_MALLOC
#include "plplot.h"

/*----------------------------------------------------------------------*\
* void plcont()
*
* Draws a contour plot from data in z(nx,ny), using the subarray 
* from kx to lx in the x direction and from ky to ly in the y 
* direction. The array of contour levels is clevel(nlevel), and 
* "pltr" is the name of a subroutine which transforms array indicies 
* into world coordinates.
\*----------------------------------------------------------------------*/

void 
c_plcont(PLFLT ** z, PLINT nx, PLINT ny, PLINT kx, PLINT lx,
       PLINT ky, PLINT ly, PLFLT * clevel, PLINT nlevel,
       void (*pltr) (PLFLT, PLFLT, PLFLT *, PLFLT *))
{
    PLINT i, mx, my, nstor, *heapc;

    mx = lx - kx + 1;
    my = ly - ky + 1;

    if (kx < 1 || lx > nx || kx >= lx || ky < 1 || ky > ny || ky >= ly)
	plexit("Argument error in plcont.");

    nstor = mx * my;
    if ((heapc = (PLINT *) malloc((size_t)(mx + 2 * nstor) * sizeof(PLINT))) 
		== NULL)
	plexit("Out of memory in plcont.");
    for (i = 0; i < nlevel; i++) {
	plcntr(z, nx, ny, kx, lx, ky, ly, clevel[i], &heapc[0],
	       &heapc[nx], &heapc[nx + nstor], nstor, pltr);
    }
    free((VOID *) heapc);
}

/* Function for no transformation case */

void 
pltr0(PLFLT x, PLFLT y, PLFLT * tx, PLFLT * ty)
{
    *tx = x;
    *ty = y;
}
