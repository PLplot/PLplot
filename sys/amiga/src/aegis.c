/* $Id$
   $Log$
   Revision 1.2  1992/10/12 17:11:17  mjl
   Amiga-specific mods, including ANSI-fication.

 * Revision 1.1  1992/05/20  21:35:20  furnish
 * Initial checkin of the whole PLPLOT project.
 *
*/

/*	aegis.c

	PLPLOT Amiga Aegis draw format device driver.
*/

#include "plplot.h"
#include <stdio.h>
#include <string.h>
#include "dispatch.h"
#include "plamiga.h"

/* Function prototypes */

static void flushbuffer (PLStream *);

/* top level declarations */

#define AEGX       10000
#define AEGY       10000
#define BSIZE  25

static short *buffptr, bufflen;
static PLINT firstline;
static short count, xmin, xmax, ymin, ymax;

/* (dev) will get passed in eventually, so this looks weird right now */

static PLDev device;
static PLDev (*dev) = &device;

/*----------------------------------------------------------------------*\
* aegisinit()
*
* Initialize device.
\*----------------------------------------------------------------------*/

void 
aegisinit(PLStream *pls)
{
    pls->termin = 0;		/* not an interactive terminal */
    pls->color = 1;
    pls->width = 1;
    pls->bytecnt = 0;
    pls->page = 0;

/* Set up device parameters */

    dev->xold = UNDEFINED;
    dev->yold = UNDEFINED;
    dev->xmin = 0;
    dev->xmax = AEGX;
    dev->ymin = 0;
    dev->ymax = AEGY;

    setpxl((PLFLT) 39.37, (PLFLT) 39.37);

    setphy(0, AEGX, 0, AEGY);

    bufflen = 2 * BSIZE;
    buffptr = (short *) malloc(sizeof(short) * bufflen);
    if (buffptr == NULL)
	plexit("Out of memory!");
}

/*----------------------------------------------------------------------*\
* aegisline()
*
* Draw a line in the current color from (x1,y1) to (x2,y2).
\*----------------------------------------------------------------------*/

void 
aegisline(PLStream *pls, PLINT x1a, PLINT y1a, PLINT x2a, PLINT y2a)
{
    int x1=x1a, y1=y1a, x2=x2a, y2=y2a;
    short *tempptr;

    if (pls->pscale)
	plSclPhy(pls, dev, &x1, &y1, &x2, &y2);

    /* If starting point of this line is the same as the ending point of */
    /* the previous line then don't raise the pen. (This really speeds up */
    /* plotting and reduces the size of the file. */

    if (firstline) {
	count = 0;
	*(buffptr + count++) = x1;
	*(buffptr + count++) = y1;
	*(buffptr + count++) = x2;
	*(buffptr + count++) = y2;
	xmin = MIN(x1, x2);
	ymin = MIN(y1, y2);
	xmax = MAX(x1, x2);
	ymax = MAX(y1, y2);
	firstline = 0;
    }
    else if (x1 == dev->xold && y1 == dev->yold) {
	if (count + 2 >= bufflen) {
	    bufflen += 2 * BSIZE;
	    tempptr = (short *) realloc((void *) buffptr, bufflen * sizeof(short));
	    if (tempptr == NULL) {
		free((void *) buffptr);
		plexit("Out of memory!");
	    }
	    buffptr = tempptr;
	}
	*(buffptr + count++) = x2;
	*(buffptr + count++) = y2;
	xmin = MIN(x2, xmin);
	ymin = MIN(y2, ymin);
	xmax = MAX(x2, xmax);
	ymax = MAX(y2, ymax);
    }
    else {
	flushbuffer(pls);
	*(buffptr + count++) = x1;
	*(buffptr + count++) = y1;
	*(buffptr + count++) = x2;
	*(buffptr + count++) = y2;
	xmin = MIN(x1, x2);
	ymin = MIN(y1, y2);
	xmax = MAX(x1, x2);
	ymax = MAX(y1, y2);
    }

    dev->xold = x2;
    dev->yold = y2;
}

