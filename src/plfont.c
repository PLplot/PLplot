/* $Id$
 * $Log$
 * Revision 1.15  1994/07/26 21:14:44  mjl
 * Improvements to the way PLplot looks for various files.  Now more
 * consistent and flexible.  In particular, environmentals can be set for
 * locations of each directory (for Tcl, binary, and library files).
 * Contributed by Mark Olesen.
 *
 * Revision 1.12  1994/05/24  19:56:56  mjl
 * Changed INSTALL_DIR to LIB_DIR for locating fonts.
 *
 * Revision 1.11  1994/03/23  08:14:55  mjl
 * Some cruft elimination.
 *
 * All external API source files: replaced call to plexit() on simple
 * (recoverable) errors with simply printing the error message (via
 * plabort()) and returning.  Should help avoid loss of computer time in some
 * critical circumstances (during a long batch run, for example).
 *
 * Revision 1.10  1994/01/15  17:28:45  mjl
 * Changed to new PDF function call syntax.
 *
 * Revision 1.9  1993/09/08  02:40:16  mjl
 * Added search of INSTALL_DIR, passed in from makefile.  Directories
 * now can be specified without the trailing slash, and the path name
 * is built up correctly (I hope) on Unix, Amiga, and MS-DOS (so special
 * handling for passing strings with a trailing backslash is gone).
*/

/*	plfont.c
 *
 *	Font management code.
 *	See the description of plfontopen() for the search path used in
 *	finding the font files.
*/

#include "plplotP.h"
#include "pdf.h"
#include <string.h>

/* An additional hardwired location is sometimes useful */

#if defined(AMIGA)
#ifndef PLFONTDEV
#define PLFONTDEV  "fonts:plplot"
#endif

#elif defined(GNU386)
#ifndef PLFONTDEV
#define PLFONTDEV "c:/lib"
#endif

#elif defined(MSDOS)
#ifndef PLFONTDEV
#define PLFONTDEV "c:\\lib"
#endif

#else

/* Anything else is assumed to be Unix */

#ifndef PLFONTDEV
#define PLFONTDEV "/usr/local/lib"
#endif

#endif

/* Function prototypes. */

static FILE	*plfontopen	(char *);

/* Declarations */

short int *fntlkup;
short int *fntindx;
SCHAR *fntbffr;
short int numberfonts, numberchars;
short int indxleng;

static short fontloaded = 0;

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
    FILE *file;
    PDFstrm *pdfs;

    if (fontloaded && (charset == fnt))
	return;

    plfontrel();
    fontloaded = 1;
    charset = fnt;

    if (fnt)
	file = plfontopen(PL_XFONT);
    else
	file = plfontopen(PL_SFONT);

    pdfs = pdf_finit(file);
    if ( ! pdfs)
	plexit("plfntld: Out of memory while allocating PDF stream data.");

/* Read fntlkup[] */

    pdf_rd_2bytes(pdfs, (U_SHORT *) &bffrleng);
    numberfonts = bffrleng / 256;
    numberchars = bffrleng & 0xff;
    bffrleng = numberfonts * numberchars;
    fntlkup = (short int *) malloc(bffrleng * sizeof(short int));
    if ( ! fntlkup)
	plexit("plfntld: Out of memory while allocating font buffer.");

    pdf_rd_2nbytes(pdfs, (U_SHORT *) fntlkup, bffrleng);

/* Read fntindx[] */

    pdf_rd_2bytes(pdfs, (U_SHORT *) &indxleng);
    fntindx = (short int *) malloc(indxleng * sizeof(short int));
    if ( ! fntindx)
	plexit("plfntld: Out of memory while allocating font buffer.");

    pdf_rd_2nbytes(pdfs, (U_SHORT *) fntindx, indxleng);

/* Read fntbffr[] */
/* Since this is an array of char, there are no endian problems */

    pdf_rd_2bytes(pdfs, (U_SHORT *) &bffrleng);
    fntbffr = (SCHAR *) malloc(2 * bffrleng * sizeof(SCHAR));
    if ( ! fntbffr)
	plexit("plfntld: Out of memory while allocating font buffer.");

    fread((void *) fntbffr, (size_t) sizeof(SCHAR),
	  (size_t) (2 * bffrleng), pdfs->file);

/* Done */

    pdf_close(pdfs);
}

/*----------------------------------------------------------------------*\
 * FILE *plfontopen(fn)
 *
 * Return file pointer to font file.
 * Locations checked:
 *	PLPLOT_LIB_ENV = $(PLPLOT_LIB)
 *	current directory
 *	PLPLOT_HOME_ENV/lib = $(PLPLOT_HOME)/lib
 *	LIB_DIR
 *	PLFONTDEV
\*----------------------------------------------------------------------*/

static FILE *
plfontopen(char *fn)
{
    FILE *file;
    char *fs = NULL, *dn = NULL;

/****	search PLPLOT_LIB_ENV = $(PLPLOT_LIB)	****/

#if defined(PLPLOT_LIB_ENV)
    if ((dn = getenv(PLPLOT_LIB_ENV)) != NULL) {
        plGetName(dn, "", fn, &fs);

        if ((file = fopen(fs, "rb")) != NULL)
            goto done;

        fprintf(stderr, PLPLOT_LIB_ENV"=\"%s\"\n", dn); /* what IS set? */
    }
#endif  /* PLPLOT_LIB_ENV */

/****	search current directory	****/

    if ((file = fopen(fn, "rb")) != NULL)
        goto done;

/****	search PLPLOT_HOME_ENV/lib = $(PLPLOT_HOME)/lib	****/

#if defined (PLPLOT_HOME_ENV)
    if ((dn = getenv(PLPLOT_HOME_ENV)) != NULL) {
        plGetName(dn, "lib", fn, &fs);

        if ((file = fopen(fs, "rb")) != NULL)
            goto done;
        fprintf(stderr, PLPLOT_HOME_ENV"=\"%s\"\n",dn); /* what IS set? */
    }
#endif  /* PLPLOT_HOME_ENV/lib */

/**** 	search installed location	****/

#if defined (LIB_DIR)
    plGetName(LIB_DIR, "", fn, &fs);

    if ((file = fopen(fs, "rb")) != NULL)
        goto done;
#endif  /* LIB_DIR */

/**** 	search hardwired location	****/

#ifdef PLFONTDEV
    plGetName(PLFONTDEV, "", fn, &fs);

    if ((file = fopen(fs, "rb")) != NULL)
	goto done;
#endif	/* PLFONTDEV */

/**** 	not found, give up 	****/

    fprintf(stderr, "\nCannot open font file: %s\n", fn);
#if defined (LIB_DIR)
    fprintf(stderr, "lib dir=\"" LIB_DIR "\"\n" );      /* what WAS set? */
#endif  /* LIB_DIR */
    plexit("");

 done:
    free_mem(fs);
    return (file);
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
	free_mem(fntindx)
	free_mem(fntbffr)
	free_mem(fntlkup)
	fontloaded = 0;
    }
}
