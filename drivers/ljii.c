/* $Id$
   $Log$
   Revision 1.1  1992/05/20 21:32:37  furnish
   Initial checkin of the whole PLPLOT project.

*/

/*	ljii.c

	PLPLOT Laser Jet II device driver.
	Note only the 150 dpi mode is supported.  The others (75,100,300) 
	should work by just changing the value of DPI and changing the 
	values passed to setphy().
*/
#ifdef LJII

#include <stdio.h>
#include "plplot.h"
#include "dispatch.h"
#include <math.h>

#ifdef MSDOS
#include <malloc.h>
#endif

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

static void setpoint	PLARGS((PLINT, PLINT));

/* top level declarations */

#define JETX     1409
#define JETY     1103

#define DPI      150		/* Resolution Dots per Inch */
#define CURX     51
#define CURY     61
#define XDOTS	 1104L		/* # dots across */
#define YDOTS	 1410L		/* # dots down	 */
#define BPROW	 XDOTS/8L	/* # bytes across */
#define NBYTES	 BPROW*YDOTS	/* total number of bytes */

/* Graphics control characters. */

#define ESC      0x1b
#define FF       0x0c

static char mask[8] = 
	{'\200', '\100', '\040', '\020', '\010', '\004', '\002', '\001'};

#ifndef MSDOS
#define _HUGE
#else
#define _HUGE _huge
#endif

static char _HUGE *bitmap;	/* pointer to an area of memory NBYTES in size */

/* (dev) will get passed in eventually, so this looks weird right now */

static PLDev device;
static PLDev *dev = &device;

/*----------------------------------------------------------------------*\
* jetinit()
*
* Initialize device.
\*----------------------------------------------------------------------*/

void 
jetinit(pls)
PLStream *pls;
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
    dev->xmax = JETX;
    dev->ymin = 0;
    dev->ymax = JETY;

    setpxl((PLFLT) 5.905, (PLFLT) 5.905);

/* Because portrait mode addressing is used here, we need to complement
   the orientation flag to get the right mapping. */

    if (!!pls->orient)
	setphy(0, JETX, 0, JETY);
    else
	setphy(0, JETY, 0, JETX);

/* Allocate storage for bit map matrix */

#ifdef MSDOS
    if ((bitmap = (char _HUGE *) halloc((long) NBYTES, sizeof(char))) == NULL)
	plexit("Out of memory in call to calloc");
#else
    if ((bitmap = (char *) calloc(NBYTES, sizeof(char))) == NULL)
	plexit("Out of memory in call to calloc");
#endif

/* Reset Printer */

    fprintf(pls->OutFile, "%cE", ESC);
}

/*----------------------------------------------------------------------*\
* jetline()
*
* Draw a line in the current color from (x1,y1) to (x2,y2).
\*----------------------------------------------------------------------*/

void 
jetline(pls, x1a, y1a, x2a, y2a)
PLStream *pls;
PLINT x1a, y1a, x2a, y2a;
{
    int   i, ori;
    int x1=x1a, y1=y1a, x2=x2a, y2=y2a;
    PLINT x1b, y1b, x2b, y2b;
    float length, fx, fy, dx, dy;

/* Because portrait mode addressing is used here, we need to complement
   the orientation flag to get the right mapping. */

    ori = pls->orient; pls->orient = ~pls->orient;
    plRotPhy(pls, dev, &x1, &y1, &x2, &y2);
    pls->orient = ori;

    if (pls->pscale)
	plSclPhy(pls, dev, &x1, &y1, &x2, &y2);

    x1b = x1, x2b = x2, y1b = y1, y2b = y2;
    length = (float) sqrt((double)
		  ((x2b - x1b) * (x2b - x1b) + (y2b - y1b) * (y2b - y1b)));

    if (length == 0.)
	length = 1.;
    dx = (x2 - x1) / length;
    dy = (y2 - y1) / length;

    fx = x1;
    fy = y1;
    setpoint((PLINT) x1, (PLINT) y1);
    setpoint((PLINT) x2, (PLINT) y2);

    for (i = 1; i <= (int) length; i++)
	setpoint((PLINT) (fx += dx), (PLINT) (fy += dy));
}

