/* $Id$
 * $Log$
 * Revision 1.17  1995/01/29 07:04:18  mjl
 * Fix to increase spacing between inverted tics and numeric labels; in some
 * cases they were running into each other.  Contributed by John Interrante.
 *
 * Revision 1.16  1995/01/06  07:54:16  mjl
 * Added new options:
 *   f: Always use fixed point numeric labels
 *   h: Draws a grid at the minor tick interval
 * See x04c for an example of both of these (second plot).
 * Upgraded plaxes() to be a perfect superset of plbox() (now plbox is
 * just a front-end to plaxes).  Substantially improved code structure and
 * eliminated some inconsistencies.
 *
 * Revision 1.15  1995/01/04  06:50:44  mjl
 * Fix for obscure 3d plotting bug, contributed by Bryan Peterson.
 *
 * Revision 1.14  1994/08/29  22:05:19  mjl
 * Fixed a bug that was preventing the exponential label from showing up
 * under certain circumstances.
*/

/*	plbox.c

	Routines for drawing axes & box around the current viewport.
*/

#include "plplotP.h"

static PLFLT xlog[8] =
{
    0.301030, 0.477121, 0.602060, 0.698970,
    0.778151, 0.845098, 0.903090, 0.954243
};

/* Static function prototypes */

static void
plxybx(const char *opt, const char *label, PLFLT wx1, PLFLT wy1,
       PLFLT wx2, PLFLT wy2, PLFLT vmin, PLFLT vmax,
       PLFLT tick, PLINT nsub, PLINT nolast, PLINT *digits);

static void
plzbx(const char *opt, const char *label, PLINT right, PLFLT dx, PLFLT dy,
      PLFLT wx, PLFLT wy1, PLFLT wy2, PLFLT vmin, PLFLT vmax,
      PLFLT tick, PLINT nsub, PLINT *digits);

static void
plxytx(PLFLT wx1, PLFLT wy1, PLFLT wx2, PLFLT wy2,
       PLFLT disp, PLFLT pos, PLFLT just, const char *text);

static void
plztx(const char *opt, PLFLT dx, PLFLT dy, PLFLT wx, PLFLT wy1,
      PLFLT wy2, PLFLT disp, PLFLT pos, PLFLT just, const char *text);

static void
plform(PLFLT value, PLINT scale, PLINT prec, char *result, PLINT ll, PLINT lf);

static void
grid_box(const char *xopt, PLFLT xtick1, PLINT nxsub1,
	 const char *yopt, PLFLT ytick1, PLINT nysub1);

static void
label_box(const char *xopt, PLFLT xtick1, const char *yopt, PLFLT ytick1);

/*----------------------------------------------------------------------*\
 * void plbox()
 *
 * This draws a box around the current viewport, complete with axes,
 * ticks, numeric labels, and grids, according to input specification.
 * Just a front-end to plaxes(), which allows arbitrary placement of
 * coordinate axes when plotted (here the origin is at 0,0).  See the
 * documentation for plaxes() for more info.
\*----------------------------------------------------------------------*/

void
c_plbox(const char *xopt, PLFLT xtick, PLINT nxsub,
	const char *yopt, PLFLT ytick, PLINT nysub)
{
    c_plaxes(0.0, 0.0, xopt, xtick, nxsub, yopt, ytick, nysub);
}

/*----------------------------------------------------------------------*\
 * void plaxes()
 *
 * This draws a box around the current viewport, complete with axes,
 * ticks, numeric labels, and grids, according to input specification.
 *
 * x0 and y0 specify the origin of the axes.
 *
 * xopt and yopt are character strings which define the box as follows:
 *
 * a: Draw axis (X is horizontal line Y=0, Y is vertical line X=0)
 * b: Draw bottom (X) or left (Y) edge of frame
 * c: Draw top (X) or right (Y) edge of frame
 * f: Always use fixed point numeric labels
 * g: Draws a grid at the major tick interval
 * h: Draws a grid at the minor tick interval
 * i: Inverts tick marks
 * l: Logarithmic axes, major ticks at decades, minor ticks at units
 * n: Write numeric label at conventional location
 * m: Write numeric label at unconventional location
 * t: Draw major tick marks
 * s: Draw minor tick marks
 * v: (for Y only) Label vertically
 *
 * xtick, ytick are the major tick intervals required, zero for
 * automatic selection
 *
 * nxsub, nysub are the number of subtick intervals in a major tick
 * interval
\*----------------------------------------------------------------------*/

