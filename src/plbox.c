/* $Id$
 * $Log$
 * Revision 1.13  1994/06/30 18:22:00  mjl
 * All core source files: made another pass to eliminate warnings when using
 * gcc -Wall.  Lots of cleaning up: got rid of includes of math.h or string.h
 * (now included by plplot.h), and other minor changes.  Now each file has
 * global access to the plstream pointer via extern; many accessor functions
 * eliminated as a result.
 *
 * Revision 1.12  1994/04/30  16:15:04  mjl
 * Fixed format field (%ld instead of %d) or introduced casts where
 * appropriate to eliminate warnings given by gcc -Wall.
 *
 * Revision 1.11  1994/04/08  12:29:28  mjl
 * Hack to prevent labels like "-0.0" on some systems, caused by roundoff.
 *
 * Revision 1.10  1994/03/23  07:54:08  mjl
 * Replaced call to plexit() on simple (recoverable) errors with simply
 * printing the error message (via plabort()) and returning.  Should help
 * avoid loss of computer time in some critical circumstances (during a long
 * batch run, for example).
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
plform(PLFLT value, PLINT scale, PLINT prec, char *result);

/*----------------------------------------------------------------------*\
 * void plbox()
 *
 * This draws a box around the current viewport. XOPT and YOPT are
 * character strings which define the box as follows:
 *
 * A: Draw axis (X is horizontal line Y=0, Y is vertical line X=0)
 * B: Draw bottom (X) or left (Y) edge of frame
 * C: Draw top (X) or right (Y) edge of frame
 * G: Draws a grid at the major tick interval
 * I: Inverts tick marks
 * L: Logarithmic axes, major ticks at decades, minor ticks at units
 * N: Write numeric label at conventional location
 * M: Write numeric label at unconventional location
 * T: Draw major tick marks
 * S: Draw minor tick marks
 * V: (for Y only) Label vertically
 *
 * xtick, ytick are the major tick intervals required, zero for
 * automatic selection
 *
 * nxsub, nysub are the number of subtick intervals in a major tick
 * interval
\*----------------------------------------------------------------------*/

