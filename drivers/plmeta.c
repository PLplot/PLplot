/* $Id$
 * $Log$
 * Revision 1.24  1994/04/08 11:38:11  mjl
 * Now allocate a PLmDev in order to keep file offset information local
 * to the driver where it belongs.  No longer keep track of bytes written
 * since the PDF output routines do that automatically.
 *
 * Revision 1.23  1994/03/23  06:39:22  mjl
 * Added support for: color map 1 color selection, color map 0 or color map 1
 * state change (palette change), polygon fills.
 *
 * All drivers: cleaned up by eliminating extraneous includes (stdio.h and
 * stdlib.h now included automatically by plplotP.h), extraneous clears
 * of pls->fileset, pls->page, and pls->OutFile = NULL (now handled in
 * driver interface or driver initialization as appropriate).  Special
 * handling for malloc includes eliminated (no longer needed) and malloc
 * prototypes fixed as necessary.
 *
 * Revision 1.22  1994/01/15  17:21:58  mjl
 * Changed to new PDF function syntax.
 *
 * Revision 1.21  1993/11/15  08:29:19  mjl
 * Now writes number of pages in file to beginning of file on each
 * new page.  For seeking to a specified number of pages before EOF.
*/

/*
    plmeta.c

    Copyright 1991, 1992
    Geoffrey Furnish
    Maurice LeBrun

    This software may be freely copied, modified and redistributed without
    fee provided that this copyright notice is preserved intact on all
    copies and modified copies.

    There is no warranty or other guarantee of fitness of this software.
    It is provided solely "as is". The author(s) disclaim(s) all
    responsibility and liability with respect to this software's usage or
    its effect upon hardware or computer systems.

* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
	
    This is a metafile writer for plplot.

*/
#ifdef PLMETA

#include "plplotP.h"
#include "drivers.h"
#include "metadefs.h"
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
/* INDENT OFF */

static void WriteHeader		(PLStream *pls);
static void plm_fill		(PLStream *pls);

/* INDENT ON */
/*----------------------------------------------------------------------*\
* plD_init_plm()
*
* Initialize device.
\*----------------------------------------------------------------------*/

void
plD_init_plm(PLStream *pls)
{
    PLmDev *dev;
    U_CHAR c = (U_CHAR) INITIALIZE;

    dbug_enter("plD_init_plm");

    pls->termin = 0;		/* not an interactive terminal */
    pls->icol0 = 1;
    pls->color = 1;
    pls->page = 0;
    pls->dev_fill0 = 1;
    pls->dev_fill1 = 1;

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

/*----------------------------------------------------------------------*\
* plD_line_plm()
*
* Draw a line in the current color from (x1,y1) to (x2,y2).
\*----------------------------------------------------------------------*/

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

	fprintf(stderr, "PLPLOT: coordinates out of bounds in driver.\n");
	fprintf(stderr, "  Actual: (%i,%i), (%i,%i)   Bounds: (%i,%i,%i,%i)\n",
		x1, y1, x2, y2, dev->xmin, dev->xmax, dev->ymin, dev->ymax);
    }
#endif

/* If continuation of previous line send the LINETO command, which uses
   the previous (x,y) point as it's starting location.  This results in a
   storage reduction of not quite 50%, since the instruction length for
   a LINETO is 5/9 of that for the LINE command, and given that most
   graphics applications use this command heavily.

   Still not quite as efficient as tektronix format since we also send the
   command each time (so shortest command is 25% larger), but a heck of
   a lot easier to implement than the tek method.
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

/*----------------------------------------------------------------------*\
* plD_polyline_plm()
*
* Draw a polyline in the current color.
\*----------------------------------------------------------------------*/

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

/*----------------------------------------------------------------------*\
* plD_eop_plm()
*
* End of page.
\*----------------------------------------------------------------------*/

void
plD_eop_plm(PLStream *pls)
{
    U_CHAR c = (U_CHAR) CLEAR;

    plm_wr( pdf_wr_1byte(pls->pdfs, c) );
}

