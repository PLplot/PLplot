/* $Id$
   $Log$
   Revision 1.1  1992/05/20 21:34:30  furnish
   Initial checkin of the whole PLPLOT project.

*/

/*	plgrid3a.c

	Routine to draw a grid around the back side of the 3d plot
	with hidden line removal.
*/

#include "plplot.h"
#include <math.h>

#ifdef PLSTDC
void
plgrid3a(PLFLT tick)
#else
void
plgrid3a(tick)
PLFLT tick;
#endif
{
    PLFLT xmin, ymin, zmin, xmax, ymax, zmax, zscale;
    PLFLT cxx, cxy, cyx, cyy, cyz;
    PLINT u[3], v[3];
    PLINT nsub, prec, mode, digmax, digits, scale;
    PLFLT tp, dummy;

    gw3wc(&cxx, &cxy, &cyx, &cyy, &cyz);
    gdom(&xmin, &xmax, &ymin, &ymax);
    grange(&zscale, &zmin, &zmax);

    plgzax(&digmax, &digits);
    nsub = 0;
    dummy = tick;	/* Get around compiler bugs (Sun, A/IX). */

    pldtik(zmin, zmax, &dummy, &nsub, &mode, &prec, digmax, &scale);
    tick = dummy;
    tp = tick * floor(zmin / tick) + tick;
    spl3upv((PLINT) 0);

    if (cxx >= 0.0 && cxy <= 0.0) {
	while (tp <= zmax) {
	    u[0] = wcpcx(w3wcx(xmin, ymax, tp));
	    v[0] = wcpcy(w3wcy(xmin, ymax, tp));
	    u[1] = wcpcx(w3wcx(xmax, ymax, tp));
	    v[1] = wcpcy(w3wcy(xmax, ymax, tp));
	    u[2] = wcpcx(w3wcx(xmax, ymin, tp));
	    v[2] = wcpcy(w3wcy(xmax, ymin, tp));
	    plnxtv(u, v, 3, 0);

	    tp += tick;
	}
	u[0] = wcpcx(w3wcx(xmax, ymax, zmin));
	v[0] = wcpcy(w3wcy(xmax, ymax, zmin));
	u[1] = wcpcx(w3wcx(xmax, ymax, zmax));
	v[1] = wcpcy(w3wcy(xmax, ymax, zmax));
	plnxtv(u, v, 2, 0);
    }
    else if (cxx <= 0.0 && cxy <= 0.0) {
	while (tp <= zmax) {
	    u[0] = wcpcx(w3wcx(xmax, ymax, tp));
	    v[0] = wcpcy(w3wcy(xmax, ymax, tp));
	    u[1] = wcpcx(w3wcx(xmax, ymin, tp));
	    v[1] = wcpcy(w3wcy(xmax, ymin, tp));
	    u[2] = wcpcx(w3wcx(xmin, ymin, tp));
	    v[2] = wcpcy(w3wcy(xmin, ymin, tp));
	    plnxtv(u, v, 3, 0);

	    tp += tick;
	}
	u[0] = wcpcx(w3wcx(xmax, ymin, zmin));
	v[0] = wcpcy(w3wcy(xmax, ymin, zmin));
	u[1] = wcpcx(w3wcx(xmax, ymin, zmax));
	v[1] = wcpcy(w3wcy(xmax, ymin, zmax));
	plnxtv(u, v, 2, 0);
    }
    else if (cxx <= 0.0 && cxy >= 0.0) {
	while (tp <= zmax) {
	    u[0] = wcpcx(w3wcx(xmax, ymin, tp));
	    v[0] = wcpcy(w3wcy(xmax, ymin, tp));
	    u[1] = wcpcx(w3wcx(xmin, ymin, tp));
	    v[1] = wcpcy(w3wcy(xmin, ymin, tp));
	    u[2] = wcpcx(w3wcx(xmin, ymax, tp));
	    v[2] = wcpcy(w3wcy(xmin, ymax, tp));
	    plnxtv(u, v, 3, 0);

	    tp += tick;
	}
	u[0] = wcpcx(w3wcx(xmin, ymin, zmin));
	v[0] = wcpcy(w3wcy(xmin, ymin, zmin));
	u[1] = wcpcx(w3wcx(xmin, ymin, zmax));
	v[1] = wcpcy(w3wcy(xmin, ymin, zmax));
	plnxtv(u, v, 2, 0);
    }
    else if (cxx >= 0.0 && cxy >= 0.0) {
	while (tp <= zmax) {
	    u[0] = wcpcx(w3wcx(xmin, ymin, tp));
	    v[0] = wcpcy(w3wcy(xmin, ymin, tp));
	    u[1] = wcpcx(w3wcx(xmin, ymax, tp));
	    v[1] = wcpcy(w3wcy(xmin, ymax, tp));
	    u[2] = wcpcx(w3wcx(xmax, ymax, tp));
	    v[2] = wcpcy(w3wcy(xmax, ymax, tp));
	    plnxtv(u, v, 3, 0);

	    tp += tick;
	}
	u[0] = wcpcx(w3wcx(xmin, ymax, zmin));
	v[0] = wcpcy(w3wcy(xmin, ymax, zmin));
	u[1] = wcpcx(w3wcx(xmin, ymax, zmax));
	v[1] = wcpcy(w3wcy(xmin, ymax, zmax));
	plnxtv(u, v, 2, 0);
    }
    spl3upv((PLINT) 1);
}
