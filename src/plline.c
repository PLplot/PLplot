/* $Id$
   $Log$
   Revision 1.5  1993/03/03 19:42:22  mjl
   Changed PLSHORT -> short everywhere; now all device coordinates are expected
   to fit into a 16 bit address space (reasonable, and good for performance).

 * Revision 1.4  1993/02/23  05:16:19  mjl
 * Changed references in error messages from plstar to plinit, and fixed
 * some error messages to be more specific.
 *
 * Revision 1.3  1993/01/23  05:56:03  mjl
 * Now holds all routines dealing with line draws, including clipping and
 * dashed line support.  Polyline capability added.
 *
 * Revision 1.2  1992/09/29  04:46:05  furnish
 * Massive clean up effort to remove support for garbage compilers (K&R).
 *
 * Revision 1.1  1992/05/20  21:34:36  furnish
 * Initial checkin of the whole PLPLOT project.
 *
*/

/*	plline.c

	Routines dealing with line generation.
*/

#include "plplot.h"

#include <math.h>

#define INSIDE(ix,iy) (BETW(ix,clpxmi,clpxma) && BETW(iy,clpymi,clpyma))

static short xline[PL_MAXPOLYLINE], yline[PL_MAXPOLYLINE];
static short xclp[PL_MAXPOLYLINE], yclp[PL_MAXPOLYLINE];

static PLINT lastx = UNDEFINED, lasty = UNDEFINED;

/* Function prototypes */
/* INDENT OFF */

static void pllclp	(short *, short *, PLINT);
static int  clipline	(PLINT *, PLINT *, PLINT *, PLINT *,
			 PLINT, PLINT, PLINT, PLINT);
static void genlin	(short *, short *, PLINT);
static void plupd	(PLINT, PLINT *, PLINT *, PLINT *,
			 PLINT *, PLINT *, PLINT *);
static void grdashline	(short, short, short, short,
			 PLINT *, PLINT *, PLINT);
/* INDENT ON */

/*----------------------------------------------------------------------*\
* void pljoin()
*
* Draws a line segment from (x1, y1) to (x2, y2).
\*----------------------------------------------------------------------*/

void
c_pljoin(PLFLT x1, PLFLT y1, PLFLT x2, PLFLT y2)
{
    movwor(x1, y1);
    drawor(x2, y2);
}

/*----------------------------------------------------------------------*\
* void plline()
*
* Draws line segments connecting a series of points.
\*----------------------------------------------------------------------*/

void
c_plline(PLINT n, PLFLT *x, PLFLT *y)
{
    PLINT level;

    glev(&level);
    if (level < 3)
	plexit("plline: Please set up window first.");

    drawor_poly(x, y, n);
}

/*----------------------------------------------------------------------*\
* void plstyl()
*
* Set up a new line style of "nms" elements, with mark and space
* lengths given by arrays "mark" and "space".
\*----------------------------------------------------------------------*/

void
c_plstyl(PLINT nms, PLINT *mark, PLINT *space)
{
    short int i;
    PLINT level;

    glev(&level);
    if (level < 1)
	plexit("plstyl: Please call plinit first.");

    if ((nms < 0) || (nms > 10))
	plexit("plstyl: Broken lines cannot have <0 or >10 elements");

    for (i = 0; i < nms; i++) {
	if ((mark[i] < 0) || (space[i] < 0))
	    plexit("plstyl: Mark and space lengths must be > 0.");
    }

    smark(mark, space, nms);
    scure(0, 1, 0, (nms > 0 ? mark[0] : 0));
}

/*----------------------------------------------------------------------*\
* void movphy()
*
* Move to physical coordinates (x,y).
\*----------------------------------------------------------------------*/

void
movphy(PLINT x, PLINT y)
{
    scurr(x, y);
}

/*----------------------------------------------------------------------*\
* void draphy()
*
* Draw to physical coordinates (x,y).
\*----------------------------------------------------------------------*/

