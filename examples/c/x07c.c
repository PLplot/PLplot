/* $Id$
 * $Log$
 * Revision 1.13  1996/11/18 19:02:18  furnish
 * Make buffer larger to prevent use of unallocated memory.
 *
 * Revision 1.12  1995/06/01  21:40:08  mjl
 * All C demo files: changed file inclusion to use quotes instead of angle
 * brackets so that dependencies are retained during development.
 *
 * Revision 1.11  1995/04/12  08:18:55  mjl
 * Changes to all C demos: now include "plcdemos.h" to get all startup
 * definitions and includes that are useful to share between them.
 *
 * Revision 1.10  1995/03/16  23:18:52  mjl
 * All example C programs: changed plParseInternalOpts() call to plParseOpts().
 *
 * Revision 1.9  1994/06/30  17:57:34  mjl
 * All C example programs: made another pass to eliminate warnings when using
 * gcc -Wall.  Lots of cleaning up: got rid of includes of math.h or string.h
 * (now included by plplot.h), eliminated redundant casts, put in more
 * uniform comments, and other minor changes.
*/

/*	x07c.c

	Font demo.
*/

#ifdef MSDOS
#pragma optimize("",off)
#endif

#include "plcdemos.h"

static int base[17] =
{0, 200, 500, 600, 700, 800, 900,
 2000, 2100, 2200, 2300, 2400, 2500, 2600, 2700, 2800, 2900};

/*--------------------------------------------------------------------------*\
 * main
 *
 * Displays the entire "plsym" symbol (font) set.
\*--------------------------------------------------------------------------*/

int
main(int argc, char *argv[])
{
    char text[10];
    int i, j, k, l;
    PLFLT x, y;

/* Parse and process command line arguments */

    (void) plParseOpts(&argc, argv, PL_PARSE_FULL);

/* Initialize plplot */

    plinit();

    plfontld(1);
    for (l = 0; l < 17; l++) {
	pladv(0);

    /* Set up viewport and window */

	plcol(2);
	plvpor(0.15, 0.95, 0.1, 0.9);
	plwind(0.0, 1.0, 0.0, 1.0);

    /* Draw the grid using plbox */

	plbox("bcgt", 0.1, 0, "bcgt", 0.1, 0);

    /* Write the digits below the frame */

	plcol(15);
	for (i = 0; i <= 9; i++) {
	    sprintf(text, "%d", i);
	    plmtex("b", 1.5, (0.1 * i + 0.05), 0.5, text);
	}

	k = 0;
	for (i = 0; i <= 9; i++) {

	/* Write the digits to the left of the frame */

	    sprintf(text, "%d", base[l] + 10 * i);
	    plmtex("lv", 1.0, (0.95 - 0.1 * i), 1.0, text);
	    for (j = 0; j <= 9; j++) {
		x = 0.1 * j + 0.05;
		y = 0.95 - 0.1 * i;

	    /* Display the symbols */

		plsym(1, &x, &y, base[l] + k);
		k = k + 1;
	    }
	}

	plmtex("t", 1.5, 0.5, 0.5, "PLplot Example 7 - PLSYM symbols");
    }
    plend();
    exit(0);
}
