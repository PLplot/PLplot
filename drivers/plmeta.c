/* $Id$
   $Log$
   Revision 1.13  1993/03/18 07:05:17  mjl
   Eliminated SWITCH_TO_TEXT and SWITCH_TO_GRAPH metafile commands from both
   driver and renderer.  These are really not necessary when a metafile is
   being used and can be aggravating when using the xterm driver.

 * Revision 1.12  1993/03/16  06:47:35  mjl
 * Made the "sick hack" to enable plplot to work with non-ANSI libc's a bit
 * more robust.
 *
 * Revision 1.11  1993/03/15  21:39:18  mjl
 * Changed all _clear/_page driver functions to the names _eop/_bop, to be
 * more representative of what's actually going on.
 *
 * Revision 1.10  1993/03/03  19:42:05  mjl
 * Changed PLSHORT -> short everywhere; now all device coordinates are expected
 * to fit into a 16 bit address space (reasonable, and good for performance).
 *
 * Revision 1.9  1993/03/03  16:17:59  mjl
 * Added #include <stdlib.h> since the code needs to exit(1) on some errors.
 *
 * Revision 1.8  1993/02/27  04:50:33  mjl
 * Simplified fgetpos/fsetpos usage, to where it will now actually work on rays,
 * and I hope the Amiga and Linux also.  Lesson to all: seeking in C is fragile!
 * Also added lots of diagnostics, enabled when DEBUG is defined.
 *
 * Revision 1.7  1993/02/27  01:41:39  mjl
 * Changed from ftell/fseek to fgetpos/fsetpos.
 *
 * Revision 1.6  1993/02/22  23:11:01  mjl
 * Eliminated the gradv() driver calls, as these were made obsolete by
 * recent changes to plmeta and plrender.  Also eliminated page clear commands
 * from grtidy() -- plend now calls grclr() and grtidy() explicitly.
 *
 * Revision 1.5  1993/01/23  05:41:50  mjl
 * Changes to support new color model, polylines, and event handler support
 * (interactive devices only).
 *
 * Revision 1.4  1992/11/07  07:48:46  mjl
 * Fixed orientation operation in several files and standardized certain startup
 * operations. Fixed bugs in various drivers.
 *
 * Revision 1.3  1992/09/30  18:24:57  furnish
 * Massive cleanup to irradicate garbage code.  Almost everything is now
 * prototyped correctly.  Builds on HPUX, SUNOS (gcc), AIX, and UNICOS.
 *
 * Revision 1.2  1992/09/29  04:44:47  furnish
 * Massive clean up effort to remove support for garbage compilers (K&R).
 *
 * Revision 1.1  1992/05/20  21:32:41  furnish
 * Initial checkin of the whole PLPLOT project.
 *
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

#include "plplot.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "drivers.h"
#include "metadefs.h"
#include "pdf.h"

/* Function prototypes */
/* INDENT OFF */

static void WriteHeader		(PLStream *);

/* Constants to determine resolution, number of pixels, etc.  Can be
   changed without affecting ability to read the metafile since they
   are stored in the header (formats 1992a and later).
*/

#define PLMETA_MAX	8191		/* About 1K dpi */

#define PLMETA_X	PLMETA_MAX	/* Number of virtual pixels in x */
#define PLMETA_Y	PLMETA_MAX	/* Number of virtual pixels in y */

static PLFLT lpage_x = 238.0;		/* Page length in x in virtual mm */
static PLFLT lpage_y = 178.0;		/* Page length in y in virtual mm */

/* top level declarations */

/* (dev) will get passed in eventually, so this looks weird right now */

static PLDev device;
static PLDev *dev = &device;

/* INDENT ON */
/*----------------------------------------------------------------------*\
* plm_init()
*
* Initialize device.
\*----------------------------------------------------------------------*/

void
plm_init(PLStream *pls)
{
    U_CHAR c = (U_CHAR) INITIALIZE;

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

/* Set up device parameters */

    dev->xold = UNDEFINED;
    dev->yold = UNDEFINED;
    dev->xmin = 0;
    dev->xmax = PLMETA_X;
    dev->ymin = 0;
    dev->ymax = PLMETA_Y;

    dev->pxlx = (dev->xmax - dev->xmin) / lpage_x;
    dev->pxly = (dev->ymax - dev->ymin) / lpage_y;

/* Forget this for now */
/*
    if (pls->pscale)
	dev->pxly = dev->pxlx * pls->aspect;
*/
    setpxl(dev->pxlx, dev->pxly);
    setphy(dev->xmin, dev->xmax, dev->ymin, dev->ymax);

/* Write Metafile header. */

    WriteHeader(pls);

/* Write initialization command. */

    plm_wr(write_1byte(pls->OutFile, c));
}

/*----------------------------------------------------------------------*\
* plm_line()
*
* Draw a line in the current color from (x1,y1) to (x2,y2).
\*----------------------------------------------------------------------*/

