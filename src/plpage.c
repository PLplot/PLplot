/* $Id$
   $Log$
   Revision 1.1  1993/01/23 05:57:59  mjl
   Now holds all page-related functions.

 * Revision 1.2  1992/09/29  04:45:41  furnish
 * Massive clean up effort to remove support for garbage compilers (K&R).
 *
 * Revision 1.1  1992/05/20  21:34:10  furnish
 * Initial checkin of the whole PLPLOT project.
 *
*/

/*	pladv.c

	Page/subpage handling routines
*/

#include "plplot.h"

static void setsub(void);

/*----------------------------------------------------------------------*\
* void pladv()
*
* Advance to subpage "page", or to the next one if "page" = 0.
\*----------------------------------------------------------------------*/

void
c_pladv(PLINT page)
{
    PLINT cursub, nsubx, nsuby;
    PLINT device, termin, graphx;
    PLINT level;

    glev(&level);
    if (level < 1)
	plexit("pladv: Please call plstar first.");

    gdev(&device, &termin, &graphx);
    gsub(&nsubx, &nsuby, &cursub);
    if (page > 0 && page <= nsubx * nsuby)
	cursub = page;
    else if (page == 0) {
	if (cursub == nsubx * nsuby) {
	    gradv();
	    cursub = 1;
	}
	else
	    cursub = cursub + 1;
    }
    else
	plexit("pladv: Invalid subpage number.");

    ssub(nsubx, nsuby, cursub);
    setsub();
}

/*----------------------------------------------------------------------*\
* void plclr()
*
* Clear (end) current page.  Should only be used with plpage().
\*----------------------------------------------------------------------*/

void
c_plclr()
{
    PLINT level;
    glev(&level);
    if (level < 1)
	plexit("plclr: Please call plstar first.");

    grclr();
}

/*----------------------------------------------------------------------*\
* void plpage()
*
* Start new page.  Should only be used with plclr().
\*----------------------------------------------------------------------*/

void
c_plpage()
{
    grpage();
}

/*----------------------------------------------------------------------*\
* void setsub()
*
* Set up the subpage boundaries according to the current subpage selected.
\*----------------------------------------------------------------------*/

static void
setsub(void)
{
    PLINT ix, iy;
    PLINT nsubx, nsuby, cursub;
    PLFLT spdxmi, spdxma, spdymi, spdyma;
    PLINT sppxmi, sppxma, sppymi, sppyma;

    gsub(&nsubx, &nsuby, &cursub);
    ix = (cursub - 1) % nsubx;
    iy = nsuby - (cursub - 1) / nsubx;

    spdxmi = (PLFLT) (ix) / (PLFLT) (nsubx);
    spdxma = (PLFLT) (ix + 1) / (PLFLT) (nsubx);
    spdymi = (PLFLT) (iy - 1) / (PLFLT) (nsuby);
    spdyma = (PLFLT) (iy) / (PLFLT) (nsuby);
    sspd(spdxmi, spdxma, spdymi, spdyma);

    sppxmi = dcpcx(spdxmi);
    sppxma = dcpcx(spdxma);
    sppymi = dcpcy(spdymi);
    sppyma = dcpcy(spdyma);
    sspp(sppxmi, sppxma, sppymi, sppyma);

    sclp(sppxmi, sppxma, sppymi, sppyma);
}

/*----------------------------------------------------------------------*\
* void plgspa()
*
* Get subpage boundaries in absolute coordinates (mm from bottom
* left-hand corner of page.
\*----------------------------------------------------------------------*/

void
c_plgspa(PLFLT *xmin, PLFLT *xmax, PLFLT *ymin, PLFLT *ymax)
{
    PLFLT spdxmi, spdxma, spdymi, spdyma;
    PLINT level;

    glev(&level);
    if (level < 1)
	plexit("plgspa: Please call plstar first.");

    gspd(&spdxmi, &spdxma, &spdymi, &spdyma);
    *xmin = dcmmx(spdxmi);
    *xmax = dcmmx(spdxma);
    *ymin = dcmmy(spdymi);
    *ymax = dcmmy(spdyma);
}
