/* $Id$
 * $Log$
 * Revision 1.34  2000/12/18 21:01:49  airwin
 * Change to new style plplot/*.h header file locations.
 *
 * Revision 1.33  1995/03/11 20:27:13  mjl
 * All drivers: eliminated unnecessary variable initializations, other cleaning
 * up.
 *
 * Revision 1.32  1995/01/10  09:37:00  mjl
 * Fixed braindamage incurred last update.  Now switches to transparent
 * background mode (no page fill) automatically if background color is white.
 * Useful for including color EPSF files into TeX or whatever.
 *
 * Revision 1.31  1995/01/09  21:48:34  mjl
 * Fixed background color fill to cover the entire page, including margin
 * area.  
 *
 * Revision 1.30  1994/09/23  07:36:27  mjl
 * Now generates ps files with the correct PLplot version number written.
 *
 * Revision 1.29  1994/09/01  22:28:09  mjl
 * Fixed bug in current point update after state change.
 *
 * Revision 1.28  1994/08/27  03:40:32  mjl
 * Fix to allow cmap1 color selections to appear in grayscale.  Contributed
 * by Radey Shouman.
*/

/*	ps.c

	PLplot PostScript device driver.
*/
#include "plplot/plDevs.h"

#ifdef PLD_ps

#include "plplot/plplotP.h"
#include "plplot/drivers.h"

#include <string.h>
#include <time.h>

/* Prototypes for functions in this file. */

static char  *ps_getdate	(void);
static void  ps_init		(PLStream *);
static void  fill_polygon	(PLStream *pls);

/* top level declarations */

#define LINELENGTH      78
#define COPIES          1
#define XSIZE           540		/* 7.5 x 10 [inches]    */
#define YSIZE           720		/* (72 points = 1 inch) */
#define ENLARGE         5
#define XPSSIZE         ENLARGE*XSIZE
#define YPSSIZE         ENLARGE*YSIZE
#define XOFFSET         36		/* Margins --     */
#define YOFFSET         36		/* .5 inches each */
#define PSX             XPSSIZE-1
#define PSY             YPSSIZE-1
#define OF		pls->OutFile
#define MIN_WIDTH	1		/* Minimum pen width */
#define MAX_WIDTH	10		/* Maximum pen width */
#define DEF_WIDTH	3		/* Default pen width */

/* These are for covering the page with the background color */

#define XMIN		-XOFFSET*ENLARGE
#define XMAX		PSX+XOFFSET*ENLARGE
#define YMIN		-XOFFSET*ENLARGE
#define YMAX		PSY+XOFFSET*ENLARGE

/* Struct to hold device-specific info. */

typedef struct {
    PLFLT pxlx, pxly;
    PLINT xold, yold;

    PLINT xmin, xmax, xlen;
    PLINT ymin, ymax, ylen;

    PLINT xmin_dev, xmax_dev, xlen_dev;
    PLINT ymin_dev, ymax_dev, ylen_dev;

    PLFLT xscale_dev, yscale_dev;

    int llx, lly, urx, ury, ptcnt;
} PSDev;

static char outbuf[128];

/*--------------------------------------------------------------------------*\
 * plD_init_ps()
 *
 * Initialize device.
\*--------------------------------------------------------------------------*/

void
plD_init_psm(PLStream *pls)
{
    pls->color = 0;		/* Not a color device */
    ps_init(pls);
}

void
plD_init_psc(PLStream *pls)
{
    pls->color = 1;		/* Is a color device */
    ps_init(pls);
}

