/* $Id$
   $Log$
   Revision 1.9  1993/03/15 21:39:07  mjl
   Changed all _clear/_page driver functions to the names _eop/_bop, to be
   more representative of what's actually going on.

 * Revision 1.8  1993/03/03  19:41:56  mjl
 * Changed PLSHORT -> short everywhere; now all device coordinates are expected
 * to fit into a 16 bit address space (reasonable, and good for performance).
 *
 * Revision 1.7  1993/02/27  04:46:33  mjl
 * Fixed errors in ordering of header file inclusion.  "plplot.h" should
 * always be included first.
 *
 * Revision 1.6  1993/02/22  23:10:52  mjl
 * Eliminated the gradv() driver calls, as these were made obsolete by
 * recent changes to plmeta and plrender.  Also eliminated page clear commands
 * from grtidy() -- plend now calls grclr() and grtidy() explicitly.
 *
 * Revision 1.5  1993/01/23  05:41:41  mjl
 * Changes to support new color model, polylines, and event handler support
 * (interactive devices only).
 *
 * Revision 1.4  1992/11/07  07:48:40  mjl
 * Fixed orientation operation in several files and standardized certain startup
 * operations. Fixed bugs in various drivers.
 *
 * Revision 1.3  1992/09/30  18:24:51  furnish
 * Massive cleanup to irradicate garbage code.  Almost everything is now
 * prototyped correctly.  Builds on HPUX, SUNOS (gcc), AIX, and UNICOS.
 *
 * Revision 1.2  1992/09/29  04:44:41  furnish
 * Massive clean up effort to remove support for garbage compilers (K&R).
 *
 * Revision 1.1  1992/05/20  21:32:34  furnish
 * Initial checkin of the whole PLPLOT project.
 *
*/

/*	hp7470.c

	PLPLOT hp7470 device driver.
*/
#ifdef HP7470

#include "plplot.h"
#include <stdio.h>
#include <string.h>
#include "drivers.h"

/* top level declarations */

/* Graphics control characters. */

#define ESC          27
#define HP7470X   10299
#define HP7470Y    7649

static char outbuf[128];

/* (dev) will get passed in eventually, so this looks weird right now */

static PLDev device;
static PLDev *dev = &device;

/*----------------------------------------------------------------------*\
* hp7470_init()
*
* Initialize device.
\*----------------------------------------------------------------------*/

void
hp7470_init(PLStream *pls)
{
    pls->termin = 0;		/* not an interactive terminal */
    pls->icol0 = 1;
    pls->width = 1;
    pls->bytecnt = 0;
    pls->page = 0;

    if (!pls->colorset)
	pls->color = 1;

/* Initialize family file info */

    plFamInit(pls);

/* Prompt for a file name if not already set */

    plOpenFile(pls);

/* Set up device parameters */

    dev->xold = UNDEFINED;
    dev->yold = UNDEFINED;
    dev->xmin = 0;
    dev->ymin = 0;

    switch (pls->orient) {

      case 1:
      case -1:
	dev->xmax = HP7470Y;
	dev->ymax = HP7470X;
	break;

      default:
	dev->xmax = HP7470X;
	dev->ymax = HP7470Y;
	break;
    }

    dev->xlen = dev->xmax - dev->xmin;
    dev->ylen = dev->ymax - dev->ymin;

    setpxl((PLFLT) 40., (PLFLT) 40.);
    setphy(dev->xmin, dev->xmax, dev->ymin, dev->ymax);

    fprintf(pls->OutFile, "%c.I200;;17:%c.N;19:%c.M;;;10:in;\n", ESC, ESC, ESC);
}

/*----------------------------------------------------------------------*\
* hp7470_line()
*
* Draw a line in the current color from (x1,y1) to (x2,y2).
\*----------------------------------------------------------------------*/

void
hp7470_line(PLStream *pls, short x1a, short y1a, short x2a, short y2a)
{
    int x1 = x1a, y1 = y1a, x2 = x2a, y2 = y2a;

    plRotPhy(pls->orient, dev, &x1, &y1, &x2, &y2);
    if (pls->pscale)
	plSclPhy(pls, dev, &x1, &y1, &x2, &y2);

    if (x1 == dev->xold && y1 == dev->yold)
	/* Add new point to path */
	sprintf(outbuf, "pd%d %d\n", x2, y2);
    else
	/* Write out old path */
	sprintf(outbuf, "pu%d %d pd%d %d\n", x1, y1, x2, y2);

    fprintf(pls->OutFile, "%s", outbuf);
    pls->bytecnt += strlen(outbuf);

    dev->xold = x2;
    dev->yold = y2;
}

/*----------------------------------------------------------------------*\
* hp7470_polyline()
*
* Draw a polyline in the current color.
\*----------------------------------------------------------------------*/

void
hp7470_polyline(PLStream *pls, short *xa, short *ya, PLINT npts)
{
    PLINT i;

    for (i = 0; i < npts - 1; i++)
	hp7470_line(pls, xa[i], ya[i], xa[i + 1], ya[i + 1]);
}

/*----------------------------------------------------------------------*\
* hp7470_eop()
*
* End of page.
\*----------------------------------------------------------------------*/

void
hp7470_eop(PLStream *pls)
{
}

/*----------------------------------------------------------------------*\
* hp7470_bop()
*
* Set up for the next page.
* Advance to next family file if necessary (file output).
\*----------------------------------------------------------------------*/

void
hp7470_bop(PLStream *pls)
{
    dev->xold = UNDEFINED;
    dev->yold = UNDEFINED;

    fprintf(pls->OutFile, "pg;\n");
    if (!pls->termin)
	plGetFam(pls);

    pls->page++;
}

/*----------------------------------------------------------------------*\
* hp7470_tidy()
*
* Close graphics file or otherwise clean up.
\*----------------------------------------------------------------------*/

void
hp7470_tidy(PLStream *pls)
{
    fprintf(pls->OutFile, "sp0\n");
    fclose(pls->OutFile);
    pls->fileset = 0;
    pls->page = 0;
    pls->OutFile = NULL;
}

/*----------------------------------------------------------------------*\
* hp7470_color()
*
* Set pen color.
\*----------------------------------------------------------------------*/

void
hp7470_color(PLStream *pls)
{
    if (pls->icol0 < 1 || pls->icol0 > 8)
	fprintf(stderr, "\nInvalid pen selection.");
    else {
	fprintf(pls->OutFile, "sp%d %d\n", pls->icol0, pls->width);
    }
}

/*----------------------------------------------------------------------*\
* hp7470_text()
*
* Switch to text mode.
\*----------------------------------------------------------------------*/

void
hp7470_text(PLStream *pls)
{
}

/*----------------------------------------------------------------------*\
* hp7470_graph()
*
* Switch to graphics mode.
\*----------------------------------------------------------------------*/

void
hp7470_graph(PLStream *pls)
{
}

/*----------------------------------------------------------------------*\
* hp7470_width()
*
* Set pen width.
\*----------------------------------------------------------------------*/

void
hp7470_width(PLStream *pls)
{
    if (pls->width < 1 || pls->width > 48)
	fprintf(stderr, "\nInvalid pen width selection.");
    else {
	fprintf(pls->OutFile, "sp%d %d\n", pls->icol0, pls->width);
    }
}

/*----------------------------------------------------------------------*\
* hp7470_esc()
*
* Escape function.
\*----------------------------------------------------------------------*/

void
hp7470_esc(PLStream *pls, PLINT op, char *ptr)
{
}

#else
int 
pldummy_hp7470()
{
    return 0;
}

#endif				/* HP7470 */
