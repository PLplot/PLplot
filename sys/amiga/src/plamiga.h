/* $Id$
 * $Log$
 * Revision 1.6  1995/06/13 03:58:11  mjl
 * Fixes for 4.99j on the Amiga.
 *
 * Revision 1.5  1994/08/23  16:39:03  mjl
 * Minor fixes to work with PLplot 4.99h distribution and other cleaning up.
 *
 * Revision 1.4  1994/03/23  08:56:55  mjl
 * Added prototype for plamiga_saveiff().
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
    int			locate_mode;	/* Set while in locate (pick) mode */
    PLGraphicsIn	gin;		/* Graphics input structure */

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

int
plamiga_saveiff(char *filename);

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

