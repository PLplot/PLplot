/* $Id$
 * $Log$
 * Revision 1.30  1993/08/13 04:42:52  mjl
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
 *
 * Revision 1.28  1993/08/09  22:15:11  mjl
 * Eliminated all vestiges of old clr/page syntax, in favor of eop/bop.
 *
 * Revision 1.27  1993/08/03  01:47:04  mjl
 * Changes to eliminate warnings when compiling with gcc -Wall.
 *
 * Revision 1.26  1993/07/31  08:20:53  mjl
 * Removed code that is now handled in the driver interface, also changes
 * to reflect new driver functions.
 *
 * Revision 1.25  1993/07/16  22:20:18  mjl
 * Eliminated obsolete flags and processing of metafile tags (still read for
 * backward compatibility).  To be replaced by operations in the driver
 * interface.
 *
 * Revision 1.24  1993/07/02  07:19:21  mjl
 * Changed over to new namespace, new options parser.  Some options handlers
 * removed (no longer necessary).
 *
 * Revision 1.23  1993/04/26  19:58:03  mjl
 * Fixes to allow (once again) output to stdout and plrender to function as
 * a filter.  A type flag was added to handle file vs stream differences.
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "plevent.h"
#include "metadefs.h"
#include "pdf.h"

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
static void	plresc_rgb	(void);
static void	plresc_ancol	(void);

/* Support functions */

static U_CHAR	getcommand	(void);
static void	ungetcommand	(U_CHAR);
static void	get_ncoords	(PLFLT *x, PLFLT *y, PLINT n);
static void	NextFamilyFile	(U_CHAR *);
static void	ReadPageHeader	(void);
static void	plr_KeyEH	(PLKey *, void *, int *);
static int	SeekToDisp	(long);
static void	SeekOnePage	();
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

static int Opt_h	(char *, char *);
static int Opt_v	(char *, char *);
static int Opt_i	(char *, char *);
static int Opt_p	(char *, char *);
static int Opt_mar	(char *, char *);

/* Global variables */
/* Mode flags for argument parsing */

static int mode_plrender = PL_PARSE_OVERRIDE;
static int mode_plplot = 0;
static int mode_showall = 0;

/* Page info */

static PLINT	disp_begin=1;	/* Where to start plotting */
static PLINT	disp_end=-1;	/* Where to stop (0 to disable) */
static PLINT	curdisp;	/* Current page number */
static PLINT	cursub;		/* Current subpage */
static PLINT	curpage;	/* Current plot number */
static PLINT	nsubx;		/* subpages in x */
static PLINT	nsuby;		/* subpages in y */
static PLINT	target_disp;	/* Page we are seeking to */
static PLINT	target_page;	/* Plot we are seeking to */
static PLINT	delta;		/* Number of pages to go forward/back */

static int	no_pagelinks;	/* Set if metafile doesn't have page links */
static int	end_of_page=1;	/* Set when we're at the end of a page */
static int	seek_mode;	/* Set after a seek, before a BOP */

static FPOS_T	prevpage_loc;	/* Byte position of previous page header */
static FPOS_T	curpage_loc;	/* Byte position of current page header */
static FPOS_T	nextpage_loc;	/* Byte position of next page header */

/* File info */

