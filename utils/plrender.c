/* $Id$
 * $Log$
 * Revision 1.38  1994/04/30 16:15:17  mjl
 * Fixed format field (%ld instead of %d) or introduced casts where
 * appropriate to eliminate warnings given by gcc -Wall.
 *
 * Revision 1.37  1994/04/08  12:37:53  mjl
 * Fixed initial tcl command that is responsible for the backpage button
 * appearing.
 *
 * Revision 1.36  1994/03/23  09:03:05  mjl
 * Added support for cmap1, color map state changes, hardware polygon fills.
 *
 * Revision 1.35  1994/01/17  20:45:02  mjl
 * Converted to new syntax for PDF function calls.
 *
 * Revision 1.34  1993/11/15  08:42:15  mjl
 * Added code to support seeks to the specified number of pages before EOF.
 * Number is specified using "--", i.e. -b --3 to start 3 pages before EOF.
 * Also works while interactively seeking.
 *
 * Revision 1.33  1993/08/28  06:39:14  mjl
 * Option table and handlers modified to include & accept new client data
 * pointer.
 *
 * Revision 1.32  1993/08/21  20:52:55  mjl
 * Deleted old argument handling for -jx, -jy, and -mar, since these are
 * now handled by plplot itself through the driver interface.
 *
 * Revision 1.31  1993/08/18  19:12:58  mjl
 * Minor fixes.
 *
 * Revision 1.30  1993/08/13  04:42:52  mjl
 * A wild and maddening rework of the page seeking facility.  The "last bugs"
 * proved amazingly difficult to stomp out.  These turned up during tests
 * over a medium-speed network link (unobservable when running locally).  I
 * hope its really right now, because I'd just as soon never look at that
 * piece of code again.
 *
 * Revision 1.29  1993/08/11  19:29:27  mjl
 * Updated command line parsing utils to work better with the new command
 * line utils, especially as regards invisible options.  Fixed a rarely
 * encountered bug in seeking.
*/

/*
    plrender.c

    Copyright 1991, 1992, 1993
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

    This file contains the code to render a PLPLOT metafile, written by
    the metafile driver, plmeta.c.
*/

char ident[] = "@(#) $Id$";

#include "plplotP.h"
#include "plstream.h"
#include "plevent.h"
#include "metadefs.h"
#include "pdf.h"

#include <string.h>
#include <ctype.h>

static char *program_name = "plrender";

/* Static function prototypes. */
/* INDENT OFF */
/* These handle the command loop */

static void	process_next	(U_CHAR c);
static void	plr_init	(U_CHAR c);
static void	plr_line	(U_CHAR c);
static void	plr_eop 	(U_CHAR c);
static void	plr_bop 	(U_CHAR c);
static void	plr_state	(U_CHAR c);
static void	plr_esc		(U_CHAR c);
static void	plresc_fill	(void);
static void	plresc_rgb	(void);
static void	plresc_ancol	(void);

/* Support functions */

static U_CHAR	getcommand	(void);
static void	ungetcommand	(U_CHAR);
static void	get_ncoords	(PLFLT *x, PLFLT *y, PLINT n);
static void	NextFamilyFile	(U_CHAR *);
static void	ReadPageHeader	(void);
static void	plr_KeyEH	(PLKey *, void *, int *);
static void	SeekToDisp	(long);
static void	SeekOnePage	(void);
static void	SeekToNextPage	(void);
static void	SeekToCurPage	(void);
static void	SeekToPrevPage	(void);
static void	SeekTo		(FPOS_T);
static void	doseek		(FPOS_T);
static void	PageIncr	(void);
static void	PageDecr	(void);

/* Initialization functions */

static void	Init		(int, char **);
static void	OpenMetaFile	(int *, char **);
static int	ReadFileHeader	(void);
static void	Help		(void);
static void 	Usage		(char *);
static void	myHelp		(void);
static void	mySyntax	(void);
static void	myNotes		(void);

/* Option handlers */

static int Opt_h	(char *, char *, void *);
static int Opt_v	(char *, char *, void *);
static int Opt_i	(char *, char *, void *);
static int Opt_b	(char *, char *, void *);
static int Opt_e	(char *, char *, void *);
static int Opt_p	(char *, char *, void *);

/* Global variables */
/* Mode flags for argument parsing */

static int mode_plrender = PL_PARSE_OVERRIDE;
static int mode_plplot = 0;
static int mode_showall = 0;

/* Stream pointer.  */
/* Fetched by some routines to directly set plplot state data */

static PLStream	*plsc;

/* Page info */

static PLINT	disp_beg=1;	/* Where to start plotting */
static PLINT	disp_end=-1;	/* Where to stop (0 to disable) */
static PLINT	curdisp;	/* Current page number */
static PLINT	cursub;		/* Current subpage */
static PLINT	curpage;	/* Current plot number */
static PLINT	nsubx;		/* subpages in x */
static PLINT	nsuby;		/* subpages in y */
static PLINT	target_disp;	/* Page we are seeking to */
static PLINT	target_page;	/* Plot we are seeking to */
static PLINT	delta;		/* Number of pages to go forward/back */
static PLINT	pages;		/* Number of pages in file */

static int	no_pagelinks;	/* Set if metafile doesn't have page links */
static int	end_of_page;	/* Set when we're at the end of a page */
static int	seek_mode;	/* Set after a seek, before a BOP */
static int	addeof_beg;	/* Set when we are counting back from eof */
static int	addeof_end;	/* Set when we are counting back from eof */

static FPOS_T	prevpage_loc;	/* Byte position of previous page header */
static FPOS_T	curpage_loc;	/* Byte position of current page header */
static FPOS_T	nextpage_loc;	/* Byte position of next page header */

/* File info */

static int	input_type;	/* 0 for file, 1 for stream */
static PDFstrm	*pdfs;		/* PDF stream handle */
static FILE	*MetaFile;	/* Actual metafile handle, for seeks etc */

static char	BaseName[80] = "", FileName[90] = "";
static PLINT	is_family, member=1;
static char	mf_magic[40], mf_version[40];

/* Dummy vars for reading stuff that is to be thrown away */

static U_CHAR	dum_uchar;
static U_SHORT	dum_ushort;
static float	dum_float;

