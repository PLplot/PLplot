/* $Id$

	Contour plotter front-ends for Fortran.
*/

#include "plstubs.h"

/*----------------------------------------------------------------------*\
* pltr0f()
*
* Identity transformation for plots from Fortran.
* Only difference from C-language identity function (pltr0) is that the
* Fortran paradigm for array index is used, i.e. starting at 1.
\*----------------------------------------------------------------------*/

void
pltr0f(PLFLT x, PLFLT y, PLFLT *tx, PLFLT *ty, void *pltr_data)
{
    *tx = x + 1.0;
    *ty = y + 1.0;
}

/*----------------------------------------------------------------------*\
* pltr2f()
*
* Does linear interpolation from doubly dimensioned coord arrays
* (row dominant, i.e. Fortran ordering).
*
* This routine includes lots of checks for out of bounds.  This would
* occur occasionally due to a bug in the contour plotter that is now fixed.
* If an out of bounds coordinate is obtained, the boundary value is provided
* along with a warning.  These checks should stay since no harm is done if
* if everything works correctly.
\*----------------------------------------------------------------------*/

void
pltr2f(PLFLT x, PLFLT y, PLFLT *tx, PLFLT *ty, void *pltr_data)
{
    PLINT ul, ur, vl, vr;
    PLFLT du, dv;
    PLFLT xll, xlr, xrl, xrr;
    PLFLT yll, ylr, yrl, yrr;
    PLFLT xmin, xmax, ymin, ymax;

    PLcGrid *cgrid = (PLcGrid *) pltr_data;
    PLFLT *xg = cgrid->xg;
    PLFLT *yg = cgrid->yg;
    PLINT nx = cgrid->nx;
    PLINT ny = cgrid->ny;

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
	plwarn("pltr2f: Invalid coordinates");

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
/* INDENT OFF */
	    *tx = xll * (1 - du) * (1 - dv) + xlr * (1 - du) * (dv) +
		  xrl *   (du)   * (1 - dv) + xrr *   (du)   * (dv);

	    *ty = yll * (1 - du) * (1 - dv) + ylr * (1 - du) * (dv) +
		  yrl *   (du)   * (1 - dv) + yrr *   (du)   * (dv);
/* INDENT ON */
	}
    }
}

/*----------------------------------------------------------------------*\
* Contour plotter front-ends.
* These specify the row-dominant function evaluator in the plfcont
* argument list.  NO TRANSPOSE IS NECESSARY.  The routines are as follows:
*
* - plcon0	no transformation
* - plcon1	linear interpolation from singly dimensioned coord arrays
* - plcon2	linear interpolation from doubly dimensioned coord arrays
*
* The latter two work by calling plfcont() with the appropriate grid
* structure for input to pltr2f().
\*----------------------------------------------------------------------*/

/* no transformation */

void
PLCON07(PLFLT *z, PLINT *nx, PLINT *ny, PLINT *kx, PLINT *lx,
	PLINT *ky, PLINT *ly, PLFLT *clevel, PLINT *nlevel)
{
    PLfGrid fgrid;

    fgrid.nx = *nx;
    fgrid.ny = *ny;
    fgrid.f = z;

    plfcont(plf2evalr, (void *) &fgrid,
	    *nx, *ny, *kx, *lx, *ky, *ly, clevel, *nlevel,
	    pltr0f, NULL);
}

/* 1-d transformation */

void
PLCON17(PLFLT *z, PLINT *nx, PLINT *ny, PLINT *kx, PLINT *lx,
	PLINT *ky, PLINT *ly, PLFLT *clevel, PLINT *nlevel,
	PLFLT *xg, PLFLT *yg)
{
    PLfGrid fgrid;
    PLcGrid cgrid;

    fgrid.nx = *nx;
    fgrid.ny = *ny;
    fgrid.f = z;

    cgrid.nx = *nx;
    cgrid.ny = *ny;
    cgrid.xg = xg;
    cgrid.yg = yg;

    plfcont(plf2evalr, (void *) &fgrid,
	    *nx, *ny, *kx, *lx, *ky, *ly, clevel, *nlevel,
	    pltr1, (void *) &cgrid);
}

/* 2-d transformation */

void
PLCON27(PLFLT *z, PLINT *nx, PLINT *ny, PLINT *kx, PLINT *lx,
	PLINT *ky, PLINT *ly, PLFLT *clevel, PLINT *nlevel,
	PLFLT *xg, PLFLT *yg)
{
    PLfGrid fgrid;
    PLcGrid cgrid;

    fgrid.nx = *nx;
    fgrid.ny = *ny;
    fgrid.f = z;

    cgrid.nx = *nx;
    cgrid.ny = *ny;
    cgrid.xg = xg;
    cgrid.yg = yg;

    plfcont(plf2evalr, (void *) &fgrid,
	    *nx, *ny, *kx, *lx, *ky, *ly, clevel, *nlevel,
	    pltr2f, (void *) &cgrid);
}

/*----------------------------------------------------------------------*\
* Here are the old contour plotters.
\*----------------------------------------------------------------------*/

