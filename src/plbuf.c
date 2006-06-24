/* $Id$

    Handle plot buffer.

    Copyright (C) 1992  Maurice LeBrun
    Copyright (C) 2004  Alan W. Irwin
    Copyright (C) 2005  Thomas J. Duck
    Copyright (C) 2006  Jim Dishaw

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
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
*/

#define NEED_PLDEBUG
#include "plplotP.h"
#include "drivers.h"
#include "metadefs.h"

#include <string.h>

/* Function prototypes */
void * plbuf_save(PLStream *pls, void *state);

static int	rd_command	(PLStream *pls, U_CHAR *p_c);
static void	rd_data		(PLStream *pls, void *buf, size_t buf_size);
static void	wr_command	(PLStream *pls, U_CHAR c);
static void	wr_data		(PLStream *pls, void *buf, size_t buf_size);
static void	plbuf_control	(PLStream *pls, U_CHAR c);

static void	rdbuf_init	(PLStream *pls);
static void	rdbuf_line	(PLStream *pls);
static void	rdbuf_polyline	(PLStream *pls);
static void	rdbuf_eop	(PLStream *pls);
static void	rdbuf_bop	(PLStream *pls);
static void	rdbuf_state	(PLStream *pls);
static void	rdbuf_esc	(PLStream *pls);

static void	plbuf_fill	(PLStream *pls);
static void	rdbuf_fill	(PLStream *pls);
static void	plbuf_swin	(PLStream *pls, PLWindow *plwin);
static void	rdbuf_swin	(PLStream *pls);

/*--------------------------------------------------------------------------*\
 * plbuf_init()
 *
 * Initialize device.
 * Actually just disables writes if plot buffer is already open (occurs
 * when one stream is cloned, as in printing).
\*--------------------------------------------------------------------------*/

void
plbuf_init(PLStream *pls)
{
    dbug_enter("plbuf_init");

    pls->plbuf_read = FALSE;
#ifdef BUFFERED_FILE
    if (pls->plbufFile != NULL)
        pls->plbuf_write = FALSE;
#else
    if (pls->plbuf_buffer != NULL)
        pls->plbuf_write = FALSE;
#endif
}

/*--------------------------------------------------------------------------*\
 * plbuf_line()
 *
 * Draw a line in the current color from (x1,y1) to (x2,y2).
\*--------------------------------------------------------------------------*/

void
plbuf_line(PLStream *pls, short x1a, short y1a, short x2a, short y2a)
{
    short xpl[2], ypl[2];

    dbug_enter("plbuf_line");

    wr_command(pls, (U_CHAR) LINE);

    xpl[0] = x1a;
    xpl[1] = x2a;
    ypl[0] = y1a;
    ypl[1] = y2a;

    wr_data(pls, xpl, sizeof(short) * 2);
    wr_data(pls, ypl, sizeof(short) * 2);
}

/*--------------------------------------------------------------------------*\
 * plbuf_polyline()
 *
 * Draw a polyline in the current color.
\*--------------------------------------------------------------------------*/

void
plbuf_polyline(PLStream *pls, short *xa, short *ya, PLINT npts)
{
    dbug_enter("plbuf_polyline");

    wr_command(pls, (U_CHAR) POLYLINE);

    wr_data(pls, &npts, sizeof(PLINT));

    wr_data(pls, xa, sizeof(short) * npts);
    wr_data(pls, ya, sizeof(short) * npts);

}

/*--------------------------------------------------------------------------*\
 * plbuf_eop()
 *
 * End of page.
\*--------------------------------------------------------------------------*/

void
plbuf_eop(PLStream *pls)
{
    dbug_enter("plbuf_eop");
}

/*--------------------------------------------------------------------------*\
 * plbuf_bop()
 *
 * Set up for the next page.
 * To avoid problems redisplaying partially filled pages, on each BOP the
 * old file is thrown away and a new one is obtained.  This way we can just
 * read up to EOF to get everything on the current page.
 *
 * Also write state information to ensure the next page is correct.
\*--------------------------------------------------------------------------*/

void
plbuf_bop(PLStream *pls)
{
    dbug_enter("plbuf_bop");

    plbuf_tidy(pls);

#ifdef BUFFERED_FILE
    pls->plbufFile = tmpfile();
    if (pls->plbufFile == NULL)
	plexit("plbuf_bop: Error opening plot data storage file.");
#else
// Need a better place to initialize this value
    pls->plbuf_buffer_grow = 128 * 1024;

    if (pls->plbuf_buffer == NULL) {
    // We have not allocated a buffer, so do it now
        if ((pls->plbuf_buffer = malloc(pls->plbuf_buffer_grow)) == NULL)
            plexit("plbuf_bop: Error allocating plot buffer.");

        pls->plbuf_buffer_size = pls->plbuf_buffer_grow;
        pls->plbuf_top = 0;
        pls->plbuf_readpos = 0;
    } else {
    // Buffer is allocated, move the top to the beginning
        pls->plbuf_top = 0;
    }
#endif

    wr_command(pls, (U_CHAR) BOP);
    plbuf_state(pls, PLSTATE_COLOR0);
    plbuf_state(pls, PLSTATE_WIDTH);
}

