/* $Id$
   $Log$
   Revision 1.4  1993/04/26 19:57:56  mjl
   Fixes to allow (once again) output to stdout and plrender to function as
   a filter.  A type flag was added to handle file vs stream differences.

 * Revision 1.3  1993/03/17  17:01:37  mjl
 * Eliminated some dead assignments that turned up when running with SAS/C's
 * global optimizer enabled on the Amiga.
 *
 * Revision 1.2  1993/03/03  17:03:20  mjl
 * Changed the -bg flag to accept a full-color argument in the form
 * -bg rrggbb, with rr, gg, bb corresponding to the background RGB
 * values in hex.  Example: -bg FFFF00 to get a yellow background.
 *
 * Revision 1.1  1993/02/23  05:28:26  mjl
 * Added code to parse command line arguments.  Accepts a data structure with
 * argument specification, syntax, description, mode flag, and function handler
 * (called if option is found).  Usable both to parse plplot command flags and
 * user command flags.  The latter is facilitated by input of user routines to
 * handle usage and help messages.  The plplot command line parser removes all
 * arguments it recognizes, allowing the user to detect invalid input.  See
 * plrender.c for a working example of usage of the case of user
 * command flags; the simpler case with no user command flags is illustrated
 * by any of the (C) example programs.
 *
*/

/*
    plargs.c

    Copyright 1993
    Maurice LeBrun

    This software may be freely copied, modified and redistributed without
    fee provided that this copyright notice is preserved intact on all
    copies and modified copies.

    There is no warranty or other guarantee of fitness of this software.
    It is provided solely "as is". The author(s) disclaim(s) all
    responsibility and liability with respect to this software's usage or
    its effect upon hardware or computer systems.

* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

    This file contains routines to extract & process command flags.
    The command flags recognized by PLPLOT are stored in the struct
    "option_table", along with strings giving the syntax, long help 
    message, and handler.  The command line parser removes all
    recognized flags (decreasing argc accordingly), so that invalid
    input may be readily detected.  These routines can also be used
    to process user command line flags.

    The command line parsers (one for internal plplot flags, one for
    user-specified flags) accept a variable "mode" which can take
    on the values:

    PL_PARSE_FULL -- Full parsing of command line and all error
    messages enabled.  Should not be used by programs that have
    other command flags (the caller should then issue help and
    syntax messages).

    PL_PARSE_PARTIAL -- Parses command line but takes no action for
    unrecognized flags (since these may be valid user command
    flags).  Still issues syntax and help messages when appropriate.
    It is up to the user to exit if an error occurs.

    PL_PARSE_QUIET -- Turns off all output except in the case of
    errors.

    The parser for user-defined flags also accepts a pointer to
    a function to be called when an error is detected, to allow
    an appropriate usage message to be issued.

    See plrender.c for examples of actual usage.
*/

#include "plplot.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "plplotio.h"
#include "plstream.h"

/* Support functions */
/* INDENT OFF */

static void Usage	(char *);
static int  ParseOpt	(int *, char ***, int *, char ***, PLOptionTable *);
static int  GetOption	(char **, int *, char ***, int *, PLOptionTable *);

static void (*UsageH) (char *) = Usage;

/* Option handlers */

static int HandleOption_h	(char *, char *);
static int HandleOption_v	(char *, char *);
static int HandleOption_dev	(char *, char *);
static int HandleOption_o	(char *, char *);
static int HandleOption_geo	(char *, char *);
static int HandleOption_a	(char *, char *);
static int HandleOption_ori	(char *, char *);
static int HandleOption_width	(char *, char *);
static int HandleOption_color	(char *, char *);
static int HandleOption_bg	(char *, char *);
static int HandleOption_fam	(char *, char *);
static int HandleOption_fsiz	(char *, char *);
static int HandleOption_np	(char *, char *);
static int HandleOption_px	(char *, char *);
static int HandleOption_py	(char *, char *);
static int HandleOption_geo	(char *, char *);

