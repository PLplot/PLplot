/* Illustration of 1-1 scaling for polar plot */
/* $Id$
   $Log$
   Revision 1.3  1992/09/30 18:25:15  furnish
   Massive cleanup to irradicate garbage code.  Almost everything is now
   prototyped correctly.  Builds on HPUX, SUNOS (gcc), AIX, and UNICOS.

 * Revision 1.2  1992/09/29  04:45:11  furnish
 * Massive clean up effort to remove support for garbage compilers (K&R).
 *
 * Revision 1.1  1992/05/20  21:32:51  furnish
 * Initial checkin of the whole PLPLOT project.
 *
*/

/* Note the compiler should automatically convert all non-pointer arguments
   to satisfy the prototype, but some have problems with constants. */

#include "plplot.h"
#include <stdio.h>
#include <math.h>

int 
main (void)
{
    int i, j;
    PLFLT dtr, theta, dx, dy, r;
    char text[3];
    static PLFLT x0[361], y0[361];
    static PLFLT x[361], y[361];

    dtr = 3.141592654 / 180.0;
    for (i = 0; i <= 360; i++) {
	x0[i] = cos(dtr * i);
	y0[i] = sin(dtr * i);
    }

/* Ask user to specify the output device */
/* Note that for this demo, neither the global aspect ratio flag nore
   the global orientation flag give the desired results, since plenv is told
   to do scaling based on physical dimensions.
   Thus we MUST print to a physical device and not globally mess with
   orientation or aspect ratio (this may be improved in the future). 
*/
    plstar(1, 1);

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
	    plptex(dx, dy, dx, dy, (PLFLT) -0.15, text);
	else
	    plptex(dx, dy, -dx, -dy, (PLFLT) 1.15, text);
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
}
