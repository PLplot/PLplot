/* $Id$
   $Log$
   Revision 1.4  1993/02/23 05:31:57  mjl
   Eliminated xxx_adv driver function.

 * Revision 1.3  1993/01/23  06:12:43  mjl
 * Preliminary work on new graphical interface (2.04-specific) for the Amiga.
 *
 * Revision 1.2  1992/10/12  17:11:20  mjl
 * Amiga-specific mods, including ANSI-fication.
 *
 * Revision 1.1  1992/05/20  21:35:23  furnish
 * Initial checkin of the whole PLPLOT project.
 *
*/

/*	amiwn.c

	PLPLOT Amiga window device driver.
*/

#include "plplot.h"
#include <stdio.h>
#include "drivers.h"
#include "plamiga.h"

/* Function prototypes */

static void beepw(void);
static void setpen(PLINT);

/* top level declarations */

PLINT XOffset, YOffset, PLWidth, PLHeight;
PLINT MaxPLWidth, MaxPLHeight;
PLINT InitPLWidth, InitPLHeight;
extern PLINT MaxColors;
static PLINT CurColor;

FILE *PlotFile;
short fbuffer;

/* (dev) will get passed in eventually, so this looks weird right now */

static PLDev device;
static PLDev (*dev) = &device;

/*----------------------------------------------------------------------*\
* amiwn_init()
*
* Initialize device.
\*----------------------------------------------------------------------*/

void
amiwn_init(PLStream *pls)
{
    PLFLT Initdpmx, Initdpmy;
    
    pls->termin = 1;		/* is an interactive terminal */
    pls->icol0 = 1;
    pls->width = 1;
    pls->bytecnt = 0;
    pls->page = 0;

    if (!pls->colorset)
        pls->color = 1;

    /* Scan defaults file */

    GetPLDefs();

    /* Open the required libraries. */

    OpenLibs();

    /* Open window for graphics */

    OpenPLWind();

    /* Set up virtual screen width and height */
    /* Virtual screen is 25 times the actual one. */

    MaxPLWidth = PLScreen->Width - PLWindow->BorderLeft - PLWindow->BorderRight - 16;
    MaxPLHeight = PLScreen->Height - PLWindow->BorderTop - PLWindow->BorderBottom - 16;
    InitPLWidth = MaxPLWidth * 25;
    InitPLHeight = MaxPLHeight * 25;
    Initdpmx = GfxBase->NormalDPMX;
    Initdpmy = GfxBase->NormalDPMY;
    if (PLScreen->ViewPort.Modes & HIRES)
	Initdpmx *= 2.;
    if (PLScreen->ViewPort.Modes & LACE)
	Initdpmy *= 2.;

/* Set up device parameters */

    dev->xold = UNDEFINED;
    dev->yold = UNDEFINED;
    dev->xmin = 0;
    dev->xmax = InitPLWidth -1;
    dev->ymin = 0;
    dev->ymax = InitPLHeight -1;

    setpxl((PLFLT) (Initdpmx / 40.), (PLFLT) (Initdpmy / 40.));
    setphy(0, (InitPLWidth - 1), 0, (InitPLHeight - 1));

    SetAPen(PLWRPort, 1);
    SetDrMd(PLWRPort, JAM1);
    setlimits();
}

/*----------------------------------------------------------------------*\
* amiwn_line()
*
* Draw a line in the current color from (x1,y1) to (x2,y2).
\*----------------------------------------------------------------------*/