void
plm_line(PLStream *pls, short x1, short y1, short x2, short y2)
{
    U_CHAR c;
    U_SHORT xy[4];

/* Failsafe check */

#ifdef DEBUG
    if (x1 < dev->xmin || x1 > dev->xmax ||
	x2 < dev->xmin || x2 > dev->xmax ||
	y1 < dev->ymin || y1 > dev->ymax ||
	y2 < dev->ymin || y2 > dev->ymax) {

	printf("PLPLOT: coordinates out of bounds in driver.\n");
	printf("  Actual: (%i,%i), (%i,%i)   Bounds: (%i,%i,%i,%i)\n",
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

   Note that there is no aspect ratio scaling done here!  That would defeat
   the purpose totally (it should only be done in the final, *physical*
   coordinate system).
*/
    if (x1 == dev->xold && y1 == dev->yold) {

	c = (U_CHAR) LINETO;
	plm_wr(write_1byte(pls->OutFile, c));
	pls->bytecnt++;

	xy[0] = x2;
	xy[1] = y2;
	plm_wr(write_2nbytes(pls->OutFile, xy, 2));
	pls->bytecnt += 4;
    }
    else {
	c = (U_CHAR) LINE;
	plm_wr(write_1byte(pls->OutFile, c));
	pls->bytecnt++;

	xy[0] = x1;
	xy[1] = y1;
	xy[2] = x2;
	xy[3] = y2;
	plm_wr(write_2nbytes(pls->OutFile, xy, 4));
	pls->bytecnt += 8;
    }
    dev->xold = x2;
    dev->yold = y2;
}

/*----------------------------------------------------------------------*\
* plm_polyline()
*
* Draw a polyline in the current color.
\*----------------------------------------------------------------------*/

void
plm_polyline(PLStream *pls, short *xa, short *ya, PLINT npts)
{
    U_CHAR c = (U_CHAR) POLYLINE;

    plm_wr(write_1byte(pls->OutFile, c));
    pls->bytecnt++;

    plm_wr(write_2bytes(pls->OutFile, (U_SHORT) npts));
    pls->bytecnt += 2;

    plm_wr(write_2nbytes(pls->OutFile, (U_SHORT *) xa, npts));
    plm_wr(write_2nbytes(pls->OutFile, (U_SHORT *) ya, npts));
    pls->bytecnt += 4 * npts;

    dev->xold = xa[npts - 1];
    dev->yold = ya[npts - 1];
}

/*----------------------------------------------------------------------*\
* plm_eop()
*
* End of page.
\*----------------------------------------------------------------------*/

void
plm_eop(PLStream *pls)
{
    U_CHAR c = (U_CHAR) CLEAR;

    plm_wr(write_1byte(pls->OutFile, c));
    pls->bytecnt++;
}

/*----------------------------------------------------------------------*\
* plm_bop()
*
* Set up for the next page.
\*----------------------------------------------------------------------*/

static long bytecnt_last;

void
plm_bop(PLStream *pls)
{
    U_CHAR c = (U_CHAR) PAGE;
    U_LONG foo;
    FPOS_T cp_offset, fwbyte_offset, bwbyte_offset;

    dev->xold = UNDEFINED;
    dev->yold = UNDEFINED;

    fflush(pls->OutFile);
    if (pl_fgetpos(pls->OutFile, &cp_offset))
	plexit("plm_bop: fgetpos call failed");

/* Seek back to previous page header and write forward byte offset. */

    if (pls->lp_offset > 0) {
#ifdef DEBUG
	printf("Location: %d, seeking to: %d\n", cp_offset, pls->lp_offset);
#endif

	fwbyte_offset = pls->lp_offset + 7;
	if (pl_fsetpos(pls->OutFile, &fwbyte_offset))
	    plexit("plm_bop: fsetpos call failed");

#ifdef DEBUG
	if (pl_fgetpos(pls->OutFile, &fwbyte_offset))
	    plexit("plm_bop: fgetpos call failed");

	printf("Now at: %d, to write: %d\n", fwbyte_offset, cp_offset);
#endif

	plm_wr(write_4bytes(pls->OutFile, (U_LONG) cp_offset));
	fflush(pls->OutFile);

#ifdef DEBUG
	if (pl_fsetpos(pls->OutFile, &fwbyte_offset))
	    plexit("plm_bop: fsetpos call failed");

	plm_rd(read_4bytes(pls->OutFile, &foo));
	printf("Value read as: %d\n", foo);
#endif

	if (pl_fsetpos(pls->OutFile, &cp_offset))
	    plexit("plm_bop: fsetpos call failed");
    }

/* Start next family file if necessary. */

    plGetFam(pls);

/* Write new page header */

    pls->page++;
    bwbyte_offset = pls->lp_offset;

    plm_wr(write_1byte(pls->OutFile, c));
    plm_wr(write_2bytes(pls->OutFile, (U_SHORT) pls->page));
    plm_wr(write_4bytes(pls->OutFile, (U_LONG) bwbyte_offset));
    plm_wr(write_4bytes(pls->OutFile, (U_LONG) 0));

    pls->bytecnt += 11;
    pls->lp_offset = cp_offset;
}

/*----------------------------------------------------------------------*\
* plm_tidy()
*
* Close graphics file
\*----------------------------------------------------------------------*/

void
plm_tidy(PLStream *pls)
{
    U_CHAR c = (U_CHAR) CLOSE;

    plm_wr(write_1byte(pls->OutFile, c));
    pls->bytecnt++;

    fclose(pls->OutFile);
    pls->fileset = 0;
    pls->page = 0;
    pls->OutFile = NULL;
}

/*----------------------------------------------------------------------*\
* plm_color()
*
* Set pen color.
\*----------------------------------------------------------------------*/

void
plm_color(PLStream *pls)
{
    U_CHAR c = (U_CHAR) NEW_COLOR;

    plm_wr(write_1byte(pls->OutFile, c));
    pls->bytecnt++;

    plm_wr(write_1byte(pls->OutFile, (U_CHAR) pls->icol0));
    pls->bytecnt++;

    if (pls->icol0 == PL_RGB_COLOR) {
	plm_wr(write_1byte(pls->OutFile, pls->curcolor.r));
	plm_wr(write_1byte(pls->OutFile, pls->curcolor.g));
	plm_wr(write_1byte(pls->OutFile, pls->curcolor.b));
    }

}

/*----------------------------------------------------------------------*\
* plm_text()
*
* Switch to text mode.
\*----------------------------------------------------------------------*/

void
plm_text(PLStream *pls)
{
}

/*----------------------------------------------------------------------*\
* plm_graph()
*
* Switch to graphics mode.
\*----------------------------------------------------------------------*/

void
plm_graph(PLStream *pls)
{
}

/*----------------------------------------------------------------------*\
* plm_width()
*
* Set pen width.
\*----------------------------------------------------------------------*/

void
plm_width(PLStream *pls)
{
    U_CHAR c = (U_CHAR) NEW_WIDTH;

    plm_wr(write_1byte(pls->OutFile, c));
    pls->bytecnt++;

    plm_wr(write_2bytes(pls->OutFile, (U_SHORT) (pls->width)));
    pls->bytecnt += 2;
}

/*----------------------------------------------------------------------*\
* plm_esc()
*
* Escape function.  Note that any data written must be in device
* independent form to maintain the transportability of the metafile.
*
* Functions:
*
* PL_SET_LPB	  Writes local plot bounds
\*----------------------------------------------------------------------*/

void
plm_esc(PLStream *pls, PLINT op, char *ptr)
{
    U_CHAR c = (U_CHAR) ESCAPE;
    U_CHAR opc;

    plm_wr(write_1byte(pls->OutFile, c));
    pls->bytecnt++;

    opc = (U_CHAR) op;
    plm_wr(write_1byte(pls->OutFile, opc));
    pls->bytecnt++;

    switch (op) {

      case PL_SET_LPB:
	plm_wr(write_2bytes(pls->OutFile, (U_SHORT) (pls->lpbpxmi)));
	plm_wr(write_2bytes(pls->OutFile, (U_SHORT) (pls->lpbpxma)));
	plm_wr(write_2bytes(pls->OutFile, (U_SHORT) (pls->lpbpymi)));
	plm_wr(write_2bytes(pls->OutFile, (U_SHORT) (pls->lpbpyma)));
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
    plm_wr(write_header(pls->OutFile, PLMETA_HEADER));
    plm_wr(write_header(pls->OutFile, PLMETA_VERSION));

/* Write initialization info.  Tag via strings to make backward
   compatibility with old metafiles as easy as possible. */

    plm_wr(write_header(pls->OutFile, "xmin"));
    plm_wr(write_2bytes(pls->OutFile, (U_SHORT) dev->xmin));

    plm_wr(write_header(pls->OutFile, "xmax"));
    plm_wr(write_2bytes(pls->OutFile, (U_SHORT) dev->xmax));

    plm_wr(write_header(pls->OutFile, "ymin"));
    plm_wr(write_2bytes(pls->OutFile, (U_SHORT) dev->ymin));

    plm_wr(write_header(pls->OutFile, "ymax"));
    plm_wr(write_2bytes(pls->OutFile, (U_SHORT) dev->ymax));

    plm_wr(write_header(pls->OutFile, "pxlx"));
    plm_wr(write_ieeef(pls->OutFile, (float) dev->pxlx));

    plm_wr(write_header(pls->OutFile, "pxly"));
    plm_wr(write_ieeef(pls->OutFile, (float) dev->pxly));

    plm_wr(write_header(pls->OutFile, "aspect"));
    plm_wr(write_ieeef(pls->OutFile, (float) pls->aspect));

    plm_wr(write_header(pls->OutFile, "orient"));
    plm_wr(write_1byte(pls->OutFile, (U_CHAR) (pls->orient)));

    plm_wr(write_header(pls->OutFile, ""));
}

#else
int 
pldummy_plmeta()
{
    return 0;
}

#endif				/* PLMETA */
