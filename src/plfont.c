/* $Id$
   $Log$
   Revision 1.7  1993/07/01 22:15:32  mjl
   Changed all plplot source files to include plplotP.h (private) rather than
   plplot.h.  Improved code that locates fonts, and changed the default font
   locations on the Amiga.

 * Revision 1.6  1993/02/23  05:14:13  mjl
 * Changed reference in error message from plstar to plinit.
 *
 * Revision 1.5  1993/01/23  05:54:32  mjl
 * Added support for device-independent font files.
 *
 * Revision 1.4  1992/10/22  17:05:35  mjl
 * Fixed warnings, errors generated when compling with HP C++.
 *
 * Revision 1.3  1992/09/30  18:25:47  furnish
 * Massive cleanup to irradicate garbage code.  Almost everything is now
 * prototyped correctly.  Builds on HPUX, SUNOS (gcc), AIX, and UNICOS.
 *
 * Revision 1.2  1992/09/29  04:45:57  furnish
 * Massive clean up effort to remove support for garbage compilers (K&R).
 *
 * Revision 1.1  1992/05/20  21:34:27  furnish
 * Initial checkin of the whole PLPLOT project.
 *
*/

/*	plfont.c
*
*	Font management code.
*
* The current directory is always searched for the fonts first, followed
* by a system-dependent (and perhaps site-dependent) search path.
* Note that directory names, if unix-like, must be defined with the
* trailing slash, if non-null.  Each system has three hard-wired devices
* (may be a logical name) that are searched (default value listed below).
* The default may be overridden from the makefile.
*
* Unix:
*	current directory
*	$(HOME)/lib/
*	$(PLFONTS)
*	PLFONTDEV1	(/usr/local/lib/)
*	PLFONTDEV2	(/usr/local/lib/plplot/
*	PLFONTDEV3	(/usr/local/plplot/)
*
* VMS:
*	current directory
*	PLFONTDEV1	(lib:)
*	PLFONTDEV1	(sys$login:)
*	PLFONTDIR	(sys$sysroot:[sysfont.plplot])
*
* Amiga:
*	current directory
*	$(PLFONTS)
*	PLFONTDEV1	(s:plplot/)
*	PLFONTDEV2	(fonts:plplot/)
*	PLFONTDEV3	(plfonts:)
*/

#define PL_NEED_MALLOC
#include "plplotP.h"

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>

#ifdef MSDOS
#include <malloc.h>
#endif

#include "pdf.h"

 /* MSDOS search path */

#ifdef MSDOS
#define PLFONTENV "PLFONTS"	/* C> set PLFONTS=dir_name_for_fonts */
#endif

 /* Unix search path */

#ifdef __unix
#define HOME_LIB
#define PLFONTENV  "PLFONTS"

#ifndef PLFONTDEV1
#define PLFONTDEV1 "/usr/local/lib/"
#endif
#ifndef PLFONTDEV2
#define PLFONTDEV2 "/usr/local/lib/plplot/"
#endif
#ifndef PLFONTDEV3
#define PLFONTDEV3 "/usr/local/plplot/"
#endif
#endif

 /* VMS search path */

#ifdef vms
#ifndef PLFONTDEV1
#define PLFONTDEV1 "lib:"
#endif
#ifndef PLFONTDEV2
#define PLFONTDEV2 "sys$login:"
#endif
#ifndef PLFONTDEV3
#define PLFONTDEV3 "sys$sysroot:[sysfont.plplot]"
#endif
#endif

 /* Amiga search path */

#ifdef AMIGA
#define PLFONTENV  "PLFONTS"

#ifndef PLFONTDEV1
#define PLFONTDEV1  "fonts:plplot/"
#endif
#ifndef PLFONTDEV2
#define PLFONTDEV2  "plfonts:"
#endif
#endif

#ifdef GNU386
#include <stddef.h>
#define PLFONTDEV1 "c:/lib/"
#endif

/* A/IX system 3 doesn't like you to call getenv() from a C program
   linked with a fortran main, so we can only use hard-coded location */

