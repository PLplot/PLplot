/* $Id$
   $Log$
   Revision 1.5  1993/12/08 06:19:05  mjl
   Fixes so that these correctly build again (neglected to apply some global
   changes in a previous release).

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
    FILE *fontfile;

    hrshlst = (short *) malloc(4 * 176 * sizeof(short));

    ib = 0;
    for (j = 0; j < 4; j++)
	for (k = 0; k < 176; k++)
	    hrshlst[ib++] = *(hersh[j] + k);

    fontfile = fopen(PL_XFONT, BINARY_WRITE);
    if (!fontfile) {
	printf("Error opening extended font file.\n");
	exit(1);
    }

    htab = 4 * 256 + 176;

    pdf_wr_2bytes(fontfile, htab);
    pdf_wr_2nbytes(fontfile, (U_SHORT *) hrshlst, 4 * 176);

    nleng = 1;
    zero = 0;
    nindx = 0;
    fpos = ftell(fontfile);
    pdf_wr_2bytes(fontfile, nindx);
    for (j = 0; j < 30; j++) {
	for (k = 0; k < 100; k++) {
	    ib = *(findex[j] + k);
	    if (ib == 0) {
		pdf_wr_2bytes(fontfile, zero);
		nindx++;
	    }
	    else {
		pdf_wr_2bytes(fontfile, nleng);
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
    fseek(fontfile, fpos, 0);
    pdf_wr_2bytes(fontfile, nindx);

    fseek(fontfile, 0, 2);
    fpos = ftell(fontfile);
    nleng = 1;
    nchars = 0;
    pdf_wr_2bytes(fontfile, nleng);
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
		    fputc(ix, fontfile);
		    fputc(iy, fontfile);
		    nleng++;
		    if (ix == 64 && iy == 64)
			break;
		}
		nchars++;
	    }
	}
    }
    nleng--;
    fseek(fontfile, fpos, 0);
    pdf_wr_2bytes(fontfile, nleng);
    fclose(fontfile);
    printf("There are %d characters in font set.\n", nchars - 1);
    exit(0);
}
