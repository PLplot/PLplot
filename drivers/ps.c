/* $Id$

	PLplot PostScript device driver.
*/
#include "plplot/plDevs.h"

#ifdef PLD_ps

#include "plplot/plplotP.h"
#include "plplot/drivers.h"
#include "plplot/ps.h"

#include <string.h>
#include <time.h>

/* Prototypes for functions in this file. */

static char  *ps_getdate	(void);
static void  ps_init		(PLStream *);
static void  fill_polygon	(PLStream *pls);
static void  proc_str           (PLStream *, EscText *);

static char  outbuf[128];
static int text = 0;
static DrvOpt driver_options[] = {{"text", DRV_INT, &text, "Use Postscript text (text=1|0)"},
				  {NULL, DRV_INT, NULL, NULL}};

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

    PLFLT pxlx = YPSSIZE/LPAGE_X;
    PLFLT pxly = XPSSIZE/LPAGE_Y;

    plParseDrvOpts(driver_options);
    if (text)
      pls->dev_text = 1; /* want to draw text */

    pls->dev_fill0 = 1;		/* Can do solid fills */

/* Initialize family file info */

    plFamInit(pls);

/* Prompt for a file name if not already set */

    plOpenFile(pls);

/* Allocate and initialize device-specific data */

    if (pls->dev != NULL)
	free((void *) pls->dev);

    pls->dev = calloc(1, (size_t) sizeof(PSDev));
    if (pls->dev == NULL)
	plexit("ps_init: Out of memory.");

    dev = (PSDev *) pls->dev;

    dev->xold = PL_UNDEFINED;
    dev->yold = PL_UNDEFINED;

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
 * Advance to next family file if necessary (file output).
\*--------------------------------------------------------------------------*/

void
plD_bop_ps(PLStream *pls)
{
    PSDev *dev = (PSDev *) pls->dev;

    dev->xold = PL_UNDEFINED;
    dev->yold = PL_UNDEFINED;

    if (!pls->termin)
	plGetFam(pls);

    pls->page++;

    if (pls->family)
        fprintf(OF, "%%%%Page: %d %d\n", (int) pls->page, 1);
    else
        fprintf(OF, "%%%%Page: %d %d\n", (int) pls->page, (int) pls->page);

    fprintf(OF, "bop\n");
    if (pls->color) {
	PLFLT r, g, b;
	if (pls->cmap0[0].r != 0xFF ||
	    pls->cmap0[0].g != 0xFF ||
	    pls->cmap0[0].b != 0xFF ) {

	    r = ((PLFLT) pls->cmap0[0].r) / 255.;
	    g = ((PLFLT) pls->cmap0[0].g) / 255.;
	    b = ((PLFLT) pls->cmap0[0].b) / 255.;

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

    if (pls->family)
        fprintf(OF, "%%%%Pages: %d\n", (int) 1);
    else
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

	dev->xold = PL_UNDEFINED;
	dev->yold = PL_UNDEFINED;
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
	    PLFLT r = ((PLFLT) pls->curcolor.r) / 255.0;
	    PLFLT g = ((PLFLT) pls->curcolor.g) / 255.0;
	    PLFLT b = ((PLFLT) pls->curcolor.b) / 255.0;

	    fprintf(OF, " S\n%.4f %.4f %.4f C", r, g, b);
	}
	else {
	    PLFLT r = ((PLFLT) pls->curcolor.r) / 255.0;
	    fprintf(OF, " S\n%.4f G", 1.0 - r);
	}
	break;
    }

/* Reinitialize current point location. */

    if (dev->xold != PL_UNDEFINED && dev->yold != PL_UNDEFINED) {
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
    case PLESC_HAS_TEXT:
      proc_str(pls, ptr);
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
    dev->xold = PL_UNDEFINED;
    dev->yold = PL_UNDEFINED;
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


void
proc_str (PLStream *pls, EscText *args)
{
  PLFLT *t = args->xform;
  PLFLT a1, alpha, ft_ht, angle, ref;
  PSDev *dev = (PSDev *) pls->dev;
  char *font;
  float font_factor = 1.6;

  /* font height */
  ft_ht = pls->chrht * 72.0/25.4; /* ft_ht in points. ht is in mm */

  /* calculate baseline text angle */
  angle = pls->diorot * 90.;
  a1 = acos(t[0]) * 180. / PI;
  if (t[2] > 0.)
    alpha = a1 - angle;
  else
    alpha = 360. - a1 - angle;

  /* TODO: parse string for format (escape) characters */
  //parse_str(args->string, return_string);

  /* 
   * Reference point (center baseline of string)
   *  If base = 0, it is aligned with the center of the text box
   *  If base = 1, it is aligned with the baseline of the text box
   *  If base = 2, it is aligned with the top of the text box
   *  Currently plplot only uses base=0
   *  postscript use base=1
   */ 

  if (args->base == 2) /* not supported by plplot */
    ref = ENLARGE * ft_ht / 2.; /* half font height */
  else if (args->base == 1)
    ref = 0.;
  else
    ref = -ENLARGE * ft_ht / 2.;

  /* rotate point in postscript is lower left corner, compensate */
  args->y = args->y + ref*cos(alpha * PI/180.);
  args->x = args->x - ref*sin(alpha * PI/180.);

  /*
   *  font family, serie and shape. Currently not fully supported by plplot
   *
   *  Use Postscript Times
   *  1: Normal font
   *  2: Roman font
   *  3: Italic font
   *  4: sans serif
   */

  switch (pls->cfont) {
  case (1): font = "Times-Roman"; break;
  case (2): font = "Times-Roman"; break;
  case (3): font = "Times-Italic"; break;
  case (4): font = "Helvetica"; break;
  default:  font = "Times-Roman"; break;
  }

  /* ps driver is rotated by default, compensate */
  plRotPhy(1, dev->xmin, dev->ymin, dev->xmax, dev->ymax, &(args->x), &(args->y));

  fprintf(OF, " %d %d moveto\n", args->x, args->y );
  fprintf(OF, "/%s findfont %f scalefont setfont\n", font, font_factor * ENLARGE * ft_ht);
  fprintf(OF, "gsave %f rotate ", alpha - 90.);
  fprintf(OF, "(%s) stringwidth %f mul exch %f mul exch rmoveto ",
	  args->string, - args->just, - args->just );
  fprintf(OF, "(%s) show grestore\n", args->string);

  /*
   * keep driver happy -- needed for background and orientation.
   * arghhh! can't calculate it, as I only have the string reference
   * point, not its extent!
   * Quick (and final?) *hack*, ASSUME that no more than two char height
   * extents after/before the string reference point.
   */

  dev->llx = MIN(dev->llx, args->x - 2. * font_factor * ft_ht * 25.4 / 72. * pls->xpmm);
  dev->lly = MIN(dev->lly, args->y - 2. * font_factor * ft_ht * 25.4 / 72. * pls->xpmm);
  dev->urx = MAX(dev->urx, args->x + 2. * font_factor * ft_ht * 25.4 / 72. * pls->xpmm);
  dev->ury = MAX(dev->ury, args->y + 2. * font_factor * ft_ht * 25.4 / 72. * pls->xpmm);
}


#else
int 
pldummy_ps()
{
    return 0;
}

#endif				/* PLD_ps */
