/* $Id$
 * $Log$
 * Revision 1.15  1994/07/19 22:30:24  mjl
 * All device drivers: enabling macro renamed to PLD_<driver>, where <driver>
 * is xwin, ps, etc.  See plDevs.h for more detail.
 *
 * Revision 1.14  1994/03/23  06:34:31  mjl
 * All drivers: cleaned up by eliminating extraneous includes (stdio.h and
 * stdlib.h now included automatically by plplotP.h), extraneous clears
 * of pls->fileset, pls->page, and pls->OutFile = NULL (now handled in
 * driver interface or driver initialization as appropriate).  Special
 * handling for malloc includes eliminated (no longer needed) and malloc
 * prototypes fixed as necessary.
*/

/*	next.c

	PLPLOT NeXT display driver.
*/
#include "plDevs.h"

#ifdef PLD_next

#include "plplotP.h"
#include "drivers.h"

/* top level declarations */

#define LINELENGTH      70
#define COPIES          1
#define XSIZE           540	/* 7.5" x 7.5"  (72 points equal 1 inch) */
#define YSIZE           540
#define ENLARGE         5
#define XPSSIZE         ENLARGE*XSIZE
#define YPSSIZE         ENLARGE*YSIZE
#define XOFFSET         18
#define YOFFSET         18
#define XSCALE          100
#define YSCALE          100
#define LINESCALE       100
#define ANGLE           90
#define PSX             XPSSIZE-1
#define PSY             YPSSIZE-1

static char outbuf[128];
static int llx = XPSSIZE, lly = YPSSIZE, urx = 0, ury = 0, ptcnt;

/*----------------------------------------------------------------------*\
* plD_init_nx()
*
* Initialize device.
\*----------------------------------------------------------------------*/

void
plD_init_nx(PLStream *pls)
{
    PLDev *dev;

    pls->termin = 1;		/* not an interactive terminal */
    pls->icol0 = 1;
    pls->color = 0;
    pls->width = 1;
    pls->bytecnt = 0;
    pls->page = 0;

/* Allocate and initialize device-specific data */

    dev = plAllocDev(pls);

    dev->xold = UNDEFINED;
    dev->yold = UNDEFINED;
    dev->xmin = 0;
    dev->xmax = PSX;
    dev->ymin = 0;
    dev->ymax = PSY;

    plP_setpxl((PLFLT) 11.81, (PLFLT) 11.81);	/* 300 dpi */

    plP_setphy(0, PSX, 0, PSY);
}

/*----------------------------------------------------------------------*\
* plD_line_nx()
*
* Draw a line in the current color from (x1,y1) to (x2,y2).
\*----------------------------------------------------------------------*/

void
plD_line_nx(PLStream *pls, short x1a, short y1a, short x2a, short y2a)
{
    PLDev *dev = (PLDev *) pls->dev;
    int x1 = x1a, y1 = y1a, x2 = x2a, y2 = y2a;
    int ori;

    if (pls->linepos + 21 > LINELENGTH) {
	putc('\n', pls->OutFile);
	pls->linepos = 0;
    }
    else
	putc(' ', pls->OutFile);

    pls->bytecnt++;

    if (x1 == dev->xold && y1 == dev->yold && ptcnt < 40) {
	sprintf(outbuf, "%d %d D", x2, y2);
	ptcnt++;
    }
    else {
	sprintf(outbuf, "Z %d %d M %d %d D", x1, y1, x2, y2);
	llx = MIN(llx, x1);
	lly = MIN(lly, y1);
	urx = MAX(urx, x1);
	ury = MAX(ury, y1);
	ptcnt = 1;
    }
    llx = MIN(llx, x2);
    lly = MIN(lly, y2);
    urx = MAX(urx, x2);
    ury = MAX(ury, y2);

    fprintf(pls->OutFile, "%s", outbuf);
    pls->bytecnt += strlen(outbuf);
    dev->xold = x2;
    dev->yold = y2;
    pls->linepos += 21;
}

/*----------------------------------------------------------------------*\
* plD_polyline_nx()
*
* Draw a polyline in the current color.
\*----------------------------------------------------------------------*/

void
plD_polyline_nx(PLStream *pls, short *xa, short *ya, PLINT npts)
{
    PLINT i;

    for (i = 0; i < npts - 1; i++)
	plD_line_nx(pls, xa[i], ya[i], xa[i + 1], ya[i + 1]);
}

/*----------------------------------------------------------------------*\
* plD_eop_nx()
*
* End of page.
\*----------------------------------------------------------------------*/

void
plD_eop_nx(PLStream *pls)
{
    fprintf(pls->OutFile, " S\neop\n");

    pclose(pls->OutFile);
}

/*----------------------------------------------------------------------*\
* plD_bop_nx()
*
* Set up for the next page.
* Advance to next family file if necessary (file output).
\*----------------------------------------------------------------------*/

