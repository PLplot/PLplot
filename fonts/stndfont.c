/* $Id$
   $Log$
   Revision 1.4  1993/01/23 06:11:30  mjl
   Added code to make generated font files device-independent.  No longer
   any endian problem.

 * Revision 1.3  1992/09/30  18:25:28  furnish
 * Massive cleanup to irradicate garbage code.  Almost everything is now
 * prototyped correctly.  Builds on HPUX, SUNOS (gcc), AIX, and UNICOS.
 *
 * Revision 1.2  1992/09/29  04:45:25  furnish
 * Massive clean up effort to remove support for garbage compilers (K&R).
 *
 * Revision 1.1  1992/05/20  21:33:34  furnish
 * Initial checkin of the whole PLPLOT project.
 *
*/

/*	stndfont.c

	Utility to generate standard font set.
*/

#define PL_NEED_MALLOC
#include "plplot.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "pdf.h"

extern short int *hersh[];
extern short int *findex[];
extern short int *buffer[];
#define DEBUG 0

int 
compare (const void *si1, const void *si2)
{
    short *a = (short *) si1;
    short *b = (short *) si2;
    
    return (*a == *b ? 0 : ( *a > *b ? 1 : -1));
}

int 
main (int argc, char **argv)
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

#if DEBUG
for( k=0; k < 176; k++ )
	printf( "%d ", hrshlst[k] );
#endif
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

#if DEBUG
for( k=0; k < 176; k++ )
	printf( "%d ", hrshidx[k] );
#endif
    fontfile = fopen(PL_SFONT, BINARY_WRITE);
    if (!fontfile) {
	printf("Error opening standard font file.\n");
	exit(1);
    }

    htab = 1 * 256 + 176;	/* # of fonts in upper byte # of chars in lower */
#ifdef PLPLOT5_FONTS
    write_2bytes(fontfile, htab);
    write_2nbytes(fontfile, (U_SHORT *) hrshidx, 176);
#else
    fwrite((char *) &htab, sizeof(short), 1, fontfile);
    fwrite((char *) hrshidx, sizeof(short), 176, fontfile);
#endif

    zero = 0;
    nindx = 0;
    nleng = 1;
    fpos = ftell(fontfile);
#if DEBUG
	printf( "\n fpos = %d\n", fpos );
#endif
#ifdef PLPLOT5_FONTS
    write_2bytes(fontfile, nindx);
#else
    fwrite((char *) &nindx, sizeof(short), 1, fontfile);
#endif
    for (j = 0; j < nstd; j++) {
	ib = *(findex[(hrshlst[j] - 1) / 100] + (hrshlst[j] - 1) % 100);
	if (ib == 0) {
#ifdef PLPLOT5_FONTS
	    write_2bytes(fontfile, zero);
#else
	    fwrite((char *) &zero, sizeof(short), 1, fontfile);
#endif
	    nindx++;
#if DEBUG
	printf(  "Wrote 0\n" );
#endif
	}
	else {
#ifdef PLPLOT5_FONTS
	    write_2bytes(fontfile, nleng);
#else
	    fwrite((char *) &nleng, sizeof(short), 1, fontfile);
#endif
#if DEBUG
	printf( "wrote %d ", nleng );
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
    fseek(fontfile, fpos, 0);
#ifdef PLPLOT5_FONTS
    write_2bytes(fontfile, nindx);
#else
    fwrite((char *) &nindx, sizeof(short), 1, fontfile);
#endif

    nchars = 0;
    nleng = 1;
    fseek(fontfile, 0, 2);	/* Go to end of file */
    fpos = ftell(fontfile);	/* Save current position */
#if DEBUG
	printf( "fpos = %d\n", fpos );
#endif
#ifdef PLPLOT5_FONTS
    write_2bytes(fontfile, nleng);
#else
    fwrite((char *) &nleng, sizeof(short), 1, fontfile);
#endif

#if DEBUG
	printf( "\nstarting next suite at %d\n", ftell(fontfile) );
#endif
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
#if DEBUG
	printf( "fputc ix=%d, iy=%d ", ix, iy );
#endif
		nleng++;
		if (ix == 64 && iy == 64)
		    break;
	    }
	    nchars++;
	}
    }
#if DEBUG
	printf( "nleng=%d pos now = %d\n", nleng, ftell(fontfile) );
#endif
    nleng--;
    fseek(fontfile, fpos, 0);
#ifdef PLPLOT5_FONTS
    write_2bytes(fontfile, nleng);
#else
    fwrite((char *) &nleng, sizeof(short), 1, fontfile);
#endif
    fclose(fontfile);
#if DEBUG
	printf( "nleng=%d\n", nleng );
#endif
    printf("There are %d characters in standard font set.\n", nchars - 1);
    exit(0);
}
