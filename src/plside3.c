/* $Id$
   $Log$
   Revision 1.2  1992/09/29 04:46:17  furnish
   Massive clean up effort to remove support for garbage compilers (K&R).

 * Revision 1.1  1992/05/20  21:34:47  furnish
 * Initial checkin of the whole PLPLOT project.
 *
*/

/*	plside3a.c

	This routine draws sides around the front of the 3d plot so that
	it does not appear to float.
*/

#include "plplot.h"

void 
plside3a (PLFLT *x, PLFLT *y, PLFLT **z, PLINT nx, PLINT ny, PLINT opt)
{
    PLINT i;
    PLFLT cxx, cxy, cyx, cyy, cyz;
    PLFLT xmin, ymin, zmin, xmax, ymax, zmax, zscale;
    PLFLT tx, ty, ux, uy;

    gw3wc(&cxx, &cxy, &cyx, &cyy, &cyz);
    gdom(&xmin, &xmax, &ymin, &ymax);
    grange(&zscale, &zmin, &zmax);

    if (cxx >= 0.0 && cxy <= 0.0) {
	/* Get x, y coordinates of legs and plot */
	if (opt != 1) {
	    for (i = 0; i < nx; i++) {
		tx = w3wcx(x[i], y[0], zmin);
		ty = w3wcy(x[i], y[0], zmin);
		ux = w3wcx(x[i], y[0], z[i][0]);
		uy = w3wcy(x[i], y[0], z[i][0]);
		pljoin(tx, ty, ux, uy);
	    }
	}

	if (opt != 2) {
	    for (i = 0; i < ny; i++) {
		tx = w3wcx(x[0], y[i], zmin);
		ty = w3wcy(x[0], y[i], zmin);
		ux = w3wcx(x[0], y[i], z[0][i]);
		uy = w3wcy(x[0], y[i], z[0][i]);
		pljoin(tx, ty, ux, uy);
	    }
	}
    }
    else if (cxx <= 0.0 && cxy <= 0.0) {
	if (opt != 1) {
	    for (i = 0; i < nx; i++) {
		tx = w3wcx(x[i], y[ny - 1], zmin);
		ty = w3wcy(x[i], y[ny - 1], zmin);
		ux = w3wcx(x[i], y[ny - 1], z[i][ny - 1]);
		uy = w3wcy(x[i], y[ny - 1], z[i][ny - 1]);
		pljoin(tx, ty, ux, uy);
	    }
	}

	if (opt != 2) {
	    for (i = 0; i < ny; i++) {
		tx = w3wcx(x[0], y[i], zmin);
		ty = w3wcy(x[0], y[i], zmin);
		ux = w3wcx(x[0], y[i], z[0][i]);
		uy = w3wcy(x[0], y[i], z[0][i]);
		pljoin(tx, ty, ux, uy);
	    }
	}
    }
    else if (cxx <= 0.0 && cxy >= 0.0) {
	if (opt != 1) {
	    for (i = 0; i < nx; i++) {
		tx = w3wcx(x[i], y[ny - 1], zmin);
		ty = w3wcy(x[i], y[ny - 1], zmin);
		ux = w3wcx(x[i], y[ny - 1], z[i][ny - 1]);
		uy = w3wcy(x[i], y[ny - 1], z[i][ny - 1]);
		pljoin(tx, ty, ux, uy);
	    }
	}

	if (opt != 2) {
	    for (i = 0; i < ny; i++) {
		tx = w3wcx(x[nx - 1], y[i], zmin);
		ty = w3wcy(x[nx - 1], y[i], zmin);
		ux = w3wcx(x[nx - 1], y[i], z[nx - 1][i]);
		uy = w3wcy(x[nx - 1], y[i], z[nx - 1][i]);
		pljoin(tx, ty, ux, uy);
	    }
	}
    }
    else if (cxx >= 0.0 && cxy >= 0.0) {
	if (opt != 1) {
	    for (i = 0; i < nx; i++) {
		tx = w3wcx(x[i], y[0], zmin);
		ty = w3wcy(x[i], y[0], zmin);
		ux = w3wcx(x[i], y[0], z[i][0]);
		uy = w3wcy(x[i], y[0], z[i][0]);
		pljoin(tx, ty, ux, uy);
	    }
	}

	if (opt != 2) {
	    for (i = 0; i < ny; i++) {
		tx = w3wcx(x[nx - 1], y[i], zmin);
		ty = w3wcy(x[nx - 1], y[i], zmin);
		ux = w3wcx(x[nx - 1], y[i], z[nx - 1][i]);
		uy = w3wcy(x[nx - 1], y[i], z[nx - 1][i]);
		pljoin(tx, ty, ux, uy);
	    }
	}
    }
}
