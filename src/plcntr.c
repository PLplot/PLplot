/* $Id$
   $Log$
   Revision 1.1  1992/05/20 21:34:17  furnish
   Initial checkin of the whole PLPLOT project.

*/

/*	plcntr.c

	Plots an individual contour.
*/

#include "plplot.h"

/*----------------------------------------------------------------------*\
* void plcntr()
*
* The contour for a given level is drawn here.  
*
* points is a pointer to a 2d array of nx by ny points.
* iscan has nx elements. ixstor and iystor each have nstor elements.
\*----------------------------------------------------------------------*/

#ifdef PLSTDC
void 
plcntr(PLFLT ** points, PLINT nx, PLINT ny, PLINT kx, PLINT lx,
       PLINT ky, PLINT ly, PLFLT zlev, PLINT * iscan,
       PLINT * ixstor, PLINT * iystor, PLINT nstor,
       void (*pltr) (PLFLT, PLFLT, PLFLT *, PLFLT *))
#else
void 
plcntr(points, nx, ny, kx, lx, ky, ly, zlev, iscan, ixstor, iystor, nstor, pltr)
PLINT nx, ny, ky, ly, kx, lx, nstor;
PLFLT zlev, **points;
PLINT *iscan, *ixstor, *iystor;
void (*pltr) ();
#endif
{
    PLINT kcol, krow, kstor, kscan, iwbeg, ixbeg, iybeg, izbeg;
    PLINT iboun, iw, ix, iy, iz, ifirst, istep, ixgo, iygo;
    PLINT l, ixg, iyg, ia, ib, ixt, iyt, jstor, next;
    PLFLT dist, dx, dy, xnew, ynew, x, y;
    PLFLT xlas = 0., ylas = 0., tpx, tpy, xt, yt;
    PLFLT z1, z2, z3, z4, zcheck;

    /* Initialize memory pointers */

    kstor = 0;
    kscan = 0;

    for (krow = ky; krow <= ly; krow++) {
	for (kcol = kx + 1; kcol <= lx; kcol++) {

	    /* Check if a contour has been crossed */

	    x = points[kcol - 2][krow - 1];
	    y = points[kcol - 1][krow - 1];
	    if (x < zlev && y >= zlev) {
		ixbeg = kcol - 1;
		iwbeg = kcol;
	    }
	    else if (y < zlev && x >= zlev) {
		ixbeg = kcol;
		iwbeg = kcol - 1;
	    }
	    else
		goto lab70;

	    iybeg = krow;
	    izbeg = krow;

	    /* Yes, a contour has been crossed. Check to see if it */
	    /* is a new one. */

	    for (l = 0; l < kscan; l++)
		if (ixbeg == iscan[l])
		    goto lab70;

	    /* Here is the section which follows and draws a contour */

	    for (iboun = 1; iboun >= -1; iboun -= 2) {

		/* Set up starting point and initial search directions */

		ix = ixbeg;
		iy = iybeg;
		iw = iwbeg;
		iz = izbeg;
		ifirst = 1;
		istep = 0;
		ixgo = iw - ix;
		iygo = iz - iy;

	lab20:
		plccal(points, nx, ny, zlev, ix, iy, ixgo, iygo, &dist);
		dx = dist * ixgo;
		dy = dist * iygo;
		xnew = ix + dx;
		ynew = iy + dy;

		/* Has a step occured in search? */

		if (istep != 0) {
		    if (ixgo * iygo == 0) {

			/* This was a diagonal step, so interpolate missed */
			/* point, rotating 45 degrees to get it */

			ixg = ixgo;
			iyg = iygo;
			plr45(&ixg, &iyg, iboun);
			ia = iw - ixg;
			ib = iz - iyg;
			plccal(points, nx, ny, zlev, ia, ib, ixg, iyg, &dist);
			(*pltr) ((PLFLT) (xlas - 1), (PLFLT) (ylas - 1), &tpx, &tpy);
			drawor(tpx, tpy);
			dx = dist * ixg;
			dy = dist * iyg;
			xlas = ia + dx;
			ylas = ib + dy;
		    }
		    else {
			if (dist > 0.5) {
			    xt = xlas;
			    xlas = xnew;
			    xnew = xt;
			    yt = ylas;
			    ylas = ynew;
			    ynew = yt;
			}
		    }
		}
		if (ifirst != 1) {
		    (*pltr) ((PLFLT) (xlas - 1), (PLFLT) (ylas - 1), &tpx, &tpy);
		    drawor(tpx, tpy);
		}
		else {
		    (*pltr) ((PLFLT) (xnew - 1), (PLFLT) (ynew - 1), &tpx, &tpy);
		    movwor(tpx, tpy);
		}
		xlas = xnew;
		ylas = ynew;

		/* Check if the contour is closed */

		if (ifirst != 1 && ix == ixbeg && iy == iybeg
		    && iw == iwbeg && iz == izbeg) {
		    (*pltr) ((PLFLT) (xlas - 1), (PLFLT) (ylas - 1), &tpx, &tpy);
		    drawor(tpx, tpy);
		    goto lab70;
		}
		ifirst = 0;

		/* Now the rotation */

		istep = 0;
		plr45(&ixgo, &iygo, iboun);
		iw = ix + ixgo;
		iz = iy + iygo;

		/* Check if out of bounds */

		if (iw < kx || iw > lx || iz < ky || iz > ly)
		    goto lab50;

		/* Has contact been lost with the contour? */

	/* Bug fix by Mike McCarrick mmccarrick@uclaph.physics.ucla.edu */

		if(ixgo*iygo == 0)
		    zcheck = points[iw-1][iz-1];
		else {
		    z1 = points[ix-1][iy-1];
		    z2 = points[iw-1][iz-1];
		    z3 = points[ix-1][iz-1];
		    z4 = points[iw-1][iy-1];
		    zcheck = MAX(z2,(z1+z2+z3+z4)/4.);
		}

		if (zcheck < zlev) {

		    /* Yes, lost contact => step to new centre */

		    istep = 1;
		    ix = iw;
		    iy = iz;
		    plr135(&ixgo, &iygo, iboun);
		    iw = ix + ixgo;
		    iz = iy + iygo;

		    /* And do the contour memory */

		    if (iy == krow) {
			kscan = kscan + 1;
			iscan[kscan - 1] = ix;
		    }
		    else if (iy > krow) {
			kstor = kstor + 1;
			if (kstor > nstor) {
			    plexit("Heap exhausted in plcont.");
			}
			ixstor[kstor - 1] = ix;
			iystor[kstor - 1] = iy;
		    }
		}
		goto lab20;
	lab50:
		/* Reach here only if boundary encountered - Draw last bit */

		(*pltr) ((PLFLT) (xnew - 1), (PLFLT) (ynew - 1), &tpx, &tpy);
		drawor(tpx, tpy);
	    }
    lab70:
	    ;			/* Null statement to carry label */
	}

	/* Search of row complete - set up memory of next row in iscan and */
	/* edit ixstor and iystor */

	if (krow < ny) {
	    jstor = 0;
	    kscan = 0;
	    next = krow + 1;
	    for (l = 1; l <= kstor; l++) {
		ixt = ixstor[l - 1];
		iyt = iystor[l - 1];

		/* Memory of next row into iscan */

		if (iyt == next) {
		    kscan = kscan + 1;
		    iscan[kscan - 1] = ixt;

		    /* Retain memory of rows to come, and forget rest */

		}
		else if (iyt > next) {
		    jstor = jstor + 1;
		    ixstor[jstor - 1] = ixt;
		    iystor[jstor - 1] = iyt;
		}
	    }
	    kstor = jstor;
	}
    }
}
