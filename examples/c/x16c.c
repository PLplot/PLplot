/* $Id$
 * $Log$
 * Revision 1.7  1995/05/06 21:30:55  mjl
 * All data structures now allocated at runtime to allow greater flexibility.
 * Command line options added: -ns <n>, sets number of shaded levels, -nx <n>
 * and -ny <n> set number of data points in x and y, respectively.
 *
 * Revision 1.6  1995/04/12  08:19:01  mjl
 * Changes to all C demos: now include "plcdemos.h" to get all startup
 * definitions and includes that are useful to share between them.
 *
 * Revision 1.5  1995/03/16  23:18:58  mjl
 * All example C programs: changed plParseInternalOpts() call to plParseOpts().
 *
 * Revision 1.4  1994/06/30  17:57:58  mjl
 * All C example programs: made another pass to eliminate warnings when using
 * gcc -Wall.  Lots of cleaning up: got rid of includes of math.h or string.h
 * (now included by plplot.h), eliminated redundant casts, put in more
 * uniform comments, and other minor changes.
 *
 * Revision 1.3  1994/04/25  19:02:55  mjl
 * Increased the number of shade regions to make it a bit more interesting
 * when playing with the palette.
 *
 * Revision 1.2  1994/04/08  12:08:21  mjl
 * Cleaned up some, switched to more interesting test function.
 *
 * Revision 1.1  1994/03/30  07:22:55  mjl
 * Added to test specifically color fill capability of plshade, with optional
 * coordinate mapping.
*/

/*	x16c.c

	plshade demo, using color fill.

	Maurice LeBrun
	IFS, University of Texas at Austin
	20 Mar 1994
*/

#include <plcdemos.h>

/* Fundamental settings.  See notes[] for more info. */

static int ns = 20;		/* Default number of shade levels */
static int nx = 35;		/* Default number of data points in x */
static int ny = 46;		/* Default number of data points in y */

/* Transformation function */

PLFLT tr[6];

static void
mypltr(PLFLT x, PLFLT y, PLFLT *tx, PLFLT *ty, void *pltr_data)
{
    *tx = tr[0] * x + tr[1] * y + tr[2];
    *ty = tr[3] * x + tr[4] * y + tr[5];
}

/* Function prototypes */

static void
f2mnmx(PLFLT **f, PLINT nx, PLINT ny, PLFLT *fmin, PLFLT *fmax);

/* Options data structure definition. */

static PLOptionTable options[] = {
{
    "ns",			/* Number of shade levels */
    NULL,
    NULL,
    &ns,
    PL_OPT_INT,
    "-ns levels",
    "Sets number of shade levels" },
{
    "nx",			/* Number of data points in x */
    NULL,
    NULL,
    &nx,
    PL_OPT_INT,
    "-nx xpts",
    "Sets number of data points in x" },
{
    "ny",			/* Number of data points in y */
    NULL,
    NULL,
    &ny,
    PL_OPT_INT,
    "-ny ypts",
    "Sets number of data points in y" },
};

static char *notes[] = {
"To get smoother color variation, increase ns, nx, and ny.  To get faster",
"response (especially on a serial link), decrease them.  A decent but quick",
"test results from ns around 5 and nx, ny around 25.",
NULL};

/*--------------------------------------------------------------------------*\
 * main
 *
 * Does several shade plots using different coordinate mappings.
\*--------------------------------------------------------------------------*/

