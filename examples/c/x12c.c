/* Demonstration program for PLPLOT: Bar chart example. */
/* $Id$
   $Log$
   Revision 1.3  1992/09/30 18:25:24  furnish
   Massive cleanup to irradicate garbage code.  Almost everything is now
   prototyped correctly.  Builds on HPUX, SUNOS (gcc), AIX, and UNICOS.

 * Revision 1.2  1992/09/29  04:45:20  furnish
 * Massive clean up effort to remove support for garbage compilers (K&R).
 *
 * Revision 1.1  1992/05/20  21:33:01  furnish
 * Initial checkin of the whole PLPLOT project.
 *
*/

/* Note the compiler should automatically convert all non-pointer arguments
   to satisfy the prototype, but some have problems with constants. */

#include "plplot.h"
#include <stdio.h>

void 
plfbox(PLFLT x0, PLFLT y0)
{
    PLFLT x[4], y[4];

    x[0] = x0;
    y[0] = 0.;
    x[1] = x0;
    y[1] = y0;
    x[2] = x0 + 1.;
    y[2] = y0;
    x[3] = x0 + 1.;
    y[3] = 0.;
    plfill(4, x, y);
    plcol(1);
    pllsty(1);
    plline(4, x, y);
}

int 
main (void)
{
    int i;
    char string[20];
    PLFLT y0[10];

    /* Ask user to specify the output device */
    plstar(1, 1);

    pladv(0);
    plvsta();
    plwind((PLFLT) 1980., (PLFLT) 1990., (PLFLT) 0., (PLFLT) 35.);
    plbox("bc", (PLFLT) 1., 0, "bcnv", (PLFLT) 10., 0);
    plcol(2);
    pllab("Year", "Widget Sales (millions)", "#frPLPLOT Example 12");

    y0[0] = 5;
    y0[1] = 15;
    y0[2] = 12;
    y0[3] = 24;
    y0[4] = 28;
    y0[5] = 30;
    y0[6] = 20;
    y0[7] = 8;
    y0[8] = 12;
    y0[9] = 3;

    for (i = 0; i < 10; i++) {
	plcol(i % 4 + 1);
	plpsty((i + 3) % 8 + 1);
	pllsty(i % 8 + 1);
	plfbox((1980. + i), y0[i]);
	sprintf(string, "%.0f", y0[i]);
	plptex((1980. + i + .5), (y0[i] + 1.), 1., 0.,
	       .5, string);
	sprintf(string, "%d", 1980 + i);
	plmtex("b", 1., ((i + 1) * .1 - .05), .5, string);
    }

    /* Don't forget to call PLEND to finish off! */
    plend();
}
