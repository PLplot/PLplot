/* $Id$
   $Log$
   Revision 1.7.2.1  2001/04/19 12:31:45  rlaboiss
   First merge against MAIN

   Revision 1.7  1992/10/24 05:18:08  mjl
   Added PL_NEED_SIZE_T defines where necessary.

 * Revision 1.6  1992/10/22  17:05:45  mjl
 * Fixed warnings, errors generated when compling with HP C++.
 *
 * Revision 1.5  1992/10/12  17:05:36  mjl
 * Converted to string-integer equivalence method for C-Fortran string passing.
 *
 * Revision 1.4  1992/09/30  18:26:00  furnish
 * Massive cleanup to irradicate garbage code.  Almost everything is now
 * prototyped correctly.  Builds on HPUX, SUNOS (gcc), AIX, and UNICOS.
 *
 * Revision 1.3  1992/09/29  04:46:35  furnish
 * Massive clean up effort to remove support for garbage compilers (K&R).
 *
 * Revision 1.2  1992/07/31  06:03:28  mjl
 * Minor bug fixes.
 *
 * Revision 1.1  1992/05/20  21:35:07  furnish
 * Initial checkin of the whole PLPLOT project.
 *
*/

/*	scconf.c

	Contour plotter front-ends for Fortran.
*/

#define PL_NEED_MALLOC
#define PL_NEED_SIZE_T

#include "plstubs.h"
#include <stdio.h>
#include <stdlib.h>

/*----------------------------------------------------------------------*\
* pltr0f()
*
* Identity transformation for plots from Fortran.
* Only difference from C-language identity function (pltr0) is that the
* Fortran paradigm for array index is used, i.e. starting at 1.
\*----------------------------------------------------------------------*/

void 
pltr0f (PLFLT x, PLFLT y, PLFLT * tx, PLFLT * ty)
{
    *tx = x + 1.0;
    *ty = y + 1.0;
}

/*----------------------------------------------------------------------*\
* pltr2f()
*
* Does linear interpolation from doubly dimensioned coord arrays
* (row dominant, as per normal Fortran 2d arrays).
*
* This routine includes lots of checks for out of bounds.  This would
* occur occasionally due to a bug in the contour plotter that is now fixed.
* If an out of bounds coordinate is obtained, the boundary value is provided
* along with a warning.  These checks should stay since no harm is done if
* if everything works correctly.
\*----------------------------------------------------------------------*/

