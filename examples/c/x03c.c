/* $Id$
 * $Log$
 * Revision 1.10  1995/03/16 23:18:50  mjl
 * All example C programs: changed plParseInternalOpts() call to plParseOpts().
 *
 * Revision 1.9  1994/06/30  17:57:13  mjl
 * All C example programs: made another pass to eliminate warnings when using
 * gcc -Wall.  Lots of cleaning up: got rid of includes of math.h or string.h
 * (now included by plplot.h), eliminated redundant casts, put in more
 * uniform comments, and other minor changes.
 *
*/

/*	x03c.c

	Polar plot demo.
*/

#include <plplot.h>

#ifndef ROUND
#define ROUND(a)    (PLINT)((a)<0. ? ((a)-.5) : ((a)+.5))
#endif

/*--------------------------------------------------------------------------*\
 * main
 *
 * Generates polar plot, with 1-1 scaling.
\*--------------------------------------------------------------------------*/

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

    (void) plParseOpts(&argc, argv, PL_PARSE_FULL);

/* Initialize plplot */

    plinit();

/* Set up viewport and window, but do not draw box */

    plenv(-1.3, 1.3, -1.3, 1.3, 1, -2);
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

	pljoin(0.0, 0.0, dx, dy);
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
    plmtex("t", 2.0, 0.5, 0.5, "#frPLplot Example 3 - r(#gh)=sin 5#gh");

/* Close the plot at end */

    plend();
    exit(0);
}
