//---------------------------------------------------------------------------//
// $Id: 
//    Simple arrow plot example
//---------------------------------------------------------------------------//
//
//---------------------------------------------------------------------------//
// Copyright (C) 2004  Andrew Ross <andrewross@users.sourceforge.net>
//
// This file is part of PLplot.
//
// PLplot is free software; you can redistribute it and/or modify
// it under the terms of the GNU Library General Public License as published by
// the Free Software Foundation; version 2 of the License.
//
// PLplot is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Library General Public License for more details.
//
// You should have received a copy of the GNU Library General Public License
// along with PLplot; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307  USA
//---------------------------------------------------------------------------//
//
//---------------------------------------------------------------------------//
// Implementation of PLplot example 22 in C++.
//---------------------------------------------------------------------------//

#include "plstream.h"
#include "plevent.h"

#include <cstdlib>
#include <cctype>
#include <cmath>

using namespace std;

static int locate_mode;
static int test_xor;
static int fontset;
static char *f_name;

static char *notes[] = {"Make sure you get it right!", ""};

class x22 {

public:
  x22(int, char **);

private:
  plstream *pls;

};


x22::x22( int argc, char ** argv ) {

    PLFLT *x, *y, *u, *v;

    // Set of points making a polygon to use as the arrow
    PLFLT arrow_x[6] = {-0.5, 0.5, 0.3, 0.5, 0.3, 0.5};
    PLFLT arrow_y[6] = {0.0, 0.0, 0.2, 0.0, -0.2, 0.0};
    PLFLT arrow2_x[6] = {-0.5, 0.3, 0.3, 0.5, 0.3, 0.3};
    PLFLT arrow2_y[6] = {0.0, 0.0,   0.2, 0.0, -0.2, 0.0};

    int i,j,k, nx, ny, npts;
    PLFLT dx, dy;
    PLFLT xmin, xmax, ymin, ymax;
    PLINT narr, fill;

    // Create new plstream
    pls = new plstream();

    // Parse and process command line arguments 

    pls->ParseOpts(&argc, argv, PL_PARSE_FULL);

    // Initialize plplot 

    pls->init();

    dx = 1.0;
    dy = 1.0;

    nx = 10;
    ny = 10;
    npts = nx*ny;

    // Allocate arrays
    x = new PLFLT[npts];
    y = new PLFLT[npts];
    u = new PLFLT[npts];
    v = new PLFLT[npts];

    xmin = -nx/2*dx;
    xmax = nx/2*dx;
    ymin = -ny/2*dy;
    ymax = ny/2*dy;

    // Create data - cirulation around the origin.
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

    // Plot vectors with default arrows
    pls->env(xmin, xmax, ymin, ymax, 0, 0);
    pls->lab("(x)", "(y)", "#frPLplot Example 22 - vector plot");
    pls->arrows(u,v,x,y,npts,-0.5,dx,dy);

    narr = 6;
    fill = 0;

    // Set arrow style using arrow_x and arrow_y then 
    // plot using these arrows.
    pls->sarrow(arrow_x, arrow_y, narr, fill);
    pls->env(xmin, xmax, ymin, ymax, 0, 0);
    pls->lab("(x)", "(y)", "#frPLplot Example 22 - vector plot");
    pls->arrows(u,v,x,y,npts,-0.5,dx,dy);

    fill = 1;

    // Set arrow style using arrow2_x and arrow2_y then 
    // plot using these filled arrows.
    pls->sarrow(arrow2_x, arrow2_y, narr, fill);
    pls->env(xmin, xmax, ymin, ymax, 0, 0);
    pls->lab("(x)", "(y)", "#frPLplot Example 22 - vector plot");
    pls->arrows(u,v,x,y,npts,-0.5,dx,dy);

    delete[] x;
    delete[] y;
    delete[] u;
    delete[] v;

    delete pls;
    exit(0);
}

int main( int argc, char ** argv ) {
  x22 *x = new x22( argc, argv );
  delete x;

}


//---------------------------------------------------------------------------//
//                              End of x22.cc
//---------------------------------------------------------------------------//