/*----------------------------------------------------------------------*\
* plD_bop_plm()
*
* Set up for the next page.
\*----------------------------------------------------------------------*/

void
plD_bop_plm(PLStream *pls)
{
    PLmDev *dev = (PLmDev *) pls->dev;
    U_CHAR c = (U_CHAR) PAGE;
    FPOS_T cp_offset=0, fwbyte_offset=0, bwbyte_offset=0;
    FILE *file = pls->OutFile;

    dbug_enter("plD_bop_plm");

    dev->xold = UNDEFINED;
    dev->yold = UNDEFINED;

    fflush(file);
    if (pls->output_type == 0) {
	if (pl_fgetpos(file, &cp_offset))
	    plexit("plD_bop_plm: fgetpos call failed");
    }

/* Seek back to previous page header and write forward byte offset. */

    if (dev->lp_offset > 0) {
#ifdef DEBUG
	U_LONG foo;
	fprintf(stderr, "Location: %d, seeking to: %d\n",
		cp_offset, dev->lp_offset);
#endif
	fwbyte_offset = dev->lp_offset + 7;
	if (pl_fsetpos(file, &fwbyte_offset))
	    plexit("plD_bop_plm: fsetpos call failed");

#ifdef DEBUG
	if (pl_fgetpos(file, &fwbyte_offset))
	    plexit("plD_bop_plm: fgetpos call failed");

	fprintf(stderr, "Now at: %d, to write: %d\n",
		fwbyte_offset, cp_offset);
#endif

	plm_wr( pdf_wr_4bytes(pls->pdfs, (U_LONG) cp_offset) );
	fflush(file);

#ifdef DEBUG
	if (pl_fsetpos(file, &fwbyte_offset))
	    plexit("plD_bop_plm: fsetpos call failed");

	plm_rd(pdf_rd_4bytes(pls->pdfs, &foo));
	fprintf(stderr, "Value read as: %d\n", foo);
#endif

	if (pl_fsetpos(file, &cp_offset))
	    plexit("plD_bop_plm: fsetpos call failed");
    }

/* Start next family file if necessary. */

    pls->bytecnt = pls->pdfs->bp;
    plGetFam(pls);

/* Update page counter */

    pls->page++;

/* Update table of contents info.  Right now only number of pages. */
/* The order here is critical */

    if (dev->toc_offset > 0) {
	if (pl_fsetpos(file, &dev->toc_offset))
	    plexit("plD_bop_plm: fsetpos call failed");

	plm_wr( pdf_wr_header(pls->pdfs, "pages") );
	plm_wr( pdf_wr_2bytes(pls->pdfs, (U_SHORT) pls->page) );

	if (pl_fsetpos(file, &cp_offset))
	    plexit("plD_bop_plm: fsetpos call failed");
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

/*----------------------------------------------------------------------*\
* plD_tidy_plm()
*
* Close graphics file
\*----------------------------------------------------------------------*/

void
plD_tidy_plm(PLStream *pls)
{
    U_CHAR c = (U_CHAR) CLOSE;

    dbug_enter("plD_tidy_plm");

    plm_wr( pdf_wr_1byte(pls->pdfs, c) );
    pdf_close(pls->pdfs);
}

/*----------------------------------------------------------------------*\
* plD_state_plm()
*
* Handle change in PLStream state (color, pen width, fill attribute, etc).
\*----------------------------------------------------------------------*/

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

/*----------------------------------------------------------------------*\
* plD_esc_plm()
*
* Escape function.  Note that any data written must be in device
* independent form to maintain the transportability of the metafile.
*
* Functions:
*
*	PLESC_FILL	Fill polygon
*
\*----------------------------------------------------------------------*/

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
    }
}

/*----------------------------------------------------------------------*\
* plm_fill()
*
* Fill polygon described in points pls->dev_x[] and pls->dev_y[].
\*----------------------------------------------------------------------*/

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

/*----------------------------------------------------------------------*\
* WriteHeader()
*
* Writes a PLPLOT Metafile header.
\*----------------------------------------------------------------------*/

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

#endif				/* PLMETA */
