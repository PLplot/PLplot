/* $Id$
   $Log$
   Revision 1.4  1992/10/29 15:56:16  mjl
   Gave plrender an ID tag.

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

char ident[]="@(#) $Id$";

#include "plplot.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "metadefs.h"
#include "pdf.h"

#define PROGRAM_NAME 	"plrender"
#define PROGRAM_VERSION PLPLOT_VERSION
#define DO_ASP_SCALING

/* prototypes for functions in this file. */

static void	GetOpts		( int , char ** );
static U_CHAR	getcommand	( void );
static void	process_next	( U_CHAR );
static void 	plr_init	( void );
static void	plresc_rgb	( void );
static void	plresc_ancol	( void );
static void	NextFamilyFile	( U_CHAR * );
static int	ReadHeader	( void );
static void	Syntax		( char * );
static void	Help		( void );
static void	Version		( void );
static void	ParseOpts	( int *, char *** );
static void	GetOption	( char **, int *, char *** );
static void	check_alignment ( FILE * );

/* top level declarations */

static PLINT	xold, yold;	/* Endpoint of prev line plotted */
static PLINT	page_begin=1;	/* Where to start plotting */
static PLINT	page_end;	/* Where to stop (0 to disable) */
static PLINT	page;		/* Current page number */
static PLINT	searching;	/* Set if we are searching for a page */
static int	no_page;	/* Flag to skip next new page */
static FILE	*MetaFile;
static char	*FileName, *FileNameOut;
static char	FamilyNameIn[80] = "", FileNameIn[90] = "";
static PLINT	familyout, numberout=1, bytemax=PL_FILESIZE_KB*1000;
static PLINT	familyin, numberin=1;
static PLFLT	xdpi, ydpi;
static PLINT	xwid, ywid, xoff, yoff;
static PLINT	nopause;
static char	*devname;
static char	mf_magic[40], mf_version[40];
static U_CHAR	dum_uchar;
static U_SHORT	dum_ushort;
static char	dum_char80[80];
static float	dum_float;
static U_CHAR	orient, plr_orient;
static float	aspect = 0.0, plr_aspect = 0.0;
static int	orientset, aspectset, plr_orientset, plr_aspectset, is_filter;
static PLINT	packx=1, packy=1;
static U_SHORT	lpbpxmi, lpbpxma, lpbpymi, lpbpyma;

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

/*
* Exit codes 
*/

#define	EX_SUCCESS	0		/* success! */
#define	EX_ARGSBAD	1		/* invalid args */
#define	EX_BADFILE	2		/* invalid filename */

/*----------------------------------------------------------------------*\
*  main()
*
*  plrender -- render a PLPLOT metafile.
\*----------------------------------------------------------------------*/

main(int argc, char *argv[])
{
    U_CHAR c;

    GetOpts(argc, argv);
    if (page_begin > 1)
	searching++;

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

/* Read & process metafile commands.
*  If familying is turned on, the end of one member file is just treated as
*  a page break assuming the next member file exists.
*/
    for (;;) {
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
*  NextFamilyFile()
*
*  Start the next family if it exists.
\*----------------------------------------------------------------------*/

static void 
NextFamilyFile (U_CHAR *c)
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

/* If the family file was created correctly, the first instruction in the
*  file MUST be an INITIALIZE.  We throw this away and put a page advance in
*  its place.
*/
    *c = getcommand();
    if (*c != INITIALIZE)
	fprintf(stderr, "Warning, first instruction in member file not an INIT\n");
    else
	*c = ADVANCE;
}

/*----------------------------------------------------------------------*\
*  ReadHeader()
*
*  Checks file header.  Returns 1 if an error occured.
\*----------------------------------------------------------------------*/

static int 
ReadHeader (void)
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
	fprintf(stderr, "Error: incapable of reading metafile version %s.\n", mf_version);
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
	if (*tag == '\0') break;

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

	if (!strcmp(tag, "orient")) {
	    plm_rd(read_1byte(MetaFile, &orient));
	    continue;
	}

	fprintf(stderr, "Unrecognized PLPLOT metafile header tag.\n");
	exit (1);
    }

    return (0);
}

/*----------------------------------------------------------------------*\
*  process_next()
*
*  Process a command
\*----------------------------------------------------------------------*/

