/* $Id$
 * $Log$
 * Revision 1.14  1993/09/08 02:27:11  mjl
 * Inserted change to ensure that streams that do not own the plot buffer
 * cannot write to it (was causing problems on plot saves).
 *
 * Revision 1.13  1993/08/30  19:11:51  mjl
 * Fixed the damage incurred by my previous "fix".  Plot buffer now appears
 * capable of accurately and unobtrusively replaying partially filled pages.
 *
 * Revision 1.12  1993/08/28  06:28:07  mjl
 * Fixed escape function reads & writes to be consistent.  Added plbuf_rewind
 * function, which writes EOP's to the end of file to make sure that dicking
 * around with the window settings before the page is complete doesn't end in
 * a core dump.
 *
 * Revision 1.11  1993/07/31  07:56:40  mjl
 * Several driver functions consolidated, for all drivers.  The width and color
 * commands are now part of a more general "state" command.  The text and
 * graph commands used for switching between modes is now handled by the
 * escape function (very few drivers require it).  The device-specific PLDev
 * structure is now malloc'ed for each driver that requires it, and freed when
 * the stream is terminated.
 *
 * Revision 1.10  1993/07/16  22:12:17  mjl
 * Simplified and slightly changed behavior to support plot dumps to cloned
 * plplot streams.
 *
 * Revision 1.9  1993/07/01  21:59:42  mjl
 * Changed all plplot source files to include plplotP.h (private) rather than
 * plplot.h.  Rationalized namespace -- all externally-visible plplot functions
 * now start with "pl"; device driver functions start with "plD_".
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
static void	plbuf_control	( PLStream *pls, U_CHAR );

void rdbuf_init		(PLStream *);
void rdbuf_line		(PLStream *);
void rdbuf_polyline	(PLStream *);
void rdbuf_eop		(PLStream *);
void rdbuf_bop		(PLStream *);
void rdbuf_tidy		(PLStream *);
void rdbuf_state	(PLStream *);
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
	if (pls->plbufFile == NULL) 
	    plexit("plbuf_init: Error opening plot data storage file.");
    }
    if ( ! pls->plbufOwner)
	pls->plbuf_write = 0;
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

    wr_command(pls, (U_CHAR) LINE);

    xpoly[0] = x1a;
    xpoly[1] = x2a;
    ypoly[0] = y1a;
    ypoly[1] = y2a;

    fwrite(xpoly, sizeof(short), 2, pls->plbufFile);
    fwrite(ypoly, sizeof(short), 2, pls->plbufFile);
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

    wr_command(pls, (U_CHAR) POLYLINE);
    fwrite(&npts, sizeof(PLINT), 1, pls->plbufFile);

    for (i = 0; i < npts; i++) {
	xpoly[i] = xa[i];
	ypoly[i] = ya[i];
    }
    fwrite(xpoly, sizeof(short), npts, pls->plbufFile);
    fwrite(ypoly, sizeof(short), npts, pls->plbufFile);
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

    wr_command(pls, (U_CHAR) EOP);
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
    wr_command(pls, (U_CHAR) BOP);
    plP_state(PLSTATE_COLOR0);
    plP_state(PLSTATE_WIDTH);
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
* plbuf_state()
*
* Handle change in PLStream state (color, pen width, fill attribute, etc).
\*----------------------------------------------------------------------*/

void 
plbuf_state(PLStream *pls, PLINT op)
{
    dbug_enter("plbuf_state");

    wr_command(pls, (U_CHAR) CHANGE_STATE);
    wr_command(pls, (U_CHAR) op);

    switch (op) {

    case PLSTATE_WIDTH:{
	U_CHAR width = pls->width;

	fwrite(&width, sizeof(U_CHAR), 1, pls->plbufFile);
	break;
    }

    case PLSTATE_COLOR0:{
	U_CHAR icol0 = pls->icol0;
	U_CHAR r = pls->curcolor.r;
	U_CHAR g = pls->curcolor.g;
	U_CHAR b = pls->curcolor.b;

	fwrite(&icol0, sizeof(U_CHAR), 1, pls->plbufFile);
	if (icol0 == PL_RGB_COLOR) {
	    fwrite(&r, sizeof(U_CHAR), 1, pls->plbufFile);
	    fwrite(&g, sizeof(U_CHAR), 1, pls->plbufFile);
	    fwrite(&b, sizeof(U_CHAR), 1, pls->plbufFile);
	}
	break;
    }

    case PLSTATE_COLOR1:
	break;
    }
}