static int	input_type;	/* 0 for file, 1 for stream */
static FILE	*MetaFile;
static char	BaseName[70] = "", FamilyName[80] = "", FileName[90] = "";
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
static PLFLT	x[PL_MAXPOLYLINE], y[PL_MAXPOLYLINE];

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
    &mode_showall,
    PL_OPT_BOOL | PL_OPT_ENABLED | PL_OPT_INVISIBLE,
    "-showall",
    "Turns on invisible options" },
{
    "h",			/* Help */
    Opt_h,
    NULL,
    PL_OPT_FUNC | PL_OPT_ENABLED,
    "-h",
    "Print out this message" },
{
    "v",			/* Version */
    Opt_v,
    NULL,
    PL_OPT_FUNC | PL_OPT_ENABLED,
    "-v",
    "Print out the plrender version number" },
{
    "i",			/* Input file */
    Opt_i,
    NULL,
    PL_OPT_FUNC | PL_OPT_ENABLED | PL_OPT_ARG,
    "-i name",
    "Input filename" },
{
    "f",			/* Filter option */
    NULL,
    &input_type,
    PL_OPT_BOOL | PL_OPT_ENABLED | PL_OPT_ARG,
    "-f",
    "Filter option -- equivalent to \"-i - -o -\"" },
{
    "b",			/* Beginning page number */
    NULL,
    &disp_begin,
    PL_OPT_INT | PL_OPT_ENABLED | PL_OPT_ARG,
    "-b number",
    "Beginning page number" },
{
    "e",			/* End page number */
    NULL,
    &disp_end,
    PL_OPT_INT | PL_OPT_ENABLED | PL_OPT_ARG,
    "-e number",
    "End page number" },
{
    "p",			/* Specified page only */
    Opt_p,
    NULL,
    PL_OPT_FUNC | PL_OPT_ENABLED | PL_OPT_ARG,
    "-p page",
    "Plot given page only" },
{
    "mar",			/* Margin */
    Opt_mar,
    NULL,
    PL_OPT_FUNC | PL_OPT_ENABLED | PL_OPT_ARG,
    NULL,
    NULL },
{
    "jx",			/* Justification in x */
    Opt_mar,
    NULL,
    PL_OPT_FUNC | PL_OPT_ENABLED | PL_OPT_ARG,
    NULL,
    NULL },
{
    "jy",			/* Justification in y */
    Opt_mar,
    NULL,
    PL_OPT_FUNC | PL_OPT_ENABLED | PL_OPT_ARG,
    NULL,
    NULL },
{
    NULL,
    NULL,
    NULL,
    0,
    NULL,
    NULL }
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
	    if (!is_family)
		break;
	}

	if ((c == BOP || c == ADVANCE) && curdisp == disp_end)
	    break;

	process_next(c);
    }

/* Done */

    (void) fclose(MetaFile);
    if (strcmp(mf_version, "1993a") < 0) 
	plP_eop();

    plP_tidy();
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
* program would.  This results in a more robust and flexible API.  On
* the other hand, it is sometimes necessary to directly call low-level
* plplot routines to achieve certain special effects.  This is probably
* tolerable if used sparingly (and points to areas where the plplot API
* needs improvement!).
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

      case ADVANCE:
	plr_eop(c);
	plr_bop(c);
	break;

      case CHANGE_STATE:
	plr_state(getcommand());
	break;

      case NEW_COLOR0:
	plr_state(PLSTATE_COLOR0);
	break;

      case NEW_COLOR1:
	plr_state(PLSTATE_COLOR1);
	break;

      case NEW_WIDTH:
	plr_state(PLSTATE_WIDTH);
	break;

      case SWITCH_TO_TEXT:
      case SWITCH_TO_GRAPH:
	break;

      case ESCAPE:
	plr_esc(c);
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
* mapping driver interface function.
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
    if (disp_begin > 1) {
	if (no_pagelinks) 
	    plwarn("plrender: Metafile does not support page seeks");
	else {
	    ReadPageHeader();
	    SeekToDisp(disp_begin);
	}
    }
}

/*----------------------------------------------------------------------*\
* plr_line()
*
* Draw a line or polyline.
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
	    if (npts == PL_MAXPOLYLINE)
		break;

	    c1 = getcommand();
	    if (c1 != LINETO) {
		ungetcommand(c1);
		break;
	    }
	}
	break;

      case POLYLINE:
	plm_rd(pdf_rd_2bytes(MetaFile, &npts));
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
    PLINT i;
    short xs[PL_MAXPOLYLINE], ys[PL_MAXPOLYLINE];

    plm_rd(pdf_rd_2nbytes(MetaFile, (U_SHORT *) xs, n));
    plm_rd(pdf_rd_2nbytes(MetaFile, (U_SHORT *) ys, n));

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
    dbug_enter("plr_state");

    switch (op) {

    case PLSTATE_WIDTH:{
	U_SHORT width;

	plm_rd(pdf_rd_2bytes(MetaFile, &width));

	plwid(width);
	break;
    }

    case PLSTATE_COLOR0:{
	U_SHORT icol;
	U_CHAR icol0, r, g, b;

	if (strcmp(mf_version, "1993a") >= 0) {
	    plm_rd(pdf_rd_1byte(MetaFile, &icol0));

	    if (icol0 == PL_RGB_COLOR) {
		plm_rd(pdf_rd_1byte(MetaFile, &r));
		plm_rd(pdf_rd_1byte(MetaFile, &g));
		plm_rd(pdf_rd_1byte(MetaFile, &b));
		plrgb1(r, g, b);
	    }
	    else {
		plcol(icol0);
	    }
	}
	else {
	    plm_rd(pdf_rd_2bytes(MetaFile, &icol));
	    plcol(icol);
	}
	break;
    }

    case PLSTATE_COLOR1:
	break;
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

    plm_rd(pdf_rd_1byte(MetaFile, &op));
    switch (op) {

      case PLESC_SET_RGB:	/* Now obsolete */
	plresc_rgb();
	return;

      case PLESC_ALLOC_NCOL:	/* Now obsolete */
	plresc_ancol();
	return;

      case PLESC_SET_LPB:	/* Now obsolete */
	plm_rd(pdf_rd_2bytes(MetaFile, &dum_ushort));
	plm_rd(pdf_rd_2bytes(MetaFile, &dum_ushort));
	plm_rd(pdf_rd_2bytes(MetaFile, &dum_ushort));
	plm_rd(pdf_rd_2bytes(MetaFile, &dum_ushort));
	return;
    }
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

    plm_rd(pdf_rd_2bytes(MetaFile, &ired));
    plm_rd(pdf_rd_2bytes(MetaFile, &igreen));
    plm_rd(pdf_rd_2bytes(MetaFile, &iblue));

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

    plm_rd(pdf_rd_1byte(MetaFile, &icolor));
    plm_rd(pdf_rd_header(MetaFile, name));
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
    (void) sprintf(FileName, "%s.%i", FamilyName, member);

    if ((MetaFile = fopen(FileName, BINARY_READ)) == NULL) {
	is_family = 0;
	return;
    }
    if (ReadFileHeader()) {
	is_family = 0;
	return;
    }

