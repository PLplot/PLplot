/* $Id$
 * $Log$
 * Revision 1.11  1994/01/25 06:34:20  mjl
 * Dashed line generation desuckified!  Should now run much faster on some
 * architectures.  The previous loop used only conditionals, assignments, and
 * integer adds at the cost of a huge number of iterations (basically testing
 * each pixel).  The new method draws directly to the desired end of dash.
 * Contributed by Wesley Ebisuzaki.
 *
 * Revision 1.10  1994/01/17  19:27:15  mjl
 * Bug fix: changed declarations of xclp[] and yclp[] from static to local
 * variables in plP_pllclp() and plP_plfclp(). plP_pllclp can call itself
 * consequently it needs local variables. Fixed problem with dash line and
 * orientation=1.  (Submitted by Wesley Ebisuzaki)
 *
 * Revision 1.9  1993/07/31  08:18:34  mjl
 * Clipping routine for polygons added (preliminary).
 *
 * Revision 1.8  1993/07/01  22:18:13  mjl
 * Changed all plplot source files to include plplotP.h (private) rather than
 * plplot.h.  Rationalized namespace -- all externally-visible internal
 * plplot functions now start with "plP_".  Added new function plP_pllclp()
 * to clip a polyline within input clip limits and using the input function
 * to do the actual draw.  Used from the new driver interface functions.
*/

/*	plline.c

	Routines dealing with line generation.
*/

#include "plplotP.h"

#include <math.h>

#define INSIDE(ix,iy) (BETW(ix,xmin,xmax) && BETW(iy,ymin,ymax))

static PLINT xline[PL_MAXPOLYLINE], yline[PL_MAXPOLYLINE];

static PLINT lastx = UNDEFINED, lasty = UNDEFINED;

/* Function prototypes */
/* INDENT OFF */

static void pllclp	(PLINT *, PLINT *, PLINT);
static int  clipline	(PLINT *, PLINT *, PLINT *, PLINT *,
			 PLINT, PLINT, PLINT, PLINT);
static void genlin	(short *, short *, PLINT);
static void plupd	(PLINT, PLINT *, PLINT *, PLINT *,
			 PLINT *, PLINT *, PLINT *);
static void grdashline	(short *, short *, PLINT *, PLINT *, PLINT);

/* INDENT ON */

/*----------------------------------------------------------------------*\
* void pljoin()
*
* Draws a line segment from (x1, y1) to (x2, y2).
\*----------------------------------------------------------------------*/

void
c_pljoin(PLFLT x1, PLFLT y1, PLFLT x2, PLFLT y2)
{
    plP_movwor(x1, y1);
    plP_drawor(x2, y2);
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

    plP_glev(&level);
    if (level < 3)
	plexit("plline: Please set up window first.");

    plP_drawor_poly(x, y, n);
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

    plP_glev(&level);
    if (level < 1)
	plexit("plstyl: Please call plinit first.");

    if ((nms < 0) || (nms > 10))
	plexit("plstyl: Broken lines cannot have <0 or >10 elements");

    for (i = 0; i < nms; i++) {
	if ((mark[i] < 0) || (space[i] < 0))
	    plexit("plstyl: Mark and space lengths must be > 0.");
    }

    plP_smark(mark, space, nms);
    plP_scure(0, 1, 0, (nms > 0 ? mark[0] : 0));
}

/*----------------------------------------------------------------------*\
* void plP_movphy()
*
* Move to physical coordinates (x,y).
\*----------------------------------------------------------------------*/

void
plP_movphy(PLINT x, PLINT y)
{
    plP_scurr(x, y);
}

/*----------------------------------------------------------------------*\
* void plP_draphy()
*
* Draw to physical coordinates (x,y).
\*----------------------------------------------------------------------*/

void
plP_draphy(PLINT x, PLINT y)
{
    PLINT currx, curry;

    plP_gcurr(&currx, &curry);
    xline[0] = currx;
    xline[1] = x;
    yline[0] = curry;
    yline[1] = y;

    pllclp(xline, yline, 2);
}

