/* $Id$
   $Log$
   Revision 1.4  1992/09/30 18:25:43  furnish
   Massive cleanup to irradicate garbage code.  Almost everything is now
   prototyped correctly.  Builds on HPUX, SUNOS (gcc), AIX, and UNICOS.

 * Revision 1.3  1992/09/29  04:45:50  furnish
 * Massive clean up effort to remove support for garbage compilers (K&R).
 *
 * Revision 1.2  1992/07/31  06:03:12  mjl
 * Minor bug fixes.
 *
 * Revision 1.1  1992/05/20  21:34:19  furnish
 * Initial checkin of the whole PLPLOT project.
 *
*/

/*	plconf.c

	General 2D transformation contour plotter.
*/

#include <stdio.h>
#include <stdlib.h>

#define PL_NEED_MALLOC
#include "plplot.h"

/*----------------------------------------------------------------------*\
* void plconf()
*
* Draws a contour plot from data in z(nx,ny), using the subarray from kx to
* lx in the x direction and from ky to ly in the y direction. The array of
* contour levels is clevel(nlevel), and the transformation from array
* indicies into world coordinates is done with the aid of an input
* transformation array for each coordinate.
\*----------------------------------------------------------------------*/

void 
c_plconf( PLFLT **z, PLINT nx, PLINT ny, PLINT kx, PLINT lx,
	 PLINT ky, PLINT ly, PLFLT *clevel, PLINT nlevel,
	 void (*pltr) (PLFLT x, PLFLT y, PLFLT * tx, PLFLT * ty, \
		       PLFLT * xg, PLFLT * yg, PLINT nx, PLINT ny),
	 PLFLT *xg, PLFLT *yg )
{
    PLINT i, mx, my, nstor, *heapc;

    mx = lx - kx + 1;
    my = ly - ky + 1;

    if (kx < 1 || lx > nx || kx >= lx || ky < 1 || ky > ny || ky >= ly)
	plexit("Argument error in plconf.");

    nstor = mx * my;
    if ((heapc = (PLINT *) malloc((size_t)(mx + 2 * nstor) * sizeof(PLINT))) 
		== NULL)
	plexit("Out of memory in plcont.");
    for (i = 0; i < nlevel; i++) {
	plcntf(z, nx, ny, kx, lx, ky, ly, clevel[i], &heapc[0],
	       &heapc[nx], &heapc[nx + nstor], nstor, pltr, xg, yg);
    }
    free((VOID *) heapc);
}

/*----------------------------------------------------------------------*\
* pltr1()
*
* Does linear interpolation from singly dimensioned coord arrays.
*
* Just abort for now if coordinates are out of bounds (don't think it's
* possible, but if so we could use linear extrapolation).
\*----------------------------------------------------------------------*/

void 
pltr1(PLFLT x, PLFLT y, PLFLT * tx, PLFLT * ty, PLFLT * xg, PLFLT * yg,
      PLINT nx, PLINT ny)
{
    PLINT ul, ur, vl, vr;
    PLFLT du, dv;
    PLFLT xl, xr, yl, yr;

    ul = (PLINT) x;
    ur = ul + 1;
    du = x - ul;

    vl = (PLINT) y;
    vr = vl + 1;
    dv = y - vl;

    if (x < 0 || x > nx - 1 || y < 0 || y > ny - 1) {
	pltext();
	printf("Invalid coordinates in pltr1\n");
	printf("x, y  are: %f %f\n", x, y);
	plgra();
	plexit("Aborting run");
    }

/* Look up coordinates in row-dominant array.
   Have to handle right boundary specially -- if at the edge, we'd
   better not reference the out of bounds point. */

    xl = *(xg + ul);
    yl = *(yg + vl);

    if (ur == nx) {
	*tx = xl;
    }
    else {
	xr = *(xg + ur);
	*tx = xl * (1 - du) + xr * du;
    }
    if (vr == ny) {
	*ty = yl;
    }
    else {
	yr = *(yg + vr);
	*ty = yl * (1 - dv) + yr * dv;
    }
}

