/* Demonstration program for PLPLOT illustrating absolute positioning */
/* $Id$
   $Log$
   Revision 1.4  1993/02/22 23:16:18  mjl
   Changed over to new style of initialization using plinit(), and added
   function to parse plplot command line flags.

 * Revision 1.3  1993/01/23  06:10:31  mjl
 * Instituted exit codes for all example codes.  Also deleted color functions
 * no longer supported (plancol).  Enhanced x09c to exploit new contour
 * capabilities.
 *
 * Revision 1.2  1992/09/29  04:45:18  furnish
 * Massive clean up effort to remove support for garbage compilers (K&R).
 *
 * Revision 1.1  1992/05/20  21:32:58  furnish
 * Initial checkin of the whole PLPLOT project.
 *
*/

/* of graphs on a page */
/* Note the compiler should automatically convert all non-pointer arguments
   to satisfy the prototype, but some have problems with constants. */

#include "plplot.h"
#include <stdlib.h>

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
