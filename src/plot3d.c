/* $Id$
   $Log$
   Revision 1.5  1993/01/23 05:57:23  mjl
   Now holds all routines dealing with 3d plots (and hence has become rather
   large).  A capability similar to that in the contour plotter (passing
   in function evaluators instead of 2d arrays) is planned but not in this
   version.

 * Revision 1.4  1992/10/12  17:08:08  mjl
 * Added PL_NEED_SIZE_T define to those files that need to know the value
 * of (size_t) for non-POSIX systems (in this case the Amiga) that require you
 * to include <stddef.h> to get it.
 *
 * Revision 1.3  1992/09/30  18:25:53  furnish
 * Massive cleanup to irradicate garbage code.  Almost everything is now
 * prototyped correctly.  Builds on HPUX, SUNOS (gcc), AIX, and UNICOS.
 *
 * Revision 1.2  1992/09/29  04:46:11  furnish
 * Massive clean up effort to remove support for garbage compilers (K&R).
 *
 * Revision 1.1  1992/05/20  21:34:42  furnish
 * Initial checkin of the whole PLPLOT project.
 *
*/

/*	plot3d.c

	3d plot routines.
*/

#define PL_NEED_MALLOC
#define PL_NEED_SIZE_T

#include "plplot.h"
#include <math.h>
#include <stdlib.h>

/* Internal constants */

#define  BINC      50		/* Block size for memory allocation */

static PLINT pl3mode = 0;	/* 0 3d solid; 1 mesh plot */
static PLINT pl3upv = 1;	/* 1 update view; 0 no update */

static PLINT zbflg = 0, zbcol;
static PLFLT zbtck;

static PLINT *oldhiview;
static PLINT *oldloview;
static PLINT *newhiview;
static PLINT *newloview;

static PLINT mhi, xxhi, newhisize;
static PLINT mlo, xxlo, newlosize;

/* Prototypes for static functions */
/* INDENT OFF */

static void plgrid3	(PLFLT);
static void plnxtv	(PLINT *, PLINT *, PLINT, PLINT);
static void plside3	(PLFLT *, PLFLT *, PLFLT **, PLINT, PLINT, PLINT);
static void plt3zz	(PLINT, PLINT, PLINT, PLINT, 
			 PLINT, PLINT, PLFLT *, PLFLT *, PLFLT **, 
			 PLINT, PLINT, PLINT *, PLINT *);
static void plnxtvhi	(PLINT *, PLINT *, PLINT, PLINT);
static void plnxtvlo	(PLINT *, PLINT *, PLINT, PLINT);
static void savehipoint	(PLINT, PLINT);
static void savelopoint	(PLINT, PLINT);
static void swaphiview	(void);
static void swaploview	(void);
static int  plabv	(PLINT, PLINT, PLINT, PLINT, PLINT, PLINT);
static void pl3cut	(PLINT, PLINT, PLINT, PLINT, PLINT, 
				PLINT, PLINT, PLINT, PLINT *, PLINT *);
/* INDENT ON */

/*----------------------------------------------------------------------*\
* void plmesh(x, y, z, nx, ny, opt)
*
* Plots a mesh representation of the function z[x][y]. The x values
* are stored as x[0..nx-1], the y values as y[0..ny-1], and the
* z values are in the 2-d array z[][]. The integer "opt" specifies:
*
*  opt = 1:  Draw lines parallel to x-axis
*  opt = 2:  Draw lines parallel to y-axis
*  opt = 3:  Draw lines parallel to both axes
\*----------------------------------------------------------------------*/