/*----------------------------------------------------------------------*\
* aegisclear()
*
* Clear page.  
* Here need to close file since only 1 page/file is allowed.
\*----------------------------------------------------------------------*/

void 
aegisclear(PLStream *pls)
{
    /* Close the file */
    if (!firstline) {
	flushbuffer(pls);
    }
    fclose(pls->OutFile);
}

/*----------------------------------------------------------------------*\
* aegispage()
*
* Set up for the next page.  
* Advance to next family file if necessary (file output).
* Here need to open a new file since only 1 page/file is allowed.
\*----------------------------------------------------------------------*/

void 
aegispage(PLStream *pls)
{
    plOpenFile(pls);

    firstline = 1;
    dev->xold = UNDEFINED;
    dev->yold = UNDEFINED;
    pls->page++;

    /* Write out header */
    fprintf(pls->OutFile, "81086 0.0 0.0 100.0 100.0 0 10.\n");
    fprintf(pls->OutFile, "\"%s\"\n-1\n", pls->FileName);
}

/*----------------------------------------------------------------------*\
* aegisadv()
*
* Advance to the next page.
\*----------------------------------------------------------------------*/

void 
aegisadv(PLStream *pls)
{
    aegisclear(pls);
    aegispage(pls);
}

/*----------------------------------------------------------------------*\
* aegistidy()
*
* Close graphics file or otherwise clean up.
\*----------------------------------------------------------------------*/

void 
aegistidy(PLStream *pls)
{
    flushbuffer(pls);
    free((VOID *) buffptr);
    fclose(pls->OutFile);
    pls->page = 0;
    pls->OutFile = NULL;
}


/*----------------------------------------------------------------------*\
* aegiscolor()
*
* Set pen color.
\*----------------------------------------------------------------------*/

void 
aegiscolor(PLStream *pls)
{
    flushbuffer(pls);
    firstline = 1;

    /* Aegis pen 1 is the "paper" color */

    if (pls->color < 2 || pls->color > 15)
	pls->color = 0;
}

/*----------------------------------------------------------------------*\
* aegistext()
*
* Switch to text mode.
\*----------------------------------------------------------------------*/

void 
aegistext(PLStream *pls)
{
}

/*----------------------------------------------------------------------*\
* aegisgraph()
*
* Switch to graphics mode.
\*----------------------------------------------------------------------*/

void 
aegisgraph(pls)
PLStream *pls;
{
}

/*----------------------------------------------------------------------*\
* aegiswidth()
*
* Set pen width.
\*----------------------------------------------------------------------*/

void 
aegiswidth(PLStream *pls)
{
    flushbuffer(pls);
    firstline = 1;

    if (pls->width < 1)
	pls->width = 1;
    else if (pls->width > 4)
	pls->width = 4;
}

/*----------------------------------------------------------------------*\
* aegisesc()
*
* Escape function.
\*----------------------------------------------------------------------*/

void 
aegisesc(PLStream *pls, PLINT op, char *ptr)
{
}

/*----------------------------------------------------------------------*\
* flushbuffer()
*
* Spit out buffer to file.
\*----------------------------------------------------------------------*/

static void 
flushbuffer(PLStream *pls)
{
    short i = 0;

    fprintf(pls->OutFile, "1 52 %.2f %.2f", xmin / 100., ymin / 100.);
    fprintf(pls->OutFile, " %.2f %.2f", xmax / 100., ymax / 100.);
    fprintf(pls->OutFile, " %d 0 0 %d 0\n", pls->color, pls->width-1);
    while (i < count) {
	fprintf(pls->OutFile, " 1 %.2f %.2f\n", *(buffptr + i) / 100., *(buffptr + i + 1) / 100.);
	i += 2;
    }
    fprintf(pls->OutFile, " 0\n");
    count = 0;
}