void
c_plaxes(PLFLT x0, PLFLT y0,
	 const char *xopt, PLFLT xtick, PLINT nxsub,
	 const char *yopt, PLFLT ytick, PLINT nysub)
{
    static char string[40];
    PLINT lax, lbx, lcx, lgx, lix, llx, lsx, ltx;
    PLINT lay, lby, lcy, lgy, liy, lly, lsy, lty;
    PLINT xmajor, xminor, ymajor, yminor;
    PLINT i, i1x, i2x, i3x, i4x, i1y, i2y, i3y, i4y;
    PLINT nxsub1, nysub1;
    PLINT lxmin, lxmax, lymin, lymax;
    PLINT pxmin, pxmax, pymin, pymax;
    PLINT vppxmi, vppxma, vppymi, vppyma;
    PLFLT xtick1, ytick1, vpwxmi, vpwxma, vpwymi, vpwyma;
    PLFLT xp0, yp0, pos, tn, tp, temp, offset, height;

    if (plsc->level < 3) {
	plabort("plbox: Please set up window first");
	return;
    }

/* Open the clip limits to the subpage limits */

    plP_gclp(&lxmin, &lxmax, &lymin, &lymax);
    plP_gphy(&pxmin, &pxmax, &pymin, &pymax);
    plP_sclp(pxmin, pxmax, pymin, pymax);

    plP_gvpp(&vppxmi, &vppxma, &vppymi, &vppyma);

/* Convert world coordinates to physical */

    xp0 = plP_wcpcx(x0);
    yp0 = plP_wcpcy(y0);

/* Set plot options from input */

    lax = plP_stsearch(xopt, 'a');
    lbx = plP_stsearch(xopt, 'b');
    lcx = plP_stsearch(xopt, 'c');
    lgx = plP_stsearch(xopt, 'g');
    lix = plP_stsearch(xopt, 'i');
    llx = plP_stsearch(xopt, 'l');
    lsx = plP_stsearch(xopt, 's');
    ltx = plP_stsearch(xopt, 't');

    lay = plP_stsearch(yopt, 'a');
    lby = plP_stsearch(yopt, 'b');
    lcy = plP_stsearch(yopt, 'c');
    lgy = plP_stsearch(yopt, 'g');
    liy = plP_stsearch(yopt, 'i');
    lly = plP_stsearch(yopt, 'l');
    lsy = plP_stsearch(yopt, 's');
    lty = plP_stsearch(yopt, 't');

/* Tick and subtick sizes in device coords */

    xmajor = MAX(ROUND(plsc->majht * plsc->ypmm), 1);
    ymajor = MAX(ROUND(plsc->majht * plsc->xpmm), 1);
    xminor = MAX(ROUND(plsc->minht * plsc->ypmm), 1);
    yminor = MAX(ROUND(plsc->minht * plsc->xpmm), 1);

    nxsub1 = nxsub;
    nysub1 = nysub;
    xtick1 = llx ? 1.0 : xtick;
    ytick1 = lly ? 1.0 : ytick;

    plP_gvpw(&vpwxmi, &vpwxma, &vpwymi, &vpwyma);

    lax = lax && (vpwymi * vpwyma < 0.0) && !llx;
    lay = lay && (vpwxmi * vpwxma < 0.0) && !lly;

/* Calculate tick spacing */

    if (ltx || lgx) 
	pldtik(vpwxmi, vpwxma, &xtick1, &nxsub1);

    if (lty || lgy) 
	pldtik(vpwymi, vpwyma, &ytick1, &nysub1);

/* Set up tick variables */

    if (lix) {
	i1x = xminor;
	i2x = 0;
	i3x = xmajor;
	i4x = 0;
    }
    else {
	i1x = 0;
	i2x = xminor;
	i3x = 0;
	i4x = xmajor;
    }

    if (liy) {
	i1y = yminor;
	i2y = 0;
	i3y = ymajor;
	i4y = 0;
    }
    else {
	i1y = 0;
	i2y = yminor;
	i3y = 0;
	i4y = ymajor;
    }

/* Draw the bottom edge of the box */

    if (lbx) {
	plP_movphy(vppxmi, vppymi);
	if (ltx) {
	    tp = xtick1 * floor(vpwxmi / xtick1);
	    for (;;) {
		tn = tp + xtick1;
		if (lsx) {
		    if (llx) {
			for (i = 0; i <= 7; i++) {
			    temp = tp + xlog[i];
			    if (BETW(temp, vpwxmi, vpwxma))
				plxtik(plP_wcpcx(temp), vppymi, i1x, i2x);
			}
		    }
		    else {
			for (i = 1; i <= nxsub1 - 1; i++) {
			    temp = tp + i * xtick1 / nxsub1;
			    if (BETW(temp, vpwxmi, vpwxma))
				plxtik(plP_wcpcx(temp), vppymi, i1x, i2x);
			}
		    }
		}
		if (!BETW(tn, vpwxmi, vpwxma))
		    break;
		plxtik(plP_wcpcx(tn), vppymi, i3x, i4x);
		tp = tn;
	    }
	}
	plP_draphy(vppxma, vppymi);
    }

/* Draw right-hand edge of box */

    if (lcy) {
	plP_movphy(vppxma, vppymi);
	if (lty) {
	    tp = ytick1 * floor(vpwymi / ytick1);
	    for (;;) {
		tn = tp + ytick1;
		if (lsy) {
		    if (lly) {
			for (i = 0; i <= 7; i++) {
			    temp = tp + xlog[i];
			    if (BETW(temp, vpwymi, vpwyma))
				plytik(vppxma, plP_wcpcy(temp), i2y, i1y);
			}
		    }
		    else {
			for (i = 1; i <= nysub1 - 1; i++) {
			    temp = tp + i * ytick1 / nysub1;
			    if (BETW(temp, vpwymi, vpwyma))
				plytik(vppxma, plP_wcpcy(temp), i2y, i1y);
			}
		    }
		}
		if (!BETW(tn, vpwymi, vpwyma))
		    break;
		plytik(vppxma, plP_wcpcy(tn), i4y, i3y);
		tp = tn;
	    }
	}
	plP_draphy(vppxma, vppyma);
    }

/* Draw the top edge of the box */

    if (lcx) {
	plP_movphy(vppxma, vppyma);
	if (ltx) {
	    tp = xtick1 * (floor(vpwxma / xtick1) + 1);
	    for (;;) {
		tn = tp - xtick1;
		if (lsx) {
		    if (llx) {
			for (i = 7; i >= 0; i--) {
			    temp = tn + xlog[i];
			    if (BETW(temp, vpwxmi, vpwxma))
				plxtik(plP_wcpcx(temp), vppyma, i2x, i1x);
			}
		    }
		    else {
			for (i = nxsub1 - 1; i >= 1; i--) {
			    temp = tn + i * xtick1 / nxsub1;
			    if (BETW(temp, vpwxmi, vpwxma))
				plxtik(plP_wcpcx(temp), vppyma, i2x, i1x);
			}
		    }
		}
		if (!BETW(tn, vpwxmi, vpwxma))
		    break;
		plxtik(plP_wcpcx(tn), vppyma, i4x, i3x);
		tp = tn;
	    }
	}
	plP_draphy(vppxmi, vppyma);
    }

/* Draw left-hand edge of box */

    if (lby) {
	plP_movphy(vppxmi, vppyma);
	if (lty) {
	    tp = ytick1 * (floor(vpwyma / ytick1) + 1);
	    for (;;) {
		tn = tp - ytick1;
		if (lsy) {
		    if (lly) {
			for (i = 7; i >= 0; i--) {
			    temp = tn + xlog[i];
			    if (BETW(temp, vpwymi, vpwyma))
				plytik(vppxmi, plP_wcpcy(temp), i1y, i2y);
			}
		    }
		    else {
			for (i = nysub1 - 1; i >= 1; i--) {
			    temp = tn + i * ytick1 / nysub1;
			    if (BETW(temp, vpwymi, vpwyma))
				plytik(vppxmi, plP_wcpcy(temp), i1y, i2y);
			}
		    }
		}
		if (!BETW(tn, vpwymi, vpwyma))
		    break;
		plytik(vppxmi, plP_wcpcy(tn), i3y, i4y);
		tp = tn;
	    }
	}
	plP_draphy(vppxmi, vppymi);
    }

/* Draw the horizontal axis */

    if (lax) {
	plP_movphy(vppxmi, yp0);
	if (ltx) {
	    tp = xtick1 * floor(vpwxmi / xtick1);
	    for (;;) {
		tn = tp + xtick1;
		if (lsx) {
		    if (llx) {
			for (i = 0; i <= 7; i++) {
			    temp = tp + xlog[i];
			    if (BETW(temp, vpwxmi, vpwxma))
				plxtik(plP_wcpcx(temp), yp0, xminor, xminor);
			}
		    }
		    else {
			for (i = 1; i <= nxsub1 - 1; i++) {
			    temp = tp + i * xtick1 / nxsub1;
			    if (BETW(temp, vpwxmi, vpwxma))
				plxtik(plP_wcpcx(temp), yp0, xminor, xminor);
			}
		    }
		}
		if (!BETW(tn, vpwxmi, vpwxma))
		    break;
		plxtik(plP_wcpcx(tn), yp0, xmajor, xmajor);
		tp = tn;
	    }
	}
	plP_draphy(vppxma, yp0);
    }

/* Draw the vertical axis */

    if (lay) {
	plP_movphy(xp0, vppymi);
	if (lty) {
	    tp = ytick1 * floor(vpwymi / ytick1);
	    for (;;) {
		tn = tp + ytick1;
		if (lsy) {
		    if (lly) {
			for (i = 0; i <= 7; i++) {
			    temp = tp + xlog[i];
			    if (BETW(temp, vpwymi, vpwyma))
				plytik(xp0, plP_wcpcy(temp), yminor, yminor);
			}
		    }
		    else {
			for (i = 1; i <= nysub1 - 1; i++) {
			    temp = tp + i * ytick1 / nysub1;
			    if (BETW(temp, vpwymi, vpwyma))
				plytik(xp0, plP_wcpcy(temp), yminor, yminor);
			}
		    }
		}
		if (!BETW(tn, vpwymi, vpwyma))
		    break;
		plytik(xp0, plP_wcpcy(tn), ymajor, ymajor);
		tp = tn;
	    }
	}
	plP_draphy(xp0, vppyma);
    }

/* Draw grids */

    grid_box(xopt, xtick1, nxsub1, yopt, ytick1, nysub1);

/* Write labels */

    label_box(xopt, xtick1, yopt, ytick1);

/* Restore the clip limits to viewport edge */

    plP_sclp(lxmin, lxmax, lymin, lymax);
}

