/* $Id$
   $Log$
   Revision 1.10  1993/07/16 22:12:17  mjl
   Simplified and slightly changed behavior to support plot dumps to cloned
   plplot streams.

 * Revision 1.9  1993/07/01  21:59:42  mjl
 * Changed all plplot source files to include plplotP.h (private) rather than
 * plplot.h.  Rationalized namespace -- all externally-visible plplot functions
 * now start with "pl"; device driver functions start with "plD_".
 *
 * Revision 1.8  1993/03/15  21:39:16  mjl
 * Changed all _clear/_page driver functions to the names _eop/_bop, to be
 * more representative of what's actually going on.
 *
 * Revision 1.7  1993/03/06  04:59:22  mjl
 * Changed the way the temporary plot buffer file is opened.  Should fix
 * X-window refresh problem observed under SunOS.
 *
 * Revision 1.6  1993/03/03  19:42:04  mjl
 * Changed PLSHORT -> short everywhere; now all device coordinates are expected
 * to fit into a 16 bit address space (reasonable, and good for performance).
 *
 * Revision 1.5  1993/02/27  04:46:39  mjl
 * Fixed errors in ordering of header file inclusion.  "plplotP.h" should
 * always be included first.
 *
 * Revision 1.4  1993/02/25  18:29:08  mjl
 * Changed buffer code to use only one temporary file per plplot invocation &
 * rewinding it as needed, instead of opening/closing a temporary file for each
 * page.  Fixed bug in redisplaying backed up page.
 *
 * Revision 1.3  1993/02/22  23:11:00  mjl
 * Eliminated the plP_adv() driver calls, as these were made obsolete by
 * recent changes to plmeta and plrender.  Also eliminated page clear commands
 * from plP_tidy() -- plend now calls plP_clr() and plP_tidy() explicitly.
 *
 * Revision 1.2  1993/01/23  05:41:49  mjl
 * Changes to support new color model, polylines, and event handler support
 * (interactive devices only).
 *
 * Revision 1.1  1992/11/07  07:57:09  mjl
 * Routines for writing to and reading from a plot buffer and recreating
 * an entire plot.  The driver need merely specify pls->plbuf_write = 1 for it
 * to be used (it must of course handle the expose/resize events itself).
 *
*/

/*
    plbuf.c -- handle plot buffer.

    Copyright 1992
    Maurice LeBrun

    This software may be freely copied, modified and redistributed without
    fee provided that this copyright notice is preserved intact on all
    copies and modified copies.

    There is no warranty or other guarantee of fitness of this software.
    It is provided solely "as is". The author(s) disclaim(s) all
    responsibility and liability with respect to this software's usage or
    its effect upon hardware or computer systems.

*/

#include "plplotP.h"
#include <stdio.h>
#include <string.h>

#include "drivers.h"
#include "pdf.h"
#include "metadefs.h"

/* Function prototypes */
/* INDENT OFF */

static int	rd_command	( PLStream *pls, U_CHAR * );
static int	wr_command	( PLStream *pls, U_CHAR );
static void	process_next	( PLStream *pls, U_CHAR );

void rdbuf_init		(PLStream *);
void rdbuf_line		(PLStream *);
void rdbuf_polyline	(PLStream *);
void rdbuf_eop		(PLStream *);
void rdbuf_bop		(PLStream *);
void rdbuf_tidy		(PLStream *);
void rdbuf_color	(PLStream *);
void rdbuf_text		(PLStream *);
void rdbuf_graph	(PLStream *);
void rdbuf_width	(PLStream *);
void rdbuf_esc		(PLStream *);

/* Static variables */

static short xpoly[PL_MAXPOLYLINE], ypoly[PL_MAXPOLYLINE];

/* INDENT ON */
/*----------------------------------------------------------------------*\
* Routines to write to the plot buffer.
\*----------------------------------------------------------------------*/

/*----------------------------------------------------------------------*\
* plbuf_init()
*
* Initialize device.
* Allow for the possibility that multiple streams are sharing a single
* plot buffer (one stream may be cloned).
\*----------------------------------------------------------------------*/

void
plbuf_init(PLStream *pls)
{
    dbug_enter("plbuf_init");

    if (pls->plbufFile == NULL) {
	pls->plbufFile = tmpfile();
	pls->plbufOwner = 1;
	if (pls->plbufFile == NULL) {
	    plexit("plbuf_init: Error opening plot data storage file.");
	}
    }
}

