/* $Id$
 * $Log$
 * Revision 1.21  1994/03/23 06:50:49  mjl
 * Added new drivers: Versaterm (Mac), and VLT (Amiga).  Tek4107 driver
 * improved to actually work on a real tek 4107 (contributed by Paul
 * Kirschner), and the driver keyword changed to "tek4107t" or "tek4107f"
 * (was "t4107t" or "t4107f").  Introduced idea of minor devices -- each
 * variation on the tektronix instruction set is a new minor device.
 *
 * Added support for: color map 1 color selection, color map 0 or color map 1
 * state change (palette change), polygon fills, both solid and pattern (use
 * negative fill pattern number to get hardware pattern).  Color map now set
 * FROM PLPLOT!!  May require some additional effort to work for all minor
 * devices.
 *
 * All drivers: cleaned up by eliminating extraneous includes (stdio.h and
 * stdlib.h now included automatically by plplotP.h), extraneous clears
 * of pls->fileset, pls->page, and pls->OutFile = NULL (now handled in
 * driver interface or driver initialization as appropriate).  Special
 * handling for malloc includes eliminated (no longer needed) and malloc
 * prototypes fixed as necessary.
 *
 * Revision 1.20  1993/12/15  08:53:23  mjl
 * Fixed mskermit driver initialization.
 *
 * Revision 1.19  1993/12/08  20:25:34  mjl
 * Added changes for MS-Kermit emulator (submitted by Paul Kirschner).
 *
 * Revision 1.18  1993/12/08  06:14:43  mjl
 * Now send an initial page clear (tek devices only) on a beginning of page.
 * This helps when there is only one graphics/alpha screen, otherwise is
 * harmless.
 *
 * Revision 1.17  1993/12/06  07:41:47  mjl
 * Changed to not turn off echo when changing tty settings.
 *
 * Revision 1.16  1993/09/14  22:25:15  mjl
 * Moved define of POSIX_TTY to plplotP.h since the SX-3 isn't POSIX-compliant.
 *
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
*/

/*	tek.c

	PLPLOT tektronix device & emulators driver.
*/
#if defined(XTERM) ||		/* xterm */ \
    defined(TEK4010) ||		/* TEK 4010 */ \
    defined(TEK4107) ||		/* TEK 4107 */ \
    defined(MSKERMIT) ||	/* MS-kermit emulator */ \
    defined(VERSATERM) ||	/* Versaterm emulator */ \
    defined(VLT)		/* VLT emulator */

#include "plplotP.h"
#include "drivers.h"
#include "plevent.h"

#include <ctype.h>

/* Static function prototypes */
/* INDENT OFF */

static void	WaitForPage	(PLStream *pls);
static void	EventHandler	(PLStream *pls, int input_char);
static void	tek_init	(PLStream *pls);
static void	tek_text	(PLStream *pls);
static void	tek_graph	(PLStream *pls);
static void	fill_polygon	(PLStream *pls);
static void	encode_int	(char *c, int i);
static void	encode_vector	(char *c, int x, int y);
static void	tek_vector	(PLStream *pls, int x, int y);
static void	scolor		(int icol, int r, int g, int b);
static void	setcmap		(PLStream *pls);

/* Stuff for handling tty cbreak mode */

#ifdef POSIX_TTY

#include <termios.h>
#include <unistd.h>

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

enum {tek4010, tek4105, tek4107, xterm, mskermit, vlt, versaterm};

static exit_eventloop = 0;
static int curcolor;

/* color for MS-DOS Kermit v2.31 (and up) tektronix emulator 
 *	0 = normal, 1 = bright 
 *	foreground color (30-37) = 30 + colors
 *		where colors are   1=red, 2=green, 4=blue 
 */
static char *kermit_color[15]= {
    "\033[0;30m","\033[0;37m",
    "\033[0;32m","\033[0;36m","\033[0;31m","\033[0;35m",
    "\033[1;34m","\033[1;33m","\033[1;31m","\033[1;37m",
    "\033[1;35m","\033[1;32m","\033[1;36m","\033[0;34m",
    "\033[0;33m"};

