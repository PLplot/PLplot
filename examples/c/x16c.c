/* $Id$
 * $Log$
 * Revision 1.3  1994/04/25 19:02:55  mjl
 * Increased the number of shade regions to make it a bit more interesting
 * when playing with the palette.
 *
 * Revision 1.2  1994/04/08  12:08:21  mjl
 * Cleaned up some, switched to more interesting test function.
 *
 * Revision 1.1  1994/03/30  07:22:55  mjl
 * Added to test specifically color fill capability of plshade, with optional
 * coordinate mapping.
 *
*/

/* Demonstration of plshade plotting */

#include <plplot.h>
#include <math.h>

#define NCONTR	30
#define XPTS    35
#define YPTS    46
#define XSPA    2./(XPTS-1)
#define YSPA    2./(YPTS-1)
#ifndef PI
#define PI	3.1415926535897932384
#endif
#ifndef MAX
#define MAX(a,b)    (((a) > (b)) ? (a) : (b))
#endif
#ifndef MIN
#define MIN(a,b)    (((a) < (b)) ? (a) : (b))
#endif

PLFLT tr[6] =
{XSPA, 0.0, -1.0, 0.0, YSPA, -1.0};

void
mypltr(PLFLT x, PLFLT y, PLFLT *tx, PLFLT *ty, void *pltr_data)
{
    *tx = tr[0] * x + tr[1] * y + tr[2];
    *ty = tr[3] * x + tr[4] * y + tr[5];
}

static PLFLT clevel[NCONTR];

void
f2mnmx(PLFLT **f, PLINT nx, PLINT ny, PLFLT *fmin, PLFLT *fmax);

int
main(int argc, char *argv[])
{
    int i, j;
    PLFLT x, y, argx, argy, distort;
    static PLINT mark = 1500, space = 1500;

    PLFLT **z, **w, zmin, zmax;
    PLFLT xg1[XPTS], yg1[YPTS];
    PLcGrid  cgrid1;
    PLcGrid2 cgrid2;

    PLFLT shade_min, shade_max, sh_color;
    PLINT sh_cmap = 1, sh_width;
    PLINT min_color = 1, min_width = 0, max_color = 0, max_width = 0;

/* Parse and process command line arguments */

    (void) plParseInternalOpts(&argc, argv, PL_PARSE_FULL);

/* Reduce colors in cmap 0 so that cmap 1 is useful on a 16-color display */

    plscmap0n(3);

/* Initialize plplot */

    plinit();

/* Set up function arrays */

    plAlloc2dGrid(&z, XPTS, YPTS);
    plAlloc2dGrid(&w, XPTS, YPTS);

/* Set up data array */

    for (i = 0; i < XPTS; i++) {
	x = (double) (i - (XPTS / 2)) / (double) (XPTS / 2);
	for (j = 0; j < YPTS; j++) {
	    y = (double) (j - (YPTS / 2)) / (double) (YPTS / 2) - 1.0;

	    z[i][j] = - sin(7.*x) * cos(7.*y) + x*x - y*y;
	    w[i][j] = - cos(7.*x) * sin(7.*y) + 2 * x * y;
	}
    }
    f2mnmx(z, XPTS, YPTS, &zmin, &zmax);
    for (i = 0; i < NCONTR; i++)
	clevel[i] = zmin + (zmax - zmin) * (i + 0.5) / (float) NCONTR;

/* Set up coordinate grids */

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
	    mypltr((PLFLT) i, (PLFLT) j, &x, &y, NULL);

	    argx = x * PI/2;
	    argy = y * PI/2;
	    distort = 0.4;

	    cgrid1.xg[i] = x + distort * cos(argx);
	    cgrid1.yg[j] = y - distort * cos(argy);

	    cgrid2.xg[i][j] = x + distort * cos(argx) * cos(argy);
	    cgrid2.yg[i][j] = y - distort * cos(argx) * cos(argy);
	}
    }

