/* $Id$
   $Log$
   Revision 1.1  1992/11/07 07:57:09  mjl
   Routines for writing to and reading from a plot buffer, as well as recreating
   an entire plot.  The driver need merely specify pls->plbuf_enable = 1 for it
   to be used (it must of course handle the expose/resize events itself).

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
#include "dispatch.h"
#include "metadefs.h"

/* Function prototypes */

static void	plbuf_open	(PLStream *pls);
static void	plbuf_close	(PLStream *pls);

static int	rd_command	( PLStream *pls, U_CHAR * );
static int	wr_command	( PLStream *pls, U_CHAR );
static void	process_next	( PLStream *pls, U_CHAR );

void rdbuf_init		(PLStream *);
void rdbuf_line		(PLStream *);
void rdbuf_clear	(PLStream *);
void rdbuf_page		(PLStream *);
void rdbuf_adv		(PLStream *);
void rdbuf_tidy		(PLStream *);
void rdbuf_color	(PLStream *);
void rdbuf_text		(PLStream *);
void rdbuf_graph	(PLStream *);
void rdbuf_width	(PLStream *);
void rdbuf_esc		(PLStream *);

static void plbufesc_ancol	(PLStream *pls, pleNcol *col);
static void rdbufesc_ancol	(PLStream *pls, pleNcol *col);
static void plbufesc_rgb	(PLStream *pls, pleRGB *cols);
static void rdbufesc_rgb	(PLStream *pls, pleRGB *cols);

/*----------------------------------------------------------------------*\
* Routines to write to the plot buffer.
\*----------------------------------------------------------------------*/

/*----------------------------------------------------------------------*\
* plbuf_init()
*
* Initialize device.
\*----------------------------------------------------------------------*/

void 
plbuf_init (PLStream *pls)
{
}

/*----------------------------------------------------------------------*\
* plbuf_line()
*
* Draw a line in the current color from (x1,y1) to (x2,y2).
\*----------------------------------------------------------------------*/

void 
plbuf_line (PLStream *pls, PLINT x1a, PLINT y1a, PLINT x2a, PLINT y2a)
{
    U_SHORT xy[4];

    if (pls->plbuf_read) return;

    if (pls->plbuf_write) {
	(void) wr_command(pls, (U_CHAR) LINE);

	xy[0] = x1a; xy[1] = y1a; xy[2] = x2a; xy[3] = y2a;
	(void) fwrite(&xy, sizeof(U_SHORT), 4, pls->plbufFile);
    }
}

/*----------------------------------------------------------------------*\
* plbuf_clear()
*
* Clear page.
\*----------------------------------------------------------------------*/

void 
plbuf_clear (PLStream *pls)
{
    if (pls->plbuf_read) return;

    if (pls->plbuf_write) {
	(void) wr_command(pls, (U_CHAR) CLEAR);

	plbuf_close(pls);
    }
}

/*----------------------------------------------------------------------*\
* plbuf_page()
*
* Set up for the next page.
* Also write state information to ensure the next page is correct.
\*----------------------------------------------------------------------*/

void 
plbuf_page (PLStream *pls)
{
    if (pls->plbuf_read) return;

    plbuf_open(pls);
    if (pls->plbuf_write) {
	(void) wr_command(pls, (U_CHAR) PAGE);
	grcol();
	grwid();
    }
}

/*----------------------------------------------------------------------*\
* plbuf_adv()
*
* Advance to the next page.
* Also write page information as in plbuf_page().
\*----------------------------------------------------------------------*/

void 
plbuf_adv (PLStream *pls)
{
    if (pls->plbuf_read) return;

    if (pls->plbuf_write) {
	(void) wr_command(pls, (U_CHAR) ADVANCE);

	plbuf_close(pls);
    }
    plbuf_open(pls);
}

/*----------------------------------------------------------------------*\
* plbuf_tidy()
*
* Close graphics file
\*----------------------------------------------------------------------*/

void 
plbuf_tidy (PLStream *pls)
{
}

/*----------------------------------------------------------------------*\
* plbuf_color()
*
* Set pen color.
\*----------------------------------------------------------------------*/