/* Plot dimensions */

static short	xmin = 0;
static short	xmax = PLMETA_X_OLD;
static short	ymin = 0;
static short	ymax = PLMETA_Y_OLD;
static PLINT	xlen, ylen;

static float	pxlx = PIXEL_RES_X_OLD;
static float	pxly = PIXEL_RES_Y_OLD;

static PLFLT	dev_xpmm, dev_ypmm;
static PLINT	dev_xmin, dev_xmax, dev_ymin, dev_ymax, dev_xlen, dev_ylen;
static PLFLT	vpxmin, vpxmax, vpxlen, vpymin, vpymax, vpylen;

/* Miscellaneous */

static U_CHAR 	c_old, c1;
static U_SHORT	npts;
static int	direction_flag, isanum, at_eop;
static char	num_buffer[20];
static PLFLT	x[PL_MAXPOLY], y[PL_MAXPOLY];

/* Exit codes */

#define	EX_SUCCESS	0		/* success! */
#define	EX_ARGSBAD	1		/* invalid args */
#define	EX_BADFILE	2		/* invalid filename or contents */

/*----------------------------------------------------------------------*\
* Options data structure definition.
\*----------------------------------------------------------------------*/

static PLOptionTable option_table[] = {
{
    "showall",			/* Turns on invisible options */
    NULL,
    NULL,
    &mode_showall,
    PL_OPT_BOOL | PL_OPT_ENABLED | PL_OPT_INVISIBLE,
    "-showall",
    "Turns on invisible options" },
{
    "h",			/* Help */
    Opt_h,
    NULL,
    NULL,
    PL_OPT_FUNC | PL_OPT_ENABLED,
    "-h",
    "Print out this message" },
{
    "v",			/* Version */
    Opt_v,
    NULL,
    NULL,
    PL_OPT_FUNC | PL_OPT_ENABLED,
    "-v",
    "Print out the plrender version number" },
{
    "i",			/* Input file */
    Opt_i,
    NULL,
    NULL,
    PL_OPT_FUNC | PL_OPT_ENABLED | PL_OPT_ARG,
    "-i name",
    "Input filename" },
{
    "f",			/* Filter option */
    NULL,
    NULL,
    &input_type,
    PL_OPT_BOOL | PL_OPT_ENABLED | PL_OPT_ARG,
    "-f",
    "Filter option -- equivalent to \"-i - -o -\"" },
{
    "b",			/* Beginning page number */
    Opt_b,
    NULL,
    NULL,
    PL_OPT_FUNC | PL_OPT_ENABLED | PL_OPT_ARG,
    "-b number",
    "Beginning page number" },
{
    "e",			/* End page number */
    Opt_e,
    NULL,
    NULL,
    PL_OPT_FUNC | PL_OPT_ENABLED | PL_OPT_ARG,
    "-e number",
    "End page number" },
{
    "p",			/* Specified page only */
    Opt_p,
    NULL,
    NULL,
    PL_OPT_FUNC | PL_OPT_ENABLED | PL_OPT_ARG,
    "-p page",
    "Plot given page only" },
{
    NULL,			/* option */
    NULL,			/* handler */
    NULL,			/* client data */
    NULL,			/* address of variable to set */
    0,				/* mode flag */
    NULL,			/* short syntax */
    NULL }			/* long syntax */
};

static char *notes[] = {
"All parameters must be white-space delimited, and plrender options override",
"any plplot options of the same name.  If the \"-i\" flag is omitted the",
"filename parameter must come last.  Specifying \"-\" for the input or output",
"filename means use stdin or stdout, respectively.  Not all options valid",
"with all drivers.  Please see the man pages for more detail.",
NULL};

/* INDENT ON */
/*----------------------------------------------------------------------*\
* main()
*
* plrender -- render a PLPLOT metafile.
\*----------------------------------------------------------------------*/

int
main(int argc, char *argv[])
{
    static U_CHAR c;

/* Initialize */

    Init(argc, argv);

/*
* Read & process metafile commands.
* If familying is turned on, the end of one member file is just treated as
* a page break assuming the next member file exists.
*/
    for (;;) {
	c_old = c;
	c = getcommand();

	if (c == CLOSE) {
	    if (is_family)
		NextFamilyFile(&c);
	    if ( ! is_family)
		break;
	}

	if ((c == BOP || c == ADVANCE) && curdisp == disp_end)
	    break;

	process_next(c);
    }

/* Done */

    pdf_close(pdfs);
    if (strcmp(mf_version, "1993a") >= 0) 
	plspause(0);

    plend();
    exit(EX_SUCCESS);
}

/*----------------------------------------------------------------------*\
* 			Process the command loop
\*----------------------------------------------------------------------*/

/*----------------------------------------------------------------------*\
* process_next()
*
* Process a command.
* Typically plrender issues commands to plplot much like an application
* program would.  This results in a more robust and flexible API.  On the
* other hand, it is sometimes necessary to directly call low-level plplot
* routines to achieve certain special effects, increase performance, or
* simplify the code.
\*----------------------------------------------------------------------*/

static void
process_next(U_CHAR c)
{
    switch ((int) c) {

    case INITIALIZE:
	plr_init(c);
	break;

    case LINE:
    case LINETO:
    case POLYLINE:
	plr_line(c);
	break;

    case EOP:
	plr_eop(c);
	break;

    case BOP:
	plr_bop(c);
	break;

    case CHANGE_STATE:
	plr_state(getcommand());
	break;

    case ESCAPE:
	plr_esc(c);
	break;

/* These are all commands that should be absent from current metafiles but */
/* are recognized here for backward compatibility with old metafiles */

    case ADVANCE:
	plr_eop(c);
	plr_bop(c);
	break;

    case NEW_WIDTH:
    case NEW_COLOR0:
    case NEW_COLOR1:
	plr_state(c);
	break;

    case SWITCH_TO_TEXT:
    case SWITCH_TO_GRAPH:
	break;

    default:
	plexit("process_next: Unrecognized command");
    }
}

/*----------------------------------------------------------------------*\
* void plr_init()
*
* Handle initialization.
\*----------------------------------------------------------------------*/

