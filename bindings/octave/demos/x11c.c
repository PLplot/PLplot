/* $Id$
 * $Log$
 * Revision 1.1  2001/03/02 23:47:59  jcard
 * Initial import of Octave bindings
 *
 * Revision 1.1.1.1  2000/04/10 17:23:07  qual
 * import  module
 *
 * Revision 1.12  1995/06/01  21:40:11  mjl
 * All C demo files: changed file inclusion to use quotes instead of angle
 * brackets so that dependencies are retained during development.
 *
 * Revision 1.11  1995/04/12  08:18:58  mjl
 * Changes to all C demos: now include "plcdemos.h" to get all startup
 * definitions and includes that are useful to share between them.
 *
 * Revision 1.10  1995/03/16  23:18:55  mjl
 * All example C programs: changed plParseInternalOpts() call to plParseOpts().
 *
 * Revision 1.9  1994/06/30  17:57:46  mjl
 * All C example programs: made another pass to eliminate warnings when using
 * gcc -Wall.  Lots of cleaning up: got rid of includes of math.h or string.h
 * (now included by plplot.h), eliminated redundant casts, put in more
 * uniform comments, and other minor changes.
 *
 * Revision 1.8  1994/03/30  07:21:55  mjl
 * Changes to all C example programs: special handling for malloc re: header
 * files eliminated, include of stdio.h and stdlib.h eliminated (now done
 * by plplot.h), include of "plplot.h" changed to <plplot.h> to enable
 * simpler builds by the general user, some cleaning up also.
*/

/*	x11c.c

	Mesh plot demo.
*/

#include "plcdemos.h"

#define XPTS    35		/* Data points in x */
#define YPTS    46		/* Datat points in y */

static int opt[] = {1, 2, 3, 3};
static PLFLT alt[] = {60.0, 20.0, 60.0, 60.0};
static PLFLT az[] = {30.0, 60.0, 120.0, 160.0};

static char *title[4] =
{
    "#frPLplot Example 11 - Alt=60, Az=30, Opt=1",
    "#frPLplot Example 11 - Alt=20, Az=60, Opt=2",
    "#frPLplot Example 11 - Alt=60, Az=120, Opt=3",
    "#frPLplot Example 11 - Alt=60, Az=160, Opt=3"
};

/*--------------------------------------------------------------------------*\
 * main
 *
 * Does a series of mesh plots for a given data set, with different
 * viewing options in each plot.
\*--------------------------------------------------------------------------*/

int
main(int argc, char *argv[])
{
    int i, j, k;
    PLFLT *x, *y, **z;
    PLFLT xx, yy;

/* Parse and process command line arguments */

    (void) plParseOpts(&argc, argv, PL_PARSE_FULL);

/* Initialize plplot */

    plinit();

    x = (PLFLT *) malloc(XPTS * sizeof(PLFLT));
    y = (PLFLT *) malloc(YPTS * sizeof(PLFLT));
    z = (PLFLT **) malloc(XPTS * sizeof(PLFLT *));
    for (i = 0; i < XPTS; i++) {
	z[i] = (PLFLT *) malloc(YPTS * sizeof(PLFLT));
	x[i] = (double) (i - (XPTS / 2)) / (double) (XPTS / 2);
    }

    for (i = 0; i < YPTS; i++)
	y[i] = (double) (i - (YPTS / 2)) / (double) (YPTS / 2);

    for (i = 0; i < XPTS; i++) {
	xx = x[i];
	for (j = 0; j < YPTS; j++) {
	    yy = y[j];
	    z[i][j] = cos(2.0 * PI * xx) * sin(2.0 * PI * yy);
	}
    }

    for (k = 0; k < 4; k++) {
	pladv(0);
	plcol(1);
	plvpor(0.0, 1.0, 0.0, 0.8);
	plwind(-1.0, 1.0, -1.0, 1.5);

	plw3d(1.0, 1.0, 1.2, -1.0, 1.0, -1.0, 1.0, -1.5, 1.5, alt[k], az[k]);
	plbox3("bnstu", "x axis", 0.0, 0,
	       "bnstu", "y axis", 0.0, 0,
	       "bcdmnstuv", "z axis", 0.0, 4);

	plcol(2);
	plmesh(x, y, z, XPTS, YPTS, opt[k]);
	plcol(3);
	plmtex("t", 1.0, 0.5, 0.5, title[k]);
    }

    plend();
    exit(0);
}
