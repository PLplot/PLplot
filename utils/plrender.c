/* $Id$
   $Log$
   Revision 1.6  1993/01/23 06:16:08  mjl
   Formatting changes only to pltek.  plrender changes include: support for
   polylines (even converts connected lines while reading into polylines for
   better response), new color model support, event handler support.  New
   events recognized allow seeking to arbitrary locations in the file (absolute
   or relative), and backward.  Some old capabilities (no longer useful)
   eliminated.

 * Revision 1.5  1992/11/07  08:08:55  mjl
 * Fixed orientation code, previously it rotated plot in the wrong direction.
 * It now supports rotations both ways or upside down (-ori 1, -ori -1, -ori 2).
 * Also eliminated some redundant code.
 *
 * Revision 1.4  1992/10/29  15:56:16  mjl
 * Gave plrender an ID tag.
 *
 * Revision 1.3  1992/10/12  17:12:58  mjl
 * Rearranged order of header file inclusion.
 * #include "plplot.h" must come first!!
 *
 * Revision 1.2  1992/09/29  04:46:46  furnish
 * Massive clean up effort to remove support for garbage compilers (K&R).
 *
 * Revision 1.1  1992/05/20  21:35:59  furnish
 * Initial checkin of the whole PLPLOT project.
 *
*/

/*
    plrender.c

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

    This file contains the code to render a PLPLOT metafile, written by
    the metafile driver, plmeta.c.
*/

char ident[] = "@(#) $Id$";

#include "plplot.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "plevent.h"
#include "metadefs.h"
#include "pdf.h"

#define PROGRAM_NAME 	"plrender"
#define PROGRAM_VERSION PLPLOT_VERSION
#define DO_ASP_SCALING

/* Static function prototypes. */
/* INDENT OFF */
/* These handle the command loop */

static void	process_next	(U_CHAR c);
static void	plr_init	(U_CHAR c);
static void	plr_line	(U_CHAR c);
static void	plr_clr		(U_CHAR c);
static void	plr_page	(U_CHAR c);
static void	plr_color	(U_CHAR c);
static void	plr_switch	(U_CHAR c);
static void	plr_width	(U_CHAR c);
static void	plr_esc		(U_CHAR c);
static void	plresc_rgb	(void);
static void	plresc_ancol	(void);

/* Support functions */

static void	Init		(void);
static void	GetOpts		(int, char **);
static U_CHAR	getcommand	(void);
static void	ungetcommand	(U_CHAR);
static void	get_ncoords	(PLFLT *x, PLFLT *y, PLINT n);
static void	NextFamilyFile	(U_CHAR *);
static int	ReadHeader	(void);
static void	plr_KeyEH	(PLKey *, void *, int *);
static void	ReadPageHeader	(void);
static void	SeekToPage	(long);
static void	check_alignment (FILE *);

static void	Syntax		(char *);
static void	Help		(void);
static void	Version		(void);
static void	ParseOpts	(int *, char ***);
static void	GetOption	(char **, int *, char ***);

/* Global variables */

/* Page info */

static PLINT	page_begin=1;	/* Where to start plotting */
static PLINT	page_end;	/* Where to stop (0 to disable) */
static PLINT	page;		/* Current page number */
static PLINT	target_page;	/* Page we are seeking to */
static int	no_page;	/* Flag to skip next new page */
static U_LONG	prevpage_loc;	/* Byte position of previous page */
static U_LONG	nextpage_loc;	/* Byte position of next page */

/* File info */

static FILE	*MetaFile;
static char	*FileName, *FileNameOut;
static char	FamilyNameIn[80] = "", FileNameIn[90] = "";
static PLINT	familyout, numberout=1, bytemax=PL_FILESIZE_KB*1000;
static PLINT	familyin, numberin=1;
static char	mf_magic[40], mf_version[40];

/* Dummy vars for reading stuff that is to be thrown away */

static U_CHAR	dum_uchar;
static U_SHORT	dum_ushort;
static char	dum_char80[80];
static float	dum_float;

/* Plot parameters */

static PLFLT	xdpi, ydpi;
static PLINT	xwid, ywid, xoff, yoff;
static PLINT	nopause;
static char	*devname;
static int	orient, set_orient;
static int	color;
static int	bgcolorset, bgcolor = 255;
static float	aspect = 0.0, set_aspect = 0.0;
static int	orientset, aspectset, set_orientset, set_aspectset, is_filter;
static int	width, set_width, widthset, set_widthset;
static PLINT	packx=1, packy=1;
static U_SHORT	lpbpxmi, lpbpxma, lpbpymi, lpbpyma;

