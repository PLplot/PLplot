/* $Id$
   $Log$
   Revision 1.2  1992/09/29 04:45:40  furnish
   Massive clean up effort to remove support for garbage compilers (K&R).

 * Revision 1.1  1992/05/20  21:34:09  furnish
 * Initial checkin of the whole PLPLOT project.
 *
*/

/*	move.c

	Move and draw routines.
*/

#include "plplot.h"

/* Move to physical coordinates (x,y) */

void 
movphy (PLINT x, PLINT y)
{
    scurr(x, y);
}

/* Draw to physical coordinates (x,y) */

void 
draphy (PLINT x, PLINT y)
{
    PLINT currx, curry;
    gcurr(&currx, &curry);
    pllclp(currx, curry, x, y);
}

/* Move to world coordinates (x,y) */

void movwor (PLFLT x, PLFLT y)
{
    scurr(wcpcx(x), wcpcy(y));
}

/* Draw to world coordinates (x,y) */

void drawor (PLFLT x, PLFLT y)
{
    PLINT currx, curry;
    gcurr(&currx, &curry);
    pllclp(currx, curry, wcpcx(x), wcpcy(y));
}
