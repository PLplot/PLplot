/* $Id$
   $Log$
   Revision 1.2  1992/09/29 04:46:23  furnish
   Massive clean up effort to remove support for garbage compilers (K&R).

 * Revision 1.1  1992/05/20  21:34:53  furnish
 * Initial checkin of the whole PLPLOT project.
 *
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
plxtik (PLINT x, PLINT y, PLINT below, PLINT above)
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
plytik (PLINT x, PLINT y, PLINT left, PLINT right)
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

void plstik (PLFLT mx, PLFLT my, PLFLT dx, PLFLT dy)
{
    draphy(mmpcx(mx), mmpcy(my));
    draphy(mmpcx((PLFLT) (mx + dx)), mmpcy((PLFLT) (my + dy)));
    draphy(mmpcx(mx), mmpcy(my));
}
