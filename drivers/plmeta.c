/* $Id$
 * $Log$
 * Revision 1.29  2000/12/18 21:01:49  airwin
 * Change to new style plplot/*.h header file locations.
 *
 * Revision 1.28  1995/10/14 17:13:12  mjl
 * Fixed bugs associated with writing the metafile to stdout instad of a file.
 *
 * Revision 1.27  1995/05/06  16:50:19  mjl
 * Changed debugging output to use new pldebug() function.
 *
 * Revision 1.26  1995/03/11  21:38:33  mjl
 * All drivers: eliminated unnecessary variable initializations, other cleaning
 * up.  Added preliminary code to write & restore plot window parameters.
 *
 * Revision 1.25  1994/07/19  22:30:26  mjl
 * All device drivers: enabling macro renamed to PLD_<driver>, where <driver>
 * is xwin, ps, etc.  See plDevs.h for more detail.
 *
 * Revision 1.24  1994/04/08  11:38:11  mjl
 * Now allocate a PLmDev in order to keep file offset information local
 * to the driver where it belongs.  No longer keep track of bytes written
 * since the PDF output routines do that automatically.
*/

/*
    plmeta.c

    Copyright 1991, 1992, 1993, 1994, 1995
    Geoffrey Furnish			furnish@dino.ph.utexas.edu
    Maurice LeBrun			mjl@dino.ph.utexas.edu
    Institute for Fusion Studies	University of Texas at Austin

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public
    License along with this library; if not, write to the Free
    Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
	
    This is a metafile writer for PLplot.

*/
#include "plplot/plDevs.h"

#ifdef PLD_plmeta

#include "plplot/plplotP.h"
#include "plplot/drivers.h"
#include "plplot/metadefs.h"
#include <string.h>

/* Struct to hold device-specific info. */

typedef struct {
    PLFLT pxlx, pxly;
    PLINT xold, yold;

    PLINT xmin, xmax, xlen;
    PLINT ymin, ymax, ylen;

    FPOS_T lp_offset, toc_offset;
} PLmDev;

/* Function prototypes */

static void WriteHeader		(PLStream *pls);
static void plm_fill		(PLStream *pls);
static void plm_swin		(PLStream *pls);

/*--------------------------------------------------------------------------*\
 * plD_init_plm()
 *
 * Initialize device.
\*--------------------------------------------------------------------------*/

void
plD_init_plm(PLStream *pls)
{
    PLmDev *dev;
    U_CHAR c = (U_CHAR) INITIALIZE;

    dbug_enter("plD_init_plm");

    pls->color = 1;		/* Is a color device */
    pls->dev_fill0 = 1;		/* Handle solid fills */
    pls->dev_fill1 = 1;		/* Handle pattern fills */

/* Initialize family file info */

    plFamInit(pls);

/* Prompt for a file name if not already set */

    plOpenFile(pls);
    pls->pdfs = pdf_finit(pls->OutFile);

/* Allocate and initialize device-specific data */

    pls->dev = calloc(1, (size_t) sizeof(PLmDev));
    if (pls->dev == NULL)
	plexit("plD_init_plm: Out of memory.");

    dev = (PLmDev *) pls->dev;

    dev->xold = UNDEFINED;
    dev->yold = UNDEFINED;

    dev->xmin = 0;
    dev->xmax = PIXELS_X - 1;
    dev->ymin = 0;
    dev->ymax = PIXELS_Y - 1;

    dev->pxlx = (double) PIXELS_X / (double) LPAGE_X;
    dev->pxly = (double) PIXELS_Y / (double) LPAGE_Y;

    plP_setpxl(dev->pxlx, dev->pxly);
    plP_setphy(dev->xmin, dev->xmax, dev->ymin, dev->ymax);

/* Write Metafile header. */

    WriteHeader(pls);

/* Write color map state info */

    plD_state_plm(pls, PLSTATE_CMAP0);
    plD_state_plm(pls, PLSTATE_CMAP1);

/* Write initialization command. */

    plm_wr( pdf_wr_1byte(pls->pdfs, c) );
}

/*--------------------------------------------------------------------------*\
 * plD_line_plm()
 *
 * Draw a line in the current color from (x1,y1) to (x2,y2).
\*--------------------------------------------------------------------------*/

