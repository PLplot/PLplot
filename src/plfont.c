/* $Id$
 * $Log$
 * Revision 1.17  1994/08/25 04:04:25  mjl
 * Eliminated unnecessary header file inclusions.
 *
 * Revision 1.16  1994/07/29  20:24:42  mjl
 * References to plfontopen() deleted, in favor of using plLibOpen().
 *
 * Revision 1.15  1994/07/26  21:14:44  mjl
 * Improvements to the way PLplot looks for various files.  Now more
 * consistent and flexible.  In particular, environmentals can be set for
 * locations of each directory (for Tcl, binary, and library files).
 * Contributed by Mark Olesen.
*/

/*	plfont.c
 *
 *	Font management code.
 *	See the description of plLibOpen() for the search path used in
 *	finding the font files.
*/

#include "plplotP.h"

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
