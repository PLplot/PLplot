/* $Id$
 * $Log$
 * Revision 1.10  1995/06/01 21:40:05  mjl
 * All C demo files: changed file inclusion to use quotes instead of angle
 * brackets so that dependencies are retained during development.
 *
 * Revision 1.9  1995/04/12  08:18:52  mjl
 * Changes to all C demos: now include "plcdemos.h" to get all startup
 * definitions and includes that are useful to share between them.
 *
 * Revision 1.8  1995/03/16  23:18:49  mjl
 * All example C programs: changed plParseInternalOpts() call to plParseOpts().
 *
 * Revision 1.7  1994/06/30  17:57:12  mjl
 * All C example programs: made another pass to eliminate warnings when using
 * gcc -Wall.  Lots of cleaning up: got rid of includes of math.h or string.h
 * (now included by plplot.h), eliminated redundant casts, put in more
 * uniform comments, and other minor changes.
*/

/*	x02c.c

	Multiple window and color map 0 demo.
*/

#include "plcdemos.h"

/*--------------------------------------------------------------------------*\
 * main
 *
 * Demonstrates multiple windows and default color map 0 palette.
\*--------------------------------------------------------------------------*/

int
main(int argc, char *argv[])
{
    int i, j;
    PLFLT vmin, vmax;
    char text[3];

/* Divide screen into 16 regions */

    plssub(4, 4);

/* Parse and process command line arguments */

    (void) plParseOpts(&argc, argv, PL_PARSE_FULL);

/* Initialize plplot */

    plinit();

    plschr(0.0, 3.5);
    plfont(4);

    for (i = 0; i <= 15; i++) {
	plcol(i);
	sprintf(text, "%d", i);
	pladv(0);
	vmin = 0.1;
	vmax = 0.9;
	for (j = 0; j <= 2; j++) {
	    plwid(j + 1);
	    plvpor(vmin, vmax, vmin, vmax);
	    plwind(0.0, 1.0, 0.0, 1.0);
	    plbox("bc", 0.0, 0, "bc", 0.0, 0);
	    vmin = vmin + 0.1;
	    vmax = vmax - 0.1;
	}
	plwid(1);
	plptex(0.5, 0.5, 1.0, 0.0, 0.5, text);
    }

    plend();
    exit(0);
}