/*
* If the family file was created correctly, the first instruction in the
* file MUST be an INITIALIZE.  We throw this away and put a page advance in
* its place.
*/
    *c = getcommand();
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
	direction_flag = 1;

    else if (key->string[0] == '-')
	direction_flag = -1;

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
		else if (direction_flag > 0)
		    target_disp = curdisp + input_num;
		else if (direction_flag < 0)
		    target_disp = curdisp - input_num;

		dun_seek = SeekToDisp(target_disp);
	    }
	}
	else {
	    target_disp = curdisp + 1;
	    dun_seek = SeekToDisp(target_disp);
	}
    }

/* Page backward */

    if (key->code == PLK_BackSpace ||
	key->code == PLK_Delete ||
	key->code == PLK_Prior) 
    {
	terminator_seen = 1;
	target_disp = curdisp - 1;
	dun_seek = SeekToDisp(target_disp);
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

static int
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
	return 1;

/* <Return> while drawing the last page */

    if ((delta >= 0) && (nextpage_loc == 0))
	return 1;

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

/* Prepare for backward seeks by seeking the start of the current page */
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
    return 1;
}

/*----------------------------------------------------------------------*\
* SeekOnePage()
*
* Seeks one page in appropriate direction, and updates delta.
* For out of bounds seeks, just stay on the boundary page (first or last).
* In the latter case, return 1, otherwise 0.
\*----------------------------------------------------------------------*/

