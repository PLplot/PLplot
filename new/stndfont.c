/*	stndfont.c

	Utility to generate standard font set.
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
compare(si1, si2)
short *si1, *si2;
{
    return (*si1 == *si2 ? 0 : (*si1 > *si2 ? 1 : -1));
}


int 
main(argc, argv)
int argc;
char **argv;
{
    short i, j, k, ib, nstd, nchars, nleng, htab, nindx, zero;
    short *hrshlst, *hrshidx;
    SCHAR ix, iy;
    long fpos;
    FILE *fontfile;

    hrshlst = (short *) malloc(176 * sizeof(short));
    hrshidx = (short *) malloc(176 * sizeof(short));

    ib = 0;
    for (k = 0; k < 176; k++)
	hrshlst[ib++] = *(hersh[0] + k);

    /* Sort list */
    qsort((char *) hrshlst, ib, sizeof(short), compare);

for( k=0; k < 176; k++ )
printf( "%d ", hrshlst[k] );

    /* Remove duplicates */
    k = 0;
    j = 0;
    do {
	if (hrshlst[k] == hrshlst[j])
	    j++;
	else
	    hrshlst[++k] = hrshlst[j];
    } while (j < ib);

    nstd = k + 1;
    /* Now reindex the fonts */
    for (k = 0; k < 176; k++)
	for (i = 0; i < nstd; i++)
	    if (*(hersh[0] + k) == hrshlst[i]) {
		hrshidx[k] = i + 1;
		break;
	    }

for( k=0; k < 176; k++ )
printf( "%d ", hrshidx[k] );

    fontfile = fopen(PL_SFONT, BINARY_WRITE);
    if (!fontfile) {
	printf("Error opening standard font file.\n");
	exit(1);
    }

    htab = 1 * 256 + 176;	/* # of fonts in upper byte # of chars in lower */
    fwrite((char *) &htab, sizeof(short), 1, fontfile);
    fwrite((char *) hrshidx, sizeof(short), 176, fontfile);

    zero = 0;
    nindx = 0;
    nleng = 1;
    fpos = ftell(fontfile);
printf( "\n fpos = %d\n", fpos );

    fwrite((char *) &nindx, sizeof(short), 1, fontfile);
    for (j = 0; j < nstd; j++) {
	ib = *(findex[(hrshlst[j] - 1) / 100] + (hrshlst[j] - 1) % 100);
	if (ib == 0) {
	    fwrite((char *) &zero, sizeof(short), 1, fontfile);
	    nindx++;
printf(  "Wrote 0\n" );
	}
	else {
	    fwrite((char *) &nleng, sizeof(short), 1, fontfile);
printf( "wrote %d ", nleng );
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
    fseek(fontfile, fpos, 0);
    fwrite((char *) &nindx, sizeof(short), 1, fontfile);

    nchars = 0;
    nleng = 1;
    fseek(fontfile, 0, 2);	/* Go to end of file */
    fpos = ftell(fontfile);	/* Save current position */
printf( "fpos = %d\n", fpos );

    fwrite((char *) &nleng, sizeof(short), 1, fontfile);

printf( "\nstarting next suite at %d\n", ftell(fontfile) );
    for (j = 0; j < nstd; j++) {
	ib = *(findex[(hrshlst[j] - 1) / 100] + (hrshlst[j] - 1) % 100);
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
printf( "fputc ix=%d, iy=%d ", ix, iy );
		nleng++;
		if (ix == 64 && iy == 64)
		    break;
	    }
	    nchars++;
	}
    }
printf( "nleng=%d pos now = %d\n", nleng, ftell(fontfile) );
    nleng--;
    fseek(fontfile, fpos, 0);
    fwrite((char *) &nleng, sizeof(short), 1, fontfile);
    fclose(fontfile);
printf( "nleng=%d\n", nleng );
    printf("There are %d characters in standard font set.\n", nchars - 1);
    exit(0);
}
