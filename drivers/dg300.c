/* $Id$
   $Log$
   Revision 1.1  1992/05/20 21:32:30  furnish
   Initial checkin of the whole PLPLOT project.

*/

/*	dg300.c

	PLPLOT dg300 device driver.
*/
#ifdef DG300

#include <stdio.h>
#include "plplot.h"
#include "dispatch.h"

/* top level declarations */

#define  DGX    639
#define  DGY    239

struct termattr {
    unsigned char com[4];
    unsigned char rom[4];
    unsigned char ram[4];
    unsigned char con[5];
    unsigned char eor;
} termattr;

/* (dev) will get passed in eventually, so this looks weird right now */

static PLDev device;
static PLDev *dev = &device;

/*----------------------------------------------------------------------*\
* dginit()
*
* Initialize device.
\*----------------------------------------------------------------------*/

void 
dginit(pls)
PLStream *pls;
{
    /* Request terminal configuration report */
    printf("\n\036\107\051\n");
    scanf("%s", (char *) &termattr);
    while (getchar() != '\n');
    if (!strncmp((char *) &termattr.ram[0], "0000", 4)) {
	printf("Please wait while graphics interpreter is downloaded.\n");
	/* Need to download graphics interpreter. */
	system("cat  /usr/local/src/g300/g300gci110.tx");
    }

    /* Clear screen, Set pen color to green, Absolute positioning */
    printf("\036\107\063\060\n\036\107\155\061\n\036\107\151\060\n");
    printf("\036\107\042\061\n");

    pls->termin = 1;	/* is an interactive device */
    pls->color = 1;
    pls->width = 1;
    pls->bytecnt = 0;
    pls->page = 0;

    dev->xold = UNDEFINED;
    dev->yold = UNDEFINED;
    dev->xmin = 0;
    dev->xmax = DGX;
    dev->ymin = 0;
    dev->ymax = DGY;

    setpxl((PLFLT) (3.316 * 16), (PLFLT) (1.655 * 16));
    setphy(0, DGX * 16, 0, DGY * 16);
}

/*----------------------------------------------------------------------*\
* dgline()
*
* Draw a line in the current color from (x1,y1) to (x2,y2).
\*----------------------------------------------------------------------*/

void 
dgline(pls, x1a, y1a, x2a, y2a)
PLStream *pls;
PLINT x1a, y1a, x2a, y2a;
{
    int x1=x1a, y1=y1a, x2=x2a, y2=y2a;

    if (pls->pscale)
	plSclPhy(pls, dev, &x1, &y1, &x2, &y2);

    printf("LINE %d %d %d %d\n", x1 >> 4, y1 >> 3, x2 >> 4, y2 >> 3);
}

/*----------------------------------------------------------------------*\
* dgclear()
*
* Clear page.  User must hit a <CR> to continue.
\*----------------------------------------------------------------------*/

void 
dgclear(pls)
PLStream *pls;
{
    /* Before clearing wait for CR */
    putchar('\007');
    fflush(stdout);
    while (getchar() != '\n');
    printf("ERASE\n");
}

/*----------------------------------------------------------------------*\
* dgpage()
*
* Set up for the next page.
\*----------------------------------------------------------------------*/

void 
dgpage(pls)
PLStream *pls;
{
    pls->page++;
}

/*----------------------------------------------------------------------*\
* dgadv()
*
* Advance to the next page.
\*----------------------------------------------------------------------*/

void 
dgadv(pls)
PLStream *pls;
{
    dgclear(pls);
    dgpage(pls);
}

/*----------------------------------------------------------------------*\
* dgtidy()
*
* Close graphics file
\*----------------------------------------------------------------------*/

void 
dgtidy(pls)
PLStream *pls;
{
    dgclear(pls);
    printf("\036\107\042\060\n");
    fflush(stdout);
    pls->fileset = 0;
    pls->page = 0;
    pls->OutFile = NULL;
}

/*----------------------------------------------------------------------*\
* dgcolor()
*
* Set pen color.
\*----------------------------------------------------------------------*/

void 
dgcolor(pls)
PLStream *pls;
{
}

/*----------------------------------------------------------------------*\
* dgtext()
*
* Switch to text mode.
\*----------------------------------------------------------------------*/

void 
dgtext(pls)
PLStream *pls;
{
}

/*----------------------------------------------------------------------*\
* dggraph()
*
* Switch to graphics mode.
\*----------------------------------------------------------------------*/

void 
dggraph(pls)
PLStream *pls;
{
}

/*----------------------------------------------------------------------*\
* dgwidth()
*
* Set pen width.
\*----------------------------------------------------------------------*/

void
dgwidth (pls)
PLStream *pls;
{
}

/*----------------------------------------------------------------------*\
* dgesc()
*
* Escape function.
\*----------------------------------------------------------------------*/

void 
dgesc(pls, op, ptr)
PLStream *pls;
PLINT op;
char *ptr;
{
}
#endif	/* DG300 */