void
c_plmesh(PLFLT *x, PLFLT *y, PLFLT **z, PLINT nx, PLINT ny, PLINT opt)
{
    pl3mode = 1;
    plot3d(x, y, z, nx, ny, opt, 0);

    free((VOID *) oldloview);
    pl3mode = 0;
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
c_plot3d(PLFLT *x, PLFLT *y, PLFLT **z,
	 PLINT nx, PLINT ny, PLINT opt, PLINT side)
{
    PLINT b, color, font;
    PLFLT cxx, cxy, cyx, cyy, cyz;
    PLINT init, *work;
    PLINT i, ix, iy;
    PLINT level;

    glev(&level);
    if (level < 3)
	plexit("plot3d: Please set up window first");

    if (opt < 1 || opt > 3)
	plexit("plot3d: Bad option");

    if (nx <= 0 || ny <= 0)
	plexit("plot3d: Bad array dimensions.");

/* Check that points in x and in y are strictly increasing */

    for (i = 0; i < nx - 1; i++)
	if (x[i] >= x[i + 1])
	    plexit("plot3d: X array must be strictly increasing");

    for (i = 0; i < ny - 1; i++)
	if (y[i] >= y[i + 1])
	    plexit("plot3d: Y array must be strictly increasing");

    work = (PLINT *) malloc((size_t) (4 * MAX(nx, ny) * sizeof(PLINT)));
    if (!work)
	plexit("plot3d: Out of memory.");

    b = 2 * MAX(nx, ny) + 1;
    gw3wc(&cxx, &cxy, &cyx, &cyy, &cyz);
    init = 1;

    if (cxx >= 0.0 && cxy <= 0.0) {
	if (opt != 2) {
	    for (iy = 2; iy <= ny; iy++) {
		plt3zz(1, iy, 1, -1, -opt, init, x, y, z, nx, ny,
		       &work[0], &work[b - 1]);
		init = 0;
	    }
	}
	else {
	    plt3zz(1, ny, 1, -1, -opt, init, x, y, z, nx, ny,
		   &work[0], &work[b - 1]);
	    init = 0;
	}
	if (opt != 1)
	    for (ix = 1; ix <= nx - 1; ix++)
		plt3zz(ix, ny, 1, -1, opt, init, x, y, z, nx, ny,
		       &work[0], &work[b - 1]);
	else
	    plt3zz(1, ny, 1, -1, opt, init, x, y, z, nx, ny,
		   &work[0], &work[b - 1]);
    }
    else if (cxx <= 0.0 && cxy <= 0.0) {
	if (opt != 1) {
	    for (ix = 2; ix <= nx; ix++) {
		plt3zz(ix, ny, -1, -1, opt, init, x, y, z, nx, ny,
		       &work[0], &work[b - 1]);
		init = 0;
	    }
	}
	else {
	    plt3zz(nx, ny, -1, -1, opt, init, x, y, z, nx, ny,
		   &work[0], &work[b - 1]);
	    init = 0;
	}
	if (opt != 2)
	    for (iy = ny; iy >= 2; iy--)
		plt3zz(nx, iy, -1, -1, -opt, init, x, y, z, nx, ny,
		       &work[0], &work[b - 1]);
	else
	    plt3zz(nx, ny, -1, -1, -opt, init, x, y, z, nx, ny,
		   &work[0], &work[b - 1]);
    }
    else if (cxx <= 0.0 && cxy >= 0.0) {
	if (opt != 2) {
	    for (iy = ny - 1; iy >= 1; iy--) {
		plt3zz(nx, iy, -1, 1, -opt, init, x, y, z, nx, ny,
		       &work[0], &work[b - 1]);
		init = 0;
	    }
	}
	else {
	    plt3zz(nx, 1, -1, 1, -opt, init, x, y, z, nx, ny,
		   &work[0], &work[b - 1]);
	    init = 0;
	}
	if (opt != 1)
	    for (ix = nx; ix >= 2; ix--)
		plt3zz(ix, 1, -1, 1, opt, init, x, y, z, nx, ny,
		       &work[0], &work[b - 1]);
	else
	    plt3zz(nx, 1, -1, 1, opt, init, x, y, z, nx, ny,
		   &work[0], &work[b - 1]);
    }
    else if (cxx >= 0.0 && cxy >= 0.0) {
	if (opt != 1) {
	    for (ix = nx - 1; ix >= 1; ix--) {
		plt3zz(ix, 1, 1, 1, opt, init, x, y, z, nx, ny,
		       &work[0], &work[b - 1]);
		init = 0;
	    }
	}
	else {
	    plt3zz(1, 1, 1, 1, opt, init, x, y, z, nx, ny,
		   &work[0], &work[b - 1]);
	    init = 0;
	}
	if (opt != 2)
	    for (iy = 1; iy <= ny - 1; iy++)
		plt3zz(1, iy, 1, 1, -opt, init, x, y, z, nx, ny,
		       &work[0], &work[b - 1]);
	else
	    plt3zz(1, 1, 1, 1, -opt, init, x, y, z, nx, ny,
		   &work[0], &work[b - 1]);
    }

    free((VOID *) work);

    if (side)
	plside3(x, y, z, nx, ny, opt);

    if (zbflg) {
	gatt(&font, &color);
	plcol(zbcol);
	plgrid3(zbtck);
	plcol(color);
    }

    free((VOID *) oldhiview);
}

/*----------------------------------------------------------------------*\
* External access routines.
\*----------------------------------------------------------------------*/

void
gzback(PLINT **zbf, PLINT **zbc, PLFLT **zbt)
{
    *zbf = &zbflg;
    *zbc = &zbcol;
    *zbt = &zbtck;
}

/*----------------------------------------------------------------------*\
* void plt3zz()
*
* Draws the next zig-zag line for a 3-d plot. The data is stored in array
* z[][] as a function of x[] and y[]. This is plotted out starting at
* index (xstar0,ystar0).
*
* Depending on the state of "flg0", the sequence of data points
* sent to plnxtv is altered so as to allow cross-hatch plotting,
* or plotting parallel to either the x-axis or the y-axis.
\*----------------------------------------------------------------------*/

static void
plt3zz(PLINT xstar0, PLINT ystar0, PLINT dx, PLINT dy, PLINT flg0, PLINT init,
       PLFLT *x, PLFLT *y, PLFLT **z, PLINT nx, PLINT ny, PLINT *u, PLINT *v)
{
    PLINT flag;
    PLINT n;
    PLINT xstart, ystart;

    n = 0;
    xstart = xstar0;
    ystart = ystar0;
    flag = flg0;

  lab1:
    if (1 <= xstart && xstart <= nx && 1 <= ystart && ystart <= ny) {
	u[n] = wcpcx(w3wcx(
		    x[xstart - 1], y[ystart - 1], z[xstart - 1][ystart - 1]));

	v[n] = wcpcy(w3wcy(
		    x[xstart - 1], y[ystart - 1], z[xstart - 1][ystart - 1]));

	if (flag == -3) {
	    ystart = ystart + dy;
	    flag = -flag;
	}
	else if (flag == -2)
	    ystart = ystart + dy;

	else if (flag == -1) {
	    ystart = ystart + dy;
	    flag = 1;
	}
	else if (flag == 1)
	    xstart = xstart + dx;

	else if (flag == 2) {
	    xstart = xstart + dx;
	    flag = -2;
	}
	else if (flag == 3) {
	    xstart = xstart + dx;
	    flag = -flag;
	}
	n = n + 1;
	goto lab1;
    }

    if (flag == 1 || flag == -2) {
	if (flag == 1) {
	    xstart = xstart - dx;
	    ystart = ystart + dy;
	}
	else if (flag == -2) {
	    ystart = ystart - dy;
	    xstart = xstart + dx;
	}

	if (1 <= xstart && xstart <= nx && 1 <= ystart && ystart <= ny) {
	    u[n] = wcpcx(w3wcx(
		    x[xstart - 1], y[ystart - 1], z[xstart - 1][ystart - 1]));

	    v[n] = wcpcy(w3wcy(
		    x[xstart - 1], y[ystart - 1], z[xstart - 1][ystart - 1]));

	    n = n + 1;
	}
    }
    plnxtv(u, v, n, init);
}

/*----------------------------------------------------------------------*\
* void plside3()
*
* This routine draws sides around the front of the 3d plot so that
* it does not appear to float.
\*----------------------------------------------------------------------*/

static void
plside3(PLFLT *x, PLFLT *y, PLFLT **z, PLINT nx, PLINT ny, PLINT opt)
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

/*----------------------------------------------------------------------*\
* void plgrid3()
*
* This routine draws a grid around the back side of the 3d plot
* with hidden line removal.
\*----------------------------------------------------------------------*/

static void
plgrid3(PLFLT tick)
{
    PLFLT xmin, ymin, zmin, xmax, ymax, zmax, zscale;
    PLFLT cxx, cxy, cyx, cyy, cyz;
    PLINT u[3], v[3];
    PLINT nsub, prec, mode, digmax, digits, scale;
    PLFLT tp;

    gw3wc(&cxx, &cxy, &cyx, &cyy, &cyz);
    gdom(&xmin, &xmax, &ymin, &ymax);
    grange(&zscale, &zmin, &zmax);

    plgzax(&digmax, &digits);
    nsub = 0;

    pldtik(zmin, zmax, &tick, &nsub, &mode, &prec, digmax, &scale);
    tp = tick * floor(zmin / tick) + tick;
    pl3upv = 0;

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
    pl3upv = 1;
}

/*----------------------------------------------------------------------*\
* void plnxtv()
*
* Draw the next view of a 3-d plot. The physical coordinates of
* the points for the next view are placed in the n points of arrays
* u and v. The silhouette found so far is stored in the heap as a
* set of m peak points.
*
* These routines dynamically allocate memory for hidden line removal.
* Memory is allocated in blocks of 2*BINC*sizeof(PLINT) bytes.  Large
* values of BINC give better performance but also allocate more memory
* than is needed. If your 3D plots are very "spiky" or you are working
* with very large matrices then you will probably want to increase BINC.
\*----------------------------------------------------------------------*/

static void
plnxtv(PLINT *u, PLINT *v, PLINT n, PLINT init)
{
    plnxtvhi(u, v, n, init);

    if (pl3mode)
	plnxtvlo(u, v, n, init);
}

/*----------------------------------------------------------------------*\
* void plnxtvhi()
*
* Draw the top side of the 3-d plot.
\*----------------------------------------------------------------------*/

static void
plnxtvhi(PLINT *u, PLINT *v, PLINT n, PLINT init)
{
    PLINT i, j, first;
    PLINT sx1 = 0, sx2 = 0, sy1 = 0, sy2 = 0;
    PLINT su1, su2, sv1, sv2;
    PLINT cx, cy, px, py;
    PLINT seg, ptold, lstold = 0, pthi, pnewhi, newhi, change, ochange = 0;

    first = 1;
    pnewhi = 0;

/*
* For the initial set of points, just display them and store them as the
* peak points.
*/
    if (init == 1) {

/* heap not yet allocated so ... */

	oldhiview = (PLINT *) malloc((size_t) (2 * n * sizeof(PLINT)));
	if (!oldhiview)
	    plexit("plnxtvhi: Out of memory.");

	movphy(u[0], v[0]);
	oldhiview[0] = u[0];
	oldhiview[1] = v[0];
	for (i = 1; i < n; i++) {
	    draphy(u[i], v[i]);
	    oldhiview[2 * i] = u[i];
	    oldhiview[2 * i + 1] = v[i];
	}
	mhi = n;
	return;
    }

/*
* Otherwise, we need to consider hidden-line removal problem. We scan
* over the points in both the old (i.e. oldhiview[]) and new (i.e. u[]
* and v[]) arrays in order of increasing x coordinate.  At each stage, we
* find the line segment in the other array (if one exists) that straddles
* the x coordinate of the point. We have to determine if the point lies
* above or below the line segment, and to check if the below/above status
* has changed since the last point.
*
* If pl3upv = 0 we do not update the view, this is useful for drawing
* lines on the graph after we are done plotting points.  Hidden line
* removal is still done, but the view is not updated.
*/
    xxhi = 0;
    i = 0;
    j = 0;
    if (pl3upv != 0) {
	newhisize = 2 * (mhi + BINC);
	newhiview = (PLINT *) malloc((size_t) (newhisize * sizeof(PLINT)));
	if (!newhiview) {
	    free((VOID *) oldhiview);
	    plexit("plnxtvhi: Out of memory.");
	}
    }

/*
* (oldhiview[2*i], oldhiview[2*i]) is the i'th point in the old array
* (u[j], v[j]) is the j'th point in the new array
*/
    while (i < mhi || j < n) {

/*
* The coordinates of the point under consideration are (px,py).  The line
* segment joins (sx1,sy1) to (sx2,sy2).  "ptold" is true if the point
* lies in the old array. We set it by comparing the x coordinates of the
* i'th old point and the j'th new point, being careful if we have fallen
* past the edges. Having found the point, load up the point and segment
* coordinates appropriately.
*/
	ptold = ((oldhiview[2 * i] < u[j] && i < mhi) || j >= n);
	if (ptold) {
	    px = oldhiview[2 * i];
	    py = oldhiview[2 * i + 1];
	    seg = j > 0 && j < n;
	    if (seg) {
		sx1 = u[j - 1];
		sy1 = v[j - 1];
		sx2 = u[j];
		sy2 = v[j];
	    }
	}
	else {
	    px = u[j];
	    py = v[j];
	    seg = i > 0 && i < mhi;
	    if (seg) {
		sx1 = oldhiview[2 * (i - 1)];
		sy1 = oldhiview[2 * (i - 1) + 1];
		sx2 = oldhiview[2 * i];
		sy2 = oldhiview[2 * i + 1];
	    }
	}

/*
* Now determine if the point is higher than the segment, using the
* logical function "above". We also need to know if it is the old view or
* the new view that is higher. "newhi" is set true if the new view is
* higher than the old.
*/
	if (seg)
	    pthi = plabv(px, py, sx1, sy1, sx2, sy2);
	else
	    pthi = 1;

	newhi = (ptold && !pthi) || (!ptold && pthi);
	change = (newhi && !pnewhi) || (!newhi && pnewhi);

/*
* There is a new intersection point to put in the peak array if the state
* of "newhi" changes.
*/
	if (first) {
	    movphy(px, py);
	    first = 0;
	    lstold = ptold;
	    savehipoint(px, py);
	    pthi = 0;
	    ochange = 0;
	}
	else if (change) {

/*
* Take care of special cases at end of arrays.  If pl3upv is 0 the
* endpoints are not connected to the old view.
*/
	    if (pl3upv == 0 && ((!ptold && j == 0) || (ptold && i == 0))) {
		movphy(px, py);
		lstold = ptold;
		pthi = 0;
		ochange = 0;
	    }
	    else if (pl3upv == 0 && ((!ptold && i >= mhi) || (ptold && j >= n))) {
		movphy(px, py);
		lstold = ptold;
		pthi = 0;
		ochange = 0;
	    }

/*
* If pl3upv is not 0 then we do want to connect the current line with the
* previous view at the endpoints.  Also find intersection point with old
* view.
*/
	    else {
		if (i == 0) {
		    sx1 = oldhiview[0];
		    sy1 = -1;
		    sx2 = oldhiview[0];
		    sy2 = oldhiview[1];
		}
		else if (i >= mhi) {
		    sx1 = oldhiview[2 * (mhi - 1)];
		    sy1 = oldhiview[2 * (mhi - 1) + 1];
		    sx2 = oldhiview[2 * (mhi - 1)];
		    sy2 = -1;
		}
		else {
		    sx1 = oldhiview[2 * (i - 1)];
		    sy1 = oldhiview[2 * (i - 1) + 1];
		    sx2 = oldhiview[2 * i];
		    sy2 = oldhiview[2 * i + 1];
		}

		if (j == 0) {
		    su1 = u[0];
		    sv1 = -1;
		    su2 = u[0];
		    sv2 = v[0];
		}
		else if (j >= n) {
		    su1 = u[n - 1];
		    sv1 = v[n - 1];
		    su2 = u[n];
		    sv2 = -1;
		}
		else {
		    su1 = u[j - 1];
		    sv1 = v[j - 1];
		    su2 = u[j];
		    sv2 = v[j];
		}

/* Determine the intersection */

		pl3cut(sx1, sy1, sx2, sy2, su1, sv1, su2, sv2, &cx, &cy);
		if (cx == px && cy == py) {
		    if (lstold && !ochange)
			movphy(px, py);
		    else
			draphy(px, py);

		    savehipoint(px, py);
		    lstold = 1;
		    pthi = 0;
		}
		else {
		    if (lstold && !ochange)
			movphy(cx, cy);
		    else
			draphy(cx, cy);

		    lstold = 1;
		    savehipoint(cx, cy);
		}
		ochange = 1;
	    }
	}

/* If point is high then draw plot to point and update view. */

	if (pthi) {
	    if (lstold && ptold)
		movphy(px, py);
	    else
		draphy(px, py);

	    savehipoint(px, py);
	    lstold = ptold;
	    ochange = 0;
	}

	pnewhi = newhi;

	if (ptold)
	    i = i + 1;
	else
	    j = j + 1;
    }

/* Set oldhiview */

    swaphiview();
}

/*----------------------------------------------------------------------*\
* void plnxtvlo()
*
* Draw the bottom side of the 3-d plot.
\*----------------------------------------------------------------------*/

static void
plnxtvlo(PLINT *u, PLINT *v, PLINT n, PLINT init)
{
    PLINT i, j, first;
    PLINT sx1 = 0, sx2 = 0, sy1 = 0, sy2 = 0;
    PLINT su1, su2, sv1, sv2;
    PLINT cx, cy, px, py;
    PLINT seg, ptold, lstold = 0, ptlo, pnewlo, newlo, change, ochange = 0;

    first = 1;
    pnewlo = 0;

/*
* For the initial set of points, just display them and store them as the
* peak points.
*/
    if (init == 1) {

/* heap not yet allocated so ... */

	oldloview = (PLINT *) malloc((size_t) (2 * n * sizeof(PLINT)));
	if (!oldloview)
	    plexit("plnxtvlo: Out of memory.");

	movphy(u[0], v[0]);
	oldloview[0] = u[0];
	oldloview[1] = v[0];
	for (i = 1; i < n; i++) {
	    draphy(u[i], v[i]);
	    oldloview[2 * i] = u[i];
	    oldloview[2 * i + 1] = v[i];
	}
	mlo = n;
	return;
    }

/*
* Otherwise, we need to consider hidden-line removal problem. We scan
* over the points in both the old (i.e. oldloview[]) and new (i.e. u[]
* and v[]) arrays in order of increasing x coordinate.  At each stage, we
* find the line segment in the other array (if one exists) that straddles
* the x coordinate of the point. We have to determine if the point lies
* above or below the line segment, and to check if the below/above status
* has changed since the last point.
*
* If pl3upv = 0 we do not update the view, this is useful for drawing
* lines on the graph after we are done plotting points.  Hidden line
* removal is still done, but the view is not updated.
*/
    xxlo = 0;
    i = 0;
    j = 0;
    if (pl3upv != 0) {
	newlosize = 2 * (mlo + BINC);
	newloview = (PLINT *) malloc((size_t) (newlosize * sizeof(PLINT)));
	if (!newloview) {
	    free((VOID *) oldloview);
	    plexit("plnxtvlo: Out of memory.");
	}
    }

/*
* (oldloview[2*i], oldloview[2*i]) is the i'th point in the old array
* (u[j], v[j]) is the j'th point in the new array.
*/
    while (i < mlo || j < n) {

/*
* The coordinates of the point under consideration are (px,py).  The line
* segment joins (sx1,sy1) to (sx2,sy2).  "ptold" is true if the point
* lies in the old array. We set it by comparing the x coordinates of the
* i'th old point and the j'th new point, being careful if we have fallen
* past the edges. Having found the point, load up the point and segment
* coordinates appropriately.
*/
	ptold = ((oldloview[2 * i] < u[j] && i < mlo) || j >= n);
	if (ptold) {
	    px = oldloview[2 * i];
	    py = oldloview[2 * i + 1];
	    seg = j > 0 && j < n;
	    if (seg) {
		sx1 = u[j - 1];
		sy1 = v[j - 1];
		sx2 = u[j];
		sy2 = v[j];
	    }
	}
	else {
	    px = u[j];
	    py = v[j];
	    seg = i > 0 && i < mlo;
	    if (seg) {
		sx1 = oldloview[2 * (i - 1)];
		sy1 = oldloview[2 * (i - 1) + 1];
		sx2 = oldloview[2 * i];
		sy2 = oldloview[2 * i + 1];
	    }
	}

/*
* Now determine if the point is lower than the segment, using the logical
* function "above". We also need to know if it is the old view or the new
* view that is lower. "newlo" is set true if the new view is lower than
* the old.
*/
	if (seg)
	    ptlo = !plabv(px, py, sx1, sy1, sx2, sy2);
	else
	    ptlo = 1;

	newlo = (ptold && !ptlo) || (!ptold && ptlo);
	change = (newlo && !pnewlo) || (!newlo && pnewlo);

/*
* There is a new intersection point to put in the peak array if the state
* of "newlo" changes.
*/
	if (first) {
	    movphy(px, py);
	    first = 0;
	    lstold = ptold;
	    savelopoint(px, py);
	    ptlo = 0;
	    ochange = 0;
	}
	else if (change) {

/*
* Take care of special cases at end of arrays.  If pl3upv is 0 the
* endpoints are not connected to the old view.
*/
	    if (pl3upv == 0 && ((!ptold && j == 0) || (ptold && i == 0))) {
		movphy(px, py);
		lstold = ptold;
		ptlo = 0;
		ochange = 0;
	    }
	    else if (pl3upv == 0 && ((!ptold && i >= mlo) || (ptold && j >= n))) {
		movphy(px, py);
		lstold = ptold;
		ptlo = 0;
		ochange = 0;
	    }

/*
* If pl3upv is not 0 then we do want to connect the current line with the
* previous view at the endpoints.  Also find intersection point with old
* view.
*/
	    else {
		if (i == 0) {
		    sx1 = oldloview[0];
		    sy1 = 100000;
		    sx2 = oldloview[0];
		    sy2 = oldloview[1];
		}
		else if (i >= mlo) {
		    sx1 = oldloview[2 * (mlo - 1)];
		    sy1 = oldloview[2 * (mlo - 1) + 1];
		    sx2 = oldloview[2 * (mlo - 1)];
		    sy2 = 100000;
		}
		else {
		    sx1 = oldloview[2 * (i - 1)];
		    sy1 = oldloview[2 * (i - 1) + 1];
		    sx2 = oldloview[2 * i];
		    sy2 = oldloview[2 * i + 1];
		}

		if (j == 0) {
		    su1 = u[0];
		    sv1 = 100000;
		    su2 = u[0];
		    sv2 = v[0];
		}
		else if (j >= n) {
		    su1 = u[n - 1];
		    sv1 = v[n - 1];
		    su2 = u[n];
		    sv2 = 100000;
		}
		else {
		    su1 = u[j - 1];
		    sv1 = v[j - 1];
		    su2 = u[j];
		    sv2 = v[j];
		}

/* Determine the intersection */

		pl3cut(sx1, sy1, sx2, sy2, su1, sv1, su2, sv2, &cx, &cy);
		if (cx == px && cy == py) {
		    if (lstold && !ochange)
			movphy(px, py);
		    else
			draphy(px, py);

		    savelopoint(px, py);
		    lstold = 1;
		    ptlo = 0;
		}
		else {
		    if (lstold && !ochange)
			movphy(cx, cy);
		    else
			draphy(cx, cy);

		    lstold = 1;
		    savelopoint(cx, cy);
		}
		ochange = 1;
	    }
	}

/* If point is low then draw plot to point and update view. */

	if (ptlo) {
	    if (lstold && ptold)
		movphy(px, py);
	    else
		draphy(px, py);

	    savelopoint(px, py);
	    lstold = ptold;
	    ochange = 0;
	}

	pnewlo = newlo;

	if (ptold)
	    i = i + 1;
	else
	    j = j + 1;
    }

/* Set oldloview */

    swaploview();
}

/*----------------------------------------------------------------------*\
* Utility functions.
\*----------------------------------------------------------------------*/

static void
savehipoint(PLINT px, PLINT py)
{
    PLINT *temp;

    if (pl3upv == 0)
	return;
    if (xxhi >= newhisize) {	/* allocate additional space */
	newhisize += 2 * BINC;
	temp = (PLINT *) realloc((VOID *) newhiview,
				 (size_t) (newhisize * sizeof(PLINT)));
	if (!temp) {
	    free((VOID *) oldhiview);
	    free((VOID *) newhiview);
	    plexit("savehipoint: Out of memory.");
	}
	newhiview = temp;
    }
    newhiview[xxhi] = px;
    xxhi++;
    newhiview[xxhi] = py;
    xxhi++;
}

static void
savelopoint(PLINT px, PLINT py)
{
    PLINT *temp;

    if (pl3upv == 0)
	return;
    if (xxlo >= newlosize) {	/* allocate additional space */
	newlosize += 2 * BINC;
	temp = (PLINT *) realloc((VOID *) newloview,
				 (size_t) (newlosize * sizeof(PLINT)));
	if (!temp) {
	    free((VOID *) oldloview);
	    free((VOID *) newloview);
	    plexit("savelopoint: Out of memory.");
	}
	newloview = temp;
    }
    newloview[xxlo] = px;
    xxlo++;
    newloview[xxlo] = py;
    xxlo++;
}

static void
swaphiview(void)
{
    if (pl3upv != 0) {
	mhi = xxhi / 2;
	free((VOID *) oldhiview);
	oldhiview = newhiview;
    }
}

static void
swaploview(void)
{
    if (pl3upv != 0) {
	mlo = xxlo / 2;
	free((VOID *) oldloview);
	oldloview = newloview;
    }
}

/*----------------------------------------------------------------------*\
* int plabv()
*
* Determines if point (px,py) lies above the line joining (sx1,sy1) to
* (sx2,sy2). It only works correctly if sx1 <= px <= sx2.
\*----------------------------------------------------------------------*/

static int
plabv(PLINT px, PLINT py, PLINT sx1, PLINT sy1, PLINT sx2, PLINT sy2)
{
    PLINT above;

    if (py >= sy1 && py >= sy2)
	above = 1;
    else if (py < sy1 && py < sy2)
	above = 0;
    else if ((PLFLT) (sx2 - sx1) * (py - sy1) >
	     (PLFLT) (px - sx1) * (sy2 - sy1))
	above = 1;
    else
	above = 0;

    return ((PLINT) above);
}

/*----------------------------------------------------------------------*\
* void pl3cut()
*
* Determines the point of intersection (cx,cy) between the line joining
* (sx1,sy1) to (sx2,sy2) and the line joining (su1,sv1) to (su2,sv2).
\*----------------------------------------------------------------------*/

static void
pl3cut(PLINT sx1, PLINT sy1, PLINT sx2, PLINT sy2,
       PLINT su1, PLINT sv1, PLINT su2, PLINT sv2, PLINT *cx, PLINT *cy)
{
    PLINT x21, y21, u21, v21, yv1, xu1, a, b;
    PLFLT fa, fb;

    x21 = sx2 - sx1;
    y21 = sy2 - sy1;
    u21 = su2 - su1;
    v21 = sv2 - sv1;
    yv1 = sy1 - sv1;
    xu1 = sx1 - su1;

    a = x21 * v21 - y21 * u21;
    fa = (PLFLT) a;
    if (a == 0) {
	if (sx2 < su2) {
	    *cx = sx2;
	    *cy = sy2;
	}
	else {
	    *cx = su2;
	    *cy = sv2;
	}
	return;
    }
    else {
	b = yv1 * u21 - xu1 * v21;
	fb = (PLFLT) b;
	*cx = (PLINT) (sx1 + (fb * x21) / fa + .5);
	*cy = (PLINT) (sy1 + (fb * y21) / fa + .5);
    }
}
