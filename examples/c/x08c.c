/* $Id$
 * $Log$
 * Revision 1.7  1994/03/30 07:21:52  mjl
 * Changes to all C example programs: special handling for malloc re: header
 * files eliminated, include of stdio.h and stdlib.h eliminated (now done
 * by plplot.h), include of "plplot.h" changed to <plplot.h> to enable
 * simpler builds by the general user, some cleaning up also.
 *
 * Revision 1.6  1993/07/28  05:47:24  mjl
 * Some minor desuckification.
 *
 * Revision 1.5  1993/02/22  23:16:16  mjl
 * Changed over to new style of initialization using plinit(), and added
 * function to parse plplot command line flags.
 *
 * Revision 1.4  1993/01/23  06:10:28  mjl
 * Instituted exit codes for all example codes.  Also deleted color functions
 * no longer supported (plancol).  Enhanced x09c to exploit new contour
 * capabilities.
*/

/* Demonstration of 3-d plotting */

#include <plplot.h>
#include <math.h>

#define   XPTS   35
#define   YPTS   46

static int opt[] =
{1, 2, 3, 3};

static PLFLT alt[] =
{60.0, 20.0, 60.0, 60.0};

static PLFLT az[] =
{30.0, 60.0, 120.0, 160.0};

static char *title[4] =
{
    "#frPLPLOT Example 8 - Alt=60, Az=30, Opt=1",
    "#frPLPLOT Example 8 - Alt=20, Az=60, Opt=2",
    "#frPLPLOT Example 8 - Alt=60, Az=120, Opt=3",
    "#frPLPLOT Example 8 - Alt=60, Az=160, Opt=3"
};

int
main(int argc, char *argv[])
{
    int i, j, k;
    PLFLT *x, *y, **z;
    PLFLT xx, yy, r;

/* Parse and process command line arguments */

    (void) plParseInternalOpts(&argc, argv, PL_PARSE_FULL);

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
	    r = sqrt(xx * xx + yy * yy);
	    z[i][j] = exp(-r * r) * cos(2.0 * 3.141592654 * r);
	}
    }

    for (k = 0; k < 4; k++) {
	pladv(0);
	plvpor(0.0, 1.0, 0.0, 0.9);
	plwind(-1.0, 1.0, -0.9, 1.1);
	plcol(1);
	plw3d(1.0, 1.0, 1.0, -1.0, 1.0, -1.0, 1.0, -1.0, 1.0, alt[k], az[k]);
	plbox3("bnstu", "x axis", 0.0, 0, "bnstu", "y axis", 0.0, 0,
	       "bcdmnstuv", "z axis", 0.0, 0);
	plcol(2);
	plot3d(x, y, z, XPTS, YPTS, opt[k], 1);
	plcol(3);
	plmtex("t", 1.0, 0.5, 0.5, title[k]);
    }

    plend();
    exit(0);
}
