/* $Id$
   $Log$
   Revision 1.6  1993/01/23 05:41:51  mjl
   Changes to support new color model, polylines, and event handler support
   (interactive devices only).

 * Revision 1.5  1992/11/07  07:48:47  mjl
 * Fixed orientation operation in several files and standardized certain startup
 * operations. Fixed bugs in various drivers.
 *
 * Revision 1.4  1992/10/22  17:04:56  mjl
 * Fixed warnings, errors generated when compling with HP C++.
 *
 * Revision 1.3  1992/09/30  18:24:58  furnish
 * Massive cleanup to irradicate garbage code.  Almost everything is now
 * prototyped correctly.  Builds on HPUX, SUNOS (gcc), AIX, and UNICOS.
 *
 * Revision 1.2  1992/09/29  04:44:48  furnish
 * Massive clean up effort to remove support for garbage compilers (K&R).
 *
 * Revision 1.1  1992/05/20  21:32:42  furnish
 * Initial checkin of the whole PLPLOT project.
 *
*/

/*	ps.c

	PLPLOT PostScript device driver.
*/
#ifdef PS

#include <stdio.h>
#include <string.h>
#include <time.h>

#include "plplot.h"
#include "drivers.h"

/* Prototypes for functions in this file. */

static char *getdate(void);

/* top level declarations */

#define LINELENGTH      70
#define COPIES          1
#define XSIZE           540	/* 7.5" x 10"  (72 points equal 1 inch) */
#define YSIZE           720
#define ENLARGE         5
#define XPSSIZE         ENLARGE*XSIZE
#define YPSSIZE         ENLARGE*YSIZE
#define XOFFSET         36	/* Offsets are .5" each */
#define YOFFSET         36
#define XSCALE          100
#define YSCALE          100
#define LINESCALE       100
#define ANGLE           90
#define PSX             XPSSIZE-1
#define PSY             YPSSIZE-1

static char outbuf[128];
static int llx = XPSSIZE, lly = YPSSIZE, urx = 0, ury = 0, ptcnt;
static PLINT orient;

/* (dev) will get passed in eventually, so this looks weird right now */

static PLDev device;
static PLDev *dev = &device;

/*----------------------------------------------------------------------*\
* ps_init()
*
* Initialize device.
\*----------------------------------------------------------------------*/