static void
ps_init(PLStream *pls)
{
    PSDev *dev;
    float pxlx = YPSSIZE/LPAGE_X;
    float pxly = XPSSIZE/LPAGE_Y;

    pls->family = 0;		/* Doesn't support familying for now */
    pls->dev_fill0 = 1;		/* Can do solid fills */

/* Prompt for a file name if not already set */

    plOpenFile(pls);

/* Allocate and initialize device-specific data */

    if (pls->dev != NULL)
	free((void *) pls->dev);

    pls->dev = calloc(1, (size_t) sizeof(PSDev));
    if (pls->dev == NULL)
	plexit("ps_init: Out of memory.");

    dev = (PSDev *) pls->dev;

    dev->xold = UNDEFINED;
    dev->yold = UNDEFINED;

    plP_setpxl(pxlx, pxly);

    dev->llx = XPSSIZE;
    dev->lly = YPSSIZE;
    dev->urx = 0;
    dev->ury = 0;
    dev->ptcnt = 0;

/* Rotate by 90 degrees since portrait mode addressing is used */

    dev->xmin = 0;
    dev->ymin = 0;
    dev->xmax = PSY;
    dev->ymax = PSX;
    dev->xlen = dev->xmax - dev->xmin;
    dev->ylen = dev->ymax - dev->ymin;

    plP_setphy(dev->xmin, dev->xmax, dev->ymin, dev->ymax);

/* Header comments into PostScript file */

    fprintf(OF, "%%!PS-Adobe-2.0 EPSF-2.0\n");
    fprintf(OF, "%%%%BoundingBox:         \n");
    fprintf(OF, "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n");

    fprintf(OF, "%%%%Title: PLplot Graph\n");
    fprintf(OF, "%%%%Creator: PLplot Version %s\n", PLPLOT_VERSION);
    fprintf(OF, "%%%%CreationDate: %s\n", ps_getdate());
    fprintf(OF, "%%%%Pages: (atend)\n");
    fprintf(OF, "%%%%EndComments\n\n");

/* Definitions */
/* Save VM state */

    fprintf(OF, "/PSSave save def\n");

/* Define a dictionary and start using it */

    fprintf(OF, "/PSDict 200 dict def\n");
    fprintf(OF, "PSDict begin\n");

    fprintf(OF, "/@restore /restore load def\n");
    fprintf(OF, "/restore\n");
    fprintf(OF, "   {vmstatus pop\n");
    fprintf(OF, "    dup @VMused lt {pop @VMused} if\n");
    fprintf(OF, "    exch pop exch @restore /@VMused exch def\n");
    fprintf(OF, "   } def\n");
    fprintf(OF, "/@pri\n");
    fprintf(OF, "   {\n");
    fprintf(OF, "    ( ) print\n");
    fprintf(OF, "    (                                       ) cvs print\n");
    fprintf(OF, "   } def\n");

/* n @copies - */

    fprintf(OF, "/@copies\n");
    fprintf(OF, "   {\n");
    fprintf(OF, "    /#copies exch def\n");
    fprintf(OF, "   } def\n");

/* - @start -  -- start everything */

    fprintf(OF, "/@start\n");
    fprintf(OF, "   {\n");
    fprintf(OF, "    vmstatus pop /@VMused exch def pop\n");
    fprintf(OF, "   } def\n");

/* - @end -  -- finished */

    fprintf(OF, "/@end\n");
    fprintf(OF, "   {flush\n");
    fprintf(OF, "    end\n");
    fprintf(OF, "    PSSave restore\n");
    fprintf(OF, "   } def\n");

/* bop -  -- begin a new page */
/* Only fill background if we are using color and if the bg isn't white */

    fprintf(OF, "/bop\n");
    fprintf(OF, "   {\n");
    fprintf(OF, "    /SaveImage save def\n");
    fprintf(OF, "   } def\n");

/* - eop -  -- end a page */

    fprintf(OF, "/eop\n");
    fprintf(OF, "   {\n");
    fprintf(OF, "    showpage\n");
    fprintf(OF, "    SaveImage restore\n");
    fprintf(OF, "   } def\n");

/* Set line parameters */

    fprintf(OF, "/@line\n");
    fprintf(OF, "   {0 setlinecap\n");
    fprintf(OF, "    0 setlinejoin\n");
    fprintf(OF, "    1 setmiterlimit\n");
    fprintf(OF, "   } def\n");

/* d @hsize -  horizontal clipping dimension */

    fprintf(OF, "/@hsize   {/hs exch def} def\n");
    fprintf(OF, "/@vsize   {/vs exch def} def\n");

/* d @hoffset - shift for the plots */

    fprintf(OF, "/@hoffset {/ho exch def} def\n");
    fprintf(OF, "/@voffset {/vo exch def} def\n");

/* Set line width */

    fprintf(OF, "/lw %d def\n", (int) (
	(pls->width < MIN_WIDTH) ? DEF_WIDTH :
	(pls->width > MAX_WIDTH) ? MAX_WIDTH : pls->width));

/* Setup user specified offsets, scales, sizes for clipping */

    fprintf(OF, "/@SetPlot\n");
    fprintf(OF, "   {\n");
    fprintf(OF, "    ho vo translate\n");
    fprintf(OF, "    XScale YScale scale\n");
    fprintf(OF, "    lw setlinewidth\n");
    fprintf(OF, "   } def\n");

/* Setup x & y scales */

    fprintf(OF, "/XScale\n");
    fprintf(OF, "   {hs %d div} def\n", YPSSIZE);
    fprintf(OF, "/YScale\n");
    fprintf(OF, "   {vs %d div} def\n", XPSSIZE);

/* Macro definitions of common instructions, to keep output small */

    fprintf(OF, "/M {moveto} def\n");
    fprintf(OF, "/D {lineto} def\n");
    fprintf(OF, "/S {stroke} def\n");
    fprintf(OF, "/Z {stroke newpath} def\n");
    fprintf(OF, "/F {fill} def\n");
    fprintf(OF, "/C {setrgbcolor} def\n");
    fprintf(OF, "/G {setgray} def\n");
    fprintf(OF, "/W {setlinewidth} def\n");
    fprintf(OF, "/B {Z %d %d M %d %d D %d %d D %d %d D %d %d closepath} def\n",
	    XMIN, YMIN, XMIN, YMAX, XMAX, YMAX, XMAX, YMIN, XMIN, YMIN);

/* End of dictionary definition */

    fprintf(OF, "end\n\n");

/* Set up the plots */

    fprintf(OF, "PSDict begin\n");
    fprintf(OF, "@start\n");
    fprintf(OF, "%d @copies\n", COPIES);
    fprintf(OF, "@line\n");
    fprintf(OF, "%d @hsize\n", YSIZE);
    fprintf(OF, "%d @vsize\n", XSIZE);
    fprintf(OF, "%d @hoffset\n", YOFFSET);
    fprintf(OF, "%d @voffset\n", XOFFSET);

    fprintf(OF, "@SetPlot\n\n");
}

