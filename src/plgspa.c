/* $Id$
   $Log$
   Revision 1.2.2.1  2001/04/19 12:31:46  rlaboiss
   First merge against MAIN

   Revision 1.2  1992/09/29 04:46:00  furnish
   Massive clean up effort to remove support for garbage compilers (K&R).

 * Revision 1.1  1992/05/20  21:34:31  furnish
 * Initial checkin of the whole PLPLOT project.
 *
*/

/*	plgspa.c

	Gets subpage boundaries in absolute coordinates (mm from bottom
	left-hand corner of page).
*/

#include "plplot.h"

void 
c_plgspa( PLFLT *xmin, PLFLT *xmax, PLFLT *ymin, PLFLT *ymax  )
{
    PLFLT spdxmi, spdxma, spdymi, spdyma;
    PLINT level;

    glev(&level);
    if (level < 1)
	plexit("Please call plstar before plgspa.");
    gspd(&spdxmi, &spdxma, &spdymi, &spdyma);
    *xmin = dcmmx(spdxmi);
    *xmax = dcmmx(spdxma);
    *ymin = dcmmy(spdymi);
    *ymax = dcmmy(spdyma);
}
