/* $Id$
   $Log$
   Revision 1.16  1993/09/14 22:25:15  mjl
   Moved define of POSIX_TTY to plplotP.h since the SX-3 isn't POSIX-compliant.

 * Revision 1.15  1993/08/03  01:46:42  mjl
 * Changes to eliminate warnings when compiling with gcc -Wall.
 *
 * Revision 1.14  1993/07/31  07:56:44  mjl
 * Several driver functions consolidated, for all drivers.  The width and color
 * commands are now part of a more general "state" command.  The text and
 * graph commands used for switching between modes is now handled by the
 * escape function (very few drivers require it).  The device-specific PLDev
 * structure is now malloc'ed for each driver that requires it, and freed when
 * the stream is terminated.
 *
 * Revision 1.13  1993/07/28  05:38:59  mjl
 * Merged with xterm code, and added code to handle Tektronix 4105/4107
 * (color) devices.  For now just the color index selection is supported.
 * Added code to change terminal line to noncanonical (cbreak) mode on
 * POSIX systems so that entered keystrokes (including <Backspace> and "Q"
 * from plrender) do not require a <CR> to be recognized.
 *
 * Revision 1.12  1993/07/16  22:11:22  mjl
 * Eliminated low-level coordinate scaling; now done by driver interface.
 *
 * Revision 1.11  1993/07/01  21:59:44  mjl
 * Changed all plplot source files to include plplotP.h (private) rather than
 * plplot.h.  Rationalized namespace -- all externally-visible plplot functions
 * now start with "pl"; device driver functions start with "plD_".
*/

/*	tek.c

	PLPLOT tektronix device driver.
	Includes support for xterm, tektronix 4010 terminal & file,
	tektronix 4107 terminal & file.
*/
#if defined(XTERM) || defined(TEK4010) || defined(TEK4107)

#include "plplotP.h"
#include <stdio.h>
#include <ctype.h>
#include "drivers.h"
#include "plevent.h"

/* Static function prototypes */
/* INDENT OFF */

static void	WaitForPage	(PLStream *);
static void	EventHandler	(PLStream *, int);
static void	tek_init	(PLStream *);
static void	tek_text	(PLStream *);
static void	tek_graph	(PLStream *);

/* Stuff for handling tty cbreak mode */

#ifdef POSIX_TTY

#include <termios.h>
#include <unistd.h>
#include <stdlib.h>

static struct termios	save_termios;
static int		ttysavefd = -1;
static enum { RESET, RAW, CBREAK } ttystate = RESET;

int  tty_cbreak		(int);
int  tty_reset		(int);
void tty_atexit		(void);

#endif

/* Pixel settings */

#define TEKX   1023
#define TEKY    779

/* Graphics control characters. */

#define ETX  3
#define BEL  7
#define FF   12
#define CAN  24
#define ESC  27
#define GS   29
#define US   31

/* Static vars */

static exit_eventloop = 0;

/* INDENT ON */
/*----------------------------------------------------------------------*\
* plD_init_xte()	xterm 
* plD_init_tekt()	Tek 4010 terminal
* plD_init_tekf()	Tek 4010 file
* plD_init_t4107t()	Tek 4105/4107 terminal
* plD_init_t4107f()	Tek 4105/4107 file
*
* Initialize device.  These just set attributes for the particular tektronix
* device, then call tek_init().  The following attributes can be set:
*
* pls->termin		if a terminal device
* pls->color		if color
* pls->dev_dual		if device has separate text and graphics screens
*
* It is possible to modify these from the user code directly by using
* plgpls() to get the stream pointer, then setting the appropriate
* attribute.  For example, I use a tek4107 emulator that has dual text
* and graphics screens, so I set the dev_dual attribute.  If you have
* a real tek4107 you may want to unset this.
\*----------------------------------------------------------------------*/

