/* $Id$
 * $Log$
 * Revision 1.11  1994/02/07 22:56:18  mjl
 * Fixed error messages to use new iodev data structure.
 *
 * Revision 1.10  1994/02/01  22:43:47  mjl
 * Slight change to error handling for unrecognized commands.
 *
 * Revision 1.9  1994/01/15  17:38:12  mjl
 * Changed to new PDF function call syntax.
 *
 * Revision 1.8  1993/09/28  21:26:38  mjl
 * Fixed an inconsistency in the byte count.
 *
 * Revision 1.7  1993/08/11  19:23:33  mjl
 * Added some extra debugging code.
 *
 * Revision 1.6  1993/08/09  22:15:02  mjl
 * Eliminated all vestiges of old clr/page syntax, in favor of eop/bop.
 *
 * Revision 1.5  1993/08/03  01:46:46  mjl
 * Changes to eliminate warnings when compiling with gcc -Wall.
 *
 * Revision 1.4  1993/07/31  08:06:54  mjl
 * More consolidation of driver functions.
 *
 * Revision 1.3  1993/07/28  05:42:38  mjl
 * Some minor changes to aid debugging.
 *
 * Revision 1.2  1993/07/16  21:58:37  mjl
 * Removed handling for orientation, aspect setting at a low level, since this
 * is now done by the driver interface layer.
 *
 * Revision 1.1  1993/07/02  06:58:30  mjl
 * The new TCL/TK driver!  Yes it's finally here!  YAAAAAAAAYYYYYYY!!!
*/

/*
    plr.c

    Copyright 1993
    Maurice LeBrun
    IFS, University of Texas at Austin

    This software may be freely copied, modified and redistributed without
    fee provided that this copyright notice is preserved intact on all
    copies and modified copies.

    There is no warranty or other guarantee of fitness of this software.
    It is provided solely "as is". The author(s) disclaim(s) all
    responsibility and liability with respect to this software's usage or
    its effect upon hardware or computer systems.

* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
* Support routines to render a PLPLOT byte stream, interpreting the PLPLOT
* metacode.  
*
* Although this code is duplicated to some extent by plrender and the
* plot buffer redraw code (in plbuf.c), they are all different in some
* significant ways, namely:
*
*    - plrender must always retain backward compatibility code to 
*      handle old metafiles, as well as stuff to support seeking.
*
*    - plbuf by definition is redrawing on the same machine so no 
*      special effort is necessary to make the byte stream portable,
*      also, only low-level graphics calls are used (grline, etc)
*
* The rendering code here must (by contrast to plbuf) use the high
* level plot routines as plrender, to support local zooms as well as
* the ability to dump the associated window into plot space to a file,
* but is otherwise pretty minimal.  A portable byte stream is used
* since eventually the FIFO communication may be replaced by network
* communication over a socket.
*
\*----------------------------------------------------------------------*/

#include "plserver.h"
#include "plevent.h"
#include "metadefs.h"

/* Some wrapper macros to return (-1) on error */

#define plr_rd(code) \
if (code) { fprintf(stderr, \
	    "Unable to read from %s in %s at line %d, bytecount %d\n", \
	    plr->iodev->typename, __FILE__, __LINE__, plr->nbytes); \
	    return(-1); }

#define plr_cmd(code) \
if ((code) == -1) return(-1);

/* Error termination */

#define barf(msg) \
{ fprintf(stderr, "%s\nCurrent command code: %d\n", msg, csave); return(-1); }

/* Static function prototypes. */

static int	plr_process1 	(PLRDev *plr, int c);
static int	plr_init	(PLRDev *plr);
static int	plr_line	(PLRDev *plr, int c);
static int	plr_eop		(PLRDev *plr);
static int	plr_bop 	(PLRDev *plr);
static int	plr_state	(PLRDev *plr);
static int	plr_esc		(PLRDev *plr);
static int	plr_get		(PLRDev *plr);
static int	plr_unget	(PLRDev *plr, U_CHAR c);
static int	get_ncoords	(PLRDev *plr, PLFLT *x, PLFLT *y, PLINT n);

/* variables */

static int	csave = -1;
static U_CHAR	dum_uchar;
static U_SHORT	dum_ushort;

/*----------------------------------------------------------------------*\
* plr_start()
*
* Set default state parameters before anyone else has a chance to.
\*----------------------------------------------------------------------*/

void
plr_start(PLRDev *plr)
{
    dbug_enter("plr_start");

    plr->xmin = 0;
    plr->xmax = PIXELS_X - 1;
    plr->ymin = 0;
    plr->ymax = PIXELS_Y - 1;

    plr->xold = UNDEFINED;
    plr->yold = UNDEFINED;
}

/*----------------------------------------------------------------------*\
* plr_process()
*
* Read & process commands until "nbyte_max" bytes have been read.
\*----------------------------------------------------------------------*/

