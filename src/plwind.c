/* $Id$
   $Log$
   Revision 1.3  1993/01/23 06:02:52  mjl
   Now holds all routines dealing with window specification.

 * Revision 1.2  1992/09/29  04:46:29  furnish
 * Massive clean up effort to remove support for garbage compilers (K&R).
 *
 * Revision 1.1  1992/05/20  21:35:01  furnish
 * Initial checkin of the whole PLPLOT project.
 *
*/

/*	plwind.c

	Routines for setting up world coordinates of the current viewport.
*/

#include "plplot.h"
#include <math.h>

#define  dtr   0.01745329252

/*----------------------------------------------------------------------*\
* void plwind()
*
* Set up world coordinates of the viewport boundaries (2d plots).
\*----------------------------------------------------------------------*/

void
c_plwind(PLFLT xmin, PLFLT xmax, PLFLT ymin, PLFLT ymax)
{
    PLINT vppxmi, vppxma, vppymi, vppyma;
    PLFLT dx, dy;
    PLFLT vpwxmi, vpwxma, vpwymi, vpwyma;
    PLFLT vpxmi, vpxma, vpymi, vpyma;
    PLFLT wpxscl, wpxoff, wpyscl, wpyoff;
    PLFLT wmxscl, wmxoff, wmyscl, wmyoff;
    PLINT level;

    glev(&level);
    if (level < 2)
	plexit("plwind: Please set up viewport first.");

    gvpp(&vppxmi, &vppxma, &vppymi, &vppyma);
    gvpd(&vpxmi, &vpxma, &vpymi, &vpyma);

    dx = (xmax - xmin) * 1.0e-5;
    dy = (ymax - ymin) * 1.0e-5;

    if ((xmin == xmax) || (ymin == ymax))
	plexit("plwind: Invalid window limits.");

/* The true plot window is made slightly larger than requested so that */
/* the end limits will be on the graph  */

    svpw((PLFLT) (xmin - dx), (PLFLT) (xmax + dx),
	 (PLFLT) (ymin - dy), (PLFLT) (ymax + dy));

    gvpw(&vpwxmi, &vpwxma, &vpwymi, &vpwyma);

/* Compute the scaling between coordinate systems */

    dx = vpwxma - vpwxmi;
    dy = vpwyma - vpwymi;

    wpxscl = (vppxma - vppxmi) / dx;
    wpxoff = (xmax * vppxmi - xmin * vppxma) / dx;
    wpyscl = (vppyma - vppymi) / dy;
    wpyoff = (ymax * vppymi - ymin * vppyma) / dy;
    swp(wpxscl, wpxoff, wpyscl, wpyoff);

    vpxmi = dcmmx(vpxmi);
    vpxma = dcmmx(vpxma);
    vpymi = dcmmy(vpymi);
    vpyma = dcmmy(vpyma);
    wmxscl = (vpxma - vpxmi) / dx;
    wmxoff = (xmax * vpxmi - xmin * vpxma) / dx;
    wmyscl = (vpyma - vpymi) / dy;
    wmyoff = (ymax * vpymi - ymin * vpyma) / dy;
    swm(wmxscl, wmxoff, wmyscl, wmyoff);

    slev(3);
}

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
c_plw3d(PLFLT basex, PLFLT basey, PLFLT height, PLFLT xmin0,
	PLFLT xmax0, PLFLT ymin0, PLFLT ymax0, PLFLT zmin0,
	PLFLT zmax0, PLFLT alt, PLFLT az)
{
    PLFLT xmin, xmax, ymin, ymax, zmin, zmax, d;
    PLFLT cx, cy, saz, caz, salt, calt, zscale;
    PLINT level;

    glev(&level);
    if (level < 3)
	plexit("plw3d: Please set up 2-d window first.");

    if (basex <= 0.0 || basey <= 0.0 || height <= 0.0)
	plexit("plw3d: Invalid world coordinate boxsize.");

    if (xmin0 == xmax0 || ymin0 == ymax0 || zmin0 == zmax0)
	plexit("plw3d: Invalid axis range.");

    if (alt < 0.0 || alt > 90.0)
	plexit("plw3d: Altitude must be between 0 and 90 degrees.");

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

    sbase(basex, basey, (PLFLT) (0.5 * (xmin + xmax)),
	  (PLFLT) (0.5 * (ymin + ymax)));

    sw3wc((PLFLT) (cx * caz), (PLFLT) (-cy * saz),
	  (PLFLT) (cx * saz * salt), (PLFLT) (cy * caz * salt),
	  (PLFLT) (zscale * calt));
}
