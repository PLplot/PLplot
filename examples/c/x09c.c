/* Demonstration of contour plotting */
/* $Id$
   $Log$
   Revision 1.1  1992/05/20 21:32:57  furnish
   Initial checkin of the whole PLPLOT project.

*/

/* Note the compiler should automatically convert all non-pointer arguments
   to satisfy the prototype, but some have problems with constants. */

#include "plplot.h"

#define XPTS      35
#define YPTS      46
#define XSPA      2./(XPTS-1)
#define YSPA      2./(YPTS-1)

FLOAT tr[6] = {XSPA, 0.0, -1.0, 0.0, YSPA, -1.0};

#ifdef PLSTDC
void 
xform(FLOAT x, FLOAT y, FLOAT * tx, FLOAT * ty)
#else
void 
xform(x, y, tx, ty)
FLOAT x, y, *tx, *ty;
#endif
{
    *tx = tr[0] * x + tr[1] * y + tr[2];
    *ty = tr[3] * x + tr[4] * y + tr[5];
}

static FLOAT clevel[11] = {-1., -.8, -.6, -.4, -.2, 0, .2, .4, .6, .8, 1.};

main()
{
    int i, j;
    FLOAT xx, yy;
    FLOAT **z, **w;
    static INT mark = 1500, space = 1500;
    char *malloc();

    z = (FLOAT **) malloc(XPTS * sizeof(FLOAT *));
    w = (FLOAT **) malloc(XPTS * sizeof(FLOAT *));
    for (i = 0; i < XPTS; i++) {
	z[i] = (FLOAT *) malloc(YPTS * sizeof(FLOAT));
	w[i] = (FLOAT *) malloc(YPTS * sizeof(FLOAT));
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
