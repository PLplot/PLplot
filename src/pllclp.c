/* $Id$
   $Log$
   Revision 1.2  1992/09/29 04:46:04  furnish
   Massive clean up effort to remove support for garbage compilers (K&R).

 * Revision 1.1  1992/05/20  21:34:35  furnish
 * Initial checkin of the whole PLPLOT project.
 *
*/

/*	pllclp.c

	Draws a line from (x1,y1) to (x2,y2) within the clip limits.
*/

#include "plplot.h"

#define INSIDE(ix,iy) (BETW(ix,clpxmi,clpxma) && BETW(iy,clpymi,clpyma))

void 
pllclp (PLINT x1, PLINT y1, PLINT x2, PLINT y2)
{
    PLINT clpxmi, clpxma, clpymi, clpyma;
    PLINT t, dx, dy, flipx, flipy;
    PLFLT slope;

    scurr(x2, y2);
    gclp(&clpxmi, &clpxma, &clpymi, &clpyma);

    if (INSIDE(x1, y1) && INSIDE(x2, y2)) {
	genlin(x1, y1, x2, y2);
	return;
    }

    if ((x1 <= clpxmi && x2 <= clpxmi) || (x1 >= clpxma && x2 >= clpxma))
	return;
    if ((y1 <= clpymi && y2 <= clpymi) || (y1 >= clpyma && y2 >= clpyma))
	return;

    flipx = 0;
    flipy = 0;

    if (x2 < x1) {
	x1 = 2 * clpxmi - x1;
	x2 = 2 * clpxmi - x2;
	clpxma = 2 * clpxmi - clpxma;
	t = clpxma;
	clpxma = clpxmi;
	clpxmi = t;
	flipx = 1;
    }

    if (y2 < y1) {
	y1 = 2 * clpymi - y1;
	y2 = 2 * clpymi - y2;
	clpyma = 2 * clpymi - clpyma;
	t = clpyma;
	clpyma = clpymi;
	clpymi = t;
	flipy = 1;
    }

    dx = x2 - x1;
    dy = y2 - y1;

    if (dx != 0)
	slope = (double) dy / (double) dx;

    if (x1 < clpxmi) {
	if (dx != 0 && dy != 0)
	    y1 = y1 + ROUND(slope * (clpxmi - x1));
	x1 = clpxmi;
    }

    if (y1 < clpymi) {
	if (dx != 0 && dy != 0)
	    x1 = x1 + ROUND((clpymi - y1) / slope);
	y1 = clpymi;
    }

    if (x1 >= clpxma || y1 >= clpyma)
	return;

    if (y2 > clpyma) {
	if (dx != 0 && dy != 0)
	    x2 = x2 - ROUND((y2 - clpyma) / slope);
	y2 = clpyma;
    }

    if (x2 > clpxma) {
	if (dx != 0 && dy != 0)
	    y2 = y2 - ROUND((x2 - clpxma) * slope);
	x2 = clpxma;
    }

    if (flipx) {
	x1 = 2 * clpxma - x1;
	x2 = 2 * clpxma - x2;
	clpxmi = 2 * clpxma - clpxmi;
	t = clpxma;
	clpxma = clpxmi;
	clpxmi = t;
    }

    if (flipy) {
	y1 = 2 * clpyma - y1;
	y2 = 2 * clpyma - y2;
	clpymi = 2 * clpyma - clpymi;
	t = clpyma;
	clpyma = clpymi;
	clpymi = t;
    }

    genlin(x1, y1, x2, y2);
}