/*--------------------------------------------------------------------------*\
 * plD_line_ps()
 *
 * Draw a line in the current color from (x1,y1) to (x2,y2).
\*--------------------------------------------------------------------------*/

void
plD_line_ps(PLStream *pls, short x1a, short y1a, short x2a, short y2a)
{
    PSDev *dev = (PSDev *) pls->dev;
    int x1 = x1a, y1 = y1a, x2 = x2a, y2 = y2a;

/* Rotate by 90 degrees */

    plRotPhy(1, dev->xmin, dev->ymin, dev->xmax, dev->ymax, &x1, &y1);
    plRotPhy(1, dev->xmin, dev->ymin, dev->xmax, dev->ymax, &x2, &y2);

    if (x1 == dev->xold && y1 == dev->yold && dev->ptcnt < 40) {
	if (pls->linepos + 12 > LINELENGTH) {
	    putc('\n', OF);
	    pls->linepos = 0;
	}
	else
	    putc(' ', OF);

	sprintf(outbuf, "%d %d D", x2, y2);
	dev->ptcnt++;
	pls->linepos += 12;
    }
    else {
	fprintf(OF, " Z\n");
	pls->linepos = 0;

	sprintf(outbuf, "%d %d M %d %d D", x1, y1, x2, y2);
	dev->llx = MIN(dev->llx, x1);
	dev->lly = MIN(dev->lly, y1);
	dev->urx = MAX(dev->urx, x1);
	dev->ury = MAX(dev->ury, y1);
	dev->ptcnt = 1;
	pls->linepos += 24;
    }
    dev->llx = MIN(dev->llx, x2);
    dev->lly = MIN(dev->lly, y2);
    dev->urx = MAX(dev->urx, x2);
    dev->ury = MAX(dev->ury, y2);

    fprintf(OF, "%s", outbuf);
    pls->bytecnt += 1 + strlen(outbuf);
    dev->xold = x2;
    dev->yold = y2;
}