void
draphy(PLINT x, PLINT y)
{
    PLINT currx, curry;
    gcurr(&currx, &curry);
    xline[0] = currx;
    xline[1] = x;
    yline[0] = curry;
    yline[1] = y;

    pllclp(xline, yline, 2);
}

/*----------------------------------------------------------------------*\
* void movwor()
*
* Move to world coordinates (x,y).
\*----------------------------------------------------------------------*/

void
movwor(PLFLT x, PLFLT y)
{
    scurr(wcpcx(x), wcpcy(y));
}

/*----------------------------------------------------------------------*\
* void drawor()
*
* Draw to world coordinates (x,y).
\*----------------------------------------------------------------------*/

void
drawor(PLFLT x, PLFLT y)
{
    PLINT currx, curry;
    gcurr(&currx, &curry);
    xline[0] = currx;
    xline[1] = wcpcx(x);
    yline[0] = curry;
    yline[1] = wcpcy(y);

    pllclp(xline, yline, 2);
}

/*----------------------------------------------------------------------*\
* void draphy_poly()
*
* Draw polyline in physical coordinates.
* Need to draw buffers in increments of (PL_MAXPOLYLINE-1) since the
* last point must be repeated (for solid lines).
\*----------------------------------------------------------------------*/

void
draphy_poly(PLINT *x, PLINT *y, PLINT n)
{
    PLINT i, j, ib, ilim;

    for (ib = 0; ib < n; ib += PL_MAXPOLYLINE - 1) {
	ilim = MIN(PL_MAXPOLYLINE, n - ib);

	for (i = 0; i < ilim; i++) {
	    j = ib + i;
	    xline[i] = x[j];
	    yline[i] = y[j];
	}
	pllclp(xline, yline, ilim);
    }
}

/*----------------------------------------------------------------------*\
* void drawor_poly()
*
* Draw polyline in world coordinates.
* Need to draw buffers in increments of (PL_MAXPOLYLINE-1) since the
* last point must be repeated (for solid lines).
\*----------------------------------------------------------------------*/

void
drawor_poly(PLFLT *x, PLFLT *y, PLINT n)
{
    PLINT i, j, ib, ilim;

    for (ib = 0; ib < n; ib += PL_MAXPOLYLINE - 1) {
	ilim = MIN(PL_MAXPOLYLINE, n - ib);

	for (i = 0; i < ilim; i++) {
	    j = ib + i;
	    xline[i] = wcpcx(x[j]);
	    yline[i] = wcpcy(y[j]);
	}
	pllclp(xline, yline, ilim);
    }
}

/*----------------------------------------------------------------------*\
* void pllclp()
*
* Draws a polyline within the clip limits.
\*----------------------------------------------------------------------*/

static void
pllclp(short *x, short *y, PLINT npts)
{
    PLINT x1, x2, y1, y2;
    PLINT clpxmi, clpxma, clpymi, clpyma, i, iclp;
    int drawable;

    gclp(&clpxmi, &clpxma, &clpymi, &clpyma);

    iclp = 0;
    for (i = 0; i < npts - 1; i++) {
	x1 = x[i];
	x2 = x[i + 1];
	y1 = y[i];
	y2 = y[i + 1];

	drawable = (INSIDE(x1, y1) && INSIDE(x2, y2));
	if (!drawable)
	    drawable = !clipline(&x1, &y1, &x2, &y2,
				 clpxmi, clpxma, clpymi, clpyma);

	if (drawable) {

/* First point of polyline. */

	    if (iclp == 0) {
		xclp[iclp] = x1;
		yclp[iclp] = y1;
		iclp++;
		xclp[iclp] = x2;
		yclp[iclp] = y2;
	    }

/* Not first point.  Check if first point of this segment matches up to
/* previous point, and if so, add it to the current polyline buffer */

	    else if (x1 == xclp[iclp] && y1 == yclp[iclp]) {
		iclp++;
		xclp[iclp] = x2;
		yclp[iclp] = y2;
	    }

/* Otherwise it's time to start a new polyline */

	    else {
		if (iclp + 1 >= 2)
		    genlin(xclp, yclp, iclp + 1);
		iclp = 0;
		xclp[iclp] = x1;
		yclp[iclp] = y1;
		iclp++;
		xclp[iclp] = x2;
		yclp[iclp] = y2;
	    }
	}
    }

/* Handle remaining polyline */

    if (iclp + 1 >= 2)
	genlin(xclp, yclp, iclp + 1);

    scurr(x2, y2);
}

