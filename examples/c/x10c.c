/* $Id$

	Window positioning demo.
*/

#include "plplot/plcdemos.h"

/*--------------------------------------------------------------------------*\
 * main
 *
 * Demonstrates absolute positioning of graphs on a page.
\*--------------------------------------------------------------------------*/

int
main(int argc, char *argv[])
{

/* Parse and process command line arguments */

    (void) plParseOpts(&argc, argv, PL_PARSE_FULL);

/* Initialize plplot */

    plinit();

    pladv(0);
    plvpor(0.0, 1.0, 0.0, 1.0);
    plwind(0.0, 1.0, 0.0, 1.0);
    plbox("bc", 0.0, 0, "bc", 0.0, 0);

    plsvpa(50.0, 150.0, 100.0, 150.0);
    plwind(0.0, 1.0, 0.0, 1.0);
    plbox("bc", 0.0, 0, "bc", 0.0, 0);
    plptex(0.5, 0.5, 1.0, 0.0, 0.5, "BOX at (50,150,100,150)");
    plend();
    exit(0);
}