static void
plr_init(U_CHAR c)
{
    float aspect, dev_aspect, ratio;

    dbug_enter("plr_init");

/* Register event handler */

    plsKeyEH(plr_KeyEH, NULL);

/* Start up plplot */

    plinit();
    plP_gsub(&nsubx, &nsuby, &cursub);

/* Set aspect ratio to the natural ratio of the metafile coordinate system. */

    xlen = xmax - xmin;
    ylen = ymax - ymin;

    aspect = (ylen / pxly) / (xlen / pxlx);

/* Aspect ratio of output device */

    plP_gphy(&dev_xmin, &dev_xmax, &dev_ymin, &dev_ymax);
    plP_gpixmm(&dev_xpmm, &dev_ypmm);

    dev_xlen = dev_xmax - dev_xmin;
    dev_ylen = dev_ymax - dev_ymin;

    dev_aspect = (dev_ylen / dev_ypmm) / (dev_xlen / dev_xpmm);

    if (dev_aspect <= 0.)
	fprintf(stderr, "Aspect ratio error: dev_aspect = %f\n", dev_aspect);

    ratio = aspect / dev_aspect;

/* Default relative coordinate space */

    vpxlen = 1.0;
    vpylen = 1.0;
    vpxmin = 0.5 - vpxlen / 2.;
    vpymin = 0.5 - vpylen / 2.;
    vpxmax = vpxmin + vpxlen;
    vpymax = vpymin + vpylen;

/*
* Construct viewport that preserves the aspect ratio of the original device
* (plmeta output file).  Thus you automatically get all physical coordinate
* plots to come out correctly.  Note: this could also be done using the
* driver interface function plsdimap.
*/
    if (ratio <= 0)
	fprintf(stderr, "Aspect ratio error: ratio = %f\n", ratio);
    else if (ratio < 1)
	vpylen = ratio;
    else
	vpxlen = 1. / ratio;

    vpxmin = (1. - vpxlen) / 2.;
    vpxmax = vpxmin + vpxlen;

    vpymin = (1. - vpylen) / 2.;
    vpymax = vpymin + vpylen;

/* Seek to first page */

    cursub = nsubx*nsuby;
    if (disp_beg > 1) {
	if (no_pagelinks) 
	    plwarn("plrender: Metafile does not support page seeks");
	else {
	    ReadPageHeader();
	    SeekToDisp(disp_beg);
	}
    }
}

/*----------------------------------------------------------------------*\
* plr_line()
*
* Draw a line or polyline.
* Multiple connected lines (i.e. LINETO's) are collapsed into a polyline.
\*----------------------------------------------------------------------*/

static void
plr_line(U_CHAR c)
{
    npts = 1;

    switch ((int) c) {

    case LINE:
	get_ncoords(x, y, 1);

    case LINETO:
	for (;;) {
	    get_ncoords(x + npts, y + npts, 1);

	    npts++;
	    if (npts == PL_MAXPOLY)
		break;

	    c1 = getcommand();
	    if (c1 != LINETO) {
		ungetcommand(c1);
		break;
	    }
	}
	break;

    case POLYLINE:
	plm_rd( pdf_rd_2bytes(pdfs, &npts) );
	get_ncoords(x, y, npts);
	break;
    }

    plline(npts, x, y);

    x[0] = x[npts - 1];
    y[0] = y[npts - 1];
}

/*----------------------------------------------------------------------*\
* get_ncoords()
*
* Read n coordinate vectors.
\*----------------------------------------------------------------------*/

static void
get_ncoords(PLFLT *x, PLFLT *y, PLINT n)
{
    short xs[PL_MAXPOLY], ys[PL_MAXPOLY];
    PLINT i;

    plm_rd( pdf_rd_2nbytes(pdfs, (U_SHORT *) xs, n) );
    plm_rd( pdf_rd_2nbytes(pdfs, (U_SHORT *) ys, n) );

    for (i = 0; i < n; i++) {
	x[i] = xs[i];
	y[i] = ys[i];
    }
}

/*----------------------------------------------------------------------*\
* plr_eop()
*
* Handle end of page.
* Here we run into a bit of difficulty with packed pages -- at the end
* there is no EOP operation done if the page is only partially full.
* So I peek ahead to see if the next operation is a CLOSE, and if so,
* push back the CLOSE and issue an EOP regardless.  
\*----------------------------------------------------------------------*/

static void
plr_eop(U_CHAR c)
{
    dbug_enter("plr_eop");

    c1 = getcommand();
    ungetcommand(c1);
    if (c1 == CLOSE)
	end_of_page = 1;

    if (cursub == nsubx * nsuby)
	end_of_page = 1;

    if (end_of_page == 1) {
	at_eop = 1;
	plP_eop();
	at_eop = 0;
    }
}

/*----------------------------------------------------------------------*\
* plr_bop()
*
* Page/subpage advancement.
\*----------------------------------------------------------------------*/

static void
plr_bop(U_CHAR c)
{
    dbug_enter("plr_bop");

    ungetcommand(c);
    ReadPageHeader();

/* Advance and setup the page or subpage */

    if (end_of_page) {
	plP_bop();
	end_of_page = 0;
	seek_mode = 0;
    }

    plP_ssub(nsubx, nsuby, cursub);
    plP_setsub();

    plvpor(vpxmin, vpxmax, vpymin, vpymax);
    plwind((PLFLT) xmin, (PLFLT) xmax, (PLFLT) ymin, (PLFLT) ymax);
}

/*----------------------------------------------------------------------*\
* plr_state()
*
* Handle change in PLStream state (color, pen width, fill attribute, etc).
\*----------------------------------------------------------------------*/