void
c_plbox(const char *xopt, PLFLT xtick, PLINT nxsub,
	const char *yopt, PLFLT ytick, PLINT nysub)
{
    static char string[40];
    PLINT lax, lbx, lcx, lgx, lix, llx, lmx, lnx, lsx, ltx;
    PLINT lay, lby, lcy, lgy, liy, lly, lmy, lny, lsy, lty, lvy;
    PLINT xmajor, xminor, ymajor, yminor;
    PLINT xmode, xprec, xdigmax, xdigits, xscale;
    PLINT ymode, yprec, ydigmax, ydigits, yscale;
    PLINT i, i1x, i2x, i3x, i4x, i1y, i2y, i3y, i4y, it0;
    PLINT nxsub1, nysub1;
    PLINT lxmin, lxmax, lymin, lymax;
    PLINT pxmin, pxmax, pymin, pymax;
    PLINT vppxmi, vppxma, vppymi, vppyma;
    PLINT lstring;
    PLFLT xtick1, ytick1, vpwxmi, vpwxma, vpwymi, vpwyma;
    PLFLT pos, tn, tp, temp, offset, height, xcrit, ycrit;

    if (plsc->level < 3) {
	plabort("plbox: Please set up window first");
	return;
    }

    /* Open  the clip limits to the subpage limits */

    plP_gclp(&lxmin, &lxmax, &lymin, &lymax);
    plP_gphy(&pxmin, &pxmax, &pymin, &pymax);
    plP_sclp(pxmin, pxmax, pymin, pymax);

    plP_gvpp(&vppxmi, &vppxma, &vppymi, &vppyma);

    /* Tick and subtick sizes in device coords */

    xmajor = MAX(ROUND(plsc->majht * plsc->ypmm), 1);
    ymajor = MAX(ROUND(plsc->majht * plsc->xpmm), 1);
    xminor = MAX(ROUND(plsc->minht * plsc->ypmm), 1);
    yminor = MAX(ROUND(plsc->minht * plsc->xpmm), 1);

    xtick1 = xtick;
    nxsub1 = nxsub;
    ytick1 = ytick;
    nysub1 = nysub;

    lax = plP_stsearch(xopt, 'a');
    lbx = plP_stsearch(xopt, 'b');
    lcx = plP_stsearch(xopt, 'c');
    lgx = plP_stsearch(xopt, 'g');
    lix = plP_stsearch(xopt, 'i');
    llx = plP_stsearch(xopt, 'l');
    lmx = plP_stsearch(xopt, 'm');
    lnx = plP_stsearch(xopt, 'n');
    lsx = plP_stsearch(xopt, 's');
    ltx = plP_stsearch(xopt, 't');

    lay = plP_stsearch(yopt, 'a');
    lby = plP_stsearch(yopt, 'b');
    lcy = plP_stsearch(yopt, 'c');
    lgy = plP_stsearch(yopt, 'g');
    liy = plP_stsearch(yopt, 'i');
    lly = plP_stsearch(yopt, 'l');
    lmy = plP_stsearch(yopt, 'm');
    lny = plP_stsearch(yopt, 'n');
    lsy = plP_stsearch(yopt, 's');
    lty = plP_stsearch(yopt, 't');
    lvy = plP_stsearch(yopt, 'v');

    plP_gvpw(&vpwxmi, &vpwxma, &vpwymi, &vpwyma);

    lax = lax && (vpwymi * vpwyma < 0.0) && !llx;
    lay = lay && (vpwxmi * vpwxma < 0.0) && !lly;

    plgxax(&xdigmax, &xdigits);
    plgyax(&ydigmax, &ydigits);

    if (llx)
	xtick1 = 1.0;
    if (lly)
	ytick1 = 1.0;

    /* Calculate tick spacing */

    if (ltx || lgx)
	pldtik(vpwxmi, vpwxma, &xtick1, &nxsub1, &xmode, &xprec, xdigmax,
	       &xscale);

    if (lty || lgy)
	pldtik(vpwymi, vpwyma, &ytick1, &nysub1, &ymode, &yprec, ydigmax,
	       &yscale);

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
			    temp = tp + i * (tn - tp) / nxsub1;
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
			    temp = tp + i * (tn - tp) / nysub1;
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
			    temp = tn + i * (tp - tn) / nxsub1;
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
			    temp = tn + i * (tp - tn) / nysub1;
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
	it0 = plP_wcpcy((PLFLT) 0.0);
	plP_movphy(vppxmi, it0);
	if (ltx) {
	    tp = xtick1 * floor(vpwxmi / xtick1);
	    for (;;) {
		tn = tp + xtick1;
		if (lsx) {
		    if (llx) {
			for (i = 0; i <= 7; i++) {
			    temp = tp + xlog[i];
			    if (BETW(temp, vpwxmi, vpwxma))
				plxtik(plP_wcpcx(temp), it0, xminor, xminor);
			}
		    }
		    else {
			for (i = 1; i <= nxsub1 - 1; i++) {
			    temp = tp + i * (tn - tp) / nxsub1;
			    if (BETW(temp, vpwxmi, vpwxma))
				plxtik(plP_wcpcx(temp), it0, xminor, xminor);
			}
		    }
		}
		if (!BETW(tn, vpwxmi, vpwxma))
		    break;
		plxtik(plP_wcpcx(tn), it0, xmajor, xmajor);
		tp = tn;
	    }
	}
	plP_draphy(vppxma, it0);
    }

    /* Draw the vertical axis */

    if (lay) {
	it0 = plP_wcpcx((PLFLT) 0.0);
	plP_movphy(it0, vppymi);
	if (lty) {
	    tp = ytick1 * floor(vpwymi / ytick1);
	    for (;;) {
		tn = tp + ytick1;
		if (lsy) {
		    if (lly) {
			for (i = 0; i <= 7; i++) {
			    temp = tp + xlog[i];
			    if (BETW(temp, vpwymi, vpwyma))
				plytik(it0, plP_wcpcy(temp), yminor, yminor);
			}
		    }
		    else {
			for (i = 1; i <= nysub1 - 1; i++) {
			    temp = tp + i * (tn - tp) / nysub1;
			    if (BETW(temp, vpwymi, vpwyma))
				plytik(it0, plP_wcpcy(temp), yminor, yminor);
			}
		    }
		}
		if (!BETW(tn, vpwymi, vpwyma))
		    break;
		plytik(it0, plP_wcpcy(tn), ymajor, ymajor);
		tp = tn;
	    }
	}
	plP_draphy(it0, vppyma);
    }

    /* Draw grid in x direction */
    /* xcrit is the minimim distance away (in fractional number of ticks) */
    /* from the boundary a grid line can be drawn.  If you are too close, */
    /* it looks bad.  The previous setting was 0.5 which was too far. */

    if (lgx) {
	xcrit = 0.1;
	tp = xtick1 * (1. + floor(vpwxmi / xtick1 + xcrit));
	for (tn = tp; BETW(tn + xtick1*xcrit, vpwxmi, vpwxma); tn += xtick1)
	    pljoin(tn, vpwymi, tn, vpwyma);
    }

    /* Draw grid in y direction */
    /* ycrit behaves analogously to xcrit */

    if (lgy) {
	ycrit = 0.1;
	tp = ytick1 * (1. + floor(vpwymi / ytick1 + 0.05));
	for (tn = tp; BETW(tn + ytick1*ycrit, vpwymi, vpwyma); tn += ytick1)
	    pljoin(vpwxmi, tn, vpwxma, tn);
    }

    /* Write horizontal label(s) */

    if ((lmx || lnx) && ltx) {
	tp = xtick1 * (1. + floor(vpwxmi / xtick1));
	for (tn = tp; BETW(tn, vpwxmi, vpwxma); tn += xtick1) {
	    if (!llx)
		plform(tn, xscale, xprec, string);
	    else
		sprintf(string, "10#u%d", (int) ROUND(tn));

	    pos = (tn - vpwxmi) / (vpwxma - vpwxmi);
	    if (lnx)
		plmtex("b", (PLFLT) 1.5, pos, (PLFLT) 0.5, string);
	    if (lmx)
		plmtex("t", (PLFLT) 1.5, pos, (PLFLT) 0.5, string);
	}
	xdigits = 2;
	plsxax(xdigmax, xdigits);

/* Write separate exponential label if mode = 1. */

	if (!llx && xmode) {
	    pos = 1.0;
	    height = 3.2;
	    sprintf(string, "(x10#u%d#d)", (int) xscale);
	    if (lnx)
		plmtex("b", height, pos, (PLFLT) 0.5, string);
	    if (lmx)
		plmtex("t", height, pos, (PLFLT) 0.5, string);
	}
    }

    /* Write vertical label(s) */

    if ((lmy || lny) && lty) {
	ydigits = 0;
	tp = ytick1 * (1. + floor(vpwymi / ytick1));
	for (tn = tp; BETW(tn, vpwymi, vpwyma); tn += ytick1) {
	    if (!lly)
		plform(tn, yscale, yprec, string);
	    else
		sprintf(string, "10#u%d", (int) ROUND(tn));

	    pos = (tn - vpwymi) / (vpwyma - vpwymi);
	    if (lny) {
		if (lvy)
		    plmtex("lv", (PLFLT) 0.5, pos, (PLFLT) 1.0, string);
		else
		    plmtex("l", (PLFLT) 1.5, pos, (PLFLT) 0.5, string);
	    }
	    if (lmy) {
		if (lvy)
		    plmtex("rv", (PLFLT) 0.5, pos, (PLFLT) 0.0, string);
		else
		    plmtex("r", (PLFLT) 1.5, pos, (PLFLT) 0.5, string);
	    }
	    lstring = strlen(string);
	    ydigits = MAX(ydigits, lstring);
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
		plmtex("t", height, pos, (PLFLT) 1.0, string);
	    }
	    if (lmy) {
		pos = 1.0 + offset;
		plmtex("t", height, pos, (PLFLT) 0.0, string);
	    }
	}
    }
    /* Restore the clip limits to viewport edge */

    plP_sclp(lxmin, lxmax, lymin, lymax);
}