void 
amiwn_line(PLStream *pls, PLSHORT x1a, PLSHORT y1a, PLSHORT x2a, PLSHORT y2a)
{
    int x1=x1a, y1=y1a, x2=x2a, y2=y2a;
    short comm, xshrt, yshrt;

    /* Check for intuition messages */
    /* Note -- very little overhead in this call */
    procmess();
    if (x1 == dev->xold && y1 == dev->yold) {
	comm = PEND;
	xshrt = x2;
	yshrt = y2;
	if (fbuffer) {
	    (void) fwrite((char *) &comm, sizeof(short), 1, PlotFile);
	    (void) fwrite((char *) &xshrt, sizeof(short), 1, PlotFile);
	    (void) fwrite((char *) &yshrt, sizeof(short), 1, PlotFile);
	}
	PLDraw((PLWidth * x2) / InitPLWidth, (PLHeight * y2) / InitPLHeight);
    }
    else {
	comm = PENU;
	xshrt = x1;
	yshrt = y1;
	if (fbuffer) {
	    (void) fwrite((char *) &comm, sizeof(short), 1, PlotFile);
	    (void) fwrite((char *) &xshrt, sizeof(short), 1, PlotFile);
	    (void) fwrite((char *) &yshrt, sizeof(short), 1, PlotFile);
	}
	comm = PEND;
	xshrt = x2;
	yshrt = y2;
	if (fbuffer) {
	    (void) fwrite((char *) &comm, sizeof(short), 1, PlotFile);
	    (void) fwrite((char *) &xshrt, sizeof(short), 1, PlotFile);
	    (void) fwrite((char *) &yshrt, sizeof(short), 1, PlotFile);
	}
	PLMove((PLWidth * x1) / InitPLWidth, (PLHeight * y1) / InitPLHeight);
	PLDraw((PLWidth * x2) / InitPLWidth, (PLHeight * y2) / InitPLHeight);
    }
    dev->xold = x2;
    dev->yold = y2;
}

/*----------------------------------------------------------------------*\
* amiwn_polyline()
*
* Draw a polyline in the current color.
\*----------------------------------------------------------------------*/

void 
amiwn_polyline (PLStream *pls, PLSHORT *xa, PLSHORT *ya, PLINT npts)
{
    PLINT i;

    for (i=0; i<npts-1; i++) 
      amiwn_line( pls, xa[i], ya[i], xa[i+1], ya[i+1] );
}

/*----------------------------------------------------------------------*\
* amiwn_clear()
*
* Clear page. 
\*----------------------------------------------------------------------*/

void 
amiwn_clear(PLStream *pls)
{
    beepw();
    setpen(0);
    RectFill(PLWRPort, XOffset, YOffset, PLWidth + XOffset, PLHeight + YOffset);
    setpen(pls->icol0);
    if (fbuffer) {
	fclose(PlotFile);
	remove(PLOTBFFR);
    }
}

/*----------------------------------------------------------------------*\
* amiwn_page()
*
* Set up for the next page.  
* Advance to next family file if necessary (file output).
\*----------------------------------------------------------------------*/

void 
amiwn_page(PLStream *pls)
{
    fbuffer = 0;
    if (PLCurPrefs.WinType & PLBUFF) {
	PlotFile = fopen(PLOTBFFR, "w+");
	if (PlotFile == NULL) {
	    plexit("Error opening plot data storage file.");
	}
	fbuffer = 1;
    }
    dev->xold = UNDEFINED;
    dev->yold = UNDEFINED;
    pls->page++;
}

/*----------------------------------------------------------------------*\
* amiwn_tidy()
*
* Close graphics file or otherwise clean up.
\*----------------------------------------------------------------------*/

void 
amiwn_tidy(PLStream *pls)
{
    ClosePLWind();
    CloseLibs();
    pls->page = 0;
    pls->OutFile = NULL;
}

/*----------------------------------------------------------------------*\
* amiwn_color()
*
* Set pen color.
\*----------------------------------------------------------------------*/

void 
amiwn_color(PLStream *pls)
{
    short shcol, comm;

    if (pls->icol0 >= 0 && pls->icol0 < 16) {
	shcol = pls->icol0;
	comm = SPEN;
	if (fbuffer) {
	    fwrite((char *) &comm, sizeof(short), 1, PlotFile);
	    fwrite((char *) &shcol, sizeof(short), 1, PlotFile);
	}
	if (pls->icol0 >= MaxColors)
	    pls->icol0 = 1;
	CurColor = pls->icol0;
	setpen(CurColor);
    }
}

/*----------------------------------------------------------------------*\
* amiwn_text()
*
* Switch to text mode.
\*----------------------------------------------------------------------*/

void 
amiwn_text(PLStream *pls)
{
}

/*----------------------------------------------------------------------*\
* amiwn_graph()
*
* Switch to graphics mode.
\*----------------------------------------------------------------------*/

void 
amiwn_graph(PLStream *pls)
{
}

/*----------------------------------------------------------------------*\
* amiwn_width()
*
* Set pen width.
\*----------------------------------------------------------------------*/

