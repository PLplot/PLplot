/* $Id$
   $Log$
   Revision 1.11  1993/07/01 21:59:44  mjl
   Changed all plplot source files to include plplotP.h (private) rather than
   plplot.h.  Rationalized namespace -- all externally-visible plplot functions
   now start with "pl"; device driver functions start with "plD_".

 * Revision 1.10  1993/03/15  21:39:21  mjl
 * Changed all _clear/_page driver functions to the names _eop/_bop, to be
 * more representative of what's actually going on.
 *
 * Revision 1.9  1993/03/03  19:42:08  mjl
 * Changed PLSHORT -> short everywhere; now all device coordinates are expected
 * to fit into a 16 bit address space (reasonable, and good for performance).
 *
 * Revision 1.8  1993/03/03  16:17:09  mjl
 * Fixed orientation-swapping code.
 *
 * Revision 1.7  1993/02/27  04:46:41  mjl
 * Fixed errors in ordering of header file inclusion.  "plplotP.h" should
 * always be included first.
 *
 * Revision 1.6  1993/02/22  23:11:02  mjl
 * Eliminated the plP_adv() driver calls, as these were made obsolete by
 * recent changes to plmeta and plrender.  Also eliminated page clear commands
 * from plP_tidy() -- plend now calls plP_clr() and plP_tidy() explicitly.
 *
 * Revision 1.5  1993/01/23  05:41:53  mjl
 * Changes to support new color model, polylines, and event handler support
 * (interactive devices only).
 *
 * Revision 1.4  1992/11/07  07:48:48  mjl
 * Fixed orientation operation in several files and standardized certain startup
 * operations. Fixed bugs in various drivers.
 *
 * Revision 1.3  1992/10/22  17:04:58  mjl
 * Fixed warnings, errors generated when compling with HP C++.
 *
 * Revision 1.2  1992/09/29  04:44:50  furnish
 * Massive clean up effort to remove support for garbage compilers (K&R).
 *
 * Revision 1.1  1992/05/20  21:32:43  furnish
 * Initial checkin of the whole PLPLOT project.
 *
*/

/*	tek.c

	PLPLOT tektronix device driver.
*/
#ifdef TEK

#include "plplotP.h"
#include <stdio.h>
#include "drivers.h"

/* Function prototypes */
/* INDENT OFF */

void tek_init(PLStream *);

/* top level declarations */

#define TEKX   1023
#define TEKY    779

/* Graphics control characters. */

#define FF   12
#define CAN  24
#define ESC  27
#define GS   29
#define US   31

/* (dev) will get passed in eventually, so this looks weird right now */

static PLDev device;
static PLDev *dev = &device;

/* INDENT ON */
/*----------------------------------------------------------------------*\
* plD_init_tekt()
*
* Initialize device (terminal).
\*----------------------------------------------------------------------*/

void
plD_init_tekt(PLStream *pls)
{
    pls->termin = 1;		/* an interactive device */

    pls->OutFile = stdout;
    pls->orient = 0;
    tek_init(pls);
    fprintf(pls->OutFile, "%c%c", ESC, FF);	/* mgg 07/23/91 via rmr */
}

/*----------------------------------------------------------------------*\
* plD_init_tekf()
*
* Initialize device (file).
\*----------------------------------------------------------------------*/

void
plD_init_tekf(PLStream *pls)
{
    pls->termin = 0;		/* not an interactive terminal */

/* Initialize family file info */

    plFamInit(pls);

/* Prompt for a file name if not already set */

    plOpenFile(pls);

/* Set up device parameters */

    tek_init(pls);
}

/*----------------------------------------------------------------------*\
* tek_init()
*
* Generic device initialization.
\*----------------------------------------------------------------------*/

void
tek_init(PLStream *pls)
{
    pls->icol0 = 1;
    pls->color = 0;
    pls->width = 1;
    pls->bytecnt = 0;
    pls->page = 0;

    dev->xold = UNDEFINED;
    dev->yold = UNDEFINED;
    dev->xmin = 0;
    dev->ymin = 0;

    if (pls->orient%2 == 1) {
	dev->xmax = TEKY * 16;
	dev->ymax = TEKX * 16;
	plP_setpxl((PLFLT) (4.653 * 16), (PLFLT) (4.771 * 16));
    }
    else {
	dev->xmax = TEKX * 16;
	dev->ymax = TEKY * 16;
	plP_setpxl((PLFLT) (4.771 * 16), (PLFLT) (4.653 * 16));
    }

    dev->xlen = dev->xmax - dev->xmin;
    dev->ylen = dev->ymax - dev->ymin;

    plP_setphy(dev->xmin, dev->xmax, dev->ymin, dev->ymax);

    fprintf(pls->OutFile, "%c", GS);
}

