/* $Id$
   $Log$
   Revision 1.1  1992/05/20 21:32:41  furnish
   Initial checkin of the whole PLPLOT project.

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

    The Geoffrey Furnish Standard Disclaimer:
    "I hate any C compiler that isn't ANSI compliant, and I refuse to waste
    my time trying to support such garbage.  If you can't compile with an
    ANSI compiler, then don't expect this to work.  No appologies,
    now or ever."

    Garbage support added by Maurice LeBrun :-).
*/
#ifdef PLMETA

#include <stdio.h>
#include "plplot.h"
#include "dispatch.h"
#include "metadefs.h"
#include "pdf.h"

/* Function prototypes */

static void plmesc_rgb		PLARGS((PLStream *, char *));
static void plmesc_ancol	PLARGS((PLStream *, char *));
static void WriteHeader		PLARGS((PLStream *));

/* Constants to determine resolution, number of pixels, etc.  Can be
   changed without affecting ability to read the metafile since they
   are stored in the header (formats 1992a and later).
*/

#define PLMETA_MAX	32767

#define PLMETA_X	PLMETA_MAX	/* Number of virtual pixels in x */
#define PLMETA_Y	PLMETA_MAX	/* Number of virtual pixels in y */

static PLFLT lpage_x = 238.0;		/* Page length in x in virtual mm */
static PLFLT lpage_y = 178.0;		/* Page length in y in virtual mm */

/* top level declarations */

/* (dev) will get passed in eventually, so this looks weird right now */

static PLDev device;
static PLDev *dev = &device;

/*----------------------------------------------------------------------*\
* plminit()
*
* Initialize device.
\*----------------------------------------------------------------------*/

void 
plminit(pls)
PLStream *pls;
{
    U_CHAR c = (U_CHAR) INITIALIZE;

    pls->termin = 0;		/* not an interactive terminal */
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
* plmline()
*
* Draw a line in the current color from (x1,y1) to (x2,y2).
\*----------------------------------------------------------------------*/

void 
plmline(pls, x1, y1, x2, y2)
PLStream *pls;
PLINT x1, y1, x2, y2;
{
    U_CHAR c;

/* Failsafe check */

    if (x1 < dev->xmin || x1 > dev->xmax ||
	x2 < dev->xmin || x2 > dev->xmax ||
	y1 < dev->ymin || y1 > dev->ymax ||
	y2 < dev->ymin || y2 > dev->ymax) {
	printf("PLPLOT: coordinates out of bounds in driver.\n");
	printf("  Actual: (%i,%i), (%i,%i)   Bounds: (%i,%i,%i,%i)\n",
		x1, y1, x2, y2, dev->xmin, dev->xmax, dev->ymin, dev->ymax);
    }

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

	plm_wr(write_2bytes(pls->OutFile, (U_SHORT) x2));
	plm_wr(write_2bytes(pls->OutFile, (U_SHORT) y2));
	pls->bytecnt += 4;
    }
    else {
	c = (U_CHAR) LINE;
	plm_wr(write_1byte(pls->OutFile, c));
	pls->bytecnt++;

	plm_wr(write_2bytes(pls->OutFile, (U_SHORT) x1));
	plm_wr(write_2bytes(pls->OutFile, (U_SHORT) y1));
	plm_wr(write_2bytes(pls->OutFile, (U_SHORT) x2));
	plm_wr(write_2bytes(pls->OutFile, (U_SHORT) y2));
	pls->bytecnt += 8;
    }
    dev->xold = x2;
    dev->yold = y2;
}

/*----------------------------------------------------------------------*\
* plmclear()
*
* Clear page.
\*----------------------------------------------------------------------*/

void 
plmclear(pls)
PLStream *pls;
{
    U_CHAR c = (U_CHAR) CLEAR;

    plm_wr(write_1byte(pls->OutFile, c));
    pls->bytecnt++;
}

/*----------------------------------------------------------------------*\
* plmpage()
*
* Set up for the next page.
* Also write: 
*     - The page number for the following page (not strictly necessary 
*	but the redundancy may someday be useful).
*     - A blank field after the command to eventually hold the byte
*	distance to the next page.
\*----------------------------------------------------------------------*/

void 
plmpage(pls)
PLStream *pls;
{
    U_CHAR c = (U_CHAR) PAGE;

    dev->xold = UNDEFINED;
    dev->yold = UNDEFINED;

    plGetFam(pls);
    pls->page++;

    plm_wr(write_1byte(pls->OutFile, c));
    plm_wr(write_2bytes(pls->OutFile, (U_SHORT) pls->page));
    plm_wr(write_2bytes(pls->OutFile, (U_SHORT) (0)));

    pls->bytecnt += 5;
}

/*----------------------------------------------------------------------*\
* plmadv()
*
* Advance to the next page.
* Also write page information as in plmpage().
\*----------------------------------------------------------------------*/

void 
plmadv(pls)
PLStream *pls;
{
    U_CHAR c = (U_CHAR) ADVANCE;

    dev->xold = UNDEFINED;
    dev->yold = UNDEFINED;

    plGetFam(pls);
    pls->page++;

    plm_wr(write_1byte(pls->OutFile, c));
    plm_wr(write_2bytes(pls->OutFile, (U_SHORT) pls->page));
    plm_wr(write_2bytes(pls->OutFile, (U_SHORT) (0)));

    pls->bytecnt += 5;
}

