/* $Id$
   $Log$
   Revision 1.3  1992/09/30 18:25:55  furnish
   Massive cleanup to irradicate garbage code.  Almost everything is now
   prototyped correctly.  Builds on HPUX, SUNOS (gcc), AIX, and UNICOS.

 * Revision 1.2  1992/09/29  04:46:31  furnish
 * Massive clean up effort to remove support for garbage compilers (K&R).
 *
 * Revision 1.1  1992/05/20  21:35:02  furnish
 * Initial checkin of the whole PLPLOT project.
 *
*/

/*	plzbx.c

	Vertical line draw for 3d plot.
*/

#include <stdio.h>
#include <math.h>
#include <string.h>

#include "plplot.h"

static void  plztx(char *, PLFLT, PLFLT, PLFLT, PLFLT, \
		   PLFLT, PLFLT, PLFLT, PLFLT, char *);

static PLFLT xlog[8] =
{0.301030, 0.477121, 0.602060, 0.698970, 0.778151, 0.845098,
0.903090, 0.954243};

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

void
plzbx(char *opt, char *label, PLINT right, PLFLT dx, PLFLT dy,
      PLFLT wx, PLFLT wy1, PLFLT wy2, PLFLT vmin, PLFLT vmax,
      PLFLT tick, PLINT nsub, PLINT *digits)
{
    static char string[40];
    PLINT lb, lc, li, ll, lm, ln, ls, lt, lu, lv;
    PLINT i, mode, prec, scale;
    PLINT nsub1;
    PLFLT xpmm, ypmm, defmaj, defmin, tick1;
    PLFLT pos, tn, tp, temp;
    PLFLT dwy, lambda, diag, major, minor, xmajor, xminor;
    PLFLT ymajor, yminor, dxm, dym, xscl, xoff, yscl, yoff;

    dwy = wy2 - wy1;

    /* Tick and subtick sizes in device coords */

    gpixmm(&xpmm, &ypmm);
    gmaj(&defmaj, &major);
    gmin(&defmin, &minor);

    tick1 = tick;
    nsub1 = nsub;

    lb = stsearch(opt, 'b');
    lc = stsearch(opt, 'c');
    li = stsearch(opt, 'i');
    ll = stsearch(opt, 'l');
    lm = stsearch(opt, 'm');
    ln = stsearch(opt, 'n');
    ls = stsearch(opt, 's');
    lt = stsearch(opt, 't');
    lu = stsearch(opt, 'u');
    lv = stsearch(opt, 'v');

    if (lu && !right)
	plztx("h", dx, dy, wx, wy1, wy2, (PLFLT) 5.0, (PLFLT) 0.5, (PLFLT) 0.5, label);
    if (lv && right)
	plztx("h", dx, dy, wx, wy1, wy2, (PLFLT) - 5.0, (PLFLT) 0.5, (PLFLT) 0.5, label);

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

    gwm(&xscl, &xoff, &yscl, &yoff);
    dxm = dx * xscl;
    dym = dy * yscl;
    diag = sqrt(dxm * dxm + dym * dym);

    xminor = minor * dxm / diag;
    xmajor = major * dxm / diag;
    yminor = minor * dym / diag;
    ymajor = major * dym / diag;

    /* Draw the line */

    movwor(wx, wy1);
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
			    plstik(wcmmx(wx), wcmmy((PLFLT) (wy1 + lambda * dwy)), xminor, yminor);
			}
		    }
		}
		else {
		    for (i = 1; i <= nsub1 - 1; i++) {
			temp = tp + i * tick1 / nsub1;
			if (BETW(temp, vmin, vmax)) {
			    lambda = (temp - vmin) / (vmax - vmin);
			    plstik(wcmmx(wx), wcmmy((PLFLT) (wy1 + lambda * dwy)), xminor, yminor);
			}
		    }
		}
	    }
	    temp = tn;
	    if (!BETW(temp, vmin, vmax))
		break;
	    lambda = (temp - vmin) / (vmax - vmin);
	    plstik(wcmmx(wx), wcmmy((PLFLT) (wy1 + lambda * dwy)), xmajor, ymajor);
	    tp = tn;
	}
    }

    drawor(wx, wy2);

    /* Label the line */

    if (ln && lt) {
	*digits = 0;
	tp = tick1 * floor(vmin / tick1);
	for (tn = tp + tick1; BETW(tn, vmin, vmax); tn += tick1) {
	    if (!ll)
		plform(tn, scale, prec, string);
	    else
		sprintf(string, "10#u%d", ROUND(tn));
	    pos = (tn - vmin) / (vmax - vmin);
	    if (ln && !right)
		plztx("v", dx, dy, wx, wy1, wy2, (PLFLT) 0.5, pos, (PLFLT) 1.0, string);
	    if (lm && right)
		plztx("v", dx, dy, wx, wy1, wy2, (PLFLT) - 0.5, pos, (PLFLT) 0.0, string);

	    *digits = MAX(*digits, strlen(string));
	}
	if (!ll && mode) {
	    sprintf(string, "(x10#u%d#d)", scale);
	    pos = 1.0;
	    if (ln && !right) {
		plztx("v", dx, dy, wx, wy1, wy2,  0.5, pos, 1.0, string);
	    }
	    if (lm && right) {
		plztx("v", dx, dy, wx, wy1, wy2, -0.5, pos, 0.0, string);
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
plztx (char *opt, PLFLT dx, PLFLT dy, PLFLT wx, PLFLT wy1, 
	PLFLT wy2, PLFLT disp, PLFLT pos, PLFLT just, char *text)
{
    PLINT refx=0, refy=0;
    PLINT vert=0;
    PLFLT shift, cc, ss, def, ht;
    PLFLT xform[4], diag;
    PLFLT xscl, xoff, yscl, yoff, wy;

    gchr(&def, &ht);
    gwm(&xscl, &xoff, &yscl, &yoff);
    cc = xscl * dx;
    ss = yscl * dy;
    diag = sqrt(cc * cc + ss * ss);
    cc = cc / diag;
    ss = ss / diag;
    gmp(&xscl, &xoff, &yscl, &yoff);

    shift = 0.0;
    if (just != 0.0)
	shift = plstrl(text) * just;
    wy = wy1 + pos * (wy2 - wy1);

    if (stsearch(opt, 'v')) {
	vert = 0;
	refx = mmpcx((PLFLT) (wcmmx(wx) - (disp * ht + shift) * cc));
	refy = mmpcy((PLFLT) (wcmmy(wy) - (disp * ht + shift) * ss));
    }
    else if (stsearch(opt, 'h')) {
	vert = 1;
	refy = wcpcy(wy) - yscl * (disp * ht * ss + shift);
	refx = mmpcx((PLFLT) (wcmmx(wx) - disp * ht * cc));
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
