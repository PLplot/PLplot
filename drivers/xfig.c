/* $Id$

	PLplot xfig device driver.
*/
#include "plplot/plDevs.h"

#ifdef PLD_xfig

#include "plplot/plplotP.h"
#include "plplot/drivers.h"

/* Function prototypes */

static void flushbuffer(PLStream *);

/* top level declarations */

#define FIGX	599
#define FIGY	599
#define DPI	80
#define BSIZE	25
#define XFIG_COLBASE 33 /* xfig first user color */

static short *buffptr, bufflen;
static short count;
static int curwid = 1;
static int curcol = 1;
static int firstline = 1;
static long cmap0_pos, cmap1_pos;
static int cmap0_ncol, cmap1_ncol;

static void stcmap0(PLStream *);
static void stcmap1(PLStream *);

/*--------------------------------------------------------------------------*\
 * plD_init_xfig()
 *
 * Initialize device.
\*--------------------------------------------------------------------------*/

void
plD_init_xfig(PLStream *pls)
{
    PLDev *dev;
    int i;

/* Initialize family file info */

    plFamInit(pls);

/* Prompt for a file name if not already set */

    plOpenFile(pls);

/* Allocate and initialize device-specific data */

    dev = plAllocDev(pls);

    dev->xold = PL_UNDEFINED;
    dev->yold = PL_UNDEFINED;
    dev->xmin = 0;
    dev->xmax = FIGX;
    dev->ymin = 0;
    dev->ymax = FIGY;
    pls->dev_fill0 = 1;	    /* Handle solid fills */
    if (!pls->colorset)
      pls->color = 1;         /* Is a color device */

    plP_setpxl(3.1496, 3.1496);	/* 80 DPI */

    plP_setphy(0, FIGX, 0, FIGY);

/* Write out header */

    /* still much work to do */
    fprintf(pls->OutFile, "#FIG 3.2\n");
    fprintf(pls->OutFile, "Landscape\n");
    fprintf(pls->OutFile, "Center\n");
    fprintf(pls->OutFile, "Metric\n");
    fprintf(pls->OutFile, "A4\n");
    fprintf(pls->OutFile, "100.0\n");
    fprintf(pls->OutFile, "Single\n");
    fprintf(pls->OutFile, "-2\n");
    fprintf(pls->OutFile, "%d 2\n", DPI);

    /* user defined colors, for colormap0 */
    cmap0_ncol = pls->ncol0;
    cmap0_pos = ftell(pls->OutFile);
    stcmap0(pls);

    /* user defined colors, for colormap1 */
    cmap1_pos = ftell(pls->OutFile);
    cmap1_ncol = pls->ncol1;
    stcmap1(pls);

    bufflen = 2 * BSIZE;
    buffptr = (short *) malloc(sizeof(short) * bufflen);
    if (buffptr == NULL)
	plexit("Out of memory!");
}

void
stcmap0(PLStream *pls)
{
  long cur_pos;
  int i;

  if (pls->ncol0 > cmap0_ncol)
    fprintf(stderr,"Too much colors for cmap0. Preallocate using command line '-ncol0 n.\n'");

      cur_pos = ftell(pls->OutFile);

      if (fseek(pls->OutFile, cmap0_pos, SEEK_SET)) {
	fprintf(stderr,"Sorry, only file based output, no pipes.\n");
	exit(0);
      }

      for (i=0; i<pls->ncol0; i++)
	fprintf(pls->OutFile,"0 %d #%.2x%.2x%.2x\n", i+XFIG_COLBASE,
		pls->cmap0[i].r, pls->cmap0[i].g, pls->cmap0[i].b);

      if (cur_pos != cmap0_pos)
	fseek(pls->OutFile, cur_pos, SEEK_SET);
}

void
stcmap1(PLStream *pls)
{
  long cur_pos;
  int i;

  if (pls->ncol1 > cmap1_ncol)
    fprintf(stderr,"Too much colors for cmap1. Preallocate using command line '-ncol1 n.\n'");

      cur_pos = ftell(pls->OutFile);

      if (fseek(pls->OutFile, cmap1_pos, SEEK_SET)) {
	fprintf(stderr,"Sorry, only file based output, no pipes.\n");
	exit(0);
      }

      for (i=0; i<pls->ncol1; i++)
	fprintf(pls->OutFile,"0 %d #%.2x%.2x%.2x\n", i+XFIG_COLBASE+cmap0_ncol,
		pls->cmap1[i].r, pls->cmap1[i].g, pls->cmap1[i].b);

      if (cur_pos != cmap1_pos)
	fseek(pls->OutFile, cur_pos, SEEK_SET);
}

/*--------------------------------------------------------------------------*\
 * plD_line_xfig()
 *
 * Draw a line in the current color from (x1,y1) to (x2,y2).
\*--------------------------------------------------------------------------*/