void 
amiwn_width(PLStream *pls)
{
    short shwid, comm;

    if (pls->width >= 1 && pls->width <= 3) {
	comm = PWID;
	shwid = pls->width;
	if (fbuffer) {
	    fwrite((char *) &comm, sizeof(short), 1, PlotFile);
	    fwrite((char *) &shwid, sizeof(short), 1, PlotFile);
	}
    }
}

/*----------------------------------------------------------------------*\
* amiwn_esc()
*
* Escape function.
\*----------------------------------------------------------------------*/

void 
amiwn_esc(PLStream *pls, PLINT op, char *ptr)
{
}

/*----------------------------------------------------------------------*\
* Utility functions
\*----------------------------------------------------------------------*/

void 
setlimits(void)
{
    XOffset = PLWindow->BorderLeft + 8;
    YOffset = PLWindow->BorderTop + 8;
    PLWidth = PLWindow->Width - XOffset - PLWindow->BorderRight - 8;
    PLHeight = PLWindow->Height - YOffset - PLWindow->BorderBottom - 8;
    if (PLCurPrefs.WinType & PLASP) {
	if (PLWidth * MaxPLHeight > PLHeight * MaxPLWidth) {
	    PLWidth = (MaxPLWidth * PLHeight) / MaxPLHeight;
	    XOffset += (PLWindow->Width - PLWidth - PLWindow->BorderLeft -
			PLWindow->BorderRight) / 2;
	}
	else {
	    PLHeight = (MaxPLHeight * PLWidth) / MaxPLWidth;
	    YOffset += (PLWindow->Height - PLHeight - PLWindow->BorderTop -
			PLWindow->BorderBottom) / 2;
	}
    }
}

void 
OpenLibs(void)
{
    IntuitionBase = (struct IntuitionBase *) OpenLibrary("intuition.library", 0L);
    if (IntuitionBase == NULL) {
	puts("\nError opening Intuition library.");
	goto IntError;
    }

    GfxBase = (struct GfxBase *) OpenLibrary("graphics.library", 0L);
    if (GfxBase == NULL) {
	puts("\nError opening Graphics library.");
	goto GfxError;
    }

    return;

  GfxError:
    CloseLibrary((struct Library *) IntuitionBase);
  IntError:
    pl_exit();
}

void 
CloseLibs(void)
{
    CloseLibrary((struct Library *) GfxBase);
    CloseLibrary((struct Library *) IntuitionBase);
}

void 
PLMove(PLINT x, PLINT y)
{
    PLINT xsc, ysc;

    xsc = XOffset + x;
    ysc = YOffset + PLHeight - y;
    Move(PLWRPort, xsc, ysc);
}

void 
PLDraw(PLINT x, PLINT y)
{
    PLINT xsc, ysc;

    xsc = XOffset + x;
    ysc = YOffset + PLHeight - y;
    Draw(PLWRPort, xsc, ysc);
}

void 
remakeplot(void)
{
    long cxy, x1, y1;
    long x, y;

    setpen(0);
    RectFill(PLWRPort, PLWindow->BorderLeft, PLWindow->BorderTop,
	     PLWindow->Width - PLWindow->BorderRight,
	     PLWindow->Height - PLWindow->BorderBottom);

    setpen(1);

    if (fbuffer == 0)
	return;
    prepupdate();

    while (!getpoint(&cxy, &x1, &y1)) {
	if (cxy == PENU) {
	    x = ((long) PLWidth * (long) x1) / InitPLWidth;
	    y = ((long) PLHeight * (long) y1) / InitPLHeight;
	    PLMove((PLINT) x, (PLINT) y);
	}
	else if (cxy == PEND) {
	    x = ((long) PLWidth * (long) x1) / InitPLWidth;
	    y = ((long) PLHeight * (long) y1) / InitPLHeight;
	    PLDraw((PLINT) x, (PLINT) y);
	}
	else if (cxy == SPEN) {
	    CurColor = x1;
	    if (x1 >= MaxColors)
		CurColor = 1;
	    setpen((PLINT) CurColor);
	}
    }

    finiupdate();
    DisplayBeep(PLScreen);
}

static void 
setpen(PLINT color)
{
    SetAPen(PLWRPort, color);
}

static void 
beepw(void)
{
    DisplayBeep(PLScreen);
    eventwait();
}
