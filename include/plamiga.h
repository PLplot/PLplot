/* $Id$
   $Log$
   Revision 1.1  1992/05/20 21:33:41  furnish
   Initial checkin of the whole PLPLOT project.

*/

#include <exec/types.h>
#include <exec/ports.h>
#include <exec/memory.h>
#include <devices/printer.h>
#include <devices/prtbase.h>
#include <graphics/display.h>
#include <graphics/gfxbase.h>
#include <graphics/view.h>
#include <graphics/gfxmacros.h>
#include <intuition/intuition.h>
#include <intuition/intuitionbase.h>
#include <intuition/screens.h>

#ifdef LATTICE_50
#include <proto/exec.h>
#include <proto/graphics.h>
#include <proto/intuition.h>
#include <proto/dos.h>
#endif

#ifdef AZTEC_C
#include <functions.h>
#endif

/* Graphics data buffer file. */
#define PLOTBFFR      "t:plplot.plt"

#ifndef PLARGS
#ifdef LATTICE_50
#define PLARGS(a)   a
#else
#define PLARGS(a)   ()
#endif
#endif


/* Flags for variables in PLPrefs structure. */
/* WinType defines */
#define PLCUST      01		/* Open on custom screen */
#define PLASP       02		/* Maintain initial aspect */
#define PLBUFF      04		/* Buffer plot in t:plplot.plt */

/* ScrType defines */
#define PLLACE      01
#define PLHIRES     02

struct PLPrefs {
    USHORT WinType;
    USHORT ScrType;
    USHORT WXPos, WYPos;	/* Position of Window (WorkBench) */
    USHORT WWidth, WHeight;	/* Window width, height (WorkBench) */
    USHORT CXPos, CYPos;	/* Position of Window (Custom) */
    USHORT CWidth, CHeight;	/* Window width, height (Custom) */
    USHORT Depth;		/* Screen Depth (1, 2, 3, 4) (Custom) */
    USHORT Color[16];		/* Color RGB values (Custom Only) */
};

/* Define graphics operation types */
#define PENU  10
#define PEND  20
#define SPEN  30
#define PWID  40

extern struct IntuitionBase *IntuitionBase;
extern struct GfxBase *GfxBase;

extern struct Screen *PLScreen;
extern struct Window *PLWindow;
extern struct RastPort *PLSRPort;	/* Screen rastport */
extern struct RastPort *PLWRPort;	/* Window rastport */
extern struct ViewPort *PLVPort;
extern struct ColorMap *PLCMap;

extern struct PLPrefs PLCurPrefs;
extern PLINT XOffset, YOffset, PLWidth, PLHeight;
extern PLINT InitPLWidth, InitPLHeight;

void OpenPLWind PLARGS((void));
void OpenLibs PLARGS((void));
void ClosePLWind PLARGS((void));
void CloseLibs PLARGS((void));
void GetPLDefs PLARGS((void));
void SetPLDefs PLARGS((void));
void setlimits PLARGS((void));
void plcolreq PLARGS((void));
void RestorePrefs PLARGS((void));
void setpen PLARGS((PLINT color));
void MakePLMenu PLARGS((void));
void enablemenus PLARGS((void));
void disablemenus PLARGS((void));
void menuselect PLARGS((ULONG class, USHORT code));
void eventwait PLARGS((void));
PLINT eventhandler PLARGS((ULONG class, USHORT code));
PLINT procmess PLARGS((void));
void remakeplot PLARGS((void));
void PLDraw PLARGS((PLINT x, PLINT y));
void PLMove PLARGS((PLINT x, PLINT y));
void prepupdate PLARGS((void));
int getpoint PLARGS((long *com, long *x, long *y));
void finiupdate PLARGS((void));

void screendump PLARGS((PLINT type));
void saveiff PLARGS((void));
void disablegads PLARGS((PLINT flag));
void enablegads PLARGS((void));
