/* $Id$
 * $Log$
 * Revision 1.5  1994/03/30 07:21:54  mjl
 * Changes to all C example programs: special handling for malloc re: header
 * files eliminated, include of stdio.h and stdlib.h eliminated (now done
 * by plplot.h), include of "plplot.h" changed to <plplot.h> to enable
 * simpler builds by the general user, some cleaning up also.
 *
 * Revision 1.4  1993/02/22  23:16:18  mjl
 * Changed over to new style of initialization using plinit(), and added
 * function to parse plplot command line flags.
*/

/* Demonstration program for PLPLOT illustrating absolute positioning */
/* of graphs on a page */

#include <plplot.h>

int
main(int argc, char *argv[])
{

/* Parse and process command line arguments */

    (void) plParseInternalOpts(&argc, argv, PL_PARSE_FULL);

/* Initialize plplot */

    plinit();

    pladv(0);
    plvpor((PLFLT) 0.0, (PLFLT) 1.0, (PLFLT) 0.0, (PLFLT) 1.0);
    plwind((PLFLT) 0.0, (PLFLT) 1.0, (PLFLT) 0.0, (PLFLT) 1.0);
    plbox("bc", (PLFLT) 0.0, 0, "bc", (PLFLT) 0.0, 0);

    plsvpa((PLFLT) 50.0, (PLFLT) 150.0, (PLFLT) 100.0, (PLFLT) 150.0);
    plwind((PLFLT) 0.0, (PLFLT) 1.0, (PLFLT) 0.0, (PLFLT) 1.0);
    plbox("bc", (PLFLT) 0.0, 0, "bc", (PLFLT) 0.0, 0);
    plptex((PLFLT) 0.5, (PLFLT) 0.5, (PLFLT) 1.0, (PLFLT) 0.0, (PLFLT) 0.5,
	   "BOX at (50,150,100,150)");
    plend();
    exit(0);
}
