/* $Id$
   $Log$
   Revision 1.2  1992/09/29 04:46:24  furnish
   Massive clean up effort to remove support for garbage compilers (K&R).

 * Revision 1.1  1992/05/20  21:34:54  furnish
 * Initial checkin of the whole PLPLOT project.
 *
*/

/*	plvasp.c

	Define viewport with given aspect ratio.
*/

#include "plplot.h"
#include <stdio.h>
#include <math.h>

/*----------------------------------------------------------------------*\
* void plvasp()
*
* Sets the edges of the viewport with the given aspect ratio, leaving
* room for labels.
\*----------------------------------------------------------------------*/

void
c_plvasp (PLFLT aspect)
{
    PLINT level;
    PLFLT chrdef, chrht, spxmin, spxmax, spymin, spymax;
    PLFLT vpxmin, vpxmax, vpymin, vpymax;
    PLFLT xsize, ysize, nxsize, nysize;
    PLFLT lb, rb, tb, bb;

    glev(&level);
    if (level < 1)
	plexit("Please call plstar before calling plvasp.");

    gchr(&chrdef, &chrht);
    lb = 8.0 * chrht;
    rb = 5.0 * chrht;
    tb = 5.0 * chrht;
    bb = 5.0 * chrht;
    plgspa(&spxmin, &spxmax, &spymin, &spymax);
    xsize = spxmax - spxmin;
    ysize = spymax - spymin;
    xsize -= lb + rb;		/* adjust for labels */
    ysize -= bb + tb;
    if (aspect * xsize > ysize) {
	nxsize = ysize / aspect;
	nysize = ysize;
    }
    else {
	nxsize = xsize;
	nysize = xsize * aspect;
    }

/* center plot within page */

    vpxmin = .5 * (xsize - nxsize) + lb;
    vpxmax = vpxmin + nxsize;
    vpymin = .5 * (ysize - nysize) + bb;
    vpymax = vpymin + nysize;
    plsvpa(vpxmin, vpxmax, vpymin, vpymax);
}
