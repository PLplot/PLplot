/* $Id$

    Simple vector plot example
    Copyright (C) 2004 Andrew Ross <andrewross@users.sourceforge.net>

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

#include "plcdemos.h"
#include "plevent.h"

/* Variables and data arrays used by plot generators */

static const int nx = 20;
static const int ny = 20;
static const int nr = 20;
static const int ntheta = 20;
static const int nper = 100;
static const int nlevel = 10;

/* Pairs of points making the line segments used to plot the user defined arrow */
static PLFLT arrow_x[6] = {-0.5, 0.3, 0.3, 0.5, 0.3, 0.3};
static PLFLT arrow_y[6] = {0.0, 0.0,   0.2, 0.0, -0.2, 0.0};

/*--------------------------------------------------------------------------*\
 * main
 *
 * Generates several simple vector plots.  
\*--------------------------------------------------------------------------*/

f2mnmx(PLFLT **f, PLINT nx, PLINT ny, PLFLT *fmin, PLFLT *fmax)
{
    int i, j;

    *fmax = f[0][0];
    *fmin = *fmax;

    for (i = 0; i < nx; i++) {
        for (j = 0; j < ny; j++) {
	    *fmax = MAX(*fmax, f[i][j]);
	    *fmin = MIN(*fmin, f[i][j]);
	}
    }
}

/* Vector plot of the gradient of a shielded potential (see example 9) */
void potential() {
    int i,j;
    PLcGrid2 cgrid2;
    PLFLT eps, q1, d1, q1i, d1i, q2, d2, q2i, d2i;
    PLFLT div1, div1i, div2, div2i;
    PLFLT **u, **v, **z, r, theta, x, y, dz;
    PLFLT xmin, xmax, ymin, ymax, rmax, zmax, zmin;
    PLFLT px[nper], py[nper], clevel[nlevel];

    /* Create data to be plotted */
    plAlloc2dGrid(&cgrid2.xg,nr,ntheta);
    plAlloc2dGrid(&cgrid2.yg,nr,ntheta);
    plAlloc2dGrid(&z,nr,ntheta);
    plAlloc2dGrid(&u,nr,ntheta);
    plAlloc2dGrid(&v,nr,ntheta);

    cgrid2.nx = nr;
    cgrid2.ny = ntheta;

/* Potential inside a conducting cylinder (or sphere) by method of images.
 * Charge 1 is placed at (d1, d1), with image charge at (d2, d2).
 * Charge 2 is placed at (d1, -d1), with image charge at (d2, -d2).
 * Also put in smoothing term at small distances.
 */

   rmax = (double) nr;

   eps = 2.;

   q1 = 1.;
   d1 = rmax/4.;

   q1i = - q1*rmax/d1;
   d1i = pow(rmax, 2.)/d1;

   q2 = -1.;
   d2 = rmax/4.;

   q2i = - q2*rmax/d2;
   d2i = pow(rmax, 2.)/d2;

   for (i = 0; i < nr; i++) {
      r = 0.5 + (double) i;
      for (j = 0; j < ntheta; j++) {
	 theta = 2.*PI/(ntheta-1)*(0.5+(double)j);
	 x = r*cos(theta);
	 y = r*sin(theta);
	 cgrid2.xg[i][j] = x;
	 cgrid2.yg[i][j] = y;
         div1 = sqrt(pow(x-d1, 2.) + pow(y-d1, 2.) + pow(eps, 2.));
         div1i = sqrt(pow(x-d1i, 2.) + pow(y-d1i, 2.) + pow(eps, 2.));
         div2 = sqrt(pow(x-d2, 2.) + pow(y+d2, 2.) + pow(eps, 2.));
         div2i = sqrt(pow(x-d2i, 2.) + pow(y+d2i, 2.) + pow(eps, 2.));
	 z[i][j] = q1/div1 + q1i/div1i + q2/div2 + q2i/div2i;
         u[i][j] = -q1*(x-d1)/pow(div1,3.) - q1i*(x-d1i)/pow(div1i,3.0) 
		 - q2*(x-d2)/pow(div2,3.) - q2i*(x-d2i)/pow(div2i,3.);
         v[i][j] = -q1*(y-d1)/pow(div1,3.) - q1i*(y-d1i)/pow(div1i,3.0) 
		 - q2*(y+d2)/pow(div2,3.) - q2i*(y+d2i)/pow(div2i,3.);
      }
   }

    f2mnmx(cgrid2.xg, nr, ntheta, &xmin, &xmax);
    f2mnmx(cgrid2.yg, nr, ntheta, &ymin, &ymax);
    f2mnmx(z, nr, ntheta, &zmin, &zmax);

    plenv(xmin, xmax, ymin, ymax, 0, 0);
    pllab("(x)", "(y)", "#frPLplot Example 22 - potential gradient vector plot");
    /* Plot contours of the potential */
    dz = (zmax-zmin)/(double) nlevel;
    for (i = 0; i < nlevel; i++) {
        clevel[i] = zmin + ((double) i + 0.5)*dz;
    }
    plcol(3);
    pllsty(2);
    plcont(z,nr,ntheta,1,nr,1,ntheta,clevel,nlevel,pltr2,(void *) &cgrid2);
    pllsty(1);
    plcol0(1);
    
    /* Plot the vectors of the gradient of the potential */
    plcol0(2);
    plvect(u,v,nr,ntheta,25.0,pltr2,(void *)&cgrid2);
    plcol0(1);

    /* Plot the perimeter of the cylinder */
    for (i=0;i<nper;i++) {
	theta = (2.*PI/(nper-1))*(double)i;
	px[i] = rmax*cos(theta);
	py[i] = rmax*sin(theta);
    }
    plline(nper,px,py);
    
    plFree2dGrid(cgrid2.xg,nr,ntheta);
    plFree2dGrid(cgrid2.yg,nr,ntheta);
    plFree2dGrid(u,nr,ntheta);
    plFree2dGrid(v,nr,ntheta);

}