/* Plot dimensions */

static U_SHORT	xmin = 0;
static U_SHORT	xmax = PLMETA_X_OLD;
static U_SHORT	ymin = 0;
static U_SHORT	ymax = PLMETA_Y_OLD;
static PLINT	xlen, ylen;

static float	pxlx = PIXEL_RES_X_OLD;
static float	pxly = PIXEL_RES_Y_OLD;

static PLFLT	dev_xpmm, dev_ypmm;
static PLINT	dev_xmin, dev_xmax, dev_ymin, dev_ymax;
static PLFLT	vpxmin, vpxmax, vpxlen, vpymin, vpymax, vpylen;
static PLFLT	mar=0.0, jx=0.5, jy=0.5;

/* Miscellaneous */

static U_CHAR 	c_old, c1;
static U_SHORT	npts;
static int	direction_flag, isanum;
static char	num_buffer[20];
static PLFLT	x[PL_MAXPOLYLINE], y[PL_MAXPOLYLINE];

/* Exit codes */

#define	EX_SUCCESS	0		/* success! */
#define	EX_ARGSBAD	1		/* invalid args */
#define	EX_BADFILE	2		/* invalid filename */

/* INDENT ON */
/*----------------------------------------------------------------------*\
* main()
*
* plrender -- render a PLPLOT metafile.
\*----------------------------------------------------------------------*/

main(int argc, char *argv[])
{
    static U_CHAR c;

/* Initialize */

    GetOpts(argc, argv);
    Init();

/*
* Read & process metafile commands.
* If familying is turned on, the end of one member file is just treated as
* a page break assuming the next member file exists.
*/

    for (;;) {
	c_old = c;
	c = getcommand();

	if (c == CLOSE) {
	    if (familyin)
		NextFamilyFile(&c);
	    if (!familyin)
		break;
	}

	if ((page > page_end) && (page_end > 0))
	    break;

	process_next(c);
    }

/* Done */

    (void) fclose(MetaFile);
    plend();
    exit(EX_SUCCESS);
}

/*----------------------------------------------------------------------*\
* 			Process the command loop
\*----------------------------------------------------------------------*/

/*----------------------------------------------------------------------*\
* process_next()
*
* Process a command
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

      case CLEAR:
	plr_clr(c);
	break;

      case PAGE:
      case ADVANCE:
	plr_page(c);
	break;

      case NEW_COLOR0:
      case NEW_COLOR1:
	plr_color(c);
	break;

      case SWITCH_TO_TEXT:
      case SWITCH_TO_GRAPH:
	plr_switch(c);
	break;

      case NEW_WIDTH:
	plr_width(c);
	break;

      case ESCAPE:
	plr_esc(c);
	break;

      default:
	plwarn("process_next: Unrecognized command\n");
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
    float dev_aspect, ratio;

/* Set up misc. parameters */

    plspage(xdpi, ydpi, xwid, ywid, xoff, yoff);
    plspause(!nopause);

/* Register event handler */

    plsKeyEH(plr_KeyEH, NULL);

/* The orientation set from the command line overrides any the user may have
   set before initializing plplot. */

    if (set_orientset)
	orient = set_orient;

/* Same for width */

    if (set_widthset) {
	width = set_width;
	plwid(width);
    }

/* Force color display for those devices that default to mono (e.g. ps) */

    if (color)
	plscolor(color);

/* Set background color */

    if (bgcolorset)
	plscol0(0, bgcolor, bgcolor, bgcolor);

/* Set up for write.  Filter option always honored. */

    if (is_filter) {
	plsfile(stdout);
	familyout = 0;
    }
    else if (FileNameOut != NULL) {
	if (!strcmp(FileNameOut, "-")) {
	    plsfile(stdout);
	    familyout = 0;
	}
	else
	    plsfnam(FileNameOut);
    }
    if (familyout)
	plsfam(familyout, numberout, bytemax);

/* Start up plplot */

    plstart(devname, packx, packy);
    pladv(0);

/* Aspect ratio scaling */

/* If the user has not set the aspect ratio in the code via plsasp() it
   will be zero, and is set to the natural ratio of the metafile coordinate
   system.  The aspect ratio set from the command line overrides this. */

    if (aspect == 0.0)
	aspect = ((ymax - ymin) / pxly) / ((xmax - xmin) / pxlx);

    if (set_aspectset)
	aspect = set_aspect;

    if (aspect <= 0.)
	fprintf(stderr,
		"Error in aspect ratio setting, aspect = %f\n", aspect);

    if (orient == 1 || orient == -1)
	aspect = 1.0 / aspect;

