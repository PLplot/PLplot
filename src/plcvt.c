/* $Id$
   $Log$
   Revision 1.3  1993/11/15 08:39:23  mjl
   Documentation changes.

 * Revision 1.2  1993/07/01  22:13:35  mjl
 * Changed all plplot source files to include plplotP.h (private) rather than
 * plplot.h.  Rationalized namespace -- all externally-visible internal
 * plplot functions now start with "plP_".
 *
 * Revision 1.1  1993/01/23  05:52:53  mjl
 * File for holding coordinate transformers (was: convert.c).
 *
*/

/*	plcvt.c

	Coordinate transformation routines.
*/

#include "plplotP.h"

static PLINT phyxmi, phyxma, phyymi, phyyma;
static PLFLT spdxmi, spdxma, spdymi, spdyma;
static PLFLT xpmm, ypmm;
static PLFLT wpxscl, wpxoff, wpyscl, wpyoff;
static PLFLT dpxscl, dpxoff, dpyscl, dpyoff;
static PLFLT mpxscl, mpxoff, mpyscl, mpyoff;
static PLFLT wmxscl, wmxoff, wmyscl, wmyoff;
static PLFLT base3x, base3y, basecx, basecy;
static PLFLT zzscl, ranmi, ranma;
static PLFLT cxx, cxy, cyx, cyy, cyz;

/*----------------------------------------------------------------------*\
*  Coordinate transformations for plotting package.
*  Conversion routines yielding an integer result.
\*----------------------------------------------------------------------*/

/* device coords to physical coords (x) */

PLINT
plP_dcpcx(PLFLT x)
{
    plP_gdp(&dpxscl, &dpxoff, &dpyscl, &dpyoff);
    return (ROUND(dpxoff + dpxscl * x));
}

/* device coords to physical coords (y) */

PLINT
plP_dcpcy(PLFLT y)
{
    plP_gdp(&dpxscl, &dpxoff, &dpyscl, &dpyoff);
    return (ROUND(dpyoff + dpyscl * y));
}

/* millimeters from bottom left-hand corner to physical coords (x) */

PLINT
plP_mmpcx(PLFLT x)
{
    plP_gmp(&mpxscl, &mpxoff, &mpyscl, &mpyoff);
    return (ROUND(mpxoff + mpxscl * x));
}

/* millimeters from bottom left-hand corner to physical coords (y) */

PLINT
plP_mmpcy(PLFLT y)
{
    plP_gmp(&mpxscl, &mpxoff, &mpyscl, &mpyoff);
    return (ROUND(mpyoff + mpyscl * y));
}

/* world coords to physical coords (x) */

PLINT
plP_wcpcx(PLFLT x)
{
    plP_gwp(&wpxscl, &wpxoff, &wpyscl, &wpyoff);
    return (ROUND(wpxoff + wpxscl * x));
}

/* world coords to physical coords (y) */

PLINT
plP_wcpcy(PLFLT y)
{
    plP_gwp(&wpxscl, &wpxoff, &wpyscl, &wpyoff);
    return (ROUND(wpyoff + wpyscl * y));
}

/*----------------------------------------------------------------------*\
*  Coordinate transformations for plotting package.
*  Conversion routines yielding an floating result.
\*----------------------------------------------------------------------*/

/* device coords to millimeters from bottom left-hand corner (x) */

PLFLT
plP_dcmmx(PLFLT x)
{
    plP_gphy(&phyxmi, &phyxma, &phyymi, &phyyma);
    plP_gpixmm(&xpmm, &ypmm);
    return ((PLFLT) (x * ABS(phyxma - phyxmi) / xpmm));
}

/* device coords to millimeters from bottom left-hand corner (y) */

PLFLT
plP_dcmmy(PLFLT y)
{
    plP_gphy(&phyxmi, &phyxma, &phyymi, &phyyma);
    plP_gpixmm(&xpmm, &ypmm);
    return ((PLFLT) (y * ABS(phyyma - phyymi) / ypmm));
}

/* define transformations between device coords and subpage coords (x) */

PLFLT
plP_dcscx(PLFLT x)
{
    plP_gspd(&spdxmi, &spdxma, &spdymi, &spdyma);
    return ((PLFLT) ((x - spdxmi) / (spdxma - spdxmi)));
}

/* define transformations between device coords and subpage coords (y) */

PLFLT
plP_dcscy(PLFLT y)
{
    plP_gspd(&spdxmi, &spdxma, &spdymi, &spdyma);
    return ((PLFLT) ((y - spdymi) / (spdyma - spdymi)));
}

/* millimeters from bottom left corner into device coords (x) */

PLFLT
plP_mmdcx(PLFLT x)
{
    plP_gphy(&phyxmi, &phyxma, &phyymi, &phyyma);
    plP_gpixmm(&xpmm, &ypmm);
    return ((PLFLT) (x * xpmm / ABS(phyxma - phyxmi)));
}

/* millimeters from bottom left corner into device coords (y) */

PLFLT
plP_mmdcy(PLFLT y)
{
    plP_gphy(&phyxmi, &phyxma, &phyymi, &phyyma);
    plP_gpixmm(&xpmm, &ypmm);
    return ((PLFLT) (y * ypmm / ABS(phyyma - phyymi)));
}

/* subpage coords to device coords (x) */

PLFLT
plP_scdcx(PLFLT x)
{
    plP_gspd(&spdxmi, &spdxma, &spdymi, &spdyma);
    return ((PLFLT) (spdxmi + (spdxma - spdxmi) * x));
}

/* subpage coords to device coords (y) */

PLFLT
plP_scdcy(PLFLT y)
{
    plP_gspd(&spdxmi, &spdxma, &spdymi, &spdyma);
    return ((PLFLT) (spdymi + (spdyma - spdymi) * y));
}

/* world coords into millimeters (x) */

PLFLT
plP_wcmmx(PLFLT x)
{
    plP_gwm(&wmxscl, &wmxoff, &wmyscl, &wmyoff);
    return ((PLFLT) (wmxoff + wmxscl * x));
}

/* world coords into millimeters (y) */

PLFLT
plP_wcmmy(PLFLT y)
{
    plP_gwm(&wmxscl, &wmxoff, &wmyscl, &wmyoff);
    return ((PLFLT) (wmyoff + wmyscl * y));
}

/* undocumented transformation for 3d plot routines (x) */

PLFLT
plP_w3wcx(PLFLT x, PLFLT y, PLFLT z)
{
    plP_gw3wc(&cxx, &cxy, &cyx, &cyy, &cyz);
    plP_gbase(&base3x, &base3y, &basecx, &basecy);
    return ((PLFLT) ((x - basecx) * cxx + (y - basecy) * cxy));
}

/* undocumented transformation for 3d plot routines (y) */

PLFLT
plP_w3wcy(PLFLT x, PLFLT y, PLFLT z)
{
    plP_gw3wc(&cxx, &cxy, &cyx, &cyy, &cyz);
    plP_gbase(&base3x, &base3y, &basecx, &basecy);
    plP_grange(&zzscl, &ranmi, &ranma);
    return ((PLFLT) ((x - basecx) * cyx + (y - basecy) * cyy +
		     (z - ranmi) * cyz));
}
