/* $Id$
   $Log$
   Revision 1.2  1992/09/29 04:45:39  furnish
   Massive clean up effort to remove support for garbage compilers (K&R).

 * Revision 1.1  1992/05/20  21:34:08  furnish
 * Initial checkin of the whole PLPLOT project.
 *
*/

/*	genlin.c

	General line-drawing routine.
	Takes line styles into account.
*/

#include "plplot.h"
#include <math.h>

static PLINT lastx = -100000, lasty = -100000;
static void plupd();

void 
genlin (PLINT x1, PLINT y1, PLINT x2, PLINT y2)
{
    PLINT nx, ny;
    PLINT *mark, *space, *nms;
    PLINT *timecnt, *alarm, *pendn, *curel;
    PLINT modulo, incr, dx, dy, diax, diay, i, temp, x, y;
    PLINT px, py, tstep;
    PLINT umx, umy;

/* Check for solid line */

    gmark(&mark, &space, &nms);
    if (*nms == 0) {
	grline(x1, y1, x2, y2);
	return;
    }

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

    if (x1 == x2 && y1 == y2) return;

    nx = x2 - x1;
    ny = y2 - y1;

    if (ABS(ny) > ABS(nx)) {
	modulo = ABS(ny);
	incr = ABS(nx);
	dx = 0;
	dy = (ny > 0) ? 1 : -1;
    }
    else {
	modulo = ABS(nx);
	incr = ABS(ny);
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
    tstep = sqrt(pow((double) (ABS(nx) * umx), 2.) +
		 pow((double) (ABS(ny) * umy), 2.)) / modulo;

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
	    plupd(*nms, mark, space, curel, pendn, timecnt, alarm);
	    if (*pendn == 0) grline(lastx, lasty, px, py);
	    lastx = x;
	    lasty = y;
	}
    }

/* Finish up */

    if (*pendn != 0) grline(lastx, lasty, x, y);
    lastx = x2;
    lasty = y2;
}

/* Updates line style variables, called whenever alarm goes off */

static void 
plupd (PLINT nms, PLINT *mark, PLINT *space, PLINT *curel, PLINT *pendn, PLINT *timecnt, PLINT *alarm)
{
    while ( *timecnt >= *alarm ) {
	if (*pendn != 0) {
	    *pendn = 0;
	    *timecnt -= *alarm;
	    *alarm = space[*curel];
	}
	else {
	    *pendn = 1;
	    *timecnt -= *alarm;
	    (*curel)++;
	    if (*curel >= nms) *curel = 0;
	    *alarm = mark[*curel];
	}
    }
}