/*----------------------------------------------------------------------*\
 * void plbox3()
 *
 * This is the 3-d analogue of plbox().
\*----------------------------------------------------------------------*/

void
c_plbox3(const char *xopt, const char *xlabel, PLFLT xtick, PLINT nsubx,
	 const char *yopt, const char *ylabel, PLFLT ytick, PLINT nsuby,
	 const char *zopt, const char *zlabel, PLFLT ztick, PLINT nsubz)
{
    PLFLT dx, dy, tx, ty, ux, uy;
    PLFLT xmin, xmax, ymin, ymax, zmin, zmax, zscale;
    PLFLT cxx, cxy, cyx, cyy, cyz;
    PLINT ln, font;
    PLINT *zbflg, *zbcol;
    PLFLT *zbtck;
    PLINT xdigmax, xdigits;
    PLINT ydigmax, ydigits;
    PLINT zdigmax, zdigits;

    if (plsc->level < 3) {
	plabort("plbox3: Please set up window first");
	return;
    }

    plP_gw3wc(&cxx, &cxy, &cyx, &cyy, &cyz);
    plP_gdom(&xmin, &xmax, &ymin, &ymax);
    plP_grange(&zscale, &zmin, &zmax);

    plgxax(&xdigmax, &xdigits);
    plgyax(&ydigmax, &ydigits);
    plgzax(&zdigmax, &zdigits);

    xdigits = xdigmax;
    ydigits = ydigmax;
    zdigits = zdigmax;

/* We have to wait until after the plot is drawn to draw back */
/* grid so store this stuff. */

    plP_gzback(&zbflg, &zbcol, &zbtck);
    *zbflg = plP_stsearch(zopt, 'd');
    if (*zbflg) {
	*zbtck = ztick;
	plP_gatt(&font, zbcol);	/* save tick spacing and color */
    }

    if (cxx >= 0.0 && cxy <= 0.0) {
	ln = plP_stsearch(xopt, 'n');
	tx = plP_w3wcx(xmin, ymin, zmin);
	ty = plP_w3wcy(xmin, ymin, zmin);
	ux = plP_w3wcx(xmax, ymin, zmin);
	uy = plP_w3wcy(xmax, ymin, zmin);
	plxybx(xopt, xlabel, tx, ty, ux, uy,
	       xmin, xmax, xtick, nsubx, 0, &xdigits);

	dx = ux - tx;
	dy = uy - ty;
	plzbx(zopt, zlabel, 1, dx, dy, ux, uy,
	      plP_w3wcy(xmax, ymin, zmax), zmin, zmax, ztick, nsubz, &zdigits);

	tx = plP_w3wcx(xmin, ymax, zmin);
	ty = plP_w3wcy(xmin, ymax, zmin);
	ux = plP_w3wcx(xmin, ymin, zmin);
	uy = plP_w3wcy(xmin, ymin, zmin);
	plxybx(yopt, ylabel, tx, ty, ux, uy,
	       ymax, ymin, ytick, nsuby, ln, &ydigits);

	dx = ux - tx;
	dy = uy - ty;
	plzbx(zopt, zlabel, 0, dx, dy, tx, ty,
	      plP_w3wcy(xmin, ymax, zmax), zmin, zmax, ztick, nsubz, &zdigits);
    }
    else if (cxx <= 0.0 && cxy <= 0.0) {
	ln = plP_stsearch(yopt, 'n');
	tx = plP_w3wcx(xmin, ymax, zmin);
	ty = plP_w3wcy(xmin, ymax, zmin);
	ux = plP_w3wcx(xmin, ymin, zmin);
	uy = plP_w3wcy(xmin, ymin, zmin);
	plxybx(yopt, ylabel, tx, ty, ux, uy,
	       ymax, ymin, ytick, nsuby, 0, &ydigits);

	dx = ux - tx;
	dy = uy - ty;
	plzbx(zopt, zlabel, 1, dx, dy, ux, uy,
	      plP_w3wcy(xmin, ymin, zmax), zmin, zmax, ztick, nsubz, &zdigits);

	tx = plP_w3wcx(xmax, ymax, zmin);
	ty = plP_w3wcy(xmax, ymax, zmin);
	ux = plP_w3wcx(xmin, ymax, zmin);
	uy = plP_w3wcy(xmin, ymax, zmin);
	plxybx(xopt, xlabel, tx, ty, ux, uy,
	       xmax, xmin, xtick, nsubx, ln, &xdigits);

	dx = ux - tx;
	dy = uy - ty;
	plzbx(zopt, zlabel, 0, dx, dy, tx, ty,
	      plP_w3wcy(xmax, ymax, zmax), zmin, zmax, ztick, nsubz, &zdigits);
    }
    else if (cxx <= 0.0 && cxy >= 0.0) {
	ln = plP_stsearch(xopt, 'n');
	tx = plP_w3wcx(xmax, ymax, zmin);
	ty = plP_w3wcy(xmax, ymax, zmin);
	ux = plP_w3wcx(xmin, ymax, zmin);
	uy = plP_w3wcy(xmin, ymax, zmin);
	plxybx(xopt, xlabel, tx, ty, ux, uy,
	       xmax, xmin, xtick, nsubx, 0, &xdigits);

	dx = ux - tx;
	dy = uy - ty;
	plzbx(zopt, zlabel, 1, dx, dy, ux, uy,
	      plP_w3wcy(xmin, ymax, zmax), zmin, zmax, ztick, nsubz, &zdigits);

	tx = plP_w3wcx(xmax, ymin, zmin);
	ty = plP_w3wcy(xmax, ymin, zmin);
	ux = plP_w3wcx(xmax, ymax, zmin);
	uy = plP_w3wcy(xmax, ymax, zmin);
	plxybx(yopt, ylabel, tx, ty, ux, uy,
	       ymin, ymax, ytick, nsuby, ln, &ydigits);

	dx = ux - tx;
	dy = uy - ty;
	plzbx(zopt, zlabel, 0, dx, dy, tx, ty,
	      plP_w3wcy(xmax, ymin, zmax), zmin, zmax, ztick, nsubz, &zdigits);
    }
    else if (cxx >= 0.0 && cxy >= 0.0) {
	ln = plP_stsearch(yopt, 'n');
	tx = plP_w3wcx(xmax, ymin, zmin);
	ty = plP_w3wcy(xmax, ymin, zmin);
	ux = plP_w3wcx(xmax, ymax, zmin);
	uy = plP_w3wcy(xmax, ymax, zmin);
	plxybx(yopt, ylabel, tx, ty, ux, uy,
	       ymin, ymax, ytick, nsuby, 0, &ydigits);

	dx = ux - tx;
	dy = uy - ty;
	plzbx(zopt, zlabel, 1, dx, dy, ux, uy,
	      plP_w3wcy(xmax, ymax, zmax), zmin, zmax, ztick, nsubz, &zdigits);

	tx = plP_w3wcx(xmin, ymin, zmin);
	ty = plP_w3wcy(xmin, ymin, zmin);
	ux = plP_w3wcx(xmax, ymin, zmin);
	uy = plP_w3wcy(xmax, ymin, zmin);
	plxybx(xopt, xlabel, tx, ty, ux, uy,
	       xmin, xmax, xtick, nsubx, ln, &xdigits);

	dx = ux - tx;
	dy = uy - ty;
	plzbx(zopt, zlabel, 0, dx, dy, tx, ty,
	      plP_w3wcy(xmin, ymin, zmax), zmin, zmax, ztick, nsubz, &zdigits);
    }
    plsxax(xdigmax, xdigits);
    plsyax(ydigmax, ydigits);
    plszax(zdigmax, zdigits);
}