/*----------------------------------------------------------------------*\
* int clipline()
*
* Get clipped endpoints
\*----------------------------------------------------------------------*/

static int
clipline(PLINT *p_x1, PLINT *p_y1, PLINT *p_x2, PLINT *p_y2,
	 PLINT clpxmi, PLINT clpxma, PLINT clpymi, PLINT clpyma)
{
    PLINT t, dx, dy, flipx, flipy;
    PLFLT slope;

/* If both points are outside clip region, return with an error */

    if ((*p_x1 <= clpxmi && *p_x2 <= clpxmi) ||
	(*p_x1 >= clpxma && *p_x2 >= clpxma) ||
	(*p_y1 <= clpymi && *p_y2 <= clpymi) ||
	(*p_y1 >= clpyma && *p_y2 >= clpyma))
	return (1);

    flipx = 0;
    flipy = 0;

    if (*p_x2 < *p_x1) {
	*p_x1 = 2 * clpxmi - *p_x1;
	*p_x2 = 2 * clpxmi - *p_x2;
	clpxma = 2 * clpxmi - clpxma;
	t = clpxma;
	clpxma = clpxmi;
	clpxmi = t;
	flipx = 1;
    }

    if (*p_y2 < *p_y1) {
	*p_y1 = 2 * clpymi - *p_y1;
	*p_y2 = 2 * clpymi - *p_y2;
	clpyma = 2 * clpymi - clpyma;
	t = clpyma;
	clpyma = clpymi;
	clpymi = t;
	flipy = 1;
    }

    dx = *p_x2 - *p_x1;
    dy = *p_y2 - *p_y1;

    if (dx != 0)
	slope = (double) dy / (double) dx;

    if (*p_x1 < clpxmi) {
	if (dx != 0 && dy != 0)
	    *p_y1 = *p_y1 + ROUND(slope * (clpxmi - *p_x1));
	*p_x1 = clpxmi;
    }

    if (*p_y1 < clpymi) {
	if (dx != 0 && dy != 0)
	    *p_x1 = *p_x1 + ROUND((clpymi - *p_y1) / slope);
	*p_y1 = clpymi;
    }

    if (*p_x1 >= clpxma || *p_y1 >= clpyma)
	return (1);

    if (*p_y2 > clpyma) {
	if (dx != 0 && dy != 0)
	    *p_x2 = *p_x2 - ROUND((*p_y2 - clpyma) / slope);
	*p_y2 = clpyma;
    }

    if (*p_x2 > clpxma) {
	if (dx != 0 && dy != 0)
	    *p_y2 = *p_y2 - ROUND((*p_x2 - clpxma) * slope);
	*p_x2 = clpxma;
    }

    if (flipx) {
	*p_x1 = 2 * clpxma - *p_x1;
	*p_x2 = 2 * clpxma - *p_x2;
	clpxmi = 2 * clpxma - clpxmi;
	t = clpxma;
	clpxma = clpxmi;
	clpxmi = t;
    }

    if (flipy) {
	*p_y1 = 2 * clpyma - *p_y1;
	*p_y2 = 2 * clpyma - *p_y2;
	clpymi = 2 * clpyma - clpymi;
	t = clpyma;
	clpyma = clpymi;
	clpymi = t;
    }

    return (0);
}

/*----------------------------------------------------------------------*\
* void genlin()
*
* General line-drawing routine.  Takes line styles into account.
* If only 2 points are in the polyline, it is more efficient to use
* grline() rather than grpolyline().
\*----------------------------------------------------------------------*/

