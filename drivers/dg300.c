/* $Id$
   $Log$
   Revision 1.11  1993/07/16 22:11:13  mjl
   Eliminated low-level coordinate scaling; now done by driver interface.

 * Revision 1.10  1993/07/01  21:59:31  mjl
 * Changed all plplot source files to include plplotP.h (private) rather than
 * plplot.h.  Rationalized namespace -- all externally-visible plplot functions
 * now start with "pl"; device driver functions start with "plD_".
 *
 * Revision 1.9  1993/03/15  21:39:03  mjl
 * Changed all _clear/_page driver functions to the names _eop/_bop, to be
 * more representative of what's actually going on.
 *
 * Revision 1.8  1993/03/03  19:41:53  mjl
 * Changed PLSHORT -> short everywhere; now all device coordinates are expected
 * to fit into a 16 bit address space (reasonable, and good for performance).
 *
 * Revision 1.7  1993/02/27  04:46:30  mjl
 * Fixed errors in ordering of header file inclusion.  "plplotP.h" should
 * always be included first.
 *
 * Revision 1.6  1993/02/22  23:10:49  mjl
 * Eliminated the plP_adv() driver calls, as these were made obsolete by
 * recent changes to plmeta and plrender.  Also eliminated page clear commands
 * from plP_tidy() -- plend now calls plP_clr() and plP_tidy() explicitly.
 *
 * Revision 1.5  1993/01/23  05:41:38  mjl
 * Changes to support new color model, polylines, and event handler support
 * (interactive devices only).
 *
 * Revision 1.4  1992/11/07  07:48:35  mjl
 * Fixed orientation operation in several files and standardized certain startup
 * operations. Fixed bugs in various drivers.
 *
 * Revision 1.3  1992/09/30  18:24:48  furnish
 * Massive cleanup to irradicate garbage code.  Almost everything is now
 * prototyped correctly.  Builds on HPUX, SUNOS (gcc), AIX, and UNICOS.
 *
 * Revision 1.2  1992/09/29  04:44:35  furnish
 * Massive clean up effort to remove support for garbage compilers (K&R).
 *
 * Revision 1.1  1992/05/20  21:32:30  furnish
 * Initial checkin of the whole PLPLOT project.
 *
*/

/*	dg300.c

	PLPLOT dg300 device driver.
*/
#ifdef DG300

#include "plplotP.h"
#include <stdio.h>
#include "drivers.h"

/* top level declarations */

#define  DGX    639
#define  DGY    239

struct termattr {
    unsigned char com[4];
    unsigned char rom[4];
    unsigned char ram[4];
    unsigned char con[5];
    unsigned char eor;
} termattr;

/* (dev) will get passed in eventually, so this looks weird right now */

static PLDev device;
static PLDev *dev = &device;

/*----------------------------------------------------------------------*\
* plD_init_dg()
*
* Initialize device.
\*----------------------------------------------------------------------*/

void
plD_init_dg(PLStream *pls)
{
    /* Request terminal configuration report */
    printf("\n\036\107\051\n");
    scanf("%s", (char *) &termattr);
    while (getchar() != '\n');
    if (!strncmp((char *) &termattr.ram[0], "0000", 4)) {
	printf("Please wait while graphics interpreter is downloaded.\n");
	/* Need to download graphics interpreter. */
	system("cat  /usr/local/src/g300/g300gci110.tx");
    }

    /* Clear screen, Set pen color to green, Absolute positioning */
    printf("\036\107\063\060\n\036\107\155\061\n\036\107\151\060\n");
    printf("\036\107\042\061\n");

    pls->termin = 1;		/* is an interactive device */
    pls->icol0 = 1;
    pls->color = 0;
    pls->width = 1;
    pls->bytecnt = 0;
    pls->page = 0;

    dev->xold = UNDEFINED;
    dev->yold = UNDEFINED;
    dev->xmin = 0;
    dev->xmax = DGX;
    dev->ymin = 0;
    dev->ymax = DGY;

    plP_setpxl((PLFLT) (3.316 * 16), (PLFLT) (1.655 * 16));
    plP_setphy(0, DGX * 16, 0, DGY * 16);
}

/*----------------------------------------------------------------------*\
* plD_line_dg()
*
* Draw a line in the current color from (x1,y1) to (x2,y2).
\*----------------------------------------------------------------------*/

void
plD_line_dg(PLStream *pls, short x1a, short y1a, short x2a, short y2a)
{
    int x1 = x1a, y1 = y1a, x2 = x2a, y2 = y2a;

    printf("LINE %d %d %d %d\n", x1 >> 4, y1 >> 3, x2 >> 4, y2 >> 3);
}

/*----------------------------------------------------------------------*\
* plD_polyline_dg()
*
* Draw a polyline in the current color.
\*----------------------------------------------------------------------*/

void
plD_polyline_dg(PLStream *pls, short *xa, short *ya, PLINT npts)
{
    PLINT i;

    for (i = 0; i < npts - 1; i++)
	plD_line_dg(pls, xa[i], ya[i], xa[i + 1], ya[i + 1]);
}

/*----------------------------------------------------------------------*\
* plD_eop_dg()
*
* End of page.  User must hit a <CR> to continue.
\*----------------------------------------------------------------------*/

void
plD_eop_dg(PLStream *pls)
{
    /* Before clearing wait for CR */
    putchar('\007');
    fflush(stdout);
    while (getchar() != '\n');
    printf("ERASE\n");
}

/*----------------------------------------------------------------------*\
* plD_bop_dg()
*
* Set up for the next page.
\*----------------------------------------------------------------------*/

void
plD_bop_dg(PLStream *pls)
{
    pls->page++;
}

/*----------------------------------------------------------------------*\
* plD_tidy_dg()
*
* Close graphics file
\*----------------------------------------------------------------------*/

void
plD_tidy_dg(PLStream *pls)
{
    printf("\036\107\042\060\n");
    fflush(stdout);
    pls->fileset = 0;
    pls->page = 0;
    pls->OutFile = NULL;
}

/*----------------------------------------------------------------------*\
* plD_color_dg()
*
* Set pen color.
\*----------------------------------------------------------------------*/

void
plD_color_dg(PLStream *pls)
{
}

/*----------------------------------------------------------------------*\
* plD_text_dg()
*
* Switch to text mode.
\*----------------------------------------------------------------------*/

void
plD_text_dg(PLStream *pls)
{
}

/*----------------------------------------------------------------------*\
* plD_graph_dg()
*
* Switch to graphics mode.
\*----------------------------------------------------------------------*/

void
plD_graph_dg(PLStream *pls)
{
}

/*----------------------------------------------------------------------*\
* plD_width_dg()
*
* Set pen width.
\*----------------------------------------------------------------------*/

void
plD_width_dg(PLStream *pls)
{
}

/*----------------------------------------------------------------------*\
* plD_esc_dg()
*
* Escape function.
\*----------------------------------------------------------------------*/

void
plD_esc_dg(PLStream *pls, PLINT op, void *ptr)
{
}

#else
int 
pldummy_dg300()
{
    return 0;
}

#endif				/* DG300 */