/*----------------------------------------------------------------------*\
* pltr2()
*
* Does linear interpolation from doubly dimensioned coord arrays
* (column dominant, as per normal C 2d arrays).
*
* This routine includes lots of checks for out of bounds.  This would
* occur occasionally due to some bugs in the contour plotter (now fixed).
* If an out of bounds coordinate is obtained, the boundary value is provided
* along with a warning.  These checks should stay since no harm is done if
* if everything works correctly.
\*----------------------------------------------------------------------*/

void 
pltr2(PLFLT x, PLFLT y, PLFLT * tx, PLFLT * ty, PLFLT * xg, PLFLT * yg,
      PLINT nx, PLINT ny)
{
    PLINT ul, ur, vl, vr;
    PLFLT du, dv;
    PLFLT xll, xlr, xrl, xrr;
    PLFLT yll, ylr, yrl, yrr;
    PLFLT xmin, xmax, ymin, ymax;

    ul = (PLINT) x;
    ur = ul + 1;
    du = x - ul;

    vl = (PLINT) y;
    vr = vl + 1;
    dv = y - vl;

    xmin = 0;
    xmax = nx - 1;
    ymin = 0;
    ymax = ny - 1;

    if (x < xmin || x > xmax || y < ymin || y > ymax) {
	pltext();
	printf("** Warning **  Invalid coordinates in pltr2\n");
	printf("       x, y  are: %f %f\n", x, y);
	plgra();
	if (x < xmin) {

	    if (y < ymin) {
		*tx = *xg;
		*ty = *yg;
	    }
	    else if (y > ymax) {
		*tx = *(xg + (ny - 1));
		*ty = *(yg + (ny - 1));
	    }
	    else {
		ul = 0;
		xll = *(xg + ul * ny + vl);
		yll = *(yg + ul * ny + vl);
		xlr = *(xg + ul * ny + vr);
		ylr = *(yg + ul * ny + vr);

		*tx = xll * (1 - dv) + xlr * (dv);
		*ty = yll * (1 - dv) + ylr * (dv);
	    }
	}
	else if (x > xmax) {

	    if (y < ymin) {
		*tx = *(xg + (ny - 1) * nx);
		*ty = *(yg + (ny - 1) * nx);
	    }
	    else if (y > ymax) {
		*tx = *(xg + (ny - 1) + (nx - 1) * ny);
		*ty = *(yg + (ny - 1) + (nx - 1) * ny);
	    }
	    else {
		ul = nx - 1;
		xll = *(xg + ul * ny + vl);
		yll = *(yg + ul * ny + vl);
		xlr = *(xg + ul * ny + vr);
		ylr = *(yg + ul * ny + vr);

		*tx = xll * (1 - dv) + xlr * (dv);
		*ty = yll * (1 - dv) + ylr * (dv);
	    }
	}
	else {
	    if (y < ymin) {
		vl = 0;
		xll = *(xg + ul * ny + vl);
		xrl = *(xg + ur * ny + vl);
		yll = *(yg + ul * ny + vl);
		yrl = *(yg + ur * ny + vl);

		*tx = xll * (1 - du) + xrl * (du);
		*ty = yll * (1 - du) + yrl * (du);
	    }
	    else if (y > ymax) {
		vr = ny - 1;
		xlr = *(xg + ul * ny + vr);
		xrr = *(xg + ur * ny + vr);
		ylr = *(yg + ul * ny + vr);
		yrr = *(yg + ur * ny + vr);

		*tx = xlr * (1 - du) + xrr * (du);
		*ty = ylr * (1 - du) + yrr * (du);
	    }
	}
    }

/* Normal case.
   Look up coordinates in row-dominant array.
   Have to handle right boundary specially -- if at the edge, we'd
   better not reference the out of bounds point. */

    else {

	xll = *(xg + ul * ny + vl);
	yll = *(yg + ul * ny + vl);

/* ur is out of bounds */

	if (ur == nx && vr < ny) {

	    xlr = *(xg + ul * ny + vr);
	    ylr = *(yg + ul * ny + vr);

	    *tx = xll * (1 - dv) + xlr * (dv);
	    *ty = yll * (1 - dv) + ylr * (dv);
	}

/* vr is out of bounds */

	else if (ur < nx && vr == ny) {

	    xrl = *(xg + ur * ny + vl);
	    yrl = *(yg + ur * ny + vl);

	    *tx = xll * (1 - du) + xrl * (du);
	    *ty = yll * (1 - du) + yrl * (du);
	}

/* both ur and vr are out of bounds */

	else if (ur == nx && vr == ny) {

	    *tx = xll;
	    *ty = yll;
	}

/* everything in bounds */

	else {

	    xrl = *(xg + ur * ny + vl);
	    xlr = *(xg + ul * ny + vr);
	    xrr = *(xg + ur * ny + vr);

	    yrl = *(yg + ur * ny + vl);
	    ylr = *(yg + ul * ny + vr);
	    yrr = *(yg + ur * ny + vr);

	    *tx = xll * (1 - du) * (1 - dv) + xlr * (1 - du) * (dv) +
		  xrl *   (du)   * (1 - dv) + xrr *   (du)   * (dv);

	    *ty = yll * (1 - du) * (1 - dv) + ylr * (1 - du) * (dv) +
		  yrl *   (du)   * (1 - dv) + yrr *   (du)   * (dv);
	}
    }
}