/*--------------------------------------------------------------------------*\
 * plD_polyline_ps()
 *
 * Draw a polyline in the current color.
\*--------------------------------------------------------------------------*/

void
plD_polyline_ps(PLStream *pls, short *xa, short *ya, PLINT npts)
{
    PLINT i;

    for (i = 0; i < npts - 1; i++)
	plD_line_ps(pls, xa[i], ya[i], xa[i + 1], ya[i + 1]);
}

/*--------------------------------------------------------------------------*\
 * plD_eop_ps()
 *
 * End of page.
\*--------------------------------------------------------------------------*/

void
plD_eop_ps(PLStream *pls)
{
    fprintf(OF, " S\neop\n");
}

/*--------------------------------------------------------------------------*\
 * plD_bop_ps()
 *
 * Set up for the next page.
\*--------------------------------------------------------------------------*/

void
plD_bop_ps(PLStream *pls)
{
    PSDev *dev = (PSDev *) pls->dev;

    dev->xold = UNDEFINED;
    dev->yold = UNDEFINED;

    pls->page++;
    fprintf(OF, "%%%%Page: %d %d\n", (int) pls->page, (int) pls->page);
    fprintf(OF, "bop\n");
    if (pls->color) {
	float r, g, b;
	if (pls->cmap0[0].r != 0xFF ||
	    pls->cmap0[0].g != 0xFF ||
	    pls->cmap0[0].b != 0xFF ) {

	    r = ((float) pls->cmap0[0].r) / 255.;
	    g = ((float) pls->cmap0[0].g) / 255.;
	    b = ((float) pls->cmap0[0].b) / 255.;

	    fprintf(OF, "B %.4f %.4f %.4f C F\n", r, g, b);
	}
    }
    pls->linepos = 0;

/* This ensures the color is set correctly at the beginning of each page */

    plD_state_ps(pls, PLSTATE_COLOR0);
}

/*--------------------------------------------------------------------------*\
 * plD_tidy_ps()
 *
 * Close graphics file or otherwise clean up.
\*--------------------------------------------------------------------------*/

void
plD_tidy_ps(PLStream *pls)
{
    PSDev *dev = (PSDev *) pls->dev;

    fprintf(OF, "\n%%%%Trailer\n");

    dev->llx /= ENLARGE;
    dev->lly /= ENLARGE;
    dev->urx /= ENLARGE;
    dev->ury /= ENLARGE;
    dev->llx += XOFFSET;
    dev->lly += YOFFSET;
    dev->urx += XOFFSET;
    dev->ury += YOFFSET;

/* changed for correct Bounding boundaries Jan Thorbecke  okt 1993*/
/* occurs from the integer truncation -- postscript uses fp arithmetic */

    dev->urx += 1;
    dev->ury += 1;

    fprintf(OF, "%%%%Pages: %d\n", (int) pls->page);
    fprintf(OF, "@end\n");

/* Backtrack to write the BoundingBox at the beginning */
/* Some applications don't like it atend */

    rewind(OF);
    fprintf(OF, "%%!PS-Adobe-2.0 EPSF-2.0\n");
    fprintf(OF, "%%%%BoundingBox: %d %d %d %d\n",
	    dev->llx, dev->lly, dev->urx, dev->ury);
    fclose(OF);
}

/*--------------------------------------------------------------------------*\
 * plD_state_ps()
 *
 * Handle change in PLStream state (color, pen width, fill attribute, etc).
\*--------------------------------------------------------------------------*/