/*----------------------------------------------------------------------*\
* void plP_movwor()
*
* Move to world coordinates (x,y).
\*----------------------------------------------------------------------*/

void
plP_movwor(PLFLT x, PLFLT y)
{
    plP_scurr(plP_wcpcx(x), plP_wcpcy(y));
}

/*----------------------------------------------------------------------*\
* void plP_drawor()
*
* Draw to world coordinates (x,y).
\*----------------------------------------------------------------------*/

void
plP_drawor(PLFLT x, PLFLT y)
{
    PLINT currx, curry;

    plP_gcurr(&currx, &curry);
    xline[0] = currx;
    xline[1] = plP_wcpcx(x);
    yline[0] = curry;
    yline[1] = plP_wcpcy(y);

    pllclp(xline, yline, 2);
}

/*----------------------------------------------------------------------*\
* void plP_draphy_poly()
*
* Draw polyline in physical coordinates.
* Need to draw buffers in increments of (PL_MAXPOLYLINE-1) since the
* last point must be repeated (for solid lines).
\*----------------------------------------------------------------------*/

void
plP_draphy_poly(PLINT *x, PLINT *y, PLINT n)
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
* void plP_drawor_poly()
*
* Draw polyline in world coordinates.
* Need to draw buffers in increments of (PL_MAXPOLYLINE-1) since the
* last point must be repeated (for solid lines).
\*----------------------------------------------------------------------*/

void
plP_drawor_poly(PLFLT *x, PLFLT *y, PLINT n)
{
    PLINT i, j, ib, ilim;

    for (ib = 0; ib < n; ib += PL_MAXPOLYLINE - 1) {
	ilim = MIN(PL_MAXPOLYLINE, n - ib);

	for (i = 0; i < ilim; i++) {
	    j = ib + i;
	    xline[i] = plP_wcpcx(x[j]);
	    yline[i] = plP_wcpcy(y[j]);
	}
	pllclp(xline, yline, ilim);
    }
}

/*----------------------------------------------------------------------*\
* void pllclp()
*
* Draws a polyline within the clip limits.
* Merely a front-end to plP_pllclp().
\*----------------------------------------------------------------------*/

static void
pllclp(PLINT *x, PLINT *y, PLINT npts)
{
    PLINT clpxmi, clpxma, clpymi, clpyma;

    plP_gclp(&clpxmi, &clpxma, &clpymi, &clpyma);
    plP_pllclp(x, y, npts, clpxmi, clpxma, clpymi, clpyma, genlin);
}

/*----------------------------------------------------------------------*\
* void plP_pllclp()
*
* Draws a polyline within the clip limits.
\*----------------------------------------------------------------------*/