/*----------------------------------------------------------------------*\
* jetclear()
*
* Clear page (prints it here).
\*----------------------------------------------------------------------*/

void 
jetclear(pls)
PLStream *pls;
{
    PLINT i, j;

    /* First move cursor to origin */
    fprintf(pls->OutFile, "%c*p%dX", ESC, CURX);
    fprintf(pls->OutFile, "%c*p%dY", ESC, CURY);

    /* Then put Laser Printer in 150 dpi mode */
    fprintf(pls->OutFile, "%c*t%dR", ESC, DPI);
    fprintf(pls->OutFile, "%c*r1A", ESC);

    /* Write out raster data */
    for (j = 0; j < YDOTS; j++) {
	fprintf(pls->OutFile, "%c*b%dW", ESC, BPROW);
	for (i = 0; i < BPROW; i++)
	    putc(*(bitmap + i + j * BPROW), pls->OutFile);
    }
    pls->bytecnt += NBYTES;

    /* End raster graphics and send Form Feed */
    fprintf(pls->OutFile, "%c*rB", ESC);
    fprintf(pls->OutFile, "%c", FF);

    /* Finally, clear out bitmap storage area */
    memset(bitmap, '\0', NBYTES);
}

/*----------------------------------------------------------------------*\
* jetpage()
*
* Set up for the next page.  
* Advance to next family file if necessary (file output).
\*----------------------------------------------------------------------*/

void 
jetpage(pls)
PLStream *pls;
{
    if (!pls->termin)
	plGetFam(pls);

    pls->page++;
}

/*----------------------------------------------------------------------*\
* jetadv()
*
* Advance to the next page.
\*----------------------------------------------------------------------*/

void 
jetadv(pls)
PLStream *pls;
{
    jetclear(pls);
    jetpage(pls);
}

/*----------------------------------------------------------------------*\
* jettidy()
*
* Close graphics file or otherwise clean up.
\*----------------------------------------------------------------------*/

void 
jettidy(pls)
PLStream *pls;
{
    jetclear(pls);
    /* Reset Printer */
    fprintf(pls->OutFile, "%cE", ESC);
    fclose(pls->OutFile);
    free((char *) bitmap);
    pls->fileset = 0;
    pls->page = 0;
    pls->OutFile = NULL;
}

/*----------------------------------------------------------------------*\
* jetcolor()
*
* Set pen color.
\*----------------------------------------------------------------------*/

void 
jetcolor(pls)
PLStream *pls;
{
}

/*----------------------------------------------------------------------*\
* jettext()
*
* Switch to text mode.
\*----------------------------------------------------------------------*/

void 
jettext(pls)
PLStream *pls;
{
}

/*----------------------------------------------------------------------*\
* jetgraph()
*
* Switch to graphics mode.
\*----------------------------------------------------------------------*/

void 
jetgraph(pls)
PLStream *pls;
{
}

/*----------------------------------------------------------------------*\
* jetwidth()
*
* Set pen width.
\*----------------------------------------------------------------------*/

void 
jetwidth(pls)
PLStream *pls;
{
}

/*----------------------------------------------------------------------*\
* jetesc()
*
* Escape function.
\*----------------------------------------------------------------------*/

void 
jetesc(pls, op, ptr)
PLStream *pls;
PLINT op;
char *ptr;
{
}

/*----------------------------------------------------------------------*\
* setpoint()
*
* Sets a bit in the bitmap.
\*----------------------------------------------------------------------*/

static void
setpoint(x, y)
PLINT x, y;
{
    PLINT index;
    index = x / 8 + y * BPROW;
    *(bitmap + index) = *(bitmap + index) | mask[x % 8];
}
#endif	/* LJII */
