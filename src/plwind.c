/* $Id$
 * $Log$
 * Revision 1.9  1994/11/02 19:58:16  mjl
 * Changed plAddCWindow() call syntax.
 *
 * Revision 1.8  1994/07/29  20:29:24  mjl
 * Change so that window coordinates are added to the window list each time
 * plwind() is called.  Contributed by Paul Casteels.
 *
 * Revision 1.7  1994/06/30  18:22:24  mjl
 * All core source files: made another pass to eliminate warnings when using
 * gcc -Wall.  Lots of cleaning up: got rid of includes of math.h or string.h
 * (now included by plplot.h), and other minor changes.  Now each file has
 * global access to the plstream pointer via extern; many accessor functions
 * eliminated as a result.
*/

/*	plwind.c

	Routines for setting up world coordinates of the current viewport.
*/

#include "plplotP.h"

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
    PLFLT vpxmi, vpxma, vpymi, vpyma;
    PLFLT wmxscl, wmxoff, wmyscl, wmyoff;

    if (plsc->level < 2) {
	plabort("plwind: Please set up viewport first");
	return;
    }

    plP_gvpp(&vppxmi, &vppxma, &vppymi, &vppyma);
    plP_gvpd(&vpxmi, &vpxma, &vpymi, &vpyma);

/* Best to just warn and recover on bounds errors */

    if (xmin == xmax) {
	plwarn("plwind: Invalid window limits in x.");
	xmin--; xmax++;
    }
    if (ymin == ymax) {
	plwarn("plwind: Invalid window limits in y.");
	ymin--; ymax++;
    }

    dx = (xmax - xmin) * 1.0e-5;
    dy = (ymax - ymin) * 1.0e-5;

/* The true plot window is made slightly larger than requested so that */
/* the end limits will be on the graph  */

    plsc->vpwxmi = xmin - dx;
    plsc->vpwxma = xmax + dx;
    plsc->vpwymi = ymin - dy;
    plsc->vpwyma = ymax + dy;

/* Compute the scaling between coordinate systems */

    dx = plsc->vpwxma - plsc->vpwxmi;
    dy = plsc->vpwyma - plsc->vpwymi;

    plsc->wpxscl = (vppxma - vppxmi) / dx;
    plsc->wpxoff = (xmax * vppxmi - xmin * vppxma) / dx;
    plsc->wpyscl = (vppyma - vppymi) / dy;
    plsc->wpyoff = (ymax * vppymi - ymin * vppyma) / dy;

    vpxmi = plP_dcmmx(vpxmi);
    vpxma = plP_dcmmx(vpxma);
    vpymi = plP_dcmmy(vpymi);
    vpyma = plP_dcmmy(vpyma);

    wmxscl = (vpxma - vpxmi) / dx;
    wmxoff = (xmax * vpxmi - xmin * vpxma) / dx;
    wmyscl = (vpyma - vpymi) / dy;
    wmyoff = (ymax * vpymi - ymin * vpyma) / dy;

    plP_swm(wmxscl, wmxoff, wmyscl, wmyoff);

/* Add coordinates of window to windows list */

    plAddCWindow();

    plsc->level = 3;
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

    if (plsc->level < 3) {
	plabort("plw3d: Please set up 2-d window first");
	return;
    }
    if (basex <= 0.0 || basey <= 0.0 || height <= 0.0) {
	plabort("plw3d: Invalid world coordinate boxsize");
	return;
    }
    if (xmin0 == xmax0 || ymin0 == ymax0 || zmin0 == zmax0) {
	plabort("plw3d: Invalid axis range");
	return;
    }
    if (alt < 0.0 || alt > 90.0) {
	plabort("plw3d: Altitude must be between 0 and 90 degrees");
	return;
    }

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

    plsc->domxmi = xmin;
    plsc->domxma = xmax;
    plsc->domymi = ymin;
    plsc->domyma = ymax;
    plsc->zzscl = zscale;
    plsc->ranmi = zmin;
    plsc->ranma = zmax;

    plsc->base3x = basex;
    plsc->base3y = basey;
    plsc->basecx = 0.5 * (xmin + xmax);
    plsc->basecy = 0.5 * (ymin + ymax);

    plsc->cxx = cx * caz;
    plsc->cxy = -cy * saz;
    plsc->cyx = cx * saz * salt;
    plsc->cyy = cy * caz * salt;
    plsc->cyz = zscale * calt;
}