static void 
plr_state(U_CHAR op)
{
    int i;

    dbug_enter("plr_state");

    switch (op) {

    case PLSTATE_WIDTH:{
	U_SHORT width;

	plm_rd( pdf_rd_2bytes(pdfs, &width) );

	plwid(width);
	break;
    }

    case PLSTATE_COLOR0:{
	U_SHORT icol;
	U_CHAR icol0, r, g, b;

	if (strcmp(mf_version, "1993a") >= 0) {
	    plm_rd( pdf_rd_1byte(pdfs, &icol0) );

	    if (icol0 == PL_RGB_COLOR) {
		plm_rd( pdf_rd_1byte(pdfs, &r) );
		plm_rd( pdf_rd_1byte(pdfs, &g) );
		plm_rd( pdf_rd_1byte(pdfs, &b) );
		plrgb1(r, g, b);
	    }
	    else {
		plcol(icol0);
	    }
	}
	else {
	    plm_rd( pdf_rd_2bytes(pdfs, &icol) );
	    plcol(icol);
	}
	break;
    }

    case PLSTATE_COLOR1:{
	U_SHORT icol1;
	PLFLT col1;

	plm_rd( pdf_rd_2bytes(pdfs, &icol1) );
	plgpls(&plsc);
	col1 = (float) icol1 / (float) plsc->ncol1;
	plcol1(col1);
	break;
    }

    case PLSTATE_FILL:{
	signed char patt;

	plm_rd( pdf_rd_1byte(pdfs, (U_CHAR *) &patt) );
	plpsty(patt);
	break;
    }

    case PLSTATE_CMAP0:{
	U_CHAR ncol0;

	plgpls(&plsc);
	plm_rd(pdf_rd_1byte(pdfs, &ncol0));
	plsc->ncol0 = ncol0;
	for (i = 0; i < plsc->ncol0; i++) {
	    plm_rd(pdf_rd_1byte(pdfs, &plsc->cmap0[i].r));
	    plm_rd(pdf_rd_1byte(pdfs, &plsc->cmap0[i].g));
	    plm_rd(pdf_rd_1byte(pdfs, &plsc->cmap0[i].b));
	}
	break;
    }

    case PLSTATE_CMAP1:{
	U_SHORT ncol1;

	plgpls(&plsc);
	plm_rd(pdf_rd_2bytes(pdfs, &ncol1));
	plsc->ncol1 = ncol1;
	for (i = 0; i < plsc->ncol1; i++) {
	    plm_rd(pdf_rd_1byte(pdfs, &plsc->cmap1[i].r));
	    plm_rd(pdf_rd_1byte(pdfs, &plsc->cmap1[i].g));
	    plm_rd(pdf_rd_1byte(pdfs, &plsc->cmap1[i].b));
	}
	break;
    }
    }
}

/*----------------------------------------------------------------------*\
* plr_esc()
*
* Handle all escape functions.
\*----------------------------------------------------------------------*/

static void
plr_esc(U_CHAR c)
{
    U_CHAR op;

    dbug_enter("plr_esc");

    plm_rd( pdf_rd_1byte(pdfs, &op) );

    switch (op) {

    case PLESC_FILL:
	plresc_fill();
	break;

/* These are all commands that should be absent from current metafiles but */
/* are recognized here for backward compatibility with old metafiles */

    case PLESC_SET_RGB:
	plresc_rgb();
	return;

    case PLESC_ALLOC_NCOL:
	plresc_ancol();
	return;

    case PLESC_SET_LPB:
	plm_rd( pdf_rd_2bytes(pdfs, &dum_ushort) );
	plm_rd( pdf_rd_2bytes(pdfs, &dum_ushort) );
	plm_rd( pdf_rd_2bytes(pdfs, &dum_ushort) );
	plm_rd( pdf_rd_2bytes(pdfs, &dum_ushort) );
	return;
    }
}

/*----------------------------------------------------------------------*\
* plresc_fill()
*
* Fill polygon described in points pls->dev_x[] and pls->dev_y[].
\*----------------------------------------------------------------------*/

static void
plresc_fill(void)
{
    dbug_enter("plresc_fill");

    plm_rd( pdf_rd_2bytes(pdfs, &npts) );
    get_ncoords(x, y, npts);
    plfill(npts, x, y);
}

/*----------------------------------------------------------------------*\
* plresc_rgb()
*
* Process escape function for RGB color selection.
* Note that RGB color selection is no longer handled this way by
* plplot but we must handle it here for old metafiles.
\*----------------------------------------------------------------------*/

static void
plresc_rgb(void)
{
    float red, green, blue;
    U_SHORT ired, igreen, iblue;

    dbug_enter("plresc_rgb");

    plm_rd( pdf_rd_2bytes(pdfs, &ired) );
    plm_rd( pdf_rd_2bytes(pdfs, &igreen) );
    plm_rd( pdf_rd_2bytes(pdfs, &iblue) );

    red = (double) ired / 65535.;
    green = (double) igreen / 65535.;
    blue = (double) iblue / 65535.;

    plrgb(red, green, blue);
}

/*----------------------------------------------------------------------*\
* plresc_ancol()
*
* Process escape function for named color table allocation.
* OBSOLETE -- just read the info and move on.
\*----------------------------------------------------------------------*/

static void
plresc_ancol(void)
{
    U_CHAR icolor;
    char name[80];

    dbug_enter("plresc_ancol");

    plm_rd( pdf_rd_1byte(pdfs, &icolor) );
    plm_rd( pdf_rd_header(pdfs, name) );
}

/*----------------------------------------------------------------------*\
* 			Support routines
\*----------------------------------------------------------------------*/

/*----------------------------------------------------------------------*\
* NextFamilyFile()
*
* Start the next family if it exists.
\*----------------------------------------------------------------------*/

static void
NextFamilyFile(U_CHAR *c)
{
    dbug_enter("NextFamilyFile");

    (void) fclose(MetaFile);
    member++;
    (void) sprintf(FileName, "%s.%i", BaseName, (int) member);

    if ((MetaFile = fopen(FileName, "rb")) == NULL) {
	is_family = 0;
	return;
    }
    if (ReadFileHeader()) {
	is_family = 0;
	return;
    }
    pdfs->file = MetaFile;

/*
* If the family file was created correctly, the first instruction in the
* file (after state information) MUST be an INITIALIZE.  We throw this
* away and put a page advance in its place. 
*/

    *c = getcommand();
    while (*c == CHANGE_STATE) {
	plr_state(getcommand());
	*c = getcommand();
    }

    if (*c != INITIALIZE)
	fprintf(stderr,
		"Warning, first instruction in member file not an INIT\n");
    else
	*c = ADVANCE;
}

/*----------------------------------------------------------------------*\
* getcommand()
*
* Read & return the next command
\*----------------------------------------------------------------------*/

