/* $Id$
   $Log$
   Revision 1.2  1993/07/01 22:13:35  mjl
   Changed all plplot source files to include plplotP.h (private) rather than
   plplot.h.  Rationalized namespace -- all externally-visible internal
   plplot functions now start with "plP_".

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

/* dcpc. converts device coordinates to physical coordinates */

PLINT
plP_dcpcx(PLFLT x)
{
    plP_gdp(&dpxscl, &dpxoff, &dpyscl, &dpyoff);
    return (ROUND(dpxoff + dpxscl * x));
}

PLINT
plP_dcpcy(PLFLT y)
{
    plP_gdp(&dpxscl, &dpxoff, &dpyscl, &dpyoff);
    return (ROUND(dpyoff + dpyscl * y));
}

/* mmpc. converts millimetres from bottom left-hand corner to physical */
/* coordinates */

PLINT
plP_mmpcx(PLFLT x)
{
    plP_gmp(&mpxscl, &mpxoff, &mpyscl, &mpyoff);
    return (ROUND(mpxoff + mpxscl * x));
}

PLINT
plP_mmpcy(PLFLT y)
{
    plP_gmp(&mpxscl, &mpxoff, &mpyscl, &mpyoff);
    return (ROUND(mpyoff + mpyscl * y));
}

/* wcpc. converts world coordinates to physical coordinates */

PLINT
plP_wcpcx(PLFLT x)
{
    plP_gwp(&wpxscl, &wpxoff, &wpyscl, &wpyoff);
    return (ROUND(wpxoff + wpxscl * x));
}

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

/* dcmm. converts from device coordinates to millimetres from bottom */
/* left-hand corner */

PLFLT
plP_dcmmx(PLFLT x)
{
    plP_gphy(&phyxmi, &phyxma, &phyymi, &phyyma);
    plP_gpixmm(&xpmm, &ypmm);
    return ((PLFLT) (x * ABS(phyxma - phyxmi) / xpmm));
}

PLFLT
plP_dcmmy(PLFLT y)
{
    plP_gphy(&phyxmi, &phyxma, &phyymi, &phyyma);
    plP_gpixmm(&xpmm, &ypmm);
    return ((PLFLT) (y * ABS(phyyma - phyymi) / ypmm));
}

/* dcsc. define transformations between device coordinates and subpage */
/* coordinates */

PLFLT
plP_dcscx(PLFLT x)
{
    plP_gspd(&spdxmi, &spdxma, &spdymi, &spdyma);
    return ((PLFLT) ((x - spdxmi) / (spdxma - spdxmi)));
}

PLFLT
plP_dcscy(PLFLT y)
{
    plP_gspd(&spdxmi, &spdxma, &spdymi, &spdyma);
    return ((PLFLT) ((y - spdymi) / (spdyma - spdymi)));
}

/* mmdc. converts millimetres from bottom left corner into device */
/* coordinates */

PLFLT
plP_mmdcx(PLFLT x)
{
    plP_gphy(&phyxmi, &phyxma, &phyymi, &phyyma);
    plP_gpixmm(&xpmm, &ypmm);
    return ((PLFLT) (x * xpmm / ABS(phyxma - phyxmi)));
}

PLFLT
plP_mmdcy(PLFLT y)
{
    plP_gphy(&phyxmi, &phyxma, &phyymi, &phyyma);
    plP_gpixmm(&xpmm, &ypmm);
    return ((PLFLT) (y * ypmm / ABS(phyyma - phyymi)));
}

/* scdc. converts subpage coordinates to device coordinates */

PLFLT
plP_scdcx(PLFLT x)
{
    plP_gspd(&spdxmi, &spdxma, &spdymi, &spdyma);
    return ((PLFLT) (spdxmi + (spdxma - spdxmi) * x));
}

PLFLT
plP_scdcy(PLFLT y)
{
    plP_gspd(&spdxmi, &spdxma, &spdymi, &spdyma);
    return ((PLFLT) (spdymi + (spdyma - spdymi) * y));
}

/* wcmm. converts world coordinates into millimetres */

PLFLT
plP_wcmmx(PLFLT x)
{
    plP_gwm(&wmxscl, &wmxoff, &wmyscl, &wmyoff);
    return ((PLFLT) (wmxoff + wmxscl * x));
}

PLFLT
plP_wcmmy(PLFLT y)
{
    plP_gwm(&wmxscl, &wmxoff, &wmyscl, &wmyoff);
    return ((PLFLT) (wmyoff + wmyscl * y));
}

PLFLT
plP_w3wcx(PLFLT x, PLFLT y, PLFLT z)
{
    plP_gw3wc(&cxx, &cxy, &cyx, &cyy, &cyz);
    plP_gbase(&base3x, &base3y, &basecx, &basecy);
    return ((PLFLT) ((x - basecx) * cxx + (y - basecy) * cxy));
}

PLFLT
plP_w3wcy(PLFLT x, PLFLT y, PLFLT z)
{
    plP_gw3wc(&cxx, &cxy, &cyx, &cyy, &cyz);
    plP_gbase(&base3x, &base3y, &basecx, &basecy);
    plP_grange(&zzscl, &ranmi, &ranma);
    return ((PLFLT) ((x - basecx) * cyx + (y - basecy) * cyy + (z - ranmi) * cyz));
}
