/* $Id$

	PLplot PostScript device driver.

  Copyright (C) 1992, 2001  Geoffrey Furnish  
  Copyright (C) 1992, 1993, 1994, 1995, 2001  Maurice LeBrun
  Copyright (C) 2000, 2001, 2002, 2004  Alan W. Irwin 
  Copyright (C) 2001, 2002  Joao Cardoso  
  Copyright (C) 2001, 2003, 2004  Rafael Laboissiere
  Copyright (C) 2004  Thomas J. Duck

  This file is part of PLplot.

  PLplot is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Library Public License as published
  by the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  PLplot is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU Library General Public License for more details.

  You should have received a copy of the GNU Library General Public License
  along with PLplot; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA

*/

#include "plDevs.h"

#ifdef PLD_ps

#include "plplotP.h"
#include "drivers.h"
#include "ps.h"

#include <string.h>
#include <time.h>


/* Device info */

char* plD_DEVICE_INFO_ps = 
  "ps:PostScript File (monochrome):0:ps:29:psm\n"
  "psc:PostScript File (color):0:ps:30:psc";


/* Prototypes for functions in this file. */

void plD_dispatch_init_psm	( PLDispatchTable *pdt );
void plD_dispatch_init_psc	( PLDispatchTable *pdt );

static char  *ps_getdate	(void);
static void  ps_init		(PLStream *);
static void  fill_polygon	(PLStream *pls);
static void  proc_str           (PLStream *, EscText *);
static void  esc_purge          (char *, const char *);

static char  outbuf[128];
static int text = 0;
static int color;

static DrvOpt ps_options[] = {{"text", DRV_INT, &text, "Use Postscript text (text=0|1)"},
			      {"color", DRV_INT, &color, "Use color (color=0|1)"},
			      {NULL, DRV_INT, NULL, NULL}};

/*--------------------------------------------------------------------------*\
 * adobe_symbol_enc (in)
 *
 * Trnasforms the character in from the PLplot Greek convention into the
 * Adobe encoding for the Symbol font.
\*--------------------------------------------------------------------------*/

static char adobe_symbol_enc (char in) 
{
  char retval = in;
  
  switch (in) {
  case 'C': retval = 'X'; break;
  case 'Y': retval = 'H'; break;
  case 'H': retval = 'Q'; break;
  case 'Q': retval = 'Y'; break;
  case 'X': retval = 'C'; break;
  case 'c': retval = 'x'; break;    
  case 'y': retval = 'h'; break;
  case 'h': retval = 'q'; break;
  case 'q': retval = 'y'; break;
  case 'x': retval = 'c'; break;
  }
    
  return retval;
}


/* text > 0 uses some postscript tricks, namely a transformation matrix
   that scales, rotates (with slanting) and offsets text strings.
   It has yet some bugs for 3d plots. */


static void ps_dispatch_init_helper( PLDispatchTable *pdt,
                                     char *menustr, char *devnam,
                                     int type, int seq, plD_init_fp init )
{
#ifndef ENABLE_DYNDRIVERS
    pdt->pl_MenuStr = menustr;
    pdt->pl_DevName = devnam;
#endif
    pdt->pl_type = type;
    pdt->pl_seq = seq;
    pdt->pl_init     = init;
    pdt->pl_line     = (plD_line_fp)     plD_line_ps;
    pdt->pl_polyline = (plD_polyline_fp) plD_polyline_ps;
    pdt->pl_eop      = (plD_eop_fp)      plD_eop_ps;
    pdt->pl_bop      = (plD_bop_fp)      plD_bop_ps;
    pdt->pl_tidy     = (plD_tidy_fp)     plD_tidy_ps;
    pdt->pl_state    = (plD_state_fp)    plD_state_ps;
    pdt->pl_esc      = (plD_esc_fp)      plD_esc_ps;
}

void plD_dispatch_init_psm	( PLDispatchTable *pdt )
{
    ps_dispatch_init_helper( pdt,
                             "PostScript File (monochrome)", "ps",
                             plDevType_FileOriented, 29,
                             (plD_init_fp) plD_init_psm );
}