static U_CHAR
getcommand(void)
{
    int c;

    c = getc(MetaFile);
    if (c == EOF)
	plexit("getcommand: Unable to read from MetaFile");

    return (U_CHAR) c;
}

/*----------------------------------------------------------------------*\
* ungetcommand()
*
* Push back the last command read.
\*----------------------------------------------------------------------*/

static void
ungetcommand(U_CHAR c)
{
    if (ungetc(c, MetaFile) == EOF)
	plexit("ungetcommand: Unable to push back character");
}

/*----------------------------------------------------------------------*\
* plr_KeyEH()
*
* Keyboard event handler.  For mapping keyboard sequences to commands
* not usually supported by plplot, such as seeking around in the
* metafile.  Recognized commands:
*
* <Backspace>	|
* <Delete>	| Back page
* <Page up>	|
*
* +<num><CR>	Seek forward <num> pages.
* -<num><CR>	Seek backward <num> pages.
*
* <num><CR>	Seek to page <num>.
* --<num><CR>	Seek to <num> pages before EOF.
*
* Both <BS> and <DEL> are recognized for a back-page since the target
* system may use either as its erase key.  <Page Up> is present on some
* keyboards (different from keypad key).
*
* No user data is passed in this case, although a test case is
* illustrated. 
*
* Illegal input is ignored.
\*----------------------------------------------------------------------*/

static void
plr_KeyEH(PLKey *key, void *user_data, int *p_exit_eventloop)
{
    char *tst = (char *) user_data;
    int input_num, dun_seek = 0, terminator_seen = 0;

#ifdef DEBUG
    fprintf(stderr, "key->code = %x\n", key->code);
#endif

/* TEST */

    if (tst != NULL) {
	pltext();
	fprintf(stderr, "tst string: %s\n", tst);
	plgra();
    }

/* The rest deals with seeking only; so we return if it is disabled */

    if (no_pagelinks || (input_type > 0)) 
	return;

/* Forward (+) or backward (-) */

    if (key->string[0] == '+')
	direction_flag++;

    else if (key->string[0] == '-')
	direction_flag--;

/* If a number, store into num_buffer */

    if (isdigit(key->string[0])) {
	isanum = TRUE;
	(void) strncat(num_buffer, key->string, (20-strlen(num_buffer)));
    }

/* 
* Seek to specified page, or page advance.
* Not done until user hits <return>.
* Need to check for both <LF> and <CR> for portability.
*/
    if (key->code == PLK_Return ||
	key->code == PLK_Linefeed ||
	key->code == PLK_Next)
    {
	terminator_seen = 1;
	if (isanum) {
	    input_num = atoi(num_buffer);
	    if (input_num == 0) {
		if (strcmp(num_buffer, "0"))
		    input_num = -1;
	    }
	    if (input_num >= 0) {
		if (direction_flag == 0)
		    target_disp = input_num;
		else if (direction_flag == 1)
		    target_disp = curdisp + input_num;
		else if (direction_flag == -1)
		    target_disp = curdisp - input_num;
		else if (direction_flag == -2)
		    target_disp = pages - input_num;

		SeekToDisp(target_disp);
		dun_seek = 1;
	    }
	}
	else {
	    target_disp = curdisp + 1;
	    SeekToDisp(target_disp);
	    dun_seek = 1;
	}
    }

/* Page backward */

    if (key->code == PLK_BackSpace ||
	key->code == PLK_Delete ||
	key->code == PLK_Prior) 
    {
	terminator_seen = 1;
	target_disp = curdisp - 1;
	SeekToDisp(target_disp);
	dun_seek = 1;
    }

/* Cleanup */

    if (terminator_seen) {
	num_buffer[0] = '\0';
	direction_flag = 0;
	isanum = 0;
	key->code = 0;
    }
    if (dun_seek && at_eop)
	*p_exit_eventloop = TRUE;
}

/*----------------------------------------------------------------------*\
* SeekToDisp()
*
* Seek to 'target_disp' displayed page.  
*
* Several things combine to make this much harder than one would think.
* These include: taking packed pages into account, seeking from mid-plot,
* and multiple contiguous seek events.  All in all a disgusting mess.
*
* When this routine is called we are probably at the end of a page, but
* maybe not (e.g. if the user hit <Backspace> or <Return> in the middle
* of the screen update, and the driver immediately processes the event).
* Also we might be at the end of the file.  So only if we are right
* before the desired displayed page AND the end of page condition is met,
* we are done.
*
* When reference is made to a displayed (i.e. possibly packed) page, I
* will use variables with the "disp" tag.  For metafile pages, I will
* use "page".  I could have used the word "plot" somewhere but I am
* reserving that for future uses.
*
* To deal with multiple contiguous seek events (e.g. the user hits
* <Backspace> twice before the renderer has a chance to respond) the
* "seek_mode" variable was found to be necessary.
\*----------------------------------------------------------------------*/

static void
SeekToDisp(long target_disp)
{
    dbug_enter("SeekToDisp");

/* Determine target_page */
/* Needs to be the last subpage on the preceding displayed page,  */
/* so we subtract 1 unless the last seek is still active */

    if ( ! seek_mode)
	--target_disp;

    target_page = target_disp * nsubx * nsuby;
    delta = target_page - curpage;
    seek_mode = 1;

/* Handle special cases first -- none of these require seeks. */
/* An example of how each condition might arise is given. */

/* <Return> at the end of a page */

    if ((delta == 0) && at_eop)
	return;

/* <Return> while drawing the last page */

    if ((delta >= 0) && (nextpage_loc == 0))
	return;

/* Anything else requires at least one seek */

#ifdef DEBUG
    fprintf(stderr,
	    "Before seek: target_page = %d, curpage = %d, curdisp = %d\n",
	    target_page, curpage, curdisp);
#endif

/* <Return> while drawing any page but the last */

    if (delta == 0) {
	SeekToNextPage();
	goto done;
    }

/* Prepare for backward seeks by seeking to the start of the current page */
/* If on the first page, we're done */

    if (delta < 0) {
	SeekToCurPage();
	if (prevpage_loc == 0)
	    goto done;
    }

/* Now seek by pages until we arrive at the target page */

    while (delta != 0)
	SeekOnePage();

 done:
#ifdef DEBUG
    fprintf(stderr, "After seek: curpage = %d, curdisp = %d, cursub = %d\n",
	    curpage, curdisp, cursub);
#endif

    end_of_page = 1;
    return;
}

