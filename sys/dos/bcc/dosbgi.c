/* $Id$
   $Log$
   Revision 1.1  1994/05/25 09:18:40  mjl
   The new locations for DOS BGI (bcc) files.

 * Revision 1.1  1993/10/18  17:07:14  mjl
 * Initial checkin of files for DOS/Borland C.
 *
*/

/*
  dosbgi.c

  Adapted from dosvga.c
  Paul Casteels
  25-Sep-1993

  A driver for Borland's BGI drivers.
  Compile this with Borland C 3.1

*/
#ifdef __BORLANDC__                    /* Only compile for Borland */

#include "plplotP.h"
#include <stdio.h>
#include <stdlib.h>
#include "drivers.h"
#include <graphics.h>
#include <process.h>
#include <conio.h>

static void	pause		(void);
static void	vga_text	(PLStream *);
static void	vga_graph	(PLStream *);

/* A flag to tell us whether we are in text or graphics mode */

#define TEXT_MODE 0
#define GRAPHICS_MODE 1

static int mode = TEXT_MODE;

#define CLEAN 0
#define DIRTY 1

static page_state;

static PLDev device;
static PLDev *dev = &device;

int gdriver,gmode,errorcode;


#ifdef TSENG4
#define Mode640x400     0
#define Mode640x480     1
#define Mode800x600     2
#define Mode1024x768    3

int Tseng4Mode(void)
{
  /* return(Mode640x400);       /**/
  /* return(Mode640x480);       /**/
  return(Mode800x600);  /**/
  /* return(Mode1024x768);      /**/
}
#endif


/*----------------------------------------------------------------------*\
* plD_init_vga()
*
* Initialize device.
\*----------------------------------------------------------------------*/

void
plD_init_vga(PLStream *pls)
{
  int driver;

    pls->termin = 1;            /* is an interactive terminal */
    pls->icol0 = 1;
    pls->width = 1;
    pls->bytecnt = 0;
    pls->page = 0;
    pls->graphx = TEXT_MODE;

    if (!pls->colorset)
	pls->color = 1;

/* Set up device parameters */

#ifdef TSENG4
  driver =installuserdriver ("TSENG4", Tseng4Mode);
  errorcode = graphresult();

  errorcode = graphresult();
  if (errorcode != grOk) {
    printf("Graphics error: %s\n", grapherrormsg(errorcode));
    printf("Press any key to halt:");
    getch();
    exit(1); /* terminate with an error code */
  }
#endif

    plD_graph_vga(pls);

    dev->xold = UNDEFINED;
    dev->yold = UNDEFINED;
    dev->xmin = 0;
    dev->xmax = getmaxx();
    dev->ymin = 0;
    dev->ymax = getmaxy();

    setpxl(2.5, 2.5);           /* Pixels/mm. */

    setphy((PLINT) 0, (PLINT) dev->xmax, (PLINT) 0, (PLINT) dev->ymax);

}

/*----------------------------------------------------------------------*\
* plD_line_vga()
*
* Draw a line in the current color from (x1,y1) to (x2,y2).
\*----------------------------------------------------------------------*/

void
plD_line_vga(PLStream *pls, short x1a, short y1a, short x2a, short y2a)
{
    int x1 = x1a, y1 = y1a, x2 = x2a, y2 = y2a;

    if (pls->pscale)
	plSclPhy(pls, dev, &x1, &y1, &x2, &y2);

    y1 = dev->ymax - y1;
    y2 = dev->ymax - y2;

    moveto(x1, y1);
    lineto(x2, y2);

    page_state = DIRTY;
}

/*----------------------------------------------------------------------*\
* plD_polyline_vga()
*
* Draw a polyline in the current color.
\*----------------------------------------------------------------------*/

void
plD_polyline_vga(PLStream *pls, short *xa, short *ya, PLINT npts)
{
    PLINT i;

    for (i = 0; i < npts - 1; i++)
	plD_line_vga(pls, xa[i], ya[i], xa[i + 1], ya[i + 1]);
}

/*----------------------------------------------------------------------*\
* plD_eop_vga()
*
* End of page.
\*----------------------------------------------------------------------*/

void
plD_eop_vga(PLStream *pls)
{
    if (page_state == DIRTY)
	pause(pls);
    if (pls->graphx == GRAPHICS_MODE) {
      clearviewport();
    }
//    _eopscreen(_GCLEARSCREEN);
    page_state = CLEAN;
}

