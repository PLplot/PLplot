Date: Thu, 2 Apr 92 03:19:18 GMT
From: <amr@egr.duke.edu>
Message-Id: <9204020319.AA13063@ferry.egr.duke.edu>
To: mjl@fusion.ph.utexas.edu
Subject: New version of next.c

Dear Maurice,
  Here's a new, improved version of next.c.  It does about every thing I
want it to now, so I don't expect to be doing any more work on it for
a while.
Tony

/*	next.c

	PLPLOT NeXT display driver.
*/
#ifdef NEXT

#include <stdio.h>
#include "plplot.h"
#include "dispatch.h"

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
static int llx=XPSSIZE, lly=YPSSIZE, urx=0, ury=0, ptcnt;

/* (dev) will get passed in eventually, so this looks weird right now */

static PLDev device;
static PLDev *dev = &device;

/*----------------------------------------------------------------------*\
* nxinit()
*
* Initialize device.
\*----------------------------------------------------------------------*/

void 
nxinit(pls)
PLStream *pls;
{
    pls->termin = 1;		/* not an interactive terminal */
    pls->color = 1;
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
* nxline()
*
* Draw a line in the current color from (x1,y1) to (x2,y2).
\*----------------------------------------------------------------------*/

void 
nxline(pls, x1a, y1a, x2a, y2a)
PLStream *pls;
PLINT x1a, y1a, x2a, y2a;
{
    int x1=x1a, y1=y1a, x2=x2a, y2=y2a;
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
        llx = MIN(llx,x1);
        lly = MIN(lly,y1);
        urx = MAX(urx,x1);
        ury = MAX(ury,y1);
	ptcnt = 1;
    }
    llx = MIN(llx,x2);
    lly = MIN(lly,y2);
    urx = MAX(urx,x2);
    ury = MAX(ury,y2);

    fprintf(pls->OutFile, "%s", outbuf);
    pls->bytecnt += strlen(outbuf);
    dev->xold = x2;
    dev->yold = y2;
    pls->linepos += 21;
}

/*----------------------------------------------------------------------*\
* nxclear()
*
* Clear page. 
\*----------------------------------------------------------------------*/

void 
nxclear(pls)
PLStream *pls;
{
    fprintf(pls->OutFile, " S\neop\n");

    pclose(pls->OutFile);
    pls->fileset = 0;
    pls->page = 0;
    pls->linepos = 0;
    pls->OutFile = NULL;
}

/*----------------------------------------------------------------------*\
* nxpage()
*
* Set up for the next page.  
* Advance to next family file if necessary (file output).
\*----------------------------------------------------------------------*/

void 
nxpage(pls)
PLStream *pls;
{
    dev->xold = UNDEFINED;
    dev->yold = UNDEFINED;

/* Pipe output to Preview */

    pls->OutFile = popen("open","w");

    /* Header comments into PostScript file */

    fprintf(pls->OutFile, "%%!PS-Adobe-2.0 EPSF-2.0\n");
    fprintf(pls->OutFile, "%%%%Title: PLPLOT Graph\n");
    fprintf(pls->OutFile, "%%%%BoundingBox: 0 0 576 576\n");
    fprintf(pls->OutFile, "%%%%Creator: PLPLOT Version 4.0\n");
    fprintf(pls->OutFile, "%%%%EndComments\n\n");

  /* Definitions */

    fprintf(pls->OutFile, "/eop\n");       /* - eop -  -- end a page */
    fprintf(pls->OutFile, "   {\n");
    fprintf(pls->OutFile, "    showpage\n");
    fprintf(pls->OutFile, "   } def\n");
    fprintf(pls->OutFile, "/@line\n");     /* set line parameters */
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
    fprintf(pls->OutFile, "/@SetPlot\n");  /* setup user specified offsets, */
    fprintf(pls->OutFile, "   {\n");       /* scales, sizes for clipping    */
    fprintf(pls->OutFile, "    ho vo translate\n");
    fprintf(pls->OutFile, "    XScale YScale scale\n");
    fprintf(pls->OutFile, "    lin lw mul setlinewidth\n");
    fprintf(pls->OutFile, "   } def\n");
    fprintf(pls->OutFile, "/XScale\n");    /* setup x scale */
    fprintf(pls->OutFile, "   {hsc hs mul %d div} def\n", YPSSIZE);
    fprintf(pls->OutFile, "/YScale\n");    /* setup y scale */
    fprintf(pls->OutFile, "   {vsc vs mul %d div} def\n", XPSSIZE);
    fprintf(pls->OutFile, "/lw 1 def\n");  /* default line width */
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
* nxadv()
*
* Advance to the next page.
\*----------------------------------------------------------------------*/

void 
nxadv(pls)
PLStream *pls;
{
    nxclear(pls);
    nxpage(pls);
}

/*----------------------------------------------------------------------*\
* nxtidy()
*
* Close graphics file or otherwise clean up.
\*----------------------------------------------------------------------*/

void 
nxtidy(pls)
PLStream *pls;
{
    nxclear(pls);
}

/*----------------------------------------------------------------------*\
* nxcolor()
*
* Set pen color.
\*----------------------------------------------------------------------*/

void 
nxcolor(pls)
PLStream *pls;
{
}

/*----------------------------------------------------------------------*\
* nxtext()
*
* Switch to text mode.
\*----------------------------------------------------------------------*/

void 
nxtext(pls)
PLStream *pls;
{
}

/*----------------------------------------------------------------------*\
* nxgraph()
*
* Switch to graphics mode.
\*----------------------------------------------------------------------*/

void 
nxgraph(pls)
PLStream *pls;
{
}

/*----------------------------------------------------------------------*\
* nxwidth()
*
* Set pen width.
\*----------------------------------------------------------------------*/

void 
nxwidth(pls)
PLStream *pls;
{
    if (pls->width < 1 || pls->width > 10)
	fprintf(stderr, "\nInvalid pen width selection.");
    else {
	fprintf(pls->OutFile, " S\n/lw %d def\n@lwidth\n", pls->width);
    }
}

/*----------------------------------------------------------------------*\
* nxesc()
*
* Escape function.
\*----------------------------------------------------------------------*/

void 
nxesc(pls, op, ptr)
PLStream *pls;
PLINT op;
char *ptr;
{
}
#endif
