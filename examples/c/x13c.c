/* $Id$
 * $Log$
 * Revision 1.11  2000/12/18 21:01:49  airwin
 * Change to new style plplot/*.h header file locations.
 *
 * Revision 1.10  1995/06/01 21:40:12  mjl
 * All C demo files: changed file inclusion to use quotes instead of angle
 * brackets so that dependencies are retained during development.
 *
 * Revision 1.9  1995/04/12  08:18:59  mjl
 * Changes to all C demos: now include "plcdemos.h" to get all startup
 * definitions and includes that are useful to share between them.
 *
 * Revision 1.8  1995/03/16  23:18:56  mjl
 * All example C programs: changed plParseInternalOpts() call to plParseOpts().
 *
 * Revision 1.7  1994/06/30  17:57:51  mjl
 * All C example programs: made another pass to eliminate warnings when using
 * gcc -Wall.  Lots of cleaning up: got rid of includes of math.h or string.h
 * (now included by plplot.h), eliminated redundant casts, put in more
 * uniform comments, and other minor changes.
*/

/*	x13c.c

	Pie chart demo.
*/

#include "plplot/plcdemos.h"

static char *text[] =
{
    "Maurice",
    "Randy",
    "Mark",
    "Steve",
    "Warner"
};

/*--------------------------------------------------------------------------*\
 * main
 *
 * Does a simple pie chart.
\*--------------------------------------------------------------------------*/

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

    (void) plParseOpts(&argc, argv, PL_PARSE_FULL);

/* Initialize plplot */

    plinit();

    plenv(0., 10., 0., 10., 1, -2);
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
	if (just < PI / 2 || just > 3 * PI / 2) 
	    just = 0.;
	else 
	    just = 1.;

	plptex((x[j / 2] + dx), (y[j / 2] + dy), 1.0, 0.0, just, text[i]);
	theta0 = theta - dthet;
    }
    plfont(2);
    plschr(0., 1.3);
    plptex(5.0, 9.0, 1.0, 0.0, 0.5, "Percentage of Sales");

/* Don't forget to call PLEND to finish off! */

    plend();
    exit(0);
}