static void
genlin(short *x, short *y, PLINT npts)
{
    PLINT i, *mark, *space, nms;

/* Check for solid line */

    gmark(&mark, &space, &nms);
    if (nms == 0) {
	if (npts > 2)
	    grpolyline(x, y, npts);
	else
	    grline(x[0], y[0], x[1], y[1]);
    }

/* Right now dashed lines don't use polyline driver capability -- this
   should be improved */

    else {
	for (i = 0; i < npts - 1; i++) {
	    grdashline(x[i], y[i], x[i + 1], y[i + 1], mark, space, nms);
	}
    }
}

/*----------------------------------------------------------------------*\
* void grdashline()
*
* Plot a dashed line using current pattern.
* This routine is disgustingly slow.
\*----------------------------------------------------------------------*/

static void
grdashline(short x1, short y1, short x2, short y2,
	   PLINT *mark, PLINT *space, PLINT nms)
{
    PLINT nx, ny, nxp, nyp;
    PLINT *timecnt, *alarm, *pendn, *curel;
    PLINT modulo, incr, dx, dy, diax, diay, i, temp, x, y;
    PLINT px, py, tstep;
    PLINT umx, umy;
    double nxstep, nystep;

/* Check if pattern needs to be restarted */

    gcure(&curel, &pendn, &timecnt, &alarm);
    if (x1 != lastx || y1 != lasty) {
	*curel = 0;
	*pendn = 1;
	*timecnt = 0;
	*alarm = mark[0];
    }

    lastx = x1;
    lasty = y1;

    if (x1 == x2 && y1 == y2)
	return;

    nx = x2 - x1;
    ny = y2 - y1;

    nxp = ABS(nx);
    nyp = ABS(ny);

    if (nyp > nxp) {
	modulo = nyp;
	incr = nxp;
	dx = 0;
	dy = (ny > 0) ? 1 : -1;
    }
    else {
	modulo = nxp;
	incr = nyp;
	dx = (nx > 0) ? 1 : -1;
	dy = 0;
    }

    diax = (nx > 0) ? 1 : -1;
    diay = (ny > 0) ? 1 : -1;

    temp = modulo / 2;
    x = x1;
    y = y1;

/* Compute the timer step */

    gumpix(&umx, &umy);
    nxstep = nxp * umx;
    nystep = nyp * umy;
    tstep = sqrt( nxstep * nxstep + nystep * nystep ) / modulo;

    for (i = 0; i < modulo; i++) {
	temp = temp + incr;
	px = x;
	py = y;
	if (temp > modulo) {
	    temp = temp - modulo;
	    x = x + diax;
	    y = y + diay;
	}
	else {
	    x = x + dx;
	    y = y + dy;
	}
	*timecnt += tstep;

	if (*timecnt >= *alarm) {
	    plupd(nms, mark, space, curel, pendn, timecnt, alarm);
	    if (*pendn == 0)
		grline(lastx, lasty, px, py);
	    lastx = x;
	    lasty = y;
	}
    }

/* Finish up */

    if (*pendn != 0)
	grline(lastx, lasty, x, y);
    lastx = x2;
    lasty = y2;
}

/*----------------------------------------------------------------------*\
* void plupd()
*
* Updates line style variables, called whenever alarm goes off.
\*----------------------------------------------------------------------*/

static void
plupd(PLINT nms, PLINT *mark, PLINT *space, PLINT *curel,
      PLINT *pendn, PLINT *timecnt, PLINT *alarm)
{
    while (*timecnt >= *alarm) {
	if (*pendn != 0) {
	    *pendn = 0;
	    *timecnt -= *alarm;
	    *alarm = space[*curel];
	}
	else {
	    *pendn = 1;
	    *timecnt -= *alarm;
	    (*curel)++;
	    if (*curel >= nms)
		*curel = 0;
	    *alarm = mark[*curel];
	}
    }
}
