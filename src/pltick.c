/* $Id$
   $Log$
   Revision 1.1  1992/05/20 21:34:53  furnish
   Initial checkin of the whole PLPLOT project.

*/

/*	pltick.c

	Draw tick marks.
*/

#include "plplot.h"

/*----------------------------------------------------------------------*\
* void plxtik()
*
* Draws a tick parallel to x.
\*----------------------------------------------------------------------*/

void 
plxtik(x, y, below, above)
PLINT x, y, below, above;
{
    draphy(x, y);
    if (below != 0)
	draphy(x, y - below);
    if (above != 0)
	draphy(x, y + above);
    draphy(x, y);
}

/*----------------------------------------------------------------------*\
* void plytik()
*
* Draws a tick parallel to y.
\*----------------------------------------------------------------------*/

void 
plytik(x, y, left, right)
PLINT x, y, left, right;
{
    draphy(x, y);
    if (left != 0)
	draphy(x - left, y);
    if (right != 0)
	draphy(x + right, y);
    draphy(x, y);
}

/*----------------------------------------------------------------------*\
* void plstik()
*
* Draws a slanting tick at position (mx,my) (measured in mm) of
* vector length (dx,dy).
\*----------------------------------------------------------------------*/

#ifdef PLSTDC
void plstik (PLFLT mx, PLFLT my, PLFLT dx, PLFLT dy)
#else
void 
plstik(mx, my, dx, dy)
PLFLT mx, my, dx, dy;
#endif
{
    draphy(mmpcx(mx), mmpcy(my));
    draphy(mmpcx((PLFLT) (mx + dx)), mmpcy((PLFLT) (my + dy)));
    draphy(mmpcx(mx), mmpcy(my));
}
