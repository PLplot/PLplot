/* $Id$
 * $Log$
 * Revision 1.8  1994/03/30 07:21:46  mjl
 * Changes to all C example programs: special handling for malloc re: header
 * files eliminated, include of stdio.h and stdlib.h eliminated (now done
 * by plplot.h), include of "plplot.h" changed to <plplot.h> to enable
 * simpler builds by the general user, some cleaning up also.
 *
 * Revision 1.7  1993/08/23  19:34:04  mjl
 * Fixed a bug that caused polar axis labels to be messed up.  Only showed
 * up for some compilers & flag combinations.
 *
 * Revision 1.6  1993/07/02  07:06:07  mjl
 * Added definition for ROUND macro.
 *
 * Revision 1.5  1993/02/22  23:16:11  mjl
 * Changed over to new style of initialization using plinit(), and added
 * function to parse plplot command line flags.
*/

/* Illustration of 1-1 scaling for polar plot */

#include <plplot.h>
#include <math.h>

#ifndef ROUND
#define ROUND(a)    (PLINT)((a)<0. ? ((a)-.5) : ((a)+.5))
#endif

int
main(int argc, char *argv[])
{
    int i, j;
    PLFLT dtr, theta, dx, dy, r;
    char text[4];
    static PLFLT x0[361], y0[361];
    static PLFLT x[361], y[361];

    dtr = 3.141592654 / 180.0;
    for (i = 0; i <= 360; i++) {
	x0[i] = cos(dtr * i);
	y0[i] = sin(dtr * i);
    }

/* Parse and process command line arguments */

    (void) plParseInternalOpts(&argc, argv, PL_PARSE_FULL);

/* Initialize plplot */

    plinit();

/* Set up viewport and window, but do not draw box */

    plenv((PLFLT) -1.3, (PLFLT) 1.3, (PLFLT) -1.3, (PLFLT) 1.3, 1, -2);
    for (i = 1; i <= 10; i++) {
	for (j = 0; j <= 360; j++) {
	    x[j] = 0.1 * i * x0[j];
	    y[j] = 0.1 * i * y0[j];
	}

/* Draw circles for polar grid */

	plline(361, x, y);
    }

    plcol(2);
    for (i = 0; i <= 11; i++) {
	theta = 30.0 * i;
	dx = cos(dtr * theta);
	dy = sin(dtr * theta);

/* Draw radial spokes for polar grid */

	pljoin((PLFLT) 0.0, (PLFLT) 0.0, dx, dy);
	sprintf(text, "%d", ROUND(theta));

/* Write labels for angle */

	if (dx >= 0)
	    plptex(dx, dy, dx, dy, -0.15, text);
	else {
	    plptex(dx, dy, -dx, -dy, 1.15, text);
	}
    }

/* Draw the graph */

    for (i = 0; i <= 360; i++) {
	r = sin(dtr * (5 * i));
	x[i] = x0[i] * r;
	y[i] = y0[i] * r;
    }
    plcol(3);
    plline(361, x, y);

    plcol(4);
    plmtex("t", (PLFLT) 2.0, (PLFLT) 0.5, (PLFLT) 0.5,
	   "#frPLPLOT Example 3 - r(#gh)=sin 5#gh");

/* Close the plot at end */

    plend();
    exit(0);
}
