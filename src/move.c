/* $Id$
   $Log$
   Revision 1.1  1992/05/20 21:34:09  furnish
   Initial checkin of the whole PLPLOT project.

*/

/*	move.c

	Move and draw routines.
*/

#include "plplot.h"

/* Move to physical coordinates (x,y) */

void 
movphy(x, y)
PLINT x, y;
{
    scurr(x, y);
}

/* Draw to physical coordinates (x,y) */

void 
draphy(x, y)
PLINT x, y;
{
    PLINT currx, curry;
    gcurr(&currx, &curry);
    pllclp(currx, curry, x, y);
}

/* Move to world coordinates (x,y) */

#ifdef PLSTDC
void movwor (PLFLT x, PLFLT y)
#else
void 
movwor(x, y)
PLFLT x, y;
#endif
{
    scurr(wcpcx(x), wcpcy(y));
}

/* Draw to world coordinates (x,y) */

#ifdef PLSTDC
void drawor (PLFLT x, PLFLT y)
#else
void 
drawor(x, y)
PLFLT x, y;
#endif
{
    PLINT currx, curry;
    gcurr(&currx, &curry);
    pllclp(currx, curry, wcpcx(x), wcpcy(y));
}
