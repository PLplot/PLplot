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
   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
*/

#define NEED_PLDEBUG
#include "plplotP.h"
#include <float.h>
#include <ctype.h>

/* Static function prototypes */

static void plP_plotvect(PLFLT x, PLFLT y, PLFLT u, PLFLT v, PLFLT scale);

/*--------------------------------------------------------------------------*\
 * void c_plsvect()
 *
 * Set the style of the arrow used by plvect
\*--------------------------------------------------------------------------*/

void
c_plsvect(PLFLT *arrowx, PLFLT *arrowy, PLINT npts, PLINT fill) {
    int i;

    if (plsc->arrow_x) free_mem(plsc->arrow_x);
    if (plsc->arrow_y) free_mem(plsc->arrow_y);

    if (((plsc->arrow_x = (PLFLT *)malloc(npts*sizeof(PLFLT)))==NULL)||
        ((plsc->arrow_y = (PLFLT *)malloc(npts*sizeof(PLFLT)))==NULL))
        {
          plexit("c_plsvect: Insufficient memory");
        }

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
static void
plP_plotvect(PLFLT x, PLFLT y, PLFLT u, PLFLT v, PLFLT scale) {
	
    PLFLT uu, vv, px0, py0, dpx, dpy;
    PLINT *a_x, *a_y;
    int j;

    uu = scale*u;
    vv = scale*v;

    if(uu == 0.0 && vv == 0.0) return;

    if (((a_x = (PLINT *)malloc(sizeof(PLINT)*(plsc->arrow_npts)))==NULL)||
        ((a_y = (PLINT *)malloc(sizeof(PLINT)*(plsc->arrow_npts)))==NULL))
        {
          plexit("plP_plotvect: Insufficient memory");
        }

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

    free((void *)a_x);
    free((void *)a_y);

}

/*
 * void plfvect()
 *
 * Internal routine to plot a vector array with arbitrary coordinate
 * and vector transformations
 */
void plfvect(PLFLT (*plf2eval) (PLINT, PLINT, PLPointer),
		PLPointer f2eval_data1, PLPointer f2eval_data2,
		PLINT nx, PLINT ny, PLFLT scale,
		void (*pltr) (PLFLT, PLFLT, PLFLT *, PLFLT *, PLPointer),
		PLPointer pltr_data) {
    PLINT i, j, i1, j1;
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
            fprintf(stderr,"plfvect: not enough points for autoscaling\n");
            return;
        }
	dxmin = 10E10;
	dymin = 10E10;
        for (j=0; j<ny; j++) {
            for (i=0 ;i<nx ;i++) {
                for (j1=j; j1<ny; j1++) {
                    for (i1=0 ;i1<nx ;i1++) {
		        dx = fabs(x[i1][j1]-x[i][j]);
		        dy = fabs(y[i1][j1]-y[i][j]);
			if (dx > 0) {
		            dxmin = (dx<dxmin)?dx:dxmin;
			}
			if (dy > 0) {
		            dymin = (dy<dymin)?dy:dymin;
			}
		    }
		}
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
	lscale = 1.5/lscale;
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
c_plvect(PLFLT **u, PLFLT **v, PLINT nx, PLINT ny, PLFLT scale,
	void (*pltr) (PLFLT, PLFLT, PLFLT *, PLFLT *, PLPointer),
	PLPointer pltr_data)
{
    PLfGrid2 grid1, grid2;

    grid1.f = u;
    grid2.f = v;

    plfvect(plf2eval2, (PLPointer) &grid1, (PLPointer) &grid2,
	       nx, ny, scale, pltr, pltr_data);
}
