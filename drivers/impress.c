/* $Id$
   $Log$
   Revision 1.10  1993/07/01 21:59:36  mjl
   Changed all plplot source files to include plplotP.h (private) rather than
   plplot.h.  Rationalized namespace -- all externally-visible plplot functions
   now start with "pl"; device driver functions start with "plD_".

 * Revision 1.9  1993/03/15  21:39:09  mjl
 * Changed all _clear/_page driver functions to the names _eop/_bop, to be
 * more representative of what's actually going on.
 *
 * Revision 1.8  1993/03/03  19:41:58  mjl
 * Changed PLSHORT -> short everywhere; now all device coordinates are expected
 * to fit into a 16 bit address space (reasonable, and good for performance).
 *
 * Revision 1.7  1993/02/27  04:46:35  mjl
 * Fixed errors in ordering of header file inclusion.  "plplotP.h" should
 * always be included first.
 *
 * Revision 1.6  1993/02/22  23:10:54  mjl
 * Eliminated the plP_adv() driver calls, as these were made obsolete by
 * recent changes to plmeta and plrender.  Also eliminated page clear commands
 * from plP_tidy() -- plend now calls plP_clr() and plP_tidy() explicitly.
 *
 * Revision 1.5  1993/01/23  05:41:43  mjl
 * Changes to support new color model, polylines, and event handler support
 * (interactive devices only).
 *
 * Revision 1.4  1992/11/07  07:48:42  mjl
 * Fixed orientation operation in several files and standardized certain startup
 * operations. Fixed bugs in various drivers.
 *
 * Revision 1.3  1992/09/30  18:24:53  furnish
 * Massive cleanup to irradicate garbage code.  Almost everything is now
 * prototyped correctly.  Builds on HPUX, SUNOS (gcc), AIX, and UNICOS.
 *
 * Revision 1.2  1992/09/29  04:44:43  furnish
 * Massive clean up effort to remove support for garbage compilers (K&R).
 *
 * Revision 1.1  1992/05/20  21:32:36  furnish
 * Initial checkin of the whole PLPLOT project.
 *
*/

/*	impress.c

	PLPLOT ImPress device driver.
*/
#ifdef IMP

#define PL_NEED_MALLOC
#include "plplotP.h"
#include <stdio.h>
#include <stdlib.h>
#include "drivers.h"

/* Function prototypes */

static void flushline(PLStream *);

/* top level declarations */

#define IMPX		2999
#define IMPY		2249

#define BUFFPTS		50
#define BUFFLENG	2*BUFFPTS

/* Graphics control characters. */

#define SET_HV_SYSTEM	0315
#define OPBYTE1		031
#define OPBYTE2		0140
#define SET_ABS_H	0207
#define SET_ABS_V	0211
#define OPWORDH1	0
#define OPWORDH2	150
#define OPWORDV1	0
#define OPWORDV2	150
#define ENDPAGE		0333

#define SET_PEN		0350
#define CREATE_PATH	0346
#define DRAW_PATH	0352
#define OPTYPE		017

static short *LineBuff;
static short FirstLine;
static int penchange = 0, penwidth;
static short count;

/* (dev) will get passed in eventually, so this looks weird right now */

static PLDev device;
static PLDev *dev = &device;

/*----------------------------------------------------------------------*\
* plD_init_imp()
*
* Initialize device (terminal).
\*----------------------------------------------------------------------*/

void
plD_init_imp(PLStream *pls)
{
    pls->termin = 0;		/* not an interactive terminal */
    pls->icol0 = 1;
    pls->color = 0;
    pls->width = 1;
    pls->bytecnt = 0;
    pls->page = 0;

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
	dev->xmax = IMPY;
	dev->ymax = IMPX;
	break;

      default:
	dev->xmax = IMPX;
	dev->ymax = IMPY;
	break;
    }

    dev->xlen = dev->xmax - dev->xmin;
    dev->ylen = dev->ymax - dev->ymin;

    plP_setpxl((PLFLT) 11.81, (PLFLT) 11.81);
    plP_setphy(dev->xmin, dev->xmax, dev->ymin, dev->ymax);

    LineBuff = (short *) malloc(BUFFLENG * sizeof(short));
    if (LineBuff == NULL) {
	plexit("Error in memory alloc in plD_init_imp().");
    }
    fprintf(pls->OutFile, "@Document(Language ImPress, jobheader off)");
    fprintf(pls->OutFile, "%c%c", SET_HV_SYSTEM, OPBYTE1);
    fprintf(pls->OutFile, "%c%c%c", SET_ABS_H, OPWORDH1, OPWORDH2);
    fprintf(pls->OutFile, "%c%c%c", SET_ABS_V, OPWORDV1, OPWORDV2);
    fprintf(pls->OutFile, "%c%c", SET_HV_SYSTEM, OPBYTE2);
}

/*----------------------------------------------------------------------*\
* plD_line_imp()
*
* Draw a line in the current color from (x1,y1) to (x2,y2).
\*----------------------------------------------------------------------*/

