/* $Id$
   $Log$
   Revision 1.2.2.1  2001/04/19 12:31:46  rlaboiss
   First merge against MAIN

   Revision 1.2  1992/09/29 04:45:41  furnish
   Massive clean up effort to remove support for garbage compilers (K&R).

 * Revision 1.1  1992/05/20  21:34:10  furnish
 * Initial checkin of the whole PLPLOT project.
 *
*/

/*	pladv.c

	Page/subpage advance routines
*/

#include "plplot.h"

static void  setsub(void);

/* Advance to subpage "page", or to the next one if "page" = 0 */

void 
c_pladv( PLINT page )
{
    PLINT cursub, nsubx, nsuby;
    PLINT device, termin, graphx;
    PLINT level;

    glev(&level);
    if (level < 1)
	plexit("Please call plstar before calling pladv.");

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
	plexit("Invalid subpage number in pladv.");

    ssub(nsubx, nsuby, cursub);
    setsub();
}

/* Clears (ends) current page.  Should only be used with plpage(). */

void 
c_plclr()
{
    PLINT level;
    glev(&level);
    if (level < 1)
	plexit("Please call plstar before calling plclr.");

    grclr();
}

/* Starts new page.  Should only be used with plclr(). */

void 
c_plpage()
{
    grpage();
}

/* Sets up the subpage boundaries according to the current subpage  */
/* selected */

static void 
setsub (void)
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