/*--------------------------------------------------------------------------*\
 * plbuf_tidy()
 *
 * Close graphics file
\*--------------------------------------------------------------------------*/

void
plbuf_tidy(PLStream *pls)
{
    dbug_enter("plbuf_tidy");

#ifdef BUFFERED_FILE
    if (pls->plbufFile == NULL)
	return;

    fclose(pls->plbufFile)
    pls->plbufFile = NULL;
#endif
}

/*--------------------------------------------------------------------------*\
 * plbuf_state()
 *
 * Handle change in PLStream state (color, pen width, fill attribute, etc).
\*--------------------------------------------------------------------------*/

void
plbuf_state(PLStream *pls, PLINT op)
{
    dbug_enter("plbuf_state");

    wr_command(pls, (U_CHAR) CHANGE_STATE);
    wr_command(pls, (U_CHAR) op);

    switch (op) {

    case PLSTATE_WIDTH: 
		wr_data(pls, &(pls->width), sizeof(pls->width));
		break;

    case PLSTATE_COLOR0:
		wr_data(pls, &(pls->icol0), sizeof(pls->icol0));
		if (pls->icol0 == PL_RGB_COLOR) {
			wr_data(pls, &(pls->curcolor.r), sizeof(pls->curcolor.r));
			wr_data(pls, &(pls->curcolor.g), sizeof(pls->curcolor.g));
			wr_data(pls, &(pls->curcolor.b), sizeof(pls->curcolor.b));
		}
		break;
    
    case PLSTATE_COLOR1:
		wr_data(pls, &(pls->icol1), sizeof(pls->icol1));
		break;
    
    case PLSTATE_FILL:
		wr_data(pls, &(pls->patt), sizeof(pls->patt));
		break;
    }
}


/*--------------------------------------------------------------------------*\
 * plbuf_image()
 *
 * write image described in points pls->dev_x[], pls->dev_y[], pls->dev_z[].
 *                      pls->nptsX, pls->nptsY.
\*--------------------------------------------------------------------------*/

static void
plbuf_image(PLStream *pls, IMG_DT *img_dt)
{
    PLINT npts = pls->dev_nptsX * pls->dev_nptsY;

    dbug_enter("plbuf_image");

    wr_data(pls, &pls->dev_nptsX, sizeof(PLINT));
    wr_data(pls, &pls->dev_nptsY, sizeof(PLINT));

    wr_data(pls, &img_dt->xmin, sizeof(PLFLT));
    wr_data(pls, &img_dt->ymin, sizeof(PLFLT));
    wr_data(pls, &img_dt->dx, sizeof(PLFLT));
    wr_data(pls, &img_dt->dy, sizeof(PLFLT));

    wr_data(pls, &pls->dev_zmin, sizeof(short));
    wr_data(pls, &pls->dev_zmax, sizeof(short));

    wr_data(pls, pls->dev_ix, sizeof(short) * npts);
    wr_data(pls, pls->dev_iy, sizeof(short) * npts);
    wr_data(pls, pls->dev_z, sizeof(unsigned short) * (pls->dev_nptsX-1)*(pls->dev_nptsY-1));
}

/*--------------------------------------------------------------------------*\
 * plbuf_text()
 *
 * Handle text call.
\*--------------------------------------------------------------------------*/

static void
plbuf_text(PLStream *pls, EscText *text)
{
  PLUNICODE fci;

  dbug_enter("plbuf_text");
  
  /* Retrieve the font characterization integer */
  plgfci(&fci);

  /* Write the text information */

  wr_data(pls, &fci, sizeof(PLUNICODE));

  wr_data(pls, &pls->chrht, sizeof(PLFLT));
  wr_data(pls, &pls->diorot, sizeof(PLFLT));
  wr_data(pls, &pls->clpxmi, sizeof(PLFLT));
  wr_data(pls, &pls->clpxma, sizeof(PLFLT));
  wr_data(pls, &pls->clpymi, sizeof(PLFLT));
  wr_data(pls, &pls->clpyma, sizeof(PLFLT));
    
  wr_data(pls, &text->base, sizeof(PLINT));
  wr_data(pls, &text->just, sizeof(PLFLT));
  wr_data(pls, text->xform, sizeof(PLFLT) * 4);
  wr_data(pls, &text->x, sizeof(PLINT));
  wr_data(pls, &text->y, sizeof(PLINT));
  wr_data(pls, &text->refx, sizeof(PLINT));
  wr_data(pls, &text->refy, sizeof(PLINT));

  wr_data(pls, &text->unicode_array_len, sizeof(PLINT));
  if(text->unicode_array_len)
    wr_data(pls, text->unicode_array, sizeof(PLUNICODE) * text->unicode_array_len);
}

