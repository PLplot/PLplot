/* $Id$
   $Log$
   Revision 1.2  1992/09/29 04:46:02  furnish
   Massive clean up effort to remove support for garbage compilers (K&R).

 * Revision 1.1  1992/05/20  21:34:33  furnish
 * Initial checkin of the whole PLPLOT project.
 *
*/

/*	pljoin.c

	Draws a line segment from (x1, y1) to (x2, y2).
*/

#include "plplot.h"

void
c_pljoin (PLFLT x1, PLFLT y1, PLFLT x2, PLFLT y2)
{
    movwor(x1, y1);
    drawor(x2, y2);
}
