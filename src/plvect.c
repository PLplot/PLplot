/* $Id$

	Vector plotting routines.

   Copyright (C) 2004  Andrew Ross

   This file is part of PLplot.

   PLplot is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Library Public License as published
   by the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   PLplot is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with PLplot; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
*/

#define NEED_PLDEBUG
#include "plplotP.h"
#include <float.h>
#include <ctype.h>

/* Static function prototypes */

void plP_plotvect(PLFLT x, PLFLT y, PLFLT u, PLFLT v, PLFLT scale);
    
void plvecf_int(PLFLT (*plf2eval) (PLINT, PLINT, PLPointer), 
		PLPointer f2evalv_data, PLPointer f2evalc_data,
		PLINT nx, PLINT ny, PLFLT scale,
		void (*pltr) (PLFLT, PLFLT, PLFLT *, PLFLT *, PLPointer),
		PLPointer pltr_data);

/*--------------------------------------------------------------------------*\
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

void
plarrows(PLFLT *u, PLFLT *v, PLFLT *x, PLFLT *y, PLINT n,
	 PLFLT scale, PLFLT dx, PLFLT dy)
{
    PLFLT uu, vv;
    PLINT i, j, npts;
    PLINT px0, py0, dpx, dpy;
    PLFLT *arrow_x, *arrow_y;
    PLINT *a_x, *a_y;
    PLFLT max_u, max_v;
    double t;

    if (n <= 0) return;

    if (plsc->level < 3) {
        plabort("plarrows: Please set up window first");
        return;
    }

    arrow_x = plsc->arrow_x;
    arrow_y = plsc->arrow_y;
    npts = plsc->arrow_npts;

    a_x = (PLINT *) malloc(npts*sizeof(PLINT));
    a_y = (PLINT *) malloc(npts*sizeof(PLINT));

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
	    a_x[j] = arrow_x[j] * dpx -
		arrow_y[j] * dpy + px0;
	    a_y[j] = arrow_x[j] * dpy +
		arrow_y[j] * dpx + py0;
	}

    /* draw the arrow */
        plP_draphy_poly(a_x,a_y,npts);
	if (plsc->arrow_fill) {
	    plP_plfclp(a_x, a_y, npts, plsc->clpxmi, plsc->clpxma,
	               plsc->clpymi, plsc->clpyma, plP_fill);
	}
    }

    free_mem(a_x);
    free_mem(a_y);
}

/*--------------------------------------------------------------------------*\
 * void plsarrow()
 *
 * Set the style of the arrow used by plarrows
\*--------------------------------------------------------------------------*/

void
plsarrow(PLFLT *arrowx, PLFLT *arrowy, PLINT npts, PLINT fill) {
    int i;

    if (plsc->arrow_x) free_mem(plsc->arrow_x);
    if (plsc->arrow_y) free_mem(plsc->arrow_y);

    plsc->arrow_x = (PLFLT *)malloc(npts*sizeof(PLFLT));
    plsc->arrow_y = (PLFLT *)malloc(npts*sizeof(PLFLT));

    plsc->arrow_npts = npts;
    plsc->arrow_fill = fill;
    for (i=0; i<npts; i++) {
      plsc->arrow_x[i] = arrowx[i];
      plsc->arrow_y[i] = arrowy[i];
    }
}

/*
 * Plot an individual vector
 */
void
plP_plotvect(PLFLT x, PLFLT y, PLFLT u, PLFLT v, PLFLT scale) {
	
    PLFLT uu, vv, px0, py0, dpx, dpy;
    PLINT a_x[plsc->arrow_npts], a_y[plsc->arrow_npts];
    int i,j;

    uu = scale*u;
    vv = scale*v;

    if(uu == 0.0 && vv == 0.0) return;

    px0 = plP_wcpcx(x);
    py0 = plP_wcpcy(y);

    pldebug("plP_plotvect", "%f %f %d %d\n",x,y,px0,py0);

    dpx = plP_wcpcx(x + 0.5*uu) - px0;
    dpy = plP_wcpcy(y + 0.5*vv) - py0;

    /* transform arrow -> a */

    for (j = 0; j < plsc->arrow_npts; j++) {
        a_x[j] = plsc->arrow_x[j] * dpx - plsc->arrow_y[j] * dpy + px0;
	a_y[j] = plsc->arrow_x[j] * dpy + plsc->arrow_y[j] * dpx + py0;
    }

    /* draw the arrow */
    plP_draphy_poly(a_x,a_y,plsc->arrow_npts);
    if (plsc->arrow_fill) {
	plP_plfclp(a_x, a_y, plsc->arrow_npts, plsc->clpxmi, plsc->clpxma,
	plsc->clpymi, plsc->clpyma, plP_fill);
    }

}

