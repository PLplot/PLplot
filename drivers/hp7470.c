/* $Id$
   $Log$
   Revision 1.3  1992/09/30 18:24:51  furnish
   Massive cleanup to irradicate garbage code.  Almost everything is now
   prototyped correctly.  Builds on HPUX, SUNOS (gcc), AIX, and UNICOS.

 * Revision 1.2  1992/09/29  04:44:41  furnish
 * Massive clean up effort to remove support for garbage compilers (K&R).
 *
 * Revision 1.1  1992/05/20  21:32:34  furnish
 * Initial checkin of the whole PLPLOT project.
 *
*/

/*	hp7470.c

	PLPLOT hp7470 device driver.
*/
#ifdef HP7470

#include <stdio.h>
#include "plplot.h"
#include "dispatch.h"

/* top level declarations */

/* Graphics control characters. */

#define ESC          27
#define HP7470X   10299
#define HP7470Y    7649

static char outbuf[128];

/* (dev) will get passed in eventually, so this looks weird right now */

static PLDev device;
static PLDev *dev = &device;

/*----------------------------------------------------------------------*\
* hp7470init()
*
* Initialize device.
\*----------------------------------------------------------------------*/

void 
hp7470init (PLStream *pls)
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
    dev->xmax = HP7470X;
    dev->ymin = 0;
    dev->ymax = HP7470Y;

    setpxl((PLFLT) 40., (PLFLT) 40.);

    if (!pls->orient)
	setphy(0, HP7470X, 0, HP7470Y);
    else
	setphy(0, HP7470Y, 0, HP7470X);

    fprintf(pls->OutFile, "%c.I200;;17:%c.N;19:%c.M;;;10:in;\n", ESC, ESC, ESC);
}

/*----------------------------------------------------------------------*\
* hp7470line()
*
* Draw a line in the current color from (x1,y1) to (x2,y2).
\*----------------------------------------------------------------------*/

void 
hp7470line (PLStream *pls, PLINT x1a, PLINT y1a, PLINT x2a, PLINT y2a)
{
    int x1=x1a, y1=y1a, x2=x2a, y2=y2a;

    plRotPhy(pls, dev, &x1, &y1, &x2, &y2);
    if (pls->pscale)
	plSclPhy(pls, dev, &x1, &y1, &x2, &y2);

    if (x1 == dev->xold && y1 == dev->yold)
	/* Add new point to path */
	sprintf(outbuf, "pd%d %d\n", x2, y2);
    else
	/* Write out old path */
	sprintf(outbuf, "pu%d %d pd%d %d\n", x1, y1, x2, y2);

    fprintf(pls->OutFile, "%s", outbuf);
    pls->bytecnt += strlen(outbuf);

    dev->xold = x2;
    dev->yold = y2;
}

/*----------------------------------------------------------------------*\
* hp7470clear()
*
* Clear page. 
\*----------------------------------------------------------------------*/

void 
hp7470clear (PLStream *pls)
{
}

/*----------------------------------------------------------------------*\
* hp7470page()
*
* Set up for the next page.  
* Advance to next family file if necessary (file output).
\*----------------------------------------------------------------------*/

void 
hp7470page (PLStream *pls)
{
    dev->xold = UNDEFINED;
    dev->yold = UNDEFINED;

    fprintf(pls->OutFile, "pg;\n");
    if (!pls->termin)
	plGetFam(pls);

    pls->page++;
}

/*----------------------------------------------------------------------*\
* hp7470adv()
*
* Advance to the next page.
\*----------------------------------------------------------------------*/

void 
hp7470adv (PLStream *pls)
{
    hp7470clear(pls);
    hp7470page(pls);
}

/*----------------------------------------------------------------------*\
* hp7470tidy()
*
* Close graphics file or otherwise clean up.
\*----------------------------------------------------------------------*/

void 
hp7470tidy (PLStream *pls)
{
    fprintf(pls->OutFile, "sp0\n");
    fclose(pls->OutFile);
    pls->fileset = 0;
    pls->page = 0;
    pls->OutFile = NULL;
}

/*----------------------------------------------------------------------*\
* hp7470color()
*
* Set pen color.
\*----------------------------------------------------------------------*/

void 
hp7470color (PLStream *pls)
{
    if (pls->color < 1 || pls->color > 8)
	fprintf(stderr, "\nInvalid pen selection.");
    else {
	fprintf(pls->OutFile, "sp%d %d\n", pls->color, pls->width);
    }
}

/*----------------------------------------------------------------------*\
* hp7470text()
*
* Switch to text mode.
\*----------------------------------------------------------------------*/

void 
hp7470text (PLStream *pls)
{
}

/*----------------------------------------------------------------------*\
* hp7470graph()
*
* Switch to graphics mode.
\*----------------------------------------------------------------------*/

void 
hp7470graph (PLStream *pls)
{
}

/*----------------------------------------------------------------------*\
* hp7470width()
*
* Set pen width.
\*----------------------------------------------------------------------*/

void 
hp7470width (PLStream *pls)
{
    if (pls->width < 1 || pls->width > 48)
	fprintf(stderr, "\nInvalid pen width selection.");
    else {
	fprintf(pls->OutFile, "sp%d %d\n", pls->color, pls->width);
    }
}

/*----------------------------------------------------------------------*\
* hp7470esc()
*
* Escape function.
\*----------------------------------------------------------------------*/

void 
hp7470esc (PLStream *pls, PLINT op, char *ptr)
{
}

#else
int hp7470() {return 0;}
#endif	/* HP7470 */