/* Aspect ratio of output device */

    gphy(&dev_xmin, &dev_xmax, &dev_ymin, &dev_ymax);
    gpixmm(&dev_xpmm, &dev_ypmm);

    dev_aspect = ((dev_ymax - dev_ymin) / dev_ypmm) /
	((dev_xmax - dev_xmin) / dev_xpmm);

    if (dev_aspect <= 0.)
	fprintf(stderr,
	      "Error in aspect ratio setting, dev_aspect = %f\n", dev_aspect);

    ratio = aspect / dev_aspect;

/* This is the default case; come back to here if things mess up */

    vpxlen = 1.0;
    vpylen = 1.0;
    vpxmin = 0.5 - vpxlen / 2.;
    vpymin = 0.5 - vpylen / 2.;
    vpxmax = vpxmin + vpxlen;
    vpymax = vpymin + vpylen;

    xlen = xmax - xmin;
    ylen = ymax - ymin;

/* If ratio < 1, you are requesting an aspect ratio (y/x) less than the natural
   aspect ratio of the output device, and you will need to reduce the length
    in y correspondingly.  Similarly, for ratio > 1, x must be reduced. */

/* Note that unless the user overrides, the default is to *preserve* the
   aspect ratio of the original device (plmeta output file).  Thus you
   automatically get all physical coordinate plots to come out correctly.
*/

#ifdef DO_ASP_SCALING
    if (ratio <= 0)
	fprintf(stderr, "Error in aspect ratio setting, ratio = %f\n", ratio);
    else if (ratio < 1)
	vpylen = ratio;
    else
	vpxlen = 1. / ratio;

    if (mar > 0.0 && mar < 1.0) {
	vpxlen *= (1.0 - mar);
	vpylen *= (1.0 - mar);
    }

    vpxmin = MAX(0., jx - vpxlen / 2.0);
    vpxmax = MIN(1., vpxmin + vpxlen);
    vpxmin = vpxmax - vpxlen;

    vpymin = MAX(0., jy - vpylen / 2.0);
    vpymax = MIN(1., vpymin + vpylen);
    vpymin = vpymax - vpylen;
#endif

    plvpor(vpxmin, vpxmax, vpymin, vpymax);
    plwind((PLFLT) xmin, (PLFLT) xmax, (PLFLT) ymin, (PLFLT) ymax);

    no_page++;			/* skip next new page */
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
	plm_rd(read_2bytes(MetaFile, &npts));
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
* Read n coordinate vectors and properly orient.
\*----------------------------------------------------------------------*/

static void
get_ncoords(PLFLT *x, PLFLT *y, PLINT n)
{
    PLINT i;
    PLSHORT xs[PL_MAXPOLYLINE], ys[PL_MAXPOLYLINE];

    plm_rd(read_2nbytes(MetaFile, (U_SHORT *) xs, n));
    plm_rd(read_2nbytes(MetaFile, (U_SHORT *) ys, n));

    switch (orient) {

      case -1:
	for (i = 0; i < n; i++) {
	    x[i] = xmin + (ymax - ys[i]) * xlen / ylen;
	    y[i] = ymin + (xs[i] - xmin) * ylen / xlen;
	}
	return;

      case 1:
	for (i = 0; i < n; i++) {
	    x[i] = xmax - (ymax - ys[i]) * xlen / ylen;
	    y[i] = ymax - (xs[i] - xmin) * ylen / xlen;
	}
	return;

      case 2:
      case -2:
	for (i = 0; i < n; i++) {
	    x[i] = xmin + (xmax - xs[i]);
	    y[i] = ymin + (ymax - ys[i]);
	}
	return;

      default:
	for (i = 0; i < n; i++) {
	    x[i] = xs[i];
	    y[i] = ys[i];
	}
    }
}

/*----------------------------------------------------------------------*\
* plr_clr()
*
* Clear screen.
\*----------------------------------------------------------------------*/

static void
plr_clr(U_CHAR c)
{
    plclr();
}

/*----------------------------------------------------------------------*\
* plr_page()
*
* Page advancement.
\*----------------------------------------------------------------------*/

