/* Demonstration program for PLPLOT: */
/* $Id$
   $Log$
   Revision 1.3  1992/09/30 18:25:25  furnish
   Massive cleanup to irradicate garbage code.  Almost everything is now
   prototyped correctly.  Builds on HPUX, SUNOS (gcc), AIX, and UNICOS.

 * Revision 1.2  1992/09/29  04:45:21  furnish
 * Massive clean up effort to remove support for garbage compilers (K&R).
 *
 * Revision 1.1  1992/05/20  21:33:02  furnish
 * Initial checkin of the whole PLPLOT project.
 *
*/

/* Note the compiler should automatically convert all non-pointer arguments
   to satisfy the prototype, but some have problems with constants. */

#include "plplot.h"
#include <stdio.h>
#include <math.h>

#ifndef PI
#define PI      3.141592654
#endif

static char *text[] = {
    "Maurice",
    "Randy",
    "Mark",
    "Steve",
    "Warner"
};

int 
main (void)
{
    int i, j;
    PLFLT dthet, theta0, theta1, theta, just, dx, dy;
    static PLFLT x[500], y[500], per[5];

    per[0] = 10.;
    per[1] = 32.;
    per[2] = 12.;
    per[3] = 30.;
    per[4] = 16.;

/* Ask user to specify the output device */
/* Note that for this demo, neither the global aspect ratio flag nore
   the global orientation flag give the desired results, since plenv is told
   to do scaling based on physical dimensions.
   Thus we MUST print to a physical device and not globally mess with
   orientation or aspect ratio (this may be improved in the future). 
*/

    plstar(1, 1);

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
}
