/* $Id$
 * $Log$
 * Revision 1.19  1993/08/11 19:18:05  mjl
 * Changed debugging code to print to stderr instead of stdout.
 *
 * Revision 1.18  1993/08/03  01:46:41  mjl
 * Changes to eliminate warnings when compiling with gcc -Wall.
 *
 * Revision 1.17  1993/07/31  07:56:41  mjl
 * Several driver functions consolidated, for all drivers.  The width and color
 * commands are now part of a more general "state" command.  The text and
 * graph commands used for switching between modes is now handled by the
 * escape function (very few drivers require it).  The device-specific PLDev
 * structure is now malloc'ed for each driver that requires it, and freed when
 * the stream is terminated.
 *
 * Revision 1.16  1993/07/16  22:13:48  mjl
 * Switched coordinates to standard meta coords.  Eliminated obsolete low-level
 * operations.  Added write of some state information (colors and pen width)
 * at the beginning of each page to aid seeking while in renderer.
 *
 * Revision 1.15  1993/07/01  22:00:51  mjl
 * Changed all plplot source files to include plplotP.h (private) rather than
 * plplot.h.  Rationalized namespace -- all externally-visible plplot functions
 * now start with "pl"; device driver functions start with "plD_".  PDF
 * functions start with "pdf_".
 *
 * Revision 1.14  1993/04/26  19:57:48  mjl
 * Fixes to allow (once again) output to stdout and plrender to function as
 * a filter.  A type flag was added to handle file vs stream differences.
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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "drivers.h"
#include "metadefs.h"
#include "pdf.h"

/* Function prototypes */
/* INDENT OFF */

static void WriteHeader		(PLStream *);

/* INDENT ON */
/*----------------------------------------------------------------------*\
* plD_init_plm()
*
* Initialize device.
\*----------------------------------------------------------------------*/

void
plD_init_plm(PLStream *pls)
{
    PLDev *dev;
    U_CHAR c = (U_CHAR) INITIALIZE;

    dbug_enter("plD_init_plm");

    pls->termin = 0;		/* not an interactive terminal */
    pls->icol0 = 1;
    pls->color = 1;
    pls->width = 1;
    pls->bytecnt = 0;
    pls->page = 0;

/* Initialize family file info */

    plFamInit(pls);

/* Prompt for a file name if not already set */

    plOpenFile(pls);

/* Allocate and initialize device-specific data */

    dev = plAllocDev(pls);

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

/* Write initialization command. */

    plm_wr(pdf_wr_1byte(pls->OutFile, c));
}

/*----------------------------------------------------------------------*\
* plD_line_plm()
*
* Draw a line in the current color from (x1,y1) to (x2,y2).
\*----------------------------------------------------------------------*/

void
plD_line_plm(PLStream *pls, short x1, short y1, short x2, short y2)
{
    PLDev *dev = (PLDev *) pls->dev;
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
	plm_wr(pdf_wr_1byte(pls->OutFile, c));
	pls->bytecnt++;

	xy[0] = x2;
	xy[1] = y2;
	plm_wr(pdf_wr_2nbytes(pls->OutFile, xy, 2));
	pls->bytecnt += 4;
    }
    else {
	c = (U_CHAR) LINE;
	plm_wr(pdf_wr_1byte(pls->OutFile, c));
	pls->bytecnt++;

	xy[0] = x1;
	xy[1] = y1;
	xy[2] = x2;
	xy[3] = y2;
	plm_wr(pdf_wr_2nbytes(pls->OutFile, xy, 4));
	pls->bytecnt += 8;
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
    PLDev *dev = (PLDev *) pls->dev;
    U_CHAR c = (U_CHAR) POLYLINE;

    dbug_enter("plD_polyline_plm");

    plm_wr(pdf_wr_1byte(pls->OutFile, c));
    pls->bytecnt++;

    plm_wr(pdf_wr_2bytes(pls->OutFile, (U_SHORT) npts));
    pls->bytecnt += 2;

    plm_wr(pdf_wr_2nbytes(pls->OutFile, (U_SHORT *) xa, npts));
    plm_wr(pdf_wr_2nbytes(pls->OutFile, (U_SHORT *) ya, npts));
    pls->bytecnt += 4 * npts;

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

    plm_wr(pdf_wr_1byte(pls->OutFile, c));
    pls->bytecnt++;
}

/*----------------------------------------------------------------------*\
* plD_bop_plm()
*
* Set up for the next page.
\*----------------------------------------------------------------------*/

