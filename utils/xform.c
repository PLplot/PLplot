/* $Id$
   $Log$
   Revision 1.2  1992/09/29 04:46:49  furnish
   Massive clean up effort to remove support for garbage compilers (K&R).

 * Revision 1.1  1992/05/20  21:36:02  furnish
 * Initial checkin of the whole PLPLOT project.
 *
*/

/*	xform.c

	Example linear transformation function for contour plotter.
*/

#include "plplot.h"

extern PLFLT tr[];

void 
xform(PLFLT x, PLFLT y, PLFLT * tx, PLFLT * ty)
{
    *tx = tr[0] * x + tr[1] * y + tr[2];
    *ty = tr[3] * x + tr[4] * y + tr[5];
}