static void
pltr(PLFLT x, PLFLT y, PLFLT *tx, PLFLT *ty, void *pltr_data)
{
    PLFLT *tr = (PLFLT *) pltr_data;

    *tx = tr[0] * x + tr[1] * y + tr[2];
    *ty = tr[3] * x + tr[4] * y + tr[5];
}

void
PLCONT7(PLFLT *z, PLINT *nx, PLINT *ny, PLINT *kx, PLINT *lx,
	PLINT *ky, PLINT *ly, PLFLT *clevel, PLINT *nlevel, PLFLT *ftr)
{
    PLfGrid fgrid;

    fgrid.nx = *nx;
    fgrid.ny = *ny;
    fgrid.f = z;

    plfcont(plf2evalr, (void *) &fgrid,
	    *nx, *ny, *kx, *lx, *ky, *ly, clevel, *nlevel,
	    pltr, (void *) ftr);
}

/*----------------------------------------------------------------------*\
* plfshade front-ends.
* These specify the row-dominant function evaluator in the plfshade
* argument list.  NO TRANSPOSE IS NECESSARY.  The routines are as follows:
*
* - plshade0	no transformation
* - plshade1	linear interpolation from singly dimensioned coord arrays
* - plshade2	linear interpolation from doubly dimensioned coord arrays
*
* The latter two work by calling plfshade() with the appropriate grid
* structure for input to pltr2f().
\*----------------------------------------------------------------------*/

void
PLSHADE07(PLFLT *z, PLINT *nx, PLINT *ny, char *defined,
	  PLFLT *xmin, PLFLT *xmax, PLFLT *ymin, PLFLT *ymax,
	  PLFLT *shade_min, PLFLT *shade_max,
	  PLINT *sh_cmap, PLFLT *sh_color, PLINT *sh_width,
	  PLINT *min_color, PLINT *min_width,
	  PLINT *max_color, PLINT *max_width)
{
    PLfGrid fgrid;
    PLINT rect = 1;

    fgrid.nx = *nx;
    fgrid.ny = *ny;
    fgrid.f = z;

    plfshade(plf2evalr, (PLPointer) &fgrid,
	     NULL, NULL,
	     *nx, *ny,
	     *xmin, *xmax, *ymin, *ymax,
	     *shade_min, *shade_max,
	     *sh_cmap, *sh_color, *sh_width,
	     *min_color, *min_width, *max_color, *max_width,
	     c_plfill, rect, NULL, NULL);
}

/* 1-d transformation */

void
PLSHADE17(PLFLT *z, PLINT *nx, PLINT *ny, char *defined,
	  PLFLT *xmin, PLFLT *xmax, PLFLT *ymin, PLFLT *ymax,
	  PLFLT *shade_min, PLFLT *shade_max,
	  PLINT *sh_cmap, PLFLT *sh_color, PLINT *sh_width,
	  PLINT *min_color, PLINT *min_width,
	  PLINT *max_color, PLINT *max_width,
	  PLFLT *xg, PLFLT *yg)
{
    PLfGrid fgrid;
    PLcGrid cgrid;
    PLINT rect = 1;

    fgrid.nx = *nx;
    fgrid.ny = *ny;
    fgrid.f = z;

    cgrid.nx = *nx;
    cgrid.ny = *ny;
    cgrid.xg = xg;
    cgrid.yg = yg;

    plfshade(plf2evalr, (PLPointer) &fgrid,
	     NULL, NULL,
	     *nx, *ny,
	     *xmin, *xmax, *ymin, *ymax,
	     *shade_min, *shade_max,
	     *sh_cmap, *sh_color, *sh_width,
	     *min_color, *min_width, *max_color, *max_width,
	     c_plfill, rect, pltr1, (PLPointer) &cgrid);
}

/* 2-d transformation */

void
PLSHADE27(PLFLT *z, PLINT *nx, PLINT *ny, char *defined,
	  PLFLT *xmin, PLFLT *xmax, PLFLT *ymin, PLFLT *ymax,
	  PLFLT *shade_min, PLFLT *shade_max,
	  PLINT *sh_cmap, PLFLT *sh_color, PLINT *sh_width,
	  PLINT *min_color, PLINT *min_width,
	  PLINT *max_color, PLINT *max_width,
	  PLFLT *xg, PLFLT *yg)
{
    PLfGrid fgrid;
    PLcGrid cgrid;
    PLINT rect = 0;

    fgrid.nx = *nx;
    fgrid.ny = *ny;
    fgrid.f = z;

    cgrid.nx = *nx;
    cgrid.ny = *ny;
    cgrid.xg = xg;
    cgrid.yg = yg;

    plfshade(plf2evalr, (PLPointer) &fgrid,
	     NULL, NULL,
	     *nx, *ny,
	     *xmin, *xmax, *ymin, *ymax,
	     *shade_min, *shade_max,
	     *sh_cmap, *sh_color, *sh_width,
	     *min_color, *min_width, *max_color, *max_width,
	     c_plfill, rect, pltr2f, (PLPointer) &cgrid);
}
