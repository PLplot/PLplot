/* $Id$
   $Log$
   Revision 1.2  1993/01/23 06:12:46  mjl
   Preliminary work on new graphical interface (2.04-specific) for the Amiga.

 * Revision 1.1  1992/10/12  17:11:23  mjl
 * Amiga-specific mods, including ANSI-fication.
 *
 * Revision 1.2  1992/09/29  04:45:31  furnish
 * Massive clean up effort to remove support for garbage compilers (K&R).
 *
 * Revision 1.1  1992/05/20  21:33:41  furnish
 * Initial checkin of the whole PLPLOT project.
 *
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
#define a)   a
#else
#define a)   ()
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

/* Function prototypes */

void	OpenPLWind	(void);
void	OpenLibs	(void);
void	ClosePLWind	(void);
void	CloseLibs	(void);
void	GetPLDefs	(void);
void	SetPLDefs	(void);
void	setlimits	(void);
void	plcolreq	(void);
void	RestorePrefs	(void);
void	MakePLMenu	(void);
void	enablemenus	(void);
void	disablemenus	(void);
void	menuselect	(ULONG class, USHORT code);

char *	plfilereq	(void);
void	eventwait	(void);
PLINT	eventhandler	(ULONG class, USHORT code);
PLINT	procmess	(void);
void	remakeplot	(void);
void	PLDraw		(PLINT x, PLINT y);
void	PLMove		(PLINT x, PLINT y);
void	prepupdate	(void);
int	getpoint	(long *com, long *x, long *y);
void	finiupdate	(void);

void	dmpport		(long, int, int);
int	openprinter	(void);
void	closeprinter	(void);
int	queryprint	(long *, long *, long *, long *, long *, long *);
void	ejectpage	(void);

void	screendump	(PLINT type);
void	saveiff		(void);
void	disablegads	(PLINT flag);
void	enablegads	(void);

int	mapinit		(long bmapx, long bmapy);
void	mapclear	(void);
void	mapfree		(void);
void	mapline		(register int x1, register int y1,
			 register int x2, register int y2);
void	iffwritefile	(PLINT xdpi, PLINT ydpi, FILE *File);
