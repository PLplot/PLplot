/* $Id$
 * $Log$
 * Revision 1.7  1994/06/30 17:57:12  mjl
 * All C example programs: made another pass to eliminate warnings when using
 * gcc -Wall.  Lots of cleaning up: got rid of includes of math.h or string.h
 * (now included by plplot.h), eliminated redundant casts, put in more
 * uniform comments, and other minor changes.
 *
 * Revision 1.6  1994/03/30  07:21:45  mjl
 * Changes to all C example programs: special handling for malloc re: header
 * files eliminated, include of stdio.h and stdlib.h eliminated (now done
 * by plplot.h), include of "plplot.h" changed to <plplot.h> to enable
 * simpler builds by the general user, some cleaning up also.
*/

/*	x02c.c

	Multiple window and color map 0 demo.
*/

#include <plplot.h>

/*----------------------------------------------------------------------*\
 * main
 *
 * Demonstrates multiple windows and default color map 0 palette.
\*----------------------------------------------------------------------*/

int
main(int argc, char *argv[])
{
    int i, j;
    PLFLT vmin, vmax;
    char text[3];

/* Divide screen into 16 regions */

    plssub(4, 4);

/* Parse and process command line arguments */

    (void) plParseInternalOpts(&argc, argv, PL_PARSE_FULL);

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
