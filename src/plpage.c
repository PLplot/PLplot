/* $Id$
   $Log$
   Revision 1.3  1993/07/01 22:13:41  mjl
   Changed all plplot source files to include plplotP.h (private) rather than
   plplot.h.  Rationalized namespace -- all externally-visible internal
   plplot functions now start with "plP_".

 * Revision 1.2  1993/02/23  05:17:53  mjl
 * Changed references in error messages from plstar to plinit.  Also changed
 * pladv to call grclr and grpage (instead of gradv, now gone).
 *
 * Revision 1.1  1993/01/23  05:57:59  mjl
 * Now holds all page-related functions.
 *
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

#include "plplotP.h"

/*----------------------------------------------------------------------*\
* void pladv()
*
* Advance to subpage "page", or to the next one if "page" = 0.
\*----------------------------------------------------------------------*/

void
c_pladv(PLINT page)
{
    PLINT cursub, nsubx, nsuby;
    PLINT level;

    plP_glev(&level);
    if (level < 1)
	plexit("pladv: Please call plinit first.");

    plP_gsub(&nsubx, &nsuby, &cursub);
    if (page > 0 && page <= nsubx * nsuby)
	cursub = page;
    else if (page == 0) {
	if (cursub == nsubx * nsuby) {
	    plP_clr();
	    plP_page();
	    cursub = 1;
	}
	else
	    cursub = cursub + 1;
    }
    else
	plexit("pladv: Invalid subpage number.");

    plP_ssub(nsubx, nsuby, cursub);
    plP_setsub();
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
    plP_glev(&level);
    if (level < 1)
	plexit("plclr: Please call plinit first.");

    plP_clr();
}

/*----------------------------------------------------------------------*\
* void plpage()
*
* Start new page.  Should only be used with plclr().
\*----------------------------------------------------------------------*/

void
c_plpage()
{
    plP_page();
}

/*----------------------------------------------------------------------*\
* void plP_setsub()
*
* Set up the subpage boundaries according to the current subpage selected.
\*----------------------------------------------------------------------*/

void
plP_setsub(void)
{
    PLINT ix, iy;
    PLINT nsubx, nsuby, cursub;
    PLFLT spdxmi, spdxma, spdymi, spdyma;
    PLINT sppxmi, sppxma, sppymi, sppyma;

    plP_gsub(&nsubx, &nsuby, &cursub);
    ix = (cursub - 1) % nsubx;
    iy = nsuby - (cursub - 1) / nsubx;

    spdxmi = (PLFLT) (ix) / (PLFLT) (nsubx);
    spdxma = (PLFLT) (ix + 1) / (PLFLT) (nsubx);
    spdymi = (PLFLT) (iy - 1) / (PLFLT) (nsuby);
    spdyma = (PLFLT) (iy) / (PLFLT) (nsuby);
    plP_sspd(spdxmi, spdxma, spdymi, spdyma);

    sppxmi = plP_dcpcx(spdxmi);
    sppxma = plP_dcpcx(spdxma);
    sppymi = plP_dcpcy(spdymi);
    sppyma = plP_dcpcy(spdyma);
    plP_sspp(sppxmi, sppxma, sppymi, sppyma);

    plP_sclp(sppxmi, sppxma, sppymi, sppyma);
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

    plP_glev(&level);
    if (level < 1)
	plexit("plgspa: Please call plinit first.");

    plP_gspd(&spdxmi, &spdxma, &spdymi, &spdyma);
    *xmin = plP_dcmmx(spdxmi);
    *xmax = plP_dcmmx(spdxma);
    *ymin = plP_dcmmy(spdymi);
    *ymax = plP_dcmmy(spdyma);
}
