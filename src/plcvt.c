/* $Id$
   $Log$
   Revision 1.1  1993/01/23 05:52:53  mjl
   File for holding coordinate transformers (was: convert.c).

*/

/*	plcvt.c

	Coordinate transformation routines.
*/

#include "plplot.h"

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
dcpcx(PLFLT x)
{
    gdp(&dpxscl, &dpxoff, &dpyscl, &dpyoff);
    return (ROUND(dpxoff + dpxscl * x));
}

PLINT
dcpcy(PLFLT y)
{
    gdp(&dpxscl, &dpxoff, &dpyscl, &dpyoff);
    return (ROUND(dpyoff + dpyscl * y));
}

/* mmpc. converts millimetres from bottom left-hand corner to physical */
/* coordinates */

PLINT
mmpcx(PLFLT x)
{
    gmp(&mpxscl, &mpxoff, &mpyscl, &mpyoff);
    return (ROUND(mpxoff + mpxscl * x));
}

PLINT
mmpcy(PLFLT y)
{
    gmp(&mpxscl, &mpxoff, &mpyscl, &mpyoff);
    return (ROUND(mpyoff + mpyscl * y));
}

/* wcpc. converts world coordinates to physical coordinates */

PLINT
wcpcx(PLFLT x)
{
    gwp(&wpxscl, &wpxoff, &wpyscl, &wpyoff);
    return (ROUND(wpxoff + wpxscl * x));
}

PLINT
wcpcy(PLFLT y)
{
    gwp(&wpxscl, &wpxoff, &wpyscl, &wpyoff);
    return (ROUND(wpyoff + wpyscl * y));
}

/*----------------------------------------------------------------------*\
*  Coordinate transformations for plotting package.
*  Conversion routines yielding an floating result.
\*----------------------------------------------------------------------*/

/* dcmm. converts from device coordinates to millimetres from bottom */
/* left-hand corner */

PLFLT
dcmmx(PLFLT x)
{
    gphy(&phyxmi, &phyxma, &phyymi, &phyyma);
    gpixmm(&xpmm, &ypmm);
    return ((PLFLT) (x * ABS(phyxma - phyxmi) / xpmm));
}

PLFLT
dcmmy(PLFLT y)
{
    gphy(&phyxmi, &phyxma, &phyymi, &phyyma);
    gpixmm(&xpmm, &ypmm);
    return ((PLFLT) (y * ABS(phyyma - phyymi) / ypmm));
}

/* dcsc. define transformations between device coordinates and subpage */
/* coordinates */

PLFLT
dcscx(PLFLT x)
{
    gspd(&spdxmi, &spdxma, &spdymi, &spdyma);
    return ((PLFLT) ((x - spdxmi) / (spdxma - spdxmi)));
}

PLFLT
dcscy(PLFLT y)
{
    gspd(&spdxmi, &spdxma, &spdymi, &spdyma);
    return ((PLFLT) ((y - spdymi) / (spdyma - spdymi)));
}

/* mmdc. converts millimetres from bottom left corner into device */
/* coordinates */

PLFLT
mmdcx(PLFLT x)
{
    gphy(&phyxmi, &phyxma, &phyymi, &phyyma);
    gpixmm(&xpmm, &ypmm);
    return ((PLFLT) (x * xpmm / ABS(phyxma - phyxmi)));
}

PLFLT
mmdcy(PLFLT y)
{
    gphy(&phyxmi, &phyxma, &phyymi, &phyyma);
    gpixmm(&xpmm, &ypmm);
    return ((PLFLT) (y * ypmm / ABS(phyyma - phyymi)));
}

/* scdc. converts subpage coordinates to device coordinates */

PLFLT
scdcx(PLFLT x)
{
    gspd(&spdxmi, &spdxma, &spdymi, &spdyma);
    return ((PLFLT) (spdxmi + (spdxma - spdxmi) * x));
}

PLFLT
scdcy(PLFLT y)
{
    gspd(&spdxmi, &spdxma, &spdymi, &spdyma);
    return ((PLFLT) (spdymi + (spdyma - spdymi) * y));
}

/* wcmm. converts world coordinates into millimetres */

PLFLT
wcmmx(PLFLT x)
{
    gwm(&wmxscl, &wmxoff, &wmyscl, &wmyoff);
    return ((PLFLT) (wmxoff + wmxscl * x));
}

PLFLT
wcmmy(PLFLT y)
{
    gwm(&wmxscl, &wmxoff, &wmyscl, &wmyoff);
    return ((PLFLT) (wmyoff + wmyscl * y));
}

PLFLT
w3wcx(PLFLT x, PLFLT y, PLFLT z)
{
    gw3wc(&cxx, &cxy, &cyx, &cyy, &cyz);
    gbase(&base3x, &base3y, &basecx, &basecy);
    return ((PLFLT) ((x - basecx) * cxx + (y - basecy) * cxy));
}

PLFLT
w3wcy(PLFLT x, PLFLT y, PLFLT z)
{
    gw3wc(&cxx, &cxy, &cyx, &cyy, &cyz);
    gbase(&base3x, &base3y, &basecx, &basecy);
    grange(&zzscl, &ranmi, &ranma);
    return ((PLFLT) ((x - basecx) * cyx + (y - basecy) * cyy + (z - ranmi) * cyz));
}
