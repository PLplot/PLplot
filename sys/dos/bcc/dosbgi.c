/* $Id$
   $Log$
   Revision 1.2  1994/05/25 22:08:19  mjl
   Win3 driver added, other changes to bring DOS/BGI (bcc) support
   up to date, contributed by Paul Casteels.

 * Revision 1.8  1993/03/15  21:39:04  mjl
 * Changed all _clear/_page driver functions to the names _eop/_bop, to be
 * more representative of what's actually going on.
 *
 * Revision 1.7  1993/03/03  19:41:54  mjl
 * Changed PLSHORT -> short everywhere; now all device coordinates are expected
 * to fit into a 16 bit address space (reasonable, and good for performance).
 *
 * Revision 1.6  1993/02/27  04:46:31  mjl
 * Fixed errors in ordering of header file inclusion.  "plplot.h" should
 * always be included first.
 *
 * Revision 1.5  1993/02/22  23:10:50  mjl
 * Eliminated the gradv() driver calls, as these were made obsolete by
 * recent changes to plmeta and plrender.  Also eliminated page clear commands
 * from grtidy() -- plend now calls grclr() and grtidy() explicitly.
 *
 * Revision 1.4  1993/01/23  05:41:39  mjl
 * Changes to support new color model, polylines, and event handler support
 * (interactive devices only).
 *
 * Revision 1.3  1992/11/07  07:48:37  mjl
 * Fixed orientation operation in several files and standardized certain startup
 * operations. Fixed bugs in various drivers.
 *
 * Revision 1.2  1992/09/29  04:44:39  furnish
 * Massive clean up effort to remove support for garbage compilers (K&R).
 *
 * Revision 1.1  1992/05/20  21:32:32  furnish
 * Initial checkin of the whole PLPLOT project.
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
#ifdef BGI

#include "plplotP.h"
#include <stdio.h>
#include <stdlib.h>
#include "drivers.h"
#include <graphics.h>
#include <process.h>
#include <conio.h>

static void pause(PLStream *pls);

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
//  return(Mode640x400);
//  return(Mode640x480);
  return(Mode800x600);
//  return(Mode1024x768);
}
#endif

/*----------------------------------------------------------------------*\
* bgi_graph()
*
* Switch to graphics mode.
\*----------------------------------------------------------------------*/

void
bgi_graph(PLStream *pls)
{
  char *bgidir;

	 if (pls->graphx == TEXT_MODE) {
	gdriver = DETECT;
	if ((bgidir = getenv("BGIDIR")) != NULL)
	  initgraph(&gdriver,&gmode,bgidir);
	else
	  initgraph(&gdriver,&gmode,"\\bc4\\bgi");
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
* bgi_init()
*
* Initialize device.
\*----------------------------------------------------------------------*/

void
plD_init_bgi(PLStream *pls)
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

	 bgi_graph(pls);

	 dev->xold = UNDEFINED;
	 dev->yold = UNDEFINED;
	 dev->xmin = 0;
	 dev->xmax = getmaxx();
	 dev->ymin = 0;
	 dev->ymax = getmaxy();

	 plP_setpxl(2.5, 2.5);           /* Pixels/mm. */

	 plP_setphy((PLINT) 0, (PLINT) dev->xmax, (PLINT) 0, (PLINT) dev->ymax);

}

/*----------------------------------------------------------------------*\
* bgi_line()
*
* Draw a line in the current color from (x1,y1) to (x2,y2).
\*----------------------------------------------------------------------*/

void
plD_line_bgi(PLStream *pls, short x1a, short y1a, short x2a, short y2a)
{
	 int x1 = x1a, y1 = y1a, x2 = x2a, y2 = y2a;
/*
	 if (pls->pscale)
	plSclPhy(pls, dev, &x1, &y1, &x2, &y2);
*/
	 y1 = dev->ymax - y1;
	 y2 = dev->ymax - y2;

	 moveto(x1, y1);
	 lineto(x2, y2);

	 page_state = DIRTY;
}

/*----------------------------------------------------------------------*\
* bgi_polyline()
*
* Draw a polyline in the current color.
\*----------------------------------------------------------------------*/

void
plD_polyline_bgi(PLStream *pls, short *xa, short *ya, PLINT npts)
{
	 PLINT i;

	 for (i = 0; i < npts - 1; i++)
	plD_line_bgi(pls, xa[i], ya[i], xa[i + 1], ya[i + 1]);
}

/*----------------------------------------------------------------------*\
* bgi_eop()
*
* End of page.
\*----------------------------------------------------------------------*/

void
plD_eop_bgi(PLStream *pls)
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
* bgi_bop()
*
* Set up for the next page.
* Advance to next family file if necessary (file output).
\*----------------------------------------------------------------------*/

void
plD_bop_bgi(PLStream *pls)
{
	 pls->page++;
	 plD_eop_bgi(pls);
}

/*----------------------------------------------------------------------*\
* bgi_tidy()
*
* Close graphics file or otherwise clean up.
\*----------------------------------------------------------------------*/

void
plD_tidy_bgi(PLStream *pls)
{
/*
	 bgi_text(pls);
*/
	 closegraph();
	 pls->page = 0;
	 pls->OutFile = NULL;
}

/*----------------------------------------------------------------------*\
* bgi_color()
*
* Set pen color.
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
//#endif
*/

void
bgi_color(PLStream *pls)
{
  int i;
	 static int cmap[16] = {
	WHITE, RED, YELLOW, GREEN,
	CYAN, WHITE, WHITE, LIGHTGRAY,
	WHITE, BLUE, GREEN, CYAN,
	RED, MAGENTA, YELLOW, WHITE
	 };

	 if (pls->icol0 < 0 || pls->icol0 > 15)
	pls->icol0 = 15;

/*
  for (i=0;i<=15;i++)
	 setpalette(i,cmap[i]);
*/
//  setpalette(0,WHITE);
  setcolor(pls->icol0);

//    _remappalette((short) pls->icol0, cmap[pls->icol0]);
//    _setcolor((short) pls->icol0);

}

void plD_state_bgi(PLStream *pls,PLINT op) {
//  PSDev *dev = (PSDev *) pls->dev;
  static int cmap[16] = {
	WHITE, RED, YELLOW, GREEN,
	CYAN, WHITE, WHITE, LIGHTGRAY,
	WHITE, BLUE, GREEN, CYAN,
	RED, MAGENTA, YELLOW, WHITE
	 };


  switch(op) {
  case PLSTATE_WIDTH:
	 break;
  case PLSTATE_COLOR0:
	 if (pls->color) {
		setcolor(pls->icol0);
	 }
	 break;
  }
}


/*----------------------------------------------------------------------*\
* bgi_text()
*
* Switch to text mode.
\*----------------------------------------------------------------------*/
/*
void
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
*/

/*----------------------------------------------------------------------*\
* bgi_width()
*
* Set pen width.
\*----------------------------------------------------------------------*/

void
bgi_width(PLStream *pls)
{
}

/*----------------------------------------------------------------------*\
* bgi_esc()
*
* Escape function.
\*----------------------------------------------------------------------*/

void
plD_esc_bgi(PLStream *pls, PLINT op, void *ptr)
{
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

#endif
