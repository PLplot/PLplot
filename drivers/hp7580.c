/* $Id$
   $Log$
   Revision 1.2  1992/09/29 04:44:42  furnish
   Massive clean up effort to remove support for garbage compilers (K&R).

 * Revision 1.1  1992/05/20  21:32:35  furnish
 * Initial checkin of the whole PLPLOT project.
 *
*/

/*	hp7580.c

	PLPLOT hp7580 device driver.
*/
#ifdef HP7580

#include <stdio.h>
#include "plplot.h"
#include "dispatch.h"

/* top level declarations */

/* Graphics control characters. */

#define ESC    27
#define HPXMIN  -4500
#define HPXMAX   4500
#define HPYMIN  -2790
#define HPYMAX   2790

static char outbuf[128];

/* (dev) will get passed in eventually, so this looks weird right now */

static PLDev device;
static PLDev *dev = &device;

/*----------------------------------------------------------------------*\
* hp7580init()
*
* Initialize device.
\*----------------------------------------------------------------------*/

void 
hp7580init (PLStream *pls)
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
    dev->xmin = HPXMIN;
    dev->xmax = HPXMAX;
    dev->ymin = HPYMIN;
    dev->ymax = HPYMAX;

    setpxl((PLFLT) 40., (PLFLT) 40.);

    if (!pls->orient)
	setphy(HPXMIN, HPXMAX, HPYMIN, HPYMAX);
    else
	setphy(HPYMIN, HPYMAX, HPXMIN, HPXMAX);

    fprintf(pls->OutFile, "%c.I200;;17:%c.N;19:%c.M;;;10:in;\n", ESC, ESC, ESC);
    fprintf(pls->OutFile, "ro 90;ip;sp 4;pa;");
}

/*----------------------------------------------------------------------*\
* hp7580line()
*
* Draw a line in the current color from (x1,y1) to (x2,y2).
\*----------------------------------------------------------------------*/

void 
hp7580line (PLStream *pls, PLINT x1a, PLINT y1a, PLINT x2a, PLINT y2a)
{
    int x1=x1a, y1=y1a, x2=x2a, y2=y2a;

    plRotPhy(pls, dev, &x1, &y1, &x2, &y2);
    if (pls->pscale)
	plSclPhy(pls, dev, &x1, &y1, &x2, &y2);

    if (x1 == dev->xold && y1 == dev->yold)
	/* Add new point to path */
	sprintf(outbuf, " %d %d", x2, y2);
    else
	/* Write out old path */
	sprintf(outbuf, "\npu%d %d pd%d %d", x1, y1, x2, y2);

    fprintf(pls->OutFile, "%s", outbuf);
    pls->bytecnt += strlen(outbuf);

    dev->xold = x2;
    dev->yold = y2;
}

/*----------------------------------------------------------------------*\
* hp7580clear()
*
* Clear page. 
\*----------------------------------------------------------------------*/

void 
hp7580clear (PLStream *pls)
{
}

/*----------------------------------------------------------------------*\
* hp7580page()
*
* Set up for the next page.  
* Advance to next family file if necessary (file output).
\*----------------------------------------------------------------------*/

void 
hp7580page (PLStream *pls)
{
    dev->xold = UNDEFINED;
    dev->yold = UNDEFINED;

    fprintf(pls->OutFile, "pg;\n");
    if (!pls->termin)
	plGetFam(pls);

    pls->page++;
}

/*----------------------------------------------------------------------*\
* hp7580adv()
*
* Advance to the next page.
\*----------------------------------------------------------------------*/

void 
hp7580adv (PLStream *pls)
{
    hp7580clear(pls);
    hp7580page(pls);
}

/*----------------------------------------------------------------------*\
* hp7580tidy()
*
* Close graphics file or otherwise clean up.
\*----------------------------------------------------------------------*/

void 
hp7580tidy (PLStream *pls)
{
    fprintf(pls->OutFile, "\nsp0");
    fclose(pls->OutFile);
    pls->fileset = 0;
    pls->page = 0;
    pls->OutFile = NULL;
}

/*----------------------------------------------------------------------*\
* hp7580color()
*
* Set pen color.
\*----------------------------------------------------------------------*/

void 
hp7580color (PLStream *pls)
{
    if (pls->color < 1 || pls->color > 8)
	fprintf(stderr, "\nInvalid pen selection.");
    else {
	fprintf(pls->OutFile, "sp%d\n", pls->color);
    }
}

/*----------------------------------------------------------------------*\
* hp7580text()
*
* Switch to text mode.
\*----------------------------------------------------------------------*/

void 
hp7580text (PLStream *pls)
{
}

/*----------------------------------------------------------------------*\
* hp7580graph()
*
* Switch to graphics mode.
\*----------------------------------------------------------------------*/

void 
hp7580graph (PLStream *pls)
{
}

/*----------------------------------------------------------------------*\
* hp7580width()
*
* Set pen width.
\*----------------------------------------------------------------------*/

void 
hp7580width (PLStream *pls)
{
}

/*----------------------------------------------------------------------*\
* hp7580esc()
*
* Escape function.
\*----------------------------------------------------------------------*/

void 
hp7580esc (PLStream *pls, PLINT op, char *ptr)
{
}
#endif
