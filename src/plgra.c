/* $Id$
   $Log$
   Revision 1.1.2.1  2001/04/19 12:31:46  rlaboiss
   First merge against MAIN

   Revision 1.1  1992/05/20 21:34:29  furnish
   Initial checkin of the whole PLPLOT project.

*/

/*	plgra.c

	Switch between graphics/text modes.
*/

#include "plplot.h"

/* Switches to graphics mode */

void 
c_plgra()
{
    PLINT level;
    glev(&level);
    if (level < 1)
	plexit("Please call plstar before plgra.");
    grgra();
}

/* Switches back to text mode */

void 
c_pltext()
{
    PLINT level;

    glev(&level);
    if (level < 1)
	plexit("Please call plstar before calling pltext.");

    grtext();
}