/*----------------------------------------------------------------------*\
 * void plaxes()
 *
 * This functions similarly to plbox() except that the origin of the axes
 * is placed at the user-specified point (x0, y0).
\*----------------------------------------------------------------------*/

void
c_plaxes(PLFLT x0, PLFLT y0, const char *xopt, PLFLT xtick, PLINT nxsub,
	 const char *yopt, PLFLT ytick, PLINT nysub)
{
    static char string[40];
    PLINT lbx, lcx, lgx, llx, lmx, lnx, lsx, ltx;
    PLINT lby, lcy, lgy, lly, lmy, lny, lsy, lty, lvy;
    PLINT xmajor, xminor, ymajor, yminor;
    PLINT xorigin, yorigin;
    PLINT xmode, xprec, xdigmax, xdigits, xscale;
    PLINT ymode, yprec, ydigmax, ydigits, yscale;
    PLINT i, i1x, i2x, i3x, i4x, i1y, i2y, i3y, i4y;
    PLINT nxsub1, nysub1;
    PLINT lxmin, lxmax, lymin, lymax;
    PLINT pxmin, pxmax, pymin, pymax;
    PLINT vppxmi, vppxma, vppymi, vppyma;
    PLINT lstring;
    PLFLT xtick1, ytick1, vpwxmi, vpwxma, vpwymi, vpwyma;
    PLFLT pos, tn, tp, temp, offset, height;

    if (plsc->level < 3) {
	plabort("plaxes: Please set up window first");
	return;
    }

    /* Open  the clip limits to the subpage limits */

    plP_gclp(&lxmin, &lxmax, &lymin, &lymax);
    plP_gphy(&pxmin, &pxmax, &pymin, &pymax);
    plP_sclp(pxmin, pxmax, pymin, pymax);

    plP_gvpp(&vppxmi, &vppxma, &vppymi, &vppyma);

    /* convert world coordinates to physical	 */

    xorigin = plP_wcpcx(x0);
    yorigin = plP_wcpcy(y0);

    /* Tick and subtick sizes in device coords */

    xmajor = MAX(ROUND(plsc->majht * plsc->ypmm), 1);
    ymajor = MAX(ROUND(plsc->majht * plsc->xpmm), 1);
    xminor = MAX(ROUND(plsc->minht * plsc->ypmm), 1);
    yminor = MAX(ROUND(plsc->minht * plsc->xpmm), 1);

    xtick1 = xtick;
    nxsub1 = nxsub;
    ytick1 = ytick;
    nysub1 = nysub;

    lbx = plP_stsearch(xopt, 'b');
    lcx = plP_stsearch(xopt, 'c');
    lgx = plP_stsearch(xopt, 'g');
    llx = plP_stsearch(xopt, 'l');
    lmx = plP_stsearch(xopt, 'm');
    lnx = plP_stsearch(xopt, 'n');
    lsx = plP_stsearch(xopt, 's');
    ltx = plP_stsearch(xopt, 't');

    lby = plP_stsearch(yopt, 'b');
    lcy = plP_stsearch(yopt, 'c');
    lgy = plP_stsearch(yopt, 'g');
    lly = plP_stsearch(yopt, 'l');
    lmy = plP_stsearch(yopt, 'm');
    lny = plP_stsearch(yopt, 'n');
    lsy = plP_stsearch(yopt, 's');
    lty = plP_stsearch(yopt, 't');
    lvy = plP_stsearch(yopt, 'v');

    plP_gvpw(&vpwxmi, &vpwxma, &vpwymi, &vpwyma);

    plgxax(&xdigmax, &xdigits);
    plgyax(&ydigmax, &ydigits);

    if (llx)
	xtick1 = 1.0;

    if (lly)
	ytick1 = 1.0;

    if (ltx || lgx)
	pldtik(vpwxmi, vpwxma, &xtick1, &nxsub1, &xmode, &xprec, xdigmax,
	       &xscale);

    if (lty || lgy)
	pldtik(vpwymi, vpwyma, &ytick1, &nysub1, &ymode, &yprec, ydigmax,
	       &yscale);

    /* Set up tick variables */

    i1x = xminor;
    i2x = xminor;
    i3x = xmajor;
    i4x = xmajor;

    i1y = yminor;
    i2y = yminor;
    i3y = ymajor;
    i4y = ymajor;

    /* draw boxing */

    if (lbx) {
	plP_movphy(vppxmi, vppymi);
	plP_draphy(vppxma, vppymi);
    }
    if (lcx) {
	plP_movphy(vppxmi, vppyma);
	plP_draphy(vppxma, vppyma);
    }
    if (lby) {
	plP_movphy(vppxmi, vppymi);
	plP_draphy(vppxmi, vppyma);
    }
    if (lcy) {
	plP_movphy(vppxma, vppymi);
	plP_draphy(vppxma, vppyma);
    }

    /* Draw the horizontal axis */

    plP_movphy(vppxmi, yorigin);
    if (ltx) {
	tp = xtick1 * floor(vpwxmi / xtick1);
	for (;;) {
	    tn = tp + xtick1;
	    if (lsx) {
		if (llx) {
		    for (i = 0; i <= 7; i++) {
			temp = tp + xlog[i];
			if (BETW(temp, vpwxmi, vpwxma))
			    plxtik(plP_wcpcx(temp), yorigin, i1x, i2x);
		    }
		}
		else {
		    for (i = 1; i <= nxsub1 - 1; i++) {
			temp = tp + i * (tn - tp) / nxsub1;
			if (BETW(temp, vpwxmi, vpwxma))
			    plxtik(plP_wcpcx(temp), yorigin, i1x, i2x);
		    }
		}
	    }
	    if (!BETW(tn, vpwxmi, vpwxma))
		break;
	    plxtik(plP_wcpcx(tn), yorigin, i3x, i4x);
	    tp = tn;
	}
    }
    plP_draphy(vppxma, yorigin);

    /* Draw vertical axis */

    if (lby) {
	plP_movphy(xorigin, vppyma);
	if (lty) {
	    tp = ytick1 * (floor(vpwyma / ytick1) + 1);
	    for (;;) {
		tn = tp - ytick1;
		if (lsy) {
		    if (lly) {
			for (i = 7; i >= 0; i--) {
			    temp = tn + xlog[i];
			    if (BETW(temp, vpwymi, vpwyma))
				plytik(xorigin, plP_wcpcy(temp), i1y, i2y);
			}
		    }
		    else {
			for (i = nysub1 - 1; i >= 1; i--) {
			    temp = tn + i * (tp - tn) / nysub1;
			    if (BETW(temp, vpwymi, vpwyma))
				plytik(xorigin, plP_wcpcy(temp), i1y, i2y);
			}
		    }
		}
		if (!BETW(tn, vpwymi, vpwyma))
		    break;
		plytik(xorigin, plP_wcpcy(tn), i3y, i4y);
		tp = tn;
	    }
	}
	plP_draphy(xorigin, vppymi);
    }

    /* Draw grid in x direction */

    if (lgx) {
	tp = xtick1 * (1. + floor(vpwxmi / xtick1 + .5));
	for (tn = tp; BETW(tn + xtick1 / 2., vpwxmi, vpwxma); tn += xtick1)
	    pljoin(tn, vpwymi, tn, vpwyma);
    }

    /* Draw grid in y direction */
    if (lgy) {
	tp = ytick1 * (1. + floor(vpwymi / ytick1 + .5));
	for (tn = tp; BETW(tn + ytick1 / 2., vpwymi, vpwyma); tn += ytick1)
	    pljoin(vpwxmi, tn, vpwxma, tn);
    }

    /* Write horizontal label(s) */

    if ((lmx || lnx) && ltx) {
	tp = xtick1 * (1. + floor(vpwxmi / xtick1));
	for (tn = tp; BETW(tn, vpwxmi, vpwxma); tn += xtick1) {
	    if (!llx)
		plform(tn, xscale, xprec, string);
	    else {
		sprintf(string, "10#u%d", (int) ROUND(tn));
	    }
	    pos = (tn - vpwxmi) / (vpwxma - vpwxmi);
	    if (lnx)
		plmtex("b", 1.5, pos, 0.5, string);
	    if (lmx)
		plmtex("t", 1.5, pos, 0.5, string);
	}
	xdigits = 2;
	plsxax(xdigmax, xdigits);

/* Write separate exponential label if mode = 1. */

	if (!llx && xmode) {
	    pos = 1.0;
	    height = 3.2;
	    sprintf(string, "(x10#u%d#d)", (int) xscale);
	    if (lnx)
		plmtex("b", height, pos, (PLFLT) 0.5, string);
	    if (lmx)
		plmtex("t", height, pos, (PLFLT) 0.5, string);
	}
    }

    /* Write vertical label(s) */

    if ((lmy || lny) && lty) {
	ydigits = 0;
	tp = ytick1 * (1. + floor(vpwymi / ytick1));
	for (tn = tp; BETW(tn, vpwymi, vpwyma); tn += ytick1) {
	    if (!lly)
		plform(tn, yscale, yprec, string);
	    else
		sprintf(string, "10#u%d", (int) ROUND(tn));

	    pos = (tn - vpwymi) / (vpwyma - vpwymi);
	    if (lny) {
		if (lvy)
		    plmtex("lv", 0.5, pos, 1.0, string);
		else
		    plmtex("l", 1.5, pos, 0.5, string);
	    }
	    if (lmy) {
		if (lvy)
		    plmtex("rv", 0.5, pos, 0.0, string);
		else
		    plmtex("r", 1.5, pos, 0.5, string);
	    }
	    lstring = strlen(string);
	    ydigits = MAX(ydigits, lstring);
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
		plmtex("t", height, pos, (PLFLT) 1.0, string);
	    }
	    if (lmy) {
		pos = 1.0 + offset;
		plmtex("t", height, pos, (PLFLT) 0.0, string);
	    }
	}
    }
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
	/* save tick spacing and color */
	*zbtck = ztick;
	plP_gatt(&font, zbcol);
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
 * void plxbyx()
 *
 * This draws a sloping line from (wx1,wy1) to (wx2,wy2) which represents an
 * axis of a 3-d graph with data values from "vmin" to "vmax". Depending on
 * "opt", vertical ticks and/or subticks are placed on the line at major tick
 * interval "tick" with "nsub" subticks between major ticks. If "tick" and/or
 * "nsub" is zero, automatic tick positions are computed
 *
 * B: Draw box boundary
 * I: Inverts tick marks (i.e. drawn downwards)
 * L: Logarithmic axes, major ticks at decades, minor ticks at units
 * N: Write numeric label
 * T: Draw major tick marks
 * S: Draw minor tick marks
 * U: Write label on line
