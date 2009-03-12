/* $Id$

    Copyright (C) 2005  Alan W. Irwin

    This file is part of PLplot.

    PLplot is free software; you can redistribute it and/or modify
    it under the terms of the GNU Library General Public License as published
    by the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    PLplot is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Library General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    along with PLplot; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA

* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

    This file contains deprecated routines to provide backwards compatibility
    for a while.  For each routine the new routine you should be using instead
    is explicitly commented.
*/

#define NEED_PLDEBUG
#include "plplotP.h"

/*--------------------------------------------------------------------------*\
 * Use plparseopts instead.
\*--------------------------------------------------------------------------*/
int
plParseOpts(int *p_argc, const char **argv, PLINT mode)
{
   plwarn("plParseOpts: function deprecated. Use plparseopts instead");
   return c_plparseopts(p_argc, argv, mode);
}


/*--------------------------------------------------------------------------*\
 * Use plhlsrgb instead.
\*--------------------------------------------------------------------------*/
void
plHLS_RGB(PLFLT h, PLFLT l, PLFLT s, PLFLT *p_r, PLFLT *p_g, PLFLT *p_b)
{
   plwarn("plHLS_RGB: function deprecated. Use plhlsrgb instead");
   c_plhlsrgb(h, l, s, p_r, p_g, p_b);
}

/*--------------------------------------------------------------------------*\
 * Use plrgbhls instead.
\*--------------------------------------------------------------------------*/
void
plRGB_HLS(PLFLT r, PLFLT g, PLFLT b, PLFLT *p_h, PLFLT *p_l, PLFLT *p_s)
{
   plwarn("plRGB_HLS: function deprecated. Use plrgbhls instead");
   c_plrgbhls(r, g, b, p_h, p_l, p_s);
}

/*--------------------------------------------------------------------------*\
 * Use plvect / plsvect instead.
 * void plarrows()
 *
 * simple arrow plotter
 * copyright 1993 Wesley Ebisuzaki
 *
 * an arrow is defined by its location (x, y) and its direction (u, v)
 *
 * inputs:
 *   u[i], v[i]      arrow's horizontal and vertical projection
 *   x[i], y[i]      arrow's location (world coordinates)
 *   n               number of arrows to draw
 *   scale           > 0  scaling factor for arrows
 *                   0    default scaling factor
 *                   < 0  default scaling factor * (-scale)
 *   dx, dy          distance between arrows
 *                   used when calculating the default arrow scaling
 *                   so that arrows don't overlap
 *
\*--------------------------------------------------------------------------*/

#define SCALE0 2.0

/* definition of original arrow: 2 line segments */

static PLFLT arrow_x[4] = {0.5, -0.5, -0.27, -0.5};
static PLFLT arrow_y[4] = {0.0, 0.0, 0.0, 0.20};

void
plarrows(PLFLT *u, PLFLT *v, PLFLT *x, PLFLT *y, PLINT n,
	 PLFLT scale, PLFLT dx, PLFLT dy)
{
    PLFLT uu, vv;
    PLINT i, j, npts = 4;
    PLINT px0, py0, dpx, dpy;
    PLINT a_x[4], a_y[4];
    PLFLT max_u, max_v;
    double t;

    plwarn("plarrows: function deprecated. Use plvect instead");

    if (n <= 0) return;

    if (scale <= 0.0) {

    /* automatic scaling */
    /* find max / min values of data */

	max_u = u[0];
	max_v = v[0];
	for (i = 1; i < n; i++) {
	    t = fabs((double) u[i]);
	    max_u = t > max_u ? t : max_u;
	    t = fabs((double) v[i]);
	    max_v = t > max_v ? t : max_v;
	}

    /* measure distance in grid boxs */

	max_u = max_u / fabs( (double) dx);
	max_v = max_v / fabs( (double) dy);

	t = (max_u > max_v ? max_u : max_v);
	t = SCALE0 / t;
	if (scale < 0) {
	    scale = -scale * t;
	}
	else {
	    scale = t;
	}
    }
    pldebug("plarrows", "scale factor=%lf n=%d\n", scale,n);

    for (i = 0; i < n; i++) {
	uu = scale * u[i];
	vv = scale * v[i];
	if (uu == 0.0 && uu == 0.0) continue;

    /* conversion to physical coordinates */

	px0 = plP_wcpcx(x[i]);
	py0 = plP_wcpcy(y[i]);

	pldebug("plarrows", "%f %f %d %d\n",x[i],y[i],px0,py0);

	dpx = plP_wcpcx(x[i] + 0.5*uu) - px0;
	dpy = plP_wcpcy(y[i] + 0.5*vv) - py0;

    /* transform arrow -> a */

	for (j = 0; j < npts; j++) {
	    a_x[j] = (PLINT)(arrow_x[j] * dpx -
		arrow_y[j] * dpy + px0);
	    a_y[j] = (PLINT)(arrow_x[j] * dpy +
		arrow_y[j] * dpx + py0);
	}

    /* draw the arrow */
	plP_movphy(a_x[0], a_y[0]);
	plP_draphy(a_x[1], a_y[1]);
	plP_movphy(a_x[2], a_y[2]);
	plP_draphy(a_x[3], a_y[3]);
    }

}