void 
plD_init_xte (PLStream *pls)
{
    pls->termin = 1;
    pls->dev_dual = 1;
    tek_init(pls);
}

void
plD_init_tekt(PLStream *pls)
{
    pls->termin = 1;
    tek_init(pls);
}

void
plD_init_tekf(PLStream *pls)
{
    tek_init(pls);
}

void
plD_init_t4107t(PLStream *pls)
{
    pls->termin = 1;
    pls->dev_dual = 1;
    pls->color = 1;
    tek_init(pls);
}

void
plD_init_t4107f(PLStream *pls)
{
    pls->color = 1;
    tek_init(pls);
}

/*----------------------------------------------------------------------*\
* tek_init()
*
* Generic tektronix device initialization.
\*----------------------------------------------------------------------*/

static void
tek_init(PLStream *pls)
{
    PLDev *dev;

    pls->icol0 = 1;
    pls->width = 1;

/* Allocate and initialize device-specific data */

    dev = plAllocDev(pls);

    dev->xold = UNDEFINED;
    dev->yold = UNDEFINED;
    dev->xmin = 0;
    dev->ymin = 0;
    dev->xmax = TEKX * 16;
    dev->ymax = TEKY * 16;
    dev->pxlx = 4.771 * 16;
    dev->pxly = 4.653 * 16;

    plP_setpxl(dev->pxlx, dev->pxly);
    plP_setphy(dev->xmin, dev->xmax, dev->ymin, dev->ymax);

/* Terminal/file initialization */

    if (pls->termin) 
	pls->OutFile = stdout;
    else {
	plFamInit(pls);
	plOpenFile(pls);
    }

#ifdef POSIX_TTY
    if (pls->termin) {
	if (tty_cbreak(STDIN_FILENO))
	    fprintf(stderr, "Unable to set up cbreak mode.\n");
	else {
	    if (atexit(tty_atexit))
		fprintf(stderr, "Unable to set up atexit handler.\n");
	}
    }
#endif

    pls->graphx = GRAPHICS_MODE;
    if (pls->dev_dual)
	fprintf(pls->OutFile, "%c[?38h", ESC);	/* open graphics window */

    fprintf(pls->OutFile, "%c", GS);		/* set to vector mode */
    if (pls->termin)
	fprintf(pls->OutFile, "%c%c", ESC, FF);	/* erase and home */
}

/*----------------------------------------------------------------------*\
* plD_line_tek()
*
* Draw a line from (x1,y1) to (x2,y2).
\*----------------------------------------------------------------------*/

void
plD_line_tek(PLStream *pls, short x1a, short y1a, short x2a, short y2a)
{
    PLDev *dev = (PLDev *) pls->dev;
    int x1 = x1a, y1 = y1a, x2 = x2a, y2 = y2a;
    int hy, ly, hx, lx;

    x1 >>= 4;
    y1 >>= 4;
    x2 >>= 4;
    y2 >>= 4;

/* If continuation of previous line just send new point */

    if (x1 == dev->xold && y1 == dev->yold) {
	hy = y2 / 32 + 32;
	ly = y2 - (y2 / 32) * 32 + 96;
	hx = x2 / 32 + 32;
	lx = x2 - (x2 / 32) * 32 + 64;
	fprintf(pls->OutFile, "%c%c%c%c", hy, ly, hx, lx);
	pls->bytecnt += 4;
    }
    else {
	fprintf(pls->OutFile, "%c", GS);
	hy = y1 / 32 + 32;
	ly = y1 - (y1 / 32) * 32 + 96;
	hx = x1 / 32 + 32;
	lx = x1 - (x1 / 32) * 32 + 64;
	fprintf(pls->OutFile, "%c%c%c%c", hy, ly, hx, lx);
	hy = y2 / 32 + 32;
	ly = y2 - (y2 / 32) * 32 + 96;
	hx = x2 / 32 + 32;
	lx = x2 - (x2 / 32) * 32 + 64;
	fprintf(pls->OutFile, "%c%c%c%c", hy, ly, hx, lx);
	pls->bytecnt += 9;
    }
    dev->xold = x2;
    dev->yold = y2;
}