/*----------------------------------------------------------------------*\
* plbuf_line()
*
* Draw a line in the current color from (x1,y1) to (x2,y2).
\*----------------------------------------------------------------------*/

void
plbuf_line(PLStream *pls, short x1a, short y1a, short x2a, short y2a)
{
    dbug_enter("plbuf_line");

    (void) wr_command(pls, (U_CHAR) LINE);

    xpoly[0] = x1a;
    xpoly[1] = x2a;
    ypoly[0] = y1a;
    ypoly[1] = y2a;

    (void) fwrite(xpoly, sizeof(short), 2, pls->plbufFile);
    (void) fwrite(ypoly, sizeof(short), 2, pls->plbufFile);
}

/*----------------------------------------------------------------------*\
* plbuf_polyline()
*
* Draw a polyline in the current color.
\*----------------------------------------------------------------------*/

void
plbuf_polyline(PLStream *pls, short *xa, short *ya, PLINT npts)
{
    PLINT i;

    dbug_enter("plbuf_polyline");

    (void) wr_command(pls, (U_CHAR) POLYLINE);
    (void) fwrite(&npts, sizeof(PLINT), 1, pls->plbufFile);

    for (i = 0; i < npts; i++) {
	xpoly[i] = xa[i];
	ypoly[i] = ya[i];
    }
    (void) fwrite(xpoly, sizeof(short), npts, pls->plbufFile);
    (void) fwrite(ypoly, sizeof(short), npts, pls->plbufFile);
}

/*----------------------------------------------------------------------*\
* plbuf_eop()
*
* End of page.
\*----------------------------------------------------------------------*/

void
plbuf_eop(PLStream *pls)
{
    dbug_enter("plbuf_eop");

    (void) wr_command(pls, (U_CHAR) CLEAR);
}

/*----------------------------------------------------------------------*\
* plbuf_bop()
*
* Set up for the next page.
* Also write state information to ensure the next page is correct.
\*----------------------------------------------------------------------*/

void
plbuf_bop(PLStream *pls)
{
    dbug_enter("plbuf_bop");

    rewind(pls->plbufFile);
    (void) wr_command(pls, (U_CHAR) PAGE);
    plP_col();
    plP_wid();
}

/*----------------------------------------------------------------------*\
* plbuf_tidy()
*
* Close graphics file
\*----------------------------------------------------------------------*/

void
plbuf_tidy(PLStream *pls)
{
    dbug_enter("plbuf_tidy");

    if (pls->plbufOwner)
	fclose(pls->plbufFile);
}

/*----------------------------------------------------------------------*\
* plbuf_color()
*
* Set pen color.
\*----------------------------------------------------------------------*/

void
plbuf_color(PLStream *pls)
{
    U_CHAR icol0 = pls->icol0;
    U_CHAR r = pls->curcolor.r;
    U_CHAR g = pls->curcolor.g;
    U_CHAR b = pls->curcolor.b;

    dbug_enter("plbuf_color");

    (void) wr_command(pls, (U_CHAR) NEW_COLOR);
    (void) fwrite(&icol0, sizeof(U_CHAR), 1, pls->plbufFile);
    if (icol0 == PL_RGB_COLOR) {
	(void) fwrite(&r, sizeof(U_CHAR), 1, pls->plbufFile);
	(void) fwrite(&g, sizeof(U_CHAR), 1, pls->plbufFile);
	(void) fwrite(&b, sizeof(U_CHAR), 1, pls->plbufFile);
    }
}

/*----------------------------------------------------------------------*\
* plbuf_text()
*
* Switch to text mode.
\*----------------------------------------------------------------------*/

void
plbuf_text(PLStream *pls)
{
    dbug_enter("plbuf_text");

    (void) wr_command(pls, (U_CHAR) SWITCH_TO_TEXT);
}

/*----------------------------------------------------------------------*\
* plbuf_graph()
*
* Switch to graphics mode.
\*----------------------------------------------------------------------*/

void
plbuf_graph(PLStream *pls)
{
    dbug_enter("plbuf_graph");

    (void) wr_command(pls, (U_CHAR) SWITCH_TO_GRAPH);
}