void
plD_line_imp(PLStream *pls, short x1a, short y1a, short x2a, short y2a)
{
    int x1 = x1a, y1 = y1a, x2 = x2a, y2 = y2a;

    plRotPhy(pls->orient, dev, &x1, &y1, &x2, &y2);
    if (pls->pscale)
	plSclPhy(pls, dev, &x1, &y1, &x2, &y2);

    if (FirstLine) {
	if (penchange) {
	    fprintf(pls->OutFile, "%c%c", SET_PEN, (char) penwidth);
	    penchange = 0;
	}
	/* Add both points to path */
	count = 0;
	FirstLine = 0;
	*(LineBuff + count++) = (short) x1;
	*(LineBuff + count++) = (short) y1;
	*(LineBuff + count++) = (short) x2;
	*(LineBuff + count++) = (short) y2;
    }
    else if ((count + 2) < BUFFLENG && x1 == dev->xold && y1 == dev->yold) {
	/* Add new point to path */
	*(LineBuff + count++) = (short) x2;
	*(LineBuff + count++) = (short) y2;
    }
    else {
	/* Write out old path */
	count /= 2;
	fprintf(pls->OutFile, "%c%c%c", CREATE_PATH, (char) count / 256, (char) count % 256);
	fwrite((char *) LineBuff, sizeof(short), 2 * count, pls->OutFile);
	fprintf(pls->OutFile, "%c%c", DRAW_PATH, OPTYPE);

	/* And start a new path */
	if (penchange) {
	    fprintf(pls->OutFile, "%c%c", SET_PEN, (char) penwidth);
	    penchange = 0;
	}
	count = 0;
	*(LineBuff + count++) = (short) x1;
	*(LineBuff + count++) = (short) y1;
	*(LineBuff + count++) = (short) x2;
	*(LineBuff + count++) = (short) y2;
    }
    dev->xold = x2;
    dev->yold = y2;
}

/*----------------------------------------------------------------------*\
* plD_polyline_imp()
*
* Draw a polyline in the current color.
\*----------------------------------------------------------------------*/

void
plD_polyline_imp(PLStream *pls, short *xa, short *ya, PLINT npts)
{
    PLINT i;

    for (i = 0; i < npts - 1; i++)
	plD_line_imp(pls, xa[i], ya[i], xa[i + 1], ya[i + 1]);
}

/*----------------------------------------------------------------------*\
* plD_eop_imp()
*
* End of page.
\*----------------------------------------------------------------------*/

void
plD_eop_imp(PLStream *pls)
{
    flushline(pls);
    fprintf(pls->OutFile, "%c", ENDPAGE);
}

/*----------------------------------------------------------------------*\
* plD_bop_imp()
*
* Set up for the next page.
* Advance to next family file if necessary (file output).
\*----------------------------------------------------------------------*/

void
plD_bop_imp(PLStream *pls)
{
    FirstLine = 1;
    dev->xold = UNDEFINED;
    dev->yold = UNDEFINED;

    if (!pls->termin)
	plGetFam(pls);

    pls->page++;
}

/*----------------------------------------------------------------------*\
* plD_tidy_imp()
*
* Close graphics file or otherwise clean up.
\*----------------------------------------------------------------------*/

void
plD_tidy_imp(PLStream *pls)
{
    free((char *) LineBuff);
    fclose(pls->OutFile);
    pls->fileset = 0;
    pls->page = 0;
    pls->OutFile = NULL;
}

/*----------------------------------------------------------------------*\
* plD_color_imp()
*
* Set pen color.
\*----------------------------------------------------------------------*/

void
plD_color_imp(PLStream *pls)
{
}

/*----------------------------------------------------------------------*\
* plD_text_imp()
*
* Switch to text mode.
\*----------------------------------------------------------------------*/

void
plD_text_imp(PLStream *pls)
{
}

/*----------------------------------------------------------------------*\
* plD_graph_imp()
*
* Switch to graphics mode.
\*----------------------------------------------------------------------*/

void
plD_graph_imp(PLStream *pls)
{
}

/*----------------------------------------------------------------------*\
* plD_width_imp()
*
* Set pen width.
\*----------------------------------------------------------------------*/

void
plD_width_imp(PLStream *pls)
{
    if (pls->width > 0 && pls->width <= 20) {
	penwidth = pls->width;
	penchange = 1;
    }
}

/*----------------------------------------------------------------------*\
* plD_esc_imp()
*
* Escape function.
\*----------------------------------------------------------------------*/

void
plD_esc_imp(PLStream *pls, PLINT op, void *ptr)
{
}

/*----------------------------------------------------------------------*\
* flushline()
*
* Spits out the line buffer.
\*----------------------------------------------------------------------*/

static void
flushline(PLStream *pls)
{
    count /= 2;
    fprintf(pls->OutFile, "%c%c%c", CREATE_PATH, (char) count / 256, (char) count % 256);
    fwrite((char *) LineBuff, sizeof(short), 2 * count, pls->OutFile);
    fprintf(pls->OutFile, "%c%c", DRAW_PATH, OPTYPE);
    FirstLine = 1;
}

#else
int 
pldummy_impress()
{
    return 0;
}

#endif				/* IMP */
