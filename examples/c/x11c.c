/* Demonstration of mesh plotting (just like x08c but mesh) */
/* $Id$
   $Log$
   Revision 1.7  1993/12/08 20:25:58  mjl
   Eliminated final pltext() call (unnecessary).

 * Revision 1.6  1993/02/22  23:16:19  mjl
 * Changed over to new style of initialization using plinit(), and added
 * function to parse plplot command line flags.
*/

/* Note the compiler should automatically convert all non-pointer arguments
   to satisfy the prototype, but some have problems with constants. */

#include "plplot.h"
#include <stdlib.h>
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
    "#frPLPLOT Example 11 - Alt=60, Az=30, Opt=1",
    "#frPLPLOT Example 11 - Alt=20, Az=60, Opt=2",
    "#frPLPLOT Example 11 - Alt=60, Az=120, Opt=3",
    "#frPLPLOT Example 11 - Alt=60, Az=160, Opt=3"
};

int
main(int argc, char *argv[])
{
    int i, j, k;
    PLFLT *x, *y, **z;
    PLFLT xx, yy;

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
	    z[i][j] = cos(2.0 * 3.141592654 * xx) * sin(2.0 * 3.141592654 * yy);
	}
    }

    for (k = 0; k < 4; k++) {
	pladv(0);
	plcol(1);
	plvpor((PLFLT) 0.0, (PLFLT) 1.0, (PLFLT) 0.0, (PLFLT) 0.8);
	plwind((PLFLT) -1.0, (PLFLT) 1.0, (PLFLT) -1.0, (PLFLT) 1.5);

	plw3d((PLFLT) 1.0, (PLFLT) 1.0, (PLFLT) 1.2, (PLFLT) -1.0, (PLFLT) 1.0,
	      (PLFLT) -1.0, (PLFLT) 1.0, (PLFLT) -1.5, (PLFLT) 1.5,
	      alt[k], az[k]);
	plbox3("bnstu", "x axis", (PLFLT) 0.0, 0, "bnstu",
	       "y axis", (PLFLT) 0.0, 0, "bcdmnstuv",
	       "z axis", (PLFLT) 0.0, 4);
	plcol(2);
	plmesh(x, y, z, XPTS, YPTS, opt[k]);
	plcol(3);
	plmtex("t", (PLFLT) 1.0, (PLFLT) 0.5, (PLFLT) 0.5, title[k]);
    }

    plend();
    exit(0);
}
