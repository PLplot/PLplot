/* $Id$
   $Log$
   Revision 1.4  1993/02/25 18:29:08  mjl
   Changed buffer code to use only one temporary file per plplot invocation &
   rewinding it as needed, instead of opening/closing a temporary file for each
   page.  Fixed bug in redisplaying backed up page.

 * Revision 1.3  1993/02/22  23:11:00  mjl
 * Eliminated the gradv() driver calls, as these were made obsolete by
 * recent changes to plmeta and plrender.  Also eliminated page clear commands
 * from grtidy() -- plend now calls grclr() and grtidy() explicitly.
 *
 * Revision 1.2  1993/01/23  05:41:49  mjl
 * Changes to support new color model, polylines, and event handler support
 * (interactive devices only).
 *
 * Revision 1.1  1992/11/07  07:57:09  mjl
 * Routines for writing to and reading from a plot buffer, as well as recreating
 * an entire plot.  The driver need merely specify pls->plbuf_enable = 1 for it
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

#include <stdio.h>
#include <string.h>

#include "plplot.h"
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
void rdbuf_clear	(PLStream *);
void rdbuf_page		(PLStream *);
void rdbuf_tidy		(PLStream *);
void rdbuf_color	(PLStream *);
void rdbuf_text		(PLStream *);
void rdbuf_graph	(PLStream *);
void rdbuf_width	(PLStream *);
void rdbuf_esc		(PLStream *);

/* Static variables */

static PLSHORT xy[4];
static PLSHORT xpoly[PL_MAXPOLYLINE], ypoly[PL_MAXPOLYLINE];

/* INDENT ON */
/*----------------------------------------------------------------------*\
* Routines to write to the plot buffer.
\*----------------------------------------------------------------------*/

/*----------------------------------------------------------------------*\
* plbuf_init()
*
* Initialize device.
\*----------------------------------------------------------------------*/

void
plbuf_init(PLStream *pls)
{
    (void) tmpnam(pls->plbufFnam);
    pls->plbufFile = fopen(pls->plbufFnam, "wb+");
    if (pls->plbufFile == NULL) {
	plexit("Error opening plot data storage file.");
    }
}

/*----------------------------------------------------------------------*\
* plbuf_line()
*
* Draw a line in the current color from (x1,y1) to (x2,y2).
\*----------------------------------------------------------------------*/

void
plbuf_line(PLStream *pls, PLSHORT x1a, PLSHORT y1a, PLSHORT x2a, PLSHORT y2a)
{
    if (pls->plbuf_read)
	return;

    if (pls->plbuf_write) {
	(void) wr_command(pls, (U_CHAR) LINE);

	xy[0] = x1a;
	xy[1] = y1a;
	xy[2] = x2a;
	xy[3] = y2a;
	(void) fwrite(xy, sizeof(PLSHORT), 4, pls->plbufFile);
    }
}

/*----------------------------------------------------------------------*\
* plbuf_polyline()
*
* Draw a polyline in the current color.
\*----------------------------------------------------------------------*/

void
plbuf_polyline(PLStream *pls, PLSHORT *xa, PLSHORT *ya, PLINT npts)
{
    PLINT i;

    if (pls->plbuf_read)
	return;

    if (pls->plbuf_write) {
	(void) wr_command(pls, (U_CHAR) POLYLINE);
	(void) fwrite(&npts, sizeof(PLINT), 1, pls->plbufFile);

	for (i = 0; i < npts; i++) {
	    xpoly[i] = xa[i];
	    ypoly[i] = ya[i];
	}
	(void) fwrite(xpoly, sizeof(PLSHORT), npts, pls->plbufFile);
	(void) fwrite(ypoly, sizeof(PLSHORT), npts, pls->plbufFile);
    }
}

/*----------------------------------------------------------------------*\
* plbuf_clear()
*
* Clear page.
\*----------------------------------------------------------------------*/

void
plbuf_clear(PLStream *pls)
{
    if (pls->plbuf_read)
	return;

    if (pls->plbuf_write)
	(void) wr_command(pls, (U_CHAR) CLEAR);
}

/*----------------------------------------------------------------------*\
* plbuf_page()
*
* Set up for the next page.
* Also write state information to ensure the next page is correct.
\*----------------------------------------------------------------------*/

void
plbuf_page(PLStream *pls)
{
    if (pls->plbuf_read)
	return;

    pls->plbuf_write = pls->plbuf_enable;

    if (pls->plbuf_write) {
	rewind(pls->plbufFile);
	(void) wr_command(pls, (U_CHAR) PAGE);
	grcol();
	grwid();
    }
}

/*----------------------------------------------------------------------*\
* plbuf_tidy()
*
* Close graphics file
\*----------------------------------------------------------------------*/