/*----------------------------------------------------------------------*\
* SeekOnePage()
*
* Seeks one page in appropriate direction, and updates delta.
* For out of bounds seeks, just stay on the boundary page (first or last).
\*----------------------------------------------------------------------*/

static void
SeekOnePage(void)
{
    if (delta > 0) {
	if (nextpage_loc == 0) {
	    SeekToCurPage();
	    delta = 0;
	    return;
	}
	SeekToNextPage();
    }
    else {
	if (prevpage_loc == 0) {
	    SeekToCurPage();
	    delta = 0;
	    return;
	}
	SeekToPrevPage();
    }

    delta = target_page - curpage;

#ifdef DEBUG
    fprintf(stderr, "Now at page %d, disp %d, delta %d\n",
	    curpage, curdisp, delta);
#endif
    return;
}

/*----------------------------------------------------------------------*\
* SeekToCurPage()
*
* Seeks to beginning of current page, changing the page counters if
* we pass a page boundary in the process.  Are you sufficiently sick
* yet?  I know I am.
\*----------------------------------------------------------------------*/

static void
SeekToCurPage(void)
{
    FPOS_T loc;

    if (pl_fgetpos(MetaFile, &loc))
	plexit("plrender: fgetpos call failed");

    if (loc != curpage_loc) 
	PageDecr();

    SeekTo(curpage_loc);
}

/*----------------------------------------------------------------------*\
* SeekToNextPage()
*
* Seeks to beginning of next page, changing the page counters if
* we pass a page boundary in the process.  
\*----------------------------------------------------------------------*/

static void
SeekToNextPage(void)
{
    FPOS_T loc;

    if (pl_fgetpos(MetaFile, &loc))
	plexit("plrender: fgetpos call failed");

    if (loc == curpage_loc) 
	PageIncr();

    SeekTo(nextpage_loc);
}

/*----------------------------------------------------------------------*\
* SeekToPrevPage()
*
* Seeks to beginning of previous page, changing page counters to
* take into account each page header we skip over.  Getting sicker
* all the time..
\*----------------------------------------------------------------------*/

static void
SeekToPrevPage(void)
{
    FPOS_T loc;

    if (pl_fgetpos(MetaFile, &loc))
	plexit("plrender: fgetpos call failed");

    if (loc != curpage_loc) 
	PageDecr();

    SeekTo(prevpage_loc);
    PageDecr();
}

/*----------------------------------------------------------------------*\
* SeekTo()
*
* Seeks to specified location, updating page links.
\*----------------------------------------------------------------------*/

static void
SeekTo(FPOS_T loc)
{
#ifdef DEBUG
    fprintf(stderr, "Seeking to: %d\n", loc);
#endif

    doseek(loc);

/* Update page links */

    ReadPageHeader();
    doseek(curpage_loc);
    PageDecr();
}

/*----------------------------------------------------------------------*\
* Utility functions:
*
* doseek()	Seeks to the specified location in the file.
* PageIncr()	Increments page counters
* PageDecr()	Decrements page counters
\*----------------------------------------------------------------------*/

static void
doseek(FPOS_T loc)
{
    if (pl_fsetpos(MetaFile, &loc))
	plexit("plrender: fsetpos call failed");
}

static void
PageDecr(void)
{
    curpage--;
    cursub--;
    if (cursub < 1) {
	cursub = nsubx * nsuby;
	curdisp--;
    }
}

static void
PageIncr(void)
{
    cursub++;
    curpage++;
    if (cursub > nsubx * nsuby) {
	cursub = 1;
	curdisp++;
    }
}

/* Yes, finally done with the seek routines.  
	"BELIEVE IT!" - John Walker, from "Buckaroo Bonzai" */

/*----------------------------------------------------------------------*\
* ReadPageHeader()
*
* Reads the metafile, processing page header info.
* Assumes the file pointer is positioned immediately before a PAGE.
\*----------------------------------------------------------------------*/

static void
ReadPageHeader(void)
{
    U_CHAR c;
    U_SHORT page;
    U_LONG prevpage, nextpage;

    dbug_enter("ReadPageHeader");

/* Read page header */

    if (input_type == 0) {
	if (pl_fgetpos(MetaFile, &curpage_loc))
	    plexit("plrender: fgetpos call failed");
    }

    c = getcommand();
    if (c != PAGE && c != ADVANCE) 
	plexit("plrender: page advance expected; none found");

/* Update page/subpage counters and update page links */

    PageIncr();

    if (strcmp(mf_version, "1992a") >= 0) {
	if (strcmp(mf_version, "1993a") >= 0) {
	    plm_rd( pdf_rd_2bytes(pdfs, &page) );
	    plm_rd( pdf_rd_4bytes(pdfs, &prevpage) );
	    plm_rd( pdf_rd_4bytes(pdfs, &nextpage) );
	    prevpage_loc = prevpage;
	    nextpage_loc = nextpage;
	}
	else {
	    plm_rd( pdf_rd_2bytes(pdfs, &dum_ushort) );
	    plm_rd( pdf_rd_2bytes(pdfs, &dum_ushort) );
	}
    }

#ifdef DEBUG
    fprintf(stderr, "Now at page %d, disp %d\n", curpage, curdisp);
#endif
}

/*----------------------------------------------------------------------*\
* Init()
*
* Do initialization for main().
\*----------------------------------------------------------------------*/

