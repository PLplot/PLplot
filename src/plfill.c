/* $Id$
   $Log$
   Revision 1.11  1993/07/16 22:36:16  mjl
   Algorithm improved and simplified, submitted by Wesley Ebisuzaki.

 * Revision 1.10  1993/07/01  22:13:37  mjl
 * Changed all plplot source files to include plplotP.h (private) rather than
 * plplot.h.  Rationalized namespace -- all externally-visible internal
 * plplot functions now start with "plP_".
 *
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
#include <stdio.h>
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
    PLINT i, j, level;
    PLINT xp1, yp1, xp2, yp2, xp3, yp3;
    PLINT k, dinc;
    PLFLT ci, si;
    PLINT *inclin, *delta, nps;
    PLFLT xpmm, ypmm;
    double temp;

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

/* Loop over sets of lines in pattern */

    for (k = 0; k < nps; k++) {
	bufferleng = 0;

        temp = DTOR * inclin[k] * 0.1;
        si = sin(temp) * ypmm;
        ci = cos(temp) * xpmm;
	/* normalize: 1 = si*si + ci*ci */
        temp = sqrt((double) (si*si + ci*ci));
	si /= temp;
	ci /= temp;

	dinc = delta[k] * SSQR(ypmm * ABS(ci), xpmm * ABS(si)) / 1000.;
	if (dinc < 0) dinc = -dinc;
	if (dinc == 0) dinc = 1;

	xp1 = plP_wcpcx(x[n-2]);
	yp1 = plP_wcpcy(y[n-2]);
	tran(&xp1, &yp1, (PLFLT) ci, (PLFLT) si);

	xp2 = plP_wcpcx(x[n-1]);
	yp2 = plP_wcpcy(y[n-1]);
	tran(&xp2, &yp2, (PLFLT) ci, (PLFLT) si);

/* Loop over points in polygon */

	for (i = 0; i < n; i++) {
	    xp3 = plP_wcpcx(x[i]);
	    yp3 = plP_wcpcy(y[i]);
	    tran(&xp3, &yp3, (PLFLT) ci, (PLFLT) si);
	    buildlist(xp1, yp1, xp2, yp2, xp3, yp3, dinc);
	    xp1 = xp2;
	    yp1 = yp2;
	    xp2 = xp3;
	    yp2 = yp3;
	}

/* Sort list by y then x */

	qsort((void *) buffer, (size_t) bufferleng / 2,
	      (size_t) sizeof(struct point), compar);

/* OK, now do the hatching */

	i = 0;

	while (i < bufferleng) {
	    xp1 = buffer[i];
	    yp1 = buffer[i + 1];
	    i += 2;
	    xp2 = xp1;
	    yp2 = yp1;
	    tran(&xp1, &yp1, (PLFLT) ci, (PLFLT) (-si));
	    plP_movphy(xp1, yp1);
	    xp1 = buffer[i];
	    yp1 = buffer[i + 1];
	    i += 2;
	    if (yp2 != yp1) {
		fprintf(stderr, "plfill: oh oh we are lost\n");
		for (j = 0; j < bufferleng; j+=2) {
		    fprintf(stderr, "plfill: %d %d\n",buffer[j],buffer[j+1]);
		}
		continue;	/* Uh oh we're lost */
	    }
	    tran(&xp1, &yp1, (PLFLT) ci, (PLFLT) (-si));
	    plP_draphy(xp1, yp1);
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

    *a = floor((double) (ta * c + tb * d + 0.5));
    *b = floor((double) (tb * c - ta * d + 0.5));
}

static void
buildlist(PLINT xp1, PLINT yp1, PLINT xp2, PLINT yp2, PLINT xp3, PLINT yp3,
	  PLINT dinc)
{
    PLINT min_y, max_y;
    PLINT dx, dy, cstep, nstep, ploty, plotx;

    dx = xp2 - xp1;
    dy = yp2 - yp1;

    if (dy == 0) {
	if (yp2 > yp3 && ((yp2 % dinc) == 0)) 
	    addcoord(xp2, yp2);
	return;
    }

    if (dy > 0) {
	cstep = 1;
	min_y = yp1;
	max_y = yp2;
    }
    else {
	cstep = -1;
	min_y = yp2;
	max_y = yp1;
    }

    nstep = (yp3 > yp2 ? 1 : -1);
    if (yp3 == yp2) nstep = 0;

    /* Build coordinate list */
    ploty = (min_y / dinc) * dinc;
    if (ploty < min_y) ploty += dinc;

    for (; ploty <= max_y; ploty += dinc) {
	if (ploty == yp1) continue;
	if (ploty == yp2) {
	    if (cstep == -nstep) continue;
	    if (yp2 == yp3 && yp1 > yp2) continue;
	}
	plotx = xp1 + floor(((double) (ploty - yp1) * dx) / dy + 0.5);
	addcoord(plotx, ploty);
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
    const struct point *pnt1, *pnt2;

    pnt1 = (const struct point *) pnum1;
    pnt2 = (const struct point *) pnum2;

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