void
plbuf_tidy(PLStream *pls)
{
    fclose(pls->plbufFile);
    remove(pls->plbufFnam);
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

    if (pls->plbuf_read)
	return;

    if (pls->plbuf_write) {
	(void) wr_command(pls, (U_CHAR) NEW_COLOR);
	(void) fwrite(&icol0, sizeof(U_CHAR), 1, pls->plbufFile);
	if (icol0 == PL_RGB_COLOR) {
	    (void) fwrite(&r, sizeof(U_CHAR), 1, pls->plbufFile);
	    (void) fwrite(&g, sizeof(U_CHAR), 1, pls->plbufFile);
	    (void) fwrite(&b, sizeof(U_CHAR), 1, pls->plbufFile);
	}
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
    if (pls->plbuf_read)
	return;

    if (pls->plbuf_write) {
	(void) wr_command(pls, (U_CHAR) SWITCH_TO_TEXT);
    }
}

/*----------------------------------------------------------------------*\
* plbuf_graph()
*
* Switch to graphics mode.
\*----------------------------------------------------------------------*/

void
plbuf_graph(PLStream *pls)
{
    if (pls->plbuf_read)
	return;

    if (pls->plbuf_write) {
	(void) wr_command(pls, (U_CHAR) SWITCH_TO_GRAPH);
    }
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

    if (pls->plbuf_read)
	return;

    if (pls->plbuf_write) {
	(void) wr_command(pls, (U_CHAR) NEW_WIDTH);
	(void) fwrite(&width, sizeof(U_CHAR), 1, pls->plbufFile);
    }
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
plbuf_esc(PLStream *pls, PLINT op, char *ptr)
{
    if (pls->plbuf_read)
	return;

    if (pls->plbuf_write) {
	(void) wr_command(pls, (U_CHAR) ESCAPE);
	(void) wr_command(pls, (U_CHAR) op);

	switch (op) {
	  case 0:
	    break;
	}
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
}

/*----------------------------------------------------------------------*\
* rdbuf_line()
*
* Draw a line in the current color from (x1,y1) to (x2,y2).
\*----------------------------------------------------------------------*/

void
rdbuf_line(PLStream *pls)
{
    (void) fread(xy, sizeof(PLSHORT), 4, pls->plbufFile);

    grline(xy[0], xy[1], xy[2], xy[3]);
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

    (void) fread(&npts, sizeof(PLINT), 1, pls->plbufFile);
    (void) fread(xpoly, sizeof(PLSHORT), npts, pls->plbufFile);
    (void) fread(ypoly, sizeof(PLSHORT), npts, pls->plbufFile);

    grpolyline(xpoly, ypoly, npts);
}

/*----------------------------------------------------------------------*\
* rdbuf_clear()
*
* Clear page.
\*----------------------------------------------------------------------*/

void
rdbuf_clear(PLStream *pls)
{
}

/*----------------------------------------------------------------------*\
* rdbuf_page()
*
* Set up for the next page.
\*----------------------------------------------------------------------*/

void
rdbuf_page(PLStream *pls)
{
}

/*----------------------------------------------------------------------*\
* rdbuf_tidy()
*
* Close graphics file
\*----------------------------------------------------------------------*/

void
rdbuf_tidy(PLStream *pls)
{
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

    grcol();
}

/*----------------------------------------------------------------------*\
* rdbuf_text()
*
* Switch to text mode.
\*----------------------------------------------------------------------*/

void
rdbuf_text(PLStream *pls)
{
    grtext();
}

/*----------------------------------------------------------------------*\
* rdbuf_graph()
*
* Switch to graphics mode.
\*----------------------------------------------------------------------*/

void
rdbuf_graph(PLStream *pls)
{
    grgra();
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

    (void) fread(&width, sizeof(U_CHAR), 1, pls->plbufFile);
    pls->width = width;
    grwid();
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
* PL_SET_LPB	  Reads local plot bounds
\*----------------------------------------------------------------------*/

void
rdbuf_esc(PLStream *pls)
{
    U_CHAR op;
    char *ptr = NULL;

    (void) fread(&op, sizeof(U_CHAR), 1, pls->plbufFile);

    switch (op) {
      case 0:
	break;
    }

    gresc(op, ptr);
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

    if (!pls->plbuf_write)
	return;

    rewind(pls->plbufFile);
    pls->plbuf_write = FALSE;
    pls->plbuf_read = TRUE;

    while (rd_command(pls, &c)) {
	if (c == CLEAR)
	    break;
	process_next(pls, c);
    }

    pls->plbuf_write = TRUE;
    pls->plbuf_read = FALSE;
}

/*----------------------------------------------------------------------*\
* process_next()
*
* Processes commands read from the plot buffer.
\*----------------------------------------------------------------------*/

static void
process_next(PLStream *pls, U_CHAR c)
{
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
	rdbuf_clear(pls);
	break;

      case PAGE:
	rdbuf_page(pls);
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
    return (int) fread(p_c, sizeof(U_CHAR), 1, pls->plbufFile);
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
    return (int) fwrite(&c1, sizeof(U_CHAR), 1, pls->plbufFile);
}