int
main(int argc, char *argv[])
{
    int i, j, npts;
    PLFLT x, y;
    PLFLT xmin, xmax, ymin, ymax, r, theta;
    PLINT narr, fill;
    PLcGrid2 cgrid2;
    PLFLT **u, **v;

/* Parse and process command line arguments */

    plParseOpts(&argc, argv, PL_PARSE_FULL);

/* Initialize plplot */

    plinit();

    plAlloc2dGrid(&cgrid2.xg,nx,ny);
    plAlloc2dGrid(&cgrid2.yg,nx,ny);
    plAlloc2dGrid(&u,nx,ny);
    plAlloc2dGrid(&v,nx,ny);

    cgrid2.nx = nx;
    cgrid2.ny = ny;

    npts = nx*ny;

    xmin = -1.6;
    xmax = 1.6;
    ymin = -0.55;
    ymax = 0.55;

/* Create the data to plot  - use the gradient of the rosen function in example 8 */
    for (i = 0; i<nx; i++) {
	for (j = 0; j<ny; j++) {
            x = 1.5*((double)(i-nx/2)+0.5)/((double) (nx/2));
	    y = 0.5*((double)(j-ny/2)+0.5)/((double) (ny/2));
	    cgrid2.xg[i][j] = x;
	    cgrid2.yg[i][j] = y;
	    u[i][j] = -2.0*(1.0-x)-200*(y-x*x);
	    v[i][j] = 200.0*(y-x*x);
	}
    }

/* Plot vectors using default arrow style */
    plenv(xmin, xmax, ymin, ymax, 0, 0);
    pllab("(x)", "(y)", "#frPLplot Example 22 - vector plot");
    plcol0(2);
    plvect(u,v,nx,ny,-0.5,pltr2,(void *)&cgrid2);
    plcol0(1);

    narr = 6;
    fill = 1;
	    
/* Create user defined arrow style and plot vectors using new style */
    plsvect(arrow_x, arrow_y, narr, fill);
    plenv(xmin, xmax, ymin, ymax, 0, 0);
    pllab("(x)", "(y)", "#frPLplot Example 22 - filled arrow");
    plcol0(2);
    plvect(u,v,nx,ny,0.0005,pltr2,(void *)&cgrid2);
    plcol0(1);

/* Example of polar plot */

    potential();

    plFree2dGrid(cgrid2.xg,nx,ny);
    plFree2dGrid(cgrid2.yg,nx,ny);
    plFree2dGrid(u,nx,ny);
    plFree2dGrid(v,nx,ny);

    plend();
    exit(0);
}
