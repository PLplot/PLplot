/* $Id$
   $Log$
   Revision 1.1  1992/05/20 21:34:36  furnish
   Initial checkin of the whole PLPLOT project.

*/

/*	plline.c

	Draws line segments connecting a series of points.
*/

#include "plplot.h"

void 
c_plline(n, x, y)
PLINT n;
PLFLT *x, *y;
{
    PLINT i, level;

    glev(&level);
    if (level < 3)
	plexit("Please set up window before calling plline.");
    movwor(x[0], y[0]);
    for (i = 1; i < n; i++)
	drawor(x[i], y[i]);
}