void
plD_line_plm(PLStream *pls, short x1, short y1, short x2, short y2)
{
    PLmDev *dev = (PLmDev *) pls->dev;
    U_CHAR c;
    U_SHORT xy[4];

    dbug_enter("plD_line_plm");

/* Failsafe check */

#ifdef DEBUG
    if (x1 < dev->xmin || x1 > dev->xmax ||
	x2 < dev->xmin || x2 > dev->xmax ||
	y1 < dev->ymin || y1 > dev->ymax ||
	y2 < dev->ymin || y2 > dev->ymax) {

	pldebug("plD_line_plm",
		"coordinates out of bounds -- \nActual: (%i,%i), (%i,%i) Bounds: (%i,%i,%i,%i)\n", 
		x1, y1, x2, y2, dev->xmin, dev->xmax, dev->ymin, dev->ymax);
    }
#endif

/* If continuation of previous line send the LINETO command, which uses
   the previous (x,y) point as it's starting location.  This results in a
   storage reduction of not quite 50%, since the instruction length for
   a LINETO is 5/9 of that for the LINE command, and given that most
   graphics applications use this command heavily.

   Still not quite as efficient as tektronix format since we also send the
   command each time (so shortest command is 25% larger), but a lot easier
   to implement than the tek method.  
 */
    if (x1 == dev->xold && y1 == dev->yold) {

	c = (U_CHAR) LINETO;
	plm_wr( pdf_wr_1byte(pls->pdfs, c) );

	xy[0] = x2;
	xy[1] = y2;
	plm_wr( pdf_wr_2nbytes(pls->pdfs, xy, 2) );
    }
    else {
	c = (U_CHAR) LINE;
	plm_wr( pdf_wr_1byte(pls->pdfs, c) );

	xy[0] = x1;
	xy[1] = y1;
	xy[2] = x2;
	xy[3] = y2;
	plm_wr( pdf_wr_2nbytes(pls->pdfs, xy, 4) );
    }
    dev->xold = x2;
    dev->yold = y2;
}

/*--------------------------------------------------------------------------*\
 * plD_polyline_plm()
 *
 * Draw a polyline in the current color.
\*--------------------------------------------------------------------------*/

void
plD_polyline_plm(PLStream *pls, short *xa, short *ya, PLINT npts)
{
    PLmDev *dev = (PLmDev *) pls->dev;
    U_CHAR c = (U_CHAR) POLYLINE;

    dbug_enter("plD_polyline_plm");

    plm_wr( pdf_wr_1byte(pls->pdfs, c) );

    plm_wr( pdf_wr_2bytes(pls->pdfs, (U_SHORT) npts) );

    plm_wr( pdf_wr_2nbytes(pls->pdfs, (U_SHORT *) xa, npts) );
    plm_wr( pdf_wr_2nbytes(pls->pdfs, (U_SHORT *) ya, npts) );

    dev->xold = xa[npts - 1];
    dev->yold = ya[npts - 1];
}

/*--------------------------------------------------------------------------*\
 * plD_eop_plm()
 *
 * End of page.
\*--------------------------------------------------------------------------*/

void
plD_eop_plm(PLStream *pls)
{
    U_CHAR c = (U_CHAR) EOP;

    plm_wr( pdf_wr_1byte(pls->pdfs, c) );
}

/*--------------------------------------------------------------------------*\
 * plD_bop_plm()
 *
 * Set up for the next page.
\*--------------------------------------------------------------------------*/