void
pltr2f(PLFLT x, PLFLT y, PLFLT * tx, PLFLT * ty, PLFLT * xg, PLFLT * yg,
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
	c_pltext();
	printf("** Warning **  Invalid coordinates in pltr2f\n");
	printf("       x, y  are: %f %f\n", x, y);
	c_plgra();
	if (x < xmin) {

	    if (y < ymin) {
		*tx = *xg;
		*ty = *yg;
	    }
	    else if (y > ymax) {
		*tx = *(xg + (ny - 1) * nx);
		*ty = *(yg + (ny - 1) * nx);
	    }
	    else {
		ul = 0;
		xll = *(xg + ul + vl * nx);
		yll = *(yg + ul + vl * nx);
		xlr = *(xg + ul + vr * nx);
		ylr = *(yg + ul + vr * nx);

		*tx = xll * (1 - dv) + xlr * (dv);
		*ty = yll * (1 - dv) + ylr * (dv);
	    }
	}
	else if (x > xmax) {

	    if (y < ymin) {
		*tx = *(xg + (nx - 1));
		*ty = *(yg + (nx - 1));
	    }
	    else if (y > ymax) {
		*tx = *(xg + (nx - 1) + (ny - 1) * nx);
		*ty = *(yg + (nx - 1) + (ny - 1) * nx);
	    }
	    else {
		ul = nx - 1;
		xll = *(xg + ul + vl * nx);
		yll = *(yg + ul + vl * nx);
		xlr = *(xg + ul + vr * nx);
		ylr = *(yg + ul + vr * nx);

		*tx = xll * (1 - dv) + xlr * (dv);
		*ty = yll * (1 - dv) + ylr * (dv);
	    }
	}
	else {
	    if (y < ymin) {
		vl = 0;
		xll = *(xg + ul + vl * nx);
		xrl = *(xg + ur + vl * nx);
		yll = *(yg + ul + vl * nx);
		yrl = *(yg + ur + vl * nx);

		*tx = xll * (1 - du) + xrl * (du);
		*ty = yll * (1 - du) + yrl * (du);
	    }
	    else if (y > ymax) {
		vr = ny - 1;
		xlr = *(xg + ul + vr * nx);
		xrr = *(xg + ur + vr * nx);
		ylr = *(yg + ul + vr * nx);
		yrr = *(yg + ur + vr * nx);

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

	xll = *(xg + ul + vl * nx);
	yll = *(yg + ul + vl * nx);

/* ur is out of bounds */

	if (ur == nx && vr < ny) {

	    xlr = *(xg + ul + vr * nx);
	    ylr = *(yg + ul + vr * nx);

	    *tx = xll * (1 - dv) + xlr * (dv);
	    *ty = yll * (1 - dv) + ylr * (dv);
	}

/* vr is out of bounds */

	else if (ur < nx && vr == ny) {

	    xrl = *(xg + ur + vl * nx);
	    yrl = *(yg + ur + vl * nx);

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

	    xrl = *(xg + ur + vl * nx);
	    xlr = *(xg + ul + vr * nx);
	    xrr = *(xg + ur + vr * nx);

	    yrl = *(yg + ur + vl * nx);
	    ylr = *(yg + ul + vr * nx);
	    yrr = *(yg + ur + vr * nx);

	    *tx = xll * (1 - du) * (1 - dv) + xlr * (1 - du) * (dv) +
		  xrl *   (du)   * (1 - dv) + xrr *   (du)   * (dv);

	    *ty = yll * (1 - du) * (1 - dv) + ylr * (1 - du) * (dv) +
		  yrl *   (du)   * (1 - dv) + yrr *   (du)   * (dv);
	}
    }
}

/*----------------------------------------------------------------------*\
* Contour plotter front-ends.
* These take the transpose of the function array (to put into proper C form)
* and store into temporary memory before calling the appropriate contour
* plotter.  The routines are as follows:
*
* - plcon0	no transformation
* - plcon1	linear interpolation from singly dimensioned coord arrays
* - plcon2	linear interpolation from doubly dimensioned coord arrays
*
* The latter two work by calling plconf(), which accepts a transformation
* array for each coordinate.
\*----------------------------------------------------------------------*/

/* no transformation */

void
PLCON0_( PLFLT *z, PLINT *nx, PLINT *ny, PLINT *kx, PLINT *lx,
	PLINT *ky, PLINT *ly, PLFLT *clevel, PLINT *nlevel )
{
    int i, j;
    PLFLT **temp;

    /* Create the vectored C matrix from the Fortran matrix */
    /* To make things easy we save a temporary copy of the transpose of the
       Fortran matrix, so that the first dimension of z corresponds to the x
       direction. */

    if (!(temp = (PLFLT **) malloc((size_t)*nx * sizeof(PLFLT *))))
	plexit("Out of memory");

    for (i = 0; i < *nx; i++)
	if (!(temp[i] = (PLFLT *) malloc((size_t)*ny * sizeof(PLFLT))))
	    plexit("Out of memory");

    for (i = 0; i < *nx; i++)
	for (j = 0; j < *ny; j++)
	    temp[i][j] = *(z + j * *nx + i);

    c_plcont(temp, *nx, *ny, *kx, *lx, *ky, *ly, clevel, *nlevel, pltr0f);

    for (i = 0; i < *nx; i++)
	free((char *) temp[i]);

    free((char *) temp);
}

/* 1-d transformation */

void
PLCON1_( PLFLT *z, PLINT *nx, PLINT *ny, PLINT *kx, PLINT *lx,
	PLINT *ky, PLINT *ly, PLFLT *clevel, PLINT *nlevel,
	PLFLT *xg, PLFLT *yg )
{
    int i, j;
    PLFLT **temp;

    /* Create the vectored C matrix from the Fortran matrix */
    /* To make things easy we save a temporary copy of the transpose of the
       Fortran matrix, so that the first dimension of z corresponds to the x
       direction. */

    if (!(temp = (PLFLT **) malloc((size_t)*nx * sizeof(PLFLT *))))
	plexit("Out of memory");

    for (i = 0; i < *nx; i++)
	if (!(temp[i] = (PLFLT *) malloc((size_t)*ny * sizeof(PLFLT))))
	    plexit("Out of memory");

    for (i = 0; i < *nx; i++)
	for (j = 0; j < *ny; j++)
	    temp[i][j] = *(z + j * *nx + i);

    c_plconf(temp, *nx, *ny, *kx, *lx, *ky, *ly, clevel, *nlevel, 
	     pltr1, xg, yg);

    for (i = 0; i < *nx; i++)
	free((char *) temp[i]);

    free((char *) temp);
}

/* 2-d transformation */

void
PLCON2_( PLFLT *z, PLINT *nx, PLINT *ny, PLINT *kx, PLINT *lx,
	PLINT *ky, PLINT *ly, PLFLT *clevel, PLINT *nlevel,
	PLFLT *xg, PLFLT *yg )
{
    int i, j;
    PLFLT **temp;

    /* Create the vectored C matrix from the Fortran matrix */
    /* To make things easy we save a temporary copy of the transpose of the
       Fortran matrix, so that the first dimension of z corresponds to the x
       direction. */

    /* Note we do NOT transpose the (x,y) mapping arrays -- the transformation
       function takes care of getting the row-dominant ordering correct. */

    if (!(temp = (PLFLT **) malloc((size_t)*nx * sizeof(PLFLT *))))
	plexit("Out of memory");

    for (i = 0; i < *nx; i++)
	if (!(temp[i] = (PLFLT *) malloc((size_t)*ny * sizeof(PLFLT))))
	    plexit("Out of memory");

    for (i = 0; i < *nx; i++)
	for (j = 0; j < *ny; j++)
	    temp[i][j] = *(z + j * *nx + i);

    c_plconf(temp, *nx, *ny, *kx, *lx, *ky, *ly, clevel, *nlevel, 
	     pltr2f, xg, yg);

    for (i = 0; i < *nx; i++)
	free((char *) temp[i]);

    free((char *) temp);
}

/*----------------------------------------------------------------------*\
* Here are the old contour plotters.
\*----------------------------------------------------------------------*/

PLFLT tr[6];

void
xform(PLFLT x, PLFLT y, PLFLT * tx, PLFLT * ty)
{
    *tx = tr[0] * x + tr[1] * y + tr[2];
    *ty = tr[3] * x + tr[4] * y + tr[5];
}

void
PLCONT_(PLFLT *z, PLINT *nx, PLINT *ny, PLINT *kx, PLINT *lx,
	PLINT *ky, PLINT *ly, PLFLT *clevel, PLINT *nlevel, PLFLT *ftr )
{
    int i, j;
    PLFLT **temp;

    for (i = 0; i < 6; i++)
	tr[i] = ftr[i];

    /* Create the vectored C matrix from the Fortran matrix */
    /* To make things easy we save a temporary copy of the transpose of the
       Fortran matrix, so that the first dimension of z corresponds to the x
       direction. */

    if (!(temp = (PLFLT **) malloc((size_t)*nx * sizeof(PLFLT *))))
	plexit("Out of memory");

    for (i = 0; i < *nx; i++)
	if (!(temp[i] = (PLFLT *) malloc((size_t)*ny * sizeof(PLFLT))))
	    plexit("Out of memory");

    for (i = 0; i < *nx; i++)
	for (j = 0; j < *ny; j++)
	    temp[i][j] = *(z + j * *nx + i);

    c_plcont(temp, *nx, *ny, *kx, *lx, *ky, *ly, clevel, *nlevel, xform);

    for (i = 0; i < *nx; i++)
	free((char *) temp[i]);

    free((char *) temp);
}
