/* $Id$

	Contour plot demo.
*/

#include "plplot/plcdemos.h"

#define XPTS    35		/* Data points in x */
#define YPTS    46		/* Datat points in y */

#define XSPA    2./(XPTS-1)
#define YSPA    2./(YPTS-1)

static PLFLT clevel[11] =
{-1., -.8, -.6, -.4, -.2, 0, .2, .4, .6, .8, 1.};

/* Transformation function */

PLFLT tr[6] =
{XSPA, 0.0, -1.0, 0.0, YSPA, -1.0};

void
mypltr(PLFLT x, PLFLT y, PLFLT *tx, PLFLT *ty, void *pltr_data)
{
    *tx = tr[0] * x + tr[1] * y + tr[2];
    *ty = tr[3] * x + tr[4] * y + tr[5];
}

/*--------------------------------------------------------------------------*\
 * main
 *
 * Does several contour plots using different coordinate mappings.
\*--------------------------------------------------------------------------*/

int
main(int argc, char *argv[])
{
    int i, j;
    PLFLT xx, yy, argx, argy, distort;
    static PLINT mark = 1500, space = 1500;

    PLFLT **z, **w;
    PLFLT xg1[XPTS], yg1[YPTS];
    PLcGrid  cgrid1;
    PLcGrid2 cgrid2;

/* Parse and process command line arguments */

    (void) plParseOpts(&argc, argv, PL_PARSE_FULL);

/* Initialize plplot */

    plinit();

/* Set up function arrays */

    plAlloc2dGrid(&z, XPTS, YPTS);
    plAlloc2dGrid(&w, XPTS, YPTS);

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

    plAlloc2dGrid(&cgrid2.xg, XPTS, YPTS);
    plAlloc2dGrid(&cgrid2.yg, XPTS, YPTS);
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

    plenv(-1.0, 1.0, -1.0, 1.0, 0, 0);
    plcol0(2);
    plcont(z, XPTS, YPTS, 1, XPTS, 1, YPTS, clevel, 11, mypltr, NULL);
    plstyl(1, &mark, &space);
    plcol0(3);
    plcont(w, XPTS, YPTS, 1, XPTS, 1, YPTS, clevel, 11, mypltr, NULL);
    plstyl(0, &mark, &space);
    plcol0(1);
    pllab("X Coordinate", "Y Coordinate", "Streamlines of flow");

    pl_setcontlabelparam(0.006, 0.3, 0.1, 1);
    plenv(-1.0, 1.0, -1.0, 1.0, 0, 0);
    plcol0(2);
    plcont(z, XPTS, YPTS, 1, XPTS, 1, YPTS, clevel, 11, mypltr, NULL);
    plstyl(1, &mark, &space);
    plcol0(3);
    plcont(w, XPTS, YPTS, 1, XPTS, 1, YPTS, clevel, 11, mypltr, NULL);
    plstyl(0, &mark, &space);
    plcol0(1);
    pllab("X Coordinate", "Y Coordinate", "Streamlines of flow");
    pl_setcontlabelparam(0.006, 0.3, 0.1, 0);
    
/* Plot using 1d coordinate transform */

    plenv(-1.0, 1.0, -1.0, 1.0, 0, 0);
    plcol0(2);
    plcont(z, XPTS, YPTS, 1, XPTS, 1, YPTS, clevel, 11,
	   pltr1, (void *) &cgrid1);

    plstyl(1, &mark, &space);
    plcol0(3);
    plcont(w, XPTS, YPTS, 1, XPTS, 1, YPTS, clevel, 11,
	   pltr1, (void *) &cgrid1);
    plstyl(0, &mark, &space);
    plcol0(1);
    pllab("X Coordinate", "Y Coordinate", "Streamlines of flow");

    pl_setcontlabelparam(0.006, 0.3, 0.1, 1);
    plenv(-1.0, 1.0, -1.0, 1.0, 0, 0);
    plcol0(2);
    plcont(z, XPTS, YPTS, 1, XPTS, 1, YPTS, clevel, 11,
	   pltr1, (void *) &cgrid1);

    plstyl(1, &mark, &space);
    plcol0(3);
    plcont(w, XPTS, YPTS, 1, XPTS, 1, YPTS, clevel, 11,
	   pltr1, (void *) &cgrid1);
    plstyl(0, &mark, &space);
    plcol0(1);
    pllab("X Coordinate", "Y Coordinate", "Streamlines of flow");
    pl_setcontlabelparam(0.006, 0.3, 0.1, 0);
    
/* Plot using 2d coordinate transform */

    plenv(-1.0, 1.0, -1.0, 1.0, 0, 0);
    plcol0(2);
    plcont(z, XPTS, YPTS, 1, XPTS, 1, YPTS, clevel, 11,
	   pltr2, (void *) &cgrid2);

    plstyl(1, &mark, &space);
    plcol0(3);
    plcont(w, XPTS, YPTS, 1, XPTS, 1, YPTS, clevel, 11,
	   pltr2, (void *) &cgrid2);
    plstyl(0, &mark, &space);
    plcol0(1);
    pllab("X Coordinate", "Y Coordinate", "Streamlines of flow");

    pl_setcontlabelparam(0.006, 0.3, 0.1, 1);
    plenv(-1.0, 1.0, -1.0, 1.0, 0, 0);
    plcol0(2);
    plcont(z, XPTS, YPTS, 1, XPTS, 1, YPTS, clevel, 11,
	   pltr2, (void *) &cgrid2);

    plstyl(1, &mark, &space);
    plcol0(3);
    plcont(w, XPTS, YPTS, 1, XPTS, 1, YPTS, clevel, 11,
	   pltr2, (void *) &cgrid2);
    plstyl(0, &mark, &space);
    plcol0(1);
    pllab("X Coordinate", "Y Coordinate", "Streamlines of flow");
    
    plend();
    free((void *) w);
    free((void *) z);
    exit(0);
}