void
plD_bop_plm(PLStream *pls)
{
    PLmDev *dev = (PLmDev *) pls->dev;
    U_CHAR c = (U_CHAR) BOP;
    FPOS_T cp_offset=0, fwbyte_offset=0, bwbyte_offset=0;
    FILE *file = pls->OutFile;

    dbug_enter("plD_bop_plm");

    dev->xold = UNDEFINED;
    dev->yold = UNDEFINED;

    fflush(file);

/* If writing to a file, find out where we are */

    if (pls->output_type == 0) {
	if (pl_fgetpos(file, &cp_offset))
	    plexit("plD_bop_plm: fgetpos call failed");

    /* Seek back to previous page header and write forward byte offset. */

	if (dev->lp_offset > 0) {
#ifdef DEBUG
	    U_LONG foo;
#endif
	    pldebug("plD_bop_plm",
		    "Location: %d, seeking to: %d\n",
		    cp_offset, dev->lp_offset);
	    fwbyte_offset = dev->lp_offset + 7;
	    if (pl_fsetpos(file, &fwbyte_offset))
		plexit("plD_bop_plm: fsetpos call failed");

#ifdef DEBUG
	    if (pl_fgetpos(file, &fwbyte_offset))
		plexit("plD_bop_plm: fgetpos call failed");

	    pldebug("plD_bop_plm",
		    "Now at: %d, to write: %d\n", fwbyte_offset, cp_offset);
#endif

	    plm_wr( pdf_wr_4bytes(pls->pdfs, (U_LONG) cp_offset) );
	    fflush(file);

#ifdef DEBUG
	    if (pl_fsetpos(file, &fwbyte_offset))
		plexit("plD_bop_plm: fsetpos call failed");

	    plm_rd(pdf_rd_4bytes(pls->pdfs, &foo));
	    pldebug("plD_bop_plm", "Value read as: %d\n", foo);
#endif

	    if (pl_fsetpos(file, &cp_offset))
		plexit("plD_bop_plm: fsetpos call failed");
	}
    }

/* Start next family file if necessary. */

    pls->bytecnt = pls->pdfs->bp;
    plGetFam(pls);

/* Update page counter */

    pls->page++;

/* Update table of contents info.  Right now only number of pages. */
/* The order here is critical */

    if (pls->output_type == 0) {
	if (dev->toc_offset > 0) {
	    if (pl_fsetpos(file, &dev->toc_offset))
		plexit("plD_bop_plm: fsetpos call failed");

	    plm_wr( pdf_wr_header(pls->pdfs, "pages") );
	    plm_wr( pdf_wr_2bytes(pls->pdfs, (U_SHORT) pls->page) );

	    if (pl_fsetpos(file, &cp_offset))
		plexit("plD_bop_plm: fsetpos call failed");
	}
    }

/* Write new page header */

    bwbyte_offset = dev->lp_offset;

    plm_wr( pdf_wr_1byte(pls->pdfs, c) );
    plm_wr( pdf_wr_2bytes(pls->pdfs, (U_SHORT) pls->page) );
    plm_wr( pdf_wr_4bytes(pls->pdfs, (U_LONG) bwbyte_offset) );
    plm_wr( pdf_wr_4bytes(pls->pdfs, (U_LONG) 0) );

    dev->lp_offset = cp_offset;

/* Write some page state information just to make things nice later on */
/* Eventually there will be more */

    plD_state_plm(pls, PLSTATE_COLOR0);
}

/*--------------------------------------------------------------------------*\
 * plD_tidy_plm()
 *
 * Close graphics file
\*--------------------------------------------------------------------------*/

void
plD_tidy_plm(PLStream *pls)
{
    U_CHAR c = (U_CHAR) CLOSE;

    dbug_enter("plD_tidy_plm");

    plm_wr( pdf_wr_1byte(pls->pdfs, c) );
    pdf_close(pls->pdfs);
}

/*--------------------------------------------------------------------------*\
 * plD_state_plm()
 *
 * Handle change in PLStream state (color, pen width, fill attribute, etc).
\*--------------------------------------------------------------------------*/

void 
plD_state_plm(PLStream *pls, PLINT op)
{
    U_CHAR c = (U_CHAR) CHANGE_STATE;
    int i;

    dbug_enter("plD_state_plm");

    plm_wr( pdf_wr_1byte(pls->pdfs, c) );
    plm_wr( pdf_wr_1byte(pls->pdfs, op) );

    switch (op) {

    case PLSTATE_WIDTH:
	plm_wr( pdf_wr_2bytes(pls->pdfs, (U_SHORT) (pls->width)) );
	break;

    case PLSTATE_COLOR0:
	plm_wr( pdf_wr_1byte(pls->pdfs, (U_CHAR) pls->icol0) );

	if (pls->icol0 == PL_RGB_COLOR) {
	    plm_wr( pdf_wr_1byte(pls->pdfs, pls->curcolor.r) );
	    plm_wr( pdf_wr_1byte(pls->pdfs, pls->curcolor.g) );
	    plm_wr( pdf_wr_1byte(pls->pdfs, pls->curcolor.b) );
	}
	break;

    case PLSTATE_COLOR1:
	plm_wr( pdf_wr_2bytes(pls->pdfs, (U_SHORT) pls->icol1) );
	break;

    case PLSTATE_FILL:
	plm_wr( pdf_wr_1byte(pls->pdfs, (U_CHAR) pls->patt) );
	break;

    case PLSTATE_CMAP0:
	plm_wr( pdf_wr_1byte(pls->pdfs, (U_CHAR) pls->ncol0) );
	for (i = 0; i < pls->ncol0; i++) {
	    plm_wr( pdf_wr_1byte(pls->pdfs, pls->cmap0[i].r) );
	    plm_wr( pdf_wr_1byte(pls->pdfs, pls->cmap0[i].g) );
	    plm_wr( pdf_wr_1byte(pls->pdfs, pls->cmap0[i].b) );
	}
	break;

    case PLSTATE_CMAP1:
	plm_wr( pdf_wr_2bytes(pls->pdfs, (U_SHORT) pls->ncol1) );
	for (i = 0; i < pls->ncol1; i++) {
	    plm_wr( pdf_wr_1byte(pls->pdfs, pls->cmap1[i].r) );
	    plm_wr( pdf_wr_1byte(pls->pdfs, pls->cmap1[i].g) );
	    plm_wr( pdf_wr_1byte(pls->pdfs, pls->cmap1[i].b) );
	}
	break;
    }
}