/* Global variables */

static char	*program_name;
static PLINT	parse_mode;

/*----------------------------------------------------------------------*\
* PLPLOT options data structure definition.
\*----------------------------------------------------------------------*/

static PLOptionTable option_table[] = {
{
    "h",			/* Help */
    HandleOption_h,
    0,
    "-h",
    "Print out this message" },
{
    "v",			/* Version */
    HandleOption_v,
    0,
    "-v",
    "Print out the plplot library version number" },
{
    "dev",			/* Output device */
    HandleOption_dev,
    PL_PARSE_ARG,
    "-dev name",
    "Output device name" },
{
    "o",			/* Output filename or X server */
    HandleOption_o,
    PL_PARSE_ARG,
    "-o name",
    "Output filename, or X server to contact" },
{
    "px",			/* Plots per page in x */
    HandleOption_px,
    PL_PARSE_ARG,
    "-px number",
    "Plots per page in x" },
{
    "py",			/* Plots per page in y */
    HandleOption_py,
    PL_PARSE_ARG,
    "-py number",
    "Plots per page in y" },
{
    "geo",			/* Geometry */
    HandleOption_geo,
    PL_PARSE_ARG,
    "-geo geom",
    "Window size, in pixels (e.g. -geo 400x400)" },
{
    "a",			/* Aspect ratio */
    HandleOption_a,
    PL_PARSE_ARG,
    "-a aspect",
    "Plot aspect ratio" },
{
    "ori",			/* Orientation */
    HandleOption_ori,
    PL_PARSE_ARG,
    "-ori orient",
    "Plot orientation (0,2=landscape, 1,3=portrait)" },
{
    "width",			/* Pen width */
    HandleOption_width,
    PL_PARSE_ARG,
    "-width width",
    "Pen width (1 <= width <= 10)" },
{
    "color",			/* Color on switch */
    HandleOption_color,
    0,
    "-color",
    "Forces color output (some drivers default to mono)" },
{
    "bg",			/* Background color */
    HandleOption_bg,
    PL_PARSE_ARG,
    "-bg color",
    "Background color (0=black, 255=white [def])" },
{
    "fam",			/* Familying on switch */
    HandleOption_fam,
    0,
    "-fam",
    "Create a family of output files" },
{
    "fsiz",			/* Family file size */
    HandleOption_fsiz,
    PL_PARSE_ARG,
    "-fsiz size",
    "Output family file size in MB (e.g. -fsiz 1.0)" },
{
    "np",			/* Page pause off switch */
    HandleOption_np,
    0,
    "-np",
    "No pause between pages" },
{
    NULL,
    NULL,
    0,
    NULL,
    NULL }
};

static char *notes[] = {
"All parameters must be white-space delimited.  Not all options valid with",
"all drivers.  Please see the plplot reference document for more detail.",
NULL};

/* INDENT ON */
/*----------------------------------------------------------------------*\
* The following routines contain some code derived from "xterm.c" and
* "ParseCmd.c" of the X-windows Version 11 distribution.  The copyright
* notice is reproduced here:

Copyright 1987, 1988 by Digital Equipment Corporation, Maynard, Massachusetts,
and the Massachusetts Institute of Technology, Cambridge, Massachusetts.

                        All Rights Reserved

* The full permission notice is given in the plplot documentation.
\*----------------------------------------------------------------------*/

/*----------------------------------------------------------------------*\
* plSyntax()
*
* Print syntax message appropriate for plplot.
\*----------------------------------------------------------------------*/

void
plSyntax()
{
    PLOptionTable *opt;
    int col, len;

    fprintf(stderr, "\nplplot options:");

    col = 80;
    for (opt = option_table; opt->syntax; opt++) {
	len = 3 + strlen(opt->syntax);		/* space [ string ] */
	if (col + len > 79) {
	    fprintf(stderr, "\r\n   ");		/* 3 spaces */
	    col = 3;
	}
	fprintf(stderr, " [%s]", opt->syntax);
	col += len;
    }
    fprintf(stderr, "\r\n");
}