/*--------------------------------------------------------------------------*\
 * plbuf_esc()
 *
 * Escape function.  Note that any data written must be in device
 * independent form to maintain the transportability of the metafile.
 *
 * Functions:
 *
 *	PLESC_FILL	Fill polygon
 *	PLESC_SWIN	Set plot window parameters
 *      PLESC_IMAGE     Draw image
 *      PLESC_HAS_TEXT  Draw PostScript text
\*--------------------------------------------------------------------------*/

void
plbuf_esc(PLStream *pls, PLINT op, void *ptr)
{
    dbug_enter("plbuf_esc");

    wr_command(pls, (U_CHAR) ESCAPE);
    wr_command(pls, (U_CHAR) op);

    switch (op) {
    case PLESC_FILL:
	plbuf_fill(pls);
	break;
    case PLESC_SWIN:
	plbuf_swin(pls, (PLWindow *) ptr);
	break;
    case PLESC_IMAGE:
	plbuf_image(pls, (IMG_DT *) ptr);
	break;
    case PLESC_HAS_TEXT:
      if(ptr!=NULL) /* Check required by GCW driver, please don't remove */
	  plbuf_text(pls, (EscText *) ptr);
	break;
    }
}

/*--------------------------------------------------------------------------*\
 * plbuf_fill()
 *
 * Fill polygon described in points pls->dev_x[] and pls->dev_y[].
\*--------------------------------------------------------------------------*/

static void
plbuf_fill(PLStream *pls)
{
    dbug_enter("plbuf_fill");

    wr_data(pls, &pls->dev_npts, sizeof(PLINT));
    wr_data(pls, pls->dev_x, sizeof(short) * pls->dev_npts);
    wr_data(pls, pls->dev_y, sizeof(short) * pls->dev_npts);
}

/*--------------------------------------------------------------------------*\
 * plbuf_swin()
 *
 * Set up plot window parameters.
\*--------------------------------------------------------------------------*/

static void
plbuf_swin(PLStream *pls, PLWindow *plwin)
{
    wr_data(pls, &plwin->dxmi, sizeof(PLFLT));
    wr_data(pls, &plwin->dxma, sizeof(PLFLT));
    wr_data(pls, &plwin->dymi, sizeof(PLFLT));
    wr_data(pls, &plwin->dyma, sizeof(PLFLT));

    wr_data(pls, &plwin->wxmi, sizeof(PLFLT));
    wr_data(pls, &plwin->wxma, sizeof(PLFLT));
    wr_data(pls, &plwin->wymi, sizeof(PLFLT));
    wr_data(pls, &plwin->wyma, sizeof(PLFLT));
}

/*--------------------------------------------------------------------------*\
 * Routines to read from & process the plot buffer.
\*--------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------*\
 * rdbuf_init()
 *
 * Initialize device.
\*--------------------------------------------------------------------------*/

static void
rdbuf_init(PLStream *pls)
{
    dbug_enter("rdbuf_init");
}

/*--------------------------------------------------------------------------*\
 * rdbuf_line()
 *
 * Draw a line in the current color from (x1,y1) to (x2,y2).
\*--------------------------------------------------------------------------*/

static void
rdbuf_line(PLStream *pls)
{
    short xpl[2], ypl[2];
    PLINT npts = 2;

    dbug_enter("rdbuf_line");

    rd_data(pls, xpl, sizeof(short) * npts);
    rd_data(pls, ypl, sizeof(short) * npts);

    plP_line(xpl, ypl);
}

/*--------------------------------------------------------------------------*\
 * rdbuf_polyline()
 *
 * Draw a polyline in the current color.
\*--------------------------------------------------------------------------*/

static void
rdbuf_polyline(PLStream *pls)
{
    short xpl[PL_MAXPOLY], ypl[PL_MAXPOLY];
    PLINT npts;

    dbug_enter("rdbuf_polyline");

    rd_data(pls, &npts, sizeof(PLINT));
    rd_data(pls, xpl, sizeof(short) * npts);
    rd_data(pls, ypl, sizeof(short) * npts);

    plP_polyline(xpl, ypl, npts);
}

/*--------------------------------------------------------------------------*\
 * rdbuf_eop()
 *
 * End of page.
\*--------------------------------------------------------------------------*/

static void
rdbuf_eop(PLStream *pls)
{
    dbug_enter("rdbuf_eop");
}

/*--------------------------------------------------------------------------*\
 * rdbuf_bop()
 *
 * Set up for the next page.
\*--------------------------------------------------------------------------*/

static void
rdbuf_bop(PLStream *pls)
{
    dbug_enter("rdbuf_bop");

    pls->nplwin = 0;
}

/*--------------------------------------------------------------------------*\
 * rdbuf_state()
 *
 * Handle change in PLStream state (color, pen width, fill attribute, etc).
\*--------------------------------------------------------------------------*/