#ifdef AIX_3_0
#ifdef PLFONTENV
#undef PLFONTENV
#endif
#ifdef HOME_LIB
#undef HOME_LIB
#endif
#endif

/* Font directory */
/* Total length for total path+file spec limited to 256 chars */

#define NFILEN	256
static char fontdir[NFILEN];

/* Function prototypes. */

static FILE	*plfontopen	(char *);
static void	barf		(char *);

char *getenv();

/* Declarations */

short int *fntlkup;
short int *fntindx;
SCHAR *fntbffr;
short int numberfonts, numberchars;
short int indxleng;

static short fontloaded = 0;

/*----------------------------------------------------------------------*\
* void c_plfont(ifont)
*
* Sets the global font flag to 'ifont'.
\*----------------------------------------------------------------------*/

void
c_plfont(PLINT ifont)
{
    PLINT ifnt, icol;
    PLINT level;

    plP_glev(&level);
    if (level < 1)
	plexit("plfont: Please call plinit first.");

    if (ifont < 1 || ifont > 4)
	plexit("plfont: Invalid font.");

    plP_gatt(&ifnt, &icol);
    plP_satt(ifont, icol);
}

/*----------------------------------------------------------------------*\
* void plfntld(fnt)
*
* Loads either the standard or extended font.
\*----------------------------------------------------------------------*/

void
plfntld(PLINT fnt)
{
    static PLINT charset;
    short bffrleng;
    FILE *fontfile;

    if (fontloaded) {
	if (charset == fnt)
	    return;
	free((void *) fntindx);
	free((void *) fntbffr);
	free((void *) fntlkup);
    }

    fontloaded = 1;
    charset = fnt;

    if (fnt)
	fontfile = plfontopen(PL_XFONT);
    else
	fontfile = plfontopen(PL_SFONT);

/* Read fntlkup[] */

    pdf_rd_2bytes(fontfile, (U_SHORT *) &bffrleng);
    numberfonts = bffrleng / 256;
    numberchars = bffrleng & 0xff;
    bffrleng = numberfonts * numberchars;
    fntlkup = (short int *) malloc(bffrleng * sizeof(short int));
    if (!fntlkup) {
	barf("plfntld: Out of memory while allocating font buffer.");
    }
    pdf_rd_2nbytes(fontfile, (U_SHORT *) fntlkup, bffrleng);

/* Read fntindx[] */

    pdf_rd_2bytes(fontfile, (U_SHORT *) &indxleng);
    fntindx = (short int *) malloc(indxleng * sizeof(short int));
    if (!fntindx) {
	free((void *) fntlkup);
	barf("plfntld: Out of memory while allocating font buffer.");
    }
    pdf_rd_2nbytes(fontfile, (U_SHORT *) fntindx, indxleng);

/* Read fntbffr[] */
/* Since this is an array of char, there are no endian problems */

    pdf_rd_2bytes(fontfile, (U_SHORT *) &bffrleng);
    fntbffr = (SCHAR *) malloc(2 * bffrleng * sizeof(SCHAR));
    if (!fntbffr) {
	free((void *) fntindx);
	free((void *) fntlkup);
	barf("plfntld: Out of memory while allocating font buffer.");
    }
    fread((void *) fntbffr, (size_t) sizeof(SCHAR),
	  (size_t) (2 * bffrleng), fontfile);

/* Done */

    fclose(fontfile);
}

/*----------------------------------------------------------------------*\
* FILE *plfontopen(fn)
*
* Return file pointer to font file.
* Lots of locations checked; see documentation for plfntld().
\*----------------------------------------------------------------------*/