/*----------------------------------------------------------------------*\
* plHelp()
*
* Print long help message.
\*----------------------------------------------------------------------*/

void
plHelp(void)
{
    PLOptionTable *opt;

    fprintf(stderr, "\nplplot options:\n");
    for (opt = option_table; opt->syntax; opt++) {
	fprintf(stderr, "    %-20s %s\n", opt->syntax, opt->desc);
    }
}

/*----------------------------------------------------------------------*\
* plNotes()
*
* Print usage notes.
\*----------------------------------------------------------------------*/

void
plNotes(void)
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
* plParseInternalOpts()
*
* Process plplot internal options list
* If mode is PL_PARSE_FULL, exit on an error.
\*----------------------------------------------------------------------*/

int
plParseInternalOpts(int *p_argc, char **argv, PLINT mode)
{
    return(plParseOpts(p_argc, argv, mode, option_table, NULL));
}

/*----------------------------------------------------------------------*\
* plParseOpts()
*
* Process options list
* An error in parsing the argument list causes a program exit if 
* parse_mode == PL_PARSE_FULL, otherwise the function returns 
* with an error.
\*----------------------------------------------------------------------*/

int
plParseOpts(int *p_argc, char **argv, PLINT mode, PLOptionTable *option_table,
	    void (*usage_handler) (char *))
{
    PLINT status=0;
    char **argsave, **argend;
    int	myargc;

/* Save the program name and advance to the first argument. */

    parse_mode = mode;
    myargc = (*p_argc); 
    argend = argv + myargc;
    if (usage_handler != NULL)
	UsageH = usage_handler;

    program_name = argv[0];
    --myargc; ++argv;
    argsave = argv;

/* No arguments */

    if (myargc == 0)
	return(0);

/* Process the command line */

    for (; myargc > 0; --myargc, ++argv) {
	status = ParseOpt(&myargc, &argv, p_argc, &argsave, option_table);
	if (status) {
	    if (parse_mode == PL_PARSE_FULL)
		exit(1);
	    else
		break;
	}
    }
    if (argsave < argend)
	(*argsave)=NULL; /* put NULL terminator on compressed argv */

    return(status);
}

/*----------------------------------------------------------------------*\
* ParseOpt()
*
* Parses & determines appropriate action for input flag.
\*----------------------------------------------------------------------*/

static int
ParseOpt(int *p_myargc, char ***p_argv, int *p_argc, char ***p_argsave,
	 PLOptionTable *option_table)
{
    int i, status;
    char *opt, *optarg;

/* Only handle actual flags and their arguments */

    if ((*p_argv)[0][0] == '-') {

	opt = (*p_argv)[0] + 1;

	for (i = 0; ; i++) {

	    if (option_table[i].opt == NULL)
		break;

	    if (!strcmp(opt, option_table[i].opt)) {
		if (option_table[i].flags & PL_PARSE_NODELETE) 
		    (*(*p_argsave)++) = (**p_argv);
		else
		    --(*p_argc);

		if (option_table[i].flags & PL_PARSE_ARG) {
		    status = GetOption(&optarg, p_myargc, p_argv, p_argc,
				       option_table);
		    if (!status) 
			status = (*option_table[i].handler) (opt, optarg);
		}
		else
		    status = (*option_table[i].handler) (opt, NULL);

		return(status);
	    }
	}
    }

/* If control reaches here the argument is unrecognized */

    (*(*p_argsave)++) = (**p_argv);  /*compress arglist*/ 

    if (parse_mode == PL_PARSE_FULL) {
	(*UsageH) (**p_argv);
	return(1);
    }
    else
	return(0);
}

/*----------------------------------------------------------------------*\
* GetOption()
*
* Retrieves an option argument.
* If an error occurs here it is a true syntax error.
\*----------------------------------------------------------------------*/

