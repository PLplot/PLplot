/* $Id$
   $Log$
   Revision 1.1  1992/05/20 21:34:56  furnish
   Initial checkin of the whole PLPLOT project.

*/

/*	plvpor.c

	Set up viewport.
*/

#include "plplot.h"

/*----------------------------------------------------------------------*\
* void plvpor()
*
* Sets the edges of the viewport to the specified normalized subpage 
* coordinates.
\*----------------------------------------------------------------------*/

#ifdef PLSTDC
void
c_plvpor (PLFLT xmin, PLFLT xmax, PLFLT ymin, PLFLT ymax)
#else
void 
c_plvpor(xmin, xmax, ymin, ymax)
PLFLT xmin, xmax, ymin, ymax;
#endif
{
    PLFLT spdxmi, spdxma, spdymi, spdyma;
    PLFLT vpdxmi, vpdxma, vpdymi, vpdyma;
    PLINT vppxmi, vppxma, vppymi, vppyma;
    PLINT nx, ny, cs;
    PLINT level;

    glev(&level);
    if (level < 1)
	plexit("Please call plstar before calling plvpor.");
    if ((xmin >= xmax) || (ymin >= ymax) || (xmin < 0.) ||
	(ymin < 0.) || (xmax > 1.) || (ymax > 1.))
	plexit("Invalid limits in plvpor.");

    gsub(&nx, &ny, &cs);
    if ((cs <= 0) || (cs > (nx * ny)))
	plexit("Please call pladv or plenv to go to a subpage.");

    gspd(&spdxmi, &spdxma, &spdymi, &spdyma);
    vpdxmi = spdxmi + (spdxma - spdxmi) * xmin;
    vpdxma = spdxmi + (spdxma - spdxmi) * xmax;
    vpdymi = spdymi + (spdyma - spdymi) * ymin;
    vpdyma = spdymi + (spdyma - spdymi) * ymax;
    svpd(vpdxmi, vpdxma, vpdymi, vpdyma);

    vppxmi = dcpcx(vpdxmi);
    vppxma = dcpcx(vpdxma);
    vppymi = dcpcy(vpdymi);
    vppyma = dcpcy(vpdyma);
    svpp(vppxmi, vppxma, vppymi, vppyma);
    sclp(vppxmi, vppxma, vppymi, vppyma);
    slev(2);
}
