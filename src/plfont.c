/* $Id$
   $Log$
   Revision 1.6  1993/02/23 05:14:13  mjl
   Changed reference in error message from plstar to plinit.

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
*	PLFONTDEV1	(fonts:plplot/)
*	PLFONTDEV2	(plfonts:)
*	PLFONTDEV3	not specified
*/

#define PL_NEED_MALLOC
#include "plplot.h"

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

#ifdef unix
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

#define NFILEN	256		/* Max length for font path+file specification */

/* Function prototypes. */

static FILE *plfontopen(char *);
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

    glev(&level);
    if (level < 1)
	plexit("plfont: Please call plinit first.");

    if (ifont < 1 || ifont > 4)
	plexit("plfont: Invalid font.");

    gatt(&ifnt, &icol);
    satt(ifont, icol);
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
	free((VOID *) fntindx);
	free((VOID *) fntbffr);
	free((VOID *) fntlkup);
    }

    fontloaded = 1;
    charset = fnt;

    if (fnt)
	fontfile = plfontopen(PL_XFONT);
    else
	fontfile = plfontopen(PL_SFONT);

/* Read fntlkup[] */

#ifdef PLPLOT5_FONTS
    read_2bytes(fontfile, (U_SHORT *) &bffrleng);
#else
    fread((void *) &bffrleng, (size_t) sizeof(short), (size_t) 1, fontfile);
#endif
    numberfonts = bffrleng / 256;
    numberchars = bffrleng & 0xff;
    bffrleng = numberfonts * numberchars;
    fntlkup = (short int *) malloc(bffrleng * sizeof(short int));
    if (!fntlkup)
	plexit("plfntld: Out of memory while allocating font buffer.");

#ifdef PLPLOT5_FONTS
    read_2nbytes(fontfile, (U_SHORT *) fntlkup, bffrleng);
#else
    fread((void *) fntlkup, (size_t) sizeof(short int), (size_t) bffrleng,
	  fontfile);
#endif

/* Read fntindx[] */

#ifdef PLPLOT5_FONTS
    read_2bytes(fontfile, (U_SHORT *) &indxleng);
#else
    fread((void *) &indxleng, (size_t) sizeof(short), (size_t) 1, fontfile);
#endif
    fntindx = (short int *) malloc(indxleng * sizeof(short int));
    if (!fntindx)
	plexit("plfntld: Out of memory while allocating font buffer.");

#ifdef PLPLOT5_FONTS
    read_2nbytes(fontfile, (U_SHORT *) fntindx, indxleng);
#else
    fread((void *) fntindx, (size_t) sizeof(short int), (size_t) indxleng,
	  fontfile);
#endif

/* Read fntbffr[] */

#ifdef PLPLOT5_FONTS
    read_2bytes(fontfile, (U_SHORT *) &bffrleng);
#else
    fread((void *) &bffrleng, (size_t) sizeof(short), (size_t) 1, fontfile);
#endif
    fntbffr = (SCHAR *) malloc(2 * bffrleng * sizeof(SCHAR));
    if (!fntbffr)
	plexit("plfntld: Out of memory while allocating font buffer.");

    fread((void *) fntbffr, (size_t) sizeof(SCHAR), (size_t) (2 * bffrleng),
	  fontfile);

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
    char fnb[NFILEN];
    char *dn;

/****	search current directory	****/

    if ((plfp = fopen(fn, BINARY_READ)) != NULL)
	return (plfp);

/**** 	search $(HOME)/lib	****/

#ifdef HOME_LIB
    if ((dn = getenv("HOME")) != NULL) {
	if ((strlen(dn) + 5 + strlen(fn)) > NFILEN)
	    plexit("plfontopen: Too many characters in font file name.\n");

	(VOID) strcpy(fnb, dn);
	(VOID) strcat(fnb, "/lib/");
	(VOID) strcat(fnb, fn);

	if ((plfp = fopen(fnb, BINARY_READ)) != NULL)
	    return (plfp);
    }
#endif

/****	search $(PLFONTS)	****/

#ifdef PLFONTENV
    if ((dn = getenv(PLFONTENV)) != NULL) {
	if ((strlen(dn) + strlen(fn)) > NFILEN)
	    plexit("plfontopen: Too many characters in font file name.\n");

	(VOID) strcpy(fnb, dn);
	(VOID) strcat(fnb, fn);

	if ((plfp = fopen(fnb, BINARY_READ)) != NULL)
	    return (plfp);
    }
#endif

/**** 	search devices		****/

#ifdef PLFONTDEV1
    if ((strlen(PLFONTDEV1) + strlen(fn)) > NFILEN)
	plexit("plfontopen: Too many characters in font file name.\n");

    (VOID) strcpy(fnb, PLFONTDEV1);
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
    fnb[strlen(fnb) - 1] = '\0';
#endif
    (VOID) strcat(fnb, fn);

    if ((plfp = fopen(fnb, BINARY_READ)) != NULL)
	return (plfp);
#endif

#ifdef PLFONTDEV2
    if ((strlen(PLFONTDEV2) + strlen(fn)) > NFILEN)
	plexit("plfontopen: Too many characters in font file name.\n");

    (VOID) strcpy(fnb, PLFONTDEV2);
#ifdef MSDOS
    fnb[strlen(fnb) - 1] = '\0';
#endif
    (VOID) strcat(fnb, fn);

    if ((plfp = fopen(fnb, BINARY_READ)) != NULL)
	return (plfp);
#endif

#ifdef PLFONTDEV3
    if ((strlen(PLFONTDEV3) + strlen(fn)) > NFILEN)
	plexit("plfontopen: Too many characters in font file name.\n");

    (VOID) strcpy(fnb, PLFONTDEV3);
#ifdef MSDOS
    fnb[strlen(fnb) - 1] = '\0';
#endif
    (VOID) strcat(fnb, fn);

    if ((plfp = fopen(fnb, BINARY_READ)) != NULL)
	return (plfp);
#endif

/**** 	not found, give up (do NOT use plexit)	****/

    fprintf(stderr, "\nUnable to open font file: %s.\n", fn);
    fprintf(stderr, "Plplot aborted.\n");
    pl_exit();
    exit(1);
    return (NULL);		/* don't ask */
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
	free((VOID *) fntindx);
	free((VOID *) fntbffr);
	free((VOID *) fntlkup);
	fontloaded = 0;
    }
}
