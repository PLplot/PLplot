/* $Id$
   $Log$
   Revision 1.2  1992/09/29 04:46:29  furnish
   Massive clean up effort to remove support for garbage compilers (K&R).

 * Revision 1.1  1992/05/20  21:35:01  furnish
 * Initial checkin of the whole PLPLOT project.
 *
*/

/*	plwind.c

	Sets up world coordinates of the viewport boundaries.
*/

#include "plplot.h"

void
c_plwind (PLFLT xmin, PLFLT xmax, PLFLT ymin, PLFLT ymax)
{
    PLINT vppxmi, vppxma, vppymi, vppyma;
    PLFLT dx, dy;
    PLFLT vpwxmi, vpwxma, vpwymi, vpwyma;
    PLFLT vpxmi, vpxma, vpymi, vpyma;
    PLFLT wpxscl, wpxoff, wpyscl, wpyoff;
    PLFLT wmxscl, wmxoff, wmyscl, wmyoff;
    PLINT level;

    glev(&level);
    if (level < 2)
	plexit("Please set up viewport before calling plwind.");

    gvpp(&vppxmi, &vppxma, &vppymi, &vppyma);
    gvpd(&vpxmi, &vpxma, &vpymi, &vpyma);

    dx = (xmax - xmin) * 1.0e-5;
    dy = (ymax - ymin) * 1.0e-5;

    if ((xmin == xmax) || (ymin == ymax))
	plexit("Invalid window limits in plwind.");

/* The true plot window is made slightly larger than requested so that */
/* the end limits will be on the graph  */

    svpw((PLFLT) (xmin - dx), (PLFLT) (xmax + dx), (PLFLT) (ymin - dy), (PLFLT) (ymax + dy));
    gvpw(&vpwxmi, &vpwxma, &vpwymi, &vpwyma);

/* Compute the scaling between coordinate systems */

    dx = vpwxma - vpwxmi;
    dy = vpwyma - vpwymi;

    wpxscl = (vppxma - vppxmi) / dx;
    wpxoff = (xmax * vppxmi - xmin * vppxma) / dx;
    wpyscl = (vppyma - vppymi) / dy;
    wpyoff = (ymax * vppymi - ymin * vppyma) / dy;
    swp(wpxscl, wpxoff, wpyscl, wpyoff);

    vpxmi = dcmmx(vpxmi);
    vpxma = dcmmx(vpxma);
    vpymi = dcmmy(vpymi);
    vpyma = dcmmy(vpyma);
    wmxscl = (vpxma - vpxmi) / dx;
    wmxoff = (xmax * vpxmi - xmin * vpxma) / dx;
    wmyscl = (vpyma - vpymi) / dy;
    wmyoff = (ymax * vpymi - ymin * vpyma) / dy;
    swm(wmxscl, wmxoff, wmyscl, wmyoff);

    slev(3);
}