/*----------------------------------------------------------------------*\
* void plcntf()
*
* The contour for a given level is drawn here.  
* Differs from 'plcntr' in that transformation arrays are used in
* obtaining coordinate values (more flexible, and suitable for calling 
* from Fortran).  These can be singly or multidimensional, since only
* pointer references are used.
*
* points is a pointer to a 2d array of nx by ny points.
* iscan has nx elements. ixstor and iystor each have nstor elements.
\*----------------------------------------------------------------------*/

void 
plcntf (PLFLT ** points, PLINT nx, PLINT ny, PLINT kx, PLINT lx,
	PLINT ky, PLINT ly, PLFLT zlev, PLINT * iscan,
	PLINT * ixstor, PLINT * iystor, PLINT nstor,
	void (*pltr) 	(PLFLT, PLFLT, PLFLT *, PLFLT *, 
			 PLFLT *, PLFLT *, PLINT, PLINT), 
	PLFLT * xg, PLFLT * yg)
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
			(*pltr) ((PLFLT)(xlas - 1), (PLFLT)(ylas - 1), &tpx, &tpy, xg, yg, nx, ny);
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
		    (*pltr) ((PLFLT)(xlas - 1), (PLFLT)(ylas - 1), &tpx, &tpy, xg, yg, nx, ny);
		    drawor(tpx, tpy);
		}
		else {
		    (*pltr) ((PLFLT)(xnew - 1), (PLFLT)(ynew - 1), &tpx, &tpy, xg, yg, nx, ny);
		    movwor(tpx, tpy);
		}
		xlas = xnew;
		ylas = ynew;

		/* Check if the contour is closed */

		if (ifirst != 1 && ix == ixbeg && iy == iybeg
		    && iw == iwbeg && iz == izbeg) {
		    (*pltr) ((PLFLT)(xlas - 1), (PLFLT)(ylas - 1), &tpx, &tpy, xg, yg, nx, ny);
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
			    pltext();
			    printf("** Warning **  Heap exhausted in plconf\n");
			    plgra();
			    goto labend;
			}
			ixstor[kstor - 1] = ix;
			iystor[kstor - 1] = iy;
		    }
		}
		goto lab20;
	lab50:
		/* Reach here only if boundary encountered - Draw last bit */

		(*pltr) ((PLFLT)(xnew - 1), (PLFLT)(ynew - 1), &tpx, &tpy, xg, yg, nx, ny);
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
labend:
    ;
}