/* INDENT ON */
/*----------------------------------------------------------------------*\
* plD_init_xterm()	xterm 
* plD_init_tekt()	Tek 4010 terminal
* plD_init_tekf()	Tek 4010 file
* plD_init_tek4107t()	Tek 4105/4107 terminal
* plD_init_tek4107f()	Tek 4105/4107 file
* plD_init_mskermit()	MS-Kermit emulator (DOS)
* plD_init_vlt()	VLT emulator (Amiga)
* plD_init_versaterm()	VersaTerm emulator (Mac)
*
* Initialize device.  These just set attributes for the particular tektronix
* device, then call tek_init().  The following attributes can be set:
*
* pls->termin		if a terminal device
* pls->color		if color
*
\*----------------------------------------------------------------------*/

void 
plD_init_xterm(PLStream *pls)
{
    pls->dev_minor = xterm;
    pls->termin = 1;
    tek_init(pls);
}

void
plD_init_tekt(PLStream *pls)
{
    pls->termin = 1;
    plD_init_tekf(pls);
}

void
plD_init_tekf(PLStream *pls)
{
    pls->dev_minor = tek4010;
    tek_init(pls);
}

void
plD_init_tek4107t(PLStream *pls)
{
    pls->termin = 1;
    plD_init_tek4107f(pls);
}

void
plD_init_tek4107f(PLStream *pls)
{
    pls->dev_minor = tek4107;
    pls->color = 1;
    pls->dev_fill0 = 1;
    tek_init(pls);
}

void
plD_init_mskermit(PLStream *pls)
{
    pls->dev_minor = mskermit;
    pls->termin = 1;
    pls->color = 1;
    pls->dev_fill0 = 1;
    tek_init(pls);
}

void
plD_init_vlt(PLStream *pls)
{
    pls->dev_minor = vlt;
    pls->termin = 1;
    pls->color = 1;
    pls->dev_fill0 = 1;
    tek_init(pls);
}

void
plD_init_versaterm(PLStream *pls)
{
    pls->dev_minor = versaterm;
    pls->termin = 1;
    pls->color = 1;
    pls->dev_fill0 = 1;
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
    pls->graphx = TEXT_MODE;

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

    switch (pls->dev_minor) {
    case tek4107:
	pls->graphx = GRAPHICS_MODE;
	fprintf(pls->OutFile, "%c%%!0", ESC);    /* set tek mode */
	fprintf(pls->OutFile, "%cKN1", ESC);     /* clear the view */
	fprintf(pls->OutFile, "%cLZ", ESC);      /* clear dialog buffer */
	fprintf(pls->OutFile, "%cML1", ESC);     /* set default color */
	break;

    default:
	tek_graph(pls);
	fprintf(pls->OutFile, "%c", GS);		/* Enter vector mode */
	if (pls->termin)
	    fprintf(pls->OutFile, "%c%c", ESC, FF);	/* erase and home */
    }

/* Initialize palette */

    if (pls->color) {
	printf("%cTM111", ESC);		/* Switch to RGB colors */
	setcmap(pls);
    }
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
    int x1 = x1a >> 4, y1 = y1a >> 4, x2 = x2a >> 4, y2 = y2a >> 4;
    int hy, ly, hx, lx;

/* If continuation of previous line just send new point */

    if (x1 != dev->xold || y1 != dev->yold) {
	fprintf(pls->OutFile, "%c", GS);
	pls->bytecnt++;
	tek_vector(pls, x1, y1);
    }
    tek_vector(pls, x2, y2);

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
* Devices that share graphics/alpha screens need a page clear.
\*----------------------------------------------------------------------*/

void
plD_bop_tek(PLStream *pls)
{
    PLDev *dev = (PLDev *) pls->dev;

    dev->xold = UNDEFINED;
    dev->yold = UNDEFINED;

    if (pls->termin) {
	switch (pls->dev_minor) {
	case mskermit:
	    fprintf(pls->OutFile, "%c%c", ESC, FF);
	    break;
	}
    }
    else
	plGetFam(pls);

    pls->page++;

/* Initialize palette */

    if (pls->color) 
	setcmap(pls);
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
	tek_text(pls);
	fflush(pls->OutFile);
    }
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
	    int icol0 = pls->icol0;
	    if (icol0 != PL_RGB_COLOR) {
		curcolor = icol0;
		switch (pls->dev_minor) {
		case mskermit:
		    fprintf(pls->OutFile, "%s", kermit_color[icol0 % 14] );
		    break;

		default:
		    fprintf(pls->OutFile, "%cML%c", ESC, icol0 + 48);
		}
	    }
	}
	break;

    case PLSTATE_COLOR1:
	if (pls->color) {
	    int icol1, ncol1;
	    if ((ncol1 = MIN(16 - pls->ncol0, pls->ncol1)) < 1)
		break;

	    icol1 = pls->ncol0 + (pls->icol1 * (ncol1-1)) / (pls->ncol1-1);
	    curcolor = icol1;
	    fprintf(pls->OutFile, "%cML%c", ESC, icol1 + 48);
	}
	break;

    case PLSTATE_CMAP0:
    case PLSTATE_CMAP1:
	if (pls->color)
	    setcmap(pls);
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

    case PLESC_FILL:
	fill_polygon(pls);
	break;
    }
}

