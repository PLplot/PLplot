/* $Id$
   $Log$
   Revision 1.2  1992/09/29 04:44:43  furnish
   Massive clean up effort to remove support for garbage compilers (K&R).

 * Revision 1.1  1992/05/20  21:32:36  furnish
 * Initial checkin of the whole PLPLOT project.
 *
*/

/*	impress.c

	PLPLOT ImPress device driver.
*/
#ifdef IMP

#include <stdio.h>
#include "plplot.h"
#include "dispatch.h"

#ifdef PLSTDC
#include <stdlib.h>
#ifdef INCLUDE_MALLOC
#include <malloc.h>
#endif

#else
extern char *malloc();
extern void free();
#endif

/* Function prototypes */

static void flushline(PLStream *);

/* top level declarations */

#define IMPX		2999
#define IMPY		2249

#define BUFFPTS		50
#define BUFFLENG	2*BUFFPTS

/* Graphics control characters. */

#define SET_HV_SYSTEM	0315
#define OPBYTE1		031
#define OPBYTE2		0140
#define SET_ABS_H	0207
#define SET_ABS_V	0211
#define OPWORDH1	0
#define OPWORDH2	150
#define OPWORDV1	0
#define OPWORDV2	150
#define ENDPAGE		0333

#define SET_PEN		0350
#define CREATE_PATH	0346
#define DRAW_PATH	0352
#define OPTYPE		017

static short *LineBuff;
static short FirstLine;
static int penchange = 0, penwidth;
static short count;

/* (dev) will get passed in eventually, so this looks weird right now */

static PLDev device;
static PLDev *dev = &device;

/*----------------------------------------------------------------------*\
* impinit()
*
* Initialize device (terminal).
\*----------------------------------------------------------------------*/

void 
impinit (PLStream *pls)
{
    pls->termin = 0;		/* not an interactive terminal */
    pls->color = 1;
    pls->width = 1;
    pls->bytecnt = 0;
    pls->page = 0;

/* Initialize family file info */

    plFamInit(pls);

/* Prompt for a file name if not already set */

    plOpenFile(pls);

/* Set up device parameters */

    dev->xold = UNDEFINED;
    dev->yold = UNDEFINED;
    dev->xmin = 0;
    dev->xmax = IMPX;
    dev->ymin = 0;
    dev->ymax = IMPY;

    setpxl((PLFLT) 11.81, (PLFLT) 11.81);

    if (!pls->orient)
	setphy(0, IMPX, 0, IMPY);
    else
	setphy(0, IMPY, 0, IMPX);

    LineBuff = (short *) malloc(BUFFLENG * sizeof(short));
    if (LineBuff == NULL) {
	plexit("Error in memory alloc in impini().");
    }
    fprintf(pls->OutFile, "@Document(Language ImPress, jobheader off)");
    fprintf(pls->OutFile, "%c%c", SET_HV_SYSTEM, OPBYTE1);
    fprintf(pls->OutFile, "%c%c%c", SET_ABS_H, OPWORDH1, OPWORDH2);
    fprintf(pls->OutFile, "%c%c%c", SET_ABS_V, OPWORDV1, OPWORDV2);
    fprintf(pls->OutFile, "%c%c", SET_HV_SYSTEM, OPBYTE2);
}

/*----------------------------------------------------------------------*\
* impline()
*
* Draw a line in the current color from (x1,y1) to (x2,y2).
\*----------------------------------------------------------------------*/

