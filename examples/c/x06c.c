/* Displays the plotter symbols for PLPOIN */
/* $Id$
   $Log$
   Revision 1.4  1993/01/23 06:10:26  mjl
   Instituted exit codes for all example codes.  Also deleted color functions
   no longer supported (plancol).  Enhanced x09c to exploit new contour
   capabilities.

 * Revision 1.3  1992/09/30  18:25:18  furnish
 * Massive cleanup to irradicate garbage code.  Almost everything is now
 * prototyped correctly.  Builds on HPUX, SUNOS (gcc), AIX, and UNICOS.
 *
 * Revision 1.2  1992/09/29  04:45:14  furnish
 * Massive clean up effort to remove support for garbage compilers (K&R).
 *
 * Revision 1.1  1992/05/20  21:32:54  furnish
 * Initial checkin of the whole PLPLOT project.
 *
*/

/* Note the compiler should automatically convert all non-pointer arguments
   to satisfy the prototype, but some have problems with constants. */

#include "plplot.h"
#include <stdio.h>
#include <stdlib.h>

int
main(void)
{
    char text[3];
    int i, j, k;
    PLFLT x, y;

    /* Full sized page for display */

    plstar(1, 1);
    pladv(0);

    /* Set up viewport and window */

    plcol(2);
    plvpor((PLFLT) 0.1, (PLFLT) 1.0, (PLFLT) 0.1, (PLFLT) 0.9);
    plwind((PLFLT) 0.0, (PLFLT) 1.0, (PLFLT) 0.0, (PLFLT) 1.3);

    /* Draw the grid using plbox */

    plbox("bcgt", (PLFLT) 0.1, 0, "bcgt", (PLFLT) 0.1, 0);

    /* Write the digits below the frame */

    plcol(15);
    for (i = 0; i <= 9; i++) {
	sprintf(text, "%d", i);
	plmtex("b", (PLFLT) 1.5, (0.1 * i + 0.05), (PLFLT) 0.5, text);
    }

    k = 0;
    for (i = 0; i <= 12; i++) {

	/* Write the digits to the left of the frame */

	sprintf(text, "%d", 10 * i);
	plmtex("lv", (PLFLT) 1.0, (1.0 - (2 * i + 1) / 26.0), (PLFLT) 1.0, text);
	for (j = 0; j <= 9; j++) {
	    x = 0.1 * j + 0.05;
	    y = 1.25 - 0.1 * i;

	    /* Display the symbols (plpoin expects that x and y are arrays so */
	    /* pass pointers) */

	    if (k < 128)
		plpoin(1, &x, &y, k);
	    k = k + 1;
	}
    }

    plmtex("t", (PLFLT) 1.5, (PLFLT) 0.5, (PLFLT) 0.5,
	   "PLPLOT Example 6 - PLPOIN symbols");
    plend();
    exit(0);
}
