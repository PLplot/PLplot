/* $Id$
   $Log$
   Revision 1.6  1994/01/15 17:22:18  mjl
   Changed to new PDF function syntax.

 * Revision 1.5  1993/12/08  06:19:05  mjl
 * Fixes so that these correctly build again (neglected to apply some global
 * changes in a previous release).
 *
 * Revision 1.4  1993/01/23  06:11:31  mjl
 * Added code to make generated font files device-independent.  No longer
 * any endian problem.
*/

/*	xtndfont.c

	Utility to generate extended font set.
*/

#define PL_NEED_MALLOC
#include "plplotP.h"

#include <stdio.h>
#include <string.h>
#include "pdf.h"

extern short int *hersh[];
extern short int *findex[];
extern short int *buffer[];

int 
main (void)
{

    short j, k, ib, nindx, nchars, nleng, htab, zero;
    short *hrshlst;
    SCHAR ix, iy;
    long fpos;
    PDFstrm *pdfs;

    hrshlst = (short *) malloc(4 * 176 * sizeof(short));

    ib = 0;
    for (j = 0; j < 4; j++)
	for (k = 0; k < 176; k++)
	    hrshlst[ib++] = *(hersh[j] + k);

    pdfs = pdf_fopen(PL_XFONT, BINARY_WRITE);
    if ( ! pdfs) {
	printf("Error opening extended font file.\n");
	exit(1);
    }

    htab = 4 * 256 + 176;

    pdf_wr_2bytes(pdfs, htab);
    pdf_wr_2nbytes(pdfs, (U_SHORT *) hrshlst, 4 * 176);

    nleng = 1;
    zero = 0;
    nindx = 0;
    fpos = ftell(pdfs->file);
    pdf_wr_2bytes(pdfs, nindx);
    for (j = 0; j < 30; j++) {
	for (k = 0; k < 100; k++) {
	    ib = *(findex[j] + k);
	    if (ib == 0) {
		pdf_wr_2bytes(pdfs, zero);
		nindx++;
	    }
	    else {
		pdf_wr_2bytes(pdfs, nleng);
		nindx++;
		for (;;) {
		    ix = *(buffer[ib / 100] + ib % 100) / 128 - 64;
		    iy = *(buffer[ib / 100] + ib % 100) % 128 - 64;
		    ib++;
		    if (ix == -64)
			ix = 64;
		    if (iy == -64)
			iy = 64;
		    nleng++;
		    if (ix == 64 && iy == 64)
			break;
		}
	    }
	}
    }
    fseek(pdfs->file, fpos, 0);
    pdf_wr_2bytes(pdfs, nindx);

    fseek(pdfs->file, 0, 2);
    fpos = ftell(pdfs->file);
    nleng = 1;
    nchars = 0;
    pdf_wr_2bytes(pdfs, nleng);
    for (j = 0; j < 30; j++) {
	for (k = 0; k < 100; k++) {
	    ib = *(findex[j] + k);
	    if (ib != 0) {
		for (;;) {
		    ix = *(buffer[ib / 100] + ib % 100) / 128 - 64;
		    iy = *(buffer[ib / 100] + ib % 100) % 128 - 64;
		    ib++;
		    if (ix == -64)
			ix = 64;
		    if (iy == -64)
			iy = 64;
		    fputc(ix, pdfs->file);
		    fputc(iy, pdfs->file);
		    nleng++;
		    if (ix == 64 && iy == 64)
			break;
		}
		nchars++;
	    }
	}
    }
    nleng--;
    fseek(pdfs->file, fpos, 0);
    pdf_wr_2bytes(pdfs, nleng);
    pdf_close(pdfs);

    printf("There are %d characters in font set.\n", nchars - 1);
    exit(0);
}