/*----------------------------------------------------------------------*\
* plbuf_width()
*
* Set pen width.
\*----------------------------------------------------------------------*/

void
plbuf_width(PLStream *pls)
{
    U_CHAR width = pls->width;

    dbug_enter("plbuf_width");

    (void) wr_command(pls, (U_CHAR) NEW_WIDTH);
    (void) fwrite(&width, sizeof(U_CHAR), 1, pls->plbufFile);
}

/*----------------------------------------------------------------------*\
* plbuf_esc()
*
* Escape function.  Note that any data written must be in device
* independent form to maintain the transportability of the metafile.
*
* Functions:
*
* PL_SET_LPB	  Writes local plot bounds
\*----------------------------------------------------------------------*/

void
plbuf_esc(PLStream *pls, PLINT op, void *ptr)
{
    dbug_enter("plbuf_esc");

    (void) wr_command(pls, (U_CHAR) ESCAPE);
    (void) wr_command(pls, (U_CHAR) op);

    switch (op) {
    case 0:
	break;
    }
}

/*----------------------------------------------------------------------*\
* Routines to read from & process the plot buffer.
\*----------------------------------------------------------------------*/

/*----------------------------------------------------------------------*\
* rdbuf_init()
*
* Initialize device.
\*----------------------------------------------------------------------*/

void
rdbuf_init(PLStream *pls)
{
    dbug_enter("rdbuf_init");
}

/*----------------------------------------------------------------------*\
* rdbuf_line()
*
* Draw a line in the current color from (x1,y1) to (x2,y2).
\*----------------------------------------------------------------------*/

void
rdbuf_line(PLStream *pls)
{
    dbug_enter("rdbuf_line");

    (void) fread(xpoly, sizeof(short), 2, pls->plbufFile);
    (void) fread(ypoly, sizeof(short), 2, pls->plbufFile);

    plP_line(xpoly, ypoly);
}

/*----------------------------------------------------------------------*\
* rdbuf_polyline()
*
* Draw a polyline in the current color.
\*----------------------------------------------------------------------*/

void
rdbuf_polyline(PLStream *pls)
{
    PLINT npts;

    dbug_enter("rdbuf_polyline");

    (void) fread(&npts, sizeof(PLINT), 1, pls->plbufFile);
    (void) fread(xpoly, sizeof(short), npts, pls->plbufFile);
    (void) fread(ypoly, sizeof(short), npts, pls->plbufFile);

    plP_polyline(xpoly, ypoly, npts);
}

/*----------------------------------------------------------------------*\
* rdbuf_eop()
*
* End of page.
\*----------------------------------------------------------------------*/

void
rdbuf_eop(PLStream *pls)
{
    dbug_enter("rdbuf_eop");
}

/*----------------------------------------------------------------------*\
* rdbuf_bop()
*
* Set up for the next page.
\*----------------------------------------------------------------------*/

void
rdbuf_bop(PLStream *pls)
{
    dbug_enter("rdbuf_bop");
}

/*----------------------------------------------------------------------*\
* rdbuf_tidy()
*
* Close graphics file
\*----------------------------------------------------------------------*/

void
rdbuf_tidy(PLStream *pls)
{
    dbug_enter("rdbuf_tidy");
}

/*----------------------------------------------------------------------*\
* rdbuf_color()
*
* Set pen color.
\*----------------------------------------------------------------------*/

void
rdbuf_color(PLStream *pls)
{
    U_CHAR icol0, r, g, b;

    dbug_enter("rdbuf_color");

    (void) fread(&icol0, sizeof(U_CHAR), 1, pls->plbufFile);
    if (icol0 == PL_RGB_COLOR) {
	(void) fread(&r, sizeof(U_CHAR), 1, pls->plbufFile);
	(void) fread(&g, sizeof(U_CHAR), 1, pls->plbufFile);
	(void) fread(&b, sizeof(U_CHAR), 1, pls->plbufFile);
    }
    else {
	if ((int) icol0 > 15) {
	    plwarn("rdbuf_color: Color table entry hosed");
	    icol0 = 1;
	}
	r = pls->cmap0[icol0].r;
	g = pls->cmap0[icol0].g;
	b = pls->cmap0[icol0].b;
    }
    pls->icol0 = icol0;
    pls->curcolor.r = r;
    pls->curcolor.g = g;
    pls->curcolor.b = b;

    plP_col();
}