int
plr_process(PLRDev *plr)
{
    int c;

    dbug_enter("plr_process");

    while (plr->nbytes > 0) {
	plr_cmd( c = plr_get(plr) );
	csave = c;
	plr_cmd( plr_process1(plr, c) );
    }
    return 0;
}

/*----------------------------------------------------------------------*\
* plr_process1()
*
* Process a command.  Note: because of line->polyline compression, this
* may actually process an arbitrary number of LINE or LINETO commands.
* Since the data buffer (fifo or socket) is only flushed after a complete
* command, there should be no danger in rushing blindly ahead to execute
* each plot command.
\*----------------------------------------------------------------------*/

static int
plr_process1(PLRDev *plr, int c)
{
    switch (c) {

      case INITIALIZE:
	plr_cmd( plr_init(plr) );
	break;

      case LINE:
      case LINETO:
      case POLYLINE:
	plr_cmd( plr_line(plr, c) );
	break;

      case EOP:
	plr->at_eop = 1;
	plr_cmd( plr_eop(plr) );
	break;

      case BOP:
	plr->at_bop = 1;
	plr_cmd( plr_bop(plr) );
	break;

      case CHANGE_STATE:
	plr_cmd( plr_state(plr) );
	break;

      case ESCAPE:
	plr_cmd( plr_esc(plr) );
	break;

      default:
	fprintf(stderr, "plr_process1: Unrecognized command code %d\n", c);
    }

    return 0;
}

/*----------------------------------------------------------------------*\
* void plr_init()
*
* Handle initialization.
\*----------------------------------------------------------------------*/

static int
plr_rd_header(PLRDev *plr, char *header)
{
    plr_rd( pdf_rd_header(plr->pdfs, header) );
    plr->nbytes -= (strlen(header) + 1);
    return 0;
}

static int
plr_init(PLRDev *plr)
{
    char tk_magic[80], tk_version[80];
    char tag[80];

    dbug_enter("plr_init");

/* Read header info */

    plr_cmd( plr_rd_header(plr, tk_magic) );
    if (strcmp(tk_magic, PLSERV_HEADER))
	barf("Invalid header");

/* Read version field of header.  We need to check that we can read the */
/* byte stream, in case this is an old version of plserver. */

    plr_cmd( plr_rd_header(plr, tk_version) );
    if (strcmp(tk_version, PLSERV_VERSION) > 0) {
	fprintf(stderr,
	    "Error: incapable of reading output of version %s.\n", tk_version);
	barf("Please obtain a newer copy of plserver.");
    }

/* Read tagged initialization info. */
/* Overkill, but a no-brainer since plrender already uses this */

    for (;;) {
	plr_cmd( plr_rd_header(plr, tag) );
	if (*tag == '\0')
	    break;

	if (!strcmp(tag, "xmin")) {
	    plr_rd( pdf_rd_2bytes(plr->pdfs, &dum_ushort) );
	    plr->xmin = dum_ushort;
	    plr->nbytes -= 2;
	    continue;
	}

	if (!strcmp(tag, "xmax")) {
	    plr_rd( pdf_rd_2bytes(plr->pdfs, &dum_ushort) );
	    plr->xmax = dum_ushort;
	    plr->nbytes -= 2;
	    continue;
	}

	if (!strcmp(tag, "ymin")) {
	    plr_rd( pdf_rd_2bytes(plr->pdfs, &dum_ushort) );
	    plr->ymin = dum_ushort;
	    plr->nbytes -= 2;
	    continue;
	}

	if (!strcmp(tag, "ymax")) {
	    plr_rd( pdf_rd_2bytes(plr->pdfs, &dum_ushort) );
	    plr->ymax = dum_ushort;
	    plr->nbytes -= 2;
	    continue;
	}

	if (!strcmp(tag, "width")) {
	    plr_rd( pdf_rd_1byte(plr->pdfs, &dum_uchar) );
	    plwid(dum_uchar);
	    plr->nbytes -= 2;
	    continue;
	}

	barf("plr_init: Unrecognized initialization tag.");
    }

    return 0;
}

/*----------------------------------------------------------------------*\
* plr_line()
*
* Draw a line or polyline.
\*----------------------------------------------------------------------*/

static int
plr_line(PLRDev *plr, int c)
{
    int c1;
    U_SHORT npts;
    PLFLT x[PL_MAXPOLYLINE], y[PL_MAXPOLYLINE];

    npts = 1;
    x[0] = plr->xold;
    y[0] = plr->yold;

    switch ((int) c) {

      case LINE:
	plr_cmd( get_ncoords(plr, x, y, 1) );

      case LINETO:
	for (;;) {
	    plr_cmd( get_ncoords(plr, x + npts, y + npts, 1) );

	    npts++;
	    if (npts == PL_MAXPOLYLINE || plr->nbytes == 0)
		break;

	    plr_cmd( c1 = plr_get(plr) );
	    if (c1 != LINETO) {
		plr_cmd( plr_unget(plr, (U_CHAR) c1) );
		break;
	    }
	}
	break;

      case POLYLINE:
	plr_rd( pdf_rd_2bytes(plr->pdfs, &npts) );
	plr->nbytes -= 2;
	plr_cmd( get_ncoords(plr, x, y, npts) );
	break;
    }

    plline(npts, x, y);

    plr->xold = x[npts - 1];
    plr->yold = y[npts - 1];

    return 0;
}