static void 
process_next(U_CHAR c)
{
    U_CHAR op;
    U_SHORT p1, x1, y1, x2, y2;
    PLFLT x1f, y1f, x2f, y2f;

    switch ((int) c) {

    case INITIALIZE:
	plr_init();
	break;

    case SWITCH_TO_TEXT:
	if (searching)
	    break;

	pltext();
	break;

    case SWITCH_TO_GRAPH:
	if (searching)
	    break;

	plgra();
	break;

    case CLEAR:
	if (searching)
	    break;

	plclr();
	break;

    case PAGE:
	page++;
	if (strcmp(mf_version, "1992a") >= 0) {
	    plm_rd(read_2bytes(MetaFile, &dum_ushort));
	    plm_rd(read_2bytes(MetaFile, &dum_ushort));
	}
	searching = (page < page_begin);
	if (searching)
	    break;
	if (no_page) {
	    no_page = 0;
	    break;
	}
	if ((packx > 1 || packy > 1) && page == 1)
	    pladv(0);
	else
	    plpage();

	plvpor(vpxmin, vpxmax, vpymin, vpymax);
	plwind((PLFLT) xmin, (PLFLT) xmax, (PLFLT) ymin, (PLFLT) ymax);
	break;

    case ADVANCE:
	page++;
	plm_rd(read_2bytes(MetaFile, &dum_ushort));
	plm_rd(read_2bytes(MetaFile, &dum_ushort));
	searching = (page < page_begin);
	if (searching)
	    break;
	if (no_page) {
	    no_page = 0;
	    break;
	}
	pladv(0);
	plvpor(vpxmin, vpxmax, vpymin, vpymax);
	plwind((PLFLT) xmin, (PLFLT) xmax, (PLFLT) ymin, (PLFLT) ymax);
	break;

    case NEW_COLOR:
	plm_rd(read_2bytes(MetaFile, &p1));
	if (searching)
	    break;

	plcol(p1);
	break;

    case NEW_WIDTH:
	plm_rd(read_2bytes(MetaFile, &p1));
	if (searching)
	    break;

	plwid(p1);
	break;

    case LINE:
	plm_rd(read_2bytes(MetaFile, &x1));
	plm_rd(read_2bytes(MetaFile, &y1));
	plm_rd(read_2bytes(MetaFile, &x2));
	plm_rd(read_2bytes(MetaFile, &y2));
	if (searching)
	    break;

	if (!orient) {
	    x1f = x1;
	    y1f = y1;
	    x2f = x2;
	    y2f = y2;
	    pljoin(x1f, y1f, x2f, y2f);
	}
	else {
	    x1f = xmin + (ymax - y1) * xlen / ylen;
	    y1f = ymin + (x1 - xmin) * ylen / xlen;
	    x2f = xmin + (ymax - y2) * xlen / ylen;
	    y2f = ymin + (x2 - xmin) * ylen / xlen;
	    pljoin(x1f, y1f, x2f, y2f);
	}
	xold = x2;
	yold = y2;
	break;

    case LINETO:
	plm_rd(read_2bytes(MetaFile, &x2));
	plm_rd(read_2bytes(MetaFile, &y2));
	if (searching)
	    break;

	if (!orient) {
	    x1f = xold;
	    y1f = yold;
	    x2f = x2;
	    y2f = y2;
	    pljoin(x1f, y1f, x2f, y2f);
	}
	else {
	    x1f = xmin + (ymax - yold) * xlen / ylen;
	    y1f = ymin + (xold - xmin) * ylen / xlen;
	    x2f = xmin + (ymax - y2) * xlen / ylen;
	    y2f = ymin + (x2 - xmin) * ylen / xlen;
	    pljoin(x1f, y1f, x2f, y2f);
	}
	xold = x2;
	yold = y2;
	break;

    case ESCAPE:
	plm_rd(read_1byte(MetaFile, &op));
	switch (op) {

	case PL_SET_RGB:
	    plresc_rgb();
	    break;

	case PL_ALLOC_NCOL:
	    plresc_ancol();
	    break;

	case PL_SET_LPB:
	    plm_rd(read_2bytes(MetaFile, &lpbpxmi));
	    plm_rd(read_2bytes(MetaFile, &lpbpxma));
	    plm_rd(read_2bytes(MetaFile, &lpbpymi));
	    plm_rd(read_2bytes(MetaFile, &lpbpyma));
	    break;
	}
    }
}

/*----------------------------------------------------------------------*\
*  void plr_init()
*
*  Handle initialization.
\*----------------------------------------------------------------------*/

static void 
plr_init (void)
{
    float dev_aspect, ratio;

/* Set up misc. parameters */

    plspage(xdpi, ydpi, xwid, ywid, xoff, yoff);
    plspause(!nopause);

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

/* The orientation set from the command line overrides any the user may have
   set before initializing plplot. */

    if (plr_orientset) 
	orient = plr_orient;

/* Aspect ratio scaling */

/* If the user has not set the aspect ratio in the code via plsasp() it
   will be zero, and is set to the natural ratio of the metafile coordinate
   system.  The aspect ratio set from the command line overrides this. */

    if (aspect == 0.0) 
	aspect = ((ymax - ymin)/pxly) / ((xmax - xmin)/pxlx);

    if (plr_aspectset)
	aspect = plr_aspect;

    if (aspect <= 0.)
	fprintf(stderr, "Error in aspect ratio setting, aspect = %f\n", aspect);

    if (orient)
	aspect = 1.0/aspect;

/* Aspect ratio of output device */

    gphy(&dev_xmin, &dev_xmax, &dev_ymin, &dev_ymax);
    gpixmm(&dev_xpmm, &dev_ypmm);

    dev_aspect = ((dev_ymax - dev_ymin)/dev_ypmm) /
		 ((dev_xmax - dev_xmin)/dev_xpmm);

    if (dev_aspect <= 0.)
	fprintf(stderr, "Error in aspect ratio setting, dev_aspect = %f\n", dev_aspect);

    ratio = aspect / dev_aspect;

/* This is the default case; come back to here if things mess up */

    vpxlen = 1.0;
    vpylen = 1.0;
    vpxmin = 0.5 - vpxlen/2.;
    vpymin = 0.5 - vpylen/2.;
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
	vpxlen = 1./ratio;

    if (mar > 0.0 && mar < 1.0) {
	vpxlen *= (1.0 - mar);
	vpylen *= (1.0 - mar);
    }

    vpxmin = MAX(0., jx - vpxlen/2.0);
    vpxmax = MIN(1., vpxmin + vpxlen);
    vpxmin = vpxmax - vpxlen;

    vpymin = MAX(0., jy - vpylen/2.0);
    vpymax = MIN(1., vpymin + vpylen);
    vpymin = vpymax - vpylen;
#endif

    plvpor(vpxmin, vpxmax, vpymin, vpymax);
    plwind((PLFLT) xmin, (PLFLT) xmax, (PLFLT) ymin, (PLFLT) ymax);

    no_page++;		/* a kludge */
}

