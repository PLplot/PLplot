/* $Id$
   $Log$
   Revision 1.2.2.1  2001/04/19 12:31:46  rlaboiss
   First merge against MAIN

   Revision 1.2  1992/09/29 04:46:20  furnish
   Massive clean up effort to remove support for garbage compilers (K&R).

 * Revision 1.1  1992/05/20  21:34:50  furnish
 * Initial checkin of the whole PLPLOT project.
 *
*/

/*	plsvpa.c

	Sets absolute viewport coordinates.
*/

#include "plplot.h"

/*----------------------------------------------------------------------*\
* void plsvpa()
*
* Sets the edges of the viewport to the specified absolute
* coordinates (mm), measured with respect to the current subpage
* boundaries.
\*----------------------------------------------------------------------*/

void
c_plsvpa (PLFLT xmin, PLFLT xmax, PLFLT ymin, PLFLT ymax)
{
    PLINT nx, ny, cs;
    PLFLT sxmin, symin;
    PLFLT spdxmi, spdxma, spdymi, spdyma;
    PLFLT vpdxmi, vpdxma, vpdymi, vpdyma;
    PLINT level;

    glev(&level);
    if (level < 1)
	plexit("Please call PLSTAR before calling plsvpa.");
    if ((xmin >= xmax) || (ymin >= ymax) || (xmin < 0.) || (ymin < 0.))
	plexit("Invalid limits in plsvpa.");

    gsub(&nx, &ny, &cs);
    if ((cs <= 0) || (cs > (nx * ny)))
	plexit("Please call pladv or plenv to go to a subpage.");

    gspd(&spdxmi, &spdxma, &spdymi, &spdyma);
    sxmin = dcmmx(spdxmi);
    symin = dcmmy(spdymi);

    vpdxmi = mmdcx((PLFLT) (sxmin + xmin));
    vpdxma = mmdcx((PLFLT) (sxmin + xmax));
    vpdymi = mmdcy((PLFLT) (symin + ymin));
    vpdyma = mmdcy((PLFLT) (symin + ymax));

    svpd(vpdxmi, vpdxma, vpdymi, vpdyma);
    svpp(dcpcx(vpdxmi), dcpcx(vpdxma), dcpcy(vpdymi), dcpcy(vpdyma));
    sclp(dcpcx(vpdxmi), dcpcx(vpdxma), dcpcy(vpdymi), dcpcy(vpdyma));
    slev(2);
}