static void
rdbuf_state(PLStream *pls)
{
    U_CHAR op;

    dbug_enter("rdbuf_state");

    rd_data(pls, &op, sizeof(U_CHAR));

    switch (op) {

    case PLSTATE_WIDTH:{
	U_CHAR width;

	rd_data(pls, &width, sizeof(U_CHAR));
	pls->width = width;
	plP_state(PLSTATE_WIDTH);

	break;
    }

    case PLSTATE_COLOR0:{
	short icol0;
	U_CHAR r, g, b;

	rd_data(pls, &icol0, sizeof(short));
	if (icol0 == PL_RGB_COLOR) {
	    rd_data(pls, &r, sizeof(U_CHAR));
	    rd_data(pls, &g, sizeof(U_CHAR));
	    rd_data(pls, &b, sizeof(U_CHAR));
	}
	else {
	    if ((int) icol0 >= pls->ncol0) {
                char buffer[256];
                sprintf(buffer, "rdbuf_state: Invalid color map entry: %d", (int) icol0);
                plabort(buffer);
                return;
	    }
	    r = pls->cmap0[icol0].r;
	    g = pls->cmap0[icol0].g;
	    b = pls->cmap0[icol0].b;
	}
	pls->icol0 = icol0;
	pls->curcolor.r = r;
	pls->curcolor.g = g;
	pls->curcolor.b = b;

	plP_state(PLSTATE_COLOR0);
	break;
    }

    case PLSTATE_COLOR1: {
	short icol1;

	rd_data(pls, &icol1, sizeof(short));

	pls->icol1 = icol1;
	pls->curcolor.r = pls->cmap1[icol1].r;
	pls->curcolor.g = pls->cmap1[icol1].g;
	pls->curcolor.b = pls->cmap1[icol1].b;

	plP_state(PLSTATE_COLOR1);
	break;
    }

    case PLSTATE_FILL: {
	signed char patt;

	rd_data(pls, &patt, sizeof(signed char));

	pls->patt = patt;
	plP_state(PLSTATE_FILL);
	break;
    }
    }
}

/*--------------------------------------------------------------------------*\
 * rdbuf_esc()
 *
 * Escape function.
 * Must fill data structure with whatever data that was written,
 * then call escape function.
 *
 * Note: it is best to only call the escape function for op-codes that
 * are known to be supported.
 *
 * Functions:
 *
 *	PLESC_FILL	Fill polygon
 *	PLESC_SWIN	Set plot window parameters
 *      PLESC_IMAGE     Draw image
 *      PLESC_HAS_TEXT  Draw PostScript text
\*--------------------------------------------------------------------------*/

static void
rdbuf_image(PLStream *pls);

static void
rdbuf_text(PLStream *pls);

static void
rdbuf_esc(PLStream *pls)
{
    U_CHAR op;

    dbug_enter("rdbuf_esc");

    rd_data(pls, &op, sizeof(U_CHAR));

    switch (op) {
    case PLESC_FILL:
	rdbuf_fill(pls);
	break;
    case PLESC_SWIN:
	rdbuf_swin(pls);
	break;
    case PLESC_IMAGE:
	rdbuf_image(pls);
	break;
    case PLESC_HAS_TEXT:
        rdbuf_text(pls);
	break;
    }
}

/*--------------------------------------------------------------------------*\
 * rdbuf_fill()
 *
 * Fill polygon described by input points.
\*--------------------------------------------------------------------------*/

static void
rdbuf_fill(PLStream *pls)
{
    short xpl[PL_MAXPOLY], ypl[PL_MAXPOLY];
    PLINT npts;

    dbug_enter("rdbuf_fill");

    rd_data(pls, &npts, sizeof(PLINT));
    rd_data(pls, xpl, sizeof(short) * npts);
    rd_data(pls, ypl, sizeof(short) * npts);

    plP_fill(xpl, ypl, npts);
}

/*--------------------------------------------------------------------------*\
 * rdbuf_image()
 *
 * .
\*--------------------------------------------------------------------------*/

static void
rdbuf_image(PLStream *pls)
{
    short *dev_ix, *dev_iy;
    unsigned short *dev_z, dev_zmin, dev_zmax;
    PLINT nptsX,nptsY, npts;
    PLFLT xmin, ymin, dx, dy;

    dbug_enter("rdbuf_image");

    rd_data(pls, &nptsX, sizeof(PLINT));
    rd_data(pls, &nptsY, sizeof(PLINT));
    npts = nptsX*nptsY;

    rd_data(pls, &xmin, sizeof(PLFLT));
    rd_data(pls, &ymin, sizeof(PLFLT));
    rd_data(pls, &dx, sizeof(PLFLT));
    rd_data(pls, &dy, sizeof(PLFLT));

    rd_data(pls, &dev_zmin, sizeof(short));
    rd_data(pls, &dev_zmax, sizeof(short));

	/* NOTE:  Even though for memory buffered version all the data is in memory,
	 * we still allocate and copy the data because I think that method works
	 * better in a multithreaded environment.  I could be wrong.
	 */
    dev_ix=(short *)malloc(npts*sizeof(short));
    dev_iy=(short *)malloc(npts*sizeof(short));
    dev_z=(unsigned short *)malloc((nptsX-1)*(nptsY-1)*sizeof(unsigned short));

    rd_data(pls, dev_ix, sizeof(short) * npts);
    rd_data(pls, dev_iy, sizeof(short) * npts);
    rd_data(pls, dev_z, sizeof(unsigned short) * (nptsX-1)*(nptsY-1));

    plP_image(dev_ix, dev_iy, dev_z, nptsX, nptsY, xmin, ymin, dx, dy, dev_zmin, dev_zmax);

    free(dev_ix);
    free(dev_iy);
    free(dev_z);
}

