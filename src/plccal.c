/* $Id$
   $Log$
   Revision 1.2.2.1  2001/04/19 12:31:46  rlaboiss
   First merge against MAIN

   Revision 1.2  1992/09/29 04:45:47  furnish
   Massive clean up effort to remove support for garbage compilers (K&R).

 * Revision 1.1  1992/05/20  21:34:16  furnish
 * Initial checkin of the whole PLPLOT project.
 *
*/

/*	plccal.c

	Interpolate along contour.
*/

#include "plplot.h"

/*----------------------------------------------------------------------*\
* void plccal()
*
* Subroutine to interpolate the position of a contour which is known
* to be next to ix,iy in the direction ixg,iyg. The unscaled distance
* along ixg,iyg is returned as dist.
\*----------------------------------------------------------------------*/

void plccal (PLFLT ** pts, PLINT nx, PLINT ny, PLFLT zlev, 
	     PLINT ix, PLINT iy, PLINT ixg, PLINT iyg, PLFLT * dist)
{
    PLINT ia, ib;
    PLFLT dbot, dtop, zmid;
    PLFLT zxy, zab, zay, zxb, zlow;

    ia = ix + ixg;
    ib = iy + iyg;
    zxy = pts[ix - 1][iy - 1];
    zab = pts[ia - 1][ib - 1];
    zxb = pts[ix - 1][ib - 1];
    zay = pts[ia - 1][iy - 1];

    if (ixg == 0 || iyg == 0) {
	dtop = zlev - zxy;
	dbot = zab - zxy;
	*dist = 0.0;
	if (dbot != 0.0)
	    *dist = dtop / dbot;
    }
    else {
	zmid = (zxy + zab + zxb + zay) / 4.0;
	*dist = 0.5;

	if ((zxy - zlev) * (zab - zlev) <= 0.) {

	    if (zmid >= zlev) {
		dtop = zlev - zxy;
		dbot = zmid - zxy;
		if (dbot != 0.0)
		    *dist = 0.5 * dtop / dbot;
	    }
	    else {
		dtop = zlev - zab;
		dbot = zmid - zab;
		if (dbot != 0.0)
		    *dist = 1.0 - 0.5 * dtop / dbot;
	    }
	}
	else {
	    zlow = (zxb + zay) / 2.0;
	    dtop = zab - zlev;
	    dbot = zab + zxy - 2.0 * zlow;
	    if (dbot != 0.0)
		*dist = 1. - dtop / dbot;
	}
    }
    if (*dist > 1.)
	*dist = 1.;
}