/*----------------------------------------------------------------------*\
* plD_line_tek()
*
* Draw a line in the current color from (x1,y1) to (x2,y2).
\*----------------------------------------------------------------------*/

void
plD_line_tek(PLStream *pls, short x1a, short y1a, short x2a, short y2a)
{
    int x1 = x1a, y1 = y1a, x2 = x2a, y2 = y2a;
    int hy, ly, hx, lx;

    plRotPhy(pls->orient, dev, &x1, &y1, &x2, &y2);
    if (pls->pscale)
	plSclPhy(pls, dev, &x1, &y1, &x2, &y2);

    x1 >>= 4;
    y1 >>= 4;
    x2 >>= 4;
    y2 >>= 4;

/* If continuation of previous line just send new point */

    if (x1 == dev->xold && y1 == dev->yold) {
	hy = y2 / 32 + 32;
	ly = y2 - (y2 / 32) * 32 + 96;
	hx = x2 / 32 + 32;
	lx = x2 - (x2 / 32) * 32 + 64;
	fprintf(pls->OutFile, "%c%c%c%c", hy, ly, hx, lx);
	pls->bytecnt += 4;
    }
    else {
	fprintf(pls->OutFile, "%c", GS);
	hy = y1 / 32 + 32;
	ly = y1 - (y1 / 32) * 32 + 96;
	hx = x1 / 32 + 32;
	lx = x1 - (x1 / 32) * 32 + 64;
	fprintf(pls->OutFile, "%c%c%c%c", hy, ly, hx, lx);
	hy = y2 / 32 + 32;
	ly = y2 - (y2 / 32) * 32 + 96;
	hx = x2 / 32 + 32;
	lx = x2 - (x2 / 32) * 32 + 64;
	fprintf(pls->OutFile, "%c%c%c%c", hy, ly, hx, lx);
	pls->bytecnt += 9;
    }
    dev->xold = x2;
    dev->yold = y2;
}

/*----------------------------------------------------------------------*\
* plD_polyline_tek()
*
* Draw a polyline in the current color.
\*----------------------------------------------------------------------*/

void
plD_polyline_tek(PLStream *pls, short *xa, short *ya, PLINT npts)
{
    PLINT i;

    for (i = 0; i < npts - 1; i++)
	plD_line_tek(pls, xa[i], ya[i], xa[i + 1], ya[i + 1]);
}

/*----------------------------------------------------------------------*\
* plD_eop_tek()
*
* End of page.  User must hit a <CR> to continue (terminal output).
\*----------------------------------------------------------------------*/

void
plD_eop_tek(PLStream *pls)
{
    if (pls->termin) {
	putchar('\007');
	fflush(stdout);
	while (getchar() != '\n');
    }
    fprintf(pls->OutFile, "%c%c", ESC, FF);
}

/*----------------------------------------------------------------------*\
* plD_bop_tek()
*
* Set up for the next page.
* Advance to next family file if necessary (file output).
\*----------------------------------------------------------------------*/

void
plD_bop_tek(PLStream *pls)
{
    dev->xold = UNDEFINED;
    dev->yold = UNDEFINED;

    if (!pls->termin)
	plGetFam(pls);

    pls->page++;
}

/*----------------------------------------------------------------------*\
* plD_tidy_tek()
*
* Close graphics file or otherwise clean up.
\*----------------------------------------------------------------------*/

void
plD_tidy_tek(PLStream *pls)
{
    if (!pls->termin) {
	fclose(pls->OutFile);
    }
    else {
	fprintf(pls->OutFile, "%c%c", US, CAN);
	fflush(pls->OutFile);
    }
    pls->fileset = 0;
    pls->page = 0;
    pls->OutFile = NULL;
}

/*----------------------------------------------------------------------*\
* plD_color_tek()
*
* Set pen color.
\*----------------------------------------------------------------------*/

void
plD_color_tek(PLStream *pls)
{
}

/*----------------------------------------------------------------------*\
* plD_text_tek()
*
* Switch to text mode.
\*----------------------------------------------------------------------*/

void
plD_text_tek(PLStream *pls)
{
    fprintf(pls->OutFile, "%c", US);
}

/*----------------------------------------------------------------------*\
* plD_graph_tek()
*
* Switch to graphics mode.
\*----------------------------------------------------------------------*/

void
plD_graph_tek(PLStream *pls)
{
}

/*----------------------------------------------------------------------*\
* plD_width_tek()
*
* Set pen width.
\*----------------------------------------------------------------------*/

void
plD_width_tek(PLStream *pls)
{
}

/*----------------------------------------------------------------------*\
* plD_esc_tek()
*
* Escape function.
\*----------------------------------------------------------------------*/

void
plD_esc_tek(PLStream *pls, PLINT op, void *ptr)
{
}

#else
int 
pldummy_tek()
{
    return 0;
}

#endif				/* TEK */
