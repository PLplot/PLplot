/* $Id$
   $Log$
   Revision 1.3  1992/09/30 18:25:53  furnish
   Massive cleanup to irradicate garbage code.  Almost everything is now
   prototyped correctly.  Builds on HPUX, SUNOS (gcc), AIX, and UNICOS.

 * Revision 1.2  1992/09/29  04:46:11  furnish
 * Massive clean up effort to remove support for garbage compilers (K&R).
 *
 * Revision 1.1  1992/05/20  21:34:42  furnish
 * Initial checkin of the whole PLPLOT project.
 *
*/

/*	plot3d.c

	Draw next view of 3d plot.
*/

#include <math.h>
#include <stdlib.h>

#define PL_NEED_MALLOC
#include "plplot.h"

/* 
*  Internal constants.
*/

static PLINT zbflg = 0, zbcol;
static PLFLT zbtck;

/*----------------------------------------------------------------------*\
* External access routines.
\*----------------------------------------------------------------------*/

void 
gzback (PLINT **zbf, PLINT **zbc, PLFLT **zbt)
{
    *zbf = &zbflg;
    *zbc = &zbcol;
    *zbt = &zbtck;
}

/*----------------------------------------------------------------------*\
* void plot3d(x, y, z, nx, ny, opt, side)
*
* Plots a 3-d representation of the function z[x][y]. The x values 
* are stored as x[0..nx-1], the y values as y[0..ny-1], and the z
* values are in the 2-d array z[][]. The integer "opt" specifies: 
*
*  opt = 1:  Draw lines parallel to x-axis 
*  opt = 2:  Draw lines parallel to y-axis 
*  opt = 3:  Draw lines parallel to both axes 
\*----------------------------------------------------------------------*/

void 
c_plot3d( PLFLT *x, PLFLT *y, PLFLT **z,
	 PLINT nx, PLINT ny, PLINT opt, PLINT side )
{
    PLINT b, color, font;
    PLFLT cxx, cxy, cyx, cyy, cyz;
    PLINT init;
    PLINT *ohivw, *work;
    PLINT i, ix, iy;
    PLINT level;

    glev(&level);
    if (level < 3)
	plexit("Please set up window before calling plot3d");

    if (opt < 1 || opt > 3)
	plexit("Bad option in plot3d");
    if (nx <= 0 || ny <= 0)
	plexit("Bad array dimensions in plot3d.");

    /* Check that points in x and in y are strictly increasing */

    for (i = 0; i < nx - 1; i++)
	if (x[i] >= x[i + 1])
	    plexit("X array must be strictly increasing in plot3d");

    for (i = 0; i < ny - 1; i++)
	if (y[i] >= y[i + 1])
	    plexit("Y array must be strictly increasing in plot3d");

    work = (PLINT *) malloc((size_t) (4 * MAX(nx, ny) * sizeof(PLINT)));
    if (!work)
	plexit("Out of memory in plot3d.");
    b = 2 * MAX(nx, ny) + 1;
    gw3wc(&cxx, &cxy, &cyx, &cyy, &cyz);
    init = 1;

    if (cxx >= 0.0 && cxy <= 0.0) {
	if (opt != 2) {
	    for (iy = 2; iy <= ny; iy++) {
		plt3zz(1, iy, 1, -1, -opt, init, x, y, z, nx, ny, &work[0], &work[b - 1]);
		init = 0;
	    }
	}
	else {
	    plt3zz(1, ny, 1, -1, -opt, init, x, y, z, nx, ny, &work[0], &work[b - 1]);
	    init = 0;
	}
	if (opt != 1)
	    for (ix = 1; ix <= nx - 1; ix++)
		plt3zz(ix, ny, 1, -1, opt, init, x, y, z, nx, ny, &work[0], &work[b - 1]);
	else
	    plt3zz(1, ny, 1, -1, opt, init, x, y, z, nx, ny, &work[0], &work[b - 1]);
    }
    else if (cxx <= 0.0 && cxy <= 0.0) {
	if (opt != 1) {
	    for (ix = 2; ix <= nx; ix++) {
		plt3zz(ix, ny, -1, -1, opt, init, x, y, z, nx, ny, &work[0], &work[b - 1]);
		init = 0;
	    }
	}
	else {
	    plt3zz(nx, ny, -1, -1, opt, init, x, y, z, nx, ny, &work[0], &work[b - 1]);
	    init = 0;
	}
	if (opt != 2)
	    for (iy = ny; iy >= 2; iy--)
		plt3zz(nx, iy, -1, -1, -opt, init, x, y, z, nx, ny, &work[0], &work[b - 1]);
	else
	    plt3zz(nx, ny, -1, -1, -opt, init, x, y, z, nx, ny, &work[0], &work[b - 1]);
    }
    else if (cxx <= 0.0 && cxy >= 0.0) {
	if (opt != 2) {
	    for (iy = ny - 1; iy >= 1; iy--) {
		plt3zz(nx, iy, -1, 1, -opt, init, x, y, z, nx, ny, &work[0], &work[b - 1]);
		init = 0;
	    }
	}
	else {
	    plt3zz(nx, 1, -1, 1, -opt, init, x, y, z, nx, ny, &work[0], &work[b - 1]);
	    init = 0;
	}
	if (opt != 1)
	    for (ix = nx; ix >= 2; ix--)
		plt3zz(ix, 1, -1, 1, opt, init, x, y, z, nx, ny, &work[0], &work[b - 1]);
	else
	    plt3zz(nx, 1, -1, 1, opt, init, x, y, z, nx, ny, &work[0], &work[b - 1]);
    }
    else if (cxx >= 0.0 && cxy >= 0.0) {
	if (opt != 1) {
	    for (ix = nx - 1; ix >= 1; ix--) {
		plt3zz(ix, 1, 1, 1, opt, init, x, y, z, nx, ny, &work[0], &work[b - 1]);
		init = 0;
	    }
	}
	else {
	    plt3zz(1, 1, 1, 1, opt, init, x, y, z, nx, ny, &work[0], &work[b - 1]);
	    init = 0;
	}
	if (opt != 2)
	    for (iy = 1; iy <= ny - 1; iy++)
		plt3zz(1, iy, 1, 1, -opt, init, x, y, z, nx, ny, &work[0], &work[b - 1]);
	else
	    plt3zz(1, 1, 1, 1, -opt, init, x, y, z, nx, ny, &work[0], &work[b - 1]);
    }

    free((VOID *) work);

    if (side)
	plside3a(x, y, z, nx, ny, opt);

    if (zbflg) {
	gatt(&font, &color);
	plcol(zbcol);
	plgrid3a(zbtck);
	plcol(color);
    }

    goldhivw(&ohivw);
    free((VOID *) ohivw);
}
