/* $Id$
   $Log$
   Revision 1.2.2.1  2001/04/19 12:31:46  rlaboiss
   First merge against MAIN

   Revision 1.2  1992/09/29 04:46:30  furnish
   Massive clean up effort to remove support for garbage compilers (K&R).

 * Revision 1.1  1992/05/20  21:35:01  furnish
 * Initial checkin of the whole PLPLOT project.
 *
*/

/*	plxybx.c

	Sloping line draw for 3d plot.
*/

#include "plplot.h"
#include <stdio.h>
#include <math.h>

static void  plxytx(PLFLT, PLFLT, PLFLT, PLFLT, \
		    PLFLT, PLFLT, PLFLT, char *);

static PLFLT xlog[8] =
{0.301030, 0.477121, 0.602060, 0.698970, 0.778151, 0.845098, 0.903090, 0.954243};

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

void
plxybx(char *opt, char *label, PLFLT wx1, PLFLT wy1,
       PLFLT wx2, PLFLT wy2, PLFLT vmin, PLFLT vmax,
       PLFLT tick, PLINT nsub, PLINT nolast, PLINT *digits)
{
    static char string[40];
    PLINT lb, li, ll, ln, ls, lt, lu;
    PLINT major, minor, mode, prec, scale;
    PLINT i, i1, i2, i3, i4;
    PLINT nsub1;
    PLFLT xpmm, ypmm, defmaj, defmin, htmaj, htmin, tick1;
    PLFLT pos, tn, tp, temp;
    PLFLT dwx, dwy, lambda;

    dwx = wx2 - wx1;
    dwy = wy2 - wy1;

    /* Tick and subtick sizes in device coords */

    gpixmm(&xpmm, &ypmm);
    gmaj(&defmaj, &htmaj);
    gmin(&defmin, &htmin);

    major = MAX(ROUND(htmaj * ypmm), 1);
    minor = MAX(ROUND(htmin * ypmm), 1);

    tick1 = tick;
    nsub1 = nsub;

    lb = stsearch(opt, 'b');
    li = stsearch(opt, 'i');
    ll = stsearch(opt, 'l');
    ln = stsearch(opt, 'n');
    ls = stsearch(opt, 's');
    lt = stsearch(opt, 't');
    lu = stsearch(opt, 'u');

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

    movwor(wx1, wy1);
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
			plxtik(wcpcx((PLFLT) (wx1 + lambda * dwx)), wcpcy((PLFLT) (wy1 + lambda * dwy)), i1, i2);
		    }
		}
	    }
	    else {
		for (i = 1; i <= nsub1 - 1; i++) {
		    temp = tp + i * (tn - tp) / nsub1;
		    if (BETW(temp, vmin, vmax)) {
			lambda = (temp - vmin) / (vmax - vmin);
			plxtik(wcpcx((PLFLT) (wx1 + lambda * dwx)), wcpcy((PLFLT) (wy1 + lambda * dwy)), i1, i2);
		    }
		}
	    }
	}
	temp = tn;
	if (BETW(temp, vmin, vmax)) {
	    lambda = (temp - vmin) / (vmax - vmin);
	    plxtik(wcpcx((PLFLT) (wx1 + lambda * dwx)), wcpcy((PLFLT) (wy1 + lambda * dwy)), i3, i4);
	    tp = tn;
	    goto lab2;
	}
    }

    drawor(wx2, wy2);

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
		sprintf(string, "10#u%-d", ROUND(tn));
	    pos = (tn - vmin) / (vmax - vmin);
	    plxytx(wx1, wy1, wx2, wy2, (PLFLT) 1.5, pos, (PLFLT) 0.5, string);
	    tp = tn;
	    goto lab82;
	}
	*digits = 2;
	if (!ll && mode) {
	    pos = 1.0;
/*	    sprintf(string, "E%+d", scale);	*/
	    sprintf(string, "(x10#u%d#d)", scale);
	    plxytx(wx1, wy1, wx2, wy2, (PLFLT) 1.5, pos, (PLFLT) 0.5, string);
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
plxytx (PLFLT wx1, PLFLT wy1, PLFLT wx2, PLFLT wy2, 
	PLFLT disp, PLFLT pos, PLFLT just, char *text)
{
    PLINT refx, refy;
    PLFLT shift, cc, ss, def, ht;
    PLFLT xform[4], diag;
    PLFLT xscl, xoff, yscl, yoff, wx, wy;

    gchr(&def, &ht);
    gwm(&xscl, &xoff, &yscl, &yoff);
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

    refx = mmpcx((PLFLT) (wcmmx(wx) - shift * cc));
    refy = mmpcy((PLFLT) (wcmmy(wy) - shift * ss - disp * ht));
    plstr(0, xform, refx, refy, text);
}
