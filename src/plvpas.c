/* $Id$
   $Log$
   Revision 1.1  1992/05/20 21:34:55  furnish
   Initial checkin of the whole PLPLOT project.

*/

/*	plvpas.c

	Set up viewport within given bounds and at given aspect ratio.
*/

#include "plplot.h"

/*----------------------------------------------------------------------*\
* void plvpas()
*
* Sets the edges of the viewport to that of the largest window at the
* given aspect ratio that fits within the specified normalized subpage 
* coordinates.
\*----------------------------------------------------------------------*/

#ifdef PLSTDC
void
c_plvpas (PLFLT xmin, PLFLT xmax, PLFLT ymin, PLFLT ymax, PLFLT aspect)
#else
void 
c_plvpas (xmin, xmax, ymin, ymax, aspect)
PLFLT xmin, xmax, ymin, ymax, aspect;
#endif
{
    PLFLT vpxmi, vpxma, vpymi, vpyma;
    PLINT level;
    PLFLT vpxmid, vpymid, vpxlen, vpylen, w_aspect, ratio;

    glev(&level);
    if (level < 1)
	plexit("Please call plstar before calling plvpas.");

    if ((xmin >= xmax) || (ymin >= ymax) || (xmin < 0.) ||
	(ymin < 0.) || (xmax > 1.) || (ymax > 1.))
	plexit("Invalid limits in plvpas.");

    if (aspect <= 0.0) {
	c_plvpor(xmin, xmax, ymin, ymax);
	return;
    }

    vpxmi = dcmmx(xmin);
    vpxma = dcmmx(xmax);
    vpymi = dcmmy(ymin);
    vpyma = dcmmy(ymax);

    vpxmid = (vpxmi + vpxma)/2.;
    vpymid = (vpymi + vpyma)/2.;

    vpxlen = vpxma - vpxmi;
    vpylen = vpyma - vpymi;

    w_aspect = vpylen / vpxlen;
    ratio = aspect / w_aspect;

/*
* If ratio < 1, you are requesting an aspect ratio (y/x) less than the natural
* aspect ratio of the specified window, and you will need to reduce the length
* in y correspondingly.  Similarly, for ratio > 1, x length must be reduced. 
*/

    if (ratio <= 0.)
	plexit("Error in aspect ratio setting");
    else if (ratio < 1.)
	vpylen = vpylen * ratio;
    else
	vpxlen = vpxlen / ratio;

    vpxmi = vpxmid - vpxlen / 2.;
    vpxma = vpxmid + vpxlen / 2.;
    vpymi = vpymid - vpylen / 2.;
    vpyma = vpymid + vpylen / 2.;

    plsvpa(vpxmi, vpxma, vpymi, vpyma);
}

