/* $Id$
   $Log$
   Revision 1.1  1992/05/20 21:34:07  furnish
   Initial checkin of the whole PLPLOT project.

*/

/*	convert.c

	Coordinate transformation routines.
*/

#include <math.h>
#include "plplot.h"

static PLINT phyxmi, phyxma, phyymi, phyyma;
static PLFLT spdxmi, spdxma, spdymi, spdyma;
static PLFLT vpdxmi, vpdxma, vpdymi, vpdyma;
static PLFLT vpwxmi, vpwxma, vpwymi, vpwyma;
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

#ifdef PLSTDC
PLINT
dcpcx(PLFLT x)
#else
PLINT 
dcpcx(x)
PLFLT x;
#endif
{
    gdp(&dpxscl, &dpxoff, &dpyscl, &dpyoff);
    return (ROUND(dpxoff + dpxscl * x));
}

#ifdef PLSTDC
PLINT
dcpcy(PLFLT y)
#else
PLINT 
dcpcy(y)
PLFLT y;
#endif
{
    gdp(&dpxscl, &dpxoff, &dpyscl, &dpyoff);
    return (ROUND(dpyoff + dpyscl * y));
}

/* mmpc. converts millimetres from bottom left-hand corner to physical */
/* coordinates */

#ifdef PLSTDC
PLINT
mmpcx(PLFLT x)
#else
PLINT 
mmpcx(x)
PLFLT x;
#endif
{
    gmp(&mpxscl, &mpxoff, &mpyscl, &mpyoff);
    return (ROUND(mpxoff + mpxscl * x));
}

#ifdef PLSTDC
PLINT
mmpcy(PLFLT y)
#else
PLINT 
mmpcy(y)
PLFLT y;
#endif
{
    gmp(&mpxscl, &mpxoff, &mpyscl, &mpyoff);
    return (ROUND(mpyoff + mpyscl * y));
}

/* wcpc. converts world coordinates to physical coordinates */

#ifdef PLSTDC
PLINT
wcpcx(PLFLT x)
#else
PLINT 
wcpcx(x)
PLFLT x;
#endif
{
    gwp(&wpxscl, &wpxoff, &wpyscl, &wpyoff);
    return (ROUND(wpxoff + wpxscl * x));
}

#ifdef PLSTDC
PLINT
wcpcy(PLFLT y)
#else
PLINT 
wcpcy(y)
PLFLT y;
#endif
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

#ifdef PLSTDC
PLFLT
dcmmx(PLFLT x)
#else
PLFLT 
dcmmx(x)
PLFLT x;
#endif
{
    gphy(&phyxmi, &phyxma, &phyymi, &phyyma);
    gpixmm(&xpmm, &ypmm);
    return ((PLFLT) (x * ABS(phyxma - phyxmi) / xpmm));
}

#ifdef PLSTDC
PLFLT
dcmmy(PLFLT y)
#else
PLFLT 
dcmmy(y)
PLFLT y;
#endif
{
    gphy(&phyxmi, &phyxma, &phyymi, &phyyma);
    gpixmm(&xpmm, &ypmm);
    return ((PLFLT) (y * ABS(phyyma - phyymi) / ypmm));
}

/* dcsc. define transformations between device coordinates and subpage */
/* coordinates */

#ifdef PLSTDC
PLFLT
dcscx(PLFLT x)
#else
PLFLT 
dcscx(x)
PLFLT x;
#endif
{
    gspd(&spdxmi, &spdxma, &spdymi, &spdyma);
    return ((PLFLT) ((x - spdxmi) / (spdxma - spdxmi)));
}

#ifdef PLSTDC
PLFLT
dcscy(PLFLT y)
#else
PLFLT 
dcscy(y)
PLFLT y;
#endif
{
    gspd(&spdxmi, &spdxma, &spdymi, &spdyma);
    return ((PLFLT) ((y - spdymi) / (spdyma - spdymi)));
}

/* mmdc. converts millimetres from bottom left corner into device */
/* coordinates */

#ifdef PLSTDC
PLFLT
mmdcx(PLFLT x)
#else
PLFLT 
mmdcx(x)
PLFLT x;
#endif
{
    gphy(&phyxmi, &phyxma, &phyymi, &phyyma);
    gpixmm(&xpmm, &ypmm);
    return ((PLFLT) (x * xpmm / ABS(phyxma - phyxmi)));
}

#ifdef PLSTDC
PLFLT
mmdcy(PLFLT y)
#else
PLFLT 
mmdcy(y)
PLFLT y;
#endif
{
    gphy(&phyxmi, &phyxma, &phyymi, &phyyma);
    gpixmm(&xpmm, &ypmm);
    return ((PLFLT) (y * ypmm / ABS(phyyma - phyymi)));
}

/* scdc. converts subpage coordinates to device coordinates */

#ifdef PLSTDC
PLFLT
scdcx(PLFLT x)
#else
PLFLT 
scdcx(x)
PLFLT x;
#endif
{
    gspd(&spdxmi, &spdxma, &spdymi, &spdyma);
    return ((PLFLT) (spdxmi + (spdxma - spdxmi) * x));
}

#ifdef PLSTDC
PLFLT
scdcy(PLFLT y)
#else
PLFLT 
scdcy(y)
PLFLT y;
#endif
{
    gspd(&spdxmi, &spdxma, &spdymi, &spdyma);
    return ((PLFLT) (spdymi + (spdyma - spdymi) * y));
}

/* wcmm. converts world coordinates into millimetres */

#ifdef PLSTDC
PLFLT
wcmmx(PLFLT x)
#else
PLFLT 
wcmmx(x)
PLFLT x;
#endif
{
    gwm(&wmxscl, &wmxoff, &wmyscl, &wmyoff);
    return ((PLFLT) (wmxoff + wmxscl * x));
}

#ifdef PLSTDC
PLFLT
wcmmy(PLFLT y)
#else
PLFLT 
wcmmy(y)
PLFLT y;
#endif
{
    gwm(&wmxscl, &wmxoff, &wmyscl, &wmyoff);
    return ((PLFLT) (wmyoff + wmyscl * y));
}

#ifdef PLSTDC
PLFLT
w3wcx(PLFLT x, PLFLT y, PLFLT z)
#else
PLFLT 
w3wcx(x, y, z)
PLFLT x, y, z;
#endif
{
    gw3wc(&cxx, &cxy, &cyx, &cyy, &cyz);
    gbase(&base3x, &base3y, &basecx, &basecy);
    return ((PLFLT) ((x - basecx) * cxx + (y - basecy) * cxy));
}

#ifdef PLSTDC
PLFLT
w3wcy(PLFLT x, PLFLT y, PLFLT z)
#else
PLFLT 
w3wcy(x, y, z)
PLFLT x, y, z;
#endif
{
    gw3wc(&cxx, &cxy, &cyx, &cyy, &cyz);
    gbase(&base3x, &base3y, &basecx, &basecy);
    grange(&zzscl, &ranmi, &ranma);
    return ((PLFLT) ((x - basecx) * cyx + (y - basecy) * cyy + (z - ranmi) * cyz));
}