static int
GetOption(char **poptarg, int *p_myargc, char ***p_argv, int *p_argc, 
	  PLOptionTable *option_table)
{
    int error = 0;

    --(*p_myargc);

    if ((*p_myargc) <= 0)		/* oops, no more arguments */
	error = 1;

    if (!error) {
	(*p_argv)++;
	if ((*p_argv)[0][0] == '-' && isalpha((*p_argv)[0][1])) {

	    (*p_argv)--;		/* oops, next arg is a flag */
	    error = 1;
	}
    }

    if (!error) {			/* yeah, the user got it right */
	(*p_argc)--;
	*poptarg = (*p_argv)[0];
	return(0);
    }
    else {
	if (parse_mode != PL_PARSE_QUIET) {
	    fprintf(stderr, "Argument missing for %s option.\n",
		    (*p_argv)[0]); 
	    (*UsageH) ("");
	    return(1);
	}
    }
}

/*----------------------------------------------------------------------*\
* Usage()
*
* Print usage & syntax message.
\*----------------------------------------------------------------------*/

static void
Usage(char *badOption)
{
    if (*badOption != '\0')
	fprintf(stderr, "\n%s:  bad command line option \"%s\"\r\n",
		program_name, badOption);

    fprintf(stderr, "\nUsage:\n        %s [plplot options]\n",
	    program_name);

    plSyntax();

    fprintf(stderr, "\r\n\nType %s -h for a full description.\r\n\n",
	    program_name);
}

/*----------------------------------------------------------------------*\
* Option handlers
\*----------------------------------------------------------------------*/

/*----------------------------------------------------------------------*\
* HandleOption_h()
*
* Performs appropriate action for option "h".
\*----------------------------------------------------------------------*/

static int
HandleOption_h(char *opt, char *optarg)
{

/* Help */

    if (parse_mode != PL_PARSE_QUIET) {
	fprintf(stderr, "\nUsage:\n        %s [plplot options]\n",
		program_name);

	plHelp();
	plNotes();
    }
    return(1);
}

/*----------------------------------------------------------------------*\
* HandleOption_v()
*
* Performs appropriate action for option "v".
\*----------------------------------------------------------------------*/

static int
HandleOption_v(char *opt, char *optarg)
{

/* Version */

    if (parse_mode != PL_PARSE_QUIET) {
	fprintf(stderr, "\nplplot library version: %s\n", PLPLOT_VERSION);
    }
    return(1);
}

/*----------------------------------------------------------------------*\
* HandleOption_dev()
*
* Performs appropriate action for option "dev".
\*----------------------------------------------------------------------*/

static int
HandleOption_dev(char *opt, char *optarg)
{
/* Output device */

    plsdev(optarg);

    return(0);
}

/*----------------------------------------------------------------------*\
* HandleOption_o()
*
* Performs appropriate action for option "o".
\*----------------------------------------------------------------------*/

static int
HandleOption_o(char *opt, char *optarg)
{
/* Output file */

    plsfnam(optarg);

    return(0);
}

/*----------------------------------------------------------------------*\
* HandleOption_a()
*
* Performs appropriate action for option "a".
\*----------------------------------------------------------------------*/

static int
HandleOption_a(char *opt, char *optarg)
{
    PLFLT aspect;

/* Override aspect ratio */

    aspect = atof(optarg);
    plsasp(aspect);

    return(0);
}

/*----------------------------------------------------------------------*\
* HandleOption_ori()
*
* Performs appropriate action for option "ori".
\*----------------------------------------------------------------------*/

static int
HandleOption_ori(char *opt, char *optarg)
{
    int orient;

/* Orientation */

    orient = atoi(optarg);
    plsori(orient);

    return(0);
}

/*----------------------------------------------------------------------*\
* HandleOption_width()
*
* Performs appropriate action for option "width".
\*----------------------------------------------------------------------*/

