/* $Id$
   $Log$
   Revision 1.3  1992/09/30 18:25:51  furnish
   Massive cleanup to irradicate garbage code.  Almost everything is now
   prototyped correctly.  Builds on HPUX, SUNOS (gcc), AIX, and UNICOS.

 * Revision 1.2  1992/09/29  04:46:07  furnish
 * Massive clean up effort to remove support for garbage compilers (K&R).
 *
 * Revision 1.1  1992/05/20  21:34:38  furnish
 * Initial checkin of the whole PLPLOT project.
 *
*/

/*	plmesh.c

	3d mesh plotter.
*/

#include <stdlib.h>

#define PL_NEED_MALLOC
#include "plplot.h"

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
c_plmesh( PLFLT *x, PLFLT *y, PLFLT **z, PLINT nx, PLINT ny, PLINT opt )
{
    PLINT *oldloview;

    spl3mode((PLINT) 1);
    plot3d(x, y, z, nx, ny, opt, 0);

    goldlovw(&oldloview);
    free((VOID *) oldloview);
    spl3mode((PLINT) 0);
}
