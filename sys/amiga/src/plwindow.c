/* $Id$
   $Log$
   Revision 1.1  1992/05/20 21:35:30  furnish
   Initial checkin of the whole PLPLOT project.

*/

#include "plplot.h"
#include "plamiga.h"
#include <stdio.h>
#include <math.h>

extern struct PLPrefs PLCurPrefs;

PLINT MaxColors;

extern PLFLT InitWSize, InitHSize;

struct Screen *PLScreen;	/* For Custom Screen */
struct Window *PLWindow;
struct RastPort *PLSRPort;	/* Screen rastport */
struct RastPort *PLWRPort;	/* Window rastport */
struct ViewPort *PLVPort;
struct ColorMap *PLCMap;
static struct TmpRas PLTmpRas;
static PLANEPTR PLTmpPlane;

struct NewWindow NewWindow =
{
    0, 0,
    0, 0,
    0, 1,
    0,
    0,
    NULL,
    NULL,
    NULL,
    NULL,			/* Screen pointer */
    NULL,
    205, 120, 1000, 1000,
    WBENCHSCREEN
};

void 
OpenPLWind()
{
    short i;

    if (PLCurPrefs.WinType & PLCUST) {
	struct NewScreen NewScreen =
	{
	    0, 0, 0, 0, 1,
	    0, 1,
	    HIRES,		/* default viewmode */
	    CUSTOMSCREEN,
	    NULL,
	    "Plplot Screen",
	    NULL,
	    NULL
	};

	NewScreen.Height = GfxBase->NormalDisplayRows;
	NewScreen.Width = GfxBase->NormalDisplayColumns;

	if (PLCurPrefs.ScrType & PLLACE) {
	    NewScreen.ViewModes |= INTERLACE;
	    NewScreen.Height *= 2;
	}
	if (!(PLCurPrefs.ScrType & PLHIRES)) {
	    NewScreen.ViewModes &= ~HIRES;
	    NewScreen.Width /= 2;
	}

	NewScreen.Depth = PLCurPrefs.Depth;
	NewWindow.Type = CUSTOMSCREEN;
	if ((PLScreen = OpenScreen(&NewScreen)) == NULL) {
	    fprintf(stderr, "Not enough memory for custom screen.\n");
	    CloseLibs();
	    pl_exit();
	}
	for (i = 0, MaxColors = 1; i < PLCurPrefs.Depth; i++)
	    MaxColors *= 2;
	PLVPort = &(PLScreen->ViewPort);
	LoadRGB4(PLVPort, &(PLCurPrefs.Color[0]), MaxColors);
	NewWindow.Screen = PLScreen;
	if (PLCurPrefs.CWidth == 0 || PLCurPrefs.CHeight == 0) {
	    NewWindow.Width = PLScreen->Width;
	    NewWindow.Height = PLScreen->Height - PLScreen->BarHeight - 1;
	    NewWindow.LeftEdge = 0;
	    NewWindow.TopEdge = PLScreen->BarHeight + 1;
	    PLCurPrefs.CXPos = NewWindow.Width;
	    PLCurPrefs.CYPos = NewWindow.Height;
	    PLCurPrefs.CWidth = NewWindow.Width;
	    PLCurPrefs.CHeight = NewWindow.Height;
	}
	else {
	    NewWindow.LeftEdge = PLCurPrefs.CXPos;
	    NewWindow.TopEdge = PLCurPrefs.CYPos;
	    NewWindow.Width = PLCurPrefs.CWidth;
	    NewWindow.Height = PLCurPrefs.CHeight;
	}
    }
    else {
	MaxColors = 4;
	NewWindow.LeftEdge = PLCurPrefs.WXPos;
	NewWindow.TopEdge = PLCurPrefs.WYPos;
	NewWindow.Width = PLCurPrefs.WWidth;
	NewWindow.Height = PLCurPrefs.WHeight;
	NewWindow.Type = WBENCHSCREEN;
    }

    NewWindow.IDCMPFlags = NEWSIZE | MENUPICK | CLOSEWINDOW;
    NewWindow.Flags = WINDOWDRAG | WINDOWDEPTH | WINDOWCLOSE | WINDOWSIZING |
	SMART_REFRESH | ACTIVATE;
    NewWindow.Title = "Plplot 4.0  Amiga window driver";

    if ((PLWindow = OpenWindow(&NewWindow)) == NULL) {
	fprintf(stderr, "Error opening window.\n");
	if (PLCurPrefs.WinType & PLCUST)
	    CloseScreen(PLScreen);
	CloseLibs();
    }

    if (!(PLCurPrefs.WinType & PLCUST))
	PLScreen = PLWindow->WScreen;

    PLSRPort = &(PLScreen->RastPort);
    PLWRPort = PLWindow->RPort;
    PLVPort = &(PLScreen->ViewPort);
    PLCMap = PLVPort->ColorMap;
    PLTmpPlane = AllocRaster(PLScreen->Width, PLScreen->Height);
    if (PLTmpPlane == NULL) {
	CloseWindow(PLWindow);
	if (PLCurPrefs.WinType & PLCUST)
	    CloseScreen(PLScreen);
	fprintf(stderr, "Out of memory!");
    }
    InitTmpRas(&PLTmpRas, PLTmpPlane, RASSIZE(PLScreen->Width, PLScreen->Height));
    PLWRPort->TmpRas = &PLTmpRas;

    MakePLMenu();
}

void 
ClosePLWind()
{
    FreeRaster(PLTmpPlane, PLScreen->Width, PLScreen->Height);
    ClearMenuStrip(PLWindow);
    CloseWindow(PLWindow);
    if ((PLScreen->Flags & SCREENTYPE) == CUSTOMSCREEN)
	CloseScreen(PLScreen);
}
