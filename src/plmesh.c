/* $Id$
   $Log$
   Revision 1.1  1992/05/20 21:34:38  furnish
   Initial checkin of the whole PLPLOT project.

*/

/*	plmesh.c

	3d mesh plotter.
*/

#include "plplot.h"

#ifdef PLSTDC
#include <stdlib.h>
#ifdef INCLUDE_STDDEF
#include <stddef.h>
#endif
#ifdef INCLUDE_MALLOC
#include <malloc.h>
#endif

#else
extern char *malloc();
extern void free();
#endif

/*----------------------------------------------------------------------*\
* void plmesh(x, y, z, nx, ny, opt)
*
* Plots a mesh representation of the function z[x][y]. The x values 
* are stored as x[0..nx-1], the y values as y[0..ny-1], and the 
* z values are in the 2-d array z[][]. The integer "opt" specifies: 
*
*  opt = 1:  Draw lines parallel to x-axis 
*  opt = 2:  Draw lines parallel to y-axis 
*  opt = 3:  Draw lines parallel to both axes 
\*----------------------------------------------------------------------*/

void 
c_plmesh(x, y, z, nx, ny, opt)
PLINT nx, ny, opt;
PLFLT *x, *y, **z;
{
    PLINT *oldloview;

    spl3mode((PLINT) 1);
    plot3d(x, y, z, nx, ny, opt, 0);

    goldlovw(&oldloview);
    free((VOID *) oldloview);
    spl3mode((PLINT) 0);
}