/*----------------------------------------------------------------------*\
*  getcommand()
*
*  Read & return the next command
\*----------------------------------------------------------------------*/

static U_CHAR 
getcommand(void)
{
    U_CHAR c;

    plm_rd(read_1byte(MetaFile, &c));
    return c;
}

/*----------------------------------------------------------------------*\
*  plresc_rgb()
*
*  Process escape function for RGB color selection.
\*----------------------------------------------------------------------*/

static void 
plresc_rgb (void)
{
    float red, green, blue;
    U_SHORT ired, igreen, iblue;

    plm_rd(read_2bytes(MetaFile, &ired));
    plm_rd(read_2bytes(MetaFile, &igreen));
    plm_rd(read_2bytes(MetaFile, &iblue));
    if (searching)
	return;

    red   = (double) ired   / 65535.;
    green = (double) igreen / 65535.;
    blue  = (double) iblue  / 65535.;

    plrgb((PLFLT) red, (PLFLT) green, (PLFLT) blue);
}

/*----------------------------------------------------------------------*\
*  plresc_ancol()
*
*  Process escape function for named color table allocation.
\*----------------------------------------------------------------------*/

static void 
plresc_ancol (void)
{
    U_CHAR icolor;
    char name[80];

    plm_rd(read_1byte(MetaFile, &icolor));
    plm_rd(read_header(MetaFile, name));

    plancol(icolor, name);
}

/*----------------------------------------------------------------------*\
*  check_alignment()
*
*  Reads the next byte and aborts if it is not an END_OF_HEADER.
\*----------------------------------------------------------------------*/

static void 
check_alignment( FILE *file )
{
    U_CHAR c;

    plm_rd(read_1byte(file, &c));
    if (c != END_OF_FIELD)
	plexit("Metafile alignment problem");
}

/*----------------------------------------------------------------------*\
*  Startup code.  The following routines are modelled after the startup
*  code for 'xterm.c', part of the X-windows Version 11 distribution.
*  The copyright notice for 'xterm.c' is as follows:

Copyright 1987, 1988 by Digital Equipment Corporation, Maynard, Massachusetts,
and the Massachusetts Institute of Technology, Cambridge, Massachusetts.

                        All Rights Reserved

*  The full permission notice is given in the plplot documentation.
\*----------------------------------------------------------------------*/

/*----------------------------------------------------------------------*\
*  Options data structure definition.
\*----------------------------------------------------------------------*/

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
{ "-ori orient",	"Plot orientation (0=landscape, 1=portrait)" },
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

/*----------------------------------------------------------------------*\
*  Syntax()
*
*  Print short help message.
\*----------------------------------------------------------------------*/

static void 
Syntax (char *badOption)
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
*  Help()
*
*  Print long help message.
\*----------------------------------------------------------------------*/

static void 
Help (void)
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
*  Version()
*
*  Spit out version number.
\*----------------------------------------------------------------------*/

static void 
Version (void)
{
    fprintf(stderr, "%s: version %s\n", PROGRAM_NAME, PROGRAM_VERSION);
}

/*----------------------------------------------------------------------*\
*  GetOpts()
*
*  Process options list
\*----------------------------------------------------------------------*/

static void 
GetOpts (int argc, char **argv)
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
*  ParseOpts()
*
*  Parses & determines appropriate action for input flag.
\*----------------------------------------------------------------------*/

static void 
ParseOpts (int *pargc, char ***pargv)
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
	plr_aspect = atof(optarg);
	plr_aspectset = 1;
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

/* Override orientation */

    if (!strcmp(opt, "ori")) {
	GetOption(&optarg, pargc, pargv);
	plr_orient = atoi(optarg);
	plr_orientset = 1;
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
*  GetOption()
*
*  Retrieves an option argument.
\*----------------------------------------------------------------------*/

static void 
GetOption (char **poptarg, int *pargc, char ***pargv)
{
    if (*pargc > 0) {
	(*pargc)--;
	(*pargv)++;
	*poptarg = (*pargv)[0];
    }
    else
	Syntax("");
}
