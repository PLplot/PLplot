/* $Id$
 * $Log$
 * Revision 1.8  1994/03/23 08:23:56  mjl
 * All external API source files: replaced call to plexit() on simple
 * (recoverable) errors with simply printing the error message (via
 * plabort()) and returning.  Should help avoid loss of computer time in some
 * critical circumstances (during a long batch run, for example).
 *
 * Revision 1.7  1993/12/08  06:22:58  mjl
 * Fix to plbop() so that the user can just use plbop/pleop if desired and
 * never need to call pladv().
*/

/*	plpage.c

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
    if (level < 1) {
	plabort("pladv: Please call plinit first");
	return;
    }

    plP_gsub(&nsubx, &nsuby, &cursub);
    if (page > 0 && page <= nsubx * nsuby)
	cursub = page;
    else if (page == 0) {
	if (cursub == nsubx * nsuby) {
	    plP_eop();
	    plP_bop();
	    cursub = 1;
	}
	else
	    cursub = cursub + 1;
    }
    else {
	plabort("pladv: Invalid subpage number");
	return;
    }

    plP_ssub(nsubx, nsuby, cursub);
    plP_setsub();
}

/*----------------------------------------------------------------------*\
* void pleop()
*
* End current page.  Should only be used with plbop().
\*----------------------------------------------------------------------*/

void
c_pleop(void)
{
    PLINT level;
    plP_glev(&level);
    if (level < 1) {
	plabort("pleop: Please call plinit first");
	return;
    }

    plP_eop();
}

/*----------------------------------------------------------------------*\
* void plbop()
*
* Start new page.  Should only be used with pleop().
\*----------------------------------------------------------------------*/

void
c_plbop(void)
{
    PLINT cursub, nsubx, nsuby;
    PLINT level;

    plP_glev(&level);
    if (level < 1) {
	plabort("pladv: Please call plinit first");
	return;
    }

    plP_gsub(&nsubx, &nsuby, &cursub);
    plP_bop();
    cursub = 1;

    plP_ssub(nsubx, nsuby, cursub);
    plP_setsub();
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
    if (level < 1) {
	plabort("plgspa: Please call plinit first");
	return;
    }

    plP_gspd(&spdxmi, &spdxma, &spdymi, &spdyma);
    *xmin = plP_dcmmx(spdxmi);
    *xmax = plP_dcmmx(spdxma);
    *ymin = plP_dcmmy(spdymi);
    *ymax = plP_dcmmy(spdyma);
}
