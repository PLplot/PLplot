/* $Id$
   $Log$
   Revision 1.2  1992/09/29 04:45:26  furnish
   Massive clean up effort to remove support for garbage compilers (K&R).

 * Revision 1.1  1992/05/20  21:33:35  furnish
 * Initial checkin of the whole PLPLOT project.
 *
*/

/*	xtndfont.c

	Utility to generate extended font set.
*/

#include "plplot.h"
#include <stdio.h>
#include <string.h>

#ifdef PLSTDC
#include <stdlib.h>
#ifdef INCLUDE_MALLOC
#include <malloc.h>
#endif

#else
extern char *malloc();
#endif

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

    htab = 4 * 256 + 176;	/* # of fonts in upper byte # of chars in lower */
    fwrite((char *) &htab, sizeof(short), 1, fontfile);
    fwrite((char *) hrshlst, sizeof(short), 4 * 176, fontfile);

    nleng = 1;
    zero = 0;
    nindx = 0;
    fpos = ftell(fontfile);
    fwrite((char *) &nindx, sizeof(short), 1, fontfile);
    for (j = 0; j < 30; j++) {
	for (k = 0; k < 100; k++) {
	    ib = *(findex[j] + k);
	    if (ib == 0) {
		fwrite((char *) &zero, sizeof(short), 1, fontfile);
		nindx++;
	    }
	    else {
		fwrite((char *) &nleng, sizeof(short), 1, fontfile);
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
    fwrite((char *) &nindx, sizeof(short), 1, fontfile);

    fseek(fontfile, 0, 2);
    fpos = ftell(fontfile);
    nleng = 1;
    nchars = 0;
    fwrite((char *) &nleng, sizeof(short), 1, fontfile);
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
    fwrite((char *) &nleng, sizeof(short), 1, fontfile);
    fclose(fontfile);
    printf("There are %d characters in font set.\n", nchars - 1);
    exit(0);
}
