/* $Id:

    Simple arrow plot example
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

static PLFLT x[100], y[100], u[100], v[100];

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
    int i,j,k, nx, ny, npts;
    PLFLT dx, dy;
    PLFLT xmin, xmax, ymin, ymax;
    PLINT narr, fill;

/* Parse and process command line arguments */

    plParseOpts(&argc, argv, PL_PARSE_FULL);

/* Initialize plplot */

    plinit();

    dx = 1.0;
    dy = 1.0;

    nx = 10;
    ny = 10;
    npts = nx*ny;

    xmin = -nx/2*dx;
    xmax = nx/2*dx;
    ymin = -ny/2*dy;
    ymax = ny/2*dy;

/* Create the data to plot */
    k = 0;
    for (i = 0; i<nx; i++) {
	for (j = 0; j<ny; j++) {
	    x[k] = (i-nx/2+0.5)*dx;
	    y[k] = (j-ny/2+0.5)*dy;
	    u[k] = y[k];
	    v[k] = -x[k];
	    k++;
	}
    }

/* Plot vectors using default arrow style */
    plenv(xmin, xmax, ymin, ymax, 0, 0);
    pllab("(x)", "(y)", "#frPLplot Example 22 - vector plot");
    plarrows(u,v,x,y,npts,-0.5,dx,dy);

    narr = 6;
    fill = 0;

/* Create user defined arrow style and plot vectors using new style */
    plsarrow(arrow_x, arrow_y, narr, fill);
    plenv(xmin, xmax, ymin, ymax, 0, 0);
    pllab("(x)", "(y)", "#frPLplot Example 22 - user defined arrow");
    plarrows(u,v,x,y,npts,-0.5,dx,dy);

    fill = 1;
	    
/* Create user defined arrow style and plot vectors using new style */
    plsarrow(arrow2_x, arrow2_y, narr, fill);
    plenv(xmin, xmax, ymin, ymax, 0, 0);
    pllab("(x)", "(y)", "#frPLplot Example 22 - filled arrow");
    plarrows(u,v,x,y,npts,-0.5,dx,dy);

    plend();
    exit(0);
}