static void
SeekOnePage()
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
	    plm_rd(pdf_rd_2bytes(MetaFile, &page));
	    plm_rd(pdf_rd_4bytes(MetaFile, &prevpage));
	    plm_rd(pdf_rd_4bytes(MetaFile, &nextpage));
	    prevpage_loc = prevpage;
	    nextpage_loc = nextpage;
	}
	else {
	    plm_rd(pdf_rd_2bytes(MetaFile, &dum_ushort));
	    plm_rd(pdf_rd_2bytes(MetaFile, &dum_ushort));
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
    if (ReadFileHeader())
	exit(EX_BADFILE);

/* Other miscellaneous housekeeping */

    plSetInternalOpt("-tcl_cmd", "set plw_create plr_create");

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
    dbug_enter("OpenMetaFile");

    if (!strcmp(FileName, "-"))
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
	strncpy(BaseName, FileName, sizeof(BaseName) - 1);
	BaseName[sizeof(BaseName) - 1] = '\0';

	if ((MetaFile = fopen(FileName, BINARY_READ)) != NULL) {
	    return;
	}

	(void) sprintf(FileName, "%s.%i", BaseName, member);
	if ((MetaFile = fopen(FileName, BINARY_READ)) != NULL) {
	    (void) sprintf(FamilyName, "%s", BaseName);
	    is_family = 1;
	    return;
	}

	(void) sprintf(FileName, "%s.plm", BaseName);
	if ((MetaFile = fopen(FileName, BINARY_READ)) != NULL) {
	    return;
	}

	(void) sprintf(FileName, "%s.plm.%i", BaseName, member);
	if ((MetaFile = fopen(FileName, BINARY_READ)) != NULL) {
	    (void) sprintf(FamilyName, "%s.plm", BaseName);
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

    plm_rd(pdf_rd_header(MetaFile, mf_magic));
    if (strcmp(mf_magic, PLMETA_HEADER)) {
	fprintf(stderr, "Not a PLPLOT metafile!\n");
	return (1);
    }

/* Read version field of header.  We need to check that we can read the
   metafile, in case this is an old version of plrender. */

    plm_rd(pdf_rd_header(MetaFile, mf_version));
    if (strcmp(mf_version, PLMETA_VERSION) > 0) {
	fprintf(stderr,
	    "Error: incapable of reading metafile version %s.\n", mf_version);
	fprintf(stderr, "Please obtain a newer copy of plrender.\n");
	return (1);
    }

/* Disable page seeking on versions without page links */

    if (strcmp(mf_version, "1993a") < 0) 
	no_pagelinks=1;

/* Return if metafile older than version 1992a (no tagged info). */

    if (strcmp(mf_version, "1992a") < 0) {
	return (0);
    }

/* Read tagged initialization info. */
/* This is an easy way to guarantee backward compatibility. */

    for (;;) {
	plm_rd(pdf_rd_header(MetaFile, tag));
	if (*tag == '\0')
	    break;

	if (!strcmp(tag, "xmin")) {
	    plm_rd(pdf_rd_2bytes(MetaFile, &dum_ushort));
	    xmin = dum_ushort;
	    continue;
	}

	if (!strcmp(tag, "xmax")) {
	    plm_rd(pdf_rd_2bytes(MetaFile, &dum_ushort));
	    xmax = dum_ushort;
	    continue;
	}

	if (!strcmp(tag, "ymin")) {
	    plm_rd(pdf_rd_2bytes(MetaFile, &dum_ushort));
	    ymin = dum_ushort;
	    continue;
	}

	if (!strcmp(tag, "ymax")) {
	    plm_rd(pdf_rd_2bytes(MetaFile, &dum_ushort));
	    ymax = dum_ushort;
	    continue;
	}

	if (!strcmp(tag, "pxlx")) {
	    plm_rd(pdf_rd_ieeef(MetaFile, &pxlx));
	    continue;
	}

	if (!strcmp(tag, "pxly")) {
	    plm_rd(pdf_rd_ieeef(MetaFile, &pxly));
	    continue;
	}

	if (!strcmp(tag, "width")) {
	    plm_rd(pdf_rd_1byte(MetaFile, &dum_uchar));
	    plwid(dum_uchar);
	    continue;
	}

/* Obsolete tags */

	if (!strcmp(tag, "orient")) {
	    plm_rd(pdf_rd_1byte(MetaFile, &dum_uchar));
	    continue;
	}

	if (!strcmp(tag, "aspect")) {
	    plm_rd(pdf_rd_ieeef(MetaFile, &dum_float));
	    continue;
	}

	fprintf(stderr, "Unrecognized PLPLOT metafile header tag.\n");
	exit(EX_BADFILE);
    }

    return (0);
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
Opt_h(char *opt, char *optarg)
{
    dbug_enter("Opt_h");

/* Help */

    Help();

    return(1);
}

/*----------------------------------------------------------------------*\
* Opt_v()
*
* Performs appropriate action for option "v".
\*----------------------------------------------------------------------*/

static int
Opt_v(char *opt, char *optarg)
{
    dbug_enter("Opt_v");

/* Version */

    fprintf(stderr, "plplot metafile version: %s\n", PLMETA_VERSION);
    fprintf(stderr, "plplot library version: %s\n", PLPLOT_VERSION);
    exit(1);
    return(1);		/* This serves a purpose */
}

/*----------------------------------------------------------------------*\
* Opt_i()
*
* Performs appropriate action for option "i".
\*----------------------------------------------------------------------*/

static int
Opt_i(char *opt, char *optarg)
{
    dbug_enter("Opt_i");

/* Input file */

    strncpy(FileName, optarg, sizeof(FileName) - 1);
    FileName[sizeof(FileName) - 1] = '\0';

    return(0);
}

/*----------------------------------------------------------------------*\
* Opt_p()
*
* Performs appropriate action for option "p".
\*----------------------------------------------------------------------*/

static int
Opt_p(char *opt, char *optarg)
{
    dbug_enter("Opt_p");

/* Specified page only */

    disp_begin = atoi(optarg);
    disp_end = disp_begin;

    return(0);
}

/*----------------------------------------------------------------------*\
* Opt_mar()
*
* Obsolete
\*----------------------------------------------------------------------*/

static int
Opt_mar(char *opt, char *optarg)
{
    dbug_enter("Opt_mar");

    fprintf(stderr,
    "-mar, -jx, and -jy options are obsolete -- use -wdev instead\n");

    return 1;
}

