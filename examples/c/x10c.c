/* $Id$
 * $Log$
 * Revision 1.9  1995/06/01 21:40:10  mjl
 * All C demo files: changed file inclusion to use quotes instead of angle
 * brackets so that dependencies are retained during development.
 *
 * Revision 1.8  1995/04/12  08:18:57  mjl
 * Changes to all C demos: now include "plcdemos.h" to get all startup
 * definitions and includes that are useful to share between them.
 *
 * Revision 1.7  1995/03/16  23:18:54  mjl
 * All example C programs: changed plParseInternalOpts() call to plParseOpts().
 *
 * Revision 1.6  1994/06/30  17:57:43  mjl
 * All C example programs: made another pass to eliminate warnings when using
 * gcc -Wall.  Lots of cleaning up: got rid of includes of math.h or string.h
 * (now included by plplot.h), eliminated redundant casts, put in more
 * uniform comments, and other minor changes.
*/

/*	x10c.c

	Window positioning demo.
*/

#include "plcdemos.h"

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
