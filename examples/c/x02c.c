/* $Id$

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
	plcol0(i);
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