/*----------------------------------------------------------------------*\
 * Support routines for 3d box draw.
\*----------------------------------------------------------------------*/

/*----------------------------------------------------------------------*\
 * void plxybx()
 *
 * This draws a sloping line from (wx1,wy1) to (wx2,wy2) which represents an
 * axis of a 3-d graph with data values from "vmin" to "vmax". Depending on
 * "opt", vertical ticks and/or subticks are placed on the line at major tick
 * interval "tick" with "nsub" subticks between major ticks. If "tick" and/or
 * "nsub" is zero, automatic tick positions are computed
 *
 * b: Draw box boundary
 * f: Always use fixed point numeric labels
 * i: Inverts tick marks (i.e. drawn downwards)
 * l: Logarithmic axes, major ticks at decades, minor ticks at units
 * n: Write numeric label
 * t: Draw major tick marks
 * s: Draw minor tick marks
 * u: Write label on line
\*----------------------------------------------------------------------*/

static void
plxybx(const char *opt, const char *label, PLFLT wx1, PLFLT wy1,
       PLFLT wx2, PLFLT wy2, PLFLT vmin, PLFLT vmax,
       PLFLT tick, PLINT nsub, PLINT nolast, PLINT *digits)
{
    static char string[40];
    PLINT lb, lf, li, ll, ln, ls, lt, lu;
    PLINT major, minor, mode, prec, scale;
    PLINT i, i1, i2, i3, i4;
    PLINT nsub1;
    PLFLT pos, tn, tp, temp, height, tick1;
    PLFLT dwx, dwy, lambda;

    dwx = wx2 - wx1;
    dwy = wy2 - wy1;

/* Tick and subtick sizes in device coords */

    major = MAX(ROUND(plsc->majht * plsc->ypmm), 1);
    minor = MAX(ROUND(plsc->minht * plsc->ypmm), 1);

    tick1 = tick;
    nsub1 = nsub;

    lb = plP_stsearch(opt, 'b');
    lf = plP_stsearch(opt, 'f');
    li = plP_stsearch(opt, 'i');
    ll = plP_stsearch(opt, 'l');
    ln = plP_stsearch(opt, 'n');
    ls = plP_stsearch(opt, 's');
    lt = plP_stsearch(opt, 't');
    lu = plP_stsearch(opt, 'u');

    if (lu)
	plxytx(wx1, wy1, wx2, wy2, 3.2, 0.5, 0.5, label);
    if (!lb)
	return;

    if (ll)
	tick1 = (vmax > vmin) ? (PLFLT) 1.0 : (PLFLT) -1.0 ;
    if (lt)
	pldtik(vmin, vmax, &tick1, &nsub1);

    if (li) {
	i1 = minor;
	i2 = 0;
	i3 = major;
	i4 = 0;
    }
    else {
	i1 = 0;
	i2 = minor;
	i3 = 0;
	i4 = major;
    }

/* Draw the line */

    plP_movwor(wx1, wy1);
    if (lt) {
	tp = tick1 * floor(vmin / tick1);
	for (;;) {
	    tn = tp + tick1;
	    if (ls) {
		if (ll) {
		    for (i = 0; i <= 7; i++) {
			temp = tp + xlog[i];
			if (BETW(temp, vmin, vmax)) {
			    lambda = (temp - vmin) / (vmax - vmin);
			    plxtik(plP_wcpcx((PLFLT) (wx1 + lambda * dwx)),
				   plP_wcpcy((PLFLT) (wy1 + lambda * dwy)),
				   i1, i2);
			}
		    }
		}
		else {
		    for (i = 1; i <= nsub1 - 1; i++) {
			temp = tp + i * (tn - tp) / nsub1;
			if (BETW(temp, vmin, vmax)) {
			    lambda = (temp - vmin) / (vmax - vmin);
			    plxtik(plP_wcpcx((PLFLT) (wx1 + lambda * dwx)),
				   plP_wcpcy((PLFLT) (wy1 + lambda * dwy)),
				   i1, i2);
			}
		    }
		}
	    }
	    temp = tn;
	    if (!BETW(temp, vmin, vmax))
		break;

	    lambda = (temp - vmin) / (vmax - vmin);
	    plxtik(plP_wcpcx((PLFLT) (wx1 + lambda * dwx)),
		   plP_wcpcy((PLFLT) (wy1 + lambda * dwy)), i3, i4);
	    tp = tn;
	}
    }

    plP_drawor(wx2, wy2);

/* Label the line */

    if (ln && lt) {
	pldprec(vmin, vmax, tick1, lf, &mode, &prec, *digits, &scale);
	pos = 1.0;
	height = 3.2;
	tp = tick1 * (1. + floor(vmin / tick1));
	for (tn = tp; BETW(tn, vmin, vmax); tn += tick1) {
	    if (nolast && !BETW(tn + tick1, vmin, vmax)) {
		pos = 0.8;
		break;
	    }
	    plform(tn, scale, prec, string, ll, lf);
	    pos = (tn - vmin) / (vmax - vmin);
	    plxytx(wx1, wy1, wx2, wy2, 1.5, pos, 0.5, string);
	}
	*digits = 2;
	if (!ll && mode) {
	    sprintf(string, "(x10#u%d#d)", (int) scale);
	    plxytx(wx1, wy1, wx2, wy2, height, pos, 0.5, string);
	}
    }
}