static int
HandleOption_width(char *opt, char *optarg)
{
    int width;

/* Width */

    width = atoi(optarg);
    if (width == 0) {
	fprintf(stderr, "?invalid width\n");
	return(1);
    }
    else
	plwid(width);

    return(0);
}

/*----------------------------------------------------------------------*\
* HandleOption_bg()
*
* Performs appropriate action for option "bg".
\*----------------------------------------------------------------------*/

static int
HandleOption_bg(char *opt, char *optarg)
{
    long bgcolor, r, g, b;

/* Background */

    bgcolor = strtol(optarg, NULL, 16);
    r = (bgcolor & 0xFF0000) >> 16;
    g = (bgcolor & 0x00FF00) >> 8;
    b = (bgcolor & 0x0000FF);
    plscolbg(r, g, b);

    return(0);
}

/*----------------------------------------------------------------------*\
* HandleOption_color()
*
* Performs appropriate action for option "color".
\*----------------------------------------------------------------------*/

static int
HandleOption_color(char *opt, char *optarg)
{

/* Color */

    plscolor(1);

    return(0);
}

/*----------------------------------------------------------------------*\
* HandleOption_fam()
*
* Performs appropriate action for option "fam".
\*----------------------------------------------------------------------*/

static int
HandleOption_fam(char *opt, char *optarg)
{

/* Family output files */

    plsfam(1, -1, -1);

    return(0);
}

/*----------------------------------------------------------------------*\
* HandleOption_fsiz()
*
* Performs appropriate action for option "fsiz".
\*----------------------------------------------------------------------*/

static int
HandleOption_fsiz(char *opt, char *optarg)
{
    PLINT bytemax;

/* Size of a member file (may be larger since eof must occur at page break) */

    bytemax = 1.0e6 * atof(optarg);
    if (bytemax == 0) {
	fprintf(stderr, "?invalid bytemax\n");
	return(1);
    }
    plsfam(-1, -1, bytemax);

    return(0);
}

/*----------------------------------------------------------------------*\
* HandleOption_np()
*
* Performs appropriate action for option "np".
\*----------------------------------------------------------------------*/

static int
HandleOption_np(char *opt, char *optarg)
{

/* No pause between pages */

    plspause(0);

    return(0);
}

/*----------------------------------------------------------------------*\
* HandleOption_px()
*
* Performs appropriate action for option "px".
\*----------------------------------------------------------------------*/

static int
HandleOption_px(char *opt, char *optarg)
{

/* Pack in x */

    plssub(atoi(optarg), -1);

    return(0);
}

/*----------------------------------------------------------------------*\
* HandleOption_py()
*
* Performs appropriate action for option "py".
\*----------------------------------------------------------------------*/

static int
HandleOption_py(char *opt, char *optarg)
{

/* Pack in y */

    plssub(-1, atoi(optarg));

    return(0);
}

/*----------------------------------------------------------------------*\
* HandleOption_geo()
*
* Performs appropriate action for option "geo".
\*----------------------------------------------------------------------*/

static int
HandleOption_geo(char *opt, char *optarg)
{
    char *field;
    PLFLT xdpi = 0., ydpi = 0.;
    PLINT xwid, ywid, xoff = 0, yoff = 0;

/* Geometry for output window (e.g. 400x400+100+0), note offsets don't work
   correctly at present. */

    if ((field = strtok(optarg, "x")) == NULL)
	return(1);

    xwid = atoi(field);
    if (xwid == 0) {
	fprintf(stderr, "?invalid xwid\n");
	return(1);
    }

    if ((field = strtok(NULL, "+")) == NULL)
	return(1);

    ywid = atoi(field);
    if (ywid == 0) {
	fprintf(stderr, "?invalid ywid\n");
	return(1);
    }

    if ((field = strtok(NULL, "+")) != NULL) {
	xoff = atoi(field);

	if ((field = strtok(NULL, "+")) != NULL)
	    yoff = atoi(field);
    }

    plspage(xdpi, ydpi, xwid, ywid, xoff, yoff);

    return(0);
}