/*--------------------------------------------------------------------------*\
 * plD_esc_plm()
 *
 * Escape function.  Note that any data written must be in device
 * independent form to maintain the transportability of the metafile.
 *
 * Functions:
 *
 *	PLESC_FILL	Fill polygon
 *	PLESC_SWIN	Set window parameters
 *
\*--------------------------------------------------------------------------*/

void
plD_esc_plm(PLStream *pls, PLINT op, void *ptr)
{
    U_CHAR c = (U_CHAR) ESCAPE;

    dbug_enter("plD_esc_plm");

    plm_wr( pdf_wr_1byte(pls->pdfs, c) );
    plm_wr( pdf_wr_1byte(pls->pdfs, (U_CHAR) op) );

    switch (op) {
    case PLESC_FILL:
	plm_fill(pls);
	break;

    case PLESC_SWIN:
	plm_swin(pls);
	break;
    }
}

/*--------------------------------------------------------------------------*\
 * plm_fill()
 *
 * Fill polygon described in points pls->dev_x[] and pls->dev_y[].
\*--------------------------------------------------------------------------*/

static void
plm_fill(PLStream *pls)
{
    PLmDev *dev = (PLmDev *) pls->dev;

    dbug_enter("plm_fill");

    plm_wr( pdf_wr_2bytes(pls->pdfs, (U_SHORT) pls->dev_npts) );

    plm_wr( pdf_wr_2nbytes(pls->pdfs, (U_SHORT *) pls->dev_x, pls->dev_npts) );
    plm_wr( pdf_wr_2nbytes(pls->pdfs, (U_SHORT *) pls->dev_y, pls->dev_npts) );

    dev->xold = UNDEFINED;
    dev->yold = UNDEFINED;
}

/*--------------------------------------------------------------------------*\
 * plm_swin()
 *
 * Set window parameters.
 * Each parameter or group of parameters is tagged to make backward
 * compatibility easier.
\*--------------------------------------------------------------------------*/

static void
plm_swin(PLStream *pls)
{
    PLmDev *dev = (PLmDev *) pls->dev;

    dbug_enter("plm_swin");
}

/*--------------------------------------------------------------------------*\
 * WriteHeader()
 *
 * Writes Metafile header.
\*--------------------------------------------------------------------------*/

static void
WriteHeader(PLStream *pls)
{
    PLmDev *dev = (PLmDev *) pls->dev;
    FILE *file = pls->OutFile;

    dbug_enter("WriteHeader(PLStream *pls");

    plm_wr( pdf_wr_header(pls->pdfs, PLMETA_HEADER) );
    plm_wr( pdf_wr_header(pls->pdfs, PLMETA_VERSION) );

/* Write table of contents info.  Right now only number of pages. */
/* The order here is critical */

    if (pl_fgetpos(file, &dev->toc_offset))
	plexit("WriteHeader: fgetpos call failed");

    plm_wr( pdf_wr_header(pls->pdfs, "pages") );
    plm_wr( pdf_wr_2bytes(pls->pdfs, (U_SHORT) 0) );

/* Write initialization info.  Tag via strings to make backward
   compatibility with old metafiles as easy as possible. */

    plm_wr( pdf_wr_header(pls->pdfs, "xmin") );
    plm_wr( pdf_wr_2bytes(pls->pdfs, (U_SHORT) dev->xmin) );

    plm_wr( pdf_wr_header(pls->pdfs, "xmax") );
    plm_wr( pdf_wr_2bytes(pls->pdfs, (U_SHORT) dev->xmax) );

    plm_wr( pdf_wr_header(pls->pdfs, "ymin") );
    plm_wr( pdf_wr_2bytes(pls->pdfs, (U_SHORT) dev->ymin) );

    plm_wr( pdf_wr_header(pls->pdfs, "ymax") );
    plm_wr( pdf_wr_2bytes(pls->pdfs, (U_SHORT) dev->ymax) );

    plm_wr( pdf_wr_header(pls->pdfs, "pxlx") );
    plm_wr( pdf_wr_ieeef(pls->pdfs, (float) dev->pxlx) );

    plm_wr( pdf_wr_header(pls->pdfs, "pxly") );
    plm_wr( pdf_wr_ieeef(pls->pdfs, (float) dev->pxly) );

    plm_wr( pdf_wr_header(pls->pdfs, "") );
}

#else
int 
pldummy_plmeta()
{
    return 0;
}

#endif				/* PLD_plmeta */