/*----------------------------------------------------------------------*\
 * void plxytx()
 *
 * Prints out text along a sloping axis joining world coordinates
 * (wx1,wy1) to (wx2,wy2). Parameters are as for plmtext.
\*----------------------------------------------------------------------*/

static void
plxytx(PLFLT wx1, PLFLT wy1, PLFLT wx2, PLFLT wy2,
       PLFLT disp, PLFLT pos, PLFLT just, const char *text)
{
    PLINT refx, refy;
    PLFLT shift, cc, ss, def, ht;
    PLFLT xform[4], diag;
    PLFLT xscl, xoff, yscl, yoff, wx, wy;

    plgchr(&def, &ht);
    plP_gwm(&xscl, &xoff, &yscl, &yoff);
    cc = xscl * (wx2 - wx1);
    ss = yscl * (wy2 - wy1);
    diag = sqrt(cc * cc + ss * ss);
    cc = cc / diag;
    ss = ss / diag;

    xform[0] = cc;
    xform[1] = 0.0;
    xform[2] = ss;
    xform[3] = 1.0;

    shift = 0.0;
    if (just != 0.0)
	shift = plstrl(text) * just;
    wx = wx1 + pos * (wx2 - wx1);
    wy = wy1 + pos * (wy2 - wy1);

    refx = plP_mmpcx((PLFLT) (plP_wcmmx(wx) - shift * cc));
    refy = plP_mmpcy((PLFLT) (plP_wcmmy(wy) - shift * ss - disp * ht));
    plstr(0, xform, refx, refy, text);
}

