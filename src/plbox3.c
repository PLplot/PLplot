/* $Id$
   $Log$
   Revision 1.2  1992/09/29 04:45:46  furnish
   Massive clean up effort to remove support for garbage compilers (K&R).

 * Revision 1.1  1992/05/20  21:34:15  furnish
 * Initial checkin of the whole PLPLOT project.
 *
*/

/*	plbox3.c

	Draws axes and axis labels for 3-d plots.
*/

#include "plplot.h"

void
c_plbox3 (char *xopt, char *xlabel, PLFLT xtick, PLINT nsubx, 
	  char *yopt, char *ylabel, PLFLT ytick, PLINT nsuby, 
	  char *zopt, char *zlabel, PLFLT ztick, PLINT nsubz)
{
    PLFLT dx, dy, tx, ty, ux, uy;
    PLFLT xmin, xmax, ymin, ymax, zmin, zmax, zscale;
    PLFLT cxx, cxy, cyx, cyy, cyz;
    PLINT ln, level, font;
    PLINT *zbflg, *zbcol;
    PLFLT *zbtck;
    PLINT xdigmax, xdigits;
    PLINT ydigmax, ydigits;
    PLINT zdigmax, zdigits;

    glev(&level);
    if (level < 3)
	plexit("Please set up window before calling plbox3");

    gw3wc(&cxx, &cxy, &cyx, &cyy, &cyz);
    gdom(&xmin, &xmax, &ymin, &ymax);
    grange(&zscale, &zmin, &zmax);

    plgxax(&xdigmax, &xdigits);
    plgyax(&ydigmax, &ydigits);
    plgzax(&zdigmax, &zdigits);

    xdigits = xdigmax;
    ydigits = ydigmax;
    zdigits = zdigmax;

/* We have to wait until after the plot is drawn to draw back */
/* grid so store this stuff. */

    gzback(&zbflg, &zbcol, &zbtck);
    *zbflg = stsearch(zopt, 'd');
    if (*zbflg) {
	/* save tick spacing and color */
	*zbtck = ztick;
	gatt(&font, zbcol);
    }

    if (cxx >= 0.0 && cxy <= 0.0) {
	ln = stsearch(xopt, 'n');
	tx = w3wcx(xmin, ymin, zmin);
	ty = w3wcy(xmin, ymin, zmin);
	ux = w3wcx(xmax, ymin, zmin);
	uy = w3wcy(xmax, ymin, zmin);
	plxybx(xopt, xlabel, tx, ty, ux, uy, 
		xmin, xmax, xtick, nsubx, 0, &xdigits);

	dx = ux - tx;
	dy = uy - ty;
	plzbx(zopt, zlabel, 1, dx, dy, ux, uy,
		w3wcy(xmax, ymin, zmax), zmin, zmax, ztick, nsubz, &zdigits);

	tx = w3wcx(xmin, ymax, zmin);
	ty = w3wcy(xmin, ymax, zmin);
	ux = w3wcx(xmin, ymin, zmin);
	uy = w3wcy(xmin, ymin, zmin);
	plxybx(yopt, ylabel, tx, ty, ux, uy, 
		ymax, ymin, ytick, nsuby, ln, &ydigits);

	dx = ux - tx;
	dy = uy - ty;
	plzbx(zopt, zlabel, 0, dx, dy, tx, ty,
	      w3wcy(xmin, ymax, zmax), zmin, zmax, ztick, nsubz, &zdigits);
    }
    else if (cxx <= 0.0 && cxy <= 0.0) {
	ln = stsearch(yopt, 'n');
	tx = w3wcx(xmin, ymax, zmin);
	ty = w3wcy(xmin, ymax, zmin);
	ux = w3wcx(xmin, ymin, zmin);
	uy = w3wcy(xmin, ymin, zmin);
	plxybx(yopt, ylabel, tx, ty, ux, uy, 
		ymax, ymin, ytick, nsuby, 0, &ydigits);

	dx = ux - tx;
	dy = uy - ty;
	plzbx(zopt, zlabel, 1, dx, dy, ux, uy,
		w3wcy(xmin, ymin, zmax), zmin, zmax, ztick, nsubz, &zdigits);

	tx = w3wcx(xmax, ymax, zmin);
	ty = w3wcy(xmax, ymax, zmin);
	ux = w3wcx(xmin, ymax, zmin);
	uy = w3wcy(xmin, ymax, zmin);
	plxybx(xopt, xlabel, tx, ty, ux, uy, 
		xmax, xmin, xtick, nsubx, ln, &xdigits);

	dx = ux - tx;
	dy = uy - ty;
	plzbx(zopt, zlabel, 0, dx, dy, tx, ty,
		w3wcy(xmax, ymax, zmax), zmin, zmax, ztick, nsubz, &zdigits);
    }
    else if (cxx <= 0.0 && cxy >= 0.0) {
	ln = stsearch(xopt, 'n');
	tx = w3wcx(xmax, ymax, zmin);
	ty = w3wcy(xmax, ymax, zmin);
	ux = w3wcx(xmin, ymax, zmin);
	uy = w3wcy(xmin, ymax, zmin);
	plxybx(xopt, xlabel, tx, ty, ux, uy, 
		xmax, xmin, xtick, nsubx, 0, &xdigits);

	dx = ux - tx;
	dy = uy - ty;
	plzbx(zopt, zlabel, 1, dx, dy, ux, uy,
		w3wcy(xmin, ymax, zmax), zmin, zmax, ztick, nsubz, &zdigits);

	tx = w3wcx(xmax, ymin, zmin);
	ty = w3wcy(xmax, ymin, zmin);
	ux = w3wcx(xmax, ymax, zmin);
	uy = w3wcy(xmax, ymax, zmin);
	plxybx(yopt, ylabel, tx, ty, ux, uy, 
		ymin, ymax, ytick, nsuby, ln, &ydigits);

	dx = ux - tx;
	dy = uy - ty;
	plzbx(zopt, zlabel, 0, dx, dy, tx, ty,
		w3wcy(xmax, ymin, zmax), zmin, zmax, ztick, nsubz, &zdigits);
    }
    else if (cxx >= 0.0 && cxy >= 0.0) {
	ln = stsearch(yopt, 'n');
	tx = w3wcx(xmax, ymin, zmin);
	ty = w3wcy(xmax, ymin, zmin);
	ux = w3wcx(xmax, ymax, zmin);
	uy = w3wcy(xmax, ymax, zmin);
	plxybx(yopt, ylabel, tx, ty, ux, uy, 
		ymin, ymax, ytick, nsuby, 0, &ydigits);

	dx = ux - tx;
	dy = uy - ty;
	plzbx(zopt, zlabel, 1, dx, dy, ux, uy,
		w3wcy(xmax, ymax, zmax), zmin, zmax, ztick, nsubz, &zdigits);

	tx = w3wcx(xmin, ymin, zmin);
	ty = w3wcy(xmin, ymin, zmin);
	ux = w3wcx(xmax, ymin, zmin);
	uy = w3wcy(xmax, ymin, zmin);
	plxybx(xopt, xlabel, tx, ty, ux, uy, 
		xmin, xmax, xtick, nsubx, ln, &xdigits);

	dx = ux - tx;
	dy = uy - ty;
	plzbx(zopt, zlabel, 0, dx, dy, tx, ty,
		w3wcy(xmin, ymin, zmax), zmin, zmax, ztick, nsubz, &zdigits);
    }
    plsxax(xdigmax, xdigits);
    plsyax(ydigmax, ydigits);
    plszax(zdigmax, zdigits);
}
