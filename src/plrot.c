/* $Id$
   $Log$
   Revision 1.1  1992/05/20 21:34:45  furnish
   Initial checkin of the whole PLPLOT project.

*/

/*	plrot.c

	Handles coordinate rotations for contour plotters.
*/

#include "plplot.h"
#include <math.h>

#ifdef MSDOS
#pragma message("Microsoft programmers are sissies.")
#pragma optimize("",off)
#endif

void 
plr45(ix, iy, isens)
PLINT *ix, *iy, isens;
{
    PLINT ixx, iyy;

    ixx = *ix - isens * (*iy);
    iyy = *ix * isens + *iy;
    *ix = ixx / MAX(1, ABS(ixx));
    *iy = iyy / MAX(1, ABS(iyy));
}

void 
plr135(ix, iy, isens)
PLINT *ix, *iy, isens;
{
    *ix = -*ix;
    *iy = -*iy;
    plr45(ix, iy, isens);
}