static void
Init(int argc, char **argv)
{
    int i, status;

    dbug_enter("Init");

/* Process plrender command line options */

    status = plParseOpts(&argc, argv, mode_plrender, option_table, Usage);

/*
* We want the plplot command line options to supercede their possible
* counterparts in the metafile header.  So we defer parsing the
* rest of the command line until after we process the metafile header.
*/

    OpenMetaFile(&argc, argv);
    pdfs = pdf_finit(MetaFile);

/* Read header */

    if (ReadFileHeader())
	exit(EX_BADFILE);

/* Other miscellaneous housekeeping */

    if (addeof_beg) disp_beg += pages;
    if (addeof_end) disp_end += pages;

    plSetInternalOpt("-tcl_cmd", "set plw_create_proc plr_create");

/* Finally, give the rest of the command line to plplot to process. */

    status = plParseInternalOpts(&argc, argv, mode_plplot);

/* 
* At this point the only remaining argument in argv should be the program
* name (which should be first).  Anything else is an error.  Handle illegal
* flags first.
*/

    for (i = 1; i < argc; i++) {
	if ((argv)[i][0] == '-')
	    Usage(argv[i]);
    }

/* These must be extraneous file names */

    if (i > 1) {
	fprintf(stderr, "Only one filename argument accepted.\n");
	Usage("");
    }
}

/*----------------------------------------------------------------------*\
* OpenMetaFile()
*
* Attempts to open the named file.
* If the output file isn't already determined via the -i or -f flags, 
* we assume it's the last argument in argv[]. 
\*----------------------------------------------------------------------*/

static void
OpenMetaFile(int *p_argc, char **argv)
{
    char name[70];

    dbug_enter("OpenMetaFile");

    if ( ! strcmp(FileName, "-"))
	input_type = 1;

    if (input_type == 1)
	MetaFile = stdin;

    else {
	if (*FileName == '\0') {
	    if (*p_argc > 1) {
		strncpy(FileName, argv[*p_argc-1], sizeof(FileName) - 1);
		FileName[sizeof(FileName) - 1] = '\0';
		argv[*p_argc] = NULL;
		(*p_argc)--;
	    }
	}
	if (*FileName == '\0') {
	    fprintf(stderr, "%s: No filename specified.\n", program_name);
	    Usage("");
	    exit(EX_ARGSBAD);
	}

/*
* Try to read named Metafile.  The following cases are checked in order:
*	<FileName>
*	<FileName>.1
*	<FileName>.plm
*	<FileName>.plm.1
*/
#ifdef DEBUG
	fprintf(stderr, "Trying to open metafile %s.\n", FileName);
#endif
	strncpy(name, FileName, sizeof(name) - 1);
	name[sizeof(name) - 1] = '\0';

	if ((MetaFile = fopen(FileName, "rb")) != NULL) {
	    return;
	}

	(void) sprintf(FileName, "%s.%i", name, (int) member);
	if ((MetaFile = fopen(FileName, "rb")) != NULL) {
	    (void) sprintf(BaseName, "%s", name);
	    is_family = 1;
	    return;
	}

	(void) sprintf(FileName, "%s.plm", name);
	if ((MetaFile = fopen(FileName, "rb")) != NULL) {
	    return;
	}

	(void) sprintf(FileName, "%s.plm.%i", name, (int) member);
	if ((MetaFile = fopen(FileName, "rb")) != NULL) {
	    (void) sprintf(BaseName, "%s.plm", name);
	    is_family = 1;
	    return;
	}

	fprintf(stderr, "Unable to open the requested metafile.\n");
	Usage("");
	exit(EX_BADFILE);
    }
}

/*----------------------------------------------------------------------*\
* ReadFileHeader()
*
* Checks file header.  Returns 1 if an error occured.
\*----------------------------------------------------------------------*/

static int
ReadFileHeader(void)
{
    char tag[80];

    dbug_enter("ReadFileHeader");

/* Read label field of header to make sure file is a PLPLOT metafile */

    plm_rd( pdf_rd_header(pdfs, mf_magic) );
    if (strcmp(mf_magic, PLMETA_HEADER)) {
	fprintf(stderr, "Not a PLPLOT metafile!\n");
	return 1;
    }

/* Read version field of header.  We need to check that we can read the
   metafile, in case this is an old version of plrender. */

    plm_rd( pdf_rd_header(pdfs, mf_version) );
    if (strcmp(mf_version, PLMETA_VERSION) > 0) {
	fprintf(stderr,
	    "Error: incapable of reading metafile version %s.\n", mf_version);
	fprintf(stderr, "Please obtain a newer copy of plrender.\n");
	return 1;
    }

/* Disable page seeking on versions without page links */

    if (strcmp(mf_version, "1993a") < 0) 
	no_pagelinks=1;

/* Return if metafile older than version 1992a (no tagged info). */

    if (strcmp(mf_version, "1992a") < 0) {
	return 0;
    }

/* Read tagged initialization info. */
/* This is an easy way to guarantee backward compatibility. */

    for (;;) {
	plm_rd( pdf_rd_header(pdfs, tag) );
	if (*tag == '\0')
	    break;

	if ( ! strcmp(tag, "pages")) {
	    plm_rd( pdf_rd_2bytes(pdfs, &dum_ushort) );
	    pages = dum_ushort;
	    continue;
	}

	if ( ! strcmp(tag, "xmin")) {
	    plm_rd( pdf_rd_2bytes(pdfs, &dum_ushort) );
	    xmin = dum_ushort;
	    continue;
	}

	if ( ! strcmp(tag, "xmax")) {
	    plm_rd( pdf_rd_2bytes(pdfs, &dum_ushort) );
	    xmax = dum_ushort;
	    continue;
	}

	if ( ! strcmp(tag, "ymin")) {
	    plm_rd( pdf_rd_2bytes(pdfs, &dum_ushort) );
	    ymin = dum_ushort;
	    continue;
	}

	if ( ! strcmp(tag, "ymax")) {
	    plm_rd( pdf_rd_2bytes(pdfs, &dum_ushort) );
	    ymax = dum_ushort;
	    continue;
	}

	if ( ! strcmp(tag, "pxlx")) {
	    plm_rd( pdf_rd_ieeef(pdfs, &pxlx) );
	    continue;
	}

	if ( ! strcmp(tag, "pxly")) {
	    plm_rd( pdf_rd_ieeef(pdfs, &pxly) );
	    continue;
	}

	if ( ! strcmp(tag, "width")) {
	    plm_rd( pdf_rd_1byte(pdfs, &dum_uchar) );
	    plwid(dum_uchar);
	    continue;
	}

/* Obsolete tags */

	if ( ! strcmp(tag, "orient")) {
	    plm_rd( pdf_rd_1byte(pdfs, &dum_uchar) );
	    continue;
	}

	if ( ! strcmp(tag, "aspect")) {
	    plm_rd( pdf_rd_ieeef(pdfs, &dum_float) );
	    continue;
	}

	fprintf(stderr, "Unrecognized PLPLOT metafile header tag.\n");
	exit(EX_BADFILE);
    }

    return 0;
}