/*----------------------------------------------------------------------*\
* plmtidy()
*
* Close graphics file
\*----------------------------------------------------------------------*/

void 
plmtidy (pls)
PLStream *pls;
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
* plmcolor()
*
* Set pen color.
\*----------------------------------------------------------------------*/

void 
plmcolor(pls)
PLStream *pls;
{
    U_CHAR c = (U_CHAR) NEW_COLOR;

    plm_wr(write_1byte(pls->OutFile, c));
    pls->bytecnt++;

    plm_wr(write_2bytes(pls->OutFile, (U_SHORT) (pls->color)));
    pls->bytecnt += 2;
}

/*----------------------------------------------------------------------*\
* plmtext()
*
* Switch to text mode.
\*----------------------------------------------------------------------*/

void 
plmtext(pls)
PLStream *pls;
{
    U_CHAR c = (U_CHAR) SWITCH_TO_TEXT;

    plm_wr(write_1byte(pls->OutFile, c));
    pls->bytecnt++;
}

/*----------------------------------------------------------------------*\
* plmgraph()
*
* Switch to graphics mode.
\*----------------------------------------------------------------------*/

void 
plmgraph(pls)
PLStream *pls;
{
    U_CHAR c = (U_CHAR) SWITCH_TO_GRAPH;

    plm_wr(write_1byte(pls->OutFile, c));
    pls->bytecnt++;
}

/*----------------------------------------------------------------------*\
* plmwidth()
*
* Set pen width.
\*----------------------------------------------------------------------*/

void
plmwidth (pls)
PLStream *pls;
{
    U_CHAR c = (U_CHAR) NEW_WIDTH;

    plm_wr(write_1byte(pls->OutFile, c));
    pls->bytecnt++;

    plm_wr(write_2bytes(pls->OutFile, (U_SHORT) (pls->width)));
    pls->bytecnt += 2;
}

/*----------------------------------------------------------------------*\
* plmesc()
*
* Escape function.  Note that any data written must be in device
* independent form to maintain the transportability of the metafile.
*
* Functions:
*
* PL_SET_RGB	  Writes three data values for R, G, B content
* PL_ALLOC_NCOL	  Writes color table allocation info
* PL_SET_LPB	  Writes local plot bounds
\*----------------------------------------------------------------------*/

void 
plmesc(pls, op, ptr)
PLStream *pls;
PLINT op;
char *ptr;
{
    U_CHAR c = (U_CHAR) ESCAPE;
    U_CHAR opc;

    plm_wr(write_1byte(pls->OutFile, c));
    pls->bytecnt++;

    opc = (U_CHAR) op;
    plm_wr(write_1byte(pls->OutFile, opc));
    pls->bytecnt++;

    switch (op) {

    case PL_SET_RGB:
	plmesc_rgb(pls, ptr);
	break;

    case PL_ALLOC_NCOL:
	plmesc_ancol(pls, ptr);
	break;

    case PL_SET_LPB:
	plm_wr(write_2bytes(pls->OutFile, (U_SHORT) (pls->lpbpxmi)));
	plm_wr(write_2bytes(pls->OutFile, (U_SHORT) (pls->lpbpxma)));
	plm_wr(write_2bytes(pls->OutFile, (U_SHORT) (pls->lpbpymi)));
	plm_wr(write_2bytes(pls->OutFile, (U_SHORT) (pls->lpbpyma)));
	break;
    }
}

/*----------------------------------------------------------------------*\
* Support routines
\*----------------------------------------------------------------------*/

/*----------------------------------------------------------------------*\
* plmesc_rgb()
*
* Writes RGB as 16 bits each.
\*----------------------------------------------------------------------*/

static void 
plmesc_rgb(pls, ptr)
PLStream *pls;
char *ptr;
{
    pleRGB *cols = (pleRGB *) ptr;
    U_SHORT ired, igreen, iblue;

    ired   = MIN(65535, MAX(0, (U_SHORT) (65535. * cols->red)));
    igreen = MIN(65535, MAX(0, (U_SHORT) (65535. * cols->green)));
    iblue  = MIN(65535, MAX(0, (U_SHORT) (65535. * cols->blue)));

    plm_wr(write_2bytes(pls->OutFile, ired));
    plm_wr(write_2bytes(pls->OutFile, igreen));
    plm_wr(write_2bytes(pls->OutFile, iblue));
    pls->bytecnt += 6;
}

/*----------------------------------------------------------------------*\
* plmesc_ancol()
*
* Writes color allocation table info.
\*----------------------------------------------------------------------*/

static void 
plmesc_ancol(pls, ptr)
PLStream *pls;
char *ptr;
{
    U_CHAR icolor;
    char *name;
    pleNcol *col = (pleNcol *) ptr;

    icolor = col->icolor;
    name = col->name;

    plm_wr(write_1byte(pls->OutFile, icolor));
    plm_wr(write_header(pls->OutFile, name));

    pls->bytecnt += 2 + strlen(name);
}

/*----------------------------------------------------------------------*\
* WriteHeader()
*
* Writes a PLPLOT Metafile header.
\*----------------------------------------------------------------------*/

static void 
WriteHeader(pls)
PLStream *pls;
{
    plm_wr(write_header(pls->OutFile, PLPLOT_HEADER));
    plm_wr(write_header(pls->OutFile, PLPLOT_VERSION));

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
#endif	/* PLMETA */