void
plD_line_xfig(PLStream *pls, short x1a, short y1a, short x2a, short y2a)
{
    PLDev *dev = (PLDev *) pls->dev;
    int x1 = x1a, y1 = y1a, x2 = x2a, y2 = y2a;
    short *tempptr;

/* If starting point of this line is the same as the ending point of */
/* the previous line then don't raise the pen. (This really speeds up */
/* plotting and reduces the size of the file. */

    if (firstline) {
	count = 0;
	*(buffptr + count++) = x1;
	*(buffptr + count++) = y1;
	*(buffptr + count++) = x2;
	*(buffptr + count++) = y2;
	firstline = 0;
    }
    else if (x1 == dev->xold && y1 == dev->yold) {
	if (count + 2 >= bufflen) {
	    bufflen += 2 * BSIZE;
	    tempptr = (short *)
		realloc((void *) buffptr, bufflen * sizeof(short));
	    if (tempptr == NULL) {
		free((void *) buffptr);
		plexit("Out of memory!");
	    }
	    buffptr = tempptr;
	}
	*(buffptr + count++) = x2;
	*(buffptr + count++) = y2;
    }
    else {
	flushbuffer(pls);
	*(buffptr + count++) = x1;
	*(buffptr + count++) = y1;
	*(buffptr + count++) = x2;
	*(buffptr + count++) = y2;
    }
    dev->xold = x2;
    dev->yold = y2;
}

/*--------------------------------------------------------------------------*\
 * plD_polyline_xfig()
 *
 * Draw a polyline in the current color.
\*--------------------------------------------------------------------------*/

void
plD_polyline_xfig(PLStream *pls, short *xa, short *ya, PLINT npts)
{
    PLINT i;

    for (i = 0; i < npts - 1; i++)
	plD_line_xfig(pls, xa[i], ya[i], xa[i + 1], ya[i + 1]);
}

/*--------------------------------------------------------------------------*\
 * plD_eop_xfig()
 *
 * End of page.
\*--------------------------------------------------------------------------*/

void
plD_eop_xfig(PLStream *pls)
{
    if (!firstline)
	flushbuffer(pls);
}

/*--------------------------------------------------------------------------*\
 * plD_bop_xfig()
 *
 * Set up for the next page.
 * Advance to next family file if necessary (file output).
\*--------------------------------------------------------------------------*/

void
plD_bop_xfig(PLStream *pls)
{
    PLDev *dev = (PLDev *) pls->dev;

    dev->xold = PL_UNDEFINED;
    dev->yold = PL_UNDEFINED;
    firstline = 1;

    if (!pls->termin)
      plGetFam(pls);

    pls->famadv = 1;
    pls->page++;
}

/*--------------------------------------------------------------------------*\
 * plD_tidy_xfig()
 *
 * Close graphics file or otherwise clean up.
\*--------------------------------------------------------------------------*/

void
plD_tidy_xfig(PLStream *pls)
{
    flushbuffer(pls);
    free((void *) buffptr);
    fclose(pls->OutFile);
}

/*--------------------------------------------------------------------------*\
 * plD_state_xfig()
 *
 * Handle change in PLStream state (color, pen width, fill attribute, etc).
\*--------------------------------------------------------------------------*/

void 
plD_state_xfig(PLStream *pls, PLINT op)
{
  switch (op) {

  case PLSTATE_WIDTH:
    flushbuffer(pls);
    firstline = 1;
    curwid = pls->width;
    break;

  case PLSTATE_COLOR0:
    flushbuffer(pls);
    curcol =  pls->icol0 + XFIG_COLBASE;
    break;

  case PLSTATE_COLOR1:
    flushbuffer(pls);
    curcol =  pls->icol1 + XFIG_COLBASE + pls->ncol0;
    break;

  case PLSTATE_CMAP0:
    stcmap0(pls);
    break;

  case PLSTATE_CMAP1:
    stcmap1(pls);
    break;

  default:
    fprintf(stderr, "state opt=%d\n", op);
  }

}

/*--------------------------------------------------------------------------*\
 * plD_esc_xfig()
 *
 * Escape function.
 * Preliminary fill support for colormap0
\*--------------------------------------------------------------------------*/

void
plD_esc_xfig(PLStream *pls, PLINT op, void *ptr)
{
  PLDev *dev = pls->dev;
  int i, npts;
 
  switch (op) { 
  case PLESC_FILL:

    npts = pls->dev_npts;
    if (npts > PL_MAXPOLY)
      plexit("FillPolygonCmd: Too many points in polygon\n");
     
    flushbuffer(pls);
    fprintf(pls->OutFile, "2 1 0 1 %d %d 50 0 20 0.0 0 0 0 0 0 %d\n",
	    curcol, curcol, npts);

    for (i = 0; i < npts; i++)
      fprintf(pls->OutFile,"%d %d ",  pls->dev_x[i],
	      dev->ymax - pls->dev_y[i]);

    fprintf(pls->OutFile, "\n");
    break;

  default:
    fprintf(stderr, "esc opt=%d\n", op);
  }  
}

/*--------------------------------------------------------------------------*\
 * Utility functions.
\*--------------------------------------------------------------------------*/

static void
flushbuffer(PLStream *pls)
{
    short i = 0;

    if (count == 0)
	return;

    fprintf(pls->OutFile, "2 1 0 %d %d 0 50 0 -1 0.0 0 0 0 0 0 %d\n",
	    curwid, curcol, count/2);
    while (i < count) {
	fprintf(pls->OutFile, "%d %d ", *(buffptr + i),
		FIGY - *(buffptr + i + 1));
	i += 2;
    }
    fprintf(pls->OutFile, "\n");
    count = 0;
}

#else
int 
pldummy_xfig()
{
    return 0;
}

#endif				/* PLD_xfig */
