/* $Id$
   $Log$
   Revision 1.3  1993/03/15 21:34:28  mjl
   Reorganization and update of Amiga drivers.  Window driver now uses Amiga
   OS 2.0 capabilities.

 * Revision 1.2  1993/01/23  06:12:46  mjl
 * Preliminary work on new graphical interface (2.04-specific) for the Amiga.
 *
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

/* These are needed by GadTools-generated display routines */

#include <exec/types.h>
#include <intuition/intuition.h>
#include <intuition/classes.h>
#include <intuition/classusr.h>
#include <intuition/imageclass.h>
#include <intuition/gadgetclass.h>
#include <libraries/gadtools.h>
#include <graphics/displayinfo.h>
#include <graphics/gfxbase.h>
#include <clib/exec_protos.h>
#include <clib/intuition_protos.h>
#include <clib/gadtools_protos.h>
#include <clib/graphics_protos.h>
#include <clib/utility_protos.h>
#include <string.h>

/* Everything else */
/* Includes header files for using reqtools.library and iff.library */

#include <libraries/reqtools.h>
#include <libraries/iff.h>
#include <proto/dos.h>
#include <proto/reqtools.h>

extern struct IntuiMessage   PlplotMsg;

/*
* This structure holds just about everything needed to describe the drawing
* area, including window, screen, geometry, etc.  
*/

typedef struct {
    int			exit_eventloop;
    int			restart;

    long		init_width;
    long		init_height;
    long		cur_width;
    long		cur_height;
    long		xoffset;
    long		yoffset;

    double		xscale;
    double		yscale;
    double		xscale_dev;
    double		yscale_dev;

    struct Screen	*screen;
    struct Window	*window;
    struct Menu         *menus;
    APTR		visual;

    struct TextAttr	*font;

    WORD		scr_left;
    WORD		scr_top;
    WORD		scr_width;
    WORD		scr_height;
    UWORD		scr_depth;
    UWORD		scr_type;
    long		scr_displayID;
    long		maxcolors;

    struct RastPort 	*SRPort;	/* Screen rastport */
    struct RastPort 	*WRPort;	/* Window rastport */
    struct ViewPort 	*VPort;
    struct ColorMap 	*CMap;

} PlAmigaWin;

extern PlAmigaWin *pla;

extern struct IntuitionBase *IntuitionBase;
extern struct GfxBase *GfxBase;
extern struct ReqToolsBase *ReqToolsBase;
extern struct Library *IFFBase;

/* Function prototypes */

int  HandlePlplotIDCMP	(void);

void  pla_InitDisplay	(void);
int   pla_OpenScreen	(void);
void  pla_CloseScreen	(void);
int   pla_OpenWindow	(void);
void  pla_CloseWindow	(void);
void  pla_SetFont	(void);

void  pla_OpenLibs	(void);
void  pla_CloseLibs	(void);

void  dmpport		(long, int, int);
int   openprinter	(void);
void  closeprinter	(void);
int   queryprint	(long *, long *, long *, long *, long *, long *);
void  ejectpage		(void);
void  screendump	(PLINT type);

int   mapinit		(long bmapx, long bmapy);
void  mapclear		(void);
void  mapfree		(void);
void  mapline		(register int x1, register int y1,
			 register int x2, register int y2);
void  iffwritefile	(PLINT xdpi, PLINT ydpi, FILE *File);

