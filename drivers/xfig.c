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

static short *buffptr, bufflen;
static short count;
static int curwid = 1;
static int curcol0 = 0;
static int firstline = 1;

/* transformation matrix from plplot *default* colormap0 to xfig colors */
static int trans[] = {0, 4, 6, 2, 33, 34, 35, 36, 37, 1, 38, 3, 39, 5, 40, 7};

/*--------------------------------------------------------------------------*\
 * plD_init_xfig()
 *
 * Initialize device.
\*--------------------------------------------------------------------------*/

void
plD_init_xfig(PLStream *pls)
{
    PLDev *dev;

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
    pls->dev_fill0 = 1;		/* Handle solid fills */

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

    /* user defined colors, for colormap0--xfig only has 8 default colors */
    fprintf(pls->OutFile, "0 33 #7fffd4\n");
    fprintf(pls->OutFile, "0 34 #ffc0cb\n");
    fprintf(pls->OutFile, "0 35 #f5deb3\n");
    fprintf(pls->OutFile, "0 36 #bebebe\n");
    fprintf(pls->OutFile, "0 37 #a52a2a\n");
    fprintf(pls->OutFile, "0 38 #8a2be2\n");
    fprintf(pls->OutFile, "0 39 #40e0d0\n");
    fprintf(pls->OutFile, "0 40 #fa8072\n");
    
    bufflen = 2 * BSIZE;
    buffptr = (short *) malloc(sizeof(short) * bufflen);
    if (buffptr == NULL)
	plexit("Out of memory!");
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

	if (pls->width <= 1)
	    curwid = 1;
	else if (pls->width >= 4)
	    curwid = 3;
	else
	    curwid = (int) pls->width;

	break;

    case PLSTATE_COLOR0:
      flushbuffer(pls);
      curcol0 =  trans[pls->icol0];
      break;

    case PLSTATE_COLOR1:
      fprintf(stderr, "Yeah\n");
      break;
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
	     curcol0, curcol0, npts);

     for (i = 0; i < npts; i++)
       fprintf(pls->OutFile,"%d %d ",  pls->dev_x[i],
	        dev->ymax - pls->dev_y[i]);

     fprintf(pls->OutFile, "\n");
     break;
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
	    curwid, curcol0, count/2);
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
