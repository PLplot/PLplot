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

static const int nx = 10;
static const int ny = 10;

/* Pairs of points making the line segments used to plot the user defined arrow */
static PLFLT arrow_x[6] = {-0.5, 0.5, 0.3, 0.5, 0.3, 0.5};
static PLFLT arrow_y[6] = {0.0, 0.0,   0.2, 0.0, -0.2, 0.0};
static PLFLT arrow2_x[6] = {-0.5, 0.3, 0.3, 0.5, 0.3, 0.3};
static PLFLT arrow2_y[6] = {0.0, 0.0,   0.2, 0.0, -0.2, 0.0};

/*--------------------------------------------------------------------------*\
 * main
 *
 * Generates several simple vector plots.  
\*--------------------------------------------------------------------------*/

int
main(int argc, char *argv[])
{
    int i, j, npts;
    PLFLT dx, dy, dr;
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

    dx = 1.0;
    dy = 1.0;

    npts = nx*ny;

    xmin = -nx/2*dx;
    xmax = nx/2*dx;
    ymin = -ny/2*dy;
    ymax = ny/2*dy;

/* Create the data to plot */
    for (i = 0; i<nx; i++) {
	for (j = 0; j<ny; j++) {
	    cgrid2.xg[i][j] = (i-nx/2+0.5)*dx;
	    cgrid2.yg[i][j] = (j-ny/2+0.5)*dy;
	    u[i][j] = cgrid2.yg[i][j];
	    v[i][j] = -cgrid2.xg[i][j];
	}
    }

/* Plot vectors using default arrow style */
    plenv(xmin, xmax, ymin, ymax, 0, 0);
    pllab("(x)", "(y)", "#frPLplot Example 22 - vector plot");
    plcol0(2);
    plvect(u,v,nx,ny,0.0,pltr2,(void *)&cgrid2);
    plcol0(1);

    narr = 6;
    fill = 0;

/* Create user defined arrow style and plot vectors using new style */
    plsvect(arrow_x, arrow_y, narr, fill);
    plenv(xmin, xmax, ymin, ymax, 0, 0);
    pllab("(x)", "(y)", "#frPLplot Example 22 - user defined arrow");
    plcol0(2);
    plvect(u,v,nx,ny,-0.5,pltr2,(void *)&cgrid2);
    plcol0(1);

    fill = 1;
	    
/* Create user defined arrow style and plot vectors using new style */
    plsvect(arrow2_x, arrow2_y, narr, fill);
    plenv(xmin, xmax, ymin, ymax, 0, 0);
    pllab("(x)", "(y)", "#frPLplot Example 22 - filled arrow");
    plcol0(2);
    plvect(u,v,nx,ny,0.3,pltr2,(void *)&cgrid2);
    plcol0(1);

/* Example of polar plot */

/* Create the data to plot */
    dr = 0.5;

    for (i = 0; i<nx; i++) {
        r = i*dr;
	for (j = 0; j<ny; j++) {
	    theta = 2.*PI/(ny-1)*(double)j;
	    cgrid2.xg[i][j] = r*cos(theta);
	    cgrid2.yg[i][j] = r*sin(theta);
	    u[i][j] = cgrid2.yg[i][j];
	    v[i][j] = -cgrid2.xg[i][j];
	}
    }

    xmin = -nx*dr;
    xmax = nx*dr;
    ymin = -nx*dr;
    ymax = nx*dr;

    plenv(xmin, xmax, ymin, ymax, 0, 0);
    pllab("(x)", "(y)", "#frPLplot Example 22 - polar vector plot");
    plcol0(2);
    plvect(u,v,nx,ny,0.5,pltr2,(void *)&cgrid2);
    plcol0(1);
    
    plFree2dGrid(cgrid2.xg,nx,ny);
    plFree2dGrid(cgrid2.yg,nx,ny);
    plFree2dGrid(u,nx,ny);
    plFree2dGrid(v,nx,ny);

    plend();
    exit(0);
}