/*----------------------------------------------------------------------*\
 * void plzbx()
 *
 * This draws a vertical line from (wx,wy1) to (wx,wy2) which represents the
 * vertical axis of a 3-d graph with data values from "vmin" to "vmax".
 * Depending on "opt", ticks and/or subticks are placed on the line at major
 * tick interval "tick" with "nsub" subticks between major ticks. If "tick"
 * and/or "nsub" is zero, automatic tick positions are computed
 *
 * b: Draws left-hand axis
 * c: Draws right-hand axis
 * f: Always use fixed point numeric labels
 * i: Inverts tick marks (i.e. drawn to the left)
 * l: Logarithmic axes, major ticks at decades, minor ticks at units
 * m: Write numeric label on right axis
 * n: Write numeric label on left axis
 * s: Draw minor tick marks
 * t: Draw major tick marks
 * u: Writes left-hand label
 * v: Writes right-hand label
\*----------------------------------------------------------------------*/

static void
plzbx(const char *opt, const char *label, PLINT right, PLFLT dx, PLFLT dy,
      PLFLT wx, PLFLT wy1, PLFLT wy2, PLFLT vmin, PLFLT vmax,
      PLFLT tick, PLINT nsub, PLINT *digits)
{
    static char string[40];
    PLINT lb, lc, lf, li, ll, lm, ln, ls, lt, lu, lv;
    PLINT i, mode, prec, scale;
    PLINT nsub1, lstring;
    PLFLT pos, tn, tp, temp, height, tick1;
    PLFLT dwy, lambda, diag, major, minor, xmajor, xminor;
    PLFLT ymajor, yminor, dxm, dym, xscl, xoff, yscl, yoff;

    dwy = wy2 - wy1;

/* Tick and subtick sizes in device coords */

    major = plsc->majht;
    minor = plsc->minht;

    tick1 = tick;
    nsub1 = nsub;

    lb = plP_stsearch(opt, 'b');
    lc = plP_stsearch(opt, 'c');
    lf = plP_stsearch(opt, 'f');
    li = plP_stsearch(opt, 'i');
    ll = plP_stsearch(opt, 'l');
    lm = plP_stsearch(opt, 'm');
    ln = plP_stsearch(opt, 'n');
    ls = plP_stsearch(opt, 's');
    lt = plP_stsearch(opt, 't');
    lu = plP_stsearch(opt, 'u');
    lv = plP_stsearch(opt, 'v');

    if (lu && !right)
	plztx("h", dx, dy, wx, wy1, wy2, 5.0, 0.5, 0.5, label);

    if (lv && right)
	plztx("h", dx, dy, wx, wy1, wy2, -5.0, 0.5, 0.5, label);

    if (right && !lc)
	return;

    if (!right && !lb)
	return;

    if (ll)
	tick1 = 1.0;

    if (lt)
	pldtik(vmin, vmax, &tick1, &nsub1);

    if ((li && !right) || (!li && right)) {
	minor = -minor;
	major = -major;
    }

    plP_gwm(&xscl, &xoff, &yscl, &yoff);
    dxm = dx * xscl;
    dym = dy * yscl;
    diag = sqrt(dxm * dxm + dym * dym);

    xminor = minor * dxm / diag;
    xmajor = major * dxm / diag;
    yminor = minor * dym / diag;
    ymajor = major * dym / diag;

/* Draw the line */

    plP_movwor(wx, wy1);
    if (lt) {
	tp = tick1 * floor(vmin / tick1);
	for (;;) {
	    tn = tp + tick1;
	    if (ls) {
		if (ll) {
		    for (i = 0; i <= 7; i++) {
			temp = tp + xlog[i];
			if (BETW(temp, vmin, vmax)) {
			    lambda = (temp - vmin) / (vmax - vmin);
			    plstik(plP_wcmmx(wx),
				   plP_wcmmy((PLFLT) (wy1 + lambda * dwy)),
				   xminor, yminor);
			}
		    }
		}
		else {
		    for (i = 1; i <= nsub1 - 1; i++) {
			temp = tp + i * tick1 / nsub1;
			if (BETW(temp, vmin, vmax)) {
			    lambda = (temp - vmin) / (vmax - vmin);
			    plstik(plP_wcmmx(wx),
				   plP_wcmmy((PLFLT) (wy1 + lambda * dwy)),
				   xminor, yminor);
			}
		    }
		}
	    }
	    temp = tn;
	    if (!BETW(temp, vmin, vmax))
		break;
	    lambda = (temp - vmin) / (vmax - vmin);
	    plstik(plP_wcmmx(wx), plP_wcmmy((PLFLT) (wy1 + lambda * dwy)),
		   xmajor, ymajor);
	    tp = tn;
	}
    }

    plP_drawor(wx, wy2);

/* Label the line */

    if ((ln || lm) && lt) {
	pldprec(vmin, vmax, tick1, lf, &mode, &prec, *digits, &scale);
	*digits = 0;
	tp = tick1 * floor(vmin / tick1);
	for (tn = tp + tick1; BETW(tn, vmin, vmax); tn += tick1) {
	    plform(tn, scale, prec, string, ll, lf);
	    pos = (tn - vmin) / (vmax - vmin);
	    if (ln && !right)
		plztx("v", dx, dy, wx, wy1, wy2, 0.5, pos, 1.0, string);

	    if (lm && right)
		plztx("v", dx, dy, wx, wy1, wy2, -0.5, pos, 0.0, string);

	    lstring = strlen(string);
	    *digits = MAX(*digits, lstring);
	}
	if (!ll && mode) {
	    sprintf(string, "(x10#u%d#d)", (int) scale);
	    pos = 1.15;
	    height = 0.5;
	    if (ln && !right) {
		plztx("v", dx, dy, wx, wy1, wy2, height, pos, 1.0, string);
	    }
	    if (lm && right) {
		plztx("v", dx, dy, wx, wy1, wy2,
		      (PLFLT) -height, pos, 0.0, string);
	    }
	}
    }
}

/*----------------------------------------------------------------------*\
 * void plztx()
 *
 * Prints out text along a vertical axis for a 3d plot joining
 * world coordinates (wx,wy1) to (wx,wy2).
\*----------------------------------------------------------------------*/

