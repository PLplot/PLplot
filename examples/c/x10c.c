/* Demonstration program for PLPLOT illustrating absolute positioning */
/* $Id$
   $Log$
   Revision 1.1  1992/05/20 21:32:58  furnish
   Initial checkin of the whole PLPLOT project.

*/

/* of graphs on a page */
/* Note the compiler should automatically convert all non-pointer arguments
   to satisfy the prototype, but some have problems with constants. */

#include "plplot.h"

main()
{
    plstar(1, 1);

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
}