/*--------------------------------------------------------------------------*\
 * rdbuf_swin()
 *
 * Set up plot window parameters.
\*--------------------------------------------------------------------------*/

static void
rdbuf_swin(PLStream *pls)
{
    PLWindow plwin;

    rd_data(pls, &plwin.dxmi, sizeof(PLFLT));
    rd_data(pls, &plwin.dxma, sizeof(PLFLT));
    rd_data(pls, &plwin.dymi, sizeof(PLFLT));
    rd_data(pls, &plwin.dyma, sizeof(PLFLT));

    rd_data(pls, &plwin.wxmi, sizeof(PLFLT));
    rd_data(pls, &plwin.wxma, sizeof(PLFLT));
    rd_data(pls, &plwin.wymi, sizeof(PLFLT));
    rd_data(pls, &plwin.wyma, sizeof(PLFLT));

    plP_swin(&plwin);
}

/*--------------------------------------------------------------------------*\
 * rdbuf_text()
 *
 * Draw PostScript text.
\*--------------------------------------------------------------------------*/

static void
rdbuf_text(PLStream *pls)
{
  PLUNICODE(fci);
  EscText text;
  PLFLT xform[4];
  PLUNICODE* unicode;

  text.xform = xform;


  /* Read in the data */

  rd_data(pls, &fci, sizeof(PLUNICODE));

  rd_data(pls, &pls->chrht, sizeof(PLFLT));
  rd_data(pls, &pls->diorot, sizeof(PLFLT));
  rd_data(pls, &pls->clpxmi, sizeof(PLFLT));
  rd_data(pls, &pls->clpxma, sizeof(PLFLT));
  rd_data(pls, &pls->clpymi, sizeof(PLFLT));
  rd_data(pls, &pls->clpyma, sizeof(PLFLT));

  rd_data(pls, &text.base, sizeof(PLINT));
  rd_data(pls, &text.just, sizeof(PLFLT));
  rd_data(pls, text.xform, sizeof(PLFLT) * 4);
  rd_data(pls, &text.x, sizeof(PLINT));
  rd_data(pls, &text.y, sizeof(PLINT));
  rd_data(pls, &text.refx, sizeof(PLINT));
  rd_data(pls, &text.refy, sizeof(PLINT));

  rd_data(pls, &text.unicode_array_len, sizeof(PLINT));
  if(text.unicode_array_len) {
    if((unicode=(PLUNICODE *)malloc(text.unicode_array_len*sizeof(PLUNICODE)))
       == NULL)
      plexit("rdbuf_text: Insufficient memory");

    rd_data(pls, unicode, sizeof(PLUNICODE) * text.unicode_array_len);
    text.unicode_array = unicode;
  }
  else text.unicode_array = NULL;

  /* Make the call for unicode devices */
  if(pls->dev_unicode) {
    plsfci(fci);
    plP_esc(PLESC_HAS_TEXT,&text);
  }
}

/*--------------------------------------------------------------------------*\
 * plRemakePlot()
 *
 * Rebuilds plot from plot buffer, usually in response to a window
 * resize or exposure event.
\*--------------------------------------------------------------------------*/