void
plP_pllclp(PLINT *x, PLINT *y, PLINT npts,
	   PLINT xmin, PLINT xmax, PLINT ymin, PLINT ymax, 
	   void (*draw) (short *, short *, PLINT))
{
    PLINT x1, x2, y1, y2;
    PLINT i, iclp = 0;
    short xclp[PL_MAXPOLYLINE], yclp[PL_MAXPOLYLINE];
    int drawable;

    for (i = 0; i < npts - 1; i++) {
	x1 = x[i];
	x2 = x[i + 1];
	y1 = y[i];
	y2 = y[i + 1];

	drawable = (INSIDE(x1, y1) && INSIDE(x2, y2));
	if (!drawable)
	    drawable = !clipline(&x1, &y1, &x2, &y2, xmin, xmax, ymin, ymax);

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
   previous point, and if so, add it to the current polyline buffer. */

	    else if (x1 == xclp[iclp] && y1 == yclp[iclp]) {
		iclp++;
		xclp[iclp] = x2;
		yclp[iclp] = y2;
	    }

/* Otherwise it's time to start a new polyline */

	    else {
		if (iclp + 1 >= 2)
		    (*draw)(xclp, yclp, iclp + 1);
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
	(*draw)(xclp, yclp, iclp + 1);

    plP_scurr(x[npts-1], y[npts-1]);
}

/*----------------------------------------------------------------------*\
* void plP_plfclp()
*
* Fills a polygon within the clip limits.
\*----------------------------------------------------------------------*/

void
plP_plfclp(PLINT *x, PLINT *y, PLINT npts,
	   PLINT xmin, PLINT xmax, PLINT ymin, PLINT ymax, 
	   void (*draw) (short *, short *, PLINT))
{
    PLINT x1, x2, y1, y2;
    PLINT i, iclp = 0;
    short xclp[PL_MAXPOLYLINE], yclp[PL_MAXPOLYLINE];
    int drawable;

    iclp = 0;
    for (i = 0; i < npts - 1; i++) {
	x1 = x[i];
	x2 = x[i + 1];
	y1 = y[i];
	y2 = y[i + 1];

	drawable = (INSIDE(x1, y1) && INSIDE(x2, y2));
	if (!drawable)
	    drawable = !clipline(&x1, &y1, &x2, &y2, xmin, xmax, ymin, ymax);

	if (drawable) {

/* First point of polyline. */

	    if (iclp == 0) {
		xclp[iclp] = x1;
		yclp[iclp] = y1;
		iclp++;
		xclp[iclp] = x2;
		yclp[iclp] = y2;
	    }

/* Not first point.  If first point of this segment matches up to the
   previous point, just add it.  */

	    else if (x1 == xclp[iclp] && y1 == yclp[iclp]) {
		iclp++;
		xclp[iclp] = x2;
		yclp[iclp] = y2;
	    }

/* If not, we need to add both points, to connect the points in the
   polygon along the clip boundary.  */

	    else {
		iclp++;
		xclp[iclp] = x1;
		yclp[iclp] = y1;
		iclp++;
		xclp[iclp] = x2;
		yclp[iclp] = y2;
	    }
	}
    }

/* Draw the sucker */

    if (iclp + 1 >= 3)
	(*draw)(xclp, yclp, iclp + 1);
}

/*----------------------------------------------------------------------*\
* int clipline()
*
* Get clipped endpoints
\*----------------------------------------------------------------------*/

static int
clipline(PLINT *p_x1, PLINT *p_y1, PLINT *p_x2, PLINT *p_y2,
	 PLINT xmin, PLINT xmax, PLINT ymin, PLINT ymax)
{
    PLINT t, dx, dy, flipx, flipy;
    PLFLT slope;

/* If both points are outside clip region, return with an error */

    if ((*p_x1 <= xmin && *p_x2 <= xmin) ||
	(*p_x1 >= xmax && *p_x2 >= xmax) ||
	(*p_y1 <= ymin && *p_y2 <= ymin) ||
	(*p_y1 >= ymax && *p_y2 >= ymax))
	return (1);

    flipx = 0;
    flipy = 0;

    if (*p_x2 < *p_x1) {
	*p_x1 = 2 * xmin - *p_x1;
	*p_x2 = 2 * xmin - *p_x2;
	xmax = 2 * xmin - xmax;
	t = xmax;
	xmax = xmin;
	xmin = t;
	flipx = 1;
    }

    if (*p_y2 < *p_y1) {
	*p_y1 = 2 * ymin - *p_y1;
	*p_y2 = 2 * ymin - *p_y2;
	ymax = 2 * ymin - ymax;
	t = ymax;
	ymax = ymin;
	ymin = t;
	flipy = 1;
    }

    dx = *p_x2 - *p_x1;
    dy = *p_y2 - *p_y1;

    if (dx != 0)
	slope = (double) dy / (double) dx;

    if (*p_x1 < xmin) {
	if (dx != 0 && dy != 0)
	    *p_y1 = *p_y1 + ROUND(slope * (xmin - *p_x1));
	*p_x1 = xmin;
    }

    if (*p_y1 < ymin) {
	if (dx != 0 && dy != 0)
	    *p_x1 = *p_x1 + ROUND((ymin - *p_y1) / slope);
	*p_y1 = ymin;
    }

    if (*p_x1 >= xmax || *p_y1 >= ymax)
	return (1);

    if (*p_y2 > ymax) {
	if (dx != 0 && dy != 0)
	    *p_x2 = *p_x2 - ROUND((*p_y2 - ymax) / slope);
	*p_y2 = ymax;
    }

    if (*p_x2 > xmax) {
	if (dx != 0 && dy != 0)
	    *p_y2 = *p_y2 - ROUND((*p_x2 - xmax) * slope);
	*p_x2 = xmax;
    }

    if (flipx) {
	*p_x1 = 2 * xmax - *p_x1;
	*p_x2 = 2 * xmax - *p_x2;
    }

    if (flipy) {
	*p_y1 = 2 * ymax - *p_y1;
	*p_y2 = 2 * ymax - *p_y2;
    }

    return (0);
}

/*----------------------------------------------------------------------*\
* void genlin()
*
* General line-drawing routine.  Takes line styles into account.
* If only 2 points are in the polyline, it is more efficient to use
* plP_line() rather than plP_polyline().
\*----------------------------------------------------------------------*/

static void
genlin(short *x, short *y, PLINT npts)
{
    PLINT i, *mark, *space, nms;

/* Check for solid line */

    plP_gmark(&mark, &space, &nms);
    if (nms == 0) {
	if (npts== 2)
	    plP_line(x, y);
	else
	    plP_polyline(x, y, npts);
    }

/* Right now dashed lines don't use polyline capability -- this
   should be improved */

    else {
	for (i = 0; i < npts - 1; i++) {
	    grdashline(x+i, y+i, mark, space, nms);
	}
    }
}

/*----------------------------------------------------------------------*\
* void grdashline()
\*----------------------------------------------------------------------*/

static void
grdashline(short *x, short *y, PLINT *mark, PLINT *space, PLINT nms)
{
    PLINT nx, ny, nxp, nyp;
    PLINT *timecnt, *alarm, *pendn, *curel;
    PLINT modulo, dx, dy, i, xtmp, ytmp;
    PLINT tstep, pix_distance, j;
    PLINT umx, umy;
    int loop_x;
    long incr, temp;
    short xl[2], yl[2];
    double nxstep, nystep;

/* Check if pattern needs to be restarted */

    plP_gcure(&curel, &pendn, &timecnt, &alarm);
    if (x[0] != lastx || y[0] != lasty) {
	*curel = 0;
	*pendn = 1;
	*timecnt = 0;
	*alarm = mark[0];
    }

    lastx = xtmp = x[0];
    lasty = ytmp = y[0];

    if (x[0] == x[1] && y[0] == y[1])
	return;

    nx = x[1] - x[0];
    dx = (nx > 0) ? 1 : -1;
    nxp = ABS(nx);

    ny = y[1] - y[0];
    dy = (ny > 0) ? 1 : -1;
    nyp = ABS(ny);

    if (nyp > nxp) {
	modulo = nyp;
	incr = nxp;
	loop_x = 0;
    }
    else {
	modulo = nxp;
	incr = nyp;
	loop_x = 1;
    }

    temp = modulo / 2;

/* Compute the timer step */

    plP_gumpix(&umx, &umy);
    nxstep = nxp * umx;
    nystep = nyp * umy;
    tstep = sqrt( nxstep * nxstep + nystep * nystep ) / modulo;
    if (tstep < 1) tstep = 1;
    /* tstep is distance per pixel moved */

    i = 0;
    while (i < modulo) {
        pix_distance = (*alarm - *timecnt + tstep - 1) / tstep;
	i += pix_distance;
	if (i > modulo)
	    pix_distance -= (i - modulo);
	*timecnt += pix_distance * tstep;

	temp += pix_distance * incr;
	j = temp / modulo;
	temp = temp % modulo;

	if (loop_x) {
	    xtmp += pix_distance * dx;
	    ytmp += j * dy;
	}
	else {
	    xtmp += j * dx;
	    ytmp += pix_distance * dy;
	}
	if (*pendn != 0) {
	    xl[0] = lastx;
	    yl[0] = lasty;
	    xl[1] = xtmp;
	    yl[1] = ytmp;
	    plP_line(xl, yl);
	}
	plupd(nms, mark, space, curel, pendn, timecnt, alarm);
	lastx = xtmp;
	lasty = ytmp;
    }
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