static void
plztx(const char *opt, PLFLT dx, PLFLT dy, PLFLT wx, PLFLT wy1,
      PLFLT wy2, PLFLT disp, PLFLT pos, PLFLT just, const char *text)
{
    PLINT refx = 0, refy = 0;
    PLINT vert = 0;
    PLFLT shift, cc, ss, def, ht;
    PLFLT xform[4], diag;
    PLFLT xscl, xoff, yscl, yoff, wy;

    plgchr(&def, &ht);
    plP_gwm(&xscl, &xoff, &yscl, &yoff);
    cc = xscl * dx;
    ss = yscl * dy;
    diag = sqrt(cc * cc + ss * ss);
    cc = cc / diag;
    ss = ss / diag;
    plP_gmp(&xscl, &xoff, &yscl, &yoff);

    shift = 0.0;
    if (just != 0.0)
	shift = plstrl(text) * just;
    wy = wy1 + pos * (wy2 - wy1);

    if (plP_stsearch(opt, 'v')) {
	vert = 0;
	refx = plP_mmpcx((PLFLT) (plP_wcmmx(wx) - (disp * ht + shift) * cc));
	refy = plP_mmpcy((PLFLT) (plP_wcmmy(wy) - (disp * ht + shift) * ss));
    }
    else if (plP_stsearch(opt, 'h')) {
	vert = 1;
	refy = plP_wcpcy(wy) - yscl * (disp * ht * ss + shift);
	refx = plP_mmpcx((PLFLT) (plP_wcmmx(wx) - disp * ht * cc));
    }
    if (vert) {
	xform[0] = 0.0;
	xform[1] = -cc;
	xform[2] = 1.0;
	xform[3] = -ss;
    }
    else {
	xform[0] = cc;
	xform[1] = 0.0;
	xform[2] = ss;
	xform[3] = 1.0;
    }
    plstr(0, xform, refx, refy, text);
}

/*----------------------------------------------------------------------*\
 * void grid_box()
 *
 * Draws grids at tick locations (major and/or minor).
 *
 * Note that 'tspace' is the minimim distance away (in fractional number
 * of ticks or subticks) from the boundary a grid line can be drawn.  If
 * you are too close, it looks bad.
\*----------------------------------------------------------------------*/

static void
grid_box(const char *xopt, PLFLT xtick1, PLINT nxsub1,
	 const char *yopt, PLFLT ytick1, PLINT nysub1)
{
    PLINT lgx, lhx, llx;
    PLINT lgy, lhy, lly;
    PLFLT vpwxmi, vpwxma, vpwymi, vpwyma;
    PLFLT tn, tp, temp, tcrit, tspace = 0.1;
    PLINT i;

/* Set plot options from input */

    lgx = plP_stsearch(xopt, 'g');
    lhx = plP_stsearch(xopt, 'h');
    llx = plP_stsearch(xopt, 'l');

    lgy = plP_stsearch(yopt, 'g');
    lhy = plP_stsearch(yopt, 'h');
    lly = plP_stsearch(yopt, 'l');

    plP_gvpw(&vpwxmi, &vpwxma, &vpwymi, &vpwyma);

/* Draw grid in x direction. */

    if (lgx) {
	tn = xtick1 * floor(vpwxmi / xtick1 + tspace);
	for (;;) {
	    if (lhx) {
		if (llx) {
		    PLFLT otemp = tn;
		    for (i = 0; i <= 7; i++) {
			temp = tn + xlog[i];
			tcrit = temp + (temp - otemp)*tspace;
			otemp = temp;
			if (BETW(tcrit, vpwxmi, vpwxma))
			    pljoin(temp, vpwymi, temp, vpwyma);
		    }
		}
		else {
		    for (i = 1; i <= nxsub1 - 1; i++) {
			temp = tn + i * xtick1 / nxsub1;
			tcrit = temp + xtick1 / nxsub1 * tspace;
			if (BETW(tcrit, vpwxmi, vpwxma))
			    pljoin(temp, vpwymi, temp, vpwyma);
		    }
		}
	    }
	    tn += xtick1;
	    tcrit = tn + xtick1*tspace;
	    if (!BETW(tcrit, vpwxmi, vpwxma))
		break;
	    pljoin(tn, vpwymi, tn, vpwyma);
	}
    }

/* Draw grid in y direction */

    if (lgy) {
	tn = ytick1 * floor(vpwymi / ytick1 + tspace);
	for (;;) {
	    if (lhy) {
		if (lly) {
		    PLFLT otemp = tn;
		    for (i = 0; i <= 7; i++) {
			temp = tn + xlog[i];
			tcrit = temp + (temp - otemp)*tspace;
			otemp = temp;
			if (BETW(tcrit, vpwymi, vpwyma))
			    pljoin(vpwxmi, temp, vpwxma, temp);
		    }
		}
		else {
		    for (i = 1; i <= nysub1 - 1; i++) {
			temp = tn + i * ytick1 / nysub1;
			tcrit = temp + ytick1 / nysub1 * tspace;
			if (BETW(tcrit, vpwymi, vpwyma))
			    pljoin(vpwxmi, temp, vpwxma, temp);
		    }
		}
	    }
	    tn += ytick1;
	    tcrit = tn + ytick1*tspace;
	    if (!BETW(tcrit, vpwymi, vpwyma))
		break;
	    pljoin(vpwxmi, tn, vpwxma, tn);
	}
    }
}

/*----------------------------------------------------------------------*\
 * void label_box()
 *
 * Writes numeric labels on side(s) of box.
\*----------------------------------------------------------------------*/