void
ps_init(PLStream *pls)
{
    float r, g, b;

    pls->termin = 0;		/* not an interactive terminal */
    pls->icol0 = 1;
    pls->bytecnt = 0;
    pls->page = 0;

    if (!pls->colorset)
	pls->color = 0;		/* no color by default: user can override */

    if (pls->widthset) {
	if (pls->width < 1 || pls->width > 10) {
	    fprintf(stderr, "\nInvalid pen width selection.");
	    pls->width = 1;
	}
	pls->widthset = 0;
    }
    else
	pls->width = 1;

/* Initialize family file info */

    plFamInit(pls);

/* Prompt for a file name if not already set */

    plOpenFile(pls);

/* Set up device parameters */

    dev->xold = UNDEFINED;
    dev->yold = UNDEFINED;

    setpxl((PLFLT) 11.81, (PLFLT) 11.81);	/* 300 dpi */

/* Because portrait mode addressing is used by postscript, we need to
   rotate by 90 degrees to get the right mapping. */

    dev->xmin = 0;
    dev->ymin = 0;

    orient = pls->orient - 1;
    switch (orient) {

      case 1:
      case -1:
	dev->xmax = PSY;
	dev->ymax = PSX;
	break;

      default:
	dev->xmax = PSX;
	dev->ymax = PSY;
	break;
    }

    dev->xlen = dev->xmax - dev->xmin;
    dev->ylen = dev->ymax - dev->ymin;

    setphy(dev->xmin, dev->xmax, dev->ymin, dev->ymax);

    /* Header comments into PostScript file */

    fprintf(pls->OutFile, "%%!PS-Adobe-2.0 EPSF-2.0\n");
    fprintf(pls->OutFile, "%%%%BoundingBox: (atend)\n");
    fprintf(pls->OutFile, "%%%%Title: PLPLOT Graph\n");
    fprintf(pls->OutFile, "%%%%Creator: PLPLOT Version 4.0\n");
    fprintf(pls->OutFile, "%%%%CreationDate: %s\n", getdate());
    fprintf(pls->OutFile, "%%%%Pages: (atend)\n");
    fprintf(pls->OutFile, "%%%%EndComments\n\n");

    /* Definitions */

    fprintf(pls->OutFile, "/PSSave save def\n");	/* save VM state */
    fprintf(pls->OutFile, "/PSDict 200 dict def\n");	/* define a dictionary */
    fprintf(pls->OutFile, "PSDict begin\n");	/* start using it */
    fprintf(pls->OutFile, "/@restore /restore load def\n");
    fprintf(pls->OutFile, "/restore\n");
    fprintf(pls->OutFile, "   {vmstatus pop\n");
    fprintf(pls->OutFile, "    dup @VMused lt {pop @VMused} if\n");
    fprintf(pls->OutFile, "    exch pop exch @restore /@VMused exch def\n");
    fprintf(pls->OutFile, "   } def\n");
    fprintf(pls->OutFile, "/@pri\n");
    fprintf(pls->OutFile, "   {\n");
    fprintf(pls->OutFile, "    ( ) print\n");
    fprintf(pls->OutFile, "    (                                       ) cvs print\n");
    fprintf(pls->OutFile, "   } def\n");
    fprintf(pls->OutFile, "/@copies\n");	/* n @copies - */
    fprintf(pls->OutFile, "   {\n");
    fprintf(pls->OutFile, "    /#copies exch def\n");
    fprintf(pls->OutFile, "   } def\n");
    fprintf(pls->OutFile, "/@start\n");	/* - @start -  -- start everything */
    fprintf(pls->OutFile, "   {\n");
    fprintf(pls->OutFile, "    vmstatus pop /@VMused exch def pop\n");
    fprintf(pls->OutFile, "   } def\n");
    fprintf(pls->OutFile, "/@end\n");	/* - @end -  -- finished */
    fprintf(pls->OutFile, "   {(VM Used: ) print @VMused @pri\n");
    fprintf(pls->OutFile, "    (. Unused: ) print vmstatus @VMused sub @pri pop pop\n");
    fprintf(pls->OutFile, "    (\\n) print flush\n");
    fprintf(pls->OutFile, "    end\n");
    fprintf(pls->OutFile, "    PSSave restore\n");
    fprintf(pls->OutFile, "   } def\n");
    fprintf(pls->OutFile, "/bop\n");	/* bop -  -- begin a new page */
    fprintf(pls->OutFile, "   {\n");
    fprintf(pls->OutFile, "    /SaveImage save def\n");
    if (pls->color) {
	fprintf(pls->OutFile, "    Z %d %d M %d %d D %d %d D %d %d D %d %d",
		0, 0, 0, PSY, PSX, PSY, PSX, 0, 0, 0);
	r = pls->cmap0[0].r / 255.;
	g = pls->cmap0[0].g / 255.;
	b = pls->cmap0[0].b / 255.;
	fprintf(pls->OutFile, "    closepath %f %f %f setrgbcolor fill\n",
		r, g, b);
    }
    fprintf(pls->OutFile, "   } def\n");
    fprintf(pls->OutFile, "/eop\n");	/* - eop -  -- end a page */
    fprintf(pls->OutFile, "   {\n");
    fprintf(pls->OutFile, "    showpage\n");
    fprintf(pls->OutFile, "    SaveImage restore\n");
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

    /* Default line width */

    fprintf(pls->OutFile, "/lw %d def\n", pls->width);

    /* Stroke definitions, to keep output file as small as possible */

    fprintf(pls->OutFile, "/M {moveto} def\n");
    fprintf(pls->OutFile, "/D {lineto} def\n");
    fprintf(pls->OutFile, "/S {stroke} def\n");
    fprintf(pls->OutFile, "/Z {stroke newpath} def\n");
    fprintf(pls->OutFile, "end\n\n");	/* end of dictionary definition */

    /* Set up the plots */

    fprintf(pls->OutFile, "PSDict begin\n");
    fprintf(pls->OutFile, "@start\n");
    fprintf(pls->OutFile, "%d @copies\n", COPIES);
    fprintf(pls->OutFile, "@line\n");
    fprintf(pls->OutFile, "%d @hsize\n", YSIZE);
    fprintf(pls->OutFile, "%d @vsize\n", XSIZE);
    fprintf(pls->OutFile, "%d @hoffset\n", YOFFSET);
    fprintf(pls->OutFile, "%d @voffset\n", XOFFSET);
    fprintf(pls->OutFile, "%d @hscale\n", YSCALE);
    fprintf(pls->OutFile, "%d @vscale\n", XSCALE);
    fprintf(pls->OutFile, "%d @lscale\n", LINESCALE);
    fprintf(pls->OutFile, "@SetPlot\n\n");

    fprintf(pls->OutFile, "bop\n");
}

/*----------------------------------------------------------------------*\
* ps_line()
*
* Draw a line in the current color from (x1,y1) to (x2,y2).
\*----------------------------------------------------------------------*/