static void
plr_page(U_CHAR c)
{
    page++;
    ReadPageHeader();

/* Since a PAGE is always first, we can seek to the starting page here */

    if (page == 1 && page_begin > 0)
	SeekToPage(page_begin);

/* May want to skip the new page in some circumstances */

    if (no_page) {
	no_page = 0;
	return;
    }

/* Actually advance the page */

    if (c == ADVANCE || ((packx > 1 || packy > 1) && page == 1))
	pladv(0);
    else
	plpage();

    plvpor(vpxmin, vpxmax, vpymin, vpymax);
    plwind((PLFLT) xmin, (PLFLT) xmax, (PLFLT) ymin, (PLFLT) ymax);
}

/*----------------------------------------------------------------------*\
* plr_color()
*
* Change color.
\*----------------------------------------------------------------------*/

static void
plr_color(U_CHAR c)
{
    U_SHORT icol;
    U_CHAR icol0, r, g, b;

    if (c == NEW_COLOR1) {
	plwarn("No support for cmap 1 yet");
	return;
    }
    if (strcmp(mf_version, "1993a") >= 0) {
	plm_rd(read_1byte(MetaFile, &icol0));

	if (icol0 == PL_RGB_COLOR) {
	    plm_rd(read_1byte(MetaFile, &r));
	    plm_rd(read_1byte(MetaFile, &g));
	    plm_rd(read_1byte(MetaFile, &b));
	    plrgb1(r, g, b);
	}
	else {
	    plcol(icol0);
	}
    }
    else {
	plm_rd(read_2bytes(MetaFile, &icol));
	plcol(icol);
    }
}

/*----------------------------------------------------------------------*\
* plr_switch()
*
* Switch between graphics/text modes.
\*----------------------------------------------------------------------*/

static void
plr_switch(U_CHAR c)
{
    if (c == SWITCH_TO_TEXT)
	pltext();

    else if (c == SWITCH_TO_GRAPH)
	plgra();
}

/*----------------------------------------------------------------------*\
* plr_width()
*
* Change pen width.
\*----------------------------------------------------------------------*/

