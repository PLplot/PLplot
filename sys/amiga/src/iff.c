/* $Id$
   $Log$
   Revision 1.2  1992/10/12 17:11:22  mjl
   Amiga-specific mods, including ANSI-fication.

 * Revision 1.1  1992/05/20  21:35:25  furnish
 * Initial checkin of the whole PLPLOT project.
 *
*/

/*	iff.c

	PLPLOT Amiga IFF device driver.
	Originally written by Tomas Rokicki (Radical Eye Software).
*/

#include "plplot.h"
#include <stdio.h>
#include "dispatch.h"
#include "plamiga.h"

/* top level declarations */

static PLINT xsubw, ysubw;
static PLINT vxwidth, vywidth;

/* (dev) will get passed in eventually, so this looks weird right now */

static PLDev device;
static PLDev (*dev) = &device;

/*----------------------------------------------------------------------*\
* iffinit()
*
* Initialize device.
\*----------------------------------------------------------------------*/

void 
iffinit(PLStream *pls)
{
    pls->termin = 0;		/* not an interactive terminal */
    pls->color = 1;
    pls->width = 1;
    pls->bytecnt = 0;
    pls->page = 0;

    if (!pls->pageset) {
	pls->xdpi    = plGetFlt("Enter desired horizontal resolution (dpi): ");
	pls->ydpi    = plGetFlt("Enter desired vertical   resolution (dpi): ");
	pls->xlength = plGetInt("Enter desired horizontal size in pixels  : ");
	pls->ylength = plGetInt("Enter desired vertical   size in pixels  : ");
	pls->pageset = 1;
    }

    vxwidth = pls->xlength * 25;
    vywidth = pls->ylength * 25;

    dev->xold = UNDEFINED;
    dev->yold = UNDEFINED;
    dev->xmin = 0;
    dev->xmax = pls->xlength;
    dev->ymin = 0;
    dev->ymax = pls->ylength;

    if (!pls->orient) {
	setpxl((PLFLT) (pls->xdpi * 25. / 25.4), (PLFLT) (pls->ydpi * 25 / 25.4));
	setphy(0, vxwidth, 0, vywidth);
    }
    else {
	setpxl((PLFLT) (pls->ydpi * 25 / 25.4), (PLFLT) (pls->xdpi * 25 / 25.4));
	setphy(0, vywidth, 0, vxwidth);
    }

    xsubw = pls->xlength - 2;
    ysubw = pls->ylength - 2;

    /* Allocate bitmap and initialize for line drawing */
    if (mapinit(pls->xlength, pls->ylength)) {
	plexit("");
    }
}

/*----------------------------------------------------------------------*\
* iffline()
*
* Draw a line in the current color from (x1,y1) to (x2,y2).
\*----------------------------------------------------------------------*/

void 
iffline(PLStream *pls, PLINT x1a, PLINT y1a, PLINT x2a, PLINT y2a)
{
    int x1=x1a, y1=y1a, x2=x2a, y2=y2a;
    long xn1, yn1, xn2, yn2;

    if (!pls->orient) {
	xn1 = (x1 * xsubw) / vxwidth;
	yn1 = (y1 * ysubw) / vywidth;
	xn2 = (x2 * xsubw) / vxwidth;
	yn2 = (y2 * ysubw) / vywidth;
	switch (pls->width) {
	case 3:
	    mapline(xn1, ysubw - yn1, xn2, ysubw - yn2);
	case 2:
	    mapline(xn1 + 2, ysubw - yn1 + 2, xn2 + 2, ysubw - yn2 + 2);
	case 1:
	default:
	    mapline(xn1 + 1, ysubw - yn1 + 1, xn2 + 1, ysubw - yn2 + 1);
	}
    }
    else {
	xn1 = (x1 * ysubw) / vywidth;
	yn1 = (y1 * xsubw) / vxwidth;
	xn2 = (x2 * ysubw) / vywidth;
	yn2 = (y2 * xsubw) / vxwidth;
	switch (pls->width) {
	case 3:
	    mapline(yn1, xn1, yn2, xn2);
	case 2:
	    mapline(yn1 + 2, xn1 + 2, yn2 + 2, xn2 + 2);
	case 1:
	default:
	    mapline(yn1 + 1, xn1 + 1, yn2 + 1, xn2 + 1);
	}
    }
}

/*----------------------------------------------------------------------*\
* iffclear()
*
* Clear page. 
* Here need to close file since only 1 page/file is allowed (for now).
\*----------------------------------------------------------------------*/

void 
iffclear(PLStream *pls)
{
    iffwritefile((PLINT) pls->xdpi, (PLINT) pls->ydpi, pls->OutFile);
    fclose(pls->OutFile);
}

/*----------------------------------------------------------------------*\
* iffpage()
*
* Set up for the next page.  
* Advance to next family file if necessary (file output).
* Here need to open a new file since only 1 page/file is allowed (for now).
\*----------------------------------------------------------------------*/

void 
iffpage(PLStream *pls)
{
    pls->page++;
    mapclear();
    plOpenFile(pls);
}

/*----------------------------------------------------------------------*\
* iffadv()
*
* Advance to the next page.
\*----------------------------------------------------------------------*/

void 
iffadv(PLStream *pls)
{
    iffclear(pls);
    iffpage(pls);
}

/*----------------------------------------------------------------------*\
* ifftidy()
*
* Close graphics file or otherwise clean up.
\*----------------------------------------------------------------------*/

void 
ifftidy(PLStream *pls)
{
    iffclear(pls);
    mapfree();
    pls->fileset = 0;
    pls->page = 0;
    pls->OutFile = NULL;
}

/*----------------------------------------------------------------------*\
* iffcolor()
*
* Set pen color.
\*----------------------------------------------------------------------*/

void 
iffcolor(PLStream *pls)
{
}

/*----------------------------------------------------------------------*\
* ifftext()
*
* Switch to text mode.
\*----------------------------------------------------------------------*/

void 
ifftext(PLStream *pls)
{
}

/*----------------------------------------------------------------------*\
* iffgraph()
*
* Switch to graphics mode.
\*----------------------------------------------------------------------*/

void 
iffgraph(PLStream *pls)
{
}

/*----------------------------------------------------------------------*\
* iffwidth()
*
* Set pen width.
\*----------------------------------------------------------------------*/

void 
iffwidth(PLStream *pls)
{
    if (pls->width < 1)
	pls->width = 1;
    else if (pls->width > 3)
	pls->width = 3;
}

/*----------------------------------------------------------------------*\
* iffesc()
*
* Escape function.
\*----------------------------------------------------------------------*/

void 
iffesc(PLStream *pls, PLINT op, char *ptr)
{
}
