/* $Id$
   $Log$
   Revision 1.1  1992/05/20 21:36:02  furnish
   Initial checkin of the whole PLPLOT project.

*/

/*	xform.c

	Example linear transformation function for contour plotter.
*/

#include "plplot.h"

extern PLFLT tr[];

#ifdef PLSTDC
void 
xform(PLFLT x, PLFLT y, PLFLT * tx, PLFLT * ty)
#else
void 
xform(x, y, tx, ty)
PLFLT x, y, *tx, *ty;
#endif
{
    *tx = tr[0] * x + tr[1] * y + tr[2];
    *ty = tr[3] * x + tr[4] * y + tr[5];
}
