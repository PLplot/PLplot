/* $Id$
   $Log$
   Revision 1.5  1993/03/03 19:42:28  mjl
   Changed PLSHORT -> short everywhere; now all device coordinates are expected
   to fit into a 16 bit address space (reasonable, and good for performance).

 * Revision 1.4  1993/02/23  05:31:59  mjl
 * Eliminated xxx_adv driver function.
 *
 * Revision 1.3  1993/01/23  06:12:45  mjl
 * Preliminary work on new graphical interface (2.04-specific) for the Amiga.
 *
 * Revision 1.2  1992/10/12  17:11:22  mjl
 * Amiga-specific mods, including ANSI-fication.
 *
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
#include "drivers.h"
#include "plamiga.h"

/* top level declarations */

static PLINT xsubw, ysubw;
static PLINT vxwidth, vywidth;

/* (dev) will get passed in eventually, so this looks weird right now */

static PLDev device;
static PLDev (*dev) = &device;

/*----------------------------------------------------------------------*\
* iff_init()
*
* Initialize device.
\*----------------------------------------------------------------------*/

void 
iff_init(PLStream *pls)
{
    pls->termin = 0;		/* not an interactive terminal */
    pls->icol0 = 1;
    pls->width = 1;
    pls->bytecnt = 0;
    pls->page = 0;

    if (!pls->colorset)
        pls->color = 1;

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
* iff_line()
*
* Draw a line in the current color from (x1,y1) to (x2,y2).
\*----------------------------------------------------------------------*/

void 
iff_line(PLStream *pls, short x1a, short y1a, short x2a, short y2a)
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
* iff_polyline()
*
* Draw a polyline in the current color.
\*----------------------------------------------------------------------*/

void 
iff_polyline (PLStream *pls, short *xa, short *ya, PLINT npts)
{
    PLINT i;

    for (i=0; i<npts-1; i++) 
      iff_line( pls, xa[i], ya[i], xa[i+1], ya[i+1] );
}

/*----------------------------------------------------------------------*\
* iff_clear()
*
* Clear page. 
* Here need to close file since only 1 page/file is allowed (for now).
\*----------------------------------------------------------------------*/

void 
iff_clear(PLStream *pls)
{
    iffwritefile((PLINT) pls->xdpi, (PLINT) pls->ydpi, pls->OutFile);
    fclose(pls->OutFile);
}

/*----------------------------------------------------------------------*\
* iff_page()
*
* Set up for the next page.  
* Advance to next family file if necessary (file output).
* Here need to open a new file since only 1 page/file is allowed (for now).
\*----------------------------------------------------------------------*/

void 
iff_page(PLStream *pls)
{
    pls->page++;
    mapclear();
    plOpenFile(pls);
}

/*----------------------------------------------------------------------*\
* iff_tidy()
*
* Close graphics file or otherwise clean up.
\*----------------------------------------------------------------------*/

void 
iff_tidy(PLStream *pls)
{
    mapfree();
    pls->fileset = 0;
    pls->page = 0;
    pls->OutFile = NULL;
}

/*----------------------------------------------------------------------*\
* iff_color()
*
* Set pen color.
\*----------------------------------------------------------------------*/

void 
iff_color(PLStream *pls)
{
}

/*----------------------------------------------------------------------*\
* iff_text()
*
* Switch to text mode.
\*----------------------------------------------------------------------*/

void 
iff_text(PLStream *pls)
{
}

/*----------------------------------------------------------------------*\
* iff_graph()
*
* Switch to graphics mode.
\*----------------------------------------------------------------------*/

void 
iff_graph(PLStream *pls)
{
}

/*----------------------------------------------------------------------*\
* iff_width()
*
* Set pen width.
\*----------------------------------------------------------------------*/

void 
iff_width(PLStream *pls)
{
    if (pls->width < 1)
	pls->width = 1;
    else if (pls->width > 3)
	pls->width = 3;
}

/*----------------------------------------------------------------------*\
* iff_esc()
*
* Escape function.
\*----------------------------------------------------------------------*/

void 
iff_esc(PLStream *pls, PLINT op, char *ptr)
{
}
