/* $Id$
 * $Log$
 * Revision 1.14  1994/07/02 21:32:47  mjl
 * Some changes to the default font directories under MSDOS, submitted
 * by Neal Holtz.
 *
 * Revision 1.13  1994/06/30  18:22:09  mjl
 * All core source files: made another pass to eliminate warnings when using
 * gcc -Wall.  Lots of cleaning up: got rid of includes of math.h or string.h
 * (now included by plplot.h), and other minor changes.  Now each file has
 * global access to the plstream pointer via extern; many accessor functions
 * eliminated as a result.
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
*/

/*	plfont.c
*
*	Font management code.
*
* The current directory is always searched for the fonts first, followed
* by a system-dependent (and perhaps site-dependent) search path.
* Directory names can be given with or without the trailing slash.  Each
* system has three hard-wired devices (may be a logical name) that are
* searched (default value listed below).  The value for LIB_DIR is
* set from the makefile.
*
* Unix:
*	current directory
*	$(HOME)/lib
*	$(PLPLOT_DIR)
*	LIB_DIR
*	PLFONTDEV1	(/usr/local/lib)
*	PLFONTDEV2	(/usr/local/lib/plplot
*	PLFONTDEV3	(/usr/local/plplot)
*
* VMS:
*	current directory
*	LIB_DIR
*	PLFONTDEV1	(lib:)
*	PLFONTDEV2	(sys$login:)
*	PLFONTDEV3	(sys$sysroot:[sysfont.plplot])
*
* Amiga:
*	current directory
*	$(PLPLOT_DIR)
*	LIB_DIR
*	PLFONTDEV1	(fonts:plplot)
*	PLFONTDEV2	(plfonts:) 
*/

#include "plplotP.h"
#include "pdf.h"

 /* MSDOS search path */

#ifdef MSDOS
#define PLFONTENV "PLPLOT_DIR"	/* C> set PLPLOT_DIR=dir_name_for_fonts */
#endif

 /* Unix search path */

#ifdef __unix
#define HOME_LIB
#define PLFONTENV  "PLPLOT_DIR"

#ifndef PLFONTDEV1
#define PLFONTDEV1 "/usr/local/lib"
#endif
#ifndef PLFONTDEV2
#define PLFONTDEV2 "/usr/local/lib/plplot"
#endif
#ifndef PLFONTDEV3
#define PLFONTDEV3 "/usr/local/plplot"
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
#define PLFONTENV  "PLPLOT_DIR"
#ifndef PLFONTDEV1
#define PLFONTDEV1  "fonts:plplot"
#endif
#ifndef PLFONTDEV2
#define PLFONTDEV2  "plfonts:"
#endif
#endif

#ifdef GNU386
#include <stddef.h>
#ifndef PLFONTDEV1
#define PLFONTDEV1 "c:/lib"
#endif
#ifndef PLFONTDEV2
#define PLFONTDEV2 "d:/util/plplot/lib"
#endif
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
 * Lots of locations checked; see documentation for plfntld().
\*----------------------------------------------------------------------*/

static FILE *
plfontopen(char *fn)
{
    FILE *file;
    char *fs = NULL, *dn = NULL;

/****	search current directory	****/

    if ((file = fopen(fn, "rb")) != NULL)
	goto done;

/**** 	search $(HOME)/lib	****/

#ifdef HOME_LIB
    if ((dn = getenv("HOME")) != NULL) {
	plGetName(dn, "lib", fn, &fs);

	if ((file = fopen(fs, "rb")) != NULL)
	    goto done;
    }
#endif

/****	search $(PLPLOT_DIR)	****/

#ifdef PLFONTENV
    if ((dn = getenv(PLFONTENV)) != NULL) {
	plGetName(dn, "", fn, &fs);

	if ((file = fopen(fs, "rb")) != NULL)
	    goto done;
    }
#endif

/**** 	search devices		****/

#ifdef LIB_DIR
    plGetName(LIB_DIR, "", fn, &fs);

    if ((file = fopen(fs, "rb")) != NULL)
	goto done;
#endif

#ifdef PLFONTDEV1
    plGetName(PLFONTDEV1, "", fn, &fs);

    if ((file = fopen(fs, "rb")) != NULL)
	goto done;
#endif

#ifdef PLFONTDEV2
    plGetName(PLFONTDEV2, "", fn, &fs);

    if ((file = fopen(fs, "rb")) != NULL)
	goto done;
#endif

#ifdef PLFONTDEV3
    plGetName(PLFONTDEV3, "", fn, &fs);

    if ((file = fopen(fs, "rb")) != NULL)
	goto done;
#endif

/**** 	not found, give up 	****/

    fprintf(stderr, "\nUnable to open font file: %s.\n", fn);
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