void 
plD_state_ps(PLStream *pls, PLINT op)
{
    PSDev *dev = (PSDev *) pls->dev;

    switch (op) {

    case PLSTATE_WIDTH:{
	int width = 
	    (pls->width < MIN_WIDTH) ? DEF_WIDTH :
	    (pls->width > MAX_WIDTH) ? MAX_WIDTH : pls->width;

	fprintf(OF, " S\n%d W", width);

	dev->xold = UNDEFINED;
	dev->yold = UNDEFINED;
	break;
    }
    case PLSTATE_COLOR0:
 	if (! pls->color) {
	    fprintf(OF, " S\n%.4f G", (pls->icol0 ? 0.0 : 1.0));
	    break;
	}
	/* else fallthrough */
    case PLSTATE_COLOR1:
	if (pls->color) {
	    float r = ((float) pls->curcolor.r) / 255.0;
	    float g = ((float) pls->curcolor.g) / 255.0;
	    float b = ((float) pls->curcolor.b) / 255.0;

	    fprintf(OF, " S\n%.4f %.4f %.4f C", r, g, b);
	}
	else {
	    float r = ((float) pls->curcolor.r) / 255.0;
	    fprintf(OF, " S\n%.4f G", 1.0 - r);
	}
	break;
    }

/* Reinitialize current point location. */

    if (dev->xold != UNDEFINED && dev->yold != UNDEFINED) {
	fprintf(OF, " %d %d M \n", (int)dev->xold, (int)dev->yold);
    }
}

/*--------------------------------------------------------------------------*\
 * plD_esc_ps()
 *
 * Escape function.
\*--------------------------------------------------------------------------*/

void
plD_esc_ps(PLStream *pls, PLINT op, void *ptr)
{
    switch (op) {
      case PLESC_FILL:
	fill_polygon(pls);
	break;
    }
}

/*--------------------------------------------------------------------------*\
 * fill_polygon()
 *
 * Fill polygon described in points pls->dev_x[] and pls->dev_y[].
 * Only solid color fill supported.
\*--------------------------------------------------------------------------*/

static void
fill_polygon(PLStream *pls)
{
    PSDev *dev = (PSDev *) pls->dev;
    PLINT n, ix = 0, iy = 0;
    int x, y;

    fprintf(OF, " Z\n");

    for (n = 0; n < pls->dev_npts; n++) {
	x = pls->dev_x[ix++];
	y = pls->dev_y[iy++];

/* Rotate by 90 degrees */

	plRotPhy(1, dev->xmin, dev->ymin, dev->xmax, dev->ymax, &x, &y);

/* First time through start with a x y moveto */

	if (n == 0) {
	    sprintf(outbuf, "%d %d M", x, y);
	    dev->llx = MIN(dev->llx, x);
	    dev->lly = MIN(dev->lly, y);
	    dev->urx = MAX(dev->urx, x);
	    dev->ury = MAX(dev->ury, y);
	    fprintf(OF, "%s", outbuf);
	    pls->bytecnt += strlen(outbuf);
	    continue;
	}

	if (pls->linepos + 21 > LINELENGTH) {
	    putc('\n', OF);
	    pls->linepos = 0;
	}
	else
	    putc(' ', OF);

	pls->bytecnt++;

	sprintf(outbuf, "%d %d D", x, y);
	dev->llx = MIN(dev->llx, x);
	dev->lly = MIN(dev->lly, y);
	dev->urx = MAX(dev->urx, x);
	dev->ury = MAX(dev->ury, y);

	fprintf(OF, "%s", outbuf);
	pls->bytecnt += strlen(outbuf);
	pls->linepos += 21;
    }
    dev->xold = UNDEFINED;
    dev->yold = UNDEFINED;
    fprintf(OF, " F ");
}

/*--------------------------------------------------------------------------*\
 * ps_getdate()
 *
 * Get the date and time
\*--------------------------------------------------------------------------*/

static char *
ps_getdate(void)
{
    int len;
    time_t t;
    char *p;

    t = time((time_t *) 0);
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

#endif				/* PLD_ps */
