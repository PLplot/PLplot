/* $Id$
   $Log$
   Revision 1.2.2.1  2001/04/19 12:31:46  rlaboiss
   First merge against MAIN

   Revision 1.2  1992/09/29 04:46:22  furnish
   Massive clean up effort to remove support for garbage compilers (K&R).

 * Revision 1.1  1992/05/20  21:34:52  furnish
 * Initial checkin of the whole PLPLOT project.
 *
*/

/*	plt3zz.c

	Draw the next zig-zag line for 3-d plot.
*/

#include "plplot.h"

/*----------------------------------------------------------------------*\
* void plt3zz()
*
* Draws the next zig-zag line for a 3-d plot. The data is stored in array
* z[][] as a function of x[] and y[]. This is plotted out starting at
* index (xstar0,ystar0).
*
* Depending on the state of "flg0", the sequence of data points
* sent to plnxtv is altered so as to allow cross-hatch plotting,
* or plotting parallel to either the x-axis or the y-axis.
\*----------------------------------------------------------------------*/

void 
plt3zz (PLINT xstar0, PLINT ystar0, PLINT dx, PLINT dy, PLINT flg0, PLINT init, PLFLT *x, PLFLT *y, PLFLT **z, PLINT nx, PLINT ny, PLINT *u, PLINT *v)
{
    PLINT flag;
    PLINT n;
    PLINT xstart, ystart;

    n = 0;
    xstart = xstar0;
    ystart = ystar0;
    flag = flg0;

lab1:
    if (1 <= xstart && xstart <= nx && 1 <= ystart && ystart <= ny) {
	u[n] = wcpcx(w3wcx(
		x[xstart - 1], y[ystart - 1], z[xstart - 1][ystart - 1]));

	v[n] = wcpcy(w3wcy(
		x[xstart - 1], y[ystart - 1], z[xstart - 1][ystart - 1]));

	if (flag == -3) {
	    ystart = ystart + dy;
	    flag = -flag;
	}
	else if (flag == -2)
	    ystart = ystart + dy;

	else if (flag == -1) {
	    ystart = ystart + dy;
	    flag = 1;
	}
	else if (flag == 1)
	    xstart = xstart + dx;

	else if (flag == 2) {
	    xstart = xstart + dx;
	    flag = -2;
	}
	else if (flag == 3) {
	    xstart = xstart + dx;
	    flag = -flag;
	}
	n = n + 1;
	goto lab1;
    }

    if (flag == 1 || flag == -2) {
	if (flag == 1) {
	    xstart = xstart - dx;
	    ystart = ystart + dy;
	}
	else if (flag == -2) {
	    ystart = ystart - dy;
	    xstart = xstart + dx;
	}

	if (1 <= xstart && xstart <= nx && 1 <= ystart && ystart <= ny) {
	    u[n] = wcpcx(w3wcx(
		x[xstart - 1], y[ystart - 1], z[xstart - 1][ystart - 1]));

	    v[n] = wcpcy(w3wcy(
		x[xstart - 1], y[ystart - 1], z[xstart - 1][ystart - 1]));

	    n = n + 1;
	}
    }
    plnxtv(u, v, n, init);
}
