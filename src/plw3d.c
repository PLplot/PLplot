/* $Id$
   $Log$
   Revision 1.2.2.1  2001/04/19 12:31:46  rlaboiss
   First merge against MAIN

   Revision 1.2  1992/09/29 04:46:27  furnish
   Massive clean up effort to remove support for garbage compilers (K&R).

 * Revision 1.1  1992/05/20  21:34:59  furnish
 * Initial checkin of the whole PLPLOT project.
 *
*/

/*	plw3d.c

	Sets up window for 3d plots.
*/

#include "plplot.h"
#include <math.h>

#define  dtr   0.01745329252

/*----------------------------------------------------------------------*\
* void plw3d()
*
* Set up a window for three-dimensional plotting. The data are mapped 
* into a box with world coordinate size "basex" by "basey" by "height", 
* with the base being symmetrically positioned about zero. Thus 
* the mapping between data 3-d and world 3-d coordinates is given by: 
*
*   x = xmin   =>   wx = -0.5*basex 
*   x = xmax   =>   wx =  0.5*basex 
*   y = ymin   =>   wy = -0.5*basey 
*   y = ymax   =>   wy =  0.5*basey 
*   z = zmin   =>   wz =  0.0 
*   z = zmax   =>   wz =  height 
*
* The world coordinate box is then viewed from position "alt"-"az", 
* measured in degrees. For proper operation, 0 <= alt <= 90 degrees, 
* but az can be any value. 
\*----------------------------------------------------------------------*/

void
c_plw3d (PLFLT basex, PLFLT basey, PLFLT height, PLFLT xmin0, 
	PLFLT xmax0, PLFLT ymin0, PLFLT ymax0, PLFLT zmin0, 
	PLFLT zmax0, PLFLT alt, PLFLT az)
{
    PLFLT xmin, xmax, ymin, ymax, zmin, zmax, d;
    PLFLT cx, cy, saz, caz, salt, calt, zscale;
    PLINT level;

    glev(&level);
    if (level < 3)
	plexit("Please set up 2-d window before calling plw3d.");
    if (basex <= 0.0 || basey <= 0.0 || height <= 0.0)
	plexit("Invalid world coordinate boxsize in plw3d.");
    if (xmin0 == xmax0 || ymin0 == ymax0 || zmin0 == zmax0)
	plexit("Invalid axis range in plw3d.");
    if (alt < 0.0 || alt > 90.0)
	plexit("Altitude must be between 0 and 90 degrees in plw3d.");

    d = 1.0e-5 * (xmax0 - xmin0);
    xmax = xmax0 + d;
    xmin = xmin0 - d;
    d = 1.0e-5 * (ymax0 - ymin0);
    ymax = ymax0 + d;
    ymin = ymin0 - d;
    d = 1.0e-5 * (zmax0 - zmin0);
    zmax = zmax0 + d;
    zmin = zmin0 - d;
    cx = basex / (xmax - xmin);
    cy = basey / (ymax - ymin);
    zscale = height / (zmax - zmin);
    saz = sin(dtr * az);
    caz = cos(dtr * az);
    salt = sin(dtr * alt);
    calt = cos(dtr * alt);

    sdom(xmin, xmax, ymin, ymax);
    srange(zscale, zmin, zmax);
    sbase(basex, basey, (PLFLT) (0.5 * (xmin + xmax)), (PLFLT) (0.5 * (ymin + ymax)));

    sw3wc((PLFLT) (cx * caz), (PLFLT) (-cy * saz), (PLFLT) (cx * saz * salt), (PLFLT) (cy * caz * salt), (PLFLT) (zscale * calt));
}