void
plD_bop_plm(PLStream *pls)
{
    PLDev *dev = (PLDev *) pls->dev;
    U_CHAR c = (U_CHAR) PAGE;
    FPOS_T cp_offset=0, fwbyte_offset=0, bwbyte_offset=0;

    dbug_enter("plD_bop_plm");

    dev->xold = UNDEFINED;
    dev->yold = UNDEFINED;

    fflush(pls->OutFile);
    if (pls->output_type == 0) {
	if (pl_fgetpos(pls->OutFile, &cp_offset))
	    plexit("plD_bop_plm: fgetpos call failed");
    }

/* Seek back to previous page header and write forward byte offset. */

    if (pls->lp_offset > 0) {
#ifdef DEBUG
	U_LONG foo;
	fprintf(stderr, "Location: %d, seeking to: %d\n",
		cp_offset, pls->lp_offset);
#endif
	fwbyte_offset = pls->lp_offset + 7;
	if (pl_fsetpos(pls->OutFile, &fwbyte_offset))
	    plexit("plD_bop_plm: fsetpos call failed");

#ifdef DEBUG
	if (pl_fgetpos(pls->OutFile, &fwbyte_offset))
	    plexit("plD_bop_plm: fgetpos call failed");

	fprintf(stderr, "Now at: %d, to write: %d\n",
		fwbyte_offset, cp_offset);
#endif

	plm_wr(pdf_wr_4bytes(pls->OutFile, (U_LONG) cp_offset));
	fflush(pls->OutFile);

#ifdef DEBUG
	if (pl_fsetpos(pls->OutFile, &fwbyte_offset))
	    plexit("plD_bop_plm: fsetpos call failed");

	plm_rd(pdf_rd_4bytes(pls->OutFile, &foo));
	fprintf(stderr, "Value read as: %d\n", foo);
#endif

	if (pl_fsetpos(pls->OutFile, &cp_offset))
	    plexit("plD_bop_plm: fsetpos call failed");
    }

/* Start next family file if necessary. */

    plGetFam(pls);

/* Write new page header */

    pls->page++;
    bwbyte_offset = pls->lp_offset;

    plm_wr(pdf_wr_1byte(pls->OutFile, c));
    plm_wr(pdf_wr_2bytes(pls->OutFile, (U_SHORT) pls->page));
    plm_wr(pdf_wr_4bytes(pls->OutFile, (U_LONG) bwbyte_offset));
    plm_wr(pdf_wr_4bytes(pls->OutFile, (U_LONG) 0));

    pls->bytecnt += 11;
    pls->lp_offset = cp_offset;

/* Write some page state information just to make things nice later on */
/* Eventually there will be more */

    plD_state_plm(pls, PLSTATE_WIDTH);
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

    plm_wr(pdf_wr_1byte(pls->OutFile, c));
    pls->bytecnt++;

    fclose(pls->OutFile);
    pls->fileset = 0;
    pls->page = 0;
    pls->OutFile = NULL;
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

    dbug_enter("plD_state_plm");

    plm_wr(pdf_wr_1byte(pls->OutFile, c));
    pls->bytecnt++;

    switch (op) {

    case PLSTATE_WIDTH:
	plm_wr(pdf_wr_1byte(pls->OutFile, op));
	pls->bytecnt++;

	plm_wr(pdf_wr_2bytes(pls->OutFile, (U_SHORT) (pls->width)));
	pls->bytecnt += 2;
	break;

    case PLSTATE_COLOR0:
	plm_wr(pdf_wr_1byte(pls->OutFile, op));
	pls->bytecnt++;

	plm_wr(pdf_wr_1byte(pls->OutFile, (U_CHAR) pls->icol0));
	pls->bytecnt++;

	if (pls->icol0 == PL_RGB_COLOR) {
	    plm_wr(pdf_wr_1byte(pls->OutFile, pls->curcolor.r));
	    plm_wr(pdf_wr_1byte(pls->OutFile, pls->curcolor.g));
	    plm_wr(pdf_wr_1byte(pls->OutFile, pls->curcolor.b));
	}
	break;

    case PLSTATE_COLOR1:
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
\*----------------------------------------------------------------------*/

void
plD_esc_plm(PLStream *pls, PLINT op, void *ptr)
{
    U_CHAR c = (U_CHAR) ESCAPE;
    U_CHAR opc;

    dbug_enter("plD_esc_plm");

    plm_wr(pdf_wr_1byte(pls->OutFile, c));
    pls->bytecnt++;

    opc = (U_CHAR) op;
    plm_wr(pdf_wr_1byte(pls->OutFile, opc));
    pls->bytecnt++;

    switch (op) {

      case 0:
	break;
    }
}

/*----------------------------------------------------------------------*\
* WriteHeader()
*
* Writes a PLPLOT Metafile header.
\*----------------------------------------------------------------------*/

static void
WriteHeader(PLStream *pls)
{
    PLDev *dev = (PLDev *) pls->dev;

    dbug_enter("WriteHeader(PLStream *pls");

    plm_wr(pdf_wr_header(pls->OutFile, PLMETA_HEADER));
    plm_wr(pdf_wr_header(pls->OutFile, PLMETA_VERSION));

/* Write initialization info.  Tag via strings to make backward
   compatibility with old metafiles as easy as possible. */

    plm_wr(pdf_wr_header(pls->OutFile, "xmin"));
    plm_wr(pdf_wr_2bytes(pls->OutFile, (U_SHORT) dev->xmin));

    plm_wr(pdf_wr_header(pls->OutFile, "xmax"));
    plm_wr(pdf_wr_2bytes(pls->OutFile, (U_SHORT) dev->xmax));

    plm_wr(pdf_wr_header(pls->OutFile, "ymin"));
    plm_wr(pdf_wr_2bytes(pls->OutFile, (U_SHORT) dev->ymin));

    plm_wr(pdf_wr_header(pls->OutFile, "ymax"));
    plm_wr(pdf_wr_2bytes(pls->OutFile, (U_SHORT) dev->ymax));

    plm_wr(pdf_wr_header(pls->OutFile, "pxlx"));
    plm_wr(pdf_wr_ieeef(pls->OutFile, (float) dev->pxlx));

    plm_wr(pdf_wr_header(pls->OutFile, "pxly"));
    plm_wr(pdf_wr_ieeef(pls->OutFile, (float) dev->pxly));

    plm_wr(pdf_wr_header(pls->OutFile, ""));
}

#else
int 
pldummy_plmeta()
{
    return 0;
}

#endif				/* PLMETA */