/*----------------------------------------------------------------------*\
* Help()
*
* Print long help message.
\*----------------------------------------------------------------------*/

static void
Help(void)
{
    dbug_enter("Help");

    fprintf(stderr,
	    "\nUsage:\n        %s [%s options] [plplot options] [filename]\n",
	    program_name, program_name);

    if (mode_showall)
	mode_plplot |= PL_PARSE_SHOWALL;

    myHelp();
    plHelp(mode_plplot);
    myNotes();

    exit(1);
}

/*----------------------------------------------------------------------*\
* myHelp()
*
* Print long help message associated with plrender options only.
\*----------------------------------------------------------------------*/

static void
myHelp(void)
{
    PLOptionTable *tab;

    fprintf(stderr, "\n%s options:\n", program_name);
    for (tab = option_table; tab->opt; tab++) {
	if ( ! (tab->mode & PL_OPT_ENABLED))
	    continue;

	if ( ! mode_showall && (tab->mode & PL_OPT_INVISIBLE))
	    continue;

	if (tab->desc == NULL)
	    continue;

	if (tab->mode & PL_OPT_INVISIBLE) 
	    fprintf(stderr, " *  %-20s %s\n", tab->syntax, tab->desc);
	else 
	    fprintf(stderr, "    %-20s %s\n", tab->syntax, tab->desc);
    }
}

/*----------------------------------------------------------------------*\
* myNotes()
*
* Print notes associated with long help message for plrender.
\*----------------------------------------------------------------------*/

static void
myNotes(void)
{
    char **cpp;

    putc('\n', stderr);
    for (cpp = notes; *cpp; cpp++) {
	fputs(*cpp, stderr);
	putc('\n', stderr);
    }
    putc('\n', stderr);
}

/*----------------------------------------------------------------------*\
* Usage()
*
* Print usage & syntax message.
\*----------------------------------------------------------------------*/

static void
Usage(char *badOption)
{
    dbug_enter("Usage");

    if (*badOption != '\0')
	fprintf(stderr, "\n%s:  bad command line option \"%s\"\r\n",
		program_name, badOption);

    fprintf(stderr,
	    "\nUsage:\n        %s [%s options] [plplot options] [filename]\n",
	    program_name, program_name);

    if (mode_showall)
	mode_plplot |= PL_PARSE_SHOWALL;

    mySyntax();
    plSyntax(mode_plplot);

    fprintf(stderr, "\nType %s -h for a full description.\r\n\n",
	    program_name);

    exit(1);
}

/*----------------------------------------------------------------------*\
* mySyntax()
*
* Print syntax message appropriate for plrender options only.
\*----------------------------------------------------------------------*/

static void
mySyntax(void)
{
    PLOptionTable *tab;
    int col, len;

    fprintf(stderr, "\n%s options:", program_name);

    col = 80;
    for (tab = option_table; tab->opt; tab++) {
	if ( ! (tab->mode & PL_OPT_ENABLED))
	    continue;

	if ( ! mode_showall && (tab->mode & PL_OPT_INVISIBLE))
	    continue;

	if (tab->syntax == NULL)
	    continue;

	len = 3 + strlen(tab->syntax);		/* space [ string ] */
	if (col + len > 79) {
	    fprintf(stderr, "\r\n   ");		/* 3 spaces */
	    col = 3;
	}
	fprintf(stderr, " [%s]", tab->syntax);
	col += len;
    }
    fprintf(stderr, "\r\n");
}

/*----------------------------------------------------------------------*\
* Input handlers
\*----------------------------------------------------------------------*/

/*----------------------------------------------------------------------*\
* Opt_h()
*
* Performs appropriate action for option "h".
\*----------------------------------------------------------------------*/

static int
Opt_h(char *opt, char *optarg, void *client_data)
{
/* Help */

    Help();

    return 1;
}

/*----------------------------------------------------------------------*\
* Opt_v()
*
* Performs appropriate action for option "v".
\*----------------------------------------------------------------------*/

static int
Opt_v(char *opt, char *optarg, void *client_data)
{
/* Version */

    fprintf(stderr, "plplot metafile version: %s\n", PLMETA_VERSION);
    fprintf(stderr, "plplot library version: %s\n", PLPLOT_VERSION);
    exit(1);
    return 1;		/* This serves a purpose */
}

/*----------------------------------------------------------------------*\
* Opt_i()
*
* Performs appropriate action for option "i".
\*----------------------------------------------------------------------*/

static int
Opt_i(char *opt, char *optarg, void *client_data)
{
/* Input file */

    strncpy(FileName, optarg, sizeof(FileName) - 1);
    FileName[sizeof(FileName) - 1] = '\0';

    return 0;
}

/*----------------------------------------------------------------------*\
* Opt_b()
*
* Performs appropriate action for option "b".
\*----------------------------------------------------------------------*/

static int
Opt_b(char *opt, char *optarg, void *client_data)
{
/* Beginning page */

    if (*optarg == '-') {
	optarg++;
	addeof_beg = 1;
    }
    disp_beg = atoi(optarg);

    return 0;
}

/*----------------------------------------------------------------------*\
* Opt_e()
*
* Performs appropriate action for option "e".
\*----------------------------------------------------------------------*/

static int
Opt_e(char *opt, char *optarg, void *client_data)
{
/* Ending page */

    if (*optarg == '-') {
	optarg++;
	addeof_end = 1;
    }
    disp_end = atoi(optarg);

    return 0;
}

/*----------------------------------------------------------------------*\
* Opt_p()
*
* Performs appropriate action for option "p".
\*----------------------------------------------------------------------*/

static int
Opt_p(char *opt, char *optarg, void *client_data)
{
/* Specified page only */

    if (*optarg == '-') {
	optarg++;
	addeof_beg = 1;
	addeof_end = 1;
    }
    disp_beg = atoi(optarg);
    disp_end = disp_beg;

    return 0;
}