/*----------------------------------------------------------------------*\
* plD_polyline_tek()
*
* Draw a polyline in the current color.
\*----------------------------------------------------------------------*/

void
plD_polyline_tek(PLStream *pls, short *xa, short *ya, PLINT npts)
{
    PLINT i;

    for (i = 0; i < npts - 1; i++)
	plD_line_tek(pls, xa[i], ya[i], xa[i + 1], ya[i + 1]);
}

/*----------------------------------------------------------------------*\
* plD_eop_tek()
*
* End of page.  User must hit a <CR> to continue (terminal output).
\*----------------------------------------------------------------------*/

void
plD_eop_tek(PLStream *pls)
{
    if (pls->termin) {
	putchar(BEL);
	fflush(stdout);
	WaitForPage(pls);
    }
    fprintf(pls->OutFile, "%c%c", ESC, FF);
}

/*----------------------------------------------------------------------*\
* plD_bop_tek()
*
* Set up for the next page.
* Advance to next family file if necessary (file output).
\*----------------------------------------------------------------------*/

void
plD_bop_tek(PLStream *pls)
{
    PLDev *dev = (PLDev *) pls->dev;

    dev->xold = UNDEFINED;
    dev->yold = UNDEFINED;

    if ( ! pls->termin)
	plGetFam(pls);

    pls->page++;
}

/*----------------------------------------------------------------------*\
* plD_tidy_tek()
*
* Close graphics file or otherwise clean up.
\*----------------------------------------------------------------------*/

void
plD_tidy_tek(PLStream *pls)
{
    if ( ! pls->termin) {
	fclose(pls->OutFile);
    }
    else {
	tek_graph(pls);
	tek_text(pls);
	fflush(pls->OutFile);
    }
    pls->fileset = 0;
    pls->page = 0;
    pls->OutFile = NULL;
}

/*----------------------------------------------------------------------*\
* plD_state_tek()
*
* Handle change in PLStream state (color, pen width, fill attribute, etc).
\*----------------------------------------------------------------------*/

void 
plD_state_tek(PLStream *pls, PLINT op)
{
    switch (op) {

    case PLSTATE_WIDTH:
	break;

    case PLSTATE_COLOR0:
	if (pls->color) {
	    if (pls->icol0 != PL_RGB_COLOR)
		fprintf(pls->OutFile, "%cML%c", ESC, pls->icol0 + 48);
	}
	break;

    case PLSTATE_COLOR1:
	break;
    }
}

/*----------------------------------------------------------------------*\
* plD_esc_tek()
*
* Escape function.
\*----------------------------------------------------------------------*/

void 
plD_esc_tek(PLStream *pls, PLINT op, void *ptr)
{
    switch (op) {

      case PLESC_TEXT:
	tek_text(pls);
	break;

      case PLESC_GRAPH:
	tek_graph(pls);
	break;
    }
}

/*----------------------------------------------------------------------*\
* tek_text()
*
* Switch to text screen (or alpha mode, for vanilla tek's).
\*----------------------------------------------------------------------*/

static void 
tek_text(PLStream *pls)
{
    if (pls->termin && (pls->graphx == GRAPHICS_MODE)) {
	pls->graphx = TEXT_MODE;
	if (pls->dev_dual) {
	    printf("%c%c", US, CAN);
	    printf("%c%c", ESC, ETX);
	    printf("%c%c", US, CAN);
	}
	else
	    printf("%c", US);
    }
}

/*----------------------------------------------------------------------*\
* tek_graph()
*
* Switch to graphics screen (or vector mode, for vanilla tek's).
\*----------------------------------------------------------------------*/

