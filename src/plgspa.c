/* $Id$
   $Log$
   Revision 1.1  1992/05/20 21:34:31  furnish
   Initial checkin of the whole PLPLOT project.

*/

/*	plgspa.c

	Gets subpage boundaries in absolute coordinates (mm from bottom
	left-hand corner of page).
*/

#include "plplot.h"

void 
c_plgspa(xmin, xmax, ymin, ymax)
PLFLT *xmin, *xmax, *ymin, *ymax;
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