/*----------------------------------------------------------------------*\
* plbuf_esc()
*
* Escape function.  Note that any data written must be in device
* independent form to maintain the transportability of the metafile.
* Don't actually write escape command unless necessary.
*
* Functions:
*
\*----------------------------------------------------------------------*/

void
plbuf_esc(PLStream *pls, PLINT op, void *ptr)
{
    dbug_enter("plbuf_esc");
/*
    switch (op) {
    case ?:
	wr_command(pls, (U_CHAR) ESCAPE);
	wr_command(pls, (U_CHAR) op);
	break;
    }
*/
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

    fread(xpoly, sizeof(short), 2, pls->plbufFile);
    fread(ypoly, sizeof(short), 2, pls->plbufFile);

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

    fread(&npts, sizeof(PLINT), 1, pls->plbufFile);
    fread(xpoly, sizeof(short), npts, pls->plbufFile);
    fread(ypoly, sizeof(short), npts, pls->plbufFile);

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
* rdbuf_state()
*
* Handle change in PLStream state (color, pen width, fill attribute, etc).
\*----------------------------------------------------------------------*/

void 
rdbuf_state(PLStream *pls)
{
    U_CHAR op;

    dbug_enter("rdbuf_state");

    fread(&op, sizeof(U_CHAR), 1, pls->plbufFile);

    switch (op) {

    case PLSTATE_WIDTH:{
	U_CHAR width;

	fread(&width, sizeof(U_CHAR), 1, pls->plbufFile);
	pls->width = width;
	plP_state(PLSTATE_WIDTH);

	break;
    }

    case PLSTATE_COLOR0:{
	U_CHAR icol0, r, g, b;

	fread(&icol0, sizeof(U_CHAR), 1, pls->plbufFile);
	if (icol0 == PL_RGB_COLOR) {
	    fread(&r, sizeof(U_CHAR), 1, pls->plbufFile);
	    fread(&g, sizeof(U_CHAR), 1, pls->plbufFile);
	    fread(&b, sizeof(U_CHAR), 1, pls->plbufFile);
	}
	else {
	    if ((int) icol0 > 15) {
		plwarn("rdbuf_state: Color map 0 entry hosed");
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

	plP_state(PLSTATE_COLOR0);
	break;
    }

    case PLSTATE_COLOR1:
	break;
    }
}

/*----------------------------------------------------------------------*\
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
\*----------------------------------------------------------------------*/

void
rdbuf_esc(PLStream *pls)
{
    U_CHAR op;
    void *ptr = NULL;

    dbug_enter("rdbuf_esc");

    fread(&op, sizeof(U_CHAR), 1, pls->plbufFile);
/* None are currently supported!
    switch (op) {
      case ?:
	plP_esc(op, ptr);
	break;
    }
*/
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
    long eofpos;

    dbug_enter("plRemakePlot");

    if (pls->plbufFile == NULL)
	return;

    fflush(pls->plbufFile);
    eofpos = ftell(pls->plbufFile);
    rewind(pls->plbufFile);

    plbuf_status = pls->plbuf_write;
    pls->plbuf_write = FALSE;
    while ((ftell(pls->plbufFile) <= eofpos) && rd_command(pls, &c)) {
	if (c == EOP)
	    break;
	plbuf_control(pls, c);
    }

    pls->plbuf_write = plbuf_status;
}

/*----------------------------------------------------------------------*\
* plbuf_control()
*
* Processes commands read from the plot buffer.
\*----------------------------------------------------------------------*/

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
	fprintf(stderr,
		"plbuf_control: Unrecognized command %d, previous %d\n",
		c, c_old);
    }
    c_old = c;
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
    if (count == 0) 
	fprintf(stderr, "Cannot read from plot buffer, char: %d\n", *p_c);
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
    if (count == 0) 
	fprintf(stderr, "Cannot write to plot buffer\n");
#endif
    return (count);
}