/*----------------------------------------------------------------------*\
* rdbuf_text()
*
* Switch to text mode.
\*----------------------------------------------------------------------*/

void
rdbuf_text(PLStream *pls)
{
    dbug_enter("rdbuf_text");

    plP_text();
}

/*----------------------------------------------------------------------*\
* rdbuf_graph()
*
* Switch to graphics mode.
\*----------------------------------------------------------------------*/

void
rdbuf_graph(PLStream *pls)
{
    dbug_enter("rdbuf_graph");

    plP_gra();
}

/*----------------------------------------------------------------------*\
* rdbuf_width()
*
* Set pen width.
\*----------------------------------------------------------------------*/

void
rdbuf_width(PLStream *pls)
{
    U_CHAR width;

    dbug_enter("rdbuf_width");

    (void) fread(&width, sizeof(U_CHAR), 1, pls->plbufFile);
    pls->width = width;
    plP_wid();
}

/*----------------------------------------------------------------------*\
* rdbuf_esc()
*
* Escape function.
* Must fill data structure with whatever data that was written,
* then call escape function.
*
* Functions:
*
\*----------------------------------------------------------------------*/

void
rdbuf_esc(PLStream *pls)
{
    U_CHAR op;
    void *ptr = NULL;

    dbug_enter("rdbuf_esc");

    (void) fread(&op, sizeof(U_CHAR), 1, pls->plbufFile);

    switch (op) {
      case 0:
	break;
    }

    plP_esc(op, ptr);
}

/*----------------------------------------------------------------------*\
* plRemakePlot()
*
* Rebuilds plot from plot buffer, usually in response to a window
* resize or exposure event.
\*----------------------------------------------------------------------*/

void
plRemakePlot(PLStream *pls)
{
    U_CHAR c;
    int plbuf_status;

    dbug_enter("plRemakePlot");

    if (pls->plbufFile == NULL)
	return;

    fflush(pls->plbufFile);
    rewind(pls->plbufFile);

    plbuf_status = pls->plbuf_write;
    pls->plbuf_write = FALSE;

    while (rd_command(pls, &c)) {
	if (c == CLEAR)
	    break;
	process_next(pls, c);
    }

    pls->plbuf_write = plbuf_status;
}

/*----------------------------------------------------------------------*\
* process_next()
*
* Processes commands read from the plot buffer.
\*----------------------------------------------------------------------*/

static void
process_next(PLStream *pls, U_CHAR c)
{
    dbug_enter("process_next");

    switch ((int) c) {

      case INITIALIZE:
	rdbuf_init(pls);
	break;

      case SWITCH_TO_TEXT:
	rdbuf_text(pls);
	break;

      case SWITCH_TO_GRAPH:
	rdbuf_graph(pls);
	break;

      case CLEAR:
	rdbuf_eop(pls);
	break;

      case PAGE:
	rdbuf_bop(pls);
	break;

      case NEW_COLOR:
	rdbuf_color(pls);
	break;

      case NEW_WIDTH:
	rdbuf_width(pls);
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
	plwarn("process_next: Unrecognized plot buffer command\n");
    }
}

/*----------------------------------------------------------------------*\
*  rd_command()
*
*  Read & return the next command
\*----------------------------------------------------------------------*/

static int
rd_command(PLStream *pls, U_CHAR *p_c)
{
    int count;
    
    count = fread(p_c, sizeof(U_CHAR), 1, pls->plbufFile);
#ifdef DEBUG
    if (count == 0) {
	plP_text();
	fprintf(stderr, "Cannot read from plot buffer\n");
	plP_gra();
    }
#endif
    return (count);
}

/*----------------------------------------------------------------------*\
*  wr_command()
*
*  Write the next command
\*----------------------------------------------------------------------*/

static int
wr_command(PLStream *pls, U_CHAR c)
{
    U_CHAR c1 = c;
    int count;

    count = fwrite(&c1, sizeof(U_CHAR), 1, pls->plbufFile);
#ifdef DEBUG
    if (count == 0) {
	plP_text();
	fprintf(stderr, "Cannot write to plot buffer\n");
	plP_gra();
    }
#endif
    return (count);
}