/*----------------------------------------------------------------------*\
* get_ncoords()
*
* Read n coordinate vectors.
\*----------------------------------------------------------------------*/

#define plr_rdn(code) \
if (code) { fprintf(stderr, \
"Unable to read from %s in %s at line %d, bytecount %d\n\
Bytes requested: %d\n", \
plr->iodev->typename, __FILE__, __LINE__, plr->nbytes, 2*n); return(-1); }

static int
get_ncoords(PLRDev *plr, PLFLT *x, PLFLT *y, PLINT n)
{
    int i;
    short xs[PL_MAXPOLYLINE], ys[PL_MAXPOLYLINE];

    plr_rdn( pdf_rd_2nbytes(plr->pdfs, (U_SHORT *) xs, n) );
    plr_rdn( pdf_rd_2nbytes(plr->pdfs, (U_SHORT *) ys, n) );
    plr->nbytes -= 4*n;

    for (i = 0; i < n; i++) {
	x[i] = xs[i];
	y[i] = ys[i];
    }
    return 0;
}

/*----------------------------------------------------------------------*\
* plr_eop()
*
* Clear screen.
\*----------------------------------------------------------------------*/

static int
plr_eop(PLRDev *plr)
{
    dbug_enter("plr_eop");

    pleop();
    return 0;
}

/*----------------------------------------------------------------------*\
* plr_bop()
*
* Page advancement.
\*----------------------------------------------------------------------*/

static int
plr_bop(PLRDev *plr)
{
    dbug_enter("plr_bop");

/* Advance and setup the page */

    plbop();
    plvpor(0., 1., 0., 1.);
    plwind(plr->xmin, plr->xmax, plr->ymin, plr->ymax);

    return 0;
}

/*----------------------------------------------------------------------*\
* plr_state()
*
* Handle change in PLStream state (color, pen width, fill attribute, etc).
\*----------------------------------------------------------------------*/

static int
plr_state(PLRDev *plr)
{
    U_CHAR op;

    plr_rd( pdf_rd_1byte(plr->pdfs, &op) );
    plr->nbytes -= 1;

    switch (op) {

    case PLSTATE_WIDTH:{
	U_SHORT width;

	plr_rd( pdf_rd_2bytes(plr->pdfs, &width) );
	plr->nbytes -= 2;

	plwid(width);
	break;
    }

    case PLSTATE_COLOR0:{
	U_CHAR icol0, r, g, b;

	plr_rd( pdf_rd_1byte(plr->pdfs, &icol0) );
	plr->nbytes -= 1;

	if (icol0 == PL_RGB_COLOR) {
	    plr_rd( pdf_rd_1byte(plr->pdfs, &r) );
	    plr_rd( pdf_rd_1byte(plr->pdfs, &g) );
	    plr_rd( pdf_rd_1byte(plr->pdfs, &b) );
	    plr->nbytes -= 3;
	    plrgb1(r, g, b);
	}
	else {
	    plcol(icol0);
	}
	break;
    }

    case PLSTATE_COLOR1:
	break;
    }

    return 0;
}

/*----------------------------------------------------------------------*\
* plr_esc()
*
* Handle all escape functions.
* None supported for now -- just read command code and move on.
\*----------------------------------------------------------------------*/

static int
plr_esc(PLRDev *plr)
{
    U_CHAR op;

    plr_rd( pdf_rd_1byte(plr->pdfs, &op) );
    plr->nbytes -= 1;
    return 0;
}

/*----------------------------------------------------------------------*\
* plr_get()
*
* Read & return the next command
\*----------------------------------------------------------------------*/

static int
plr_get(PLRDev *plr)
{
    int c;

    c = pdf_getc(plr->pdfs);
    if (c == EOF) {
	fprintf(stderr, "plr_get: at byte count: %d\n", plr->nbytes);
	barf("plr_get: Unable to read character");
    }

    plr->nbytes -= 1;
    return c;
}

/*----------------------------------------------------------------------*\
* plr_unget()
*
* Push back the last command read.
\*----------------------------------------------------------------------*/

static int
plr_unget(PLRDev *plr, U_CHAR c)
{
    if (pdf_ungetc(c, plr->pdfs) == EOF) {
	barf("plr_unget: Unable to push back character");
    }

    plr->nbytes += 1;
    return 0;
}
