/* $Id$
   $Log$
   Revision 1.8  1993/03/03 19:42:00  mjl
   Changed PLSHORT -> short everywhere; now all device coordinates are expected
   to fit into a 16 bit address space (reasonable, and good for performance).

 * Revision 1.7  1993/02/27  04:46:37  mjl
 * Fixed errors in ordering of header file inclusion.  "plplot.h" should
 * always be included first.
 *
 * Revision 1.6  1993/02/22  23:10:56  mjl
 * Eliminated the gradv() driver calls, as these were made obsolete by
 * recent changes to plmeta and plrender.  Also eliminated page clear commands
 * from grtidy() -- plend now calls grclr() and grtidy() explicitly.
 *
 * Revision 1.5  1993/01/23  05:41:46  mjl
 * Changes to support new color model, polylines, and event handler support
 * (interactive devices only).
 *
 * Revision 1.4  1992/11/07  07:48:44  mjl
 * Fixed orientation operation in several files and standardized certain startup
 * operations. Fixed bugs in various drivers.
 *
 * Revision 1.3  1992/09/30  18:24:55  furnish
 * Massive cleanup to irradicate garbage code.  Almost everything is now
 * prototyped correctly.  Builds on HPUX, SUNOS (gcc), AIX, and UNICOS.
 *
 * Revision 1.2  1992/09/29  04:44:45  furnish
 * Massive clean up effort to remove support for garbage compilers (K&R).
 *
 * Revision 1.1  1992/05/20  21:32:37  furnish
 * Initial checkin of the whole PLPLOT project.
 *
*/

/*	next.c

	PLPLOT NeXT display driver.
*/
#ifdef NEXT

#include "plplot.h"
#include <stdio.h>
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

/* (dev) will get passed in eventually, so this looks weird right now */

static PLDev device;
static PLDev *dev = &device;

/*----------------------------------------------------------------------*\
* nx_init()
*
* Initialize device.
\*----------------------------------------------------------------------*/

void
nx_init(PLStream *pls)
{
    pls->termin = 1;		/* not an interactive terminal */
    pls->icol0 = 1;
    pls->color = 0;
    pls->width = 1;
    pls->bytecnt = 0;
    pls->page = 0;

/* Set up device parameters */

    dev->xold = UNDEFINED;
    dev->yold = UNDEFINED;
    dev->xmin = 0;
    dev->xmax = PSX;
    dev->ymin = 0;
    dev->ymax = PSY;

    setpxl((PLFLT) 11.81, (PLFLT) 11.81);	/* 300 dpi */

    setphy(0, PSX, 0, PSY);
}

/*----------------------------------------------------------------------*\
* nx_line()
*
* Draw a line in the current color from (x1,y1) to (x2,y2).
\*----------------------------------------------------------------------*/

void
nx_line(PLStream *pls, short x1a, short y1a, short x2a, short y2a)
{
    int x1 = x1a, y1 = y1a, x2 = x2a, y2 = y2a;
    int ori;

    if (pls->linepos + 21 > LINELENGTH) {
	putc('\n', pls->OutFile);
	pls->linepos = 0;
    }
    else
	putc(' ', pls->OutFile);

    pls->bytecnt++;

    if (pls->pscale)
	plSclPhy(pls, dev, &x1, &y1, &x2, &y2);

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
* nx_polyline()
*
* Draw a polyline in the current color.
\*----------------------------------------------------------------------*/

void
nx_polyline(PLStream *pls, short *xa, short *ya, PLINT npts)
{
    PLINT i;

    for (i = 0; i < npts - 1; i++)
	nx_line(pls, xa[i], ya[i], xa[i + 1], ya[i + 1]);
}

/*----------------------------------------------------------------------*\
* nx_clear()
*
* Clear page.
\*----------------------------------------------------------------------*/

void
nx_clear(PLStream *pls)
{
    fprintf(pls->OutFile, " S\neop\n");

    pclose(pls->OutFile);
    pls->fileset = 0;
    pls->page = 0;
    pls->linepos = 0;
    pls->OutFile = NULL;
}

/*----------------------------------------------------------------------*\
* nx_page()
*
* Set up for the next page.
* Advance to next family file if necessary (file output).
\*----------------------------------------------------------------------*/

void
nx_page(PLStream *pls)
{
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
* nx_tidy()
*
* Close graphics file or otherwise clean up.
\*----------------------------------------------------------------------*/

void
nx_tidy(PLStream *pls)
{
}

/*----------------------------------------------------------------------*\
* nx_color()
*
* Set pen color.
\*----------------------------------------------------------------------*/

void
nx_color(PLStream *pls)
{
}

/*----------------------------------------------------------------------*\
* nx_text()
*
* Switch to text mode.
\*----------------------------------------------------------------------*/

void
nx_text(PLStream *pls)
{
}

/*----------------------------------------------------------------------*\
* nx_graph()
*
* Switch to graphics mode.
\*----------------------------------------------------------------------*/

void
nx_graph(PLStream *pls)
{
}

/*----------------------------------------------------------------------*\
* nx_width()
*
* Set pen width.
\*----------------------------------------------------------------------*/

void
nx_width(PLStream *pls)
{
    if (pls->width < 1 || pls->width > 10)
	fprintf(stderr, "\nInvalid pen width selection.");
    else {
	fprintf(pls->OutFile, " S\n/lw %d def\n@lwidth\n", pls->width);
    }
}

/*----------------------------------------------------------------------*\
* nx_esc()
*
* Escape function.
\*----------------------------------------------------------------------*/

void
nx_esc(pls, op, ptr)
PLStream *pls;
PLINT op;
char *ptr;
{
}

#else
int 
pldummy_next()
{
    return 0;
}

#endif
