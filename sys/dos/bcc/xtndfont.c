/* $Id$
   $Log$
   Revision 1.2.2.1  2001/04/19 12:31:46  rlaboiss
   First merge against MAIN

   Revision 1.2  1994/05/25 22:08:25  mjl
   Win3 driver added, other changes to bring DOS/BGI (bcc) support
   up to date, contributed by Paul Casteels.

 * Revision 1.1  1993/10/18  17:07:17  mjl
 * Initial checkin of files for DOS/Borland C.
 *
*/

/*      xtndfont.c

	Utility to generate extended font set.
*/

#define PL_NEED_MALLOC
#include "plplotP.h"

#include <stdio.h>
/* 
  PaulC added stdlib.h 
  changed PLPLOT5_FONT in PLPLOT51_FONT to inhibit the use of
  write_2byte and write_2nbyte
*/
#include <stdlib.h>
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

    htab = 4 * 256 + 176;       /* # of fonts in upper byte # of chars in lower */
#ifdef PLPLOT51_FONTS
    write_2bytes(fontfile, htab);
    write_2nbytes(fontfile, (U_SHORT *) hrshlst, 4 * 176);
#else
    fwrite((char *) &htab, sizeof(short), 1, fontfile);
    fwrite((char *) hrshlst, sizeof(short), 4 * 176, fontfile);
#endif

    nleng = 1;
    zero = 0;
    nindx = 0;
    fpos = ftell(fontfile);
#ifdef PLPLOT51_FONTS
    write_2bytes(fontfile, nindx);
#else
    fwrite((char *) &nindx, sizeof(short), 1, fontfile);
#endif
    for (j = 0; j < 30; j++) {
	for (k = 0; k < 100; k++) {
	    ib = *(findex[j] + k);
	    if (ib == 0) {
#ifdef PLPLOT51_FONTS
		write_2bytes(fontfile, zero);
#else
		fwrite((char *) &zero, sizeof(short), 1, fontfile);
#endif
		nindx++;
	    }
	    else {
#ifdef PLPLOT51_FONTS
		write_2bytes(fontfile, nleng);
#else
		fwrite((char *) &nleng, sizeof(short), 1, fontfile);
#endif
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
#ifdef PLPLOT51_FONTS
    write_2bytes(fontfile, nindx);
#else
    fwrite((char *) &nindx, sizeof(short), 1, fontfile);
#endif

    fseek(fontfile, 0, 2);
    fpos = ftell(fontfile);
    nleng = 1;
    nchars = 0;
#ifdef PLPLOT51_FONTS
    write_2bytes(fontfile, nleng);
#else
    fwrite((char *) &nleng, sizeof(short), 1, fontfile);
#endif
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
#ifdef PLPLOT51_FONTS
    write_2bytes(fontfile, nleng);
#else
    fwrite((char *) &nleng, sizeof(short), 1, fontfile);
#endif
    fclose(fontfile);
    printf("There are %d characters in font set.\n", nchars - 1);
    exit(0);
}