/*----------------------------------------------------------------------*\
* fill_polygon()
*
* Fill polygon described in points pls->dev_x[] and pls->dev_y[].
* Only solid color fill supported.
\*----------------------------------------------------------------------*/

static void
fill_polygon(PLStream *pls)
{
    int i;
    char fillcol[4], firstpoint[5];

    if (pls->dev_npts < 1)
	return;

    encode_int(fillcol, -curcolor);
    encode_vector(firstpoint, pls->dev_x[0]>>4, pls->dev_y[0]>>4);

/* Select the fill pattern */

    fprintf(pls->OutFile, "%cMP%s", ESC, fillcol);

/* Begin panel boundary */
/* Change %s0 to %s1 to see the boundary of each fill box -- cool! */

    fprintf(pls->OutFile, "%cLP%s0", ESC, firstpoint);

/* Specify boundary (use vector mode) */

    fprintf(pls->OutFile, "%c", GS);
    pls->bytecnt++;
    for (i = 1; i < pls->dev_npts; i++) {
	tek_vector(pls, pls->dev_x[i]>>4, pls->dev_y[i]>>4);
    }

/* End panel */

    fprintf(pls->OutFile, "%cLE", ESC);
}

/*----------------------------------------------------------------------*\
* tek_text()
*
* Switch to text screen (or alpha mode, for vanilla tek's).
*
* Note: xterm behaves strangely in the following circumstance: switch to
* the text screen, print a string, and switch to the graphics screen, all
* done in quick succession.  The first character of the printed string
* usually comes out blank -- but only apparently so, because if you force
* a refresh of the screen in this area it will reappear.  This is a
* reproducible bug on the HP 720 under X11R5.  If you insert a sleep(1)
* after the switch to text screen or before the switch to graphics screen,
* the string is printed correctly.  I've been unable to find a workaround
* for this problem (and I've tried, you can believe eet man).
\*----------------------------------------------------------------------*/

static void 
tek_text(PLStream *pls)
{
    if (pls->termin && (pls->graphx == GRAPHICS_MODE)) {
	pls->graphx = TEXT_MODE;
	switch (pls->dev_minor) {
	case xterm:
	    printf("%c%c", ESC, ETX);		/* switch to vt100 screen */
	    break;

	case mskermit:
	case vlt:
	    printf("%c[?38l", ESC);		/* switch to vt100 screen */
	    break;

	case versaterm:
	    printf("%c%%!2", ESC);		/* switch to vt100 screen */
	    break;

	case tek4107:
	    printf("%cLV1", ESC);		/* set dialog visible */
	    printf("%c%%!1", ESC);		/* set ansi mode */
	    break;

	default:
	    printf("%c", US);			/* enter alpha mode */
	}
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
	switch (pls->dev_minor) {
	case xterm:
	case mskermit:
	case vlt:
	    printf("%c[?38h", ESC);		/* switch to tek screen */
	    break;

	case versaterm:
	    printf("%c%%!0", ESC);		/* switch to tek4107 screen */
	    break;

	case tek4107:
	    printf("%c%%!0", ESC);		/* set tek mode */
	    printf("%c%c", ESC, FF);		/* clear screen */
	    printf("%cLV0", ESC);		/* set dialog invisible */
	    break;

	default:
	    printf("%c", GS);			/* enter vector mode */
	}
    }
}