void
plRemakePlot(PLStream *pls)
{
    U_CHAR c;
    int plbuf_status;
	PLStream *save_pls;

    dbug_enter("plRemakePlot");

    /* Change the status of the flags before checking for a buffer.
     * Actually, more thought is needed if we want to support multithreaded
     * code correctly, specifically the case where two threads are using
     * the same plot stream (e.g. one thread is drawing the plot and another
     * thread is processing window manager messages).
     */
    plbuf_status = pls->plbuf_write;
    pls->plbuf_write = FALSE;
    pls->plbuf_read = TRUE;

#ifdef BUFFERED_FILE
    if (pls->plbufFile) {
        rewind(pls->plbufFile);
#else
    if (pls->plbuf_buffer) {
        pls->plbuf_readpos = 0;
#endif
        /* Need to change where plsc points to before processing the commands.
         * If we have multiple plot streams, this will prevent the commands from
         * going to the wrong plot stream.
         */
        save_pls = plsc;
        plsc = pls;

        while (rd_command(pls, &c)) {
            plbuf_control(pls, c);
        }

        plsc = save_pls;
    }

    pls->plbuf_read = FALSE;
    pls->plbuf_write = plbuf_status;
}

/*--------------------------------------------------------------------------*\
 * plbuf_control()
 *
 * Processes commands read from the plot buffer.
\*--------------------------------------------------------------------------*/

static void
plbuf_control(PLStream *pls, U_CHAR c)
{
    static U_CHAR c_old = 0;

    dbug_enter("plbuf_control");

    switch ((int) c) {

    case INITIALIZE:
	rdbuf_init(pls);
	break;

    case EOP:
	rdbuf_eop(pls);
	break;

    case BOP:
	rdbuf_bop(pls);
	break;

    case CHANGE_STATE:
	rdbuf_state(pls);
	break;

    case LINE:
	rdbuf_line(pls);
	break;

    case POLYLINE:
	rdbuf_polyline(pls);
	break;

    case ESCAPE:
	rdbuf_esc(pls);
	break;

    default:
      pldebug("plbuf_control", "Unrecognized command %d, previous %d\n", c, c_old);
    }
    c_old = c;
}

/*--------------------------------------------------------------------------*\
 * rd_command()
 *
 * Read & return the next command
\*--------------------------------------------------------------------------*/

static int
rd_command(PLStream *pls, U_CHAR *p_c)
{
    int count;

#ifdef BUFFERED_FILE
    count = fread(p_c, sizeof(U_CHAR), 1, pls->plbufFile);
#else
    if (pls->plbuf_readpos < pls->plbuf_top) {
        *p_c = *(U_CHAR *)((U_CHAR *)pls->plbuf_buffer + pls->plbuf_readpos);
        pls->plbuf_readpos += sizeof(U_CHAR);
        count = sizeof(U_CHAR);
    } else {
        count = 0;
    }
#endif
    return(count);
}

/*--------------------------------------------------------------------------*\
 * rd_data()
 *
 * Read the data associated with the command
\*--------------------------------------------------------------------------*/

static void
rd_data(PLStream *pls, void *buf, size_t buf_size)
{
#ifdef BUFFERED_FILE
    plio_fread(buf, buf_size, 1, pls->plbufFile);
#else
/* If U_CHAR is not the same size as what memcpy() expects (typically 1 byte)
 * then this code will have problems.  A better approach might be to use
 * uint8_t from <stdint.h> but I do not know how portable that approach is
 */
    memcpy(buf, (U_CHAR *)pls->plbuf_buffer + pls->plbuf_readpos, buf_size);
    pls->plbuf_readpos += buf_size;
#endif
}

/*--------------------------------------------------------------------------*\
 * wr_command()
 *
 * Write the next command
\*--------------------------------------------------------------------------*/

static void
wr_command(PLStream *pls, U_CHAR c)
{
#ifdef BUFFERED_FILE
    plio_fwrite(&c1, sizeof(U_CHAR), 1, pls->plbufFile);
#else
    if ((pls->plbuf_top + sizeof(U_CHAR)) >= pls->plbuf_buffer_size) {
    // Not enough space, need to grow the buffer
        pls->plbuf_buffer_size += pls->plbuf_buffer_grow;

        if (pls->verbose)
            printf("Growing buffer to %d KB\n", pls->plbuf_buffer_size / 1024);
        if ((pls->plbuf_buffer = realloc(pls->plbuf_buffer, pls->plbuf_buffer_size)) == NULL) 
            plexit("plbuf wr_data:  Plot buffer grow failed");
    }

    *(U_CHAR *)((U_CHAR *)pls->plbuf_buffer + pls->plbuf_top) = c;
    pls->plbuf_top += sizeof(U_CHAR);
#endif
}

/*--------------------------------------------------------------------------*\
 * wr_data()
 *
 * Write the data associated with a command
\*--------------------------------------------------------------------------*/

static void 
wr_data(PLStream *pls, void *buf, size_t buf_size)
{
#ifdef BUFFERED_FILE
    plio_fwrite(buf, buf_size, 1, pls->plbufFile);
#else
    if ((pls->plbuf_top + buf_size) >= pls->plbuf_buffer_size) {
    // Not enough space, need to grow the buffer
        pls->plbuf_buffer_size += pls->plbuf_buffer_grow;

        if ((pls->plbuf_buffer = realloc(pls->plbuf_buffer, pls->plbuf_buffer_size)) == NULL) 
            plexit("plbuf wr_data:  Plot buffer grow failed");
    }

/* If U_CHAR is not the same size as what memcpy() expects (typically 1 byte)
 * then this code will have problems.  A better approach might be to use
 * uint8_t from <stdint.h> but I do not know how portable that approach is
 */
    memcpy((U_CHAR *)pls->plbuf_buffer + pls->plbuf_top, buf, buf_size);
    pls->plbuf_top += buf_size;
#endif
}

/* plbuf_save(state)
 *
 * Saves the current state of the plot into a save buffer.
 * This code was originally in gcw.c and gcw-lib.c.  The original
 * code used a temporary file for the plot buffer and memory
 * to perserve colormaps.  That method does not offer a clean
 * break between using memory buffers and file buffers.  This
 * function preserves the same functionality by returning a data
 * structure that saves the plot buffer and colormaps seperately.
 *
 * The caller passes an existing save buffer for reuse or NULL
 * to force the allocation of a new buffer.  Since one malloc()
 * is used for everything, the entire save buffer can be freed
 * with one free() call.
 *
 */
struct _color_map {
    PLColor *cmap;
    PLINT icol;
    PLINT ncol;
};

struct _state {
    size_t size;       /* Size of the save buffer */
    int    valid;      /* Flag to indicate a valid save state */
#ifdef BUFFERED_FILE
    FILE *plbufFile;
#else
    void *plbuf_buffer;
    size_t plbuf_buffer_size;
    size_t plbuf_top;
    size_t plbuf_readpos;
#endif
    struct _color_map *color_map;
};

void * plbuf_save(PLStream *pls, void *state)
{
    size_t save_size;
    struct _state *plot_state = (struct _state *)state;
    unsigned int i;
    U_CHAR *buf;      /* Assume that this is byte-sized */
   
    if(pls->plbuf_write) {
        pls->plbuf_write = FALSE;
        pls->plbuf_read = TRUE;

        /* Determine the size of the buffer required to save everything. We
         * assume that there are only two colormaps, but have written the code
         * that more than two can be handled with minimal changes.
         */
        save_size = sizeof(struct _state) 
            + 2 * sizeof(struct _color_map)
            + pls->ncol0 * sizeof(PLColor)
            + pls->ncol1 * sizeof(PLColor);

#ifndef BUFFERED_FILE
        /* Only copy as much of the plot buffer that is being used */
        save_size += pls->plbuf_top;
#endif

        /* If a buffer exists, determine if we need to resize it */
        if( state != NULL ) {
           /* We have a save buffer, is it smaller than the current size requirement? */
            if(plot_state->size < save_size) {
                /* Yes, reallocate a larger one */
                if((plot_state = (struct _state *)realloc(state, save_size)) == NULL) {
                    /* NOTE: If realloc fails, then plot_state ill be NULL.  
                     * This will leave the original buffer untouched, thus we
                     * mark it as invalid and return it back to the caller.
                     */
                    plwarn("plbuf: Unable to reallocate sufficient memory to save state");
                    plot_state->valid = 0;

                    return state;
                } 
                plot_state->size = save_size;
            }
        } else {
            /* A buffer does not exist, so we need to allocate one */
            if((plot_state = (struct _state *)malloc(save_size)) == NULL) {
                plwarn("plbuf: Unable to allocate sufficient memory to save state");

                return NULL;
            }
            plot_state->size = save_size;
            
#ifdef BUFFERED_FILE
            /* Make sure the FILE pointer is NULL in order to preven bad things from happening... */
            plot_state->plbufFile = NULL;
#endif
        }

        /* At this point we have an appropriately sized save buffer.
         * We need to invalidate the state of the save buffer, since it
         * will not be valid until after everything is copied.  We use 
         * this approach vice freeing the memory and returning a NULL pointer
         * in order to prevent allocating and freeing memory needlessly.
         */
        plot_state->valid = 0;

        /* Point buf to the space after the struct _state */
        buf = (U_CHAR *)(plot_state + 1);

#ifdef BUFFERED_FILE
        /* Remove the old tempfile, if it exists */
        if( plot_state->plbufFile != NULL ) {
            fclose(plot_state->plbufFile);            
        }

        /* Copy the plot buffer to a tempfile */
        if((plot_state->plbufFile = tmpfile()) == NULL) {
            /* Throw a warning since this might be a permissions problem
             * and we may not want to force an exit 
             */
            plwarn("plbuf: Unable to open temporary file to save state");
            return (void *)plot_state;
        } else {
            U_CHAR tmp;

            rewind(pls->plbufFile);
            while(count = fread(&tmp, sizeof(U_CHAR), 1, pls->plbufFile)) {
	            if(fwrite(&tmp, sizeof(U_CHAR), 1, plot_state->plbufFile)!=count) {
                    /* Throw a warning since this might be a permissions problem
                     * and we may not want to force an exit 
                     */
                    plwarn("plbuf: Unable to write to temporary file");
	                fclose(plot_state->plbufFile);
	                plot_state->plbufFile = NULL;
                    return (void *)plot_state;
	            }
            }
        }
#else
        /* Again, note, that we only copy the portion of the plot buffer that is being used */
        plot_state->plbuf_buffer_size = pls->plbuf_top;
        plot_state->plbuf_top = pls->plbuf_top;
        plot_state->plbuf_readpos = 0;

        /* Create a pointer that points in the space we allocated after struct _state */
        plot_state->plbuf_buffer = (void *)buf;
        buf += pls->plbuf_top;

        /* Copy the plot buffer to our new buffer.  Again, I must stress, that we only
         * are copying the portion of the plot buffer that is being used
         */
        if(memcpy(plot_state->plbuf_buffer, pls->plbuf_buffer, pls->plbuf_top ) == NULL) {
            /* This should never be NULL */
            plwarn("plbuf: Got a NULL in memcpy!");
            return (void *)plot_state;
        }        
#endif

        pls->plbuf_write = TRUE;
        pls->plbuf_read = FALSE;      
      
        /* Save the colormaps.  First create a pointer that points in the space we allocated
         * after the plot buffer */
        plot_state->color_map = (struct _color_map *)buf;
        buf += sizeof(struct _color_map) * 2;

        /* Then we need to make space for the colormaps themselves */
        plot_state->color_map[0].cmap =  (PLColor *)buf;
        buf += sizeof(PLColor) * pls->ncol0;
        plot_state->color_map[1].cmap =  (PLColor *)buf;
        buf += sizeof(PLColor) * pls->ncol1;

        /* Save cmap 0 */
        plot_state->color_map[0].icol = pls->icol0;
        plot_state->color_map[0].ncol = pls->ncol0;
        for (i = 0; i < pls->ncol0; i++) {
	        pl_cpcolor(&(plot_state->color_map[0].cmap[i]), &pls->cmap0[i]);
        }
   
        /* Save cmap 1 */
        plot_state->color_map[1].icol = pls->icol1;
        plot_state->color_map[1].ncol = pls->ncol1;
        for (i = 0; i < pls->ncol1; i++) {
	        pl_cpcolor(&(plot_state->color_map[1].cmap[i]), &pls->cmap1[i]);
        }

        plot_state->valid = 1;
        return (void *)plot_state;
    }

    return NULL;
}

/* plbuf_restore(PLStream *, state)
 *
 * Restores the passed state
 */
void plbuf_restore(PLStream *pls, void *state)
{
    struct _state *new_state;

#ifdef BUFFERED_FILE
    pls->plbufFile = new_state->save_file;
#else
    pls->plbuf_buffer = new_state->plbuf_buffer;
    pls->plbuf_buffer_size = new_state->plbuf_buffer_size;
    pls->plbuf_top = new_state->plbuf_top;
    pls->plbuf_readpos = new_state->plbuf_readpos;
#endif
    /* cmap 0 */
    pls->cmap0 = new_state->color_map[0].cmap;
    pls->icol0 = new_state->color_map[0].icol;
    pls->ncol0 = new_state->color_map[0].ncol;
    /* cmap 1 */
    pls->cmap1 = new_state->color_map[1].cmap;
    pls->icol1 = new_state->color_map[1].icol;
    pls->ncol1 = new_state->color_map[1].ncol;
}

/* plbuf_switch(PLStream *, state)
 *
 * Makes the passed state the current one.  Preserves the previous state 
 * by returning a save buffer. 
 *
 * NOTE:  The current implementation can cause a memory leak under the
 * following scenario:
 *    1) plbuf_save() is called
 *    2) plbuf_switch() is called
 *    3) Commands are called which cause the plot buffer to grow
 *    4) plbuf_swtich() is called
 */
void * plbuf_switch(PLStream *pls, void *state)
{
    struct _state *new_state = (struct _state *)state;
    struct _state *prev_state;    
    size_t save_size;

    /* No saved state was passed, return a NULL--we hope the caller
     * is smart enough to notice
     */
    if(state == NULL) return NULL;

    if(! new_state->valid) {    
        plwarn("plbuf: Attempting to switch to an invalid saved state");
        return NULL;
    }

    save_size = sizeof(struct _state) 
            + 2 * sizeof(struct _color_map);

    if((prev_state = (struct _state *)malloc(save_size)) == NULL) {
        plwarn("plbuf: Unable to allocate memory to save state");
        return NULL;
    }

    /* Set some housekeeping variables */
    prev_state->size = save_size;
    prev_state->valid = 1;

    /* Preserve the existing state */
#ifdef BUFFERED_FILE
    prev_state->plbufFile = pls->plbufFile;
#else
    prev_state->plbuf_buffer = pls->plbuf_buffer;
    prev_state->plbuf_buffer_size = pls->plbuf_buffer_size;
    prev_state->plbuf_top = pls->plbuf_top;
    prev_state->plbuf_readpos = pls->plbuf_readpos;
#endif
    /* cmap 0 */
    prev_state->color_map[0].cmap = pls->cmap0;
    prev_state->color_map[0].icol = pls->icol0;
    prev_state->color_map[0].ncol = pls->ncol0;
    /* cmap 1 */
    prev_state->color_map[1].cmap = pls->cmap1;
    prev_state->color_map[1].icol = pls->icol1;
    prev_state->color_map[1].ncol = pls->ncol1;

    plbuf_restore(pls, new_state);
 
    return (void *) prev_state;
}


