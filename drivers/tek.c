/* $Id$
   $Log$
   Revision 1.4  1992/11/07 07:48:48  mjl
   Fixed orientation operation in several files and standardized certain startup
   operations. Fixed bugs in various drivers.

 * Revision 1.3  1992/10/22  17:04:58  mjl
 * Fixed warnings, errors generated when compling with HP C++.
 *
 * Revision 1.2  1992/09/29  04:44:50  furnish
 * Massive clean up effort to remove support for garbage compilers (K&R).
 *
 * Revision 1.1  1992/05/20  21:32:43  furnish
 * Initial checkin of the whole PLPLOT project.
 *
*/

/*	tek.c

	PLPLOT tektronix device driver.
*/
static int dummy;
#ifdef TEK

#include <stdio.h>
#include "plplot.h"
#include "dispatch.h"

/* Function prototypes */

void tekinit(PLStream *);

/* top level declarations */

#define TEKX   1023
#define TEKY    779

/* Graphics control characters. */

#define FF   12
#define CAN  24
#define ESC  27
#define GS   29
#define US   31

/* (dev) will get passed in eventually, so this looks weird right now */

static PLDev device;
static PLDev *dev = &device;

/*----------------------------------------------------------------------*\
* tektinit()
*
* Initialize device (terminal).
\*----------------------------------------------------------------------*/

void 
tektinit (PLStream *pls)
{
    pls->termin = 1;			/* an interactive device */

    pls->OutFile = stdout;
    pls->orient = 0;
    tekinit(pls);
    fprintf(pls->OutFile, "%c%c", ESC, FF);	/* mgg 07/23/91 via rmr */
}

/*----------------------------------------------------------------------*\
* tekfinit()
*
* Initialize device (file).
\*----------------------------------------------------------------------*/

void 
tekfinit (PLStream *pls)
{
    pls->termin = 0;			/* not an interactive terminal */

/* Initialize family file info */

    plFamInit(pls);

/* Prompt for a file name if not already set */

    plOpenFile(pls);

/* Set up device parameters */

    tekinit(pls);
}

/*----------------------------------------------------------------------*\
* tekinit()
*
* Generic device initialization.
\*----------------------------------------------------------------------*/

void 
tekinit (PLStream *pls)
{
    pls->color = 1;
    pls->width = 1;
    pls->bytecnt = 0;
    pls->page = 0;

    dev->xold = UNDEFINED;
    dev->yold = UNDEFINED;
    dev->xmin = 0;
    dev->ymin = 0;

    if (!pls->orient) {				/* landscape mode */
	dev->xmax = TEKX * 16;
	dev->ymax = TEKY * 16;
	setpxl((PLFLT) (4.771 * 16), (PLFLT) (4.653 * 16));
    }
    else {					/* portrait mode */
	dev->xmax = TEKY * 16;
	dev->ymax = TEKX * 16;
	setpxl((PLFLT) (4.653 * 16), (PLFLT) (4.771 * 16));
    }
    setphy(dev->xmin, dev->xmax, dev->ymin, dev->ymax);

    fprintf(pls->OutFile, "%c", GS);
}

/*----------------------------------------------------------------------*\
* tekline()
*
* Draw a line in the current color from (x1,y1) to (x2,y2).
\*----------------------------------------------------------------------*/

void 
tekline (PLStream *pls, PLINT x1a, PLINT y1a, PLINT x2a, PLINT y2a)
{
    int x1 = x1a, y1 = y1a, x2 = x2a, y2 = y2a;
    int hy, ly, hx, lx;

    plRotPhy(pls, dev, &x1, &y1, &x2, &y2);
    if (pls->pscale)
	plSclPhy(pls, dev, &x1, &y1, &x2, &y2);

    x1 >>= 4;
    y1 >>= 4;
    x2 >>= 4;
    y2 >>= 4;

/* If continuation of previous line just send new point */

    if (x1 == dev->xold && y1 == dev->yold) {
	hy = y2 / 32 + 32;
	ly = y2 - (y2 / 32) * 32 + 96;
	hx = x2 / 32 + 32;
	lx = x2 - (x2 / 32) * 32 + 64;
	fprintf(pls->OutFile, "%c%c%c%c", hy, ly, hx, lx);
	pls->bytecnt += 4;
    }
    else {
	fprintf(pls->OutFile, "%c", GS);
	hy = y1 / 32 + 32;
	ly = y1 - (y1 / 32) * 32 + 96;
	hx = x1 / 32 + 32;
	lx = x1 - (x1 / 32) * 32 + 64;
	fprintf(pls->OutFile, "%c%c%c%c", hy, ly, hx, lx);
	hy = y2 / 32 + 32;
	ly = y2 - (y2 / 32) * 32 + 96;
	hx = x2 / 32 + 32;
	lx = x2 - (x2 / 32) * 32 + 64;
	fprintf(pls->OutFile, "%c%c%c%c", hy, ly, hx, lx);
	pls->bytecnt += 9;
    }
    dev->xold = x2;
    dev->yold = y2;
}

/*----------------------------------------------------------------------*\
* tekclear()
*
* Clear page.  User must hit a <CR> to continue (terminal output).
\*----------------------------------------------------------------------*/

void 
tekclear (PLStream *pls)
{
    if (pls->termin) {
	putchar('\007');
	fflush(stdout);
	while (getchar() != '\n');
    }
    fprintf(pls->OutFile, "%c%c", ESC, FF);
}

/*----------------------------------------------------------------------*\
* tekpage()
*
* Set up for the next page.  
* Advance to next family file if necessary (file output).
\*----------------------------------------------------------------------*/

void 
tekpage (PLStream *pls)
{
    dev->xold = UNDEFINED;
    dev->yold = UNDEFINED;

    if (!pls->termin)
	plGetFam(pls);

    pls->page++;
}

/*----------------------------------------------------------------------*\
* tekadv()
*
* Advance to the next page.
\*----------------------------------------------------------------------*/

void 
tekadv (PLStream *pls)
{
    tekclear(pls);
    tekpage(pls);
}

/*----------------------------------------------------------------------*\
* tektidy()
*
* Close graphics file or otherwise clean up.
\*----------------------------------------------------------------------*/

void 
tektidy (PLStream *pls)
{
    if (!pls->termin) {
	fclose(pls->OutFile);
    }
    else {
	tekclear(pls);
	fprintf(pls->OutFile, "%c%c", US, CAN);
	fflush(pls->OutFile);
    }
    pls->fileset = 0;
    pls->page = 0;
    pls->OutFile = NULL;
}

/*----------------------------------------------------------------------*\
* tekcolor()
*
* Set pen color.
\*----------------------------------------------------------------------*/

void 
tekcolor (PLStream *pls)
{
}

/*----------------------------------------------------------------------*\
* tektext()
*
* Switch to text mode.
\*----------------------------------------------------------------------*/

void 
tektext (PLStream *pls)
{
    fprintf(pls->OutFile, "%c", US);
}

/*----------------------------------------------------------------------*\
* tekgraph()
*
* Switch to graphics mode.
\*----------------------------------------------------------------------*/

void 
tekgraph (PLStream *pls)
{
}

/*----------------------------------------------------------------------*\
* tekwidth()
*
* Set pen width.
\*----------------------------------------------------------------------*/

void 
tekwidth (PLStream *pls)
{
}

/*----------------------------------------------------------------------*\
* tekesc()
*
* Escape function.
\*----------------------------------------------------------------------*/

void 
tekesc (PLStream *pls, PLINT op, char *ptr)
{
}
#endif	/* TEK */
