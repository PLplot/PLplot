/* $Id$
   $Log$
   Revision 1.1  1992/05/20 21:34:33  furnish
   Initial checkin of the whole PLPLOT project.

*/

/*	pljoin.c

	Draws a line segment from (x1, y1) to (x2, y2).
*/

#include "plplot.h"

#ifdef PLSTDC
void
c_pljoin (PLFLT x1, PLFLT y1, PLFLT x2, PLFLT y2)
#else
void 
c_pljoin(x1, y1, x2, y2)
PLFLT x1, x2, y1, y2;
#endif
{
    movwor(x1, y1);
    drawor(x2, y2);
}
