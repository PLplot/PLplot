/* $Id$
 * $Log$
 * Revision 1.16  1994/07/29 20:24:42  mjl
 * References to plfontopen() deleted, in favor of using plLibOpen().
 *
 * Revision 1.15  1994/07/26  21:14:44  mjl
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
 *	See the description of plLibOpen() for the search path used in
 *	finding the font files.
*/

#include "plplotP.h"
#include "pdf.h"
#include <string.h>

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
	file = plLibOpen(PL_XFONT);
    else
	file = plLibOpen(PL_SFONT);

    if (file == NULL)
	plexit("Unable to open font file");

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