static void 
tek_graph(PLStream *pls)
{
    if (pls->termin && (pls->graphx == TEXT_MODE)) {
	pls->graphx = GRAPHICS_MODE;
	if (pls->dev_dual) 
	    printf("%c[?38h", ESC);
	else
	    printf("%c", GS);
    }
}

/*----------------------------------------------------------------------*\
* WaitForPage()
*
* This routine waits for the user to advance the plot, while handling
* all other events.
\*----------------------------------------------------------------------*/

static void
WaitForPage(PLStream *pls)
{
    int input_char;

    if (pls->nopause)
	return;

    while ( ! exit_eventloop) {
	input_char = getchar();
	EventHandler(pls, input_char);
    }
    exit_eventloop = FALSE;
}

/*----------------------------------------------------------------------*\
* EventHandler()
*
* Event handler routine for xterm.  
* Just reacts to keyboard input.
\*----------------------------------------------------------------------*/

static void
EventHandler(PLStream *pls, int input_char)
{
    PLKey key;

    key.code = 0;
    key.string[0] = '\0';

/* Translate keystroke into a PLKey */

    if ( ! isprint(input_char)) {
	switch (input_char) {
	case 0x0A:
	    key.code = PLK_Linefeed;
	    break;

	case 0x0D:
	    key.code = PLK_Return;
	    break;

	case 0x08:
	    key.code = PLK_BackSpace;
	    break;

	case 0x7F:
	    key.code = PLK_Delete;
	    break;
	}
    }
    else {
	key.string[0] = input_char;
	key.string[1] = '\0';
    }

#ifdef DEBUG
    pltext();
    fprintf(stderr, "Input char %x, Keycode %x, string: %s\n",
	    input_char, key.code, key.string);
    plgra();
#endif

/* Call user event handler */
/* Since this is called first, the user can disable all plplot internal
   event handling by setting key.code to 0 and key.string to '\0' */

    if (pls->KeyEH != NULL)
	(*pls->KeyEH) (&key, pls->KeyEH_data, &exit_eventloop);

/* Handle internal events */

/* Advance to next page (i.e. terminate event loop) on a <eol> */

    if (key.code == PLK_Linefeed)
	exit_eventloop = TRUE;

/* Terminate on a 'Q' (not 'q', since it's too easy to hit by mistake) */

    if (key.string[0] == 'Q') {
	pls->nopause = TRUE;
	plexit("");
    }
}

/*----------------------------------------------------------------------*\
* tty cbreak-mode handlers
*
* Taken from "Advanced Programming in the UNIX(R) Environment", 
* by W. Richard Stevens.
\*----------------------------------------------------------------------*/

#ifdef POSIX_TTY

int
tty_cbreak(int fd)			/* put terminal into a cbreak mode */
{
    struct termios buf;

    if (tcgetattr(fd, &save_termios) < 0)
	return -1;

    buf = save_termios;			/* structure copy */

    buf.c_lflag &= ~(ECHO | ICANON);	/* echo & canonical mode off */
    buf.c_cc[VMIN] = 1;			/* 1 byte at a time */
    buf.c_cc[VTIME] = 0;		/* no timer */

    if (tcsetattr(fd, TCSAFLUSH, &buf) < 0)
	return -1;

    ttystate = CBREAK;
    ttysavefd = fd;
    return 0;
}

int
tty_reset(int fd)			/* restore terminal's mode */
{
    if (ttystate != CBREAK && ttystate != RAW)
	return 0;

    if (tcsetattr(fd, TCSAFLUSH, &save_termios) < 0)
	return -1;

    ttystate = RESET;
    return 0;
}

void
tty_atexit(void)			/* exit handler */
{
    if (ttysavefd >=0)
	tty_reset(ttysavefd);
}

#endif		/* POSIX_TTY */

#else
int pldummy_tek() {return 0;}

#endif	/*  defined(XTERM) || defined(TEK4010) || defined(TEK4107) */
