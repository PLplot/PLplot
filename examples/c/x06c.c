/* $Id$
 * $Log$
 * Revision 1.6  1994/03/30 07:21:49  mjl
 * Changes to all C example programs: special handling for malloc re: header
 * files eliminated, include of stdio.h and stdlib.h eliminated (now done
 * by plplot.h), include of "plplot.h" changed to <plplot.h> to enable
 * simpler builds by the general user, some cleaning up also.
 *
 * Revision 1.5  1993/02/22  23:16:14  mjl
 * Changed over to new style of initialization using plinit(), and added
 * function to parse plplot command line flags.
 *
 * Revision 1.4  1993/01/23  06:10:26  mjl
 * Instituted exit codes for all example codes.  Also deleted color functions
 * no longer supported (plancol).  Enhanced x09c to exploit new contour
 * capabilities.
*/

/* Displays the plotter symbols for PLPOIN */

#include <plplot.h>

int
main(int argc, char *argv[])
{
    char text[3];
    int i, j, k;
    PLFLT x, y;

/* Parse and process command line arguments */

    (void) plParseInternalOpts(&argc, argv, PL_PARSE_FULL);

/* Initialize plplot */

    plinit();

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