void
ps_line(PLStream *pls, PLSHORT x1a, PLSHORT y1a, PLSHORT x2a, PLSHORT y2a)
{
    int x1 = x1a, y1 = y1a, x2 = x2a, y2 = y2a;

    if (pls->linepos + 21 > LINELENGTH) {
	putc('\n', pls->OutFile);
	pls->linepos = 0;
    }
    else
	putc(' ', pls->OutFile);

    pls->bytecnt++;

/* Because portrait mode addressing is used here, we need to complement
   the orientation flag to get the right mapping. */

    orient = pls->orient - 1;
    plRotPhy(orient, dev, &x1, &y1, &x2, &y2);

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
* ps_polyline()
*
* Draw a polyline in the current color.
\*----------------------------------------------------------------------*/

void
ps_polyline(PLStream *pls, PLSHORT *xa, PLSHORT *ya, PLINT npts)
{
    PLINT i;

    for (i = 0; i < npts - 1; i++)
	ps_line(pls, xa[i], ya[i], xa[i + 1], ya[i + 1]);
}

/*----------------------------------------------------------------------*\
* ps_clear()
*
* Clear page.
\*----------------------------------------------------------------------*/

void
ps_clear(PLStream *pls)
{
    fprintf(pls->OutFile, " S\neop\nbop\n");
}

/*----------------------------------------------------------------------*\
* ps_page()
*
* Set up for the next page.
* Advance to next family file if necessary (file output).
\*----------------------------------------------------------------------*/

void
ps_page(PLStream *pls)
{
    dev->xold = UNDEFINED;
    dev->yold = UNDEFINED;

    if (!pls->termin)
	plGetFam(pls);

    pls->page++;
    fprintf(pls->OutFile, "%%%%Page: %d %d\n", pls->page, pls->page);
    pls->linepos = 0;
}

/*----------------------------------------------------------------------*\
* ps_adv()
*
* Advance to the next page.
\*----------------------------------------------------------------------*/

void
ps_adv(PLStream *pls)
{
    ps_clear(pls);
    ps_page(pls);
}

/*----------------------------------------------------------------------*\
* ps_tidy()
*
* Close graphics file or otherwise clean up.
\*----------------------------------------------------------------------*/

void
ps_tidy(PLStream *pls)
{
    fprintf(pls->OutFile, " S\neop\n");
    fprintf(pls->OutFile, "@end\n\n");
    fprintf(pls->OutFile, "%%%%Trailer\n");

    llx /= ENLARGE;
    lly /= ENLARGE;
    urx /= ENLARGE;
    ury /= ENLARGE;
    llx += XOFFSET;
    lly += YOFFSET;
    urx += XOFFSET;
    ury += YOFFSET;
    fprintf(pls->OutFile, "%%%%BoundingBox: %d %d %d %d\n", llx, lly, urx, ury);
    fprintf(pls->OutFile, "%%%%Pages: %d\n", pls->page);

    fclose(pls->OutFile);
    pls->fileset = 0;
    pls->page = 0;
    pls->linepos = 0;
    pls->OutFile = NULL;
}

/*----------------------------------------------------------------------*\
* ps_color()
*
* Set pen color.
\*----------------------------------------------------------------------*/

void
ps_color(PLStream *pls)
{
    float r, g, b;

    if (pls->color) {
	r = pls->curcolor.r / 255.0;
	g = pls->curcolor.g / 255.0;
	b = pls->curcolor.b / 255.0;

	fprintf(pls->OutFile, " S %f %f %f setrgbcolor\n", r, g, b);
    }
}

/*----------------------------------------------------------------------*\
* ps_text()
*
* Switch to text mode.
\*----------------------------------------------------------------------*/

void
ps_text(PLStream *pls)
{
}

/*----------------------------------------------------------------------*\
* ps_graph()
*
* Switch to graphics mode.
\*----------------------------------------------------------------------*/

void
ps_graph(PLStream *pls)
{
}

/*----------------------------------------------------------------------*\
* ps_width()
*
* Set pen width.
\*----------------------------------------------------------------------*/

void
ps_width(PLStream *pls)
{
    if (pls->width < 1 || pls->width > 10)
	fprintf(stderr, "\nInvalid pen width selection.");
    else {
	fprintf(pls->OutFile, " S\n/lw %d def\n@lwidth\n", pls->width);
    }
}

/*----------------------------------------------------------------------*\
* ps_esc()
*
* Escape function.
\*----------------------------------------------------------------------*/

void
ps_esc(PLStream *pls, PLINT op, char *ptr)
{
}

/*----------------------------------------------------------------------*\
* getdate()
*
* Get the date and time
\*----------------------------------------------------------------------*/

static char *
getdate(void)
{
    int len;
    long t;
    char *p;

    t = time((long *) 0);
    p = ctime(&t);
    len = strlen(p);
    *(p + len - 1) = '\0';	/* zap the newline character */
    return p;
}

#else
int 
pldummy_ps()
{
    return 0;
}

#endif				/* PS */
