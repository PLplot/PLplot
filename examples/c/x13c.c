/* $Id$
 * $Log$
 * Revision 1.6  1994/03/30 07:21:57  mjl
 * Changes to all C example programs: special handling for malloc re: header
 * files eliminated, include of stdio.h and stdlib.h eliminated (now done
 * by plplot.h), include of "plplot.h" changed to <plplot.h> to enable
 * simpler builds by the general user, some cleaning up also.
 *
 * Revision 1.5  1993/02/22  23:16:21  mjl
 * Changed over to new style of initialization using plinit(), and added
 * function to parse plplot command line flags.
*/

/* Demonstration program for PLPLOT: */

#include <plplot.h>
#include <math.h>

#ifndef PI
#define PI      3.141592654
#endif

static char *text[] =
{
    "Maurice",
    "Randy",
    "Mark",
    "Steve",
    "Warner"
};

int
main(int argc, char *argv[])
{
    int i, j;
    PLFLT dthet, theta0, theta1, theta, just, dx, dy;
    static PLFLT x[500], y[500], per[5];

    per[0] = 10.;
    per[1] = 32.;
    per[2] = 12.;
    per[3] = 30.;
    per[4] = 16.;

/* Parse and process command line arguments */

    (void) plParseInternalOpts(&argc, argv, PL_PARSE_FULL);

/* Initialize plplot */

    plinit();

    plenv((PLFLT) 0., (PLFLT) 10., (PLFLT) 0., (PLFLT) 10., 1, -2);
    plcol(2);

    theta0 = 0.;
    dthet = 2 * PI / 500;
    for (i = 0; i <= 4; i++) {
	j = 0;
	x[j] = 5.;
	y[j++] = 5.;
	theta1 = theta0 + 2 * PI * per[i] / 100.;
	if (i == 4)
	    theta1 = 2 * PI;
	for (theta = theta0; theta <= theta1; theta += dthet) {
	    x[j] = 5 + 3 * cos(theta);
	    y[j++] = 5 + 3 * sin(theta);
	}
	plcol(i + 1);
	plpsty((i + 3) % 8 + 1);
	plfill(j, x, y);
	plcol(1);
	plline(j, x, y);
	just = (theta0 + theta1) / 2.;
	dx = .25 * cos(just);
	dy = .25 * sin(just);
	if (just < PI / 2 || just > 3 * PI / 2) {
	    just = 0.;
	}
	else {
	    just = 1.;
	}
	plptex((x[j / 2] + dx), (y[j / 2] + dy), (PLFLT) 1., (PLFLT) 0.,
	       just, text[i]);
	theta0 = theta - dthet;
    }
    plfont(2);
    plschr((PLFLT) 0., (PLFLT) 1.3);
    plptex((PLFLT) 5., (PLFLT) 9., (PLFLT) 1., (PLFLT) 0., (PLFLT) .5,
	   "Percentage of Sales");

/* Don't forget to call PLEND to finish off! */

    plend();
    exit(0);
}
