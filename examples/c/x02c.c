/* $Id$
 * $Log$
 * Revision 1.6  1994/03/30 07:21:45  mjl
 * Changes to all C example programs: special handling for malloc re: header
 * files eliminated, include of stdio.h and stdlib.h eliminated (now done
 * by plplot.h), include of "plplot.h" changed to <plplot.h> to enable
 * simpler builds by the general user, some cleaning up also.
 *
 * Revision 1.5  1993/02/22  23:16:10  mjl
 * Changed over to new style of initialization using plinit(), and added
 * function to parse plplot command line flags.
*/

/* Demonstrates multiple windows and default color map 0 palette */

#include <plplot.h>
#include <math.h>

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

    plschr((PLFLT) 0.0, (PLFLT) 3.5);
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
	    plwind((PLFLT) 0.0, (PLFLT) 1.0, (PLFLT) 0.0, (PLFLT) 1.0);
	    plbox("bc", (PLFLT) 0.0, 0, "bc", (PLFLT) 0.0, 0);
	    vmin = vmin + 0.1;
	    vmax = vmax - 0.1;
	}
	plwid(1);
	plptex((PLFLT) 0.5, (PLFLT) 0.5, (PLFLT) 1.0, (PLFLT) 0.0, (PLFLT) 0.5, text);
    }

    plend();
    exit(0);
}