/*----------------------------------------------------------------------*\
* plD_bop_vga()
*
* Set up for the next page.
* Advance to next family file if necessary (file output).
\*----------------------------------------------------------------------*/

void
plD_bop_vga(PLStream *pls)
{
    pls->page++;
    plD_eop_vga(pls);
}

/*----------------------------------------------------------------------*\
* plD_tidy_vga()
*
* Close graphics file or otherwise clean up.
\*----------------------------------------------------------------------*/

void
plD_tidy_vga(PLStream *pls)
{
    plD_text_vga(pls);
    pls->page = 0;
    pls->OutFile = NULL;
}

/*----------------------------------------------------------------------*\
* plD_state_vga()
*
* Handle change in PLStream state (color, pen width, fill attribute, etc).
\*----------------------------------------------------------------------*/

/*
enum COLORS {
    BLACK,
    BLUE,
    GREEN,
    CYAN,
    RED,
    MAGENTA,
    BROWN,
    LIGHTGRAY,
    DARKGRAY,
    LIGHTBLUE,
    LIGHTGREEN,
    LIGHTCYAN,
    LIGHTRED,
    LIGHTMAGENTA,
    YELLOW,
    WHITE
};
*/

void 
plD_state_vga(PLStream *pls, PLINT op)
{
    switch (op) {

    case PLSTATE_WIDTH:
	break;

    case PLSTATE_COLOR0:{
	static long cmap[16] = {
	    _WHITE, _RED, _LIGHTYELLOW, _GREEN,
	    _CYAN, _WHITE, _WHITE, _GRAY,
	    _WHITE, _BLUE, _GREEN, _CYAN,
	    _RED, _MAGENTA, _LIGHTYELLOW, _WHITE
	    };

	if (pls->icol0 < 0 || pls->icol0 > 15)
	    pls->icol0 = 15;

	setcolor(pls->icol0);
//	_remappalette((short) pls->icol0, cmap[pls->icol0]);
//	_setcolor((short) pls->icol0);
	break;
    }

    case PLSTATE_COLOR1:
	break;
    }
}

/*----------------------------------------------------------------------*\
* plD_esc_vga()
*
* Escape function.
\*----------------------------------------------------------------------*/

void
plD_esc_vga(PLStream *pls, PLINT op, void *ptr)
{
    switch (op) {

      case PLESC_TEXT:
	bgi_text(pls);
	break;

      case PLESC_GRAPH:
	bgi_graph(pls);
	break;
    }
}

/*----------------------------------------------------------------------*\
* bgi_text()
*
* Switch to text mode.
\*----------------------------------------------------------------------*/

static void
bgi_text(PLStream *pls)
{
    if (pls->graphx == GRAPHICS_MODE) {
	if (page_state == DIRTY)
	    pause(pls);
	restorecrtmode();
//      _setvideomode(_DEFAULTMODE);
	pls->graphx = TEXT_MODE;
    }
}

/*----------------------------------------------------------------------*\
* bgi_graph()
*
* Switch to graphics mode.
\*----------------------------------------------------------------------*/

static void
bgi_graph(PLStream *pls)
{
  char *bgidir;

    if (pls->graphx == TEXT_MODE) {
	gdriver = DETECT;
	if ((bgidir = getenv("BGIDIR")) != NULL)
	  initgraph(&gdriver,&gmode,bgidir);
	else
	  initgraph(&gdriver,&gmode,"\\borlandc\\bgi");
	errorcode = graphresult();
	if (errorcode != grOk) {
	    printf("Unable to set graphics mode.");
	    exit(0);
	}
	pls->graphx = GRAPHICS_MODE;
	page_state = CLEAN;
    }
}

/*----------------------------------------------------------------------*\
* pause()
*
* Wait for a keystroke.
\*----------------------------------------------------------------------*/

static void
pause(PLStream *pls)
{
  char *pstr = "pause->";

  if (pls->graphx == TEXT_MODE) {
    gotoxy(0,0);
    cprintf(pstr);
  } else
    outtextxy(0,0,pstr);   // are we in graphics mode ??
    while (!kbhit());
    getch();
}

#else
int
pldummy_dosbgi()
{
    return 0;
}

#endif                          /* __BORLANDC__ */
