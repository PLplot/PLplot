/* $Id$
   $Log$
   Revision 1.10  1993/07/01 22:13:37  mjl
   Changed all plplot source files to include plplotP.h (private) rather than
   plplot.h.  Rationalized namespace -- all externally-visible internal
   plplot functions now start with "plP_".

 * Revision 1.9  1993/01/23  05:54:04  mjl
 * Now holds all routines dealing with fills.
 *
 * Revision 1.8  1992/10/12  17:08:03  mjl
 * Added PL_NEED_SIZE_T define to those files that need to know the value
 * of (size_t) for non-POSIX systems (in this case the Amiga) that require you
 * to include <stddef.h> to get it.
 *
 * Revision 1.7  1992/09/30  18:25:46  furnish
 * Massive cleanup to irradicate garbage code.  Almost everything is now
 * prototyped correctly.  Builds on HPUX, SUNOS (gcc), AIX, and UNICOS.
 *
 * Revision 1.6  1992/09/29  04:45:56  furnish
 * Massive clean up effort to remove support for garbage compilers (K&R).
 *
 * Revision 1.5  1992/05/27  00:01:12  furnish
 * Having now verified that my change to plfill.c works on dino, ctrss2 and
 * ctribm1 (at a total time cost of less than 10 minutes) I am now committing
 * yet another fix to plfill.c.  Anyone who touches this file in the next
 * four days will die!
 *
 * Revision 1.4  1992/05/26  20:41:34  mjl
 * Fixed my fix to my fix to work under non-ANSI C (i.e. SUNOS).
 *
 * Revision 1.3  1992/05/26  18:27:49  mjl
 * Fixed (hopefully for the last time) the problems introduced with the
 * last "fix".
 *
 * Revision 1.2  1992/05/22  16:39:05  mjl
 * Fixed bug in qsort call.  I can't see how it managed to avoid a core dump
 * before.  This may explain some of our many problems with this routine.
 *
 * Revision 1.1  1992/05/20  21:34:25  furnish
 * Initial checkin of the whole PLPLOT project.
 *
*/

/*	plfill.c

	Polygon pattern fill.
*/

#define PL_NEED_MALLOC
#define PL_NEED_SIZE_T

#include "plplotP.h"
#include <stdlib.h>
#include <math.h>

#define DTOR            0.0174533
#define BINC            50

struct point {
    PLINT x, y;
};
static PLINT bufferleng, buffersize, *buffer;

/* Static function prototypes */
/* INDENT OFF */

static int   compar	(const void *, const void *);
static void  addcoord	(PLINT, PLINT);
static void  tran	(PLINT *, PLINT *, PLFLT, PLFLT);
static void  buildlist	(PLINT, PLINT, PLINT, PLINT, PLINT, PLINT, PLINT);

/* INDENT ON */

/*----------------------------------------------------------------------*\
* void plfill()
*
* Pattern fills the polygon bounded by the input points.
\*----------------------------------------------------------------------*/

