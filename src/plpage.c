/* $Id$
 * $Log$
 * Revision 1.9  1994/06/30 18:27:17  mjl
 * All core source files: made another pass to eliminate warnings when using
 * gcc -Wall.  Lots of cleaning up: got rid of includes of math.h or string.h
 * (now included by plplot.h), and other minor changes.  Now each file has
 * global access to the plstream pointer via extern; many accessor functions
 * eliminated as a result.  Subpage initialization code moved to this file --
 * subpage settings can now be changed any time (previously, it had to be
 * done before calling plinit).
 *
 * Revision 1.8  1994/03/23  08:23:56  mjl
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
    if (plsc->level < 1) {
	plabort("pladv: Please call plinit first");
	return;
    }

    if (page > 0 && page <= plsc->nsubx * plsc->nsuby)
	plsc->cursub = page;

    else if (page == 0) {
	if (plsc->cursub >= plsc->nsubx * plsc->nsuby) {
	    plP_eop();
	    plP_bop();
	    plsc->cursub = 1;
	}
	else
	    plsc->cursub++;
    }
    else {
	plabort("pladv: Invalid subpage number");
	return;
    }

    plP_setsub();
}

/*----------------------------------------------------------------------*\
 * void pleop()
 *
 * End current page.
\*----------------------------------------------------------------------*/

void
c_pleop(void)
{
    if (plsc->level < 1) {
	plabort("pleop: Please call plinit first");
	return;
    }

    plsc->cursub = plsc->nsubx * plsc->nsuby;
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
    if (plsc->level < 1) {
	plabort("pladv: Please call plinit first");
	return;
    }
    plP_bop();
    plsc->cursub = 1;
    plP_setsub();
}

/*----------------------------------------------------------------------*\
 * void plP_subpInit()
 *
 * Set up plot parameters according to the number of subpages.
\*----------------------------------------------------------------------*/

void
plP_subpInit(void)
{
    PLFLT gscale, hscale;
    PLFLT size_chr, size_sym, size_maj, size_min;

/* Subpage checks */

    if (plsc->nsubx <= 0)
	plsc->nsubx = 1;
    if (plsc->nsuby <= 0)
	plsc->nsuby = 1;

/* Force a page advance */

    plP_eop();
    plP_bop();
    plsc->cursub = 0;

/*
 * Set default sizes
 * Global scaling:
 *	Normalize to the page length for more uniform results.
 * 	A virtual page length of 200 mm is assumed.
 * Subpage scaling:
 *	Reduce sizes with plot area (non-proportional, so that character
 *	size doesn't get too small).
 */
    gscale = 0.5 *
	((plsc->phyxma - plsc->phyxmi) / plsc->xpmm +
	 (plsc->phyyma - plsc->phyymi) / plsc->ypmm) / 200.0;

    hscale = gscale / sqrt((double) plsc->nsuby);

    size_chr = 4.0;
    size_sym = 4.0;		/* All these in virtual plot units */
    size_maj = 3.0;
    size_min = 1.5;

    plsc->chrdef = plsc->chrht = size_chr * hscale;
    plsc->symdef = plsc->symht = size_sym * hscale;
    plsc->majdef = plsc->majht = size_maj * hscale;
    plsc->mindef = plsc->minht = size_min * hscale;
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

    ix = (plsc->cursub - 1) % plsc->nsubx;
    iy = plsc->nsuby - (plsc->cursub - 1) / plsc->nsubx;

    plsc->spdxmi = (PLFLT) (ix)     / (PLFLT) (plsc->nsubx);
    plsc->spdxma = (PLFLT) (ix + 1) / (PLFLT) (plsc->nsubx);
    plsc->spdymi = (PLFLT) (iy - 1) / (PLFLT) (plsc->nsuby);
    plsc->spdyma = (PLFLT) (iy)     / (PLFLT) (plsc->nsuby);

    plsc->sppxmi = plP_dcpcx(plsc->spdxmi);
    plsc->sppxma = plP_dcpcx(plsc->spdxma);
    plsc->sppymi = plP_dcpcy(plsc->spdymi);
    plsc->sppyma = plP_dcpcy(plsc->spdyma);

    plP_sclp(plsc->sppxmi, plsc->sppxma, plsc->sppymi, plsc->sppyma);
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
    if (plsc->level < 1) {
	plabort("plgspa: Please call plinit first");
	return;
    }
    *xmin = plP_dcmmx(plsc->spdxmi);
    *xmax = plP_dcmmx(plsc->spdxma);
    *ymin = plP_dcmmy(plsc->spdymi);
    *ymax = plP_dcmmy(plsc->spdyma);
}