/*
 * void plvecf_int()
 *
 * Internal routine to plot a vector array with arbitrary coordinate 
 * and vector transformations
 */
void plvecf_int(PLFLT (*plf2eval) (PLINT, PLINT, PLPointer), 
		PLPointer f2eval_data1, PLPointer f2eval_data2,
		PLINT nx, PLINT ny, PLFLT scale,
		void (*pltr) (PLFLT, PLFLT, PLFLT *, PLFLT *, PLPointer),
		PLPointer pltr_data) {
    PLINT i, j;
    PLFLT **u, **v, **x, **y; 
    PLFLT lscale, dx, dy, dxmin, dymin, umax, vmax;

    plAlloc2dGrid(&u, nx, ny);
    plAlloc2dGrid(&v, nx, ny);
    plAlloc2dGrid(&x, nx, ny);
    plAlloc2dGrid(&y, nx, ny);

    for (j=0; j<ny; j++) {
        for (i=0 ;i<nx ;i++) {
	    u[i][j] = plf2eval(i,j,f2eval_data1);
	    v[i][j] = plf2eval(i,j,f2eval_data2);
	    pltr((PLFLT) i, (PLFLT) j, &x[i][j], &y[i][j], pltr_data);
	}
    }
	    
    /* Calculate apropriate scaling if necessary */
    if (scale <= 0.0 ) {
	if (nx <= 1 && ny <= 1) {
	    fprintf(stderr,"plvecf_int: not enough points for autoscaling\n");
	    return;
	}
	dxmin = x[1][0]-x[0][0];
	dymin = y[0][1]-y[0][0];
        for (j=0; j<ny-1; j++) {
            for (i=0 ;i<nx-1 ;i++) {
		dx = (x[i+1][j]-x[i][j]);
		dy = (y[i][j+1]-y[i][j]);
		dxmin = (dx<dxmin)?dx:dxmin;
		dymin = (dy<dymin)?dy:dymin;
	    }
	}
	umax = u[0][0];
	vmax = v[0][0];
        for (j=0; j<ny; j++) {
            for (i=0 ;i<nx ;i++) {
		umax = (u[i][j]>umax)?u[i][j]:umax;
		vmax = (v[i][j]>vmax)?v[i][j]:vmax;
	    }
	}
	umax = umax/dxmin;
	vmax = vmax/dymin;
	lscale = (umax<vmax)?umax:vmax;
	lscale = 2.0/lscale;
	if (scale < 0.0) {
	    scale = -scale*lscale;
	}
	else {
	    scale = lscale;
	}
    }

    for (j=0; j<ny; j++) {
        for (i=0 ;i<nx ;i++) {
	    plP_plotvect(x[i][j],y[i][j],u[i][j],v[i][j],scale);
	}
    }

    plFree2dGrid(u, nx, ny);
    plFree2dGrid(v, nx, ny);
    plFree2dGrid(x, nx, ny);
    plFree2dGrid(y, nx, ny);

}

void
plarrows2(PLFLT **u, PLFLT **v, PLINT nx, PLINT ny, PLFLT scale, 
	void (*pltr) (PLFLT, PLFLT, PLFLT *, PLFLT *, PLPointer),
	PLPointer pltr_data)
{
    PLfGrid2 grid1, grid2;

    grid1.f = u;
    grid2.f = v;

    plvecf_int(plf2eval2, (PLPointer) &grid1, (PLPointer) &grid2, 
	       nx, ny, scale, pltr, pltr_data);
}