void 
plbuf_color (PLStream *pls)
{
    U_SHORT icol = pls->color;

    if (pls->plbuf_read) return;

    if (pls->plbuf_write) {
	(void) wr_command(pls, (U_CHAR) NEW_COLOR);
	(void) fwrite(&icol, sizeof(U_SHORT), 1, pls->plbufFile);
    }
}

/*----------------------------------------------------------------------*\
* plbuf_text()
*
* Switch to text mode.
\*----------------------------------------------------------------------*/

void 
plbuf_text (PLStream *pls)
{
    if (pls->plbuf_read) return;

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
plbuf_graph (PLStream *pls)
{
    if (pls->plbuf_read) return;

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
plbuf_width (PLStream *pls)
{
    U_SHORT width = pls->width;

    if (pls->plbuf_read) return;

    if (pls->plbuf_write) {
	(void) wr_command(pls, (U_CHAR) NEW_WIDTH);
	(void) fwrite(&width, sizeof(U_SHORT), 1, pls->plbufFile);
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
* PL_SET_RGB	  Writes three data values for R, G, B content
* PL_ALLOC_NCOL	  Writes color table allocation info
* PL_SET_LPB	  Writes local plot bounds
\*----------------------------------------------------------------------*/

void 
plbuf_esc (PLStream *pls, PLINT op, char *ptr)
{
    if (pls->plbuf_read) return;

    if (pls->plbuf_write) {
	(void) wr_command(pls, (U_CHAR) ESCAPE);
	(void) wr_command(pls, (U_CHAR) op);

	switch (op) {
	  case PL_SET_RGB:
	    plbufesc_rgb(pls, (pleRGB *) ptr);
	    break;

	  case PL_ALLOC_NCOL:
	    plbufesc_ancol(pls, (pleNcol *) ptr);
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
rdbuf_init (PLStream *pls)
{
}

/*----------------------------------------------------------------------*\
* rdbuf_line()
*
* Draw a line in the current color from (x1,y1) to (x2,y2).
\*----------------------------------------------------------------------*/

void 
rdbuf_line (PLStream *pls)
{
    PLINT x1a, y1a, x2a, y2a;
    U_SHORT xy[4];

    (void) fread(&xy, sizeof(U_SHORT), 4, pls->plbufFile);
    x1a = xy[0]; y1a = xy[1]; x2a = xy[2]; y2a = xy[3];

    grline( x1a, y1a, x2a, y2a );
}

/*----------------------------------------------------------------------*\
* rdbuf_clear()
*
* Clear page.
\*----------------------------------------------------------------------*/

void 
rdbuf_clear (PLStream *pls)
{
}

/*----------------------------------------------------------------------*\
* rdbuf_page()
*
* Set up for the next page.
* Also write: 
*     - The page number for the following page (not strictly necessary 
*	but the redundancy may someday be useful).
*     - A blank field after the command to eventually hold the byte
*	distance to the next page.
\*----------------------------------------------------------------------*/

void 
rdbuf_page (PLStream *pls)
{
}

/*----------------------------------------------------------------------*\
* rdbuf_adv()
*
* Advance to the next page.
* Also write page information as in rdbuf_page().
\*----------------------------------------------------------------------*/

void 
rdbuf_adv (PLStream *pls)
{
}

/*----------------------------------------------------------------------*\
* rdbuf_tidy()
*
* Close graphics file
\*----------------------------------------------------------------------*/

void 
rdbuf_tidy (PLStream *pls)
{
}

/*----------------------------------------------------------------------*\
* rdbuf_color()
*
* Set pen color.
\*----------------------------------------------------------------------*/

void 
rdbuf_color (PLStream *pls)
{
    U_SHORT icol;

    (void) fread(&icol, sizeof(U_SHORT), 1, pls->plbufFile);
    pls->color = icol;
    grcol();
}

/*----------------------------------------------------------------------*\
* rdbuf_text()
*
* Switch to text mode.
\*----------------------------------------------------------------------*/

void 
rdbuf_text (PLStream *pls)
{
    grtext();
}

/*----------------------------------------------------------------------*\
* rdbuf_graph()
*
* Switch to graphics mode.
\*----------------------------------------------------------------------*/

void 
rdbuf_graph (PLStream *pls)
{
    grgra();
}

/*----------------------------------------------------------------------*\
* rdbuf_width()
*
* Set pen width.
\*----------------------------------------------------------------------*/

void 
rdbuf_width (PLStream *pls)
{
    U_SHORT width;

    (void) fread(&width, sizeof(U_SHORT), 1, pls->plbufFile);
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
* PL_SET_RGB	  Reads three data values for R, G, B content
* PL_ALLOC_NCOL	  Reads color table allocation info
* PL_SET_LPB	  Reads local plot bounds
\*----------------------------------------------------------------------*/

void 
rdbuf_esc (PLStream *pls)
{
    U_CHAR op;
    pleRGB cols;
    pleNcol col;
    char * ptr;

    (void) fread(&op, sizeof(U_CHAR), 1, pls->plbufFile);

    switch (op) {
      case PL_SET_RGB:
	rdbufesc_rgb(pls, &cols);
	ptr = (char *) &cols;
	break;

      case PL_ALLOC_NCOL:
	rdbufesc_ancol(pls, &col);
	ptr = (char *) &col;
	break;
    }
    gresc (op, ptr);
}

/*----------------------------------------------------------------------*\
* Escape function: Allocate named color.
\*----------------------------------------------------------------------*/

static void 
plbufesc_ancol (PLStream *pls, pleNcol *col)
{
    (void) fwrite(&col->icolor, sizeof(int), 1, pls->plbufFile);
    (void) write_header(pls->plbufFile, &col->name);
}

static void 
rdbufesc_ancol (PLStream *pls, pleNcol *col)
{
    (void) fread(&col->icolor, sizeof(int), 1, pls->plbufFile);
    (void) read_header(pls->plbufFile, &col->name);
}

/*----------------------------------------------------------------------*\
* Escape function: Set rgb color.
\*----------------------------------------------------------------------*/

static void 
plbufesc_rgb (PLStream *pls, pleRGB *cols)
{
    (void) fwrite(&cols->red,   sizeof(float), 1, pls->plbufFile);
    (void) fwrite(&cols->green, sizeof(float), 1, pls->plbufFile);
    (void) fwrite(&cols->blue,  sizeof(float), 1, pls->plbufFile);
}

static void 
rdbufesc_rgb (PLStream *pls, pleRGB *cols)
{
    (void) fread(&cols->red,   sizeof(float), 1, pls->plbufFile);
    (void) fread(&cols->green, sizeof(float), 1, pls->plbufFile);
    (void) fread(&cols->blue,  sizeof(float), 1, pls->plbufFile);
}

/*----------------------------------------------------------------------*\
* plRemakePlot()
*
* Rebuilds plot from plot buffer, usually in response to an X
* resize or exposure event.
\*----------------------------------------------------------------------*/

void 
plRemakePlot(PLStream *pls)
{
    int istat;
    U_CHAR c;

    if (!pls->plbuf_write) return;

    pls->plbufFile = freopen(pls->plbufFnam, "rb+", pls->plbufFile);
    pls->plbuf_write = FALSE;
    pls->plbuf_read = TRUE;

    while (rd_command(pls, &c)) {
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

      case ADVANCE:
	rdbuf_adv(pls);
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

      case ESCAPE:
	rdbuf_esc(pls);
	break;
    }
}

/*----------------------------------------------------------------------*\
* Support routines
\*----------------------------------------------------------------------*/

static void
plbuf_open(PLStream *pls)
{
    pls->plbuf_write = FALSE;
    if (!pls->plbuf_enable) return;

    (void) tmpnam(pls->plbufFnam);
    pls->plbufFile = fopen(pls->plbufFnam, "wb");
    if (pls->plbufFile == NULL) {
	fprintf(stderr, "\nError opening plot data storage file.\n");
	exit(1);
    }
    pls->plbuf_write = TRUE;
}

static void
plbuf_close(PLStream *pls)
{
    fclose(pls->plbufFile);
    remove(pls->plbufFnam);
}

/*----------------------------------------------------------------------*\
*  rd_command()
*
*  Read & return the next command
\*----------------------------------------------------------------------*/

static int
rd_command(PLStream *pls, U_CHAR *p_c)
{
    return fread(p_c, sizeof(U_CHAR), 1, pls->plbufFile);
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
    return fwrite(&c1, sizeof(U_CHAR), 1, pls->plbufFile);
}

