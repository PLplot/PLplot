/* $Id$
   $Log$
   Revision 1.12  1993/07/31 07:56:32  mjl
   Several driver functions consolidated, for all drivers.  The width and color
   commands are now part of a more general "state" command.  The text and
   graph commands used for switching between modes is now handled by the
   escape function (very few drivers require it).  The device-specific PLDev
   structure is now malloc'ed for each driver that requires it, and freed when
   the stream is terminated.

 * Revision 1.11  1993/07/16  22:11:17  mjl
 * Eliminated low-level coordinate scaling; now done by driver interface.
 *
 * Revision 1.10  1993/07/01  21:59:35  mjl
 * Changed all plplot source files to include plplotP.h (private) rather than
 * plplot.h.  Rationalized namespace -- all externally-visible plplot functions
 * now start with "pl"; device driver functions start with "plD_".
*/

/*	hp7580.c

	PLPLOT hp7580 device driver.
*/
#ifdef HP7580

#include "plplotP.h"
#include <stdio.h>
#include <string.h>
#include "drivers.h"

/* top level declarations */

/* Graphics control characters. */

#define ESC    27
#define HPXMIN  -4500
#define HPXMAX   4500
#define HPYMIN  -2790
#define HPYMAX   2790

static char outbuf[128];

/*----------------------------------------------------------------------*\
* plD_init_hp7580()
*
* Initialize device.
\*----------------------------------------------------------------------*/

void
plD_init_hp7580(PLStream *pls)
{
    PLDev *dev;

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

/* Allocate and initialize device-specific data */

    dev = plAllocDev(pls);

    dev->xold = UNDEFINED;
    dev->yold = UNDEFINED;
    dev->xmin = HPXMIN;
    dev->xmax = HPXMAX;
    dev->ymin = HPYMIN;
    dev->ymax = HPYMAX;
    dev->xlen = dev->xmax - dev->xmin;
    dev->ylen = dev->ymax - dev->ymin;

    plP_setpxl((PLFLT) 40., (PLFLT) 40.);
    plP_setphy(dev->xmin, dev->xmax, dev->ymin, dev->ymax);

    fprintf(pls->OutFile, "%c.I200;;17:%c.N;19:%c.M;;;10:in;\n",
	    ESC, ESC, ESC);

    fprintf(pls->OutFile, "ro 90;ip;sp 4;pa;");
}

/*----------------------------------------------------------------------*\
* plD_line_hp7580()
*
* Draw a line in the current color from (x1,y1) to (x2,y2).
\*----------------------------------------------------------------------*/

void
plD_line_hp7580(PLStream *pls, short x1a, short y1a, short x2a, short y2a)
{
    PLDev *dev = (PLDev *) pls->dev;
    int x1 = x1a, y1 = y1a, x2 = x2a, y2 = y2a;

    if (x1 == dev->xold && y1 == dev->yold)
	/* Add new point to path */
	sprintf(outbuf, " %d %d", x2, y2);
    else
	/* Write out old path */
	sprintf(outbuf, "\npu%d %d pd%d %d", x1, y1, x2, y2);

    fprintf(pls->OutFile, "%s", outbuf);
    pls->bytecnt += strlen(outbuf);

    dev->xold = x2;
    dev->yold = y2;
}

/*----------------------------------------------------------------------*\
* plD_polyline_hp7580()
*
* Draw a polyline in the current color.
\*----------------------------------------------------------------------*/

void
plD_polyline_hp7580(PLStream *pls, short *xa, short *ya, PLINT npts)
{
    PLINT i;

    for (i = 0; i < npts - 1; i++)
	plD_line_hp7580(pls, xa[i], ya[i], xa[i + 1], ya[i + 1]);
}

/*----------------------------------------------------------------------*\
* plD_eop_hp7580()
*
* End of page.
\*----------------------------------------------------------------------*/

void
plD_eop_hp7580(PLStream *pls)
{
}

/*----------------------------------------------------------------------*\
* plD_bop_hp7580()
*
* Set up for the next page.
* Advance to next family file if necessary (file output).
\*----------------------------------------------------------------------*/

void
plD_bop_hp7580(PLStream *pls)
{
    PLDev *dev = (PLDev *) pls->dev;

    dev->xold = UNDEFINED;
    dev->yold = UNDEFINED;

    fprintf(pls->OutFile, "pg;\n");
    if (!pls->termin)
	plGetFam(pls);

    pls->page++;
}

/*----------------------------------------------------------------------*\
* plD_tidy_hp7580()
*
* Close graphics file or otherwise clean up.
\*----------------------------------------------------------------------*/

void
plD_tidy_hp7580(PLStream *pls)
{
    fprintf(pls->OutFile, "\nsp0");
    fclose(pls->OutFile);
    pls->fileset = 0;
    pls->page = 0;
    pls->OutFile = NULL;
}

/*----------------------------------------------------------------------*\
* plD_state_hp7580()
*
* Handle change in PLStream state (color, pen width, fill attribute, etc).
\*----------------------------------------------------------------------*/

void 
plD_state_hp7580(PLStream *pls, PLINT op)
{
    switch (op) {

    case PLSTATE_WIDTH:
	break;

    case PLSTATE_COLOR0:
	if (pls->icol0 < 1 || pls->icol0 > 8)
	    fprintf(stderr, "\nInvalid pen selection.");
	else 
	    fprintf(pls->OutFile, "sp%d\n", pls->icol0);

	break;

    case PLSTATE_COLOR1:
	break;
    }
}

/*----------------------------------------------------------------------*\
* plD_esc_hp7580()
*
* Escape function.
\*----------------------------------------------------------------------*/

void
plD_esc_hp7580(PLStream *pls, PLINT op, void *ptr)
{
}

#else
int 
pldummy_hp7580()
{
    return 0;
}

#endif
