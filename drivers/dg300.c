/* $Id$
   $Log$
   Revision 1.8  1993/03/03 19:41:53  mjl
   Changed PLSHORT -> short everywhere; now all device coordinates are expected
   to fit into a 16 bit address space (reasonable, and good for performance).

 * Revision 1.7  1993/02/27  04:46:30  mjl
 * Fixed errors in ordering of header file inclusion.  "plplot.h" should
 * always be included first.
 *
 * Revision 1.6  1993/02/22  23:10:49  mjl
 * Eliminated the gradv() driver calls, as these were made obsolete by
 * recent changes to plmeta and plrender.  Also eliminated page clear commands
 * from grtidy() -- plend now calls grclr() and grtidy() explicitly.
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

#include "plplot.h"
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
* dg_init()
*
* Initialize device.
\*----------------------------------------------------------------------*/

void
dg_init(PLStream *pls)
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

    setpxl((PLFLT) (3.316 * 16), (PLFLT) (1.655 * 16));
    setphy(0, DGX * 16, 0, DGY * 16);
}

/*----------------------------------------------------------------------*\
* dg_line()
*
* Draw a line in the current color from (x1,y1) to (x2,y2).
\*----------------------------------------------------------------------*/

void
dg_line(PLStream *pls, short x1a, short y1a, short x2a, short y2a)
{
    int x1 = x1a, y1 = y1a, x2 = x2a, y2 = y2a;

    if (pls->pscale)
	plSclPhy(pls, dev, &x1, &y1, &x2, &y2);

    printf("LINE %d %d %d %d\n", x1 >> 4, y1 >> 3, x2 >> 4, y2 >> 3);
}

/*----------------------------------------------------------------------*\
* dg_polyline()
*
* Draw a polyline in the current color.
\*----------------------------------------------------------------------*/

void
dg_polyline(PLStream *pls, short *xa, short *ya, PLINT npts)
{
    PLINT i;

    for (i = 0; i < npts - 1; i++)
	dg_line(pls, xa[i], ya[i], xa[i + 1], ya[i + 1]);
}

/*----------------------------------------------------------------------*\
* dg_clear()
*
* Clear page.  User must hit a <CR> to continue.
\*----------------------------------------------------------------------*/

void
dg_clear(PLStream *pls)
{
    /* Before clearing wait for CR */
    putchar('\007');
    fflush(stdout);
    while (getchar() != '\n');
    printf("ERASE\n");
}

/*----------------------------------------------------------------------*\
* dg_page()
*
* Set up for the next page.
\*----------------------------------------------------------------------*/

void
dg_page(PLStream *pls)
{
    pls->page++;
}

/*----------------------------------------------------------------------*\
* dg_tidy()
*
* Close graphics file
\*----------------------------------------------------------------------*/

void
dg_tidy(PLStream *pls)
{
    printf("\036\107\042\060\n");
    fflush(stdout);
    pls->fileset = 0;
    pls->page = 0;
    pls->OutFile = NULL;
}

/*----------------------------------------------------------------------*\
* dg_color()
*
* Set pen color.
\*----------------------------------------------------------------------*/

void
dg_color(PLStream *pls)
{
}

/*----------------------------------------------------------------------*\
* dg_text()
*
* Switch to text mode.
\*----------------------------------------------------------------------*/

void
dg_text(PLStream *pls)
{
}

/*----------------------------------------------------------------------*\
* dg_graph()
*
* Switch to graphics mode.
\*----------------------------------------------------------------------*/

void
dg_graph(PLStream *pls)
{
}

/*----------------------------------------------------------------------*\
* dg_width()
*
* Set pen width.
\*----------------------------------------------------------------------*/

void
dg_width(PLStream *pls)
{
}

/*----------------------------------------------------------------------*\
* dg_esc()
*
* Escape function.
\*----------------------------------------------------------------------*/

void
dg_esc(PLStream *pls, PLINT op, char *ptr)
{
}

#else
int 
pldummy_dg300()
{
    return 0;
}

#endif				/* DG300 */
