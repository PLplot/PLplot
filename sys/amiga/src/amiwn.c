/* $Id$
   $Log$
   Revision 1.1  1992/05/20 21:35:23  furnish
   Initial checkin of the whole PLPLOT project.

*/

/*	amiwn.c

	PLPLOT Amiga window device driver.
*/
#include <stdio.h>
#include "plplot.h"
#include "dispatch.h"
#include "plamiga.h"

#ifdef AZTEC_C
#define remove(name)    unlink(name)	/* Psuedo-ANSI compatibility */
struct IntuitionBase *IntuitionBase;	/* Lattice defines these for you */
struct GfxBase *GfxBase;
#endif

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
* amiwninit()
*
* Initialize device.
\*----------------------------------------------------------------------*/

void
amiwninit(pls)
PLStream *pls;
{
    PLFLT Initdpmx, Initdpmy;

    pls->termin = 1;		/* is an interactive terminal */
    pls->color = 1;
    pls->width = 1;
    pls->bytecnt = 0;
    pls->page = 0;

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
* amiwnline()
*
* Draw a line in the current color from (x1,y1) to (x2,y2).
\*----------------------------------------------------------------------*/

void 
amiwnline(pls, x1a, y1a, x2a, y2a)
PLStream *pls;
PLINT x1a, y1a, x2a, y2a;
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
* amiwnclear()
*
* Clear page. 
\*----------------------------------------------------------------------*/

void 
amiwnclear(pls)
PLStream *pls;
{
    void beepw(), setpen();

    beepw();
    setpen(0);
    RectFill(PLWRPort, XOffset, YOffset, PLWidth + XOffset, PLHeight + YOffset);
    setpen(pls->color);
    if (fbuffer) {
	fclose(PlotFile);
	remove(PLOTBFFR);
    }
}

/*----------------------------------------------------------------------*\
* amiwnpage()
*
* Set up for the next page.  
* Advance to next family file if necessary (file output).
\*----------------------------------------------------------------------*/

void 
amiwnpage(pls)
PLStream *pls;
{
    fbuffer = 0;
    if (PLCurPrefs.WinType & PLBUFF) {
	PlotFile = fopen(PLOTBFFR, "w+");
	if (PlotFile == NULL) {
	    fprintf(stderr, "\nError opening plot data storage file.\n");
	    ClosePLWind();
	    CloseLibs();
	    exit(1);
	}
	fbuffer = 1;
    }
    dev->xold = UNDEFINED;
    dev->yold = UNDEFINED;
    pls->page++;
}

/*----------------------------------------------------------------------*\
* amiwnadv()
*
* Advance to the next page.
\*----------------------------------------------------------------------*/

void 
amiwnadv(pls)
PLStream *pls;
{
    amiwnclear(pls);
    amiwnpage(pls);
}

/*----------------------------------------------------------------------*\
* amiwntidy()
*
* Close graphics file or otherwise clean up.
\*----------------------------------------------------------------------*/

void 
amiwntidy(pls)
PLStream *pls;
{
    void beepw();

    beepw();
    ClosePLWind();
    CloseLibs();
    if (fbuffer) {
	fclose(PlotFile);
	remove(PLOTBFFR);
    }
    pls->page = 0;
    pls->OutFile = NULL;
}

/*----------------------------------------------------------------------*\
* amiwncolor()
*
* Set pen color.
\*----------------------------------------------------------------------*/

void 
amiwncolor(pls)
PLStream *pls;
{
    short shcol, comm;
    void setpen();

    if (pls->color >= 0 && pls->color < 16) {
	shcol = pls->color;
	comm = SPEN;
	if (fbuffer) {
	    fwrite((char *) &comm, sizeof(short), 1, PlotFile);
	    fwrite((char *) &shcol, sizeof(short), 1, PlotFile);
	}
	if (pls->color >= MaxColors)
	    pls->color = 1;
	CurColor = pls->color;
	setpen(CurColor);
    }
}

/*----------------------------------------------------------------------*\
* amiwntext()
*
* Switch to text mode.
\*----------------------------------------------------------------------*/

void 
amiwntext(pls)
PLStream *pls;
{
}

/*----------------------------------------------------------------------*\
* amiwngraph()
*
* Switch to graphics mode.
\*----------------------------------------------------------------------*/

void 
amiwngraph(pls)
PLStream *pls;
{
}

/*----------------------------------------------------------------------*\
* amiwnwidth()
*
* Set pen width.
\*----------------------------------------------------------------------*/

void 
amiwnwidth(pls)
PLStream *pls;
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
* amiwnesc()
*
* Escape function.
\*----------------------------------------------------------------------*/

void 
amiwnesc(pls, op, ptr)
PLStream *pls;
PLINT op;
char *ptr;
{
}

/*----------------------------------------------------------------------*\
* Utility functions
\*----------------------------------------------------------------------*/

void 
setlimits()
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
OpenLibs()
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
CloseLibs()
{
    CloseLibrary((struct Library *) GfxBase);
    CloseLibrary((struct Library *) IntuitionBase);
}

void 
PLMove(x, y)
PLINT x, y;
{
    PLINT xsc, ysc;

    xsc = XOffset + x;
    ysc = YOffset + PLHeight - y;
    Move(PLWRPort, xsc, ysc);
}

void 
PLDraw(x, y)
PLINT x, y;
{
    PLINT xsc, ysc;

    xsc = XOffset + x;
    ysc = YOffset + PLHeight - y;
    Draw(PLWRPort, xsc, ysc);
}

void 
remakeplot()
{
    long cxy, x1, y1;
    long x, y;
    void setpen();

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
setpen(color)
PLINT color;
{
    SetAPen(PLWRPort, color);
}

static void 
beepw()
{
    DisplayBeep(PLScreen);
    eventwait();
}
