/* $Id$
   $Log$
   Revision 1.11  1993/07/16 22:11:16  mjl
   Eliminated low-level coordinate scaling; now done by driver interface.

 * Revision 1.10  1993/07/01  21:59:34  mjl
 * Changed all plplot source files to include plplotP.h (private) rather than
 * plplot.h.  Rationalized namespace -- all externally-visible plplot functions
 * now start with "pl"; device driver functions start with "plD_".
 *
 * Revision 1.9  1993/03/15  21:39:07  mjl
 * Changed all _clear/_page driver functions to the names _eop/_bop, to be
 * more representative of what's actually going on.
 *
 * Revision 1.8  1993/03/03  19:41:56  mjl
 * Changed PLSHORT -> short everywhere; now all device coordinates are expected
 * to fit into a 16 bit address space (reasonable, and good for performance).
 *
 * Revision 1.7  1993/02/27  04:46:33  mjl
 * Fixed errors in ordering of header file inclusion.  "plplotP.h" should
 * always be included first.
 *
 * Revision 1.6  1993/02/22  23:10:52  mjl
 * Eliminated the plP_adv() driver calls, as these were made obsolete by
 * recent changes to plmeta and plrender.  Also eliminated page clear commands
 * from plP_tidy() -- plend now calls plP_clr() and plP_tidy() explicitly.
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

#include "plplotP.h"
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
* plD_init_hp7470()
*
* Initialize device.
\*----------------------------------------------------------------------*/

void
plD_init_hp7470(PLStream *pls)
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

    plP_setpxl((PLFLT) 40., (PLFLT) 40.);
    plP_setphy(dev->xmin, dev->xmax, dev->ymin, dev->ymax);

    fprintf(pls->OutFile, "%c.I200;;17:%c.N;19:%c.M;;;10:in;\n", ESC, ESC, ESC);
}

/*----------------------------------------------------------------------*\
* plD_line_hp7470()
*
* Draw a line in the current color from (x1,y1) to (x2,y2).
\*----------------------------------------------------------------------*/

void
plD_line_hp7470(PLStream *pls, short x1a, short y1a, short x2a, short y2a)
{
    int x1 = x1a, y1 = y1a, x2 = x2a, y2 = y2a;

    plRotPhy(pls->orient, dev, &x1, &y1, &x2, &y2);

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
* plD_polyline_hp7470()
*
* Draw a polyline in the current color.
\*----------------------------------------------------------------------*/

void
plD_polyline_hp7470(PLStream *pls, short *xa, short *ya, PLINT npts)
{
    PLINT i;

    for (i = 0; i < npts - 1; i++)
	plD_line_hp7470(pls, xa[i], ya[i], xa[i + 1], ya[i + 1]);
}

/*----------------------------------------------------------------------*\
* plD_eop_hp7470()
*
* End of page.
\*----------------------------------------------------------------------*/

void
plD_eop_hp7470(PLStream *pls)
{
}

/*----------------------------------------------------------------------*\
* plD_bop_hp7470()
*
* Set up for the next page.
* Advance to next family file if necessary (file output).
\*----------------------------------------------------------------------*/

void
plD_bop_hp7470(PLStream *pls)
{
    dev->xold = UNDEFINED;
    dev->yold = UNDEFINED;

    fprintf(pls->OutFile, "pg;\n");
    if (!pls->termin)
	plGetFam(pls);

    pls->page++;
}

/*----------------------------------------------------------------------*\
* plD_tidy_hp7470()
*
* Close graphics file or otherwise clean up.
\*----------------------------------------------------------------------*/

void
plD_tidy_hp7470(PLStream *pls)
{
    fprintf(pls->OutFile, "sp0\n");
    fclose(pls->OutFile);
    pls->fileset = 0;
    pls->page = 0;
    pls->OutFile = NULL;
}

/*----------------------------------------------------------------------*\
* plD_color_hp7470()
*
* Set pen color.
\*----------------------------------------------------------------------*/

void
plD_color_hp7470(PLStream *pls)
{
    if (pls->icol0 < 1 || pls->icol0 > 8)
	fprintf(stderr, "\nInvalid pen selection.");
    else {
	fprintf(pls->OutFile, "sp%d %d\n", pls->icol0, pls->width);
    }
}

/*----------------------------------------------------------------------*\
* plD_text_hp7470()
*
* Switch to text mode.
\*----------------------------------------------------------------------*/

void
plD_text_hp7470(PLStream *pls)
{
}

/*----------------------------------------------------------------------*\
* plD_graph_hp7470()
*
* Switch to graphics mode.
\*----------------------------------------------------------------------*/

void
plD_graph_hp7470(PLStream *pls)
{
}

/*----------------------------------------------------------------------*\
* plD_width_hp7470()
*
* Set pen width.
\*----------------------------------------------------------------------*/

void
plD_width_hp7470(PLStream *pls)
{
    if (pls->width < 1 || pls->width > 48)
	fprintf(stderr, "\nInvalid pen width selection.");
    else {
	fprintf(pls->OutFile, "sp%d %d\n", pls->icol0, pls->width);
    }
}

/*----------------------------------------------------------------------*\
* plD_esc_hp7470()
*
* Escape function.
\*----------------------------------------------------------------------*/

void
plD_esc_hp7470(PLStream *pls, PLINT op, void *ptr)
{
}

#else
int 
pldummy_hp7470()
{
    return 0;
}

#endif				/* HP7470 */
