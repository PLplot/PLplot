/* Displays the plotter symbols for PLSYM */
/* $Id$
   $Log$
   Revision 1.5  1993/02/22 23:16:15  mjl
   Changed over to new style of initialization using plinit(), and added
   function to parse plplot command line flags.

 * Revision 1.4  1993/01/23  06:10:27  mjl
 * Instituted exit codes for all example codes.  Also deleted color functions
 * no longer supported (plancol).  Enhanced x09c to exploit new contour
 * capabilities.
 *
 * Revision 1.3  1992/09/30  18:25:19  furnish
 * Massive cleanup to irradicate garbage code.  Almost everything is now
 * prototyped correctly.  Builds on HPUX, SUNOS (gcc), AIX, and UNICOS.
 *
 * Revision 1.2  1992/09/29  04:45:15  furnish
 * Massive clean up effort to remove support for garbage compilers (K&R).
 *
 * Revision 1.1  1992/05/20  21:32:55  furnish
 * Initial checkin of the whole PLPLOT project.
 *
*/

/* Note the compiler should automatically convert all non-pointer arguments
   to satisfy the prototype, but some have problems with constants. */

#ifdef MSDOS
#pragma message("Microsoft programmers are sissies.")
#pragma optimize("",off)
#endif

#include "plplot.h"
#include <stdio.h>
#include <stdlib.h>

static int base[17] =
{0, 200, 500, 600, 700, 800, 900,
 2000, 2100, 2200, 2300, 2400, 2500, 2600, 2700, 2800, 2900};

int
main(int argc, char *argv[])
{
    char text[4];
    int i, j, k, l;
    PLFLT x, y;

/* Parse and process command line arguments */

    (void) plParseInternalOpts(&argc, argv, PL_PARSE_FULL);

/* Initialize plplot */

    plinit();

    plfontld(1);
    for (l = 0; l < 17; l++) {
	pladv(0);

/* Set up viewport and window */

	plcol(2);
	plvpor((PLFLT) 0.1, (PLFLT) 1.0, (PLFLT) 0.1, (PLFLT) 0.9);
	plwind((PLFLT) 0.0, (PLFLT) 1.0, (PLFLT) 0.0, (PLFLT) 1.0);

/* Draw the grid using plbox */

	plbox("bcgt", (PLFLT) 0.1, 0, "bcgt", (PLFLT) 0.1, 0);

/* Write the digits below the frame */

	plcol(15);
	for (i = 0; i <= 9; i++) {
	    sprintf(text, "%d", i);
	    plmtex("b", (PLFLT) 1.5, (0.1 * i + 0.05), (PLFLT) 0.5, text);
	}

	k = 0;
	for (i = 0; i <= 9; i++) {

/* Write the digits to the left of the frame */

	    sprintf(text, "%d", base[l] + 10 * i);
	    plmtex("lv", (PLFLT) 1.0, (0.95 - 0.1 * i), (PLFLT) 1.0, text);
	    for (j = 0; j <= 9; j++) {
		x = 0.1 * j + 0.05;
		y = 0.95 - 0.1 * i;

/* Display the symbols */

		plsym(1, &x, &y, base[l] + k);
		k = k + 1;
	    }
	}

	plmtex("t", (PLFLT) 1.5, (PLFLT) 0.5, (PLFLT) 0.5,
	       "PLPLOT Example 7 - PLSYM symbols");
    }
    plend();
    exit(0);
}