int
main(int argc, char *argv[])
{
    int i, j;
    PLFLT x, y, argx, argy, distort;

    PLFLT **z, **w, zmin, zmax;
    PLFLT *clevel, *xg1, *yg1;
    PLcGrid  cgrid1;
    PLcGrid2 cgrid2;

    PLFLT shade_min, shade_max, sh_color;
    PLINT sh_cmap = 1, sh_width;
    PLINT min_color = 1, min_width = 0, max_color = 0, max_width = 0;

/* Parse and process command line arguments */

    plMergeOpts(options, "x16c options", notes);
    plParseOpts(&argc, argv, PL_PARSE_FULL);

/* Reduce colors in cmap 0 so that cmap 1 is useful on a 16-color display */

    plscmap0n(3);

/* Initialize plplot */

    plinit();

/* Set up transformation function */

    tr[0] = 2./(nx-1);
    tr[1] = 0.0;
    tr[2] = -1.0;
    tr[3] = 0.0;
    tr[4] = 2./(ny-1);
    tr[5] = -1.0;

/* Allocate data structures */

    clevel = (PLFLT *) calloc(ns, sizeof(PLFLT));
    xg1 = (PLFLT *) calloc(nx, sizeof(PLFLT));
    yg1 = (PLFLT *) calloc(ny, sizeof(PLFLT));

    plAlloc2dGrid(&z, nx, ny);
    plAlloc2dGrid(&w, nx, ny);

/* Set up data array */

    for (i = 0; i < nx; i++) {
	x = (double) (i - (nx / 2)) / (double) (nx / 2);
	for (j = 0; j < ny; j++) {
	    y = (double) (j - (ny / 2)) / (double) (ny / 2) - 1.0;

	    z[i][j] = - sin(7.*x) * cos(7.*y) + x*x - y*y;
	    w[i][j] = - cos(7.*x) * sin(7.*y) + 2 * x * y;
	}
    }
    f2mnmx(z, nx, ny, &zmin, &zmax);
    for (i = 0; i < ns; i++)
	clevel[i] = zmin + (zmax - zmin) * (i + 0.5) / (float) ns;

/* Set up coordinate grids */

    cgrid1.xg = xg1;
    cgrid1.yg = yg1;
    cgrid1.nx = nx;
    cgrid1.ny = ny;

    plAlloc2dGrid(&cgrid2.xg, nx, ny);
    plAlloc2dGrid(&cgrid2.yg, nx, ny);
    cgrid2.nx = nx;
    cgrid2.ny = ny;

    for (i = 0; i < nx; i++) {
	for (j = 0; j < ny; j++) {
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

    for (i = 0; i < ns; i++) {
	shade_min = zmin + (zmax - zmin) * i / (float) ns;
	shade_max = zmin + (zmax - zmin) * (i +1) / (float) ns;
	sh_color = i / (float) (ns-1);
	sh_width = 2;
	plpsty(0);

	plshade(z, nx, ny, NULL, -1., 1., -1., 1., 
		shade_min, shade_max, 
		sh_cmap, sh_color, sh_width,
		min_color, min_width, max_color, max_width,
		plfill, 1, NULL, NULL);
    }

    plcol(1);
    plbox("bcnst", 0.0, 0, "bcnstv", 0.0, 0);
    plcol(2);
/*
    plcont(w, nx, ny, 1, nx, 1, ny, clevel, ns, mypltr, NULL);
    */
    pllab("distance", "altitude", "Bogon density");

/* Plot using 1d coordinate transform */
    
    pladv(0);
    plvpor(0.1, 0.9, 0.1, 0.9);
    plwind(-1.0, 1.0, -1.0, 1.0);

    for (i = 0; i < ns; i++) {
	shade_min = zmin + (zmax - zmin) * i / (float) ns;
	shade_max = zmin + (zmax - zmin) * (i +1) / (float) ns;
	sh_color = i / (float) (ns-1);
	sh_width = 2;
	plpsty(0);

	plshade(z, nx, ny, NULL, -1., 1., -1., 1., 
		shade_min, shade_max, 
		sh_cmap, sh_color, sh_width,
		min_color, min_width, max_color, max_width,
		plfill, 1, pltr1, (void *) &cgrid1);
    }

    plcol(1);
    plbox("bcnst", 0.0, 0, "bcnstv", 0.0, 0);
    plcol(2);
/*
    plcont(w, nx, ny, 1, nx, 1, ny, clevel, ns, pltr1, (void *) &cgrid1);
    */
    pllab("distance", "altitude", "Bogon density");

/* Plot using 2d coordinate transform */

    pladv(0);
    plvpor(0.1, 0.9, 0.1, 0.9);
    plwind(-1.0, 1.0, -1.0, 1.0);

    for (i = 0; i < ns; i++) {
	shade_min = zmin + (zmax - zmin) * i / (float) ns;
	shade_max = zmin + (zmax - zmin) * (i +1) / (float) ns;
	sh_color = i / (float) (ns-1);
	sh_width = 2;
	plpsty(0);

	plshade(z, nx, ny, NULL, -1., 1., -1., 1., 
		shade_min, shade_max, 
		sh_cmap, sh_color, sh_width,
		min_color, min_width, max_color, max_width,
		plfill, 0, pltr2, (void *) &cgrid2);
    }

    plcol(1);
    plbox("bcnst", 0.0, 0, "bcnstv", 0.0, 0);
    plcol(2);
    plcont(w, nx, ny, 1, nx, 1, ny, clevel, ns, pltr2, (void *) &cgrid2);

    pllab("distance", "altitude", "Bogon density, with streamlines");

/* Clean up */

    plend();

    free((void *) clevel);
    free((void *) xg1);
    free((void *) yg1);
    plFree2dGrid(z, nx, ny);
    plFree2dGrid(w, nx, ny);
    plFree2dGrid(cgrid2.xg, nx, ny);
    plFree2dGrid(cgrid2.yg, nx, ny);

    exit(0);
}

/*--------------------------------------------------------------------------*\
 * f2mnmx
 *
 * Returns min & max of input 2d array.
\*--------------------------------------------------------------------------*/

static void
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