/*----------------------------------------------------------------------*\
* encode_int()
*
* Encodes a single int into standard tek integer format, storing into a
* NULL-terminated character string (must be length 4 or greater).  This
* scheme does not work for negative integers less than 15.
\*----------------------------------------------------------------------*/

static void
encode_int(char *c, int i)
{
    int negative = 0;

    if (i > 0) {
	if (i & 0x7C00)				/* are any of bits 10-14 set?*/
	    *c++ = ((i >> 10) & 0x1F) | 0x40;
	if (i & 0x03F0)				/* are any of bits 4-9 set? */
	    *c++ = ((i >> 4) & 0x3F) | 0x40;
    }
    else {
	i = -i;
	negative = 1;
    }

    if (i & 0x000F)				/* are any of bits 0-3 set? */
	*c = (i & 0x0F) | 0x20;
    else					/* if not, put in a space */
	*c = 0x20;
	
    if ( ! negative)				/* if positive, set sign bit */
	*c |= 0x10;

    c++; *c = '\0';				/* NULL-terminate */
    return;
}

/*----------------------------------------------------------------------*\
* encode_vector()
*
* Encodes an xy vector (2 ints) into standard tek vector format, storing
* into a NULL-terminated character string of length 5.  Note that the
* y coordinate always comes first.
\*----------------------------------------------------------------------*/

static void
encode_vector(char *c, int x, int y)
{
    int hy, ly, hx, lx;

    c[0] = y / 32 + 32;
    c[1] = y - (y / 32) * 32 + 96;
    c[2] = x / 32 + 32;
    c[3] = x - (x / 32) * 32 + 64;
}

/*----------------------------------------------------------------------*\
* tek_vector()
*
* Issues a vector draw command, assuming we are in vector plot mode.
* XY coordinates are encoded according to the standard xy encoding scheme. 
\*----------------------------------------------------------------------*/

static void
tek_vector(PLStream *pls, int x, int y)
{
    int hy, ly, hx, lx;

    hy = y / 32 + 32;
    ly = y - (y / 32) * 32 + 96;
    hx = x / 32 + 32;
    lx = x - (x / 32) * 32 + 64;
    fprintf(pls->OutFile, "%c%c%c%c", hy, ly, hx, lx);
    pls->bytecnt += 4;
}

/*----------------------------------------------------------------------*\
* scolor()
*
* Sets a color by tek-encoded RGB values.
* Need to convert PLPLOT RGB color range (0 to 255) to Tek RGB color 
* range (0 to 100).
\*----------------------------------------------------------------------*/

static void
scolor(int icol, int r, int g, int b)
{
    char tek_col[4], tek_r[4], tek_g[4], tek_b[4];

    encode_int(tek_col, icol);
    encode_int(tek_r, 100*r/255);
    encode_int(tek_g, 100*g/255);
    encode_int(tek_b, 100*b/255);

    printf("%cTG14%s%s%s%s", ESC, tek_col, tek_r, tek_g, tek_b);
}

/*----------------------------------------------------------------------*\
* setcmap()
*
* Sets up color palette.
\*----------------------------------------------------------------------*/

static void
setcmap(PLStream *pls)
{
    int i, ncol1 = MIN(16 - pls->ncol0, pls->ncol1);
    PLColor cmap1col;

/* Initialize cmap 0 colors */

    for (i = 1; i < pls->ncol0; i++) 
	scolor(i, pls->cmap0[i].r, pls->cmap0[i].g, pls->cmap0[i].b);

/* Initialize any remaining slots for cmap1 */

    for (i = 0; i < ncol1; i++) {
	plcol_interp(pls, &cmap1col, i, ncol1);
	scolor(i + pls->ncol0, cmap1col.r, cmap1col.g, cmap1col.b);
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

    buf.c_lflag &= ~(ICANON);		/* canonical mode off */
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

#endif	/*  defined(XTERM) || ... */