/* Plot using identity transform */

    pladv(0);
    plvpor(0.1, 0.9, 0.1, 0.9);
    plwind(-1.0, 1.0, -1.0, 1.0);

    for (i = 0; i < NCONTR; i++) {
	shade_min = zmin + (zmax - zmin) * i / (float) NCONTR;
	shade_max = zmin + (zmax - zmin) * (i +1) / (float) NCONTR;
	sh_color = i / (float) (NCONTR-1);
	sh_width = 2;
	plpsty(0);

	plshade(z, XPTS, YPTS, NULL, -1., 1., -1., 1., 
		shade_min, shade_max, 
		sh_cmap, sh_color, sh_width,
		min_color, min_width, max_color, max_width,
		plfill, 1, NULL, NULL);
    }

    plcol(1);
    plbox("bcnst", 0.0, 0, "bcnstv", 0.0, 0);
    plcol(2);
/*
    plcont(w, XPTS, YPTS, 1, XPTS, 1, YPTS, clevel, NCONTR, mypltr, NULL);
    */
    pllab("distance", "altitude", "Bogon density");

/* Plot using 1d coordinate transform */
    
    pladv(0);
    plvpor(0.1, 0.9, 0.1, 0.9);
    plwind(-1.0, 1.0, -1.0, 1.0);

    for (i = 0; i < NCONTR; i++) {
	shade_min = zmin + (zmax - zmin) * i / (float) NCONTR;
	shade_max = zmin + (zmax - zmin) * (i +1) / (float) NCONTR;
	sh_color = i / (float) (NCONTR-1);
	sh_width = 2;
	plpsty(0);

	plshade(z, XPTS, YPTS, NULL, -1., 1., -1., 1., 
		shade_min, shade_max, 
		sh_cmap, sh_color, sh_width,
		min_color, min_width, max_color, max_width,
		plfill, 1, pltr1, (void *) &cgrid1);
    }

    plcol(1);
    plbox("bcnst", 0.0, 0, "bcnstv", 0.0, 0);
    plcol(2);
/*
    plcont(w, XPTS, YPTS, 1, XPTS, 1, YPTS, clevel, NCONTR,
	   pltr1, (void *) &cgrid1);
	   */
    pllab("distance", "altitude", "Bogon density");

/* Plot using 2d coordinate transform */

    pladv(0);
    plvpor(0.1, 0.9, 0.1, 0.9);
    plwind(-1.0, 1.0, -1.0, 1.0);

    for (i = 0; i < NCONTR; i++) {
	shade_min = zmin + (zmax - zmin) * i / (float) NCONTR;
	shade_max = zmin + (zmax - zmin) * (i +1) / (float) NCONTR;
	sh_color = i / (float) (NCONTR-1);
	sh_width = 2;
	plpsty(0);

	plshade(z, XPTS, YPTS, NULL, -1., 1., -1., 1., 
		shade_min, shade_max, 
		sh_cmap, sh_color, sh_width,
		min_color, min_width, max_color, max_width,
		plfill, 0, pltr2, (void *) &cgrid2);
    }

    plcol(1);
    plbox("bcnst", 0.0, 0, "bcnstv", 0.0, 0);
    plcol(2);

    plcont(w, XPTS, YPTS, 1, XPTS, 1, YPTS, clevel, NCONTR,
	   pltr2, (void *) &cgrid2);

    pllab("distance", "altitude", "Bogon density, with streamlines");

/* Clean up */

    plend();
    free((void *) w);
    free((void *) z);
    exit(0);
}

/*----------------------------------------------------------------------*\
* f2mnmx
*
* Returns min & max of input 2d array.
\*----------------------------------------------------------------------*/

void
f2mnmx(PLFLT **f, PLINT nx, PLINT ny, PLFLT *fmin, PLFLT *fmax)
{
    int i, j;

    *fmax = f[0][0];
    *fmin = *fmax;

    for (i = 0; i < nx; i++) {
	for (j = 0; j < ny; j++) {
            *fmax = MAX(*fmax, f[i][j]);
            *fmin = MIN(*fmin, f[i][j]);
	}
    }
}