void
plD_bop_nx(PLStream *pls)
{
    PLDev *dev = (PLDev *) pls->dev;

    dev->xold = UNDEFINED;
    dev->yold = UNDEFINED;

/* Pipe output to Preview */

    pls->OutFile = popen("open", "w");

    /* Header comments into PostScript file */

    fprintf(pls->OutFile, "%%!PS-Adobe-2.0 EPSF-2.0\n");
    fprintf(pls->OutFile, "%%%%Title: PLPLOT Graph\n");
    fprintf(pls->OutFile, "%%%%BoundingBox: 0 0 576 576\n");
    fprintf(pls->OutFile, "%%%%Creator: PLPLOT Version 4.0\n");
    fprintf(pls->OutFile, "%%%%EndComments\n\n");

    /* Definitions */

    fprintf(pls->OutFile, "/eop\n");	/* - eop -  -- end a page */
    fprintf(pls->OutFile, "   {\n");
    fprintf(pls->OutFile, "    showpage\n");
    fprintf(pls->OutFile, "   } def\n");
    fprintf(pls->OutFile, "/@line\n");	/* set line parameters */
    fprintf(pls->OutFile, "   {0 setlinecap\n");
    fprintf(pls->OutFile, "    0 setlinejoin\n");
    fprintf(pls->OutFile, "    1 setmiterlimit\n");
    fprintf(pls->OutFile, "   } def\n");
    /* d @hsize -  horizontal clipping dimension */
    fprintf(pls->OutFile, "/@hsize   {/hs exch def} def\n");
    fprintf(pls->OutFile, "/@vsize   {/vs exch def} def\n");
    /* d @hoffset - shift for the plots */
    fprintf(pls->OutFile, "/@hoffset {/ho exch def} def\n");
    fprintf(pls->OutFile, "/@voffset {/vo exch def} def\n");
    /* s @hscale - scale factors */
    fprintf(pls->OutFile, "/@hscale  {100 div /hsc exch def} def\n");
    fprintf(pls->OutFile, "/@vscale  {100 div /vsc exch def} def\n");
    /* s @lscale - linewidth scale factor */
    fprintf(pls->OutFile, "/@lscale  {100 div /lin exch def} def\n");
    fprintf(pls->OutFile, "/@lwidth  {lin lw mul setlinewidth} def\n");
    fprintf(pls->OutFile, "/@SetPlot\n");	/* setup user specified
						   offsets, */
    fprintf(pls->OutFile, "   {\n");	/* scales, sizes for clipping    */
    fprintf(pls->OutFile, "    ho vo translate\n");
    fprintf(pls->OutFile, "    XScale YScale scale\n");
    fprintf(pls->OutFile, "    lin lw mul setlinewidth\n");
    fprintf(pls->OutFile, "   } def\n");
    fprintf(pls->OutFile, "/XScale\n");	/* setup x scale */
    fprintf(pls->OutFile, "   {hsc hs mul %d div} def\n", YPSSIZE);
    fprintf(pls->OutFile, "/YScale\n");	/* setup y scale */
    fprintf(pls->OutFile, "   {vsc vs mul %d div} def\n", XPSSIZE);
    fprintf(pls->OutFile, "/lw 1 def\n");	/* default line width */
    fprintf(pls->OutFile, "/M {moveto} def\n");
    fprintf(pls->OutFile, "/D {lineto} def\n");
    fprintf(pls->OutFile, "/S {stroke} def\n");
    fprintf(pls->OutFile, "/Z {stroke newpath} def\n");

    /* Set up the plots */

    fprintf(pls->OutFile, "@line\n");
    fprintf(pls->OutFile, "%d @hsize\n", YSIZE);
    fprintf(pls->OutFile, "%d @vsize\n", XSIZE);
    fprintf(pls->OutFile, "%d @hoffset\n", YOFFSET);
    fprintf(pls->OutFile, "%d @voffset\n", XOFFSET);
    fprintf(pls->OutFile, "%d @hscale\n", YSCALE);
    fprintf(pls->OutFile, "%d @vscale\n", XSCALE);
    fprintf(pls->OutFile, "%d @lscale\n", LINESCALE);
    fprintf(pls->OutFile, "@SetPlot\n\n");
    pls->page++;
    pls->linepos = 0;
}

/*----------------------------------------------------------------------*\
* plD_tidy_nx()
*
* Close graphics file or otherwise clean up.
\*----------------------------------------------------------------------*/

void
plD_tidy_nx(PLStream *pls)
{
}

/*----------------------------------------------------------------------*\
* plD_state_nx()
*
* Handle change in PLStream state (color, pen width, fill attribute, etc).
\*----------------------------------------------------------------------*/

void 
plD_state_nx(PLStream *pls, PLINT op)
{
    switch (op) {

    case PLSTATE_WIDTH:
	if (pls->width < 1 || pls->width > 10)
	    fprintf(stderr, "\nInvalid pen width selection.");
	else {
	    fprintf(pls->OutFile, " S\n/lw %d def\n@lwidth\n", pls->width);
	}
	dev->xold = UNDEFINED;
	dev->yold = UNDEFINED;
	break;

    case PLSTATE_COLOR0:
	break;

    case PLSTATE_COLOR1:
	break;
    }
}

/*----------------------------------------------------------------------*\
* plD_esc_nx()
*
* Escape function.
\*----------------------------------------------------------------------*/

void
plD_esc_nx(PLStream *pls, PLINT op, void *ptr)
{
}

#else
int 
pldummy_next()
{
    return 0;
}

#endif			/* PLD_next */