\*----------------------------------------------------------------------*/

static void
plxybx(const char *opt, const char *label, PLFLT wx1, PLFLT wy1,
       PLFLT wx2, PLFLT wy2, PLFLT vmin, PLFLT vmax,
       PLFLT tick, PLINT nsub, PLINT nolast, PLINT *digits)
{
    static char string[40];
    PLINT lb, li, ll, ln, ls, lt, lu;
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
    li = plP_stsearch(opt, 'i');
    ll = plP_stsearch(opt, 'l');
    ln = plP_stsearch(opt, 'n');
    ls = plP_stsearch(opt, 's');
    lt = plP_stsearch(opt, 't');
    lu = plP_stsearch(opt, 'u');

    if (lu)
	plxytx(wx1, wy1, wx2, wy2, (PLFLT) 3.2, (PLFLT) 0.5, (PLFLT) 0.5, label);
    if (!lb)
	return;

    if (ll)
	tick1 = 1.0;
    if (lt)
	pldtik(vmin, vmax, &tick1, &nsub1, &mode, &prec, *digits, &scale);

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
      lab2:
	tn = tp + tick1;
	if (ls) {
	    if (ll) {
		for (i = 0; i <= 7; i++) {
		    temp = tp + xlog[i];
		    if (BETW(temp, vmin, vmax)) {
			lambda = (temp - vmin) / (vmax - vmin);
			plxtik(plP_wcpcx((PLFLT) (wx1 + lambda * dwx)),
			       plP_wcpcy((PLFLT) (wy1 + lambda * dwy)), i1, i2);
		    }
		}
	    }
	    else {
		for (i = 1; i <= nsub1 - 1; i++) {
		    temp = tp + i * (tn - tp) / nsub1;
		    if (BETW(temp, vmin, vmax)) {
			lambda = (temp - vmin) / (vmax - vmin);
			plxtik(plP_wcpcx((PLFLT) (wx1 + lambda * dwx)),
			       plP_wcpcy((PLFLT) (wy1 + lambda * dwy)), i1, i2);
		    }
		}
	    }
	}
	temp = tn;
	if (BETW(temp, vmin, vmax)) {
	    lambda = (temp - vmin) / (vmax - vmin);
	    plxtik(plP_wcpcx((PLFLT) (wx1 + lambda * dwx)),
		   plP_wcpcy((PLFLT) (wy1 + lambda * dwy)), i3, i4);
	    tp = tn;
	    goto lab2;
	}
    }

    plP_drawor(wx2, wy2);

    /* Label the line */

    if (ln && lt) {
	tp = tick1 * floor(vmin / tick1);
      lab82:
	tn = tp + tick1;
	if (nolast && !BETW(tn + tick1, vmin, vmax))
	    return;
	if (BETW(tn, vmin, vmax)) {
	    if (!ll)
		plform(tn, scale, prec, string);
	    else
		sprintf(string, "10#u%-d", (int) ROUND(tn));
	    pos = (tn - vmin) / (vmax - vmin);
	    plxytx(wx1, wy1, wx2, wy2, (PLFLT) 1.5, pos, (PLFLT) 0.5, string);
	    tp = tn;
	    goto lab82;
	}
	*digits = 2;
	if (!ll && mode) {
	    pos = 1.0;
	    height = 3.2;
	    sprintf(string, "(x10#u%d#d)", (int) scale);
	    plxytx(wx1, wy1, wx2, wy2, height, pos, (PLFLT) 0.5, string);
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
 * B: Draws left-hand axis
 * C: Draws right-hand axis
 * I: Inverts tick marks (i.e. drawn to the left)
 * L: Logarithmic axes, major ticks at decades, minor ticks at units
 * M: Write numeric label on right axis
 * N: Write numeric label on left axis
 * S: Draw minor tick marks
 * T: Draw major tick marks
 * U: Writes left-hand label
 * V: Writes right-hand label
\*----------------------------------------------------------------------*/

static void
plzbx(const char *opt, const char *label, PLINT right, PLFLT dx, PLFLT dy,
      PLFLT wx, PLFLT wy1, PLFLT wy2, PLFLT vmin, PLFLT vmax,
      PLFLT tick, PLINT nsub, PLINT *digits)
{
    static char string[40];
    PLINT lb, lc, li, ll, lm, ln, ls, lt, lu, lv;
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
    li = plP_stsearch(opt, 'i');
    ll = plP_stsearch(opt, 'l');
    lm = plP_stsearch(opt, 'm');
    ln = plP_stsearch(opt, 'n');
    ls = plP_stsearch(opt, 's');
    lt = plP_stsearch(opt, 't');
    lu = plP_stsearch(opt, 'u');
    lv = plP_stsearch(opt, 'v');

    if (lu && !right)
	plztx("h", dx, dy, wx, wy1, wy2,
	      (PLFLT) 5.0, (PLFLT) 0.5, (PLFLT) 0.5, label);

    if (lv && right)
	plztx("h", dx, dy, wx, wy1, wy2,
	      (PLFLT) -5.0, (PLFLT) 0.5, (PLFLT) 0.5, label);

    if (right && !lc)
	return;

    if (!right && !lb)
	return;

    if (ll)
	tick1 = (PLFLT) 1.0;

    if (lt)
	pldtik(vmin, vmax, &tick1, &nsub1, &mode, &prec, *digits, &scale);

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
	*digits = 0;
	tp = tick1 * floor(vmin / tick1);
	for (tn = tp + tick1; BETW(tn, vmin, vmax); tn += tick1) {
	    if (!ll)
		plform(tn, scale, prec, string);
	    else
		sprintf(string, "10#u%d", (int) ROUND(tn));
	    pos = (tn - vmin) / (vmax - vmin);
	    if (ln && !right)
		plztx("v", dx, dy, wx, wy1, wy2,
		      (PLFLT) 0.5, pos, (PLFLT) 1.0, string);

	    if (lm && right)
		plztx("v", dx, dy, wx, wy1, wy2,
		      (PLFLT) -0.5, pos, (PLFLT) 0.0, string);

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
 * void plform()
 *
 * Formats a floating point value in one of the following formats 
 * (i)	If scale == 0, use floating point format with "prec" places
 *	after the decimal point. 
 * (ii)	If scale == 1, use scientific notation with one place before 
 *	the decimal point and "prec" places after.  In this case, the
 *	value must be divided by 10^scale.
\*----------------------------------------------------------------------*/

static void
plform(PLFLT value, PLINT scale, PLINT prec, char *result)
{
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
    strcpy(result, temp);
}

