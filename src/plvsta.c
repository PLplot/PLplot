/* $Id$
   $Log$
   Revision 1.1.2.1  2001/04/19 12:31:46  rlaboiss
   First merge against MAIN

   Revision 1.1  1992/05/20 21:34:58  furnish
   Initial checkin of the whole PLPLOT project.

*/

/*	plvsta.c

	Defines standard viewport.
*/

#include "plplot.h"

/*----------------------------------------------------------------------*\
* void plvsta()
*
* Defines a "standard" viewport with seven character heights for   
* the left margin and four character heights everywhere else.
\*----------------------------------------------------------------------*/

void 
c_plvsta()
{
    PLFLT xmin, xmax, ymin, ymax;
    PLFLT chrdef, chrht, spdxmi, spdxma, spdymi, spdyma;
    PLINT level;

    glev(&level);
    if (level < 1)
	plexit("Please call plstar before calling plvsta.");

    gchr(&chrdef, &chrht);
    gspd(&spdxmi, &spdxma, &spdymi, &spdyma);

/*  Find out position of subpage boundaries in millimetres, reduce by */
/*  the desired border, and convert back into normalized subpage */
/*  coordinates */

    xmin = dcscx(mmdcx((PLFLT) (dcmmx(spdxmi) + 8 * chrht)));
    xmax = dcscx(mmdcx((PLFLT) (dcmmx(spdxma) - 5 * chrht)));
    ymin = dcscy(mmdcy((PLFLT) (dcmmy(spdymi) + 5 * chrht)));
    ymax = dcscy(mmdcy((PLFLT) (dcmmy(spdyma) - 5 * chrht)));

    plvpor(xmin, xmax, ymin, ymax);
}