void plD_dispatch_init_psc	( PLDispatchTable *pdt )
{
    ps_dispatch_init_helper( pdt,
                             "PostScript File (color)", "psc",
                             plDevType_FileOriented, 30,
                             (plD_init_fp) plD_init_psc );
}

/*--------------------------------------------------------------------------*\
 * plD_init_ps()
 *
 * Initialize device.
\*--------------------------------------------------------------------------*/

void
plD_init_psm(PLStream *pls)
{
  color = 0;
  pls->color = 0;		/* Not a color device */

  plParseDrvOpts(ps_options);
  if (color)
    pls->color = 1;		/* But user wants color */
  ps_init(pls);
}

void
plD_init_psc(PLStream *pls)
{
  color = 1;
  pls->color = 1;		/* Is a color device */
  plParseDrvOpts(ps_options);

  if (!color)
    pls->color = 0;		/* But user does not want color */
  ps_init(pls);
}

static void
ps_init(PLStream *pls)
{
    PSDev *dev;

    PLFLT pxlx = YPSSIZE/LPAGE_X;
    PLFLT pxly = XPSSIZE/LPAGE_Y;

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

/* If portrait mode is specified, then set up an additional rotation 
 * transformation with aspect ratio allowed to adjust via freeaspect.  
 * Default orientation is landscape (ORIENTATION == 3 or 90 deg rotation 
 * counter-clockwise from portrait).  (Legacy PLplot used seascape
 * which was equivalent to ORIENTATION == 1 or 90 deg clockwise rotation 
 * from portrait.) */

    if (pls->portrait) {
       plsdiori((PLFLT)(4 - ORIENTATION));
       pls->freeaspect = 1;
    }

/* Header comments into PostScript file */

    fprintf(OF, "%%!PS-Adobe-2.0 EPSF-2.0\n");
    fprintf(OF, "%%%%BoundingBox:         \n");
    fprintf(OF, "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n");

    fprintf(OF, "%%%%Title: PLplot Graph\n");
    fprintf(OF, "%%%%Creator: PLplot Version %s\n", VERSION);
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
    fprintf(OF, "/A {0.5 0 360 arc} def\n");
    fprintf(OF, "/S {stroke} def\n");
    fprintf(OF, "/Z {stroke newpath} def\n");
    fprintf(OF, "/F {fill} def\n");
    fprintf(OF, "/C {setrgbcolor} def\n");
    fprintf(OF, "/G {setgray} def\n");
    fprintf(OF, "/W {setlinewidth} def\n");
    fprintf(OF, "/SF {selectfont} def\n");
    fprintf(OF, "/R {rotate} def\n");
    fprintf(OF, "/SW {stringwidth 2 index mul exch 2 index mul exch rmoveto pop} bind def\n");
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

    plRotPhy(ORIENTATION, dev->xmin, dev->ymin, dev->xmax, dev->ymax, &x1, &y1);
    plRotPhy(ORIENTATION, dev->xmin, dev->ymin, dev->xmax, dev->ymax, &x2, &y2);

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

	if (x1 == x2 && y1 == y2) /* must be a single dot, draw a circle */
	  sprintf(outbuf, "%d %d A", x1, y1);
	else
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

/* This ensures the color and line width are set correctly at the beginning of
   each page */

    plD_state_ps(pls, PLSTATE_COLOR0);
    plD_state_ps(pls, PLSTATE_WIDTH);
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
      proc_str(pls, (EscText *)ptr);
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

	plRotPhy(ORIENTATION, dev->xmin, dev->ymin, dev->xmax, dev->ymax, &x, &y);

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


/*--------------------------------------------------------------------------*\
 * proc_str()
 *
 * Prints postscript strings.
 *
 * 5 Dec 04: Significantly revised by Tom Duck <tom.duck@dal.ca>.
 *
\*--------------------------------------------------------------------------*/

void
proc_str (PLStream *pls, EscText *args)
{
  PLFLT *t = args->xform, tt[4]; /* Transform matrices */
  PLFLT theta, shear;  /* Rotation angle and shear from the matrix */
  PLFLT ft_ht, offset; /* Font height and offset */
  PSDev *dev = (PSDev *) pls->dev;
  char *font, *ofont, str[128], esc, *strp;
  const char *cur_str;
  float font_factor = 1.4;
  int symbol;
  PLINT clxmin, clxmax, clymin, clymax; /* Clip limits */

  PLFLT scale = 1., up = 0.; /* Font scaling and shifting parameters */

  int i=0; /* String index */

  /* finish previous polyline */

  dev->xold = PL_UNDEFINED;
  dev->yold = PL_UNDEFINED;

  /* Determine the font height */
  ft_ht = pls->chrht * 72.0/25.4; /* ft_ht in points, ht is in mm */

  /*
   * Choose the font family, series and shape. Currently not fully 
   * supported by plplot
   *
   * For plplot:
   *   1: Normal font
   *   2: Roman font
   *   3: Italic font
   *   4: cursive
   */
  switch (pls->cfont) {
    case 1: ofont = "Helvetica"; break;
    case 2: ofont = "Times-Roman"; break;
    case 3: ofont = "Times-Italic"; break;
    case 4: ofont = "ZapfChancery"; break; /* there is no script (cursive) 
					    * font in the standard 35 
					    * postscript fonts.  
					    * ZapfChancery is the 
					    * fanciest. 
					    */
    default:  ofont = "Helvetica";
  }

  /* The transform matrix has only rotations and shears; extract them */
  theta = acos(t[0]) * 180. / PI;  /* Determine the rotation (in degrees)... */
  if (t[2] < 0.) theta *= -1.;     /* ... and sign ... */
  if(cos(theta*PI/180.)<0.000001)  /* ... and shear */
    shear = t[3]/sin(theta*PI/180.);
  else shear = (t[1]+sin(theta*PI/180.))/cos(theta*PI/180.); 

  /* 
   * Reference point conventions:
   *   If base = 0, it is aligned with the center of the text box
   *   If base = 1, it is aligned with the baseline of the text box
   *   If base = 2, it is aligned with the top of the text box
   *
   * Currently plplot only uses base=0
   * Postscript uses base=1
   *
   * We must calculate the difference between the two and apply the offset.
   */ 

  if (args->base == 2) /* not supported by plplot */
    offset = ENLARGE * ft_ht / 2.; /* half font height */
  else if (args->base == 1)
    offset = 0.;
  else
    offset = -ENLARGE * ft_ht / 2.;

  args->y += offset*cos(theta*PI/180.);
  args->x -= offset*sin(theta*PI/180.);

  /* Apply plplot difilt transformations */
  difilt(&args->x, &args->y, 1, &clxmin, &clxmax, &clymin, &clymax);

  /* ps driver is rotated by default */
  plRotPhy(ORIENTATION, dev->xmin, dev->ymin, dev->xmax, dev->ymax, 
	   &(args->x), &(args->y));

  /* Make adjustments for page orientation */
  theta+=90. - 90.*pls->diorot;
   

  /* Output */

  plgesc(&esc);

  cur_str = args->string;

  /* move to string reference point */
  fprintf(OF, " %d %d M\n", args->x, args->y );

  /* Save the current position and set the string rotation */
  fprintf(OF, "gsave %.3f R\n",theta);

  /* Purge escape sequences from string, so that postscript can find it's 
   * length.  The string length is computed with the current font, and can
   * thus be wrong if there are font change escape sequences in the string 
   */

  esc_purge(str, args->string);

  fprintf(OF, "/%s %.3f SF\n", ofont,font_factor * ENLARGE * ft_ht);    

  /* Output string, writing brackets as \( and \); this string is
   * output for measurement purposes only.
   */
  fprintf(OF, "%.3f (", - args->just);
  while (str[i]!='\0') {
    if (str[i]=='(') fprintf(OF,"\\(");
    else {
      if (str[i]==')') fprintf(OF,"\\)");
      else fprintf(OF,"%c",str[i]);
    }
    i++;
  }
  fprintf(OF,") SW\n");


  /* Parse string for escape sequences and print everything out */

  do {

    strp = str;
    font = ofont;
    symbol = 0;

    if (*cur_str == esc) {
      cur_str++;

      if (*cur_str == esc) { /* <esc><esc> */
	*strp++ = *cur_str++;
      }
      else switch (*cur_str) {

        case 'f':
	cur_str++;
	switch (*cur_str) {
	case 'n': font = "Helvetica"; break;
	case 'r': font = "Times-Roman"; break;
	case 'i': font = "Times-Italic"; break;
	case 's': font = "ZapfChancery"; break;  /* there is no script
						  * (cursive) font in the 
						  * standard 35 postscript 
						  * fonts.  ZapfChancery is 
						  * the fanciest. 
						  */
	default:  font = "Helvetica";
	}
	cur_str++;
	break;

      case 'd':
	if(up>0.) scale *= 1.25;  /* Subscript scaling parameter */
	else scale *= 0.8;  /* Subscript scaling parameter */
	up -= font_factor * ENLARGE * ft_ht / 2.;
	cur_str++;
	break;

      case 'u':
	if(up<0.) scale *= 1.25;  /* Subscript scaling parameter */
	else scale *= 0.8;  /* Subscript scaling parameter */
	up += font_factor * ENLARGE * ft_ht / 2.;
	cur_str++;
	break;

      case 'g':
	cur_str++;
	ofont = font;
	*strp++ = adobe_symbol_enc (*cur_str++);
	symbol = 1;
	font = "Symbol";
	break;

	/* ignore the next sequences */

      case '+':
      case '-':
      case 'b':
	plwarn("'+', '-', and 'b' text escape sequences not processed.");
	cur_str++;
	break;

      case '(':
	plwarn("'g(...)' text escape sequence not processed.");
	while (*cur_str++ != ')');
	break;
      }
    }

    /* copy from current to next token, adding a postscript escape 
     * char \ if necessary 
     */
    while(!symbol && *cur_str && *cur_str != esc) {
      if (*cur_str == '(' || *cur_str == ')')
	*strp++ = '\\';
      *strp++ = *cur_str++;
    }
    *strp = '\0';

    if(fabs(up)<0.001) up = 0.; /* Watch out for small differences */

    /* Apply the scaling and the shear */
    fprintf(OF, "/%s [%.3f 0 %.3f %.3f 0 0] SF\n",
	    font,
	    font_factor * ENLARGE * ft_ht * scale,
	    shear * font_factor * ENLARGE * ft_ht * scale,
	    font_factor * ENLARGE * ft_ht * scale);

    /* if up/down escape sequences, save current point and adjust baseline */

    if(up!=0.) fprintf(OF, "gsave 0 %.3f rmoveto\n", up);

    /* print the string */
    fprintf(OF, "(%s) show\n", str);  

    /* back to baseline */
    if (up!=0.) fprintf(OF, "grestore (%s) stringwidth rmoveto\n", str);

  }while(*cur_str);

  fprintf(OF, "grestore\n");

  /*
   * keep driver happy -- needed for background and orientation.
   * arghhh! can't calculate it, as I only have the string reference
   * point, not its extent!
   * Quick (and final?) *hack*, ASSUME that no more than two char height
   * extents after/before the string reference point.
   */

  dev->llx = MIN(dev->llx, args->x - 2. * font_factor * ft_ht * 25.4 / 72. * pls->xpmm);
  dev->lly = MIN(dev->lly, args->y - 2. * font_factor * ft_ht * 25.4 / 72. * pls->ypmm);
  dev->urx = MAX(dev->urx, args->x + 2. * font_factor * ft_ht * 25.4 / 72. * pls->xpmm);
  dev->ury = MAX(dev->ury, args->y + 2. * font_factor * ft_ht * 25.4 / 72. * pls->ypmm);
}

static void
esc_purge(char *dstr, const char *sstr) {
  char esc;

  plgesc(&esc);

  while(*sstr){
    if (*sstr != esc) {
      *dstr++ = *sstr++;
      continue;
    }

    sstr++;
    if (*sstr == esc)
      continue;
    else {
      switch(*sstr++) {
      case 'g': 
      case 'f':
	sstr++;
	break; /* two chars sequence */

      case '(':
	while (*sstr++ != ')'); /* multi chars sequence */
	break;

      default:
	break;  /* single char escape */
      }
    }
  }
  *dstr = '\0';
}

#else
int 
pldummy_ps()
{
    return 0;
}

#endif				/* PLD_ps */