void
c_plfill(PLINT n, PLFLT *x, PLFLT *y)
{
    PLINT i, level;
    PLINT xp, yp, xpmin, ypmin, xp1, yp1, xp2, yp2, xp3, yp3;
    PLINT k, dinc;
    PLFLT ci, si, thetd;
    PLINT *inclin, *delta, nps;
    PLFLT xpmm, ypmm;
    short swap;

    plP_glev(&level);
    if (level < 3)
	plexit("plfill: Please set up window first.");
    if (n < 3)
	plexit("plfill: Not enough points in object!");

    buffersize = 2 * BINC;
    buffer = (PLINT *) malloc((size_t) buffersize * sizeof(PLINT));
    if (!buffer)
	plexit("plfill: Out of memory.");

    plP_gpat(&inclin, &delta, &nps);
    plP_gpixmm(&xpmm, &ypmm);

    for (k = 0; k < nps; k++) {
	bufferleng = 0;

	if (ABS(inclin[k]) <= 450) {
	    thetd = atan(tan(DTOR * inclin[k] / 10.) * ypmm / xpmm);
	    swap = 0;
	}
	else {
	    thetd = atan(tan(DTOR * SIGN(inclin[k]) *
			     (ABS(inclin[k]) - 900) / 10.) * xpmm / ypmm);
	    swap = 1;
	}
	ci = cos(thetd);
	si = sin(thetd);
	if (swap)
	    dinc = delta[k] * SSQR(xpmm * ABS(ci), ypmm * ABS(si)) / 1000.;
	else
	    dinc = delta[k] * SSQR(ypmm * ABS(ci), xpmm * ABS(si)) / 1000.;

	xpmin = plP_wcpcx(x[0]);
	ypmin = plP_wcpcy(y[0]);
	for (i = 1; i < n; i++) {
	    xp = plP_wcpcx(x[i]);
	    yp = plP_wcpcy(y[i]);
	    xpmin = MIN(xpmin, xp);
	    ypmin = MIN(ypmin, yp);
	}

	xp1 = plP_wcpcx(x[0]) - xpmin;
	yp1 = plP_wcpcy(y[0]) - ypmin;
	tran(&xp1, &yp1, (PLFLT) ci, (PLFLT) si);

	xp2 = plP_wcpcx(x[1]) - xpmin;
	yp2 = plP_wcpcy(y[1]) - ypmin;
	tran(&xp2, &yp2, (PLFLT) ci, (PLFLT) si);

	for (i = 2; i < n; i++) {
	    xp3 = plP_wcpcx(x[i]) - xpmin;
	    yp3 = plP_wcpcy(y[i]) - ypmin;
	    tran(&xp3, &yp3, (PLFLT) ci, (PLFLT) si);
	    if (swap)
		buildlist(yp1, xp1, yp2, xp2, yp3, xp3, dinc);
	    else
		buildlist(xp1, yp1, xp2, yp2, xp3, yp3, dinc);
	    xp1 = xp2;
	    yp1 = yp2;
	    xp2 = xp3;
	    yp2 = yp3;
	}
	xp3 = plP_wcpcx(x[0]) - xpmin;
	yp3 = plP_wcpcy(y[0]) - ypmin;
	tran(&xp3, &yp3, (PLFLT) ci, (PLFLT) si);

	if (swap)
	    buildlist(yp1, xp1, yp2, xp2, yp3, xp3, dinc);
	else
	    buildlist(xp1, yp1, xp2, yp2, xp3, yp3, dinc);

	xp1 = xp2;
	yp1 = yp2;
	xp2 = xp3;
	yp2 = yp3;
	xp3 = plP_wcpcx(x[1]) - xpmin;
	yp3 = plP_wcpcy(y[1]) - ypmin;
	tran(&xp3, &yp3, (PLFLT) ci, (PLFLT) si);

	if (swap)
	    buildlist(yp1, xp1, yp2, xp2, yp3, xp3, dinc);
	else
	    buildlist(xp1, yp1, xp2, yp2, xp3, yp3, dinc);

/* Sort list by y then x */

	qsort((void *) buffer, (size_t) bufferleng / 2,
	      (size_t) sizeof(struct point), compar);

/* OK, now do the hatching */

	i = 0;
	while (i < bufferleng) {
	    if (swap) {
		xp1 = buffer[i + 1];
		yp1 = buffer[i];
	    }
	    else {
		xp1 = buffer[i];
		yp1 = buffer[i + 1];
	    }
	    i += 2;
	    xp2 = xp1;
	    yp2 = yp1;
	    tran(&xp1, &yp1, (PLFLT) ci, (PLFLT) (-si));
	    plP_movphy(xp1 + xpmin, yp1 + ypmin);
	    if (swap) {
		xp1 = buffer[i + 1];
		yp1 = buffer[i];
	    }
	    else {
		xp1 = buffer[i];
		yp1 = buffer[i + 1];
	    }
	    i += 2;
	    if ((swap && xp2 != xp1) || (!swap && yp2 != yp1))
		continue;	/* Uh oh we're lost */
	    tran(&xp1, &yp1, (PLFLT) ci, (PLFLT) (-si));
	    plP_draphy(xp1 + xpmin, yp1 + ypmin);
	}
    }
    free((void *) buffer);
}

/*----------------------------------------------------------------------*\
* Utility functions
\*----------------------------------------------------------------------*/

static void
tran(PLINT *a, PLINT *b, PLFLT c, PLFLT d)
{
    PLINT ta, tb;

    ta = *a;
    tb = *b;

    *a = ROUND(ta * c + tb * d);
    *b = ROUND(tb * c - ta * d);
}

static void
buildlist(PLINT xp1, PLINT yp1, PLINT xp2, PLINT yp2, PLINT xp3, PLINT yp3,
	  PLINT dinc)
{
    PLINT i;
    PLINT dx, dy, cstep, nstep, lines, ploty, plotx;

    dx = xp2 - xp1;
    dy = yp2 - yp1;

    if (dy == 0)
	return;

    cstep = (yp2 > yp1 ? 1 : -1);
    nstep = (yp3 > yp2 ? 1 : -1);
    if (yp3 == yp2)
	nstep = 0;

    /* Build coordinate list */
    lines = ABS(dy) / dinc + 1;
    if (cstep == 1 && yp1 > 0)
	ploty = (yp1 / dinc + 1) * dinc;
    else if (cstep == -1 && yp1 < 0)
	ploty = (yp1 / dinc - 1) * dinc;
    else
	ploty = (yp1 / dinc) * dinc;

    for (i = 0; i < lines; i++) {
	if (!BETW(ploty, yp1, yp2))
	    break;
	plotx = xp1 + ROUND(((float) (ploty - yp1) * dx) / dy + .5);
	/* Check for extremum at end point, otherwise add to coord list */
	if (!((ploty == yp1) || (ploty == yp2 && nstep != cstep)))
	    addcoord(plotx, ploty);
	ploty += cstep * dinc;
    }
}

static void
addcoord(PLINT xp1, PLINT yp1)
{
    PLINT *temp;

    if (bufferleng + 2 > buffersize) {
	buffersize += 2 * BINC;
	temp = (PLINT *) realloc((void *) buffer,
				 (size_t) buffersize * sizeof(PLINT));
	if (!temp) {
	    free((void *) buffer);
	    plexit("plfill: Out of memory!");
	}
	buffer = temp;
    }

    buffer[bufferleng++] = xp1;
    buffer[bufferleng++] = yp1;
}

static int
compar(const void *pnum1, const void *pnum2)
{
    struct point *pnt1, *pnt2;

    pnt1 = (struct point *) pnum1;
    pnt2 = (struct point *) pnum2;

    if (pnt1->y < pnt2->y)
	return (-1);
    else if (pnt1->y > pnt2->y)
	return (1);

    /* Only reach here if y coords are equal, so sort by x */
    if (pnt1->x < pnt2->x)
	return (-1);
    else if (pnt1->x > pnt2->x)
	return (1);

    return (0);
}