static void
plr_width(U_CHAR c)
{
    U_SHORT width;

    plm_rd(read_2bytes(MetaFile, &width));

    plwid(width);
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

    plm_rd(read_1byte(MetaFile, &op));
    switch (op) {

      case PL_SET_RGB:
	plresc_rgb();
	return;

      case PL_ALLOC_NCOL:
	plresc_ancol();
	return;

      case PL_SET_LPB:
	plm_rd(read_2bytes(MetaFile, &lpbpxmi));
	plm_rd(read_2bytes(MetaFile, &lpbpxma));
	plm_rd(read_2bytes(MetaFile, &lpbpymi));
	plm_rd(read_2bytes(MetaFile, &lpbpyma));
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

    plm_rd(read_2bytes(MetaFile, &ired));
    plm_rd(read_2bytes(MetaFile, &igreen));
    plm_rd(read_2bytes(MetaFile, &iblue));

    red = (double) ired / 65535.;
    green = (double) igreen / 65535.;
    blue = (double) iblue / 65535.;

    plrgb((PLFLT) red, (PLFLT) green, (PLFLT) blue);
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

    plm_rd(read_1byte(MetaFile, &icolor));
    plm_rd(read_header(MetaFile, name));
}

/*----------------------------------------------------------------------*\
* 			Support routines
\*----------------------------------------------------------------------*/

/*----------------------------------------------------------------------*\
* Init()
*
* Do initialization for main().
\*----------------------------------------------------------------------*/

static void
Init(void)
{
/* Set up for read.  Filter option always honored. */

    if (is_filter)
	MetaFile = stdin;

    else if (FileName == NULL) {
	Syntax("");
	exit(EX_ARGSBAD);
    }
    else if (!strcmp(FileName, "-"))
	MetaFile = stdin;

/* Next try to read named Metafile.  If it fails, then try again for
   a family member file. */

    else if ((MetaFile = fopen(FileName, BINARY_READ)) == NULL) {

	strncpy(FamilyNameIn, FileName, sizeof(FamilyNameIn) - 1);
	FamilyNameIn[sizeof(FamilyNameIn) - 1] = '\0';
	familyin++;

	(void) sprintf(FileNameIn, "%s.%i", FamilyNameIn, numberin);

	if ((MetaFile = fopen(FileNameIn, BINARY_READ)) == NULL) {
	    fprintf(stderr, "Unable to open the requested metafile.\n");
	    exit(EX_BADFILE);
	}
    }

/* Check validity of header */

    if (ReadHeader())
	exit(EX_BADFILE);
}

/*----------------------------------------------------------------------*\
* NextFamilyFile()
*
* Start the next family if it exists.
\*----------------------------------------------------------------------*/

static void
NextFamilyFile(U_CHAR *c)
{
    (void) fclose(MetaFile);
    numberin++;
    (void) sprintf(FileNameIn, "%s.%i", FamilyNameIn, numberin);

    if ((MetaFile = fopen(FileNameIn, BINARY_READ)) == NULL) {
	familyin = 0;
	return;
    }
    if (ReadHeader()) {
	familyin = 0;
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
* ReadHeader()
*
* Checks file header.  Returns 1 if an error occured.
\*----------------------------------------------------------------------*/

static int
ReadHeader(void)
{
    char tag[80];

/* Read label field of header to make sure file is a PLPLOT metafile */

    plm_rd(read_header(MetaFile, mf_magic));
    if (strcmp(mf_magic, PLPLOT_HEADER)) {
	fprintf(stderr, "Not a PLPLOT metafile!\n");
	return (1);
    }

/* Read version field of header.  We need to check that we can read the
   metafile, in case this is an old version of plrender. */

    plm_rd(read_header(MetaFile, mf_version));
    if (strcmp(mf_version, PLPLOT_VERSION) > 0) {
	fprintf(stderr,
	    "Error: incapable of reading metafile version %s.\n", mf_version);
	fprintf(stderr, "Please obtain a newer copy of plrender.\n");
	return (1);
    }

/* Return if metafile older than version 1992a (no tagged info). */

    if (strcmp(mf_version, "1992a") < 0) {
	return (0);
    }

/* Read tagged initialization info. */
/* This is an easy way to guarantee backward compatibility. */

    for (;;) {
	plm_rd(read_header(MetaFile, tag));
	if (*tag == '\0')
	    break;

	if (!strcmp(tag, "xmin")) {
	    plm_rd(read_2bytes(MetaFile, &xmin));
	    continue;
	}

	if (!strcmp(tag, "xmax")) {
	    plm_rd(read_2bytes(MetaFile, &xmax));
	    continue;
	}

	if (!strcmp(tag, "ymin")) {
	    plm_rd(read_2bytes(MetaFile, &ymin));
	    continue;
	}

	if (!strcmp(tag, "ymax")) {
	    plm_rd(read_2bytes(MetaFile, &ymax));
	    continue;
	}

	if (!strcmp(tag, "pxlx")) {
	    plm_rd(read_ieeef(MetaFile, &pxlx));
	    continue;
	}

	if (!strcmp(tag, "pxly")) {
	    plm_rd(read_ieeef(MetaFile, &pxly));
	    continue;
	}

	if (!strcmp(tag, "aspect")) {
	    plm_rd(read_ieeef(MetaFile, &aspect));
	    continue;
	}

	if (!strcmp(tag, "width")) {
	    plm_rd(read_1byte(MetaFile, (U_CHAR *) &width));
	    continue;
	}

	if (!strcmp(tag, "orient")) {
	    plm_rd(read_1byte(MetaFile, (U_CHAR *) &orient));
	    continue;
	}

	fprintf(stderr, "Unrecognized PLPLOT metafile header tag.\n");
	exit(1);
    }

    return (0);
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
* No client data is passed in this case, although a test case is
* illustrated. 
*
* Illegal input is ignored.
\*----------------------------------------------------------------------*/

static void
plr_KeyEH(PLKey *key, void *client_data, int *p_exit_eventloop)
{
    char *tst = (char *) client_data;
    int input_num;

/* TEST */

    if (tst != NULL) {
	pltext();
	printf("tst string: %s\n", tst);
	plgra();
    }

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

/* Seek to specified page, or page advance.
   Not done until user hits <return>.
   When this routine is called, the metafile pointer is situated just past
   the page header for the next page, so we need to adjust the relative
   targets downward by one.

   Need to check for both <LF> and <CR> for portability.
*/

    if (key->code == PLK_Return ||
	key->code == PLK_Linefeed ||
	key->code == PLK_Next)
    {
	if (isanum) {
	    input_num = atoi(num_buffer);
	    if (input_num > 0) {
		if (direction_flag == 0)
		    target_page = input_num;
		else if (direction_flag > 0)
		    target_page = page + input_num - 1;
		else if (direction_flag < 0)
		    target_page = page - input_num - 1;

		SeekToPage(target_page);
	    }
	}
	*p_exit_eventloop = TRUE;
    }

/* Page backward */

    if (key->code == PLK_BackSpace ||
	key->code == PLK_Delete ||
	key->code == PLK_Prior) 
    {
	*p_exit_eventloop = TRUE;
	target_page = page - 2;
	SeekToPage(target_page);
    }

/* DEBUG */

#ifdef DEBUG
    pltext();
    printf("key->code = %x, target_page = %d, page = %d,\
           exit_eventloop = %d\n", key->code, target_page, page,
	   *p_exit_eventloop);
    plgra();
#endif

/* Cleanup */

    if (*p_exit_eventloop == TRUE) {
	num_buffer[0] = '\0';
	direction_flag = 0;
	isanum = 0;
    }
}

/*----------------------------------------------------------------------*\
* SeekToPage()
*
* Seek to 'target_page'.
\*----------------------------------------------------------------------*/

static void
SeekToPage(long target_page)
{
    U_CHAR c;
    long delta;

    if (strcmp(mf_version, "1993a") < 0) {
	plwarn("plrender: Metafile does not support page seeks\n");
	return;
    }

    delta = target_page - page;

    while (delta != 0) {
	if (delta > 0) {
	    if (nextpage_loc == 0) 
		break;
	    fseek(MetaFile, nextpage_loc, 0);
	    page++;
	}
	else {
	    if (prevpage_loc == 0) 
		break;
	    fseek(MetaFile, prevpage_loc, 0);
	    page--;
	}
	c = getcommand();
	if (c != PAGE && c != ADVANCE) {
	    plwarn("plrender: page advance expected; none found\n");
	    return;
	}
	ReadPageHeader();
	delta = target_page - page;
    }
}

/*----------------------------------------------------------------------*\
* ReadPageHeader()
*
* Reads the metafile, processing page header info.
* Assumes a 'page' or 'advance' command encountered immediately before.
\*----------------------------------------------------------------------*/

static void
ReadPageHeader(void)
{
    U_SHORT curpage;

/* Read page header */

    if (strcmp(mf_version, "1992a") >= 0) {
	if (strcmp(mf_version, "1993a") >= 0) {
	    plm_rd(read_2bytes(MetaFile, &curpage));
	    plm_rd(read_4bytes(MetaFile, &prevpage_loc));
	    plm_rd(read_4bytes(MetaFile, &nextpage_loc));
	    if (curpage != page && !familyin) {
		plwarn("plrender: page counter hosed\n");
	    }
	}
	else {
	    plm_rd(read_2bytes(MetaFile, &dum_ushort));
	    plm_rd(read_2bytes(MetaFile, &dum_ushort));
	}
    }
}

/*----------------------------------------------------------------------*\
* check_alignment()
*
* Reads the next byte and aborts if it is not an END_OF_HEADER.
\*----------------------------------------------------------------------*/

static void
check_alignment(FILE *file)
{
    U_CHAR c;

    plm_rd(read_1byte(file, &c));
    if (c != END_OF_FIELD)
	plexit("check_alignment: Metafile alignment problem");
}

/*----------------------------------------------------------------------*\
* Startup code.  The following routines are modelled after the startup
* code for 'xterm.c', part of the X-windows Version 11 distribution.
* The copyright notice for 'xterm.c' is as follows:

Copyright 1987, 1988 by Digital Equipment Corporation, Maynard, Massachusetts,
and the Massachusetts Institute of Technology, Cambridge, Massachusetts.

                        All Rights Reserved

* The full permission notice is given in the plplot documentation.
\*----------------------------------------------------------------------*/

/*----------------------------------------------------------------------*\
* Options data structure definition.
\*----------------------------------------------------------------------*/
/* INDENT OFF */

static struct _options {
    char *opt;
    char *desc;
} options[] = {
{ "-h",			"Print out this message" },
{ "-v",			"Print out version info" },
{ "-dev name",		"Output device name"},
{ "-i name",		"Input filename" },
{ "-o name",		"Output filename, or X server to contact" },
{ "-f",			"Filter option -- equivalent to \"-i - -o -\"" },
{ "-geo geom",		"X window size, in pixels (e.g. -geo 400x400)" },
{ "-b number",		"Beginning page number" },
{ "-e number",		"End page number" },
{ "-p page",		"Plot given page only" },
{ "-a aspect",		"Plot aspect ratio" },
{ "-mar margin",	"Total fraction of page to reserve for margins" },
{ "-ori orient",	"Plot orientation (0=landscape, 1,-1=portrait)" },
{ "-width width",	"Pen width (1 <= width <= 10)" },
{ "-color",		"Forces color output (some drivers default to mono)"},
{ "-bg color",		"Background color (0=black, 255=white [def])"},
{ "-jx number",		"Justification of plot on page in x (0.0 to 1.0)" },
{ "-jy number",		"Justification of plot on page in y (0.0 to 1.0)" },
{ "-px number",		"Plots per page in x" },
{ "-py number",		"Plots per page in y" },
{ "-fam",		"Create a family of output files" },
{ "-fsiz size",		"Output family file size in MB (e.g. -fsiz 1.0)" },
{ "-fmem member",	"Starting family member number on input [1]" },
{ "-np",		"No pause between pages" },
{NULL, NULL }};

static char *notes[] = {
"All parameters must be white-space delimited.  If you omit the \"-i\" flag,",
"the filename parameter must come last.  Specifying \"-\" for the input or",
"output filename means use stdin or stdout, respectively.  Only one filename",
"parameter is recognized.  Not all options valid with all drivers.",
"Please see the man pages for more detail.",
NULL};

/* INDENT ON */
/*----------------------------------------------------------------------*\
* Syntax()
*
* Print short help message.
\*----------------------------------------------------------------------*/

static void
Syntax(char *badOption)
{
    struct _options *opt;
    int col, len;

    if (*badOption != '\0')
	fprintf(stderr, "%s:  bad command line option \"%s\"\r\n\n",
		PROGRAM_NAME, badOption);

    fprintf(stderr, "\nusage:  %s", PROGRAM_NAME);
    col = 8 + strlen(PROGRAM_NAME);
    for (opt = options; opt->opt; opt++) {
	len = 3 + strlen(opt->opt);	/* space [ string ] */
	if (col + len > 79) {
	    fprintf(stderr, "\r\n   ");	/* 3 spaces */
	    col = 3;
	}
	fprintf(stderr, " [%s]", opt->opt);
	col += len;
    }

    len = 3 + strlen("[filename]");
    if (col + len > 79) {
	fprintf(stderr, "\r\n   ");
	col = 3;
    }
    fprintf(stderr, " %s", "[filename]");
    col += len;

    fprintf(stderr, "\r\n\nType %s -h for a full description.\r\n\n",
	    PROGRAM_NAME);
    exit(EX_ARGSBAD);
}

/*----------------------------------------------------------------------*\
* Help()
*
* Print long help message.
\*----------------------------------------------------------------------*/

static void
Help(void)
{
    struct _options *opt;
    char **cpp;

    fprintf(stderr, "\nusage:\n        %s [-options ...] [filename]\n\n",
	    PROGRAM_NAME);
    fprintf(stderr, "where options include:\n");
    for (opt = options; opt->opt; opt++) {
	fprintf(stderr, "    %-20s %s\n", opt->opt, opt->desc);
    }

    putc('\n', stderr);
    for (cpp = notes; *cpp; cpp++) {
	fputs(*cpp, stderr);
	putc('\n', stderr);
    }
    putc('\n', stderr);
}

/*----------------------------------------------------------------------*\
* Version()
*
* Spit out version number.
\*----------------------------------------------------------------------*/

static void
Version(void)
{
    fprintf(stderr, "%s: version %s\n", PROGRAM_NAME, PROGRAM_VERSION);
}

/*----------------------------------------------------------------------*\
* GetOpts()
*
* Process options list
\*----------------------------------------------------------------------*/

static void
GetOpts(int argc, char **argv)
{
    /* Parse the command line */

    for (argc--, argv++; argc > 0; argc--, argv++) {

	if (argv[0][0] == '-') {
	    ParseOpts(&argc, &argv);
	}
	else
	    FileName = argv[0];
    }
}

/*----------------------------------------------------------------------*\
* ParseOpts()
*
* Parses & determines appropriate action for input flag.
\*----------------------------------------------------------------------*/

static void
ParseOpts(int *pargc, char ***pargv)
{
    char *opt, *optarg, *field;

    opt = (*pargv)[0] + 1;

/* Help */

    if (!strcmp(opt, "h")) {
	Help();
	exit(0);
    }

/* Version */

    if (!strcmp(opt, "v")) {
	Version();
	exit(0);
    }

/* Output device */

    if (!strcmp(opt, "dev")) {
	GetOption(&optarg, pargc, pargv);
	devname = optarg;
	return;
    }

/* Input file */

    if (!strcmp(opt, "i")) {
	GetOption(&optarg, pargc, pargv);
	FileName = optarg;
	return;
    }

/* Output file */

    if (!strcmp(opt, "o")) {
	GetOption(&optarg, pargc, pargv);
	FileNameOut = optarg;
	return;
    }

/* Use as a filter */

    if (!strcmp(opt, "f")) {
	is_filter++;
	return;
    }

/* Beginning page */

    if (!strcmp(opt, "b")) {
	GetOption(&optarg, pargc, pargv);
	page_begin = atoi(optarg);
	return;
    }

/* End page */

    if (!strcmp(opt, "e")) {
	GetOption(&optarg, pargc, pargv);
	page_end = atoi(optarg);
	return;
    }

/* Given page only */

    if (!strcmp(opt, "p")) {
	GetOption(&optarg, pargc, pargv);
	page_begin = atoi(optarg);
	page_end = page_begin;
	return;
    }

/* Override aspect ratio */

    if (!strcmp(opt, "a")) {
	GetOption(&optarg, pargc, pargv);
	set_aspect = atof(optarg);
	set_aspectset = 1;
	return;
    }

/* Set margin factor -- total fraction of page to reserve at edge (includes
   contributions at both sides). */

    if (!strcmp(opt, "mar")) {
	GetOption(&optarg, pargc, pargv);
	mar = atof(optarg);
	return;
    }

/* Set justification in x (0.0 < jx < 1.0). jx = 0.5 (centered) is default */

    if (!strcmp(opt, "jx")) {
	GetOption(&optarg, pargc, pargv);
	jx = atof(optarg);
	return;
    }

/* Set justification in y (0.0 < jy < 1.0). jy = 0.5 (centered) is default */

    if (!strcmp(opt, "jy")) {
	GetOption(&optarg, pargc, pargv);
	jy = atof(optarg);
	return;
    }

/* Orientation */

    if (!strcmp(opt, "ori")) {
	GetOption(&optarg, pargc, pargv);
	set_orient = atoi(optarg);
	set_orientset = 1;
	return;
    }

/* Width */

    if (!strcmp(opt, "width")) {
	GetOption(&optarg, pargc, pargv);
	set_width = atoi(optarg);
	set_widthset = 1;
	return;
    }

/* Background */

    if (!strcmp(opt, "bg")) {
	GetOption(&optarg, pargc, pargv);
	bgcolor = atoi(optarg);
	bgcolorset = 1;
	return;
    }

/* Color */

    if (!strcmp(opt, "color")) {
	color = 1;
	return;
    }

/* Pack in x */

    if (!strcmp(opt, "px")) {
	GetOption(&optarg, pargc, pargv);
	packx = atoi(optarg);
	return;
    }

/* Pack in y */

    if (!strcmp(opt, "py")) {
	GetOption(&optarg, pargc, pargv);
	packy = atoi(optarg);
	return;
    }

/* Family output files */

    if (!strcmp(opt, "fam")) {
	familyout++;
	return;
    }

/* Size of a member file (may be larger since eof must occur at page break) */

    if (!strcmp(opt, "fsiz")) {
	GetOption(&optarg, pargc, pargv);
	bytemax = 1.0e6 * atof(optarg);
	return;
    }

/* Starting member number when reading family files */

    if (!strcmp(opt, "fmem")) {
	GetOption(&optarg, pargc, pargv);
	numberin = atoi(optarg);
	return;
    }

/* No pause between pages */

    if (!strcmp(opt, "np")) {
	nopause++;
	return;
    }

/* Geometry for output window (e.g. 400x400+100+0), note offsets don't work
   correctly at present. */

    if (!strcmp(opt, "geo")) {
	GetOption(&optarg, pargc, pargv);

	field = strtok(optarg, "x");
	if (field == NULL)
	    return;
	xwid = atoi(field);

	field = strtok(NULL, "+");
	if (field == NULL)
	    return;
	ywid = atoi(field);

	field = strtok(NULL, "+");
	if (field == NULL)
	    return;
	xoff = atoi(field);

	field = strtok(NULL, "+");
	if (field == NULL)
	    return;
	yoff = atoi(field);

	return;
    }
    Syntax(**pargv);
}

/*----------------------------------------------------------------------*\
* GetOption()
*
* Retrieves an option argument.
\*----------------------------------------------------------------------*/

static void
GetOption(char **poptarg, int *pargc, char ***pargv)
{
    if (*pargc > 0) {
	(*pargc)--;
	(*pargv)++;
	*poptarg = (*pargv)[0];
    }
    else
	Syntax("");
}
