/* Test of plshade function */

/* $Id$
 * $Log$
 * Revision 1.1  1993/08/18 19:06:44  mjl
 * This file for testing plshade capability.  Just sketched out at this
 * point.
 *
*/

#define PL_NEED_MALLOC
#include "plplot.h"
#include <stdlib.h>
#include <math.h>

#define XPTS    35
#define YPTS    46
#define XSPA    2./(XPTS-1)
#define YSPA    2./(YPTS-1)
#ifndef PI
#define PI	3.1415926535897932384
#endif

PLFLT tr[6] =
{XSPA, 0.0, -1.0, 0.0, YSPA, -1.0};

void
mypltr(PLFLT x, PLFLT y, PLFLT *tx, PLFLT *ty, void *pltr_data)
{
    *tx = tr[0] * x + tr[1] * y + tr[2];
    *ty = tr[3] * x + tr[4] * y + tr[5];
}

static PLFLT clevel[11] =
{-1., -.8, -.6, -.4, -.2, 0, .2, .4, .6, .8, 1.};

int
main(int argc, char *argv[])
{
    int i, j;
    PLFLT xx, yy, argx, argy, distort;
    static PLINT mark = 1500, space = 1500;

    PLFLT z[XPTS][YPTS], w[XPTS][YPTS];
    PLFLT xg1[XPTS], yg1[YPTS];
    PLcGrid  cgrid1;
    PLcGrid2 cgrid2;

/* Parse and process command line arguments */

    (void) plParseInternalOpts(&argc, argv, PL_PARSE_FULL);

/* Initialize plplot */

    plinit();

/* Set up function arrays */

    for (i = 0; i < XPTS; i++) {
	xx = (double) (i - (XPTS / 2)) / (double) (XPTS / 2);
	for (j = 0; j < YPTS; j++) {
	    yy = (double) (j - (YPTS / 2)) / (double) (YPTS / 2) - 1.0;
	    z[i][j] = xx * xx - yy * yy;
	    w[i][j] = 2 * xx * yy;
	}
    }

/* Set up grids */

    cgrid1.xg = xg1;
    cgrid1.yg = yg1;
    cgrid1.nx = XPTS;
    cgrid1.ny = YPTS;

    Alloc2dGrid(&cgrid2.xg, XPTS, YPTS);
    Alloc2dGrid(&cgrid2.yg, XPTS, YPTS);
    cgrid2.nx = XPTS;
    cgrid2.ny = YPTS;

    for (i = 0; i < XPTS; i++) {
	for (j = 0; j < YPTS; j++) {
	    mypltr((PLFLT) i, (PLFLT) j, &xx, &yy, NULL);

	    argx = xx * PI/2;
	    argy = yy * PI/2;
	    distort = 0.4;

	    cgrid1.xg[i] = xx + distort * cos(argx);
	    cgrid1.yg[j] = yy - distort * cos(argy);

	    cgrid2.xg[i][j] = xx + distort * cos(argx) * cos(argy);
	    cgrid2.yg[i][j] = yy - distort * cos(argx) * cos(argy);
	}
    }

/* Plot using identity transform */

    plenv((PLFLT) -1.0, (PLFLT) 1.0, (PLFLT) -1.0, (PLFLT) 1.0, 0, 0);
    plcol(2);
{
    PLFLT shade_min, shade_max;	/* Defines the interval to be shaded. If */
				/* shade_max <= shade_min, plshade does */
				/* nothing. */

    PLINT sh_color, sh_width;	/* defines pen color, width used by the */
				/* fill pattern */
    PLINT min_color, min_width;
    PLINT max_color, max_width;	/* defines pen color, width used by the */
				/* boundary of shaded region. The min */
				/* values are used for the shade_min */
				/* boundary, and the max values are used */
				/* on the shade_max boundary. Set color */
				/* and width to zero for no plotted */
				/* boundaries. */

    shade_min = -0.2;
    shade_max = 0.2;
    min_color = 1;
    max_color = 8;
    min_width = 0;
    max_width = 0;

    plshade(&z, XPTS, YPTS, NULL, -1., 1., -1., 1., 
	    NULL, shade_min, shade_max, 
	    sh_color, sh_width, min_color, min_width,
	    max_color, max_width, plfill, 1);

}
    plcol(1);
    pllab("X Coordinate", "Y Coordinate", "Streamlines of flow");
}
