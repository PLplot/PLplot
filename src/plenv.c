/* $Id$
   $Log$
   Revision 1.2  1992/09/29 04:45:53  furnish
   Massive clean up effort to remove support for garbage compilers (K&R).

 * Revision 1.1  1992/05/20  21:34:22  furnish
 * Initial checkin of the whole PLPLOT project.
 *
*/

/*	plenv.c

	Define viewport and window.
*/

#include "plplot.h"
#include <stdio.h>
#include <math.h>

/*----------------------------------------------------------------------*\
* void plenv()
*
* Simple interface for defining viewport and window. If "just"=1,
* X and Y scales will be the same, otherwise they are scaled
* independently. The "axis" parameter is interpreted as follows:
*
*	axis=-2 : draw no box, axis or labels
*	axis=-1 : draw box only
*	axis= 0 : Draw box and label with coordinates
*	axis= 1 : Also draw the coordinate axes
*	axis= 2 : Draw a grid at major tick positions
*	axis=10 : Logarithmic X axis, Linear Y axis, No X=0 axis
*	axis=11 : Logarithmic X axis, Linear Y axis, X=0 axis
*	axis=20 : Linear X axis, Logarithmic Y axis, No Y=0 axis
*	axis=21 : Linear X axis, Logarithmic Y axis, Y=0 axis
*	axis=30 : Logarithmic X and Y axes
\*----------------------------------------------------------------------*/

void
c_plenv (PLFLT xmin, PLFLT xmax, PLFLT ymin, PLFLT ymax,
	 PLINT just, PLINT axis)
{
    PLINT level;
    PLFLT chrdef, chrht;
    PLFLT lb, rb, tb, bb, dx, dy;
    PLFLT xsize, ysize, xscale, yscale, scale;
    PLFLT spxmin, spxmax, spymin, spymax;
    PLFLT vpxmin, vpxmax, vpymin, vpymax;

    glev(&level);
    if (level < 1)
	plexit("Please call plstar before plenv.");

    if (xmin == xmax)
	plexit("Invalid xmin and xmax arguments in plenv");
    if (ymin == ymax)
	plexit("Invalid ymin and ymax arguments in plenv");
    if ((just != 0) && (just != 1))
	plexit("Invalid just option in plenv");


    pladv(0);
    if (just == 0)
	plvsta();
    else {
	gchr(&chrdef, &chrht);
	lb = 8.0 * chrht;
	rb = 5.0 * chrht;
	tb = 5.0 * chrht;
	bb = 5.0 * chrht;
	dx = ABS(xmax - xmin);
	dy = ABS(ymax - ymin);
	plgspa(&spxmin, &spxmax, &spymin, &spymax);
	xsize = spxmax - spxmin;
	ysize = spymax - spymin;
	xscale = dx / (xsize - lb - rb);
	yscale = dy / (ysize - tb - bb);
	scale = MAX(xscale, yscale);
	vpxmin = MAX(lb, 0.5 * (xsize - dx / scale));
	vpxmax = vpxmin + (dx / scale);
	vpymin = MAX(bb, 0.5 * (ysize - dy / scale));
	vpymax = vpymin + (dy / scale);

	plsvpa(vpxmin, vpxmax, vpymin, vpymax);
    }
    plwind(xmin, xmax, ymin, ymax);
    if (axis == -2);
    else if (axis == -1)
	plbox("bc", (PLFLT) 0.0, 0, "bc", (PLFLT) 0.0, 0);
    else if (axis == 0)
	plbox("bcnst", (PLFLT) 0.0, 0, "bcnstv", (PLFLT) 0.0, 0);
    else if (axis == 1)
	plbox("abcnst", (PLFLT) 0.0, 0, "abcnstv", (PLFLT) 0.0, 0);
    else if (axis == 2)
	plbox("abcgnst", (PLFLT) 0.0, 0, "abcgnstv", (PLFLT) 0.0, 0);
    else if (axis == 10)
	plbox("bclnst", (PLFLT) 0.0, 0, "bcnstv", (PLFLT) 0.0, 0);
    else if (axis == 11)
	plbox("bclnst", (PLFLT) 0.0, 0, "abcnstv", (PLFLT) 0.0, 0);
    else if (axis == 20)
	plbox("bcnst", (PLFLT) 0.0, 0, "bclnstv", (PLFLT) 0.0, 0);
    else if (axis == 21)
	plbox("bcnst", (PLFLT) 0.0, 0, "abclnstv", (PLFLT) 0.0, 0);
    else if (axis == 30)
	plbox("bclnst", (PLFLT) 0.0, 0, "bclnstv", (PLFLT) 0.0, 0);
    else
	fprintf(stderr, "Invalid axis argument in plenv.\n");
}
