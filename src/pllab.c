/* $Id$
   $Log$
   Revision 1.2.2.1  2001/04/19 12:31:46  rlaboiss
   First merge against MAIN

   Revision 1.2  1992/09/29 04:46:03  furnish
   Massive clean up effort to remove support for garbage compilers (K&R).

 * Revision 1.1  1992/05/20  21:34:34  furnish
 * Initial checkin of the whole PLPLOT project.
 *
*/

/*	pllab.c

	Simple routine for labelling graphs.
*/

#include "plplot.h"

void 
c_pllab( char *xlabel, char *ylabel, char *tlabel )
{
    PLINT level;

    glev(&level);
    if (level < 2)
	plexit("Please set up viewport before calling pllab.");

    plmtex("t", (PLFLT) 2.0, (PLFLT) 0.5, (PLFLT) 0.5, tlabel);
    plmtex("b", (PLFLT) 3.2, (PLFLT) 0.5, (PLFLT) 0.5, xlabel);
    plmtex("l", (PLFLT) 5.0, (PLFLT) 0.5, (PLFLT) 0.5, ylabel);
}
