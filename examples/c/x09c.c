/* Demonstration of contour plotting */
/* $Id$
   $Log$
   Revision 1.3  1992/09/30 18:25:22  furnish
   Massive cleanup to irradicate garbage code.  Almost everything is now
   prototyped correctly.  Builds on HPUX, SUNOS (gcc), AIX, and UNICOS.

 * Revision 1.2  1992/09/29  04:45:17  furnish
 * Massive clean up effort to remove support for garbage compilers (K&R).
 *
 * Revision 1.1  1992/05/20  21:32:57  furnish
 * Initial checkin of the whole PLPLOT project.
 *
*/

/* Note the compiler should automatically convert all non-pointer arguments
   to satisfy the prototype, but some have problems with constants. */

#define PL_NEED_MALLOC
#include "plplot.h"

#define XPTS      35
#define YPTS      46
#define XSPA      2./(XPTS-1)
#define YSPA      2./(YPTS-1)

PLFLT tr[6] = {XSPA, 0.0, -1.0, 0.0, YSPA, -1.0};

void 
xform(PLFLT x, PLFLT y, PLFLT * tx, PLFLT * ty)
{
    *tx = tr[0] * x + tr[1] * y + tr[2];
    *ty = tr[3] * x + tr[4] * y + tr[5];
}

static PLFLT clevel[11] = {-1., -.8, -.6, -.4, -.2, 0, .2, .4, .6, .8, 1.};

int 
main (void)
{
    int i, j;
    PLFLT xx, yy;
    PLFLT **z, **w;
    static PLINT mark = 1500, space = 1500;

    z = (PLFLT **) malloc(XPTS * sizeof(PLFLT *));
    w = (PLFLT **) malloc(XPTS * sizeof(PLFLT *));
    for (i = 0; i < XPTS; i++) {
	z[i] = (PLFLT *) malloc(YPTS * sizeof(PLFLT));
	w[i] = (PLFLT *) malloc(YPTS * sizeof(PLFLT));
	xx = (double) (i - (XPTS / 2)) / (double) (XPTS / 2);
	for (j = 0; j < YPTS; j++) {
	    yy = (double) (j - (YPTS / 2)) / (double) (YPTS / 2) - 1.0;
	    z[i][j] = xx * xx - yy * yy;
	    w[i][j] = 2 * xx * yy;
	}
    }
    plstar(1, 1);
    plenv((PLFLT) -1.0, (PLFLT) 1.0, (PLFLT) -1.0, (PLFLT) 1.0, 0, 0);
    plcol(2);
    plcont(z, XPTS, YPTS, 1, XPTS, 1, YPTS, clevel, 11, xform);
    plstyl(1, &mark, &space);
    plcol(3);
    plcont(w, XPTS, YPTS, 1, XPTS, 1, YPTS, clevel, 11, xform);
    plcol(1);
    pllab("X Coordinate", "Y Coordinate", "Contour Plots of Saddle Points");
    plend();
}