static FILE *
plfontopen(char *fn)
{
    FILE *plfp;
    char fs[NFILEN];
    char *dn;

/****	use results of previous search	****/

    if (fontdir != NULL) {
	strcpy(fs, fontdir);
	strcat(fs, fn);
	if ((plfp = fopen(fs, BINARY_READ)) != NULL)
	    return (plfp);
    }

/****	search current directory	****/

    strcpy(fontdir, "");
    if ((plfp = fopen(fn, BINARY_READ)) != NULL)
	return (plfp);

/**** 	search $(HOME)/lib	****/

#ifdef HOME_LIB
    if ((dn = getenv("HOME")) != NULL) {
	if ((strlen(dn) + 5 + strlen(fn)) > NFILEN)
	    barf("plfontopen: Too many characters in font file name.\n");

	strcpy(fontdir, dn);
	strcat(fontdir, "/lib/");
	strcpy(fs, fontdir);
	strcat(fs, fn);

	if ((plfp = fopen(fs, BINARY_READ)) != NULL)
	    return (plfp);
    }
#endif

/****	search $(PLFONTS)	****/

#ifdef PLFONTENV
    if ((dn = getenv(PLFONTENV)) != NULL) {
	if ((strlen(dn) + strlen(fn)) > NFILEN)
	    barf("plfontopen: Too many characters in font file name.\n");

	strcpy(fontdir, dn);
	strcpy(fs, fontdir);
	strcat(fs, fn);

	if ((plfp = fopen(fs, BINARY_READ)) != NULL)
	    return (plfp);
    }
#endif

/**** 	search devices		****/

#ifdef PLFONTDEV1
    if ((strlen(PLFONTDEV1) + strlen(fn)) > NFILEN)
	barf("plfontopen: Too many characters in font file name.\n");

    strcpy(fontdir, PLFONTDEV1);
#ifdef MSDOS
    /* Strip off the trailing space.  No way to get PLFONTDEV1 to be
    the dir path without a space before the final quote.  Unbelievalbe idiots
    at Microsoft.  You can say:
	cl "-DPLFONTDEV1=\"d:\\lib\\ \""
    but you can't say:
	cl "-DPLFONTDEV1=\"d:\\lib\\\""
    or it generates an error.  Somebody fire the moron.

    And, thanks to the idiots on the ANSI committee, there is no way to use
    the stringizing operator to get a string literal in your code with the
    value specified on the command line.  With standards committees like
    these, maybe we should go back to abacci.
    */
    fontdir[strlen(fontdir) - 1] = '\0';
#endif
    strcpy(fs, fontdir);
    strcat(fs, fn);

    if ((plfp = fopen(fs, BINARY_READ)) != NULL)
	return (plfp);
#endif

#ifdef PLFONTDEV2
    if ((strlen(PLFONTDEV2) + strlen(fn)) > NFILEN)
	barf("plfontopen: Too many characters in font file name.\n");

    strcpy(fontdir, PLFONTDEV2);
#ifdef MSDOS
    fontdir[strlen(fontdir) - 1] = '\0';
#endif
    strcpy(fs, fontdir);
    strcat(fs, fn);

    if ((plfp = fopen(fs, BINARY_READ)) != NULL)
	return (plfp);
#endif

#ifdef PLFONTDEV3
    if ((strlen(PLFONTDEV3) + strlen(fn)) > NFILEN)
	barf("plfontopen: Too many characters in font file name.\n");

    strcpy(fontdir, PLFONTDEV3);
#ifdef MSDOS
    fontdir[strlen(fontdir) - 1] = '\0';
#endif
    strcpy(fs, fontdir);
    strcat(fs, fn);

    if ((plfp = fopen(fs, BINARY_READ)) != NULL)
	return (plfp);
#endif

/**** 	not found, give up 	****/

    fprintf(stderr, "\nUnable to open font file: %s.\n", fn);
    barf("");

    return (NULL);		/* don't ask */
}

/*----------------------------------------------------------------------*\
* barf
*
* Terminates program --
* don't use plexit since we don't want plfontrel to be called.
\*----------------------------------------------------------------------*/

static void
barf(char *msg)
{
    if (*msg != '\0')
	fprintf(stderr, "%s\n", msg);

    fprintf(stderr, "Plplot aborted.\n");
    pl_exit();
    exit(1);
}

/*----------------------------------------------------------------------*\
* void plfontrel()
*
* Release memory for fonts.
\*----------------------------------------------------------------------*/

void
plfontrel(void)
{
    if (fontloaded) {
	free((void *) fntindx);
	free((void *) fntbffr);
	free((void *) fntlkup);
	fontloaded = 0;
    }
}