void 
impline (PLStream *pls, PLINT x1a, PLINT y1a, PLINT x2a, PLINT y2a)
{
    int x1=x1a, y1=y1a, x2=x2a, y2=y2a;

    plRotPhy(pls, dev, &x1, &y1, &x2, &y2);
    if (pls->pscale)
	plSclPhy(pls, dev, &x1, &y1, &x2, &y2);

    if (FirstLine) {
	if (penchange) {
	    fprintf(pls->OutFile, "%c%c", SET_PEN, (char) penwidth);
	    penchange = 0;
	}
	/* Add both points to path */
	count = 0;
	FirstLine = 0;
	*(LineBuff + count++) = (short) x1;
	*(LineBuff + count++) = (short) y1;
	*(LineBuff + count++) = (short) x2;
	*(LineBuff + count++) = (short) y2;
    }
    else if ((count + 2) < BUFFLENG && x1 == dev->xold && y1 == dev->yold) {
	/* Add new point to path */
	*(LineBuff + count++) = (short) x2;
	*(LineBuff + count++) = (short) y2;
    }
    else {
	/* Write out old path */
	count /= 2;
	fprintf(pls->OutFile, "%c%c%c", CREATE_PATH, (char) count / 256, (char) count % 256);
	fwrite((char *) LineBuff, sizeof(short), 2 * count, pls->OutFile);
	fprintf(pls->OutFile, "%c%c", DRAW_PATH, OPTYPE);

	/* And start a new path */
	if (penchange) {
	    fprintf(pls->OutFile, "%c%c", SET_PEN, (char) penwidth);
	    penchange = 0;
	}
	count = 0;
	*(LineBuff + count++) = (short) x1;
	*(LineBuff + count++) = (short) y1;
	*(LineBuff + count++) = (short) x2;
	*(LineBuff + count++) = (short) y2;
    }
    dev->xold = x2;
    dev->yold = y2;
}

/*----------------------------------------------------------------------*\
* impclear()
*
* Clear page. 
\*----------------------------------------------------------------------*/

void 
impclear (PLStream *pls)
{
    flushline(pls);
    fprintf(pls->OutFile, "%c", ENDPAGE);
}

/*----------------------------------------------------------------------*\
* imppage()
*
* Set up for the next page.  
* Advance to next family file if necessary (file output).
\*----------------------------------------------------------------------*/

void 
imppage (PLStream *pls)
{
    FirstLine = 1;
    dev->xold = UNDEFINED;
    dev->yold = UNDEFINED;

    if (!pls->termin)
	plGetFam(pls);

    pls->page++;
}

/*----------------------------------------------------------------------*\
* impadv()
*
* Advance to the next page.
\*----------------------------------------------------------------------*/

void 
impadv (PLStream *pls)
{
    impclear(pls);
    imppage(pls);
}

/*----------------------------------------------------------------------*\
* imptidy()
*
* Close graphics file or otherwise clean up.
\*----------------------------------------------------------------------*/

void 
imptidy (PLStream *pls)
{
    flushline(pls);
    fprintf(pls->OutFile, "%c", ENDPAGE);
    free((char *) LineBuff);
    fclose(pls->OutFile);
    pls->fileset = 0;
    pls->page = 0;
    pls->OutFile = NULL;
}

/*----------------------------------------------------------------------*\
* impcolor()
*
* Set pen color.
\*----------------------------------------------------------------------*/

void 
impcolor (PLStream *pls)
{
}

/*----------------------------------------------------------------------*\
* imptext()
*
* Switch to text mode.
\*----------------------------------------------------------------------*/

void 
imptext (PLStream *pls)
{
}

/*----------------------------------------------------------------------*\
* impgraph()
*
* Switch to graphics mode.
\*----------------------------------------------------------------------*/

void 
impgraph (PLStream *pls)
{
}

/*----------------------------------------------------------------------*\
* impwidth()
*
* Set pen width.
\*----------------------------------------------------------------------*/

void 
impwidth (PLStream *pls)
{
    if (pls->width > 0 && pls->width <= 20) {
	penwidth = pls->width;
	penchange = 1;
    }
}

/*----------------------------------------------------------------------*\
* impesc()
*
* Escape function.
\*----------------------------------------------------------------------*/

void 
impesc (PLStream *pls, PLINT op, char *ptr)
{
}

/*----------------------------------------------------------------------*\
* flushline()
*
* Spits out the line buffer.
\*----------------------------------------------------------------------*/

static void 
flushline (PLStream *pls)
{
    count /= 2;
    fprintf(pls->OutFile, "%c%c%c", CREATE_PATH, (char) count / 256, (char) count % 256);
    fwrite((char *) LineBuff, sizeof(short), 2 * count, pls->OutFile);
    fprintf(pls->OutFile, "%c%c", DRAW_PATH, OPTYPE);
    FirstLine = 1;
}
#endif	/* IMP */