static void
label_box(const char *xopt, PLFLT xtick1, const char *yopt, PLFLT ytick1)
{
    static char string[40];
    PLINT lfx, lix, llx, lmx, lnx, ltx;
    PLINT lfy, liy, lly, lmy, lny, lty, lvy;
    PLFLT vpwxmi, vpwxma, vpwymi, vpwyma;
    PLFLT pos, tn, tp, temp, offset, height;

/* Set plot options from input */

    lfx = plP_stsearch(xopt, 'f');
    lix = plP_stsearch(xopt, 'i');
    llx = plP_stsearch(xopt, 'l');
    lmx = plP_stsearch(xopt, 'm');
    lnx = plP_stsearch(xopt, 'n');
    ltx = plP_stsearch(xopt, 't');

    lfy = plP_stsearch(yopt, 'f');
    liy = plP_stsearch(yopt, 'i');
    lly = plP_stsearch(yopt, 'l');
    lmy = plP_stsearch(yopt, 'm');
    lny = plP_stsearch(yopt, 'n');
    lty = plP_stsearch(yopt, 't');
    lvy = plP_stsearch(yopt, 'v');

    plP_gvpw(&vpwxmi, &vpwxma, &vpwymi, &vpwyma);

/* Write horizontal label(s) */

    if ((lmx || lnx) && ltx) {
	PLINT xmode, xprec, xdigmax, xdigits, xscale;

	plgxax(&xdigmax, &xdigits);
	pldprec(vpwxmi, vpwxma, xtick1, lfx, &xmode, &xprec, xdigmax, &xscale);

	tp = xtick1 * (1. + floor(vpwxmi / xtick1));
	for (tn = tp; BETW(tn, vpwxmi, vpwxma); tn += xtick1) {
	    plform(tn, xscale, xprec, string, llx, lfx);
	    height = lix ? 1.75 : 1.5;
	    pos = (tn - vpwxmi) / (vpwxma - vpwxmi);
  	    if (lnx)
		plmtex("b", height, pos, 0.5, string);
	    if (lmx)
		plmtex("t", height, pos, 0.5, string);
	}
	xdigits = 2;
	plsxax(xdigmax, xdigits);

    /* Write separate exponential label if mode = 1. */

	if (!llx && xmode) {
	    pos = 1.0;
	    height = 3.2;
	    sprintf(string, "(x10#u%d#d)", (int) xscale);
	    if (lnx)
		plmtex("b", height, pos, 0.5, string);
	    if (lmx)
		plmtex("t", height, pos, 0.5, string);
	}
    }

/* Write vertical label(s) */

    if ((lmy || lny) && lty) {
	PLINT ymode, yprec, ydigmax, ydigits, yscale;

	plgyax(&ydigmax, &ydigits);
	pldprec(vpwymi, vpwyma, ytick1, lfy, &ymode, &yprec, ydigmax, &yscale);

	ydigits = 0;
	tp = ytick1 * (1. + floor(vpwymi / ytick1));
	for (tn = tp; BETW(tn, vpwymi, vpwyma); tn += ytick1) {
	    plform(tn, yscale, yprec, string, lly, lfy);
	    pos = (tn - vpwymi) / (vpwyma - vpwymi);
	    if (lny) {
		if (lvy) {
		    height = liy ? 1.0 : 0.5;
		    plmtex("lv", height, pos, 1.0, string);
		} else {
		    height = liy ? 1.75 : 1.5;
		    plmtex("l", height, pos, 0.5, string);
		}
	    }
	    if (lmy) {
		if (lvy) {
		    height = liy ? 1.0 : 0.5;
		    plmtex("rv", height, pos, 0.0, string);
		} else {
		    height = liy ? 1.75 : 1.5;
		    plmtex("r", height, pos, 0.5, string);
		}
	    }
	    ydigits = MAX(ydigits, strlen(string));
	}
	if (!lvy)
	    ydigits = 2;

	plsyax(ydigmax, ydigits);

    /* Write separate exponential label if mode = 1. */

	if (!lly && ymode) {
	    sprintf(string, "(x10#u%d#d)", (int) yscale);
	    offset = 0.02;
	    height = 2.0;
	    if (lny) {
		pos = 0.0 - offset;
		plmtex("t", height, pos, 1.0, string);
	    }
	    if (lmy) {
		pos = 1.0 + offset;
		plmtex("t", height, pos, 0.0, string);
	    }
	}
    }
}

/*----------------------------------------------------------------------*\
 * void plform()
 *
 * Formats a PLFLT value in one of the following formats.
 *
 * If ll (logarithmic), then:
 *
 *    -	If lf (fixed), then used fixed point notation, i.e. .1, 1, 10, etc,
 *	with unnecessary trailing .'s or 0's removed.
 *
 *    -	If !lf (default), then use exponential notation, i.e. 10^-1, etc.
 *
 * If !ll (linear), then:
 *
 *    - If scale == 0, use fixed point format with "prec" places after the
 *	decimal point.
 *
 *    -	If scale == 1, use scientific notation with one place before the
 *	decimal point and "prec" places after.  In this case, the value
 *	must be divided by 10^scale.
\*----------------------------------------------------------------------*/

static void
plform(PLFLT value, PLINT scale, PLINT prec, char *string, PLINT ll, PLINT lf)
{
    if (ll) {

    /* Logarithmic */

	if (lf) {

	/* Fixed point, i.e. .1, 1, 10, etc */

	    int exponent = ROUND(value);

	    value = pow(10.0, exponent);
	    if (exponent < 0) {
		char form[10];
		sprintf(form, "%%.%df", ABS(exponent));
		sprintf(string, form, value);
	    }
	    else {
		sprintf(string, "%d", (int) value);
	    }
	}
	else {

	/* Exponential, i.e. 10^-1, 10^0, 10^1, etc */

	    sprintf(string, "10#u%d", (int) ROUND(value));
	}
    }
    else {

    /* Linear */

	PLINT setpre, precis;
	char form[10], temp[30];
	double scale2;

	plP_gprec(&setpre, &precis);

	if(setpre)
	    prec = precis;

	if(scale)
	    value /= pow(10.,(double)scale);

    /* This is necessary to prevent labels like "-0.0" on some systems */

	scale2 = pow(10., prec);
	value = floor((value * scale2) + .5) / scale2;

	sprintf(form, "%%.%df", (int) prec);
	sprintf(temp, form, value);
	strcpy(string, temp);
    }
}
