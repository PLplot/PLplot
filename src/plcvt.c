/* $Id$
 * $Log$
 * Revision 1.4  1994/06/30 18:22:05  mjl
 * All core source files: made another pass to eliminate warnings when using
 * gcc -Wall.  Lots of cleaning up: got rid of includes of math.h or string.h
 * (now included by plplot.h), and other minor changes.  Now each file has
 * global access to the plstream pointer via extern; many accessor functions
 * eliminated as a result.
 *
*/

/*	plcvt.c

	Coordinate transformation routines.
*/

#include "plplotP.h"

/*----------------------------------------------------------------------*\
 *  Coordinate transformations for plotting package.
 *  Conversion routines yielding an integer result.
\*----------------------------------------------------------------------*/

/* device coords to physical coords (x) */

PLINT
plP_dcpcx(PLFLT x)
{
    return (ROUND(plsc->dpxoff + plsc->dpxscl * x));
}

/* device coords to physical coords (y) */

PLINT
plP_dcpcy(PLFLT y)
{
    return (ROUND(plsc->dpyoff + plsc->dpyscl * y));
}

/* millimeters from bottom left-hand corner to physical coords (x) */

PLINT
plP_mmpcx(PLFLT x)
{
    return (ROUND(plsc->mpxoff + plsc->mpxscl * x));
}

/* millimeters from bottom left-hand corner to physical coords (y) */

PLINT
plP_mmpcy(PLFLT y)
{
    return (ROUND(plsc->mpyoff + plsc->mpyscl * y));
}

/* world coords to physical coords (x) */

PLINT
plP_wcpcx(PLFLT x)
{
    return (ROUND(plsc->wpxoff + plsc->wpxscl * x));
}

/* world coords to physical coords (y) */

PLINT
plP_wcpcy(PLFLT y)
{
    return (ROUND(plsc->wpyoff + plsc->wpyscl * y));
}

/*----------------------------------------------------------------------*\
 *  Coordinate transformations for plotting package.
 *  Conversion routines yielding an floating result.
\*----------------------------------------------------------------------*/

/* device coords to millimeters from bottom left-hand corner (x) */

PLFLT
plP_dcmmx(PLFLT x)
{
    return ((PLFLT) (x * ABS(plsc->phyxma - plsc->phyxmi) / plsc->xpmm));
}

/* device coords to millimeters from bottom left-hand corner (y) */

PLFLT
plP_dcmmy(PLFLT y)
{
    return ((PLFLT) (y * ABS(plsc->phyyma - plsc->phyymi) / plsc->ypmm));
}

/* define transformations between device coords and subpage coords (x) */

PLFLT
plP_dcscx(PLFLT x)
{
    return ((PLFLT) ((x - plsc->spdxmi) / (plsc->spdxma - plsc->spdxmi)));
}

/* define transformations between device coords and subpage coords (y) */

PLFLT
plP_dcscy(PLFLT y)
{
    return ((PLFLT) ((y - plsc->spdymi) / (plsc->spdyma - plsc->spdymi)));
}

/* millimeters from bottom left corner into device coords (x) */

PLFLT
plP_mmdcx(PLFLT x)
{
    return ((PLFLT) (x * plsc->xpmm / ABS(plsc->phyxma - plsc->phyxmi)));
}

/* millimeters from bottom left corner into device coords (y) */

PLFLT
plP_mmdcy(PLFLT y)
{
    return ((PLFLT) (y * plsc->ypmm / ABS(plsc->phyyma - plsc->phyymi)));
}

/* subpage coords to device coords (x) */

PLFLT
plP_scdcx(PLFLT x)
{
    return ((PLFLT) (plsc->spdxmi + (plsc->spdxma - plsc->spdxmi) * x));
}

/* subpage coords to device coords (y) */

PLFLT
plP_scdcy(PLFLT y)
{
    return ((PLFLT) (plsc->spdymi + (plsc->spdyma - plsc->spdymi) * y));
}

/* world coords into millimeters (x) */

PLFLT
plP_wcmmx(PLFLT x)
{
    return ((PLFLT) (plsc->wmxoff + plsc->wmxscl * x));
}

/* world coords into millimeters (y) */

PLFLT
plP_wcmmy(PLFLT y)
{
    return ((PLFLT) (plsc->wmyoff + plsc->wmyscl * y));
}

/* undocumented transformation for 3d plot routines (x) */

PLFLT
plP_w3wcx(PLFLT x, PLFLT y, PLFLT z)
{
    return ((PLFLT) ((x - plsc->basecx) * plsc->cxx +
		     (y - plsc->basecy) * plsc->cxy));
}

/* undocumented transformation for 3d plot routines (y) */

PLFLT
plP_w3wcy(PLFLT x, PLFLT y, PLFLT z)
{
    return ((PLFLT) ((x - plsc->basecx) * plsc->cyx +
		     (y - plsc->basecy) * plsc->cyy +
		     (z - plsc->ranmi) * plsc->cyz));
}
